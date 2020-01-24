#ifndef _CPP_UTILS_FUNCTIONAL_HEADER__
#define _CPP_UTILS_FUNCTIONAL_HEADER__

#include <functional>

namespace cpp_utils {

    /**
     * @brief identity function
     * 
     * @code
     *  identity<int> x;
     *  x(5) //give 5
     * @endcode
     * 
     * @tparam T type to accept and return
     * @param x the value to return
     * @return T @c x
     */
    template <typename T>
    struct identity {

        T operator ()(const T& x) {
            return value(x);
        }
        static T value(const T& x) {
            return x;
        }
    };


    template <typename IN, typename OUT>
    using function_t = std::function<OUT(const IN& in)>;

    template <typename IN1, typename IN2, typename OUT>
    using bifunction_t = std::function<OUT(const IN1& in, const IN2& in2)>;

    template <typename IN1, typename IN2, typename IN3, typename OUT>
    using trifunction_t = std::function<OUT(const IN1& in, const IN2& in2, const IN3& in3)>;

    /**
     * @brief a function whic accept a certain type and return the same type
     * 
     * Doesn't encessary mean that it is the identify function. For isntance,
     * 
     * @code
     *  autofunction_t<int> double = [&](const int& a) { return 2*a; };
     * @endcode
     * 
     * @tparam X 
     */
    template <typename X>
    using autofunction_t =  function_t<X, X>;

    template <typename IN>
    using predicate_t = function_t<IN, bool>;

    template <typename IN1, typename IN2>
    using bipredicate_t = bifunction_t<IN1, IN2, bool>;

    template <typename IN1, typename IN2, typename IN3>
    using tripredicate_t = trifunction_t<IN1, IN2, IN3,bool>;

    using runnable_t = std::function<void()>;

    template <typename IN>
    using consumer_t = std::function<void(const IN& in)>;

    template <typename IN1, typename IN2>
    using biconsumer_t = std::function<void(const IN1& in, const IN2& in2)>;

    template <typename IN1, typename IN2, typename IN3>
    using triconsumer_t = std::function<void(const IN1& in, const IN2& in2, const IN3& in3)>;
}

#endif 