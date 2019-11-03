#ifndef _CPP_UTILS_IQUEUE_HEADER__
#define _CPP_UTILS_IQUEUE_HEADER__

#include "imemory.hpp"
#include "ICleanable.hpp"

namespace cpp_utils {

    typedef unsigned long priority_t;

    /**
     * @brief An item which has a priority which can be exploit by the queue to order the elements
     * 
     * The item itself should see this priority as just random data and nothing more.
     * For sorting the elements in the queue, the item should implement the operator \f$<\f$
     * 
     * The class is not templated with the context type because the interface reprsents that the object has a priority,
     * but it does say who is going to exploit the priority. Hence the `void*` in the methods: everyone can potentially use
     * the same getPriority and setPriority methods.
     * 
     * @tparam the type a queue implementation uses to sort elements. Implementation dependent
     */
    template <typename PRIORITY>
    class HasPriority {
    public:
        /**
         * @brief Get the Priority object
         * 
         * @param context the queue whose priority you need to retrieve. Essential if the item belong to multiple queues.
         * @return priority_t thew priority of the object
         */
        virtual PRIORITY getPriority(const void* context) const = 0;
        /**
         * @brief Set the Priority object
         * 
         * The method is called when:
         *  - a new element is added in a context;
         *  - an old element is swapped with another one in a context;
         * 
         * @param context the queue whose priority you need to retrieve. Essential if the item belong to multiple queues.
         * @param p the priority to set
         */
        virtual void setPriority(const void* context, PRIORITY p) = 0;
    };

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

        virtual void pushOrDecreaseKey(ITEM& val) {
            if (this->contains(val)) {
                this->decrease_key(val);
            } else{
                this->push(val);
            }
        }

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

        /**
         * @brief Check if a particular item is stored in the queue
         * 
         * @note
         * This operation is not an operation from heaps, but sometimes (e.g., A\*) we require
         * it.
         * 
         * @param n the item we want to check if it's inside this queue
         * @return true if the element is in the queue, 
         * @return false otherwise
         */
        virtual bool contains(const ITEM& n) const = 0;

        /**
         * @brief retrieve the "best" element from the queue, but does not remove it from it
         * 
         * @return ITEM& the top item
         */
        virtual ITEM& peek() const = 0;

        /**
         * @brief number of elements in the queue
         * 
         * @return size_t 
         */
        virtual size_t size() const = 0;
    };


}

#endif