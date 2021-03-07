#pragma once
#include <type_traits>

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
    void setInitialized();

  public:
    /**
     * @brief       Get initialize status.
     *
     * @return      Value of initialized flag.
     */
    bool initialized();

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
