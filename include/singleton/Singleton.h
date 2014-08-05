#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <cstdlib>

template< typename T >
class Singleton : public T
{
protected:
    /**
     * @brief Default constructor
     */
    Singleton() : T() {}

    /**
     * @brief Default destructor
     */
    virtual ~Singleton() {}

    /**
     * @brief Copy constructor
     */
    Singleton(const Singleton<T> &) {}

    /**
     * @brief Assignment operator
     */
    Singleton<T> &operator= (const Singleton<T> &) {}

public:
    /**
     * @brief Method for checing if pointer is valid, ie. is there some object
     */
    static inline bool isValid() {return iPtr != NULL;}

    /**
     * @brief Method for validating object, ie. create an object if necessary
     */
    static inline void validate()
    {
        // TODO: add mutex here.
        if (!isValid())
        {
            iPtr = new Singleton<T>();
        }
    }

    /**
     * @brief Method for getting reference to object
     */
    static inline T &getRef()
    {
        return *getPtr();
    }

    /**
     * @brief Method for getting pointer to object
     */
    static inline T*getPtr()
    {
        validate();
        return iPtr;
    }
private:
    /**
     * @brief Singleton object
     */
    static Singleton<T> *iPtr;
};

template< typename T >
Singleton<T> *Singleton< T >::iPtr = NULL;

#endif // _SINGLETON_H_