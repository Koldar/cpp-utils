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
     * Dealing with infinities
     * =======================
     * 
     * Suppose you encounter this operation:
     * 
     * \f$ x = \infty \geq \infty \f$
     * 
     * What is its result? You have 2 roads:
     * @li mathematically this is an undeterminable expression since it's the same of \f$ \infty - \infty \geq 0 \f$
     * @li for some algorithms you might want to consider \f$ \infty \f$ as a real big value. For instance in Dijkstra you need to perform the check the triangular inqeuality
     *  when 2 of the 3 triangular sides are \f$ \infty \f$: in this case it would be best to consider (for algorithm simplicity) infinity as jsut a big value. Hence \f$ \infty \geq \infty \f$ should
     *  be true; Note that even here expressions like \f$ \infty - NUM = \infty \f$. This concepts applies only for comparisons!
     * 
     * Since we cannot decide which operation outcome you want, the library offers you 2 ways to perform operations:
     * 
     * @li **strict mode** where we perform operation using the mathematic point of view: if we have an undeterminable operation, we will generate an error.
     *  This operation is used in every operator overloading;
     * @li **non strict mode** where we perform operation using the algorithmic point of this. here we will consider infinity as a really big number.
     *  This operation can be used in methods like lessThan, greaterThan and so on;
     * 
     * List of **non strict mode**
     * ---------------------------
     * 
     * Ambiguous operations (infinities against infinities) will lead to a runtime error. 
     * @li \f$ \infty \geq \infty  \f$ should be ambiguous but here it lead to `true`;
     * @li \f$ \infty > \infty  \f$ should be ambiguous, but here it lead to `false`;
     * @li \f$ \infty \leq \infty  \f$ should be ambiguous, but here it lead to `true`;
     * @li \f$ \infty < \infty  \f$ should be ambiguous, but here it lead to `false`;
     * @li \f$ \infty = \infty  \f$ should be ambiguous, but here it lead to `true`;
     * @li \f$ \infty \not =  \infty  \f$ should be ambiguous, but here it lead to `false`;
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
        friend struct ::std::hash<safe_inf_uint>;
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

        constexpr safe_inf_uint(const safe_inf_uint& other): value{other.value} {

        }

        constexpr safe_inf_uint(safe_inf_uint&& other): value{other.value} {

        }

        safe_inf_uint& operator = (const safe_inf_uint& other) {
            this->value = other.value;
            return *this;
        }
        safe_inf_uint& operator = (safe_inf_uint&& other) {
            this->value = other.value;
            return *this;
        }

        ~safe_inf_uint() {
            
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
        /**
         * @brief check if \f$ a < b \f$
         * 
         * when playing with algorithms, sometimes it happen you need to compute \f$ +\infty < +\infty \f$.
         * Maths says that this operation is ambiguous, but sometimes you would like to have a result nethertheless. When @c strict is set to @c false \f$ \infty \f$ is treated like a number which you can compare against itself. Hence when @c strict is set to @c false the operation will return @c false (since for every number \f$ x < x \f$ is false.
         * If @c strict is set to @c true we will use the mathematical viewpoint and return an exception, since the operation is indetermined.
         * 
         * @param other the other number to compare
         * @param strict true if you want to use the mathematical definition of tyhe operation for \f$ \infty \f$; false otherwise
         * @return true 
         * @return false 
         */
        bool lessThan(const safe_inf_uint& other, bool strict=false) const {
            if (this->isInfinity() && other.isInfinity()) {
                if (strict) {
                    throw cpp_utils::exceptions::NumericalOperationException{"<", this->value, other.value};
                }
                return false;
            }
            return this->value < other.value;
        }
        /**
         * @brief check if \f$ a \leq b \f$
         * 
         * when playing with algorithms, sometimes it happen you need to compute \f$ +\infty \leq +\infty \f$.
         * Maths says that this operation is ambiguous, but sometimes you would like to have a result nethertheless. When @c strict is set to @c false \f$ \infty \f$ is treated like a number which you can compare against itself. Hence when @c strict is set to @c false the operation will return @c true (since for every number \f$ x \leq x \f$ is true.
         * If @c strict is set to @c true we will use the mathematical viewpoint and return an exception, since the operation is indetermined.
         * 
         * @param other the other number to compare
         * @param strict true if you want to use the mathematical definition of tyhe operation for \f$ \infty \f$; false otherwise
         * @return true 
         * @return false 
         */
        bool lessOrEqualThan(const safe_inf_uint& other, bool strict=false) const {
            if (this->isInfinity() && other.isInfinity()) {
                if (strict) {
                    throw cpp_utils::exceptions::NumericalOperationException{"<=", this->value, other.value};
                }
                return true;
            }
            return this->value <= other.value;
        }
        /**
         * @brief check if \f$ a > b \f$
         * 
         * when playing with algorithms, sometimes it happen you need to compute \f$ +\infty > +\infty \f$.
         * Maths says that this operation is ambiguous, but sometimes you would like to have a result nethertheless. When @c strict is set to @c false \f$ \infty \f$ is treated like a number which you can compare against itself. Hence when @c strict is set to @c false the operation will return @c false (since for every number \f$ x > x \f$ is false.
         * If @c strict is set to @c true we will use the mathematical viewpoint and return an exception, since the operation is indetermined.
         * 
         * @param other the other number to compare
         * @param strict true if you want to use the mathematical definition of tyhe operation for \f$ \infty \f$; false otherwise
         * @return true 
         * @return false 
         */
        bool greaterThan(const safe_inf_uint& other, bool strict=false) const {
            if (this->isInfinity() && other.isInfinity()) {
                if (strict) {
                    throw cpp_utils::exceptions::NumericalOperationException{">", this->value, other.value};
                }
                return false;
            }
            return this->value > other.value;
        }
        /**
         * @brief check if \f$ a \geq b \f$
         * 
         * when playing with algorithms, sometimes it happen you need to compute \f$ +\infty \geq +\infty \f$.
         * Maths says that this operation is ambiguous, but sometimes you would like to have a result nethertheless. When @c strict is set to @c false \f$ \infty \f$ is treated like a number which you can compare against itself. Hence when @c strict is set to @c false the operation will return @c true (since for every number \f$ x \geq x \f$ is true.
         * If @c strict is set to @c true we will use the mathematical viewpoint and return an exception, since the operation is indetermined.
         * 
         * @param other the other number to compare
         * @param strict true if you want to use the mathematical definition of tyhe operation for \f$ \infty \f$; false otherwise
         * @return true 
         * @return false 
         */
        bool greaterOrEqualThan(const safe_inf_uint& other, bool strict=false) const {
            if (this->isInfinity() && other.isInfinity()) {
                if (strict) {
                    throw cpp_utils::exceptions::NumericalOperationException{">=", this->value, other.value};
                }
                return true;
            }
            return this->value >= other.value;
        }
        /**
         * @brief check if \f$ a = b \f$
         * 
         * when playing with algorithms, sometimes it happen you need to compute \f$ +\infty = +\infty \f$.
         * Maths says that this operation is ambiguous, but sometimes you would like to have a result nethertheless. When @c strict is set to @c false \f$ \infty \f$ is treated like a number which you can compare against itself. 
         * Hence when @c strict is set to @c false the operation will return @c true (since for every number \f$ x = x \f$ is true.
         * If @c strict is set to @c true we will use the mathematical viewpoint and return an exception, since the operation is indetermined.
         * 
         * @param other the other number to compare
         * @param strict true if you want to use the mathematical definition of tyhe operation for \f$ \infty \f$; false otherwise
         * @return true 
         * @return false 
         */
        bool equalTo(const safe_inf_uint& other, bool strict = false) const {
            if (this->isInfinity() && other.isInfinity()) {
                if (strict) {
                    throw cpp_utils::exceptions::NumericalOperationException{"=", this->value, other.value};
                }
                return true;
            }
            return this->value == other.value;
        }
        /**
         * @brief check if \f$ a \not = b \f$
         * 
         * when playing with algorithms, sometimes it happen you need to compute \f$ +\infty \not = +\infty \f$.
         * Maths says that this operation is ambiguous, but sometimes you would like to have a result nethertheless. When @c strict is set to @c false \f$ \infty \f$ is treated like a number which you can compare against itself. 
         * Hence when @c strict is set to @c false the operation will return @c false (since for every number \f$ x \not = x \f$ is false.
         * If @c strict is set to @c true we will use the mathematical viewpoint and return an exception, since the operation is indetermined.
         * 
         * @param other the other number to compare
         * @param strict true if you want to use the mathematical definition of tyhe operation for \f$ \infty \f$; false otherwise
         * @return true 
         * @return false 
         */
        bool notEqualTo(const safe_inf_uint& other, bool strict=false) const {
            if (this->isInfinity() && other.isInfinity()) {
                if (strict) {
                    throw cpp_utils::exceptions::NumericalOperationException{"!=", this->value, other.value};
                }
                return false;
            }
            return this->value != other.value;
        }
        /**
         * @brief true if the number is infinity
         * 
         * @return true 
         * @return false 
         */
        bool isInfinity() const {
            return this->value == UINT64_MAX;
        }
        /**
         * @brief true if the number is not infinity
         * 
         * @return true 
         * @return false 
         */
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

namespace std {

    template <>
    struct hash<cpp_utils::safe_inf_uint> {
        size_t operator() (const cpp_utils::safe_inf_uint& e) const {
            size_t seed = 0;
            boost::hash_combine(seed, e.value);
            return seed;
        }
    };
}

//TODO when you use boost_combine in hash, it implicitly call struct hash, but not from std, but in boost namespace!
//even worse, i still need the std::hash inside for the normal std::unordered_set... For now I need to duplicte the methods... ugly i know
namespace boost {

    template <>
    struct hash<cpp_utils::safe_inf_uint> {
        size_t operator() (const cpp_utils::safe_inf_uint& e) const {
            return ::std::hash<cpp_utils::safe_inf_uint>{}(e);
        }
    };
}

#endif