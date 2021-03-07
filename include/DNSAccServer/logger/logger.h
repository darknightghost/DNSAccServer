/**
 * Copyright 2021, 王思远 <darknightghost.cn@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <cstdint>

#include <unistd.h>

#include <DNSAccServer/common/interfaces/i_singleton.h>

namespace logger {

/**
 * @brief   Logger.
 */
class Logger final : virtual public ::common::ISingleton<Logger> {
  public:
    /**
     * @brief   Log level.
     */
    enum class LogLevel : uint32_t {
        All     = 0x00000000,
        Trace   = 0x20000000,
        Debug   = 0x40000000,
        Info    = 0x60000000,
        Warning = 0x80000000,
        Error   = 0xA0000000,
        Fatal   = 0xC0000000,
        Off     = 0xFFFFFFFF
    };

  private:
    /**
     * @brief     Constructor.
     */
    Logger(uid_t uid, gid_t gid, const ::std::string &logPath);

  public:
    using ::common::ISingleton<Logger>::initialize;
    using ::common::ISingleton<Logger>::instance;

  public:
    /**
     * @brief   Destructor.
     */
    virtual ~Logger();
};

} // namespace logger
