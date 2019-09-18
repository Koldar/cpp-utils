#ifndef _MAPPLUS_HEADER__
#define _MAPPLUS_HEADER__

#include <iostream>
#include <sstream>
#include <unordered_map>
#include "ICleanable.hpp"

namespace cpp_utils {


/**
 * @brief derived class of map
 * 
 * @note
 * The class just add some convenience method. Since I'm coming from Java, I felt I would like to have them as well here in C++
 * (yes, I'm aware of std::find and such).
 * 
 * @tparam K 
 * @tparam V 
 */
template <typename K, typename V, 
    typename HASH = std::hash<K>,
	typename PRED = std::equal_to<K>,
	typename ALLOC = std::allocator<std::pair<const K, V>>
>
class MapPlus: public std::unordered_map<K, V, HASH, PRED, ALLOC>, public ICleanable {
public:
    friend std::ostream& operator <<(std::ostream& out, const MapPlus<K, V, HASH, PRED, ALLOC>& a) {
        out << "{";
        for (auto it=a.cbegin(); it!=a.cend(); ++it) {
            out << it->first << ": " << it->second << ", ";
        }
        out << "}";
        return out;
    }
public:
    MapPlus(): std::unordered_map<K, V, HASH, PRED, ALLOC>{} {

    }
    virtual ~MapPlus() {

    }
    MapPlus(const MapPlus<K, V, HASH, PRED, ALLOC>& other): std::unordered_map<K, V, HASH, PRED, ALLOC>{other} {

    }
public:
    void put(const K& k, const V& v) {
        (*this)[k] = v;
    }
    const V& get(const K& k) const {
        return (*this)[k];
    }
    V& get(const K& k) {
        return (*this)[k];
    }
    /**
     * @brief check if the map contains a specific key
     * 
     * @param k the key we're looking for
     * @return true if the key is inside the map
     * @return false otherwise
     */
    bool containsKey(const K& k) const {
        return this->find(k) != this->end();
    }
    bool containsValue(const V& v) const {
        for (auto it=this->cbegin(); it!=this->cend(); ++it) {
            if (it->second == v) {
                return true;
            }
        }
        return false;
    }
    K keyOf(const V& v) const {
        for (auto it=this->cbegin(); it!=this->cend(); ++it) {
            if (it->second == v) {
                return it->first;
            }
        }
        throw cpp_utils::exceptions::ElementNotFoundException<V, MapPlus<K, V, HASH, PRED, ALLOC>>{v, *this};
    }
public:
    virtual void cleanup() {
        this->clear();
    }
};



}

#endif 