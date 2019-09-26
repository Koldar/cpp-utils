#ifndef _VECTORPLUS_HEADER__
#define _VECTORPLUS_HEADER__

#include <vector>
#include <algorithm>
#include <functional>
#include <type_traits>
#include "ICleanable.hpp"
#include "imemory.hpp"

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
class vectorplus : public std::vector<EL>, ICleanable, IMemorable {
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
    template <typename... OTHER>
    vectorplus(const EL& first, const EL& second, const EL& third, const OTHER&... args) : std::vector<EL>{} {
        this->add(first);
        this->add(second);
        this->add(third);
        this->add(args...);
    }
    vectorplus(std::vector<EL>&& other): std::vector<EL>{::std::move(other)} {

    }
    vectorplus(const cpp_utils::vectorplus<EL>& other): std::vector<EL>{} {
        for (auto el : other) {
            this->add(el);
        }
    }
    //TODO implement
    vectorplus(cpp_utils::vectorplus<EL>&& other) : std::vector<EL>{::std::move(other)} {
    }

    vectorplus<EL>& operator=(const vectorplus<EL>& other) {
        ::std::vector<EL>::operator =(other);
    }

    vectorplus<EL>& operator=(const vectorplus<EL>&& other) {
        ::std::vector<EL>::operator =(std::move(other));
    }


protected:
    /**
     * @brief dummy add. Needed by resource unpacking
     * 
     * @return cpp_utils::vectorplus<EL>& 
     */
    cpp_utils::vectorplus<EL>& add() {
        return *this;
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
    cpp_utils::vectorplus<EL>& add(const EL& el) {
        this->push_back(el);
        return *this;
    }

    /**
     * @brief add an element at the end of the vector
     * 
     * @param el the element to add
     */
    cpp_utils::vectorplus<EL>& addTail(const EL& el) {
        this->push_back(el);
        return *this;
    }

    /**
     * @brief add an element at the beginning of the vector
     * 
     * @param el the element to add
     */
    cpp_utils::vectorplus<EL>& addHead(const EL& el) {
        this->insert(this->begin(), el);
        return *this;
    }

    /**
     * @brief add all the elements in the given container inside this one
     * 
     * @tparam CONTAINER the type of the other container
     * @param other the other container
     */
    template <template<typename> typename CONTAINER>
    cpp_utils::vectorplus<EL>& addAll(const CONTAINER<EL>& other) {
        for (auto x : other) {
            this->add(x);
        }
        return *this;
    }

    template <typename... OTHER>
    cpp_utils::vectorplus<EL>& add(const EL& first, const OTHER&... args) {
        this->add(first);
        return this->add(args...);
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

    /**
     * @brief alter the current vector by sorting it according a specific comparator
     * 
     * @post
     *  @li the vector is sorted
     * 
     * @param sorter the function used to sort. It accept 2 parameters which are 2 elements in the array. 
     *  If the first element is "less than" the other, return true, otherwise return false
     * 
     */
    cpp_utils::vectorplus<EL>& sort(std::function<bool(EL,EL)> sorter) {
        std::sort(this->begin(), this->end(), sorter);
        return *this;
    }

    template<typename OUTPUT>
    vectorplus<OUTPUT> map(std::function<OUTPUT(EL)> lambda) const {
        vectorplus<OUTPUT> result{};
        for (auto el: *this) {
            result.add(lambda(el));
        }
        return result;
    }

    /**
     * @brief generates a **new** vector only with the elements satisfying the filter
     * 
     * @param lambda the filter
     * @return vectorplus<EL> a new vector
     */
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
     * @brief generates a **new** vector only with the elements that **don't satisfy** the filter
     * 
     * @param lambda the filter
     * @return vectorplus<EL> a new vector
     */
    vectorplus<EL> reject(std::function<bool(EL)> lambda) const {
        vectorplus<EL> result{};
        for (auto el: *this) {
            if (!lambda(el)) {
                result.add(el);
            }
        }
        return result;
    }

    /**
     * @brief left reduce operation
     * 
     * @code
     * vector={1,2,3,4};
     * first=0;
     * lambda=+
     * (((((0) + 1) + 2) + 3) + 4)
     * @endcode
     * 
     * @tparam OUTPUT the type of the result of this operation
     * @param first first value used to combine to the head of the vector
     * @param lambda function to apply
     * @return OUTPUT value obtained by the reduction
     */
    template <typename OUTPUT>
    OUTPUT lreduce(const OUTPUT first, std::function<OUTPUT(EL, OUTPUT)> lambda) const {
        OUTPUT result = first;
        for (auto i=0; i<this->size(); ++i) {
            result = lambda((*this)[i], result);
        }
        return result;
    }
    /**
     * @brief right reduce operation
     * 
     * @code
     * vector={1,2,3,4};
     * first=0;
     * lambda=+
     * (1 + (2 + (3 + (4 + (0)))))
     * @endcode
     * 
     * @tparam OUTPUT the type of the result of this operation
     * @param first first value used to combine to the tail of the vector
     * @param lambda function to apply
     * @return OUTPUT value obtained by the reduction
     */
    template <typename OUTPUT>
    OUTPUT rreduce(const OUTPUT& first, std::function<OUTPUT(EL, OUTPUT)> lambda) const {
        OUTPUT result = first;
        for (auto i=this->lastIndex(); i>=0; --i) {
            result = lambda((*this)[i], result);
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
    MemoryConsumption getByteMemoryOccupied() const {
        return sizeof(EL) * this->size();
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