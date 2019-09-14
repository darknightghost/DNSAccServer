#include <chrono>

#include <core/event_loop.h>

::std::shared_ptr<EventLoop>	EventLoop::_instance		= nullptr;

/**
 * @brief		Constructor.
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
 * @brief		Destructor.
 */
EventLoop::~EventLoop()
{
    if(m_running) {
        this->exit(0);
    }

    ::uv_loop_close(&m_loop);
}

/**
 * @brief		Get the instance of loop.
 *
 * @return		Pointer to the instance.
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
 * @brief		Get point to the loop of libuv.
 *
 * @return		Pointer to the loop.
 */
::uv_loop_t* EventLoop::loop()
{
    return &m_loop;
}

/**
 * @brief		Run the loop, not thread-safe.
 *
 * @return		Exit code.
 */
int EventLoop::exec()
{
    m_running = true;
    m_run = true;

    m_loopThreadId = ::uv_thread_self();

    while(m_run) {
        ::uv_run(&m_loop, UV_RUN_DEFAULT);
        ::std::this_thread::sleep_for(::std::chrono::milliseconds(1));
    }

    return m_exitCode;
}

/**
 * @brief		Exit the loop, only loop thread can call it.
 *
 * @param[in]	exitCode		Exit code.
 */
void EventLoop::exit(int exitCode)
{
    m_exitCode = exitCode;
    m_run = false;
    ::uv_stop(&m_loop);
}
