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

#include <DNSAccServer/common/interfaces/i_initialized.h>
#include <DNSAccServer/common/interfaces/i_this_ptr.h>

namespace common {

/**
 * @brief   Interface to provide a singleton object.
 *
 * @tparam  T       Final class which implements this interface.
 * @tparam  Args    Types of arguments of the constructor of the class which
 *                  implements this interface.
 */
template<class T, typename... Args>
class ISingleton : virtual public IInitialized<T>, virtual public IThisPtr<T> {
  private:
    static ::std::shared_ptr<T> _instance; ///< Instance.

  protected:
    /**
     * @brief     Constructor.
     */
    ISingleton();

  public:
    /**
     * @brief       Initialize the instance with teh arguments if not
     *              initialized.
     *
     * @param[in]   args    Arguments of the constructor.
     *
     * @return      If the instance is initialized, the method will return the
     *              existing instace. If not, the method will initialize a new
     *              instance and return it if success, otherwise returns \c
     *              nullptr.
     */
    static inline ::std::shared_ptr<T> initialize(Args &...args);

    /**
     * @brief       Get the instance of the object.
     *
     * @return      If initialize, the method will return the initialized
     *              inscance, otherwise returns \c nullptr.
     */
    static inline ::std::shared_ptr<T> instance();

    /**
     * @brief       Destructor.
     */
    virtual ~ISingleton();
};

/**
 * @brief     Constructor.
 */
template<class T, typename... Args>
ISingleton<T, Args...>::ISingleton()
{
    static_assert(::std::is_base_of<ISingleton<T, Args...>, T>::value,
                  "ISingleton<T, Args...> is not a base class of T.");
}

/**
 * @brief     Create object.
 */
template<class T, typename... Args>
::std::shared_ptr<T> ISingleton<T, Args...>::initialize(Args &...args)
{
    if (_instance != nullptr) {
        return _instance;
    }

    // Create object.
    ::std::shared_ptr<T> ret
        = ::std::shared_ptr<T>(new T(::std::forward<Args &...>(args)...));

    // Check result.
    if (ret == nullptr) {
        return ret;

    } else if (! ret->IInitialized<T>::initialized()) {
        return nullptr;

    } else {
        ret->setThisPtr(ret);
        _instance = ret;
        return ret;
    }
}

/**
 * @brief       Get the instance of the object.
 */
template<class T, typename... Args>
::std::shared_ptr<T> ISingleton<T, Args...>::instance()
{
    return _instance;
}

/**
 * @brief       Destructor.
 */
template<class T, typename... Args>
ISingleton<T, Args...>::~ISingleton()
{}

#define SINGLETON_OBJECT(T, ...) \
    friend class ::common::ISingleton<T, ##__VA_ARGS__>;

} // namespace common
