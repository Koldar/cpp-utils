#ifndef _MATH_HEADER__
#define _MATH_HEADER__

#include <cmath>
#include "exceptions.hpp"
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

    /**
     * @brief converts a decimal number into a pair of numerator and denominator
     * 
     * The function will generate ratios where the denominator is a power of 10.
     * 
     * @tparam T the type of the decimale number. Ususally either `double` or `float`
     * @tparam OUT the type of the numerator and denominator
     * @param decimal the number to convert
     * @param numerator the number which  will represents the numerator
     * @param denominator the number which  will represents the denominator
     * @param epsilon threshold of accuracy for the decimal value
     * @param limit tries to perform before giving up the conversion. Ususally the module of the exponent of `epsilon`. Needs to be > 0
     */
    template <typename T, typename OUT>
    void getRatioOf(T decimal, OUT& numerator, OUT& denominator, T epsilon, int limit) {
        denominator = 1;
        T denominatorT = 1;

        while (true) {
            if (limit == 0) {
                throw cpp_utils::exceptions::makeInvalidArgumentException("cannot convert fraction", decimal, "into numerator and denominator", decimal, ". epsilon=", epsilon, "limit", limit);
            }
            limit -= 1;

            numerator = static_cast<OUT>(decimal * static_cast<T>(denominator));
            T numeratorT = decimal * denominatorT;

            T numeratorTimesDivided10 = (static_cast<T>(10.) * numeratorT)/(static_cast<T>(10.));
            critical("decimal=", decimal, "numerator=", numerator, "denominator=", denominator, "numeratorT=", numeratorT, "denominatorT=", denominatorT, "numeratorTimesDivided10=", numeratorTimesDivided10, "limit=", limit, "epsilon=", epsilon);
            if (isApproximatelyEqual(numeratorTimesDivided10, static_cast<T>(numerator), epsilon)) {
                //only an integer number would return the same value
                return;
            } else {
                //increase the numerator and denominator
                denominator *= 10;
                denominatorT *= 10;
            }
        }
    }

    namespace internal {

        template<typename T>
        constexpr int _argmin2(int minimumIndex, int nIndex, const T& minimum) {
            return minimumIndex;
        }

        template<typename T, typename... NUMS>
        constexpr int _argmin2(int minimumIndex, int nIndex, const T& minimum, const T& n, const NUMS&... args) {
            return (n < minimum)
                ? _argmin2(nIndex, nIndex + 1, n, args...)
                : _argmin2(minimumIndex, nIndex + 1, minimum, args...)
            ;
        }

        template<typename T, typename... NUMS>
        constexpr int _argmin1(const T& first, const NUMS&... args) {
            return _argmin2(0, 1, first, args...);
        }

        template<typename T>
        constexpr int _argmax2(int maximumIndex, int nIndex, const T& maximum) {
            return maximumIndex;
        }

        template<typename T, typename... NUMS>
        constexpr int _argmax2(int maximumIndex, int nIndex, const T& maximum, const T& n, const NUMS&... args) {
            return (n > maximum)
                ? _argmax2(nIndex, nIndex + 1, n, args...)
                : _argmax2(maximumIndex, nIndex + 1, maximum, args...)
                ;
        }

        template<typename T, typename... NUMS>
        constexpr int _argmax1(const T& first, const NUMS&... args) {
            return _argmax2(0, 1, first, args...);
        }

    }

    /**
     * @brief computes the argmin of a sequence of values
     * 
     * the argmin is the index of the element which is the minimum
     * 
     * @note
     * if multiple values are the minimum, we will return the index of the first one
     * 
     * @tparam NUMS 
     * @param args 
     * @return constexpr int 
     */
    template<typename... NUMS>
    constexpr int argmin(const NUMS&... args) {
        return internal::_argmin1(args...);
    }

    template <typename T>
    constexpr int argmin(const T& num) {
        return 0;
    }

    /**
     * @brief computes the argmax of a sequence of values
     * 
     * the argmax is the index of the element which is the maximum
     * 
     * @note
     * if multiple values are the maximum, we will return the index of the first one
     * 
     * @tparam NUMS 
     * @param args 
     * @return constexpr int 
     */
    template<typename... NUMS>
    constexpr int argmax(const NUMS&... args) {
        return internal::_argmax1(args...);
    }

    template<typename T>
    constexpr int argmax(const T& num) {
        return 0;
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

    /**
     * @brief compute a value when it is inside a ring bound.
     * 
     * The ring has as lowerbound 0 and as excluded upperbound @c ub
     * 
     * @code
     * ringBound(5, 10) // 5
     * ringBound(0, 10) // 0
     * ringBound(10, 10) // 0
     * ringBound(-1, 10) // 9
     * ringBound(11, 10) // 1
     * @endcode
     * 
     * @tparam T integer type (like int)
     * @param val the value to ringBound
     * @param ub the (excluded) upperbound of the interval
     * @return T value inside the bound
     */
    template <typename T>
    constexpr T ringBound(T val, const T& ub) {
        return  (val < 0) ? ringBound(ub + val, ub) : 
                (val >= ub) ? val % ub :
                val
                ;
    }

    /**
     * @brief like ::ringBound but with a lowerbound which is not 0
     * 
     * 
     * @code
     * ringBound(5, 2, 10) // 5
     * ringBound(2, 2, 10) // 2
     * ringBound(0, 2, 10) // 8
     * @endcode
     * 
     * @tparam T integer type (like int)
     * @param val the value to ringBound
     * @param ub the (excluded) upperbound of the interval
     * @return T value inside the bound
     */
    template <typename T>
    constexpr T ringBound(T val, const T& lb, const T& ub) {
        return lb + ringBound(val - lb, ub - lb);
    }

}

#endif