/**
 * @file
 * 
 * @brief A pool of pre-allocated memory specialised for the construction of single structs of a fixed size.
 * 
 * To achieve efficient re-allocation each pre-allocated chunk of memory has associated with it a stack of memory offsets
 * which have been previously freed. This introduces a 12.5% overhead to total memory consumption.
 * 
 * @note
 * this code has been copied from Daniel Harabor Warthog source code and then tweaked. To highlight this, the author name
 * has left unaltered
 * 
 * @author dharabor
 * @date 23/08/2012
 */

#ifndef _POOL_HEADER__
#define _POOL_HEADER__

#include <algorithm>
#include <cassert>
#include <iostream>
#include "imemory.hpp"
#include "ICleanable.hpp"
#include "system.hpp"

namespace cpp_utils {

    const int DEFAULT_CHUNK_SIZE = 1024*256; // 256KB

    namespace internal {

        /**
         * @brief represents a chunk of manager heap memory where we can store several objects of the same type
         * 
         * Each chunk is large depending on how many object `X` can fit in a given area
         * 
         */
        template <typename OBJ>
        class cchunk : public IMemorable, public ICleanable {
        private:
            /**
             * @brief an area on the heap that contains the developer custom data
             * 
             * it points to the first available memory which can be used to store custom data
             */
            char* mem_;
            /**
             * @brief pointer to the next area in ::mem_ the developer can fill with its custom data
             * 
             */
            char* next_;
            /**
             * @brief points to the **last** available memory which can be used to store custom data
             * 
             */
            char* max_;

            /**
             * @brief size in byte of the allocated contiguous area pointed by ::mem_
             * 
             */
            size_t pool_size_; 

            // keep a stack of freed objects
            
            /**
             * @brief positions in mem_ which have been marked as "to be freed"
             * 
             * For example if a cell 3 contains the number 5 it means (assuming stack_size_ set to 10):
             * @li there are 10 objects in this chunk which have been marked as "to be freed";
             * @li the third object which should be freed is stored in the address `mem_ + sizeof(obj)*5`
             * 
             * @note
             * the stack is outside the memory assigned to this class
             */
            int* freed_stack_;
            /**
             * @brief number of objects in this chunk which are marked as "to be freed"
             * 
             * "to be freed" means that we no longer need those objects and hence their data can be safely overwritten.
             * This also represents the last index of ::freed_stack_ which contains semantically meaningful values
             * 
             */
            size_t stack_size_;
        public:
            /**
             * @brief Construct a new cchunk object
             * 
             * @note
             * `pool_size` is only the requested size. There are some cases where the requested size won't be satisfied at all:
             *  fort example when the object is larger than the pool requested or when the saving multiple objects with the same size
             *  will lead to some unused memory (e.g., save int (size 4) and requested 23 bytes: 3 bytes will be left unused).
             * 
             * @param pool_size size of the chunk we want to build
             */
            cchunk(size_t pool_size) : pool_size_{pool_size - (pool_size % sizeof(OBJ))} { // round down
                if(this->pool_size_ < sizeof(OBJ)) {
                    debug("cchunk object size < pool size (", pool_size, "<", sizeof(OBJ), "); setting pool size to object size");
                    this->pool_size_ = sizeof(OBJ);
                }

                this->mem_ = new char[pool_size_];
                this->next_ = mem_;
                this->max_ = mem_ + pool_size_;

                this->freed_stack_ = new int[(pool_size_/sizeof(OBJ))];
                this->stack_size_ = 0;
            }

            virtual ~cchunk() {
                if (this->mem_ != nullptr) {
                    //it can be null if the object has been MOVED in another one
                    delete [] mem_;
                    this->mem_ = nullptr;
                }
                if (this->freed_stack_ != nullptr) {
                    //it can be null if the object has been MOVED in another one
                    delete [] freed_stack_;
                    this->freed_stack_ = nullptr;
                }
            }

            cchunk(const cchunk<OBJ>& other) = delete;
            cchunk<OBJ>& operator =(const cchunk<OBJ>& other) = delete;

