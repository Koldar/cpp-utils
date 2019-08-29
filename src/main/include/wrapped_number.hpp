#ifndef _SAFE_NUMBER_HPP
#define _SAFE_NUMBER_HPP

#include <iostream>

namespace cpp_utils {


template<typename NUM, NUM LB, NUM UB>
class wrapped_number;

template<typename NUM, NUM LB, NUM UB>
bool operator == (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
bool operator != (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
bool operator < (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
bool operator <= (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
bool operator > (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
bool operator >= (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator + (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator + (const NUM& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator - (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator - (const NUM& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator * (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator * (const NUM& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator / (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator / (const wrapped_number<NUM, LB, UB>& a, const int& b);

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator / (const NUM& a, const wrapped_number<NUM, LB, UB>& b);

template<typename NUM, NUM LB, NUM UB>
std::ostream& operator << (std::ostream& s, const wrapped_number<NUM,LB,UB>& v);

/**
 * @brief a number which has a well defined lower bound and upper bound.
 * 
 * If the number goes over one of the 2 values it immediately be set to either be lowerbound or the upperbound
 * 
 * The template parameters are as follows:
 *  - the first is the underlying number type;
 *  - the second is the lowerbound;
 *  - the third is the upperbound
 */

/**
 * @brief a number which has a lowerbound and upperbound (both included)
 * 
 * @tparam NUM 
 * @tparam LB 
 * @tparam UB 
 */
template<typename NUM, NUM LB, NUM UB>
class wrapped_number {
    friend bool operator == <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend bool operator != <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend bool operator > <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend bool operator >= <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend bool operator < <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend bool operator <= <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend wrapped_number<NUM, LB, UB> operator + <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend wrapped_number<NUM, LB, UB> operator + <>(const NUM& a, const wrapped_number<NUM, LB, UB>& b);
    friend wrapped_number<NUM, LB, UB> operator - <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend wrapped_number<NUM, LB, UB> operator - <>(const NUM& a, const wrapped_number<NUM, LB, UB>& b);
    friend wrapped_number<NUM, LB, UB> operator * <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend wrapped_number<NUM, LB, UB> operator * <>(const NUM& a, const wrapped_number<NUM, LB, UB>& b);
    friend wrapped_number<NUM, LB, UB> operator / <>(const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b);
    friend wrapped_number<NUM, LB, UB> operator / <>(const wrapped_number<NUM, LB, UB>& a, const int& b);
    friend wrapped_number<NUM, LB, UB> operator / <>(const NUM& a, const wrapped_number<NUM, LB, UB>& b);

    friend std::ostream& operator << <>(std::ostream& s, const wrapped_number<NUM,LB,UB>& v);
private:
    NUM value;
public:
    /**
     * @brief create a new number equal to the lowerbound
     * 
     */
    explicit wrapped_number<NUM, LB, UB>() : value{LB} {

    }
    explicit wrapped_number<NUM, LB, UB>(int value) : value{static_cast<NUM>(value)} { 
        this->restrainValue();
    }

    wrapped_number<NUM, LB, UB>(const wrapped_number<NUM, LB, UB>& value) : value{value.value} { 
    }

    explicit wrapped_number<NUM, LB, UB>(wrapped_number<NUM, LB, UB> && other) : value{other.value} { 
    }
    ~wrapped_number() { }
private:
    void restrainValue() {
        if (this->value > UB) {
            this->value = UB;
        } else if (this->value < LB) {
            this->value = LB;
        }
    }
public:
    NUM getUnderlyingValue() const {
        return this->value;
    }
    wrapped_number& operator = (const wrapped_number<NUM, LB, UB>& other) {
        if (this != &other) {
            this->value = other.value;
        }
        return *this;
    }
    
    wrapped_number& operator = (wrapped_number && other) {
        this->value = other.value;
        this->restrainValue();
        return *this;
    }

    wrapped_number& operator += (const NUM other) {
        this->value += value;
        this->restrainValue();
        return *this;
    }
    wrapped_number& operator += (const wrapped_number<NUM, LB, UB>& other) {
        this->value += other.value;
        this->restrainValue();
        return *this;
    }

    wrapped_number& operator -= (const NUM other) {
        this->value -= value;
        this->restrainValue();
        return *this;
    }
    wrapped_number& operator -= (const wrapped_number<NUM, LB, UB>& other) {
        this->value -= other.value;
        this->restrainValue();
        return *this;
    }

    wrapped_number& operator *= (const NUM other) {
        this->value *= value;
        this->restrainValue();
        return *this;
    }
    wrapped_number& operator *= (const wrapped_number<NUM, LB, UB>& other) {
        this->value *= other.value;
        this->restrainValue();
        return *this;
    }

    wrapped_number& operator /= (const NUM other) {
        this->value /= value;
        this->restrainValue();
        return *this;
    }
    wrapped_number& operator /= (const wrapped_number<NUM, LB, UB>& other) {
        this->value /= other.value;
        this->restrainValue();
        return *this;
    }

    wrapped_number operator -() {
        return wrapped_number{-this->value};
    }

    explicit operator int() {
        return static_cast<int>(this->value);
    }

    std::ostream& operator<<(std::ostream& os) {
        os << this->value;
        return os;
    }

};

template<typename NUM, NUM LB, NUM UB>
 bool operator == (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    return a.value == b.value;
}
template<typename NUM, NUM LB, NUM UB>
 bool operator != (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    return a.value != b.value;
}

template<typename NUM, NUM LB, NUM UB>
 bool operator < (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    return a.value < b.value;
}

template<typename NUM, NUM LB, NUM UB>
 bool operator <= (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    return a.value <= b.value;
}

template<typename NUM, NUM LB, NUM UB>
 bool operator > (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    return a.value > b.value;
}

template<typename NUM, NUM LB, NUM UB>
 bool operator >= (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    return a.value >= b.value;
}

template<typename NUM, NUM LB, NUM UB>
 wrapped_number<NUM, LB, UB> operator + (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    wrapped_number<NUM, LB, UB> result{a.value + b.value};
    return result;
}

template<typename NUM, NUM LB, NUM UB>
 wrapped_number<NUM, LB, UB> operator + (const NUM& a, const wrapped_number<NUM, LB, UB>& b) {
    return wrapped_number<NUM, LB, UB>{a + b.value};
}

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator - (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    return wrapped_number<NUM, LB, UB>{a.value - b.value};
}

template<typename NUM, NUM LB, NUM UB>
 wrapped_number<NUM, LB, UB> operator - (const NUM& a, const wrapped_number<NUM, LB, UB>& b) {
    return wrapped_number<NUM, LB, UB>{a - b.value};
}

template<typename NUM, NUM LB, NUM UB>
 wrapped_number<NUM, LB, UB> operator * (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    return wrapped_number<NUM, LB, UB>{a.value * b.value};
}

template<typename NUM, NUM LB, NUM UB>
 wrapped_number<NUM, LB, UB> operator * (const NUM& a, const wrapped_number<NUM, LB, UB>& b) {
    return wrapped_number<NUM, LB, UB>{a * b.value};
}

template<typename NUM, NUM LB, NUM UB>
 wrapped_number<NUM, LB, UB> operator / (const wrapped_number<NUM, LB, UB>& a, const wrapped_number<NUM, LB, UB>& b) {
    return wrapped_number<NUM, LB, UB>{a.value / b.value};
}

template<typename NUM, NUM LB, NUM UB>
wrapped_number<NUM, LB, UB> operator / (const wrapped_number<NUM, LB, UB>& a, const int& b) {
    return wrapped_number<NUM, LB, UB>{a.value / b};
}

template<typename NUM, NUM LB, NUM UB>
 wrapped_number<NUM, LB, UB> operator / (const NUM& a, const wrapped_number<NUM, LB, UB>& b) {
    return wrapped_number<NUM, LB, UB>{a / b.value};
}

template<typename NUM, NUM LB, NUM UB>
std::ostream& operator << (std::ostream& s, const wrapped_number<NUM,LB,UB>& v) {
    s << v.value;
    return s;
}

}
#endif