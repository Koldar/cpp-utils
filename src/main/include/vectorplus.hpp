#ifndef _VECTORPLUS_HEADER__
#define _VECTORPLUS_HEADER__

#include <vector>
#include <algorithm>
#include <functional>
#include <type_traits>

#include "functional.hpp"
#include "ICleanable.hpp"
#include "imemory.hpp"
#include "Random.hpp"

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
public:
    using This = vectorplus<EL>;
    using Super1 = std::vector<EL>;
    using Super1::size;
    using Super1::begin;
    using Super1::end;
public:
    friend std::ostream& operator << <>(std::ostream& out, const vectorplus<EL>& vec);
public:
    vectorplus(): Super1{} {
    }
    vectorplus(const EL& el): Super1(20, el) {
    }
    vectorplus(const Super1& other): Super1{other} {

    }
    vectorplus(std::size_t size, const EL& el): Super1(size, el) {

    }
    template <typename... OTHER>
    vectorplus(const EL& first, const EL& second, const EL& third, const OTHER&... args) : Super1{} {
        this->add(first);
        this->add(second);
        this->add(third);
        this->add(args...);
    }
    vectorplus(Super1&& other): Super1{other} {

    }
    vectorplus(const cpp_utils::vectorplus<EL>& other): Super1{} {
        debug("building the path!!!!");
        for (auto el : other) {
            this->add(el);
        }
    }

    vectorplus(cpp_utils::vectorplus<EL>&& other) : Super1{other} {
    }

    This& operator=(const This& other) {
        Super1::operator =(other);
        return *this;
    }

    vectorplus<EL>& operator=(This&& other) {
        Super1::operator =(std::move(other));
        return *this;
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
    size_t size() const {
        return Super1::size();
    }
public:
    template <typename OUT>
    vectorplus<OUT> map(const function_t<EL, OUT>& mapper) const {
        vectorplus<OUT> result{};
        for (auto el : *this) {
            result.add(mapper(el));
        }
        return result;
    }

    /**
     * @brief generates a **new** vector only with the elements satisfying the filter
     * 
     * @param lambda the filter
     * @return vectorplus<EL> a new vector
     */
    vectorplus<EL> select(const predicate_t<EL>& filter) const {
        vectorplus<EL> result{};
        for (auto el: *this) {
            if (filter(el)) {
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
    vectorplus<EL> reject(const predicate_t<EL>& lambda) const {
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
     * @tparam OUT the type of the result of this operation
     * @param first first value used to combine to the head of the vector
     * @param lambda function to apply
     * @return OUT value obtained by the reduction
     */
    template <typename OUT>
    OUT lreduce(const OUT& first, const bifunction_t<EL, OUT, OUT>& lambda) const {
        OUT result{first};
        for (auto el : *this) {
            result = lambda(el, result);
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
     * @tparam OUT the type of the result of this operation
     * @param first first value used to combine to the tail of the vector
     * @param lambda function to apply
     * @return OUT value obtained by the reduction
     */
    template <typename OUT>
    OUT rreduce(const OUT& first, const bifunction_t<EL, OUT, OUT>& lambda) const {
        OUT result{first};
        for (int i=this->lastIndex(); i>=0; --i) {
            result = lambda((*this)[i], result);
        }
        return result;
    }
public:
    bool isEmpty() const {
        return Super1::empty();
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
        EL& result = std::vector<EL>::operator[](this->toAbsolute(index));
        return result;
    }

    /**
     * @brief fetch a subrange of the vector
     * 
     * @code
     *  {3,4,1,5,7}.at(1,-1) //{4,1,5}
     * @endcode
     * 
     * @param start the first index (included) which will be included in the return value
     * @param end the last index (excluded) which will included in tyhe return value
     * @return const This the subrange wished
     */
    const This at(int start, int end) const {
        This result{};
        for (int i=this->toAbsolute(start); i<this->toAbsolute(end); ++i) {
            result.add(std::vector<EL>::operator[](i));
        }
        return result;
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
     * @brief alias of `getHead`
     * 
     * @return const EL& head of the vector
     */
    const EL& peek() const {
        return (*this)[0];
    }
    /**
     * @brief alias of `getHead`
     * 
     * @return L& head of the vector
     */
    EL& peek() {
        return (*this)[0];
    }
    /**
     * @brief yields the first item of the vector and removes it from the vector
     * 
     * @pre
     *  @li the element is not empty
     * 
     * @return EL the first element of the vector
     */
    EL popHead() {
        EL result = this->front();
		this->erase(this->begin());
        return result;
    }
    /**
     * @brief Alias of `popHead`
     * 
     * @return EL the first element of the vector
     */
    EL pop() {
        return this->popHead();
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

    /**
     * @brief get a random element compliant with the given range
     * 
     * @param i the range involved
     * @return const EL& a random element within the range
     */
    const EL& getRandomIn(const Interval<int>& i) const {
        return (*this)[Random::next(i)];
    }

    /**
     * @brief get a randomic index for this vector
     * 
     * @return int 
     */
    int getRandomIndex() const {
        return Random::next(0, static_cast<int>(this->size()), false);   
    }

    /**
     * @brief get a random element in the vector
     * 
     * @return const EL& a random element in the vector
     */
    const EL& getRandom() const {
        return (*this)[Random::next(0, this->size(), true, false)];
    }

    /**
     * @brief like ::getRandom(), but  yield the index of the chosen element
     * 
     * @return std::pair<int, const EL&> 
     */
    std::pair<int, const EL&> getRandomPair() const {
        int id = Random::next(0, static_cast<int>(this->size()), false);
        return std::pair{id, this->at(id)};
    }
    
    /**
     * @brief create a string from the vector plus
     * 
     * @param start the string to put at the beginning of the vector
     * @param sep the string to put between one element and the next one of the vector
     * @param end the string to put at the end of the vector
     * @return std::string the built string
     */
    std::string makeString(const std::string& start, const std::string& sep, const std::string& end) const {
        std::stringstream ss;
        ss << start;
        for (int i=0; i<this->size(); ++i) {
            ss << (*this)[i];
            if ((i+1) < this->size()) {
                //intermediate element
                ss << sep;
            } else {
                //last element
            }
        }
        ss << end;

        return ss.str();
    }

    /**
     * @brief like ::makeString, but the first and the last characters are, respectively "[" and "]"
     * 
     * @param sep the string to put between one element and the next one of the vector
     * @return std::string the string built
     */
    std::string makeString(const std::string& sep) const {
        return this->makeString("[", sep, "]");
    }

    /**
     * @brief like ::makeString, but the sep string is ","
     * 
     * @return std::string the string built
     */
    std::string makeString() const {
        return this->makeString(",");
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