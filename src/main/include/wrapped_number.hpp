#ifndef _CPPUTILS_WRAPPEDNUMBER_HEADER__
#define _CPPUTILS_WRAPPEDNUMBER_HEADER__

#include <iostream>
#include "log.hpp"

namespace cpp_utils {

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
    template<typename NUM, NUM LB, NUM UB, typename TONUM>
    class wrapped_number {
    public:
        using This = wrapped_number<NUM, LB, UB, TONUM>;
    private:
        NUM value;
    public:
        /**
         * @brief create a new number equal to the lowerbound
         * 
         */
        wrapped_number() : value{LB} {

        }

        wrapped_number(const NUM& value) : value{value} { 
            this->restrainValue();
        }

        wrapped_number(const This& value) : value{value.value} { 
        }

        wrapped_number(This&& value) : value{std::move(value.value)} { 
        }

        This& operator = (const This& o) {
            this->value = o.value;
            return *this;
        }

        This& operator = (This&& o) {
            this->value = std::move(o.value);
            return *this;
        }

        virtual ~wrapped_number() {

        }
    private:
        /**
         * @brief bound the value within the given internal
         * 
         */
        void restrainValue() {
            if (this->value > UB) {
                this->value = UB;
            } else if (this->value < LB) {
                this->value = LB;
            }
        }
    public:
        /**
         * @brief yield a copy of the underying value
         * 
         * @return NUM 
         */
        NUM getUnderlyingValue() const {
            return this->value;
        }

        This& operator += (const This& other) {
            this->value += other.value;
            this->restrainValue();
            return *this;
        }
        This& operator -= (const This& other) {
            this->value -= other.value;
            this->restrainValue();
            return *this;
        }

        This& operator *= (const This& other) {
            this->value *= other.value;
            this->restrainValue();
            return *this;
        }

        This& operator *= (double other) {
            this->value = static_cast<NUM>(static_cast<double>(this->value) * other);
            this->restrainValue();
            return *this;
        }

        This& operator /= (const This& other) {
            this->value /= other.value;
            this->restrainValue();
            return *this;
        }

        This& operator /= (double other) {
            this->value = static_cast<NUM>(static_cast<double>(this->value) / other);
            this->restrainValue();
            return *this;
        }

        wrapped_number operator -() {
            return wrapped_number{-this->value};
        }

        explicit operator TONUM() const {
            return static_cast<TONUM>(this->value);
        }
    public:
        friend bool operator ==(const This& a, const This& b) {
            return a.value == b.value;
        }
        friend bool operator !=(const This& a, const This& b) {
            return a.value != b.value;
        }
        friend bool operator <(const This& a, const This& b) {
            return a.value < b.value;
        }
        friend bool operator <=(const This& a, const This& b) {
            return a.value <= b.value;
        }
        friend bool operator >(const This& a, const This& b) {
            return a.value > b.value;
        }
        friend bool operator >=(const This& a, const This& b) {
            return a.value >= b.value;
        }
        friend std::ostream& operator <<(std::ostream& out, const This& a) {
            return out << static_cast<TONUM>(a);
        }
        friend This operator +(const This& a, const This& b) {
            auto result{a};
            return result += b;
        }
        friend This operator -(const This& a, const This& b) {
            auto result{a};
            return result -= b;
        }
        friend This operator *(const This& a, const This& b) {
            auto result{a};
            return result *= b;
        }
        friend This operator *(double a, const This& b) {
            auto result{b};
            return result *= a;
        }
        friend This operator *(const This& a, double b) {
            auto result{a};
            return result *= b;
        }
        friend This operator /(const This& a, const This& b) {
            auto result{a};
            return result /= b;
        }
        friend This operator /(double a, const This& b) {
            return This{static_cast<NUM>(a/static_cast<double>(b.value))};
        }
        friend This operator /(const This& a, double b) {
            auto result{a};
            return result /= b;
        }
    };

}
#endif