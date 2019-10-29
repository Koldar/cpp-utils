#ifndef _CPP_UTILS_SET_PLUS_HEADER__
#define _CPP_UTILS_SET_PLUS_HEADER__

#include <unordered_set>
#include "Random.hpp"
#include "serializers.hpp"

namespace cpp_utils {

    /**
     * @brief Represents a more powerful version of unordered_set. Keep in mind that you can always use unordered_set. Thsi class is just for my convenience
     * 
     * @tparam T 
     * @tparam std::hash<T> 
     * @tparam std::equal_to<T> 
     * @tparam std::allocator<T> 
     */
    template<class T, class _Hash = std::hash<T>, class _Pred = std::equal_to<T>, class _Alloc = std::allocator<T> >
    class SetPlus: public std::unordered_set<T, _Hash, _Pred, _Alloc> {
        typedef std::unordered_set<T, _Hash, _Pred, _Alloc> Super;
        typedef SetPlus<T, _Hash, _Pred, _Alloc> SetPlusInstance;
    public:
        /**
         * @brief check if the set contains the item
         * 
         * @param item the item to check
         * @return true 
         * @return false 
         */
        bool contains(const T& item) const {
            return Super::find(item) != Super::end();
        }
        /**
         * @brief Adds an element inside the set
         * 
         * @param item the item to add in the set
         */
        void add(const T& item) {
            Super::insert(item);
        }

        /**
         * @brief create a copy of this set where each item is mapped into something else
         * 
         * @tparam OUT type of the new set
         * @param mapping function converting each element into something else
         * @return SetPlus<OUT> new set
         */
        template <typename OUT>
        SetPlus<OUT> map(std::function<OUT(const T& el)> mapping) const {
            SetPlus<OUT> result{};
            for (auto it=Super::begin(); it!=Super::end(); ++it) {
                result.add(mapping(*it));
            }
            return result;
        }

        /**
         * @brief create a copy of this set by considering only the element satisfying the filter function
         * 
         * @param filter the filter function
         * @return SetPlusInstance a new copy where all the elements are satisfying @c filter
         */
        SetPlusInstance select(std::function<bool(const T& el)> filter) const {
            SetPlusInstance result{};
            for (auto it=Super::begin(); it!=Super::end(); ++it) {
                if (filter(*it)) {
                    result.add(*it);
                }
            }
            return result;
        }

        /**
         * @brief create a copy of this set by considering only the element **not** satysfying the filter function
         * 
         * @param filter the filter function
         * @return SetPlusInstance a new copy where all the elements satisfying @c filter has been ignored
         */
        SetPlusInstance reject(std::function<bool(const T& el)> filter) const {
            return this->select([&](const T& el) { return !filter(el); });
        }

        /**
         * @brief get a random element in the set
         * 
         * @return const T& the generated element
         */
        const T& getRandomItem() const {
            if (this->isEmpty()) {
                throw cpp_utils::exceptions::EmptyObjectException<SetPlusInstance>{*this};
            }
            int n = Random::nextNum<int>(0, this->size());
            for (auto it=this->begin(); it!=this->end(); ++it) {
                if (n == 0) {
                    return *it;
                } else {
                    n -= 1;
                }
            }
        }

        /**
         * @brief remove an item in the set
         * 
         * @param item the item to delete
         */
        void remove(const T& item) {
            auto it = this->find(item);
            this->erase(it);
        }

        /**
         * @brief Check if the set is empty
         * 
         * @return true 
         * @return false 
         */
        bool isEmpty() const {
            return this->size() == 0;
        }
    };

}

namespace cpp_utils::serializers {

    template<class T, class _Hash = std::hash<T>, class _Pred = std::equal_to<T>, class _Alloc = std::allocator<T> >
    void saveToFile(FILE* file, const cpp_utils::SetPlus<T, _Hash, _Pred, _Alloc>& set)  {
        saveToFile(file, set.size());
        for (auto it=set.begin(); it!=set.end(); ++it) {
            T el{};
            // /* Clear node struct to suppress valgrind warnings */
            memset(&el, 0, sizeof(el));
            el = *it;
            if(std::fwrite(&el, sizeof(el), 1, file) != 1) {
                throw cpp_utils::exceptions::FileOpeningException{recoverFilename(file)};
            }
            // TODO I don't know wh this doesn't work when used in external project with T set to a Edge<PerturbatedCost>
            //saveToFile(file, *it);
        }
    }

    template<typename T, class _Hash = std::hash<T>, class _Pred = std::equal_to<T>, class _Alloc = std::allocator<T> >
    cpp_utils::SetPlus<T, _Hash, _Pred, _Alloc>& loadFromFile(FILE* f, cpp_utils::SetPlus<T, _Hash, _Pred, _Alloc>& set) {
        set.clear();
        size_t size = 0;
        loadFromFile(f, size);

        for (int i=0; i<size; ++i) {
            T element;
            size_t stuffRead = std::fread(&element, sizeof(element), 1, f);
            if((int)stuffRead != 1) {
                error("we were expecting to read ", 1, " but we read", stuffRead, "elements instead");
                throw cpp_utils::exceptions::FileOpeningException{recoverFilename(f)};
            }
            set.add(element);
        }
        
        // TODO I don't know wh this doesn't work when used in external project with T set to a Edge<PerturbatedCost>
        // for (int i=0; i<size; ++i) {
        //     T element;
        //     loadFromFile(f, element);
        //     set.add(element);
        // }
        
        return set;
    }

}

#endif