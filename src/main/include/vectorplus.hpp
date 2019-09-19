#ifndef _VECTORPLUS_HEADER__
#define _VECTORPLUS_HEADER__

#include <vector>
#include <algorithm>
#include <functional>
#include <type_traits>
#include "ICleanable.hpp"

namespace cpp_utils {

template<typename EL>
class vectorplus;

template <typename EL>
std::ostream& operator << (std::ostream& out, const vectorplus<EL>& vec);


/**
 * @brief a vector which has defined more utility functions
 * 
 * @tparam EL 
 */
template<typename EL>
class vectorplus : public std::vector<EL>, ICleanable {
    friend std::ostream& operator << <>(std::ostream& out, const vectorplus<EL>& vec);
public:
    vectorplus(): std::vector<EL>{} {
    }
    vectorplus(const EL& el): std::vector<EL>(20, el) {
    }
    vectorplus(const std::vector<EL>& other): std::vector<EL>{other} {

    }
    vectorplus(std::size_t size, const EL& el): std::vector<EL>(size, el) {

    }
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
        return this->std::vector<EL>::operator[](this->toAbsolute(index));
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
     * @brief add an element at the end of the vector
     * 
     * @param el the element to add
     */
    void addTail(const EL& el) {
        this->push_back(el);
    }

    /**
     * @brief add an element at the beginning of the vector
     * 
     * @param el the element to add
     */
    void addHead(const EL& el) {
        this->insert(this->begin(), el);
    }

    /**
     * @brief add all the elements in the given container inside this one
     * 
     * @tparam CONTAINER the type of the other container
     * @param other the other container
     */
    template <template<typename> typename CONTAINER>
    void addAll(const CONTAINER<EL>& other) {
        for (auto x : other) {
            this->add(x);
        }
    }

    template <typename... OTHER>
    void add(EL first, OTHER... args) {
        this->add(first);
        this->add(args...);
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
    template<typename OUTPUT>
    vectorplus<OUTPUT> map(std::function<OUTPUT(EL)> lambda) const {
        vectorplus<OUTPUT> result{};
        for (auto el: *this) {
            result.add(lambda(el));
        }
        return result;
    }
    vectorplus<EL> filter(std::function<bool(EL)> lambda) const {
        vectorplus<EL> result{};
        for (auto el: *this) {
            if (lambda(el)) {
                result.add(el);
            }
        }
        return result;
    }
    /**
     * @brief set all the cell in the vector with the given value
     * 
     * @param el the value each cell will have after the completition of this method
     * @return this
     */
    vectorplus<EL>& fill(const EL& el) {
        std::fill(this->begin(), this->end(), el);
        return *this;
    }
    /**
     * @brief reverse the entire vector
     * 
     * @post
     *  @li the elements of the vector are reversed
     * 
     * @code
     *  {1,2,3,4}
     *  //reversed vector
     *  {4,3,2,1}
     * @endcode
     * 
     * @return this
     */
    vectorplus<EL>& reverse() {
        std::reverse(this->begin(), this->end());
        return *this;
    }
    /**
     * @brief the first element of this vector
     * 
     * @pre
     *  @li vector is not empty
     * 
     * @return EL& 
     */
    const EL& getHead() const {
        return (*this)[0];
    }
    /**
     * @brief the first element of this vector
     * 
     * @pre
     *  @li vector is not empty
     * 
     * @return EL& 
     */
    EL& getHead() {
        return (*this)[0];
    }
    /**
     * @brief the last element of this vector
     * 
     * @pre
     *  @li vector is not empty
     * 
     * @return EL& 
     */
    const EL& getTail() const {
        return (*this)[this->size() - 1];
    }
    /**
     * @brief the last element of this vector
     * 
     * @pre
     *  @li vector is not empty
     * 
     * @return EL& 
     */
    EL& getTail() {
        return (*this)[this->size() - 1];
    }
    /**
     * @brief the first index of this vector
     * 
     * @pre
     *  @li vector is not empty
     * 
     * @return int always 0
     */
    int firstIndex() const {
        return 0;
    }
    /**
     * @brief the last index of this vector
     * 
     * @pre
     *  @li vector is not empty
     * 
     * @return int 
     */
    int lastIndex() const {
        return this->size() - 1;
    }
public:
    template <typename... OTHER>
    static vectorplus<EL> make(OTHER... other) {
        vectorplus<EL> result{};
        return make(result, other...);
    }

    template <typename FIRST>
    static vectorplus<EL>& make(vectorplus<EL>& vec, FIRST f) {
        vec.add(f);
        return vec;
    }

    template <typename FIRST, typename... OTHER>
    static vectorplus<EL>& make(vectorplus<EL>& vec, FIRST f, OTHER... other) {
        vec.add(f);
        return make(vec, other...);
    }
public:
    virtual void cleanup() {
        this->clear();
    }
private:
    int toAbsolute(int index) const {
        return index >= 0 ? index : this->size() + index;
    }
};

template <typename EL>
std::ostream& operator << (std::ostream& out, const vectorplus<EL>& vec) {
    out << "[";
    bool first = true; 
    for (auto x: vec) {
        if (first) {
            out << x;
            first = false;
        } else {
            out << ", " << x;
        }
    }
    out << "]";
    return out;
}

}

#endif