#ifndef _MATH_HEADER__
#define _MATH_HEADER__

#include <cmath>

#include <boost/math/special_functions/relative_difference.hpp>

#include "exceptions.hpp"
#include "log.hpp"

namespace cpp_utils {

    /**
     * @brief pi value
     * 
     * @return constexpr double 
     */
    constexpr double pi() {
        return std::atan(1)*4;
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

    template <typename NUM>
    constexpr NUM abs(const NUM& n) {
        return n >= 0 ? n : -n;
    }

    namespace internal {

        template <typename NUM, typename ...OTHER>
        constexpr NUM _min2(const NUM& min, const NUM& f, const OTHER&... others) {
            return _min2(f < min ? f : min, others...);
        }

        template <typename NUM>
        constexpr NUM _min2(const NUM& min, const NUM& f) {
            return f < min ? f : min;
        }

        template <typename NUM, typename ...OTHER>
        constexpr NUM _max2(const NUM& max, const NUM& f, const OTHER&... others) {
            return _max2(f > max ? f : max, others...);
        }

        template <typename NUM>
        constexpr NUM _max2(const NUM& max, const NUM& f) {
            return f > max ? f : max;
        }

    }

    template <typename NUM, typename ...OTHER>
    constexpr NUM min(const NUM& f, const OTHER&... n) {
        return internal::_min2(f, n...);
    }

    template <typename NUM, typename ...OTHER>
    constexpr NUM max(const NUM& f, const OTHER&... n) {
        return internal::_max2(f, n...);
    }

    /**
     * @brief compute the number of digits a number has
     * 
     * @note
     *  examples if `countZero` is false
     * @code
     *  getIntegerDigits(0); //0
     *  getIntegerDigits(1); //1
     *  getIntegerDigits(11); //2
     *  getIntegerDigits(0.3); //0
     *  getIntegerDigits(1.3); //1
     *  getIntegerDigits(11.3); //2
     * @endcode
     * 
     * @tparam NUM type of the number involved
     * @param n nuymber involved
     * @param countZero if true, values like 0.3 will return 1, since "0" is treated as a normal digit. If false we will return 0 since "0" is not trated as a digit.
     * @return constexpr int number of digit before the decimal part
     */
    template <typename NUM>
    constexpr int getIntegerDigits(const NUM& n, bool countZero = true) {
        debug("n=", n);
        if (n < 0) {
            return getIntegerDigits(-n, countZero);
        }

        if (n < 1) {
            return countZero ? 1 : 0;
        }
        return static_cast<int>(floor(log10(n))) + 1;    
    }

    

    // template <typename NUM>
    // NUM pow10(int a) {
    //     NUM n{1};
    //     return internal::_pow10(n, a);
    // }

    template <typename NUM>
    NUM pow10(int a) {
        static double pow[] = {
            1e-20, 1e-19, 1e-18, 1e-17, 1e-16, 
            1e-15, 1e-14, 1e-13, 1e-12, 1e-11,
            1e-10, 1e-9, 1e-8, 1e-7, 1e-6, 
            1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 
            1,
            1e+1, 1e+2, 1e+3, 1e+4, 1e+5, 
            1e+6, 1e+7, 1e+8, 1e+9, 1e+10, 
            1e+11, 1e+12, 1e+13, 1e+14, 1e+15, 
            1e+16, 1e+17, 1e+18, 1e+19, 1e+20, 
        };
        debug("a=", a);
        assert(a >= -20 && a <= +20);
        return static_cast<NUM>(pow[20 + a]);
    }

    /**
     * @brief get angular coefficient of a line going through 2 points
     * 
     * The linear equation is:
     * ```
     * y = mx + q
     * ```
     * 
     * @pre
     *  @li \f$ xa -xb \not = 0 \f$;
     * 
     * @tparam NUM type fo the points
     * @param xa x of the first point
     * @param ya y of the first point
     * @param xb x of the second point
     * @param yb y of the second point
     * @return m
     */
    template <typename NUM1, typename NUM2, typename NUM3, typename NUM4>
    constexpr double getM(const NUM1& xa, const NUM2& ya, const NUM3& xb, const NUM4& yb) {
        return (static_cast<double>(ya) - static_cast<double>(yb)) / (static_cast<double>(xa) - static_cast<double>(xb));
    }

    /**
     * @brief get q of a line going through 2 points
     * 
     * The linear equation is:
     * ```
     * y = mx + q
     * ```
     * 
     * @pre
     *  @li \f$ xa -xb \not = 0 \f$;
     * 
     * @tparam NUM type fo the points
     * @param xa x of the first point
     * @param ya y of the first point
     * @param xb x of the second point
     * @param yb y of the second point
     * @return q
     */
    template <typename NUM1, typename NUM2, typename NUM3, typename NUM4>
    constexpr double getQ(const NUM1& xa, const NUM2& ya, const NUM3& xb, const NUM4& yb) {
        return static_cast<double>(ya) - getM(xa, ya, xb, yb) * static_cast<double>(xa);
    }

    /**
     * @brief get q of a line going through 2 points
     * 
     * The linear equation is:
     * ```
     * y = mx + q
     * ```
     * 
     * @tparam NUM type fo the points
     * @param xa x of the first point
     * @param ya y of the first point
     * @param m angular coefficient of the line
     * @return q
     */
    template <typename NUM1, typename NUM2, typename NUM3>
    constexpr double getQ(const NUM1& xa, const NUM2& ya, const NUM3& m) {
        return static_cast<double>(ya) - static_cast<double>(m)*static_cast<double>(xa);
    }

    /**
     * @brief transform a value linearly
     * 
     * The transformation follows the following equation:
     * ```
     * y = mx + q
     * ```
     * 
     * @param x the value to transform
     * @param m angular coefficient of the line
     * @param q y of the point in x=0
     * @return y transformed value
     */
    template <typename NUM1, typename NUM2, typename NUM3>
    constexpr double linearTransform(const NUM1& x, const NUM2& m, const NUM3& q) {
        return static_cast<double>(m)*static_cast<double>(x) + static_cast<double>(q);
    }

    /**
     * @brief transform a value linearly
     * 
     * The transformation follows the following equation:
     * ```
     * y = mx + q
     * ```
     * 
     * @param x the value to transform
     * @param xa x of the first point the line go through
     * @param ya y of the first point the line go through
     * @param xb x of the second point the line go through
     * @param yb y of the second point the line go through
     * @return y
     */
    template <typename NUM1, typename NUM2, typename NUM3, typename NUM4, typename NUM5>
    constexpr double linearTransform(const NUM1& x, const NUM2& xa, const NUM3& ya, const NUM4& xb, const NUM5& yb) {
        auto m = getM(xa, ya, xb, yb);
        auto q = getQ(xa, ya, m);
        return linearTransform(x, m, q);
    }

    /**
     * @brief get a function tha monotonically crescent. Starts from 0 up till 1
     * 
     * The function will yield values monotonically crescent in a "smooth way"
     * 
     * @note
     * implementationwise, it uses atan function
     * 
     * @pre
     *  @li \f$ x > 0 \f$;
     * 
     * @param x value
     * @return a monotonically crescent value
     */
    template <typename NUM>
    constexpr double getMonotonicallyCrescent(const NUM& x) {
        return (2./cpp_utils::pi()) * std::atan(static_cast<double>(x));
    }

    /**
     * @brief get a function tha monotonically crescent. Starts from `minY` up till `maxY`
     * 
     * The function will yield values monotonically crescent in a "smooth way"
     * 
     * @note
     * implementationwise, it uses atan function
     * 
     * @pre
     *  @li \f$ x > 0 \f$;
     * 
     * @param x value
     * @param minY the minimum value the function can yield
     * @param maxY the maximum value the function can yield
     * @return a monotonically crescent value
     */
    template <typename NUM1, typename NUM2, typename NUM3>
    constexpr double getMonotonicallyCrescent(const NUM1& x, const NUM2& minY, const NUM3& maxY) {
        return static_cast<double>(minY) + (static_cast<double>(maxY) - static_cast<double>(minY)) * getMonotonicallyCrescent(x);
    }

    /**
     * @brief get a function tha monotonically crescent. Starts from `minY` up till `maxY`
     * 
     * The function will yield values monotonically crescent in a "smooth way"
     * 
     * @note
     * implementationwise, it uses atan function
     * 
     * @pre
     *  @li \f$ x \in [minX, maxX]\f$;
     *  @li  \f$ ratio > 0 \f$;
     * 
     * @param x value
     * @param ratio a number allowing you to determine how fast the function monotonically increment. 1 for normal increment. Values greater than 1 means that the function reaches maxY faster w.r.t of the same @c x. Values smaller than 0 means that the function reaches maxY slower w.r.t the same @c c.
     * @param minX the minimum value @c x can have. if \f$ x = minX \f$, the functon yields @c minY. If \f$x = maxX \f$, the function yields @c maxY.
     * @param maxX the maximum value @c x can have
     * @param minY the minimum value the function can yield
     * @param maxY the maximum value the function can yield
     * @return a monotonically crescent value
     */
    template <typename NUM1, typename NUM2, typename NUM3, typename NUM4, typename NUM5, typename NUM6>
    constexpr double getMonotonicallyCrescent(const NUM1& x, const NUM2& ratio, const NUM3& minX, const NUM4& maxX, const NUM5& minY, const NUM6& maxY) {
        //we map the x from [minX, maxX] to [0,10000], since atan(0) = 0 and atan(1000) is about 1
        auto bigN = 10000.;
        auto m = getM(minX, 0., maxX, bigN);
        auto q = getQ(minX, 0., m);
        auto newX = linearTransform(x, m, q);
        debug("m=", m, "q=", q, "x=", x, "newX=", newX);
        return getMonotonicallyCrescent(ratio * newX, minY, maxY);
    }

    /**
     * @brief like ::getMonotonicallyCrescent but instead of repeatadly computing the same operation to fetch m and q, the developers gives them in input
     * 
     * @code
     *  minX = 5;
     *  minY = 2;
     *  maxX = 10;
     *  maxY = 3;
     * auto m = getM(minX, minY, maxX, maxY);
     * auto q = getQ(minX, minY, m);
     * //call several time the function
     * getMonotonicallyCrescent(x1, ratio1, m, q, minY, maxY);
     * getMonotonicallyCrescent(x2, ratio2, m, q, minY, maxY);
     * getMonotonicallyCrescent(x3, ratio3, m, q, minY, maxY);
     * getMonotonicallyCrescent(x4, ratio4, m, q, minY, maxY);
     * @endcode
     * 
     * @param x the number in input to compute a monotonically increase number
     * @param ratio a number allowing you to determine how fast the function monotonically increment. 1 for normal increment. Values greater than 1 means that the function reaches maxY faster w.r.t of the same @c x. Values smaller than 0 means that the function reaches maxY slower w.r.t the same @c c.
     * @param m angular coefficient computed previously. 
     * @param q y-value of the point on the line whose x=0.
     * @param minY the minimum value the function can yield
     * @param maxY the maximum value the function can yield
     * @return a monotonically crescent value
     */
    template <typename NUM1, typename NUM2, typename NUM3, typename NUM4, typename NUM5, typename NUM6>
    constexpr double getMonotonicallyCrescentFast(const NUM1& x, const NUM2& ratio, const NUM3& m, const NUM4& q, const NUM5& minY, const NUM6& maxY) {
        auto newX = linearTransform(x, m, q);
        debug("m=", m, "q=", q, "x=", x, "newX=", newX);
        return getMonotonicallyCrescent(ratio * newX, minY, maxY);
    }

    /**
     * @brief Get normal distribution
     * 
     * @code
     *  (1/sigma sqrt(2*pi))* exp(0.5 * (x- mu/sigma)^2)
     * @endcode
     * 
     * @param x 
     * @param mean 
     * @param stddev 
     * @return double 
     */
    double getNormal(double x, double mean, double stddev);

    /**
     * @brief Get normal distribution
     * 
     * @code
     *  compute a gaussian with mean 0
     * @endcode
     * 
     * @param x 
     * @param mean 
     * @param stddev 
     * @return double 
     */
    double getGaussian(double x, double stddev);

    /**
     * @brief compute a gaussian whose minimum y and maximum y are given
     * 
     * @param minY 
     * @param maxY 
     * @param x 
     * @param stddev 
     * @return double 
     */
    double getGaussian(double minY, double maxY, double x, double stddev);

    /**
     * @brief compute a gaussian which is 
     * 
     * @note
     * in the gaussian, \f$ \mu + 3 \sigma \f$ holds 99.7% of the data
     * 
     * @param x 
     * @param mean 
     * @param stddevN a number which represents how rapidly the gaussian slows down. It is correlated to the standard deviation. 0 means the 
     * @param minX 
     * @param maxX 
     * @param minY 
     * @param maxY 
     * @return double 
     * @see https://en.wikipedia.org/wiki/Normal_distribution
     */
    double getCenteredGaussian(double x, double stddevN, double minX, double maxX, double minY, double maxY);

    double getLeftGaussian(double x, double stddev, double minX, double maxX, double minY, double maxY);

    double getRightGaussian(double x, double stddev, double minX, double maxX, double minY, double maxY);

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
     * @see https://stackoverflow.com/a/253874/1887602
     */
    template <typename T>
    constexpr bool isApproximatelyEqual(const T& a, const T& b, const T& epsilon) {
        //see https://stackoverflow.com/a/41405501/1887602
        auto_debug(a);
        auto_debug(b);
        auto_debug(epsilon);
        auto diff = std::fabs(a - b);
        if (diff <= epsilon)
            return true;

        if (diff < std::fmax(std::fabs(a), std::fabs(b)) * epsilon)
            return true;

        return false;
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
                throw cpp_utils::exceptions::InvalidArgumentException{"cannot convert fraction", decimal, "into numerator and denominator", decimal, ". epsilon=", epsilon, "limit", limit};
            }
            limit -= 1;

            numerator = static_cast<OUT>(decimal * static_cast<T>(denominator));
            T numeratorT = decimal * denominatorT;

            T numeratorTimesDivided10 = (static_cast<T>(10.) * numeratorT)/(static_cast<T>(10.));
            debug("decimal=", decimal, "numerator=", numerator, "denominator=", denominator, "numeratorT=", numeratorT, "denominatorT=", denominatorT, "numeratorTimesDivided10=", numeratorTimesDivided10, "limit=", limit, "epsilon=", epsilon);
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

    /**
     * @brief constraint a value within an inclusive interval
     * 
     * @tparam NUM1 type of @c x
     * @tparam NUM2 type of @c lb
     * @tparam NUM3 type of @c ub
     * @param x the value to test
     * @param lb lowerbound of the interval
     * @param ub upperbound of the interval
     * @return lb if \f$ x < lb \f$, ub if \f$ x > ub \f$, @c x otherwise
     */
    template <typename NUM1, typename NUM2, typename NUM3>
    constexpr NUM1 bound(const NUM1& x, const NUM2& lb, const NUM3& ub) {
        if (x < lb) {
            return static_cast<NUM1>(lb);
        }
        if (x > ub) {
            return static_cast<NUM2>(ub);
        }
        return x;
    }

}

#endif