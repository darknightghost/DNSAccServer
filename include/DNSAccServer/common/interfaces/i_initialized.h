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

#include <type_traits>

namespace common {

/**
 * @brief   Interface to provide initialized flag.
 *
 * @tparam  T       Final class which implements this interface.
 */
template<class T>
class IInitialized {
  private:
    bool m_initialized; ///< Initialized flag.

  protected:
    /**
     * @brief       Constructor.
     */
    IInitialized();

    /**
     * @brief       Set initialized flag.
     */
    inline void setInitialized();

  public:
    /**
     * @brief       Get initialize status.
     *
     * @return      Value of initialized flag.
     */
    inline bool initialized();

    /**
     * @brief       Destructor.
     */
    virtual ~IInitialized();
};

/**
 * @brief       Constructor.
 */
template<class T>
IInitialized<T>::IInitialized() : m_initialized(false)
{
    static_assert(::std::is_base_of<IInitialized<T>, T>::value,
                  "IInitialized<T> is not a base class of T.");
}

/**
 * @brief       Set initialized flag.
 */
template<class T>
void IInitialized<T>::setInitialized()
{
    m_initialized = true;
}

/**
 * @brief       Get initialize status.
 */
template<class T>
bool IInitialized<T>::initialized()
{
    return m_initialized;
}

/**
 * @brief       Destructor.
 */
template<class T>
IInitialized<T>::~IInitialized()
{}

} // namespace common
