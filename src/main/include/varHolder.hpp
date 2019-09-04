#ifndef _VARHOLDER_HEADER__
#define _VARHOLDER_HEADER__

#include <cstring>

namespace cpp_utils {

template <typename T>
class VarHolder;

template <typename T>
std::ostream& operator << (std::ostream& out, const VarHolder<T>& a);
template <typename T>
bool operator == (const VarHolder<T>& a, const VarHolder<T>& b);
template <typename T>
bool operator != (const VarHolder<T>& a, const VarHolder<T>& b);
template <typename T>
bool operator < (const VarHolder<T>& a, const VarHolder<T>& b);
template <typename T>
bool operator > (const VarHolder<T>& a, const VarHolder<T>& b);
template <typename T>
bool operator <= (const VarHolder<T>& a, const VarHolder<T>& b);
template <typename T>
bool operator >= (const VarHolder<T>& a, const VarHolder<T>& b);
template <typename T>
VarHolder<T> operator +(const VarHolder<T>& a, const VarHolder<T>& b);
template <typename T>
VarHolder<T> operator -(const VarHolder<T>& a, const VarHolder<T>& b);
template <typename T>
VarHolder<T> operator *(const VarHolder<T>& a, const VarHolder<T>& b);
template <typename T>
VarHolder<T> operator /(const VarHolder<T>& a, const VarHolder<T>& b);

/**
 * @brief Allows to put space in the stack without having to initialize it
 * 
 * Suppose you're dealing with a class `X` and you don't care how to initialize a certain field `alpha` since you will fill it later.
 * Suppose that the type of such field `alpha` is class `Y` and that you haven't specified a default copnstructor.
 * 
 * You need something that allocate space in each instance of `X` without having to explicitly call the default constructor of `Y`.
 * This class does that.
 * @code
 *  
 * @endcode
 * 
 * @tparam T type that will be put in the wrapper
 */
template <typename T>
struct VarHolder {
public:
    friend std::ostream& operator << <>(std::ostream& out, const VarHolder<T>& a);
    friend bool operator == <>(const VarHolder<T>& a, const VarHolder<T>& b);
    friend bool operator != <>(const VarHolder<T>& a, const VarHolder<T>& b);
    friend bool operator < <>(const VarHolder<T>& a, const VarHolder<T>& b);
    friend bool operator > <>(const VarHolder<T>& a, const VarHolder<T>& b);
    friend bool operator <= <>(const VarHolder<T>& a, const VarHolder<T>& b);
    friend bool operator >= <>(const VarHolder<T>& a, const VarHolder<T>& b);
    friend VarHolder<T> operator + <>(const VarHolder<T>& a, const VarHolder<T>& b);
    friend VarHolder<T> operator - <>(const VarHolder<T>& a, const VarHolder<T>& b);
    friend VarHolder<T> operator * <>(const VarHolder<T>& a, const VarHolder<T>& b);
    friend VarHolder<T> operator / <>(const VarHolder<T>& a, const VarHolder<T>& b);
private:
    char space[sizeof(T)];
public:
    VarHolder() {

    }
    VarHolder(const VarHolder<T>& other) {
        memcpy(space, &other.space, sizeof(T));
    }
    ~VarHolder() {

    }
    VarHolder<T>& operator = (const VarHolder<T>& other) {
        memcpy(space, &other.space, sizeof(T));
        return *this;
    }
    VarHolder<T>& operator = (const T& other) {
        //this.space = other;
        memcpy(space, &other, sizeof(T));
        return *this;
    }
    T& get() {
        return *(T*)(&(this->space[0]));
    }
    const T& get() const {
        return *(T*)(&(this->space[0]));
    }

    /*implicit*/ operator T() const {
        return this->get();
    }

    /*implicit*/ operator T&() {
        return this->get();
    }

    /*implicit*/ operator T*() const {
        return &this->get();
    }

    const T*/*implicit*/ operator &() const {
        return &(this->get());
    }

    T*/*implicit*/ operator &() {
        return &(this->get());
    }

    VarHolder<T>& operator += (const VarHolder<T>& other) {
        this->get() += other.get();
        return *this;
    }
    VarHolder<T>& operator -= (const VarHolder<T>& other) {
        this->get() -= other.get();
        return *this;
    }
    VarHolder<T>& operator *= (const VarHolder<T>& other) {
        this->get() *= other.get();
        return *this;
    }
    VarHolder<T>& operator /= (const VarHolder<T>& other) {
        this->get() /= other.get();
        return *this;
    }
};

template <typename T>
std::ostream& operator << (std::ostream& out, const VarHolder<T>& a) {
    out << a.get();
    return out;
}

template <typename T>
bool operator == (const VarHolder<T>& a, const VarHolder<T>& b) {
    return a.get() == b.get();
}
template <typename T>
bool operator != (const VarHolder<T>& a, const VarHolder<T>& b) {
    return a.get() != b.get();
}
template <typename T>
bool operator < (const VarHolder<T>& a, const VarHolder<T>& b) {
    return a.get() < b.get();
}
template <typename T>
bool operator > (const VarHolder<T>& a, const VarHolder<T>& b) {
    return a.get() > b.get();
}
template <typename T>
bool operator <= (const VarHolder<T>& a, const VarHolder<T>& b) {
    return a.get() <= b.get();
}
template <typename T>
bool operator >= (const VarHolder<T>& a, const VarHolder<T>& b) {
    return a.get() >= b.get();
}

template <typename T>
VarHolder<T> operator +(const VarHolder<T>& a, const VarHolder<T>& b) {
    VarHolder<T> result{a};
    return a += b;
}

template <typename T>
VarHolder<T> operator -(const VarHolder<T>& a, const VarHolder<T>& b) {
    VarHolder<T> result{a};
    return a -= b;
}

template <typename T>
VarHolder<T> operator *(const VarHolder<T>& a, const VarHolder<T>& b) {
    VarHolder<T> result{a};
    return a *= b;
}

template <typename T>
VarHolder<T> operator /(const VarHolder<T>& a, const VarHolder<T>& b) {
    VarHolder<T> result{a};
    return a /= b;
}

}

#endif