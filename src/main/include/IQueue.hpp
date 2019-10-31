#ifndef _IQUEUE_HEADER__
#define _IQUEUE_HEADER__

#include "imemory.hpp"
#include "ICleanable.hpp"

namespace cpp_utils {

template <typename ITEM>
class IQueue: public IMemorable, public ICleanable {
public:
    /**
     * @brief Check if the queue has no elements
     * 
     * @return true 
     * @return false 
     */
    virtual bool isEmpty() const = 0;
    virtual void decrease_key(ITEM& val) = 0;

    virtual void pushOrDecreaseKey(ITEM& val) = 0;

    virtual void increase_key(ITEM& val) = 0;

    /**
     * @brief put the item in the queue.
     * 
     * The element can be put wherever in the queue.
     * It doesn't need to be in according to LIFO, FIFO 
     * or whatever
     * 
     * @post
     *  @li `val` in the queue
     * 
     * @param val the element to be add
     */
    virtual void push(ITEM& val) = 0;

    /**
     * @brief remove from the queue the "best" element
     * 
     * What is the best depends on the actual queue implementation
     * 
     * @return ITEM&  the "best" element so far
     */
    virtual ITEM& pop() = 0;

    virtual bool contains(const ITEM& n) const = 0;

    virtual ITEM& peek() const = 0;

	virtual size_t size() const = 0;
};


}

#endif