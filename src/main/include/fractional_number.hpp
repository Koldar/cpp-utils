#ifndef _CPP_UTILS_FRACTIONAL_NUMBER_HEADER__
#define _CPP_UTILS_FRACTIONAL_NUMBER_HEADER__

#include "imemory.hpp"
#include "math.hpp"
#include <iostream>

namespace cpp_utils {

    /**
     * @brief class representing a decimal number as a fraction
     * 
     * Some algorithms (like WA*) requires a double parameter. This parameter allows you to tune the algorithm.
     * However, floating point operations are slow so you would prefer to use integers for such operations.
     * 
     * Consider WA*, whose f(n) can be computed as:
     * 
     * ```
     * f(n) = g(n) + w * h(n)
     * ```
     * 
     * if f(n), g(n) and h(n) are int, you would like to have w as int as well. Suppose w is `1.1`. You can view 1.1 as 11/10, and compute
     * the whole equation as:
     * 
     * f(n) = g(n)  + 11/10 * h(n) => 10 * f(n) = 10*g(n) + 11 h(n)
     * 
     * The equation is the same but now it doesn't require any floating point operation!
     * This class view your number as a pair fo 2 values, numerator and denominator
     * 
     * @tparam TYPE the type of both numerator and denominator
     */
    template <typename TYPE>
    class fractional_number: public MemoryConsumption {
    public:
        typedef fractional_number<TYPE> This;
    private:
        TYPE numerator;
        TYPE denominator;
    public:
        explicit fractional_number(const TYPE& num, const TYPE& den): numerator{num}, denominator{den} {

        }
        explicit fractional_number(double number, double epsilon): numerator{}, denominator{} {
            cpp_utils::getRatioOf(number, this->numerator, this->denominator, epsilon, 10);
        }
        explicit fractional_number(float number, float epsilon): numerator{}, denominator{} {
            cpp_utils::getRatioOf(number, this->numerator, this->denominator, epsilon, 10);
        }
        fractional_number(const This& o): numerator{o.numerator}, denominator{o.denominator} {

        }
        virtual ~fractional_number() {

        }
        This& operator = (const This& o) {
            this->numerator = o.numerator;
            this->denominator = o.denominator;
            return *this;
        }
        This& operator=(This&& o) {
            this->numerator = std::move(o.numerator);
            this->numerator = std::move(o.denominator);
            return *this;
        }
    public:
        explicit operator double() {
            return static_cast<double>(this->numerator)/static_cast<double>(this->denominator);
        }
        explicit operator float() {
            return static_cast<float>(this->numerator)/static_cast<float>(this->denominator);
        }
    public:
        TYPE getNumerator() const {
            return this->numerator;
        }
        TYPE getDenominator() const {
            return this->denominator;
        }
        double getRatio() const {
            return static_cast<double>(this->numerator)/static_cast<double>(this->denominator);
        }
    public:
        friend std::ostream& operator <<(std::ostream& out, const This& num) {
            return out << num.numerator << "/" << num.denominator;
        }
        friend bool operator ==(const This& a, const This& b) {
            return a.numerator == b.numerator && a.denominator == b.denominator;
        }
        friend bool operator !=(const This& a, const This& b) {
            return a.numerator != b.numerator || a.denominator != b.denominator;
        }
    };

}

#endif