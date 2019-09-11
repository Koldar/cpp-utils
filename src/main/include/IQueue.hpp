#ifndef _IQUEUE_HEADER__
#define _IQUEUE_HEADER__

#include "imemory.hpp"
#include "ICleanable.hpp"

namespace cpp_utils {

template <typename ITEM>
class IQueue: public IMemorable, public ICleanable {
public:
    virtual bool isEmpty() const = 0;
    virtual void decrease_key(ITEM& val) = 0;

    virtual void pushOrDecreaseKey(ITEM& val) = 0;

    virtual void increase_key(ITEM& val) = 0;

    virtual void push(ITEM& val) = 0;

    virtual ITEM& pop() = 0;

    virtual bool contains(const ITEM& n) const = 0;

    virtual ITEM& peek() const = 0;

	virtual size_t size() const = 0;
};


}

#endif