            cchunk(cchunk<OBJ>&& other) : mem_{other.mem_}, next_{other.next_}, max_{other.max_}, pool_size_{other.pool_size_}, freed_stack_{other.freed_stack_}, stack_size_{other.stack_size_} {
                other.mem_ = nullptr;
                other.next_ = nullptr;
                other.max_ = nullptr;
                other.freed_stack_ = nullptr;
            }
            cchunk<OBJ>& operator=(cchunk<OBJ>&& other) {
                this->mem_ = other.mem_;
                this->next_ = other.next_;
                this->max_ = other.max_;
                this->pool_size_ = other.pool_size_;
                this->freed_stack_ = other.freed_stack_;
                this->stack_size_ = other.stack_size_;

                other.mem_ = nullptr;
                other.next_ = nullptr;
                other.max_ = nullptr;
                other.freed_stack_ = nullptr;
                return *this;
            }

        public:
            /**
             * @brief tells the system that every object in ::mem_ is now useless
             * 
             * now the developer can now use again the whole chunk as she wishes
             * 
             */
            inline void reclaim() {
                next_ = mem_;
                stack_size_ = 0;
            }

            /**
             * @brief allocate some space for a new object we need to store in this chunk
             * 
             * @return char* a pointer to the address of memory where the object we need to allocate can be positioned
             */
            inline char* allocate() {
                if(next_ < max_) {
                    char* retval = next_;
                    next_ += sizeof(OBJ);
                    return retval;
                }
                //the chunk is completely filled. Check if there are some objects in the chunk which are marked as "to be freed"
                if(stack_size_ > 0) {
                    --stack_size_;
                    //let the allocator put the new object in the heap in the position where the last "to be freed" object was once was
                    return mem_ + freed_stack_[stack_size_];
                }
                //all the chunk is used. return null pointer
                return nullptr;
            }

            /**
             * @brief mark a certain address as "to be freed"
             * 
             * @param addr the address which needs to be marked
             */
            inline void deallocate(char* addr) {
                DO_ON_DEBUG {
                    assert(mem_ >= addr);
                    if((unsigned)(addr-mem_) >= pool_size_) {
                        log_error("err; warthog::mem::cchunk; freeing memory outside range of the chunk at addr: ", &mem_);
                    }
                }

                freed_stack_[stack_size_] = addr - mem_;
                stack_size_++;
            }

            /**
             * @brief check if an address is actually stored in this chunk
             * 
             * @param addr the address to check
             * @return true if the address is infact inside mem_
             * @return false otherwise
             */
            inline bool contains(char* addr) {
                DO_ON_DEBUG {
                    assert(mem_ >= addr);
                }
                return ((unsigned)(addr - mem_) < pool_size_);
            }

            /**
             * @brief get the address which starts the area where the developer can put her own objects
             * 
             * @return char* start address
             */
            inline char* first_addr() const {
                return mem_;
            }

            /**
             * @brief number of bytes which the managed heap area spans
             * 
             * @return size_t 
             */
            inline size_t pool_size() const {
                return pool_size_;
            }

            void print(std::ostream& out) const {
                out << "warthog::mem::cchunk pool_size: "
                    << pool_size_ 
                    << " obj_size: "
                    << sizeof(OBJ)
                    << " freed_stack_ size: "
                    << stack_size_;
            }
        public:
            /**
             * @brief number of bytes this object waste
             * 
             * @return size_t 
             */
            MemoryConsumption getByteMemoryOccupied() const {
                return sizeof(*this)
                    //mem_
                    + sizeof(char) * pool_size_
                    //stack_freed
                    + sizeof(int) * (pool_size_/sizeof(OBJ));
            }
        public:
            void cleanup() {
                this->reclaim();
            }
        };

    }

    template <typename OBJ>
    std::ostream& operator << (std::ostream& out, const internal::cchunk<OBJ>& c) {
        c.print(out);
        return out;
    }

