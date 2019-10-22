#ifndef _CPP_UTILS_INTERVAL_HEADER__
#define _CPP_UTILS_INTERVAL_HEADER__

#include <regex>

namespace cpp_utils {


    template <typename T>
    class Interval {
        typedef Interval<T> IntervalInstance;
    private:
        T lb;
        T ub;
        bool lbIncluded;
        bool ubIncluded;
    public:
        Interval(const T& lb, const T& ub, bool lbIncluded, bool ubIncluded): lb{lb}, ub{ub}, lbIncluded{lbIncluded}, ubIncluded{ubIncluded} {

        }
        ~Interval() {

        }
        Interval(const IntervalInstance& o): lb{o.lb}, ub{o.ub}, lbIncluded{o.lbIncluded}, ubIncluded{o.ubIncluded} {

        }
        Interval(IntervalInstance&& o): lb{o.lb}, ub{o.ub}, lbIncluded{o.lbIncluded}, ubIncluded{o.ubIncluded} {
        }
        IntervalInstance& operator =(const IntervalInstance& o) {
            this->lb = o.lb;
            this->ub = o.ub;
            this->lbIncluded = o.lbIncluded;
            this->ubIncluded = o.ubIncluded;
            return *this;
        }
        IntervalInstance& operator =(IntervalInstance&& o) {
            this->lb = o.lb;
            this->ub = o.ub;
            this->lbIncluded = o.lbIncluded;
            this->ubIncluded = o.ubIncluded;
            return *this;
        }
    public:
        /**
         * @return the lowerrbound you can put to a c++ rnadom distribution in order to get a random number
         */
        T getLB() const {
            return lb + (lbIncluded ? 0 : 1);
        }

        /**
         * @return the upperbound you can put to a c++ rnadom distribution in order to get a random number
         */
        T getUB() const {
            return ub - (ubIncluded ? 0 : 1);
        }
        /**
         * @brief check if a value is inside this range
         * 
         * @param v 
         * @return true 
         * @return false 
         */
        bool contains(const T& v) const {
            if (v == lb && lbIncluded) {
                return true;
            }
            if (v == ub && ubIncluded) {
                return true;
            }
            return (v > lb) && (v < ub);
        }
    public:
        /**
         * generate a Interval from a mathematical Interval string
         *
         * @code
         * fromMath("[5;7["); //lowerbound=5, upperbound=7, lbincluded=tue, ubincluded=false
         * @endocde
         *
         * @param[in] fromMath the string to consider
         * @return the Interval structure generated
         */
        static Interval<int> fromMath(const std::string& mathInterval) {
            std::regex re{"^([\\[\\]\\(])(\\d+)\\s*,\\s*(\\d+)([\\[\\]\\)])$"};
            std::smatch matches;
            bool canMatch = std::regex_match(mathInterval, matches, re);
            if (!canMatch) {
                error("interval was \"", mathInterval, "\"");
                throw std::invalid_argument{"regex didn't match"};
            }

            Interval<int> result{0, 0, true, true};

            if (matches[1].str() == "[") {
                result.lbIncluded = true;
            } else if (matches[1].str() == "]") {
                result.lbIncluded = false;
            } else if (matches[1].str() == "(") {
                result.lbIncluded = false;
            } else {
                throw std::invalid_argument{"incompatible lower bound parenthesis"};
            }

            result.lb = atoi(matches[2].str().c_str());
            result.ub = atoi(matches[3].str().c_str());

            if (matches[4].str() == "]") {
                result.ubIncluded = true;
            } else if (matches[4].str() == "[") {
                result.ubIncluded = false;
            } else if (matches[4].str() == ")") {
                result.ubIncluded = false;
            } else {
                throw std::invalid_argument{"incompatible upper bound parenthesis"};
            }

            return result;
        }
    };

}


#endif