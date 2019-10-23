#ifndef _CPP_UTILS_RANDOM_HEADER__
#define _CPP_UTILS_RANDOM_HEADER__

#include "exceptions.hpp"
#include "Interval.hpp"
#include "math.hpp"

namespace cpp_utils {

    class Random {
    public:
        /**
         * @brief Dummy instance, You can use it but be aware that it doesn't set @c srand
         * 
         */
        Random();
        /**
         * @brief Creates a new instance by setting @c srand
         * 
         * @param seed 
         */
        Random(unsigned int seed);
        Random(const Random& o);
        Random(Random&& o);
        Random& operator = (const Random& o);
        Random& operator = (Random&& o);
        virtual ~Random();
    public:
        template <typename T>
        static T nextNum(T lowerbound, T upperbound, bool includeUpperbound = false) {
            return Random{}.next(lowerbound, upperbound, includeUpperbound);
        }
        template <typename T>
        static T nextNum(const Interval<T>& interval) {
            return Random{}.next(interval);
        }
    public:
        template <typename T>
        T next(T lowerbound, T upperbound, bool includeUpperbound = false) {
            if (includeUpperbound) {
                upperbound += 1;
            }
            T diff = upperbound - lowerbound;
            if (diff <= 0) {
                debug("lowerbound =", lowerbound);
                debug("upperbound =", upperbound);
                throw cpp_utils::exceptions::ImpossibleException{"upperbound less than lowerbund!"};
            }
            T x = (RAND_MAX + 0u) / diff; //number of times u-l can be put inside RAND_MAX
            T y = x * diff; //obtain a valule smaller than rand_max
            T r;
            do {
                r = rand();
                debug("r is", r, "y=", y);
            } while(r >= y); //if rand is greater than the admissible value, retry
            return lowerbound + (r / x);

        }
        template <typename T>
        T next(const Interval<T>& interval) {
            return this->next(interval.getLB(), interval.getUB(), true);
        }
        bool flip(double successProbability);
    };

}

#endif