    /**
     * @brief implement the Pool design pattern
     * 
     * the pool is heap-located place where you can allocate and free objects all with the same
     * object type.
     * 
     * The pool itself is composed by several chunks: each chunk is a contiguous space of memory while
     * the pool is just a sequence of such chunks.
     * 
     * The pool will greedily allocate as much space as possible.
     * 
     * @tparam OBJ type of object we want to store in this pool
     */
    template <typename OBJ>
    class cpool: public IMemorable, public ICleanable {
    private:
        /**
         * @brief an array of chunks
         * 
         * each array cell contains a pointer pointing to the chunk area
         */
        internal::cchunk<OBJ>** chunks_;
        /**
         * @brief pointer to the chunk a new object will be stored into
         * 
         */
        internal::cchunk<OBJ>* current_chunk_;

        /**
         * @brief current number of chunks this pool object has
         * 
         */
        size_t num_chunks_;
        /**
         * @brief maximum number of chunks this pool object may have
         * 
         * After filling all the chunk allowed, the pool will double this amount and 
         */
        size_t max_chunks_;
    public:
        /**
         * @brief Construct a new cpool object
         * 
         * The pool will be initially empty
         * 
         * @param max_chunks maximum number of chunks the pool has
         */
        cpool(size_t max_chunks) :
            num_chunks_{0}, max_chunks_{max_chunks} {
            debug("creating cpool with max", this->max_chunks_);
            this->init();
        }

        /**
         * @brief Construct a new cpool object
         * 
         * we will build a pool with at most 20 chunks
         */
        cpool() : num_chunks_{0}, max_chunks_{20} {
            debug("creating cpool with max", 20);
            init();
        }

        virtual ~cpool() {
            warning("I don'tmknow why, but within loops cpool don't actually free memory!");
            //if the pool has been moved, chunks_ is left nullptr, hence we need to do nothing
            if (this->chunks_ != nullptr) {
                //destroy every chunk
                for(size_t i=0; i < num_chunks_; i++) {
                    delete chunks_[i];
                }
                //destroy the array of chunks
                delete [] this->chunks_;
                this->chunks_ = nullptr;
            }
            if (this->chunks_ != nullptr) {
                delete[] this->chunks_;
            }
            debug("destroyed pool!!!! bytes used are", getProcessUsedRAM(getCurrentPID()));
        }
        cpool(const cpool<OBJ>& other) = delete;
        cpool<OBJ>& operator =(const cpool<OBJ>& other) = delete;

        cpool(cpool<OBJ>&& other) : chunks_{other.chunks_}, current_chunk_{other.current_chunk_}, num_chunks_{other.num_chunks_}, max_chunks_{other.max_chunks_} {
            debug("moving cpool with max");
            other.chunks_ = nullptr;
            other.current_chunk_ = nullptr;
        }
        cpool<OBJ>& operator=(cpool<OBJ>&& other) {
            debug("moving cpool with max");
            this->chunks_ = other.chunks_;
            this->current_chunk_ = other.current_chunk_;
            this->num_chunks_ = other.num_chunks_;
            this->max_chunks_ = other.max_chunks_;

            other.chunks_ = nullptr;
            other.current_chunk_ = nullptr;
            return *this;
        }
            
    public:

        /**
         * @brief mark every object inside every chunk as "reusable"
         * 
         * this effectively mean that we are allowed to overwrite every object stored in the pool
         */
        inline void reclaim() {
            debug("reclaiming pool!");
            for(size_t i=0; i < num_chunks_; i++) {
                chunks_[i]->reclaim();
            }
        }

        /**
         * @brief reserve a bit of space in the pool for a new object
         * 
         * @return char* the address where the new object will be put
         */
        inline char* allocate() {
            char* mem_ptr = current_chunk_->allocate();
            //pointer might be nullptr if there is no space left in the chunk
            if (mem_ptr == nullptr) {
                //the current chunk is completely full
                // look for space in an existing chunk
                // NB: linear-time search! increase DEFAULT_CHUNK_SIZE if
                // number of chunks grows too large
                for(unsigned int i=0; i < num_chunks_; i++) {
                    mem_ptr = chunks_[i]->allocate();
                    if(mem_ptr != nullptr) {
                        /* we have found a new chunk which has some space left. Mark that chunk as "current" since maybe
                        * it can contain other objects as well!
                        */
                        current_chunk_ = chunks_[i];
                        return mem_ptr;
                    }
                }

                // not enough space in any existing chunk; make a new chunk
                debug("adding new chunks in the pool!");
                add_chunk(DEFAULT_CHUNK_SIZE);
                current_chunk_ = chunks_[num_chunks_-1];
                mem_ptr = current_chunk_->allocate();
            }
            return mem_ptr;
        }

