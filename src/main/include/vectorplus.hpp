#ifndef _VECTORPLUS_HEADER__
#define _VECTORPLUS_HEADER__

#include <vector>
#include <algorithm>
#include <functional>

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
class vectorplus : public std::vector<EL> {
    friend std::ostream& operator << <>(std::ostream& out, const vectorplus<EL>& vec);
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
    vectorplus<OUTPUT> map(std::function<OUTPUT(EL)> lambda) {
        vectorplus<OUTPUT> result{};
        for (auto el: *this) {
            result.add(lambda(el));
        }
        return result;
    }
    vectorplus<EL> filter(std::function<bool(EL)> lambda) {
        vectorplus<EL> result{};
        for (auto el: *this) {
            if (lambda(el)) {
                result.add(el);
            }
        }
        return result;
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
private:
    int toAbsolute(int index) {
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