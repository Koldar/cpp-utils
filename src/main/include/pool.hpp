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
class cchunk {
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
    cchunk(size_t pool_size) :
        pool_size_{pool_size - (pool_size % sizeof(OBJ))} { // round down
        if(pool_size_ < sizeof(OBJ)) {
            critical("cchunk object size < pool size (", pool_size, "<", sizeof(OBJ), "); setting pool size to object size");
            pool_size_ = sizeof(OBJ);
        }

        mem_ = new char[pool_size_];
        next_ = mem_;
        max_ = mem_ + pool_size_;

        freed_stack_ = new int[(pool_size_/sizeof(OBJ))];
        stack_size_ = 0;
    }

    ~cchunk() {
        delete [] mem_;
        delete [] freed_stack_;
    }

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
                error("err; warthog::mem::cchunk; freeing memory outside range of the chunk at addr: ", &mem);
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
    
    /**
     * @brief number of bytes this object waste
     * 
     * @return size_t 
     */
    inline size_t mem() const {
        //this
        size_t bytes = sizeof(*this);
        //mem_
        bytes += sizeof(char)*pool_size_;
        //stack_freed
        bytes += sizeof(int)*(pool_size_/sizeof(OBJ));
        return bytes;
    }

    void print(std::ostream& out) const {
        out << "warthog::mem::cchunk pool_size: "
            << pool_size_ 
            << " obj_size: "
            << sizeof(OBJ)
            << " freed_stack_ size: "
            << stack_size_;
    }


};

template <typename OBJ>
std::ostream& operator << (std::ostream& out, const cchunk& c) {
    c.print(out);
    return out;
}

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
class cpool {
private:
    /**
     * @brief an array of chunks
     * 
     * each array cell contains a pointer pointing to the chunk area
     */
    internal::cchunk** chunks_;
    /**
     * @brief pointer to the chunk a new object will be stored into
     * 
     */
    internal::cchunk* current_chunk_;

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
        init();
    }

    /**
     * @brief Construct a new cpool object
     * 
     * we will build a pool with at most 20 chunks
     */
    cpool() : num_chunks_{0}, max_chunks_{20} {
        init();
    }

    ~cpool() {
        //destroy every chunk
        for(size_t i=0; i < num_chunks_; i++) {
            delete chunks_[i];
        }
        //destroy the array of chunks
        delete [] chunks_;
    }

    /**
     * @brief mark every object inside every chunk as "reusable"
     * 
     * this effectively mean that we are allowed to overwrite every object stored in the pool
     */
    inline void reclaim() {
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
            add_chunk(warthog::mem::DEFAULT_CHUNK_SIZE);
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
     * @brief number of bytes the whole pool uses in the system memory
     * 
     * @return size_t 
     */
    size_t mem() const {
        size_t bytes = 0;
        for(unsigned int i=0; i < num_chunks_; i++)
        {
            //space each chunk occupies
            bytes += chunks_[i]->mem();
        }
        //chunk array
        bytes += sizeof(warthog::mem::cchunk*)*max_chunks_;
        //this
        bytes += sizeof(*this);
        return bytes;
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
            << sizeof(OBJ);
            << std::endl;
        for(unsigned int i = 0; i < num_chunks_; i++) {
            chunks_[i]->print(out);
            out << std::endl;
        }
    }

private:

    // no copy
    cpool(const cpool& other) = delete; //{ } 
    cpool& operator=(const cpool& other) = delete; //{ return *this; }

    void init() {
        //initialize array
        chunks_ = new cchunk*[max_chunks_];
        for(int i = 0; i < (int) max_chunks_; i++) {
            add_chunk(DEFAULT_CHUNK_SIZE);
        }
        //mark the first chunk as the current one
        current_chunk_ = chunks_[0];
    }

    void add_chunk(size_t pool_size) {
        if(num_chunks_ < max_chunks_) {
            //we have space for another chunk. Create it
            chunks_[num_chunks_] = new cchunk<OBJ>(pool_size);
            num_chunks_++;
        } else {
            /* we have already reached the maximum chunk number allowed
             * but we need to allocate space for another object.
             * Enlarge the max_chunk threshold
             */

            // ENLARGE THE THRESHOLD
            //big_max is the new max_chunk threshold. we need to update the chunk array as well
            size_t big_max= max_chunks_*2;
            //CREATE THE NEW CHUNK ARRAY
            cchunk** big_chunks = new cchunk<OBJ>*[big_max];
            for(unsigned int i = 0; i < max_chunks_; i++) {
                big_chunks[i] = chunks_[i];
            }
            // DELETE PREVIOUS CHUNK ARRAY
            delete [] chunks_;
            chunks_ = big_chunks;
            //UPDATE THE THRESHOLD
            max_chunks_ = big_max;

            // finally; add a new chunk
            chunks_[num_chunks_] = new cchunk<OBJ>(pool_size);
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

}


#endif