        /**
         * @brief deallocate the space reserved for an object (basically a free)
         * 
         * @param addr the pointer which needs to be deallocated
         */
        inline void deallocate(char* addr) {
            for(unsigned int i=0; i < num_chunks_; i++) {
                //look for the chunk where the address belong to
                if((unsigned)(addr-chunks_[i]->first_addr()) < chunks_[i]->pool_size()) {
                    chunks_[i]->deallocate(addr);
                    return;
                }
            }
            throw cpp_utils::exceptions::ImpossibleException{"cpool::free trie to free an adfdress not in any chunk!"};
        }

        /**
         * @brief string representation of this pool
         * 
         * @param out the stream where to put the string representation on
         */
        void print(std::ostream& out) const {
            out << "warthog::mem::cpool #chunks: "
                << num_chunks_ 
                <<	" #max_chunks "
                << max_chunks_ 
                << " obj_size: "
                << sizeof(OBJ)
                << std::endl;
            for(unsigned int i = 0; i < num_chunks_; i++) {
                chunks_[i]->print(out);
                out << std::endl;
            }
        }
    public:
        /**
         * @brief number of bytes the whole pool uses in the system memory
         * 
         * @return size_t 
         */
        MemoryConsumption getByteMemoryOccupied() const {
            MemoryConsumption result;
            for(unsigned int i=0; i < num_chunks_; i++) {
                //space each chunk occupies
                result += chunks_[i]->getByteMemoryOccupied();
            }
            //chunk array
            result += sizeof(internal::cchunk<OBJ>*) * max_chunks_;
            //this
            result += sizeof(*this);
            return result;
        }
    public:
        void cleanup() {
            this->reclaim();
        }
    private:
        void init() {
            //initialize array
            this->chunks_ = new internal::cchunk<OBJ>*[this->max_chunks_];
            for(int i = 0; i < (int) this->max_chunks_; i++) {
                this->add_chunk(DEFAULT_CHUNK_SIZE);
            }
            //mark the first chunk as the current one
            debug("checking....");
            this->current_chunk_ = this->chunks_[0];
        }

        void add_chunk(size_t pool_size) {
            if(this->num_chunks_ < this->max_chunks_) {
                //we have space for another chunk. Create it
                debug("num chunks is", this->num_chunks_);
                this->chunks_[num_chunks_] = new internal::cchunk<OBJ>{pool_size};
                debug("chunks[", num_chunks_, "] is", this->chunks_[num_chunks_]);
                num_chunks_++;
            } else {
                /* we have already reached the maximum chunk number allowed
                * but we need to allocate space for another object.
                * Enlarge the max_chunk threshold
                */

                // ENLARGE THE THRESHOLD
                //big_max is the new max_chunk threshold. we need to update the chunk array as well
                size_t big_max = max_chunks_ * 2;
                //CREATE THE NEW CHUNK ARRAY
                internal::cchunk<OBJ>** big_chunks = new internal::cchunk<OBJ>*[big_max];
                for(unsigned int i = 0; i < max_chunks_; i++) {
                    big_chunks[i] = chunks_[i];
                }
                // DELETE PREVIOUS CHUNK ARRAY
                delete [] chunks_;
                chunks_ = big_chunks;
                //UPDATE THE THRESHOLD
                max_chunks_ = big_max;

                // finally; add a new chunk
                chunks_[num_chunks_] = new internal::cchunk<OBJ>(pool_size);
                num_chunks_++;
            }
        }
    };

    template <typename OBJ>
    std::ostream& operator << (std::ostream& out, const cpool<OBJ>& pool) {
        pool.print(out);
        return out;
    }

}


#endif