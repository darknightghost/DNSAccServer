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

#include <string>
#include <vector>

namespace common {

/**
 * @brief       Split path.
 *
 * @param[in]   path        Path to split.
 *
 * @return      Splitted path.
 */
::std::vector<::std::string> splitPath(const ::std::string path);

/**
 * @brief       Join path.
 *
 * @param[in]   paths       Paths to join.
 *
 * @return      Path.
 */
::std::string joinPath(const ::std::vector<::std::string> paths);

} // namespace common
