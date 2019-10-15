#ifndef _CPP_UTILS_SET_PLUS_HEADER__
#define _CPP_UTILS_SET_PLUS_HEADER__

#include <unordered_set>

namespace cpp_utils {

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
        void add(const T& item) {
            Super::insert(item);
        }
    };

}

#endif