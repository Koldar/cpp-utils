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
        /**
         * @brief get a singleton of ::Random
         * 
         * sometimes you just want to fetch a a random number and you don't want to depend on an external
         * ::Random instance. Use this method to do so.
         * 
         * @return Random& 
         */
        static Random& getDefault();
    public:
        Random& operator = (const Random& o);
        Random& operator = (Random&& o);
        virtual ~Random();
    public:
        /**
         * @brief fetch a next int involved in `[a, b)` interval
         * 
         * @param lowerbound the lowerbound of interval `[a, b)` (included)
         * @param upperbound the upperbound of interval `[a, b)` (excluded)
         * @param includeUpperbound true if you want to use the intervla `[a,b]` instead of `[a,b)`
         * @return int an int inside [a, b)
         */
        static int next(int lowerbound, int upperbound, bool includeUpperbound = false);
        /**
         * @brief fetch a next int involved in `[a, b)` interval
         * 
         * @param interval interval `[a, b)`
         * @return int an int inside [a, b)
         */
        static int next(const Interval<int>& interval);
        /**
         * @brief fetch a next double involved in `[a, b)` interval
         * 
         * @param lowerbound the lowerbound of interval `[a, b)` (included)
         * @param upperbound the upperbound of interval `[a, b)` (excluded)
         * @param includeUpperbound true if you want to use the intervla `[a,b]` instead of `[a,b)`
         * @return double a double inside [a, b)
         */
        static double next(double lowerbound, double upperbound, bool includeUpperbound = false);
        /**
         * @brief fetch a next double involved in `[a, b)` interval
         * 
         * @param interval interval `[a, b)`
         * @return double a double inside [a, b)
         */
        static double next(const Interval<double>& interval);
    public:
        double nextDouble(double lowerbound, double upperbound, bool includeUpperbound = false) const;
        double nextDouble(const Interval<double>& interval) const;
        int nextInt(int lowerbound, int upperbound, bool includeUpperbound = false) const;
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
        int nextInt(const Interval<int>& interval) const;
        /**
         * @brief flip a coin which has the given probability to return OK and 1- the given probability to return KO
         * 
         * @param successProbability probability to yield true
         * @return true 
         * @return false 
         */
        bool flip(double successProbability) const;
    private:
        mutable MapPlus<std::tuple<double, double, bool>, std::uniform_real_distribution<double>> uniformRealDistributions;
        mutable MapPlus<std::tuple<int, int>, std::uniform_int_distribution<int>> uniformIntDistributions;
        mutable std::mt19937_64 randomEngine;
    };
}

#endif