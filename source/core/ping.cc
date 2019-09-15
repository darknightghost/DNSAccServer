#include <uv.h>
#include <oping.h>

#include <core/ping.h>
#include <core/event_loop.h>

/**
 * @brief   Constructor.
 *
 * @param[in]       address     Target IP address.
 * @param[in]       timeout     Timeout.
 * @param[in]       callback    Callback function. See @PingCallback.
 */
Ping::Ping(
    const ::std::string&        address,
    ::std::chrono::milliseconds timeout,
    PingCallback                callback) :
    m_address(address),
    m_timeout(timeout),
    m_callback(callback),
    m_finished(false)
{
    /// Regist exit callback
    m_exitCallbackID = EventLoop::instance()->addExitCallback(
    [this]()->void {
        ::std::unique_lock<::std::mutex> guard(m_lock);

        if(!m_finished)
        {
            /// Wait for finish.
            m_cond.wait(guard);
        }
    });
}

/**
 * @brief       Do ping target.
 *
 * @param[in]   thisPtr     Shared pointer to ping object.
 *
 * @return      On success, true is returned. Othrewise returns false.
 */
bool Ping::exec(::std::shared_ptr<const Ping>    thisPtr)
{
    if(m_exitCallbackID < 0) {
        return false;
    }

    /// Create thread.
    m_workThread = ::std::unique_ptr<::std::thread>(new ::std::thread(
    [this](::std::shared_ptr<const Ping> thisPtr)->void {
        m_success = false;
        double timeout = (double)(m_timeout.count());

        /// Ping
        ::pingobj* pingObj = ::ping_construct();

        if(pingObj == nullptr)
        {
            goto FAILED_CONSTRUCT;
        }

        if(::ping_setopt(pingObj, PING_OPT_TIMEOUT, &timeout) < 0)
        {
            goto FAILED_SET_TIMEOUT;
        }

        if(::ping_host_add(pingObj, m_address.c_str()) < 0)
        {
            goto FAILED_ADD;
        }

        if(::ping_send(pingObj) < 0)
        {
            goto FAILED_PING;
        }

        /// Get result
        for(::pingobj_iter_t* iter = ::ping_iterator_get(pingObj);
            iter != nullptr;
            iter = ::ping_iterator_next(iter))
        {
            double latency;
            size_t sz = sizeof(latency);

            if(::ping_iterator_get_info(iter, PING_INFO_LATENCY, &latency,
                                        &sz) >= 0) {
                if(latency < 0) {
                    continue;
                }

                m_success = true;
                m_latency = ::std::chrono::milliseconds((unsigned long)latency);
            }
        }
FAILED_PING:
FAILED_ADD:
FAILED_SET_TIMEOUT:
        ::ping_destroy(pingObj);
FAILED_CONSTRUCT:

        EventLoop::instance()->syncCall(m_callback, thisPtr);
        m_finished = true;
        m_cond.notify_all();
        return;
    }, thisPtr));

    m_workThread->detach();

    return true;
}

/**
 * @brief   Destructor.
 */
Ping::~Ping()
{
}

/**
 * @brief   Ping server.
 *
 * @param[in]       address     Target IP address.
 * @param[in]       timeout     Timeout.
 * @param[in]       callback    Callback function. See @PingCallback.
 *
 * @return      On success, true is returned. Othrewise returns false.
 */
bool Ping::ping(
    const ::std::string&        address,
    ::std::chrono::milliseconds timeout,
    PingCallback                callback)
{
    ::std::shared_ptr<Ping> obj(new Ping(address,
                                         timeout,
                                         callback));
    return obj->exec(obj);
}

/**
 * @brief   Get address.
 *
 * @return  Address.
 */
const ::std::string& Ping::address() const
{
    return m_address;
}

/**
 * @brief   Get latency.
 *
 * @return  Latency.
 */
::std::chrono::milliseconds Ping::latency() const
{
    return m_latency;
}

/**
 * @brief   Get ping status.
 *
 * @return  Success or not.
 */
bool Ping::success() const
{
    return m_success;
}
