#ifndef _VECTORPLUS_HEADER__
#define _VECTORPLUS_HEADER__

#include <vector>
#include <algorithm>

namespace cpp_utils {

/**
 * @brief a vector which has defined more utility functions
 * 
 * @tparam EL 
 */
template<typename EL>
class vectorplus : public std::vector<EL> {
public:
    /**
     * @brief check if the vector is actually empty
     * 
     * @return true if the vector has no elements
     * @return false otherwise
     */
    bool isEmpty() const {
        return std::vector<EL>::empty();
    }
    /**
     * @brief check if the vector has at least one element inside it
     * 
     * @return true if the vector has at least one element
     * @return false otherwise
     */
    bool hasAtLeastAnElement() const {
        return this->size() > 0;
    }
    /**
     * @brief an element in the vector. use negative indices for going backwards in the vector
     * 
     * @param index 
     * @return const EL& 
     */
    const EL& at(int index) const {
        return std::vector<EL>::operator[](this->toAbsolute(index));
    }
    /**
     * @brief an element in the vector. use negative indices for going backwards in the vector
     * 
     * @param index 
     * @return const EL& 
     */
    EL& at(int index) {
        return std::vector<EL>::operator[](this->toAbsolute(index));
    }
    const EL& operator [](int index) const {
        return this->at(index);
    }
    EL& operator [](int index) {
        return this->at(index);
    }
    /**
     * @brief add an element at the end of the vector
     * 
     * @param el the element to add
     */
    void add(const EL& el) {
        this->push_back(el);
    }
    /**
     * @brief remove the element in the given index
     * 
     * @param index the index involved. error if the index is not valid. index canm be negative for starting backwards
     */
    void removeAt(int index) {
        this->erase(this->begin() + this->toAbsolute(index));
    }
    /**
     * @brief remove the first value equal to `el` occuring in the vector
     * 
     * @param el the element to remove
     */
    void remove(const EL& el) {
        this->erase(std::remove(this->begin(), this->end(), el), this->end());
    }
    /**
     * @brief check if an element is inside the vector
     * 
     * @param el the element involved
     * @return true if the element is inside the vector
     * @return false otherwise
     */
    bool contains(const EL& el) const {
        auto position = std::find(this->begin(), this->end(), el);
        return position != this->end();
    }
private:
    int toAbsolute(int index) {
        return index >= 0 ? index : this->size() + index;
    }
};

}

#endif