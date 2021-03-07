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

#include <memory>
#include <type_traits>

namespace common {

/**
 * @brief   Interface to provide method to get the shared_ptr points to current
 *          object.
 *
 * @tparam  T       Final class which implements this interface.
 */
template<class T>
class IThisPtr {
  private:
    ::std::weak_ptr<T> m_thisPtr; ///< Pointer to current object.

  protected:
    /**
     * @brief       Constructor.
     */
    IThisPtr();

    /**
     * @brief       Set pointer value.
     *
     * @param[in]   thisPtr     Pointer to the object.
     */
    inline void setThisPtr(::std::shared_ptr<T> thisPtr);

  public:
    /**
     * @brief       Get the pointer to the object.
     *
     * @return      Pointer.
     */
    inline ::std::shared_ptr<T> thisPtr();

    /**
     * @brief       Destructor.
     */
    virtual ~IThisPtr();
};

/**
 * @brief       Constructor.
 */
template<class T>
IThisPtr<T>::IThisPtr()
{
    static_assert(::std::is_base_of<IThisPtr<T>, T>::value,
                  "IThisPtr<T> is not a base class of T.");
}

/**
 * @brief       Set pointer value.
 */
template<class T>
void IThisPtr<T>::setThisPtr(::std::shared_ptr<T> thisPtr)
{
    m_thisPtr = thisPtr;
}

/**
 * @brief       Get the pointer to the object.
 */
template<class T>
::std::shared_ptr<T> IThisPtr<T>::thisPtr()
{
    return m_thisPtr.lock();
}

/**
 * @brief       Destructor.
 */
template<class T>
IThisPtr<T>::~IThisPtr()
{}

} // namespace common
