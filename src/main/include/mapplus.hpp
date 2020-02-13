#ifndef _MAPPLUS_HEADER__
#define _MAPPLUS_HEADER__

#include <iostream>
#include <sstream>
#include <unordered_map>
#include "ICleanable.hpp"
#include "exceptions.hpp"

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
    using This = MapPlus<K, V, HASH, PRED, ALLOC>;
    using Super = std::unordered_map<K, V, HASH, PRED, ALLOC>;
public:
    friend std::ostream& operator <<(std::ostream& out, const This& a) {
        out << "{";
        for (auto it=a.cbegin(); it!=a.cend(); ++it) {
            out << it->first << ": " << it->second << ", ";
        }
        out << "}";
        return out;
    }
public:
    MapPlus(): Super{} {

    }
    virtual ~MapPlus() {

    }
    MapPlus(const This& other): Super{other} {

    }
    MapPlus(This&& other): Super{std::move(other)} {

    }
    This& operator =(const This& o) {
        Super::operator =(o);
        return *this;
    }
    This& operator =(This&& o) {
        Super::operator =(std::move(o));
        return *this;
    }
public:
    V& operator [](const K& k) {
        return Super::operator [](k);
    }
    const V& operator [](const K& k) const {
        return Super::at(k);
    }
public:
    /**
     * @brief adds a key-mapping inside the map
     * 
     * @param k key to add
     * @param v value to add
     */
    void put(const K& k, const V& v) {
        (*this)[k] = v;
    }
    /**
     * @brief retrieve the valu associated to the given key
     * 
     * @param k key involved
     * @return const V& the value of the key
     */
    const V& get(const K& k) const {
        return (*this)[k];
    }
    /**
     * @brief retrieve the valu associated to the given key
     * 
     * @param k key involved
     * @return const V& the value of the key
     */
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

    /**
     * @brief remove the key-mapping present in the map
     * 
     * The function does nothing if the key was not present in the map
     * 
     * @param k the key to remove
     */
    void removeKey(const K& k) {
        (*this).erase(k);   
    }
public:
    virtual void cleanup() {
        this->clear();
    }
};



}

#endif 