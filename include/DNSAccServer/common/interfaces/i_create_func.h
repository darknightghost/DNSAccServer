#pragma once

#include <type_traits>

#include <DNSAccServer/common/interfaces/i_initialized.h>
#include <DNSAccServer/common/interfaces/i_this_ptr.h>

/**
 * @brief   Interface to provide a static create method.
 *
 * @tparam  T       Final class which implements this interface.
 * @tparam  Args    Types of arguments of the constructor of the class which
 *                  implements this interface.
 */
template<class T, typename... Args>
class ICreateFunc : virtual public IInitialized<T>, virtual public IThisPtr<T> {
  protected:
    /**
     * @brief     Constructor.
     */
    ICreateFunc();

  public:
    /**
     * @brief     Create object.
     *
     * @param[in] args        Arguments of the constructor.
     */
    static ::std::shared_ptr<T> create(Args &...args);

    /**
     * @brief       Destructor.
     */
    virtual ~ICreateFunc();
};

/**
 * @brief     Constructor.
 */
template<class T, typename... Args>
ICreateFunc<T, Args...>::ICreateFunc()
{
    static_assert(::std::is_base_of<ICreateFunc<T, Args...>, T>::value,
                  "ICreateFunc<T, Args...> is not a base class of T.");
}

/**
 * @brief     Create object.
 */
template<class T, typename... Args>
::std::shared_ptr<T> ICreateFunc<T, Args...>::create(Args &...args)
{
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
        return ret;
    }
}

/**
 * @brief       Destructor.
 */
template<class T, typename... Args>
ICreateFunc<T, Args...>::~ICreateFunc()
{}

#define CREATE_FUNC(T, ...) friend class ICreateFunc<T, ##__VA_ARGS__>;
