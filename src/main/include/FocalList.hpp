#ifndef _CPP_UTILS_FOCAL_LIST_HEADER__
#define _CPP_UTILS_FOCAL_LIST_HEADER__

namespace cpp_utils {

    template <typename ITEM, typename BASE_COMPARATOR, typename FOCUS_COMPARATOR>
    class FocalList : public IQueue<ITEM> {
    private:
    template <typename ITEM, typename BASE_COMPARATOR, typename FOCUS_COMPARATOR>
        typedef FocalList<ITEM, BASE_COMPARATOR, > This;
        typedef IQueue<ITEM> Super;
    private:
        BinaryHeap baseQueue;
        BinaryHeap focalQueue;
    public:
        FocalList(std::function<bool(const ITEM& a, const ITEM& b)> baseComparator, std::function<bool(const ITEM& a, const ITEM& b)> focalComparator): baseQueue{baseComparator}, focalComparator{focalComparator} {

        }
        virtual ~FocalList() {

        }
        FocalList(const This& o): baseComparator{o.}
    public:
        /**
         * @brief Check if the queue has no elements
         * 
         * @return true 
         * @return false 
         */
        virtual bool isEmpty() const = 0;
        virtual void decrease_key(ITEM& val) = 0;

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