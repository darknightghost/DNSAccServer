#pragma once
#include <string>
#include <chrono>
#include <functional>
#include <core/event_loop.h>

class EventLoop;
class Ping;

/**
 * @brief       Ping callback function type.
 *
 * @param[in]   pingObject      Ping object.
 */
typedef void (*PingCallbackFunc)(
    ::std::shared_ptr<const Ping>   pingObject);

/**
 * @brief       Ping callback. called in loop thread.
 */
typedef ::std::function<void(::std::shared_ptr<const Ping>)>    PingCallback;

/**
 * @brief   Ping ip address.
 */
class Ping {
    protected:
        const ::std::string                 m_address;          ///< Target address.
        ::std::chrono::milliseconds         m_timeout;          ///< Timeout.
        PingCallback                        m_callback;         ///< Callback.
        ::std::chrono::milliseconds         m_latency;          ///< Latency.
        ::std::mutex                        m_lock;             ///< Lock.
        ::std::condition_variable           m_cond;             ///< Finish cond.
        int32_t                             m_exitCallbackID;   ///< Exit callback id;
        volatile bool                       m_finished;         ///< Finished flag.
        ::std::unique_ptr<::std::thread>    m_workThread;       ///< Work thread.
        bool                                m_success;          ///< Ping successed.

    protected:
        /**
         * @brief   Constructor.
         *
         * @param[in]       address         Target IP address.
         * @param[in]       timeout         Timeout.
         * @param[in]       callback        Callback function. See @PingCallback.
         */
        Ping(
            const ::std::string&        address,
            ::std::chrono::milliseconds timeout,
            PingCallback                callback);

        /**
         * @brief       Do ping target.
         *
         * @param[in]   thisPtr     Shared pointer to ping object.
         *
         * @return      On success, true is returned. Othrewise returns false.
         */
        bool        exec(::std::shared_ptr<const Ping>    thisPtr);

    public:
        /**
         * @brief   Ping server.
         *
         * @param[in]       address     Target IP address.
         * @param[in]       timeout     Timeout.
         * @param[in]       callback    Callback function. See @PingCallback.
         *
         * @return      On success, true is returned. Othrewise returns false.
         */
        static bool     ping(
            const ::std::string&        address,
            ::std::chrono::milliseconds timeout,
            PingCallback                callback);

        /**
         * @brief   Get address.
         *
         * @return  Address.
         */
        const ::std::string&            address()   const;

        /**
         * @brief   Get latency.
         *
         * @return  Latency.
         */
        ::std::chrono::milliseconds     latency()   const;

        /**
         * @brief   Get ping status.
         *
         * @return  Success or not.
         */
        bool                            success()   const;

        /**
         * @brief   Destructor.
         */
        virtual     ~Ping();
};
