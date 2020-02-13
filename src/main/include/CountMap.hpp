/**
 * @file CountMap.hpp
 * @author Massimo Bono (you@domain.com)
 * @brief a map which count the number of times a key is present in some place
 * @version 0.1
 * @date 2020-02-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _CPPUTILS_COUNTMAP_HEADER__
#define _CPPUTILS_COUNTMAP_HEADER__

#include "mapplus.hpp"

namespace cpp_utils::datastructures {

    /**
     * @brief a map which keeps track on the number of times an element appears somewhere
     * 
     * For example, you may want to keep track of the number appeared in a runing stream of int.
     * 
     * you can use this structure to do do:
     * 
     * @code
     *  CountMap<int> map{};
     *  while (true) {
     *      int x = getNumberFromStream();
     *      map.increase(x, 1);
     *  }
     * @endcode
     * 
     * As soon as the value associated by a key goes to 0 (or below it), the map automatically removes the mapping from itself
     * 
     * @tparam K type of the key
     */
    template <typename K>
    class CountMap {
    public:
        using This = CountMap<K>;
    private:
        MapPlus<K, int> mapping;
    public:
        CountMap(): mapping{} {

        }
        CountMap(const This& o): mapping{o} {

        }
        CountMap(This&& o): mapping{std::move(o)} {

        }
        This& operator =(const This& o) {
            this->mapping = o.mapping;
            return *this;
        }
        This& operator =(This&& o) {
            this->mapping = std::move(o.mapping);
            return *this;
        }
        virtual ~CountMap() {

        }
    public:
        int size() const {
            return this->mapping.size();
        }
        int count(const K& k) const {
            if (this->mapping.containsKey(k)) {
                return this->mapping[k];
            } else {
                return 0;
            }   
        }
        /**
         * @brief increase a certain key by 1
         * 
         * @param k the key involved
         * @return This& this
         */
        This& increase1(const K& k) {
            return this->increase(k, 1);
        }

        /**
         * @brief increase a certain key by a given value
         * 
         * @param k the key involved
         * @param value the value which will be summed to the value associated to `k`
         * @return This& this
         */
        This& increase(const K& k, int value) {
            this->mapping[k] += value;
            return *this;
        }

        /**
         * @brief decrease a certain key by a given value
         * 
         * @param k the key involved
         * @param value the value which will be subtracted to the value associated to `k`
         * @return This& this
         */
        This& decrease(const K& k, int value) {
            this->mapping[k] -= value;
            if (this->mapping[k] <= 0) {
                this->mapping.removeKey(k);
            }
            return *this;
        }

        /**
         * @brief decrease a certain key by 1
         * 
         * @param k the key involved
         * @return This& this
         */
        This& decrease1(const K& k) {
            return this->decrease(k, 1);
        }

        This& set(const K& k, int value) {
            this->mapping[k] = value;
            if (value <= 0) {
                this->mapping.removeKey(k);
            }
        }
    };

}

#endif