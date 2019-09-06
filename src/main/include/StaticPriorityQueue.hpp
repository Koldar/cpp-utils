#ifndef _PRIORITYQUEUE_HEADER__
#define _PRIORITYQUEUE_HEADER__

#include <cassert>
#include "imemory.hpp"
#include "macros.hpp"
#include "exceptions.hpp"
#include "ICleanable.hpp"

namespace cpp_utils {

typedef unsigned long priority_t;

/**
 * @brief An item which has a priority which can be exploit by the queue to order the elements
 * 
 */
class HasPriority {
public:
    /**
     * @brief Get the Priority object
     * 
     * @return priority_t thew priority of the object
     */
    virtual priority_t getPriority() const = 0;
    /**
     * @brief Set the Priority object
     * 
     * @param p the priority to set
     */
    virtual void setPriority(priority_t p) = 0;
};

/**
 * @brief a minimum priority queue based on Daniel Harabor one
 * 
 * The priority queue is implemented by a static heap with maximum size.
 * 
 * The priority represents the index
 * 
 * A min priority queue. Loosely based on an implementation from HOG
 * by Nathan Sturtevant.
 * @author: dharabor
 * @created: 09/08/2012
 */
template <typename ITEM, EXTENDS(ITEM, HasPriority)>
class StaticPriorityQueue : public IMemorable, public ICleanable {
    private:
        /**
         * @brief the maximum capacity of the queue
         * 
         * The queue won't be able to store more than the given amount of items
         */
		unsigned int capacity;
        /**
         * @brief true if the queue put as head the item with least priority 
         * 
         * false if the queue puts the item with highest priority
         * 
         */
		bool minqueue;
        /**
         * @brief number of elements in the queue
         * 
         * It is always less of ::capacity
         * 
         */
		unsigned int qsize;
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
            ss << "[queue ";
                for(priority_t i=0; i < q.size(); i++) {
                    ss << *q.heap[i];
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
		StaticPriorityQueue(unsigned int capacity, bool minqueue) : capacity{capacity}, minqueue{minqueue}, qsize{0}, heap{nullptr} {
            this->resize(capacity);
        }
		~StaticPriorityQueue() {
            delete[] this->heap;
        }

		 
        /**
         * @brief removes all elements from the pqueue
         * 
         */
		void clear() {
            for(unsigned int i=0; i < this->qsize; i++) {
                heap[i] = nullptr;
            }
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
         * 
         * 
         * @param val 
         */
		void decrease_key(const ITEM& val) {
            assert(val.getPriority() < this->qsize);
            if(this->minqueue) {
                heapify_up(val.getPriority());
            } else {
                heapify_down(val.getPriority());
            }
        }

        /**
         * @brief reprioreitize the specified element (up)
         * 
         * @param val 
         */
		void increase_key(ITEM& val) {
            assert(val.getPriority() < this->qsize);
            if(this->minqueue) {
                heapify_down(val.getPriority());
            } else {
                heapify_up(val.getPriority());
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

            if((this->qsize + 1) > this->capacity) {
                this->resize(2*this->capacity);
            }
            priority_t priority = this->qsize;
            this->heap[priority] = &val; //since &(ref of obj) == &obj we can safely do this
            val.setPriority(priority);
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

            ITEM* result = this->heap[0];
            this->qsize -= 1;

            if(this->qsize > 0) {
                this->heap[0] = this->heap[this->qsize];
                this->heap[0]->setPriority(0);
                heapify_down(0);
            }
            return *result;
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
			priority_t priority = n.getPriority();
            //if((priority < this->size) && &*n == &*this->heap[priority])
            //&(const ref of obj) == &obj
            if((priority < this->qsize) && &n == this->heap[priority]) {
				return true;
			}
			return false;
		}

        /**
         * @brief retrieve the top element without removing it
         * 
         * @throw EmptyObjectException if the queue is empty
         * @return ITEM* the item on the top of the queue
         */
		inline ITEM& peek() const {
			if (this->qsize > 0) {
				return *this->heap[0];
			}
			throw exceptions::EmptyObjectException<StaticPriorityQueue<ITEM>>{*this};
		}

		inline unsigned int size() const {
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
			return this->capacity * sizeof(ITEM*) + sizeof(*this);
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
                priority_t parent = (index-1) >> 1;
                if(rotate(*this->heap[parent], *this->heap[index])) {
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
                if ((child2 < this->qsize) && (*this->heap[child2] < *this->heap[child1])) {
                    which = child2;
                }

                // swap child with parent if necessary
                if(*this->heap[which] < *this->heap[index]) {
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

            ITEM** tmp = new ITEM*[newcapacity];
            for(unsigned int i=0; i < this->qsize; i++)
            {
                tmp[i] = this->heap[i];
            }
            delete [] this->heap;
            this->heap = tmp;
            this->capacity = newcapacity;
        }
	
        /**
         * @brief 
         * 
         * @param first the first element that we might swap
         * @param second the second element that we might swap
         * @return true minqueue is true and the priority of second < first or minqueue is false and the priority of second > first
         * @return false otherwise
         */
		inline bool rotate(ITEM& first, ITEM& second) {
			if(this->minqueue) {
				if(second < first) {
					return true;
				}
			} else if(first < second) {
				return true;
			}
			return false;
		}

        /**
         * @brief swap the positions of two nodes in the underlying array
         * 
         * @param index1 first element to swap
         * @param index2 second element to swap
         */
		inline void swap(priority_t index1, priority_t index2) {
			assert(index1 < this->qsize && index2 < this->qsize);

			ITEM* tmp = this->heap[index1];
			this->heap[index1] = this->heap[index2];
			this->heap[index1]->setPriority(index1);
			this->heap[index2] = tmp;
			tmp->setPriority(index2);
		}
};



}

#endif 