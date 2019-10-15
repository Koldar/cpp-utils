
#include "Random.hpp"
#include <time.h>


namespace cpp_utils {

Random::Random() {
}

Random::Random(unsigned int seed) {
    srand(seed);
}

Random::Random(const Random& o) {

}

Random::Random(Random&& o) {

}

Random& Random::operator = (const Random& o) {
    return *this;
}

Random& Random::operator = (Random&& o) {
    return *this;
}

Random::~Random() {

}

bool Random::flip(double successProbability) {
    if (cpp_utils::isApproximatelyEqual(successProbability, 1.0, 1e-6)) {
        return true;
    }
    auto rnd = rand();
    return rnd < (successProbability * RAND_MAX);
}

}