#include <chrono>

#include <core/event_loop.h>

::std::shared_ptr<EventLoop>    EventLoop::_instance        = nullptr;

/**
 * @brief       Constructor.
 */
EventLoop::EventLoop() :
    m_running(false),
    m_run(false),
    m_exitCode(0)
{
    ::uv_loop_init(&m_loop);
    ::uv_loop_set_data(&m_loop, this);
}

/**
 * @brief       Destructor.
 */
EventLoop::~EventLoop()
{
    if(m_running) {
        this->exit(0);
    }

    ::uv_loop_close(&m_loop);
}

/**
 * @brief       Get the instance of loop.
 *
 * @return      Pointer to the instance.
 */
::std::shared_ptr<EventLoop> EventLoop::instance()
{
    if(_instance == nullptr) {
        /// Create instance
        _instance = ::std::shared_ptr<EventLoop>(new EventLoop());
    }

    return _instance;
}

/**
 * @brief       Get point to the loop of libuv.
 *
 * @return      Pointer to the loop.
 */
::uv_loop_t* EventLoop::loop()
{
    return &m_loop;
}

/**
 * @brief       Run the loop.
 *
 * @return      Exit code.
 */
int EventLoop::exec()
{
    {
        ::std::lock_guard<::std::mutex> guard(m_lock);

        if(m_running) {
            return -1;
        }

        m_running = true;
        m_run = true;
        m_loopThreadId = ::uv_thread_self();
    }


    while(m_run) {
        ::uv_run(&m_loop, UV_RUN_DEFAULT);

        if(m_run) {
            ::std::this_thread::sleep_for(::std::chrono::milliseconds(1));
        }
    }

    {
        ::std::lock_guard<::std::mutex> guard(m_lock);
        m_running = false;

        /// Remove all callbacks.
        m_exitCallbacks.clear();
        m_freedIds.clear();
    }

    return m_exitCode;
}

/**
 * @brief       Regist a callback which will be called when exec() returns.
 *
 * @param[in]   callback        Callback.
 *
 * @return      ID.
 */
int32_t EventLoop::addExitCallback(::std::function<void()> callback)
{
    ::std::lock_guard<::std::mutex> guard(m_lock);

    if(m_running && m_run) {
        /// Allocate id
        int32_t id;

        if(m_freedIds.size() == 0) {
            id = (int32_t)m_exitCallbacks.size();

        } else {
            auto iter = m_freedIds.begin();
            id = *iter;
            m_freedIds.erase(iter);
        }

        /// Add callback
        m_exitCallbacks[id] = callback;
        return id;

    } else {
        return -1;
    }
}

/**
 * @brief       Remove a callback.
 *
 * @param[in]   id      ID.
 */
void EventLoop::removeExitCallback(int32_t id)
{
    ::std::lock_guard<::std::mutex> guard(m_lock);

    if(m_running && m_run) {
        /// Add callback
        auto iter = m_exitCallbacks.find(id);

        if(iter != m_exitCallbacks.end()) {
            m_exitCallbacks.erase(iter);
            m_freedIds.insert(id);
        }

    }
}

/**
 * @brief       Exit the loop, only loop thread can call it.
 *
 * @param[in]   exitCode        Exit code.
 */
void EventLoop::exit(int exitCode)
{
    ::std::lock_guard<::std::mutex> guard(m_lock);

    if(m_running && m_run) {
        ::std::lock_guard<::std::mutex> guard(m_lock);
        m_exitCode = exitCode;

        /// Call all callbacks.
        for(auto& pairs : m_exitCallbacks) {
            this->asyncCall(pairs.second);
        }

        m_run = false;
    }
}
