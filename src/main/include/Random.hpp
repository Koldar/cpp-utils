#ifndef _CPP_UTILS_RANDOM_HEADER__
#define _CPP_UTILS_RANDOM_HEADER__

#include <random>

#include "exceptions.hpp"
#include "Interval.hpp"
#include "math.hpp"
#include "mapplus.hpp"
#include "operators.hpp"

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
    public:
        Random& operator = (const Random& o);
        Random& operator = (Random&& o);
        virtual ~Random();
    public:
        template <typename T>
        static T nextNum(T lowerbound, T upperbound, bool includeUpperbound = false) {
            return Random{}.nextInt(lowerbound, upperbound, includeUpperbound);
        }
        template <typename T>
        static T nextNum(const Interval<T>& interval) {
            return Random{}.nextInt(interval);
        }
    public:
        double nextDouble(double lowerbound, double upperbound, bool includeUpperbound = false);
        int nextInt(int lowerbound, int upperbound, bool includeUpperbound = false);
        // template <typename T>
        // T next(T lowerbound, T upperbound, bool includeUpperbound = false) {
        //     if (includeUpperbound) {
        //         upperbound += 1;
        //     }
        //     T diff = upperbound - lowerbound;
        //     if (diff <= 0) {
        //         debug("lowerbound =", lowerbound);
        //         debug("upperbound =", upperbound);
        //         throw cpp_utils::exceptions::ImpossibleException{"upperbound less than lowerbund!"};
        //     }
        //     T x = (RAND_MAX + 0u) / diff; //number of times u-l can be put inside RAND_MAX
        //     T y = x * diff; //obtain a valule smaller than rand_max
        //     T r;
        //     do {
        //         r = rand();
        //         debug("r is", r, "y=", y);
        //     } while(r >= y); //if rand is greater than the admissible value, retry
        //     return lowerbound + (r / x);

        // }
        /**
         * @brief fetch a value in a interval [a, b]
         * 
         * @param interval the involved interval
         * @return int a value in the interval
         */
        int nextInt(const Interval<int>& interval);
        /**
         * @brief flip a coin which has the given probability to return OK and 1- the given probability to return KO
         * 
         * @param successProbability probability to yield true
         * @return true 
         * @return false 
         */
        bool flip(double successProbability);
    private:
        MapPlus<std::tuple<double, double, bool>, std::uniform_real_distribution<double>> uniformRealDistributions;
        MapPlus<std::tuple<int, int>, std::uniform_int_distribution<int>> uniformIntDistributions;
        std::mt19937_64 randomEngine;
    };
}

#endif