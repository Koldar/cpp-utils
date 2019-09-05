#ifndef _ICLEANABLE_HEADER__
#define _ICLEANABLE_HEADER__

/**
 * @brief any class implementing this interface will implement the `cleanup` method
 * 
 */
class ICleanable {
    /**
     * @brief set of operation that allows you to reuse the same instance over a different scenario
     * 
     * after calling this method, the object will be in the same state as it was when it was jsut initialized
     * 
     */
    virtual void cleanup() = 0;
};


#endif