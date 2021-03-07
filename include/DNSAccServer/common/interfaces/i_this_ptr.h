#pragma once

#include <memory>
#include <type_traits>

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
    void setThisPtr(::std::shared_ptr<T> thisPtr);

  public:
    /**
     * @brief       Get the pointer to the object.
     *
     * @return      Pointer.
     */
    ::std::shared_ptr<T> thisPtr();

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
