#ifndef _MATH_HEADER__
#define _MATH_HEADER__

#include <cmath>
#include "log.hpp"

namespace cpp_utils {

    /**
     * @brief parse a number from a string
     * 
     * @tparam T the type of the number to parse
     * @param s the string representing a number
     * @return T the parsed number
     */
    template <typename T>
    T parseFromString(const std::string& s) {
  
        // object from the class stringstream 
        std::stringstream converter(s); 
    
        // The object has the value 12345 and stream 
        // it to the integer x 
        T x; 
        converter >> x; 
        return x; 
    }

    /**
     * @brief parse a number from a string
     * 
     * @tparam T the type of the number to parse
     * @param s the string representing a number
     * @return T the parsed number
     */
    template <typename T>
    T parseFromString(const char* s) {
        return parseFromString<T>(std::string{s});
    }

    /**
     * @brief check if 2 decimal numbers are more or less equal
     * 
     * @tparam T either float or double
     * @param a first number to check
     * @param b second number to check
     * @param epsilon threshold of equality. (e.g., 0.001, 0.0001). If the difference between the 2 numbers is less than the threshold, the 2 numbers are equal
     * @return true if `a` is the same of `b`
     * @return false otherwise
     */
    template <typename T>
    bool isApproximatelyEqual(T a, T b, T epsilon) {
        //debug(std::abs(a - b), "<=", (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)), "*", epsilon, "=", ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon));
        return std::abs(a - b) <= ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
    }

    /**
     * @brief check if 2 decimal numbers are equal. Use together with ::isApproximatelyEqual
     * 
     * @tparam T either float or double
     * @param a first number to check
     * @param b second number to check
     * @param epsilon threshold of equality. (e.g., 0.001, 0.0001). If the difference between the 2 numbers is less than the threshold, the 2 numbers are equal
     * @return true if `a` is the same of `b`
     * @return false otherwise
     */
    template <typename T>
    bool isEssentiallyEqual(T a, T b, T epsilon) {
        return std::abs(a - b) <= ( (std::abs(a) > std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
    }

    /**
     * @brief check if one number is for sure greater than another one.
     * 
     * @tparam T either float or double
     * @param a first number to check
     * @param b second number to check
     * @param epsilon threshold of equality. (e.g., 0.001, 0.0001).
     * @return true if `a > b`
     * @return false otherwise
     */
    template <typename T>
    bool isDefinitelyGreaterThan(T a, T b, T epsilon) {
        return (a - b) > ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
    }

    /**
     * @brief check if one number is for sure less than another one.
     * 
     * @tparam T either float or double
     * @param a first number to check
     * @param b second number to check
     * @param epsilon threshold of equality. (e.g., 0.001, 0.0001).
     * @return true if `a < b`
     * @return false otherwise
     */
    template <typename T>
    bool isDefinitelyLessThan(T a, T b, T epsilon) {
        return (b - a) > ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
    }

    /**
     * @brief compute log 2
     * 
     * @note
     * this function allows for computation compile time
     * 
     * @tparam T type of both the input and the output
     * @param n input
     * @return constexpr T \f$log_{2}(n)\f$
     */
    template <typename T>
    constexpr T log2(T n) {
        return (n<2) ? 0 : 1 + log2(n/2);
    }

    template <typename T>
    constexpr T _Pow2GreaterThan(T n, T power) {
        return (power >= n) ? power : _Pow2GreaterThan(n, 2*power);
    }

    /**
     * @brief retrieve the smallest power of 2 which is greater or equal than the given number
     * 
     * @code
     *  ceilPow(1) //1
     *  ceilPow(2) //2
     *  ceilPow(3) //4
     *  ceilPow(4) //4
     *  ceilPow(5) //8
     * @endcode
     * 
     * @tparam T type of the number
     * @param n the number involved
     * @return constexpr T 
     */
    template <typename T>
    constexpr T pow2GreaterThan(T n) {
        return n == 0 ? 0 : _Pow2GreaterThan(n, 1);
    }

    template <typename T>
    bool isDecimal(const T& n) {
        return isApproximatelyEqual(std::fmod(n, 1.0), 0.0, 1e-3);
    }

    template<int>
    bool isDecimal(const int& n) {
        return true;
    }

    template<unsigned int>
    bool isDecimal(const unsigned int& n) {
        return true;
    }

    template<long>
    bool isDecimal(const long& n) {
        return true;
    }

    template<unsigned long>
    bool isDecimal(const unsigned long& n) {
        return true;
    }

    

}

#endif