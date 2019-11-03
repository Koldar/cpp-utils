#ifndef _CPP_UTILS_BOOSTQUEUE_HEADER__
#define _CPP_UTILS_BOOSTQUEUE_HEADER__

#include <boost/heap/d_ary_heap.hpp>
#include <unordered_map>

#include "exceptions.hpp"
#include "IQueue.hpp"

namespace cpp_utils {

    template <typename ITEM, typename COMPARATOR>
    class BoostQueue;

    namespace internal {

        template <typename ITEM, typename COMPARATOR>
        class HeapNode;

    }

    template <typename ITEM>
    struct HeapNodeDataCompare : public std::binary_function<const ITEM&, const ITEM&, bool> {
        bool operator() (const ITEM& a, const ITEM& b) {
            return (a < b);
        }
    };

    namespace internal {

        template <typename ITEM>
        using BinaryHeap = typename boost::heap::d_ary_heap<
            ITEM, 
            boost::heap::arity<2>, 
            boost::heap::mutable_<true>
            //implicitly "<" of HeapNode
        >;

        template <typename ITEM>
        using binary_heap_handle_t = typename BinaryHeap<ITEM>::handle_type;

        /**
         * @brief a memory chunk in the queue
         * 
         * @tparam ITEM_PTR type of item inside. It's always a pointer
         */
        template <typename ITEM, typename COMPARATOR >
        class HeapNode {
            typedef HeapNode<ITEM, COMPARATOR> This;
        public:
            friend class ::cpp_utils::BoostQueue<ITEM, COMPARATOR>;
        private:
            binary_heap_handle_t<This> priority; 
            ITEM* data;
        public:
            friend bool operator <(const This& a, const This& b) {
                //boost queue are max-queue, but I want min-queue
                return COMPARATOR()(*(b.data), *(a.data));
            }
            friend bool operator >(const This& a, const This& b) {
                //boost queue are max-queue, but I want min-queue
                return COMPARATOR()(*(a.data), *(b.data));
            }
        public:
            HeapNode(ITEM* data): data{data}, priority{} {

            }
            virtual ~HeapNode() {
                //we don't own the data, so no deallocating
            }
            HeapNode(const This& o): data{o.data}, priority{o.priority} {

            }
            HeapNode(This&& o): data{o.data}, priority{::std::move(o.priority)} {
                o.data = nullptr;
            }
            This& operator=(const This& o) {
                this->data = o.data;
                this->priority = o.priority;

                return *this;
            }
            This& operator=(This&& o) {
                this->data = o.data;
                this->priority = ::std::move(o.priority);

                o.data = nullptr;
                return *this;
            }
                  
        };

    }

    /**
     * @brief A queue wrapping boost::d_ary_heap
     * 
     * The boost api is really flexible, but sometimes we want a simpler interface.
     * By default the comparator used is the binary function "<" on ITEM.
     * However this can be set by changing the second template parameter of BoostQueue. Example of custom
     * comparator:
     * 
     * @code
     *  struct NewOrder {
     *      bool operator() (const FooBoost& a, const FooBoost& b) {
     *          //reverse the order of them  heap
     *          return (b < a);
     *      }
     *  };
     *  //usage
     *  BoostQueue<FooBoost, NewOrder> q{};
     * @endcode
     * 
     * Note that this queue saves the **pointers** of the data, so to let every work you can't `move` or `clone`
     * the objects in the queue.
     * 
     * No precondition needs to be satisfied for ITEM: this implementation does not need HasPriority
     * implemented at all
     * 
     * @tparam ITEM the item to store. Note that we will always stopre the popinter of ITEM, not ITEMs themselves
     */
    template <typename ITEM, typename COMPARATOR = HeapNodeDataCompare<ITEM> >
    class BoostQueue: public IQueue<ITEM> {
    public:
        using This = BoostQueue<ITEM, COMPARATOR>;
        using Super = IQueue<ITEM>;
        using heap_node_type = typename internal::HeapNode<ITEM, COMPARATOR>;
        using handle_type = typename internal::BinaryHeap<heap_node_type>::handle_type;
    private:
        std::unordered_map<ITEM*, handle_type> itemToHandleHeap;
        internal::BinaryHeap<heap_node_type> queue;
    public:
        explicit BoostQueue(): queue{}, itemToHandleHeap{} {

        }
        virtual ~BoostQueue() {

        }
        BoostQueue(const This& o): queue{o.queue}, itemToHandleHeap{o.itemToHandleHeap} {

        }
        BoostQueue(This&& o): queue{o.queue}, itemToHandleHeap{::std::move(o.itemToHandleHeap)} {
            o.queue.clear();
        }
        This& operator=(const This& o) {
            this->queue = o.queue;
            this->itemToHandleHeap = o.itemToHandleHeap;
            return *this;
        }
        This& operator=(This&& o) {
            this->queue = ::std::move(o.queue);
            this->itemToHandleHeap = ::std::move(o.itemToHandleHeap);
            return *this;
        }
    public:
        virtual bool isEmpty() const {
            return this->queue.size() == 0;
        }
        virtual void decrease_key(ITEM& val) {
            auto handle = this->itemToHandleHeap[&val];
            this->queue.decrease(handle);
        }

        virtual void increase_key(ITEM& val) {
            auto handle = this->itemToHandleHeap[&val];
            this->queue.increase(handle);
        }

        virtual void push(ITEM& val) {
            auto ha = queue.push(heap_node_type{&val});
            (*ha).priority = ha;
            this->itemToHandleHeap.insert(std::make_pair<>(&val, ha));
        }

        virtual ITEM& pop() {
            ITEM& result = this->peek();
            this->queue.pop();
            this->itemToHandleHeap.erase(&result);
            return result;
        }

        virtual bool contains(const ITEM& n) const {
            return this->itemToHandleHeap.find(const_cast<ITEM*>(&n)) != this->itemToHandleHeap.end();
        }

        virtual ITEM& peek() const {
            heap_node_type x = queue.top();
            return *(x.data);
        }

        virtual size_t size() const {
            return this->queue.size();
        }
    public:
        virtual void cleanup() {
            this->queue.clear();
        }
    public:
        virtual MemoryConsumption getByteMemoryOccupied() const {
            throw cpp_utils::exceptions::NotYetImplementedException{__func__};
        }
    };

}

#endif