#ifndef _MATH_HEADER__
#define _MATH_HEADER__

#include <cmath>
#include "log.hpp"

namespace cpp_utils {

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

}

#endif