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
#include <cstring>

#include <DNSAccServer/common/err.h>

namespace common {

/**
 * @brief       Parse errno to string.
 */
::std::string strErrno(int errNum)
{
    char          buffer[512];
    ::std::string ret = ::strerror_r(errNum, buffer, sizeof(buffer));

    return ret;
}

} // namespace common
