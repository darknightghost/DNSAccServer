#pragma once

#include <memory>

#include <uv.h>

/**
 * @brief	Libuv event loop.
 *
 * Libuv event loop.
 */
class EventLoop {
    protected:
        /// Normal members
        ::uv_loop_t		m_loop;		//< Event loop.

    protected:
        /// Static members
        ::std::shared_ptr<EventLoop>	_instance;	//< Instance

    protected:
        EventLoop();
        virtual			~EventLoop();

    public:
};
