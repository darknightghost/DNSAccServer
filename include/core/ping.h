#pragma once
#include <string>
#include <chrono>
#include <functional>
#include <core/event_loop.h>

class EventLoop;

/**
 * @brief       Ping callback function type.
 *
 * @param[in]   timeouted       True if the time is out. otherwiose returns
 *                              false.
 * @param[int]  time            Time used;
 */
typedef void (*PingCallbackFunc)(
    bool                        timeouted,
    ::std::chrono::milliseconds time);

/**
 * @brief       Ping callback. called in loop thread.
 */
typedef ::std::function<PingCallbackFunc>       PingCallback;

/**
 * @brief   Ping ip address.
 */
class Ping {
    public:
        /**
         * @brief   Ping server.
         *
         * @param[in]       address     Target IP address.
         * @param[in]       timeout     Timeout.
         * @param[in]       callback    Callback function. See @PingCallback.
         */
        static void     ping(
            const ::std::string&        address,
            ::std::chrono::milliseconds timeout,
            PingCallback                callback);
};
