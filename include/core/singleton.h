#pragma once

#include <memory>

/**
 * @brief   Singleton interface.
 */
template <class T>
class Singleton {
    protected:
        static ::std::shared_ptr<T>     _instance;  //< Instance.

    public:
        /**
         * @brief       Get instance.
         * if _instance is null, the method wiil call create to create a new
         * instance, otherwise returns the existing instance.
         *
         * @return      Instance.
         */
        static ::std::shared_ptr<T>     instance() {
            if(_instance == nullptr) {
                _instance = T::create();
            }

            return _instance;
        }

        /**
         * @brief   Destructor.
         */
        virtual                         ~Singleton() {};
};

/// Expand T
#define _FORWARD_NAME(T)  T

/// Defination of members of singleton classes.
#define SINGLETON_CREATE(T)   \
    template<> \
    ::std::shared_ptr<T>    Singleton<T>::_instance = nullptr;  /*< Instance*/\
    ::std::shared_ptr<T>    _FORWARD_NAME(T)::create()

/// Declaration of members of singleton classes.
#define SIGNLETON_OBJECT(T) \
    protected: \
    /**
     * @brief  Create an instace of the signleton object.
     *
     * @return New instance.
     */ \
    static ::std::shared_ptr<T>     create(); \
    friend ::std::shared_ptr<T>     Singleton<T>::instance(); \
    private:

