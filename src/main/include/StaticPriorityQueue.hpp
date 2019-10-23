#ifndef _PRIORITYQUEUE_HEADER__
#define _PRIORITYQUEUE_HEADER__

#include <type_traits>
#include <cassert>
#include "imemory.hpp"
#include "macros.hpp"
#include "exceptions.hpp"
#include "ICleanable.hpp"
#include "commons.hpp"
#include "IQueue.hpp"

namespace cpp_utils {

typedef unsigned long priority_t;

/**
 * @brief An item which has a priority which can be exploit by the queue to order the elements
 * 
 * The item itself should see this priority as just a random number and nothing more.
 * For sorting the elements in the queue, the item should implement the operator \f$<\f$
 * 
 * The class is not templated with the context type because the interface reprsents that the object has a priority,
 * but it does say who is going to exploit the priority. Hence the `void*` in the methods: everyone can potentially use
 * the same getPriority and setPriority methods.
 */
class HasPriority {
public:
    /**
     * @brief Get the Priority object
     * 
     * @param context the queue whose priority you need to retrieve. Essential if the item belong to multiple queues.
     * @return priority_t thew priority of the object
     */
    virtual priority_t getPriority(const void* context) const = 0;
    /**
     * @brief Set the Priority object
     * 
     * @param context the queue whose priority you need to retrieve. Essential if the item belong to multiple queues.
     * @param p the priority to set
     */
    virtual void setPriority(const void* context, priority_t p) = 0;
};

/**
 * @brief Simple defaut implementation of an object with priority
 * 
 * @tparam T type of the object involved
 */
template <typename T>
class ValueWithPriority: public HasPriority {
public:
    friend bool operator ==(const ValueWithPriority<T>& a, const ValueWithPriority<T>& b) {
        if (&a == &b) {
            return true;
        }
        return a.value == b.value;
    }
    friend bool operator <(const ValueWithPriority<T>& a, const ValueWithPriority<T>& b) {
        return a.value < b.value;
    }
    friend bool operator >(const ValueWithPriority<T>& a, const ValueWithPriority<T>& b) {
        return a.value > b.value;
    }
    friend std::ostream& operator <<(std::ostream& out, const ValueWithPriority<T>& a) {
        out << a.value;
        return out;
    }
private:
    T value;
    priority_t priority;
public:
    ValueWithPriority(const T& value): value{value}, priority{0} {

    }
    ValueWithPriority(const ValueWithPriority<T>& other): value{other.value}, priority{other.value} {

    }
    virtual ~ValueWithPriority() {

    }
    ValueWithPriority<T>& operator=(const ValueWithPriority<T>& other) {
        this->value = other.value;
        this->priority = other.priority;
        return *this;
    }
public:
    priority_t getPriority(const void* context) const {
        return this->priority;
    }
    void setPriority(const void* context, priority_t p) {
        this->priority = p;
    }
};

/**
 * @brief a minimum priority queue based on Daniel Harabor one
 * 
 * The priority queue is implemented by a static heap with maximum size.
 * 
 * The priority of an element represents the index. The heap uses this temporary value to quickly have access to the item in the
 * queue from the item itself.
 * 
 * A min priority queue. Loosely based on an implementation from HOG
 * by Nathan Sturtevant.
 * 
 * The queue itself do **stores** only **pointers**, not actual values
 * 
 * @author: dharabor
 * @created: 09/08/2012
 * 
 * @tpaqram ITEM of a type
 */
template <typename ITEM>
class StaticPriorityQueue : public IQueue<ITEM> {
    private:
        /**
         * @brief true if the queue put as head the item with least priority 
         * 
         * @c false if the queue puts the item with highest priority
         */
		bool minqueue;
        /**
         * @brief number of elements in the queue
         * 
         * It is always less of ::capacity
         */
		unsigned int qsize;
        /**
         * @brief maximum size of @c heap
         * 
         */
        size_t heapCapacity;
        /**
         * @brief an array of pointers representing a contiguous area where @c ITEM s are located
         * This is the actual heap.
         * 
         * Note that the queue do not OWN the data
         * 
         * It is an array of pointers and not of references because a value inside the array can be null
         * 
         */
        ITEM** heap;
    public:
        friend std::ostream& operator << (std::ostream& ss, const StaticPriorityQueue<ITEM>& q) {
            ss << "(size=" << q.size() << ")[";
            for(priority_t i=0; i < q.size(); i++) {
                ss << i <<": " << *q.heap[i];
                ss << ", ";
            }
            ss << "]";
            return ss;
        }
	public:
        /**
         * @brief create a new priority queue
         * 
         * @param capacity the maximum number of items the queue can hold
         * @param minqueue true if you want to have on the head of the queue the minimum item, false if you want as head
         *  of the queue the maximum item instead
         */
		StaticPriorityQueue(size_t capacity, bool minqueue) : minqueue{minqueue}, qsize{0}, heapCapacity{capacity}, heap{nullptr} {
            this->resize(capacity);
        }
		virtual ~StaticPriorityQueue() {
            debug("deleting StaticPriorityQueue!");
            delete[] this->heap;
        }

		 
        /**
         * @brief removes all elements from the pqueue
         * 
         */
		void clear() {
            this->qsize = 0;
        }

        /**
         * @brief check if the queue is empty
         * 
         * @return true if the queue has no elements
         * @return false otherwise
         */
        bool isEmpty() const {
            return this->qsize == 0;
        }
		
        /**
         * @brief reprioritise the specified element down
         * 
         * Use it when you know that the item @c val has modified its priority by lowering it.
         * 
         * @code
         * Foo a{5};
         * a.temp -= 6;
         * //we know that "temp" field is involved in the outcome of "<" (priority in the openlist), hence we need to call
         * //decrease_key
         * priorityQueue.decrease_key(a);
         * @endcode
         * 
         * @param val 
         */
		void decrease_key(ITEM& val) {
            assert(val.getPriority(this) < this->qsize);
            if(this->minqueue) {
                heapify_up(val.getPriority(this));
            } else {
                heapify_down(val.getPriority(this));
            }
        }

        void pushOrDecreaseKey(ITEM& val) {
            if (this->contains(val)) {
                return this->decrease_key(val);
            } else{
                this->push(val);
            }
        }

        /**
         * @brief reprioreitize the specified element (up)
         * 
         * Use it when you know that the item @c val has modified its priority by lowering it.
         * 
         * @param val 
         */
		void increase_key(ITEM& val) {
            assert(val.getPriority(this) < this->qsize);
            if(this->minqueue) {
                heapify_down(val.getPriority(this));
            } else {
                heapify_up(val.getPriority(this));
            }
        }

        /**
         * @brief add a new element to the pqueue
         * 
         * @param val the item to add to the queue
         */
		void push(ITEM& val) {
            if(this->contains(val)) {
                return;
            }

            if((this->qsize + 1) > this->heapCapacity) {
                this->resize(2 * this->heapCapacity);
            }
            priority_t priority = this->qsize;
            this->heap[priority] = &val; //since &(ref of obj) == &obj we can safely do this
            val.setPriority(this, priority);
            
            this->qsize += 1;
            this->heapify_up(priority);
        }

        /**
         * @brief remove the top element from the pqueue
         * 
         * @return ITEM* the element which has the "best" priority
         * 
         */
		ITEM& pop() {
            if (this->qsize == 0) {
                throw exceptions::EmptyObjectException<StaticPriorityQueue<ITEM>>{*this};
            }

            ITEM& result = *this->heap[0];
            this->qsize -= 1;

            if(this->qsize > 0) {
                this->heap[0] = this->heap[this->qsize];
                this->heap[0]->setPriority(this, 0);
                heapify_down(0);
            }
            return result;
        }

        /**
         * @brief check if an element is inside the queue
         * 
         * @note
         * The time is O(1)
         * 
         * @param n 
         * @return true true if the element n correspond to the element in the queue having the same priority
         * @return false if the element is not in the queue
         */
		inline bool contains(const ITEM& n) const {
            priority_t priority = n.getPriority(this);
            //if((priority < this->size) && &*n == &*this->heap[priority])
            //&(const ref of obj) == &obj
            if((priority < this->qsize) && (n == (*(this->heap[priority])))) {
				return true;
			}
			return false;
		}

        /**
         * @brief retrieve the top element without removing it
         * 
         * @throw EmptyObjectException if the queue is empty
         * @return PTR* the item on the top of the queue
         */
		inline ITEM& peek() const {
			if (this->qsize > 0) {
				return *this->heap[0];
			}
			throw exceptions::EmptyObjectException<StaticPriorityQueue<ITEM>>{*this};
		}

		inline size_t size() const {
			return this->qsize;
		}

		inline bool isMinqueue() const { 
			return this->minqueue; 
		}
    public:
        void cleanup() {
            this->clear();
        }
    public:
		virtual MemoryConsumption getByteMemoryOccupied() const {
			return sizeof(*this);
		}
    private:
        /**
         * @brief reorders the subpqueue containing elts_[index]
         * 
         * @param index 
         */
		void heapify_up(priority_t index) {
            assert(index < this->qsize);
            while(index > 0) {
                priority_t parent = (index - 1) >> 1;
                if(shouldRotate(*this->heap[parent], *this->heap[index])) {
                    swap(parent, index);
                    index = parent;
                } else {
                    break;
                }
            }
        }
		
        /**
         * @brief reorders the subpqueue under elts_[index]
         * 
         * @param index 
         */
		void heapify_down(priority_t index) {
            priority_t first_leaf_index = this->qsize >> 1;
            while(index < first_leaf_index) {
                // find smallest (or largest, depending on heap type) child
                priority_t child1 = (index << 1) + 1;
                priority_t child2 = (index << 1) + 2;
                priority_t which = child1;
                if ((child2 < this->qsize) && (*(this->heap[child2]) < *(this->heap[child1]))) {
                    which = child2;
                }

                // swap child with parent if necessary
                if(*(this->heap[which]) < *(this->heap[index])) {
                    swap(index, which);
                    index = which;
                } else {
                    break;
                }
            }
        }

        /**
         * @brief allocates more memory so the pqueue can grow
         * 
         * @param capacity the new cpaacity of the queue. Must be greater than the old capacity
         */
		void resize(unsigned int newcapacity) {
            if(newcapacity < this->qsize) {
                throw exceptions::InvalidArgumentException{"queue resize new size < queuesize (%ld < %ld)", newcapacity, this->qsize};
            }

            debug("Resizing StaticPriorityQueue from ", this->heapCapacity, "to", newcapacity);
            ITEM** tmp = new ITEM*[sizeof(ITEM*) * newcapacity];
            if (this->heap != nullptr) {
                memcpy(tmp, this->heap, sizeof(ITEM*) * this->heapCapacity);
                delete[] this->heap;
            }

            this->heap = tmp;
            this->heapCapacity = newcapacity;
        }
	
        /**
         * @brief 
         * 
         * @param first the first element that we might swap
         * @param second the second element that we might swap
         * @return true minqueue is true and the priority of second < first or minqueue is false and the priority of second > first
         * @return false otherwise
         */
		inline bool shouldRotate(const ITEM& first, const ITEM& second) const {
			if(this->minqueue) {
				return second < first;
			} else {
                return first < second;
			}
		}

        /**
         * @brief swap the positions of two nodes in the underlying array
         * 
         * @param index1 first element to swap
         * @param index2 second element to swap
         */
		inline void swap(priority_t index1, priority_t index2) {
			assert(index1 < this->qsize && index2 < this->qsize);

            priority_t p1 = this->heap[index1]->getPriority(this);
            priority_t p2 = this->heap[index2]->getPriority(this);

            //swap positions
            ITEM* tmp = this->heap[index1];
			this->heap[index1] = this->heap[index2];
            this->heap[index2] = tmp;
            //swap priorities: @ index1 we should put the priority of the old index1
            this->heap[index1]->setPriority(this, p1);
            this->heap[index2]->setPriority(this, p2);
		}
};



}

#endif 