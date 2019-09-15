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

#if (UV_VERSION_MAJOR < 1 || UV_VERSION_MINOR < 19)
    #error("The version of libuv must be 1.19.0 or above.")

#endif

/**
 * @brief   Libuv event loop.
 *
 * Libuv event loop.
 */
class EventLoop {
    protected:
        /// Types
        template <typename R, typename... Args>
        struct syncCallArgs {
            EventLoop*                      thisPtr;    //< This.
            R                               ret;        //< Return value.
            ::std::function<R(Args...)>     func;       //< Function to call.
            ::std::tuple<Args...>           args;       //< Arguments.
            ::std::mutex                    lock;       //< Lock.
            ::std::condition_variable       cond;       //< Conditional variable to wait.
        };

        template <typename... Args>
        struct syncCallArgs<void, Args...> {
            EventLoop*                      thisPtr;    //< This.
            ::std::function<void(Args...)>  func;       //< Function to call.
            ::std::tuple<Args...>           args;       //< Arguments.
            ::std::mutex                    lock;       //< Lock.
            ::std::condition_variable       cond;       //< Conditional variable to wait.
        };

        template <typename... Args>
        struct asyncCallArgs {
            EventLoop*                      thisPtr;    //< This.
            ::std::function<void(Args...)>  func;       //< Function to call.
            ::std::tuple<Args...>           args;       //< Arguments.
        };

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
        /// Static members
        static ::std::shared_ptr<EventLoop> _instance;          //< Instance

    protected:
        /**
         * @brief       Constructor.
         */
        EventLoop();

        /**
         * @brief       Extend tuple as arguments and run callback.
         */
        template <typename... Args, std::size_t... I>
        void            argumentsExtender(::std::function<void(Args...)>    func,
                                          ::std::tuple<Args...>&            args,
                                          ::std::index_sequence<I...>) {
            func(std::get<I>(std::forward<::std::tuple<Args...>>(args))...);
            return;
        }

        /**
         * @brief       Extend tuple as arguments and run callback.
         */
        template <typename R, typename... Args, std::size_t... I>
        R               argumentsExtender(::std::function<R(Args...)>   func,
                                          ::std::tuple<Args...>&        args,
                                          ::std::index_sequence<I...>) {
            return func(std::get<I>(std::forward<::std::tuple<Args...>>(args))...);
        }

    public:
        /**
         * @brief       Get the instance of loop.
         *
         * @return      Pointer to the instance.
         */
        static ::std::shared_ptr<EventLoop>         instance();

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
         */
        template <typename... Args>
        void                asyncCall(::std::function<void(Args...)> func,
                                      Args... args) {
            /// Prepare arguments
            asyncCallArgs<Args...> *infos = new asyncCallArgs<Args...>;
            infos->thisPtr = this;
            infos->func = func;
            infos->args = ::std::tuple<Args...>(args...);

            /// Prepare for async call
            ::uv_async_t* asyncHandle = new ::uv_async_t;
            ::uv_async_init(&m_loop,
                            asyncHandle,
            [](uv_async_t* handle) -> void {
                /// Get infos
                asyncCallArgs<Args...>* infos
                = (asyncCallArgs<Args...>*)::uv_handle_get_data(
                    (::uv_handle_t*)handle);

                /// Extend and call function
                infos->thisPtr->argumentsExtender(
                    infos->func,
                    infos->args,
                    ::std::make_index_sequence <
                    ::std::tuple_size <
                    ::std::tuple<Args... >>::value > ());

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
         */
        template <typename... Args>
        void                syncCall(::std::function<void(Args...)> func,
                                     Args... args) {
            if(::uv_thread_self() == m_loopThreadId) {
                /// Call the function
                func(args...);
                return;

            } else {
                /// Arguments
                syncCallArgs<void, Args...> infos;
                infos.thisPtr = this;
                infos.func = func;
                infos.args = ::std::tuple<Args...>(args...);

                /// Prepare for async call
                ::uv_async_t* asyncHandle = new ::uv_async_t;
                ::uv_async_init(&m_loop,
                                asyncHandle,
                [](uv_async_t* handle) -> void {
                    /// Get infos
                    syncCallArgs<void, Args...>* infos
                    = (syncCallArgs<void, Args...>*)::uv_handle_get_data(
                        (::uv_handle_t*)handle);

                    /// Extend and call function
                    infos->thisPtr->argumentsExtender(
                        infos->func,
                        infos->args,
                        ::std::make_index_sequence <
                        ::std::tuple_size <
                        ::std::tuple<Args... >>::value > ());

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
         */
        template <typename R, typename... Args>
        typename ::std::enable_if < !std::is_void<R>::value, R >::type
        syncCall(::std::function<R(Args...)> func,
                 Args... args) {
            if(::uv_thread_self() == m_loopThreadId) {
                /// Call trhe function.
                return func(args...);

            } else {
                /// Arguments
                syncCallArgs<R, Args...> infos;
                infos.thisPtr = this;
                infos.func = func;
                infos.args = ::std::tuple<Args...>(args...);

                /// Prepare for async call
                ::uv_async_t* asyncHandle = new ::uv_async_t;
                ::uv_async_init(&m_loop,
                                asyncHandle,
                [](uv_async_t* handle) -> void {
                    /// Get infos
                    syncCallArgs<R, Args...>* infos
                    = (syncCallArgs<R, Args...>*)::uv_handle_get_data(
                        (::uv_handle_t*)handle);

                    /// Extend and call function
                    infos->ret = infos->thisPtr->argumentsExtender(
                        infos->func,
                        infos->args,
                        ::std::make_index_sequence <
                        ::std::tuple_size <
                        ::std::tuple<Args... >>::value > ());

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
                return infos.ret;
            }
        }
};
