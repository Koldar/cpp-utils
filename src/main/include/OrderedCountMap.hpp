/**
 * @file OrderedCountMap.hpp
 * @author Massimo Bono (you@domain.com)
 * @brief a CountMap where the keys are ordered in some way
 * @version 0.1
 * @date 2020-02-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _CPPUTILS_ORDEREDCOUNTMAP_HEADER__
#define _CPPUTILS_ORDEREDCOUNTMAP_HEADER__

#include <map>

#include "assertions.hpp"

namespace cpp_utils::datastructures {

    template <typename K>
    class OrderedCountMap {
    public:
        using This = OrderedCountMap<K>;
    private:
        std::map<K, int, std::less<K>> mapping;
    public:
        OrderedCountMap(): mapping{} {

        }
        OrderedCountMap(const This& o): mapping{o} {

        }
        OrderedCountMap(This&& o): mapping{std::move(o)} {

        }
        This& operator =(const This& o) {
            this->mapping = o.mapping;
            return *this;
        }
        This& operator =(This&& o) {
            this->mapping = std::move(o.mapping);
            return *this;
        }
        virtual ~OrderedCountMap() {

        }
    public:
        /**
         * @brief get the key which has the lowest score according to the criterion of this OrderedCountMap
         * 
         * @pre
         *  @li container not empty
         * 
         * @return const K& the first key
         */
        const K& getFirstKey() const {
            assertDoNotHappen(this->mapping.empty());
            auto it = this->mapping.cbegin();
            return it->first;
        }
        const K& getLastKey() const {
            assertDoNotHappen(this->mapping.empty());
            auto it = this->mapping.crbegin();
            return it->first;
        }
        int size() const {
            return this->mapping.size();
        }
        int count(const K& k) const {
            auto it = this->mapping.find(k);
            if (it != this->mapping.end()) {
                return it->second;
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
                this->mapping.erase(k);
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
                this->mapping.erase(k);
            }
        }
    public:
        friend std::ostream& operator <<(std::ostream& out, const This& a) {
            out << "{";
            for (auto it=a.mapping.cbegin(); it!= a.mapping.cend(); ++it) {
                out << it->first << " = " << it->second << ", ";
            }
            out << "}";
            return out;
        }
    };

}

#endif