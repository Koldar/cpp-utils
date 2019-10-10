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
}

#endif 