
#include "Random.hpp"
#include <time.h>
#include <limits>


namespace cpp_utils {

    Random::Random() {
    }

    Random::Random(unsigned int seed) : uniformIntDistributions{}, uniformRealDistributions{}, randomEngine{seed} {
        srand(seed);
    }

    Random::Random(const Random& o) : uniformIntDistributions{o.uniformIntDistributions}, uniformRealDistributions{o.uniformRealDistributions}, randomEngine{o.randomEngine} {

    }

    Random::Random(Random&& o) : uniformIntDistributions{::std::move(o.uniformIntDistributions)}, uniformRealDistributions{::std::move(o.uniformRealDistributions)}, randomEngine{::std::move(o.randomEngine)}{

    }

    Random& Random::getDefault() {
        static Random RANDOM{0};
        return RANDOM;
    }

    Random& Random::operator = (const Random& o) {
        this->uniformRealDistributions = o.uniformRealDistributions;
        this->uniformIntDistributions = o.uniformIntDistributions;
        this->randomEngine = o.randomEngine;
        return *this;
    }

    Random& Random::operator = (Random&& o) {
        this->uniformRealDistributions = ::std::move(o.uniformRealDistributions);
        this->uniformIntDistributions = ::std::move(o.uniformIntDistributions);
        this->randomEngine = ::std::move(o.randomEngine);
        return *this;
    }

    Random::~Random() {

    }

    int Random::next(int lowerbound, int upperbound, bool includeUpperbound) {
        return Random::getDefault().nextInt(lowerbound, upperbound, includeUpperbound);
    }

    int Random::next(const Interval<int>& interval) {
        return Random::getDefault().nextInt(interval);
    }

    double Random::next(double lowerbound, double upperbound, bool includeUpperbound) {
        return Random::getDefault().nextDouble(lowerbound, upperbound, includeUpperbound);
    }
    
    double Random::next(const Interval<double>& interval) {
        return Random::getDefault().nextDouble(interval);
    }

    bool Random::flip(double successProbability) const {
        if (cpp_utils::isApproximatelyEqual(successProbability, 1.0, 1e-6)) {
            return true;
        }
        auto rnd = rand();
        return rnd < (successProbability * RAND_MAX);
    }

    double Random::nextDouble(double lowerbound, double upperbound, bool includeUpperbound) const {
        auto key = std::make_tuple(lowerbound, upperbound, includeUpperbound);
        if (!this->uniformRealDistributions.containsKey(key)) {
            if (includeUpperbound) {
                //uniform_reals_distribution picks from [a,b)
                this->uniformRealDistributions.put(key, std::uniform_real_distribution<double>{lowerbound, upperbound});
            } else {
                this->uniformRealDistributions.put(key, std::uniform_real_distribution<double>{lowerbound, std::nextafter(upperbound, std::numeric_limits<double>::max())});
            }
        }
        return this->uniformRealDistributions[key](this->randomEngine);
    }

    double Random::nextDouble(const Interval<double>& interval) const {
        return this->nextDouble(interval.getLowerbound(), interval.getUpperbound(), interval.isUpperboundIncluded());
    }
    
    int Random::nextInt(int lowerbound, int upperbound, bool includeUpperbound) const {
        //uniform_int_distribution picks from [a,b]
        upperbound -= includeUpperbound ? 0 : 1;
        auto key = std::make_tuple(lowerbound, upperbound);
        if (!this->uniformIntDistributions.containsKey(key)) {
            this->uniformIntDistributions.put(key, std::uniform_int_distribution<int>{lowerbound, upperbound});
        }
        return this->uniformIntDistributions[key](this->randomEngine);
    }


    int Random::nextInt(const Interval<int>& interval) const {
        return this->nextInt(interval.getLB(), interval.getUB(), true);
    }

}