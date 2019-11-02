#pragma once

#include <cstdint>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>
#include <utility>
#include <tuple>
#include <condition_variable>
#include <type_traits>
#include <map>
#include <set>

#include <uv.h>

#include <core/singleton.h>

#if (UV_VERSION_MAJOR < 1 || UV_VERSION_MINOR < 19)
    #error("The version of libuv must be 1.19.0 or above.")

#endif

class EventLoop;

template <typename R>
struct syncCallArgs {
    EventLoop*                                                  thisPtr;//< This.
    typename ::std::enable_if < !::std::is_void<R>::value, R >  ret;    //< Return value.
    ::std::function<R()>                                        func;   //< Function to call.
    ::std::mutex                                                lock;   //< Lock.
    ::std::condition_variable                                   cond;   //< Conditional variable to wait.
};

template <>
struct syncCallArgs<void> {
    EventLoop*                      thisPtr;    //< This.
    ::std::function<void()>         func;       //< Function to call.
    ::std::mutex                    lock;       //< Lock.
    ::std::condition_variable       cond;       //< Conditional variable to wait.
};

template <typename... Args>
struct asyncCallArgs {
    EventLoop*                      thisPtr;    //< This.
    ::std::function<void()>         func;       //< Function to call.
};

/**
 * @brief   Libuv event loop.
 *
 * Libuv event loop.
 */
class EventLoop : public Singleton<EventLoop> {
        SIGNLETON_OBJECT(EventLoop)
    protected:
        /// Types

    protected:
        /// Normal members
        ::uv_loop_t                                     m_loop;         //< Event loop.
        volatile bool                                   m_running;      //< Running status.
        volatile bool                                   m_run;          //< Run flag.
        volatile int                                    m_exitCode;     //< Exitcode.
        ::uv_thread_t                                   m_loopThreadId; //< Loop thread id.
        ::std::map<int32_t, ::std::function<void()>>    m_exitCallbacks;//< Exit callbacks.
        ::std::set<int32_t>                             m_freedIds;     //< Freed callback ids.
        ::std::mutex                                    m_lock;         //< Flag lock.

    protected:
        /**
         * @brief       Constructor.
         */
        EventLoop();

    public:
        /**
         * @brief       Get point to the loop of libuv.
         *
         * @return      Pointer to the loop.
         */
        ::uv_loop_t*                                loop();

        /**
         * @brief       Run the loop.
         *
         * @return      Exit code.
         */
        int                                         exec();

        /**
         * @brief       Regist a callback to cleanup when someone called exit.
         *
         * @param[in]   callback        Callback.
         *
         * @return      ID.
         */
        int32_t                                     addExitCallback(::std::function<void()> callback);

        /**
         * @brief       Remove a callback.
         *
         * @param[in]   id      ID.
         */
        void                                        removeExitCallback(int32_t id);

        /**
         * @brief       Exit the loop, only loop thread can call it.
         *
         * @param[in]   exitCode        Exit code.
         */
        void                                        exit(int exitCode);

        /**
         * @brief       Destructor.
         */
        virtual         ~EventLoop();

        /**
         * @brief       Call the callback in the loop.
         *
         * @param[in]   func        Function to call.
         * @param[in]   args        Arguments.
         */
        template <typename... Args>
        void                asyncCall(::std::function<void(Args...)> func,
                                      Args... args) {
            /// Prepare arguments
            asyncCallArgs<Args...> *infos = new asyncCallArgs<Args...>;
            infos->thisPtr = this;
            infos->func = ::std::bind(func, args...);

            /// Prepare for async call
            ::uv_async_t* asyncHandle = new ::uv_async_t;
            ::uv_async_init(&m_loop,
                            asyncHandle,
            [](uv_async_t* handle) -> void {
                /// Get infos
                asyncCallArgs<Args...>* infos
                = (asyncCallArgs<Args...>*)::uv_handle_get_data(
                    (::uv_handle_t*)handle);

                /// Call function
                infos->func();

                /// Close and free handle
                delete infos;
                ::uv_close((uv_handle_t*)handle,
                [](uv_handle_t* handle)->void{
                    delete(::uv_async_t*)handle;
                });
            });
            ::uv_handle_set_data((::uv_handle_t*)asyncHandle,
                                 &infos);

            /// Call
            ::uv_async_send(asyncHandle);
        }

        /**
         * @brief       Run callback in the loop Synchronically
         *
         * @param[in]   func        Function to call.
         * @param[in]   args        Arguments.
         */
        template <typename... Args>
        void                syncCall(::std::function<void(Args...)> func,
                                     Args... args) {
            if(::uv_thread_self() == m_loopThreadId) {
                /// Call the function
                ::std::bind(func, args...)();
                return;

            } else {
                /// Arguments
                syncCallArgs<void> infos;
                infos.thisPtr = this;
                infos.func = ::std::bind(func, args...);

                /// Prepare for async call
                ::uv_async_t* asyncHandle = new ::uv_async_t;
                ::uv_async_init(&m_loop,
                                asyncHandle,
                [](uv_async_t* handle) -> void {
                    /// Get infos
                    syncCallArgs<void>* infos
                    = (syncCallArgs<void>*)::uv_handle_get_data(
                        (::uv_handle_t*)handle);

                    /// Extend and call function
                    infos->func();

                    /// Tell caller the function hasn benn returned.
                    infos->cond.notify_all();

                    /// Close and free handle
                    ::uv_close((uv_handle_t*)handle,
                    [](uv_handle_t* handle)->void{
                        delete(::uv_async_t*)handle;
                    });
                    return;
                });
                ::uv_handle_set_data((::uv_handle_t*)asyncHandle,
                                     &infos);

                /// Call and Wait for result
                {
                    ::std::unique_lock<std::mutex> lock(infos.lock);
                    ::uv_async_send(asyncHandle);
                    infos.cond.wait(lock);
                }
                return;
            }
        }

        /**
         * @brief       Run callback in the loop Synchronically
         *
         * @param[in]   func        Function to call.
         * @param[in]   args        Arguments.
         *
         * @return      The return value of func.
         */
        template <typename R, typename... Args>
        typename ::std::enable_if < !std::is_void<R>::value, R >::type
        syncCall(::std::function<R(Args...)> func,
                 Args... args) {
            if(::uv_thread_self() == m_loopThreadId) {
                /// Call the function.
                return ::std::bind(func, args...)();

            } else {
                /// Arguments
                syncCallArgs<R> infos;
                infos.thisPtr = this;
                infos.func = ::std::bind(func, args...);

                /// Prepare for async call
                ::uv_async_t* asyncHandle = new ::uv_async_t;
                ::uv_async_init(&m_loop,
                                asyncHandle,
                [](uv_async_t* handle) -> void {
                    /// Get infos
                    syncCallArgs<R>* infos
                    = (syncCallArgs<R>*)::uv_handle_get_data(
                        (::uv_handle_t*)handle);

                    /// Extend and call function
                    infos->ret = infos->func();

                    /// Tell caller the function has been returned.
                    infos->cond.notify_all();

                    /// Close and free handle
                    ::uv_close((uv_handle_t*)handle,
                    [](uv_handle_t* handle)->void{
                        delete(::uv_async_t*)handle;
                    });
                    return;
                });
                ::uv_handle_set_data((::uv_handle_t*)asyncHandle,
                                     &infos);

                /// Call and Wait for result
                {
                    ::std::unique_lock<std::mutex> lock(infos.lock);
                    ::uv_async_send(asyncHandle);
                    infos.cond.wait(lock);
                }
                return infos.ret;
            }
        }
};
