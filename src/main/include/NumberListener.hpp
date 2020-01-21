#ifndef _CPP_NUMBERLISTENER_HEADER__
#define _CPP_NUMBERLISTENER_HEADER__

#include "ICleanable.hpp"

namespace cpp_utils {

    /**
     * @brief represents a listener that inform you when a number updates its value
     * 
     * @tparam NUMBER type of the number to watch
     */
    template <typename NUMBER>
    class NumberListener: public ICleanable {
    public:
        /**
         * @brief instructions to execute when the numer has increased its value
         * 
         * @param oldValue the old number value
         * @param newValue the new number value
         */
        virtual void onNumberIncreased(const NUMBER& oldValue, const NUMBER& newValue) = 0;

        /**
         * @brief instructions to execute when the numer has decreased its value
         * 
         * @param oldValue the old number value
         * @param newValue the new number value
         */
        virtual void onNumberDecreased(const NUMBER& oldValue, const NUMBER& newValue) = 0;
    };

    /**
     * @brief ::NumberListener watching an int
     * 
     */
    using IntNumberListener = NumberListener<int>;
    /**
     * @brief ::NumberListener watching a float
     * 
     */
    using FloatNumberListener = NumberListener<float>;
    /**
     * @brief ::NumberListener watching a double
     * 
     */
    using DoubleNumberListener = NumberListener<double>;

}

#endif