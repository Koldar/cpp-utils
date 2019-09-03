#ifndef _SAFE_UINT_HEADER__
#define _SAFE_UINT_HEADER__

#include <cstdint>
#include <iostream>
#include "exceptions.hpp"
#include "log.hpp"

namespace cpp_utils {

struct safe_inf_uint;

safe_inf_uint operator +(const safe_inf_uint& a, const safe_inf_uint& b);
safe_inf_uint operator -(const safe_inf_uint& a, const safe_inf_uint& b);
safe_inf_uint operator *(const safe_inf_uint& a, const safe_inf_uint& b);
safe_inf_uint operator /(const safe_inf_uint& a, const safe_inf_uint& b);

bool operator <(const safe_inf_uint& a, const safe_inf_uint& b);
bool operator <=(const safe_inf_uint& a, const safe_inf_uint& b);
bool operator >(const safe_inf_uint& a, const safe_inf_uint& b);
bool operator >=(const safe_inf_uint& a, const safe_inf_uint& b);
bool operator !=(const safe_inf_uint& a, const safe_inf_uint& b);
bool operator ==(const safe_inf_uint& a, const safe_inf_uint& b);

/**
 * @brief an unsigned integer 64 bit whose minimum value is 0 and whose maximum value is infinity.
 * 
 * if an overflow or an underflow is detected the value is automatically trimmed to the border value.
 * Once the value is set to infinity, all operations will treat an operator as inifity (hence infinity minus something is still infinity).
 * 
 * Ambiguous operations will lead to a runtime error.
 * 
 * @note
 * I wanted to code this struct because this is normally the base case when dealing with A* search (f, g and h usually may be infinite, but never be below 0)
 * 
 * In this structure, all constructor can be called by the compiler "implicitly" allowing it to promote normal number to safe_inf_uint.
 * However, the developer will need to explicitly cast `safe_inf_uint` to normal number.
 * This is by design because I think you should use this structure as much as possible.
 * 
 * 
 */
struct safe_inf_uint {
    friend safe_inf_uint operator +(const safe_inf_uint& a, const safe_inf_uint& b);
    friend safe_inf_uint operator -(const safe_inf_uint& a, const safe_inf_uint& b);
    friend safe_inf_uint operator *(const safe_inf_uint& a, const safe_inf_uint& b);
    friend safe_inf_uint operator /(const safe_inf_uint& a, const safe_inf_uint& b);

    friend bool operator <(const safe_inf_uint& a, const safe_inf_uint& b);
    friend bool operator <=(const safe_inf_uint& a, const safe_inf_uint& b);
    friend bool operator >(const safe_inf_uint& a, const safe_inf_uint& b);
    friend bool operator >=(const safe_inf_uint& a, const safe_inf_uint& b);
    friend bool operator !=(const safe_inf_uint& a, const safe_inf_uint& b);
    /**
     * @brief check if 2 nuymbers are the same
     * 
     * the operation is defined even with inifities: 2 numbers are the same when they are both the same values or they are both infinity
     * 
     * @param a first value
     * @return true 
     * @param b second value
     * @return false 
     */
    friend bool operator ==(const safe_inf_uint& a, const safe_inf_uint& b);

    friend std::ostream& operator <<(std::ostream& out, const safe_inf_uint& n);
public:
    static safe_inf_uint const ZERO;
    static safe_inf_uint const MIN;
    static safe_inf_uint const MAX;
    static safe_inf_uint const INFTY;
private:
    uint64_t value;

public:
    constexpr safe_inf_uint(): value{0UL} {
    }

    constexpr safe_inf_uint(const safe_inf_uint& other): value{other.value} {
    }

    safe_inf_uint(int n): value{static_cast<uint64_t>(n)} {
        if (n < 0) {
            throw cpp_utils::exceptions::NumericalOperationException{"cast from int to unsigned", n};
        }
    }
    safe_inf_uint(long n): value{static_cast<uint64_t>(n)} {
        if (n < 0) {
            throw cpp_utils::exceptions::NumericalOperationException{"cast from long to unsigned", n};
        }
    }

    constexpr safe_inf_uint(unsigned int n): value{n} {
    }
    constexpr safe_inf_uint(unsigned long n): value{n} {
    }
    safe_inf_uint(float n): value{static_cast<uint64_t>(n)} {
        if (n < 0) {
            throw cpp_utils::exceptions::NumericalOperationException{"cast from float to unsigned", n};
        }
        if (n > UINT64_MAX) {
            throw cpp_utils::exceptions::NumericalOperationException{"cast from float(overflow) to unsigned", n};
        }
    }
    safe_inf_uint(double n): value{static_cast<uint64_t>(n)} {
        if (n < 0) {
            throw cpp_utils::exceptions::NumericalOperationException{"cast from double to unsigned", n};
        }
        if (n > UINT64_MAX) {
            throw cpp_utils::exceptions::NumericalOperationException{"cast from double(overflow) to unsigned", n};
        }
    }

public:
    inline static safe_inf_uint zero() {
        return ZERO;
    }
    inline static safe_inf_uint min() {
        return MIN;
    }
    inline static safe_inf_uint max() {
        return MAX;
    }
    inline static safe_inf_uint infinity() {
        return INFTY;
    }

public:
    bool isInfinity() const {
        return this->value == UINT64_MAX;
    }
    bool isNotInfinity() const {
        return this->value != UINT64_MAX;
    }
    safe_inf_uint& operator +=(const safe_inf_uint& b) {
        safe_inf_uint result{};
        if (__builtin_add_overflow(this->value, b.value, &result.value)) {
            //overflow set to infinity
            this->setToInfinity();
        } else {
            this->value = result.value;
        }
        return *this;
    }
    safe_inf_uint& operator -=(const safe_inf_uint& b) {
        if (this->isInfinity()) {
            if (b.isInfinity()) {
                //ambiguous operation
                throw cpp_utils::exceptions::NumericalOperationException{"-", this->value, b.value};
            } else {
                this->setToInfinity();
            }
        } else {
            if (b.isInfinity()) {
                //it should go to -inf, but we cannot go there
                throw cpp_utils::exceptions::NumericalOperationException{"-", this->value, b.value};
            } else {
                safe_inf_uint result{};
                if (__builtin_sub_overflow(this->value, b.value, &result.value)) {
                    //underflow set to infinity
                    this->setToInfinity();
                }
                this->value = result.value;
            }
        }
        return *this;
    }
    safe_inf_uint& operator *=(const safe_inf_uint& b) {
        safe_inf_uint result{};
        debug("self is", *this, "while b is", b);
        if (__builtin_mul_overflow(this->value, b.value, &result.value)) {
            //overflow set to infinity
            this->setToInfinity();
        } else {
            this->value = result.value;
        }
        return *this;
    }
    safe_inf_uint& operator /=(const safe_inf_uint& b) {
        if (this->isInfinity()) {
            if (b.isInfinity()) {
                throw cpp_utils::exceptions::NumericalOperationException{"/", this->value, b.value};
            }
            this->setToInfinity();
        } else if (b.isInfinity()) {
            this->value = 0UL;
        } else {
            this->value /= b.value;
        }
        return *this;
    }
private:
    void setToInfinity() {
        this->value = UINT64_MAX;
    }

public:
    explicit operator int() {
        return (int)this->value;
    }
    explicit operator long() {
        return (long)this->value;
    }
    explicit operator float() {
        return (float)this->value;
    }
    explicit operator double() {
        return (double)this->value;
    }
    explicit operator unsigned int() {
        return (unsigned int)this->value;
    }
    explicit operator unsigned long() {
        return (unsigned long)this->value;
    }
};

}

#endif