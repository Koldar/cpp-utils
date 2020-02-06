#ifndef _CPP_UTILS_INTERVAL_HEADER__
#define _CPP_UTILS_INTERVAL_HEADER__

#include <regex>
#include <cmath>
#include <iostream>

#include "math.hpp"

namespace cpp_utils {


    /**
     * @brief interval of 2 numbers.
     * 
     * @tparam T type of the values
     */
    template <typename T>
    class Interval {
        using This = Interval<T>;
    private:
        T lb;
        T ub;
        bool lbIncluded;
        bool ubIncluded;
    public:
        friend std::ostream& operator <<(std::ostream& ss, const This& i) {
            ss  << "{" 
                << (i.lbIncluded ? "[" : "(")
                << i.lb
                << "; "
                << i.ub
                << (i.ubIncluded ? "]" : ")")
                << "}";
            return ss;
        }
        friend This operator +(const This&a , const This& b) {
            This result{a};
            result += b;
            return result;
        }
        friend This operator -(const This&a , const This& b) {
            This result{a};
            result -= b;
            return result;
        }
        friend This operator *(const This&a , const This& b) {
            This result{a};
            result *= b;
            return result;
        }
    public:
        explicit Interval(const T& v): lb{v}, ub{v}, lbIncluded{true}, ubIncluded{true} {
            
        }
        explicit Interval(const T& lb, const T& ub, bool lbIncluded, bool ubIncluded): lb{lb}, ub{ub}, lbIncluded{lbIncluded}, ubIncluded{ubIncluded} {

        }
        virtual ~Interval() {

        }
        Interval(const This& o): lb{o.lb}, ub{o.ub}, lbIncluded{o.lbIncluded}, ubIncluded{o.ubIncluded} {

        }
        Interval(This&& o): lb{o.lb}, ub{o.ub}, lbIncluded{o.lbIncluded}, ubIncluded{o.ubIncluded} {
        }
        This& operator =(const This& o) {
            this->lb = o.lb;
            this->ub = o.ub;
            this->lbIncluded = o.lbIncluded;
            this->ubIncluded = o.ubIncluded;
            return *this;
        }
        This& operator =(This&& o) {
            this->lb = o.lb;
            this->ub = o.ub;
            this->lbIncluded = o.lbIncluded;
            this->ubIncluded = o.ubIncluded;
            return *this;
        }
    public:
        This& operator +=(const This& o) {
            this->lb += o.lb;
            this->ub += o.ub;
            this->lbIncluded = this->lbIncluded && o.lbIncluded;
            this->ubIncluded = this->ubIncluded && o.ubIncluded;
            return *this;
        }
        This& operator +=(const T& v) {
            this->lb += v;
            this->ub += v;
            return *this;
        }
        This& operator -=(const This& o) {
            this->lb = this->lb - o.ub;
            this->ub -= this->ub - o.lb;
            this->lbIncluded = o.ubIncluded && this->lbIncluded;
            this->ubIncluded = this->ubIncluded && o.lbIncluded;
            return *this;
        }
        This& operator -=(const T& v) {
            this->lb -= v;
            this->ub -= v;
            return *this;
        }
        This& operator *=(const This& o) {
            this->lb = std::min(this->lb * o.lb, this->lb * o.ub, this->ub * o.lb, this->ub * o.ub);
            this->ub -= std::max(this->lb * o.lb, this->lb * o.ub, this->ub * o.lb, this->ub * o.ub);
            
            int amin = argmin(this->lb * o.lb, this->lb * o.ub, this->ub * o.lb, this->ub * o.ub);
            switch (amin) {
                case 0: this->lbIncluded = this->lbIncluded && o.lbIncluded; break;
                case 1: this->lbIncluded = this->lbIncluded && o.ubIncluded; break;
                case 2: this->lbIncluded = this->ubIncluded && o.lbIncluded; break;
                case 3: this->lbIncluded = this->ubIncluded && o.ubIncluded; break;
                default: {
                    throw cpp_utils::exceptions::InvalidScenarioException{"minimum", amin};
                }
            }

            int amax = argmax(this->lb * o.lb, this->lb * o.ub, this->ub * o.lb, this->ub * o.ub);
            switch (amax) {
                case 0: this->ubIncluded = this->lbIncluded && o.lbIncluded; break;
                case 1: this->ubIncluded = this->lbIncluded && o.ubIncluded; break;
                case 2: this->ubIncluded = this->ubIncluded && o.lbIncluded; break;
                case 3: this->ubIncluded = this->ubIncluded && o.ubIncluded; break;
                default: {
                    throw cpp_utils::exceptions::InvalidScenarioException{"maximum", amax};
                }
            }

            return *this;
        }
        This& operator *=(const T& v) {
            this->lb *= v;
            this->ub *= v;
            return *this;
        }
    public:
        This& scale(const T& v) const {
            This result{*this};
            result *= v;
            return result;
        }
        This& translate(const T& v) const {
            This result{*this};
            result += v;
            return result;
        }
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
         * @brief the lowerbound of the interval as shown in the notation [a,b)
         * 
         * Doesn't consider ::lbIncluded
         * 
         * @return T 
         */
        T getLowerbound() const {
            return this->lb;
        }
        /**
         * @brief the upperbound of the interval as shown in the notation [a,b)
         * 
         * Doesn't consider ::ubIncluded
         * 
         * @return true 
         * @return false 
         */
        T getUpperbound() const {
            return this->ub;
        }
        /**
         * @brief check if the lowerbound of the itnerval is included in the interval
         * 
         * @return true 
         * @return false 
         */
        bool isLowerboundIncluded() const {
            return this->lbIncluded;
        }
        /**
         * @brief check if the upperbound of the itnerval is included in the interval
         * 
         * @return true 
         * @return false 
         */
        bool isUpperboundIncluded() const {
            return this->ubIncluded;
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
         * @brief create an interval which contains only a single value
         * 
         * @param v the value to contain
         * @return This 
         */
        static This singleton(const T& v) {
            return This{v, v, true, true};
        }
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
                log_error("interval was \"", mathInterval, "\"");
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

        static Interval<double> parseDoubleInterval(const std::string& mathInterval) {
            std::regex re{"^([\\[\\]\\(])(\\d+(?:\\.\\d*)?)\\s*,\\s*(\\d+(?:\\.\\d*)?)([\\[\\]\\)])$"};
            std::smatch matches;
            bool canMatch = std::regex_match(mathInterval, matches, re);
            if (!canMatch) {
                log_error("interval was \"", mathInterval, "\"");
                throw std::invalid_argument{"regex didn't match"};
            }

            Interval<double> result{0., 0., true, true};

            if (matches[1].str() == "[") {
                result.lbIncluded = true;
            } else if (matches[1].str() == "]") {
                result.lbIncluded = false;
            } else if (matches[1].str() == "(") {
                result.lbIncluded = false;
            } else {
                throw cpp_utils::exceptions::InvalidArgumentException{"incompatible lower bound parenthesis. Expected one of []( but got '%c'", matches[1].str()};
            }

            result.lb = atof(matches[2].str().c_str());
            result.ub = atof(matches[3].str().c_str());

            if (matches[4].str() == "]") {
                result.ubIncluded = true;
            } else if (matches[4].str() == "[") {
                result.ubIncluded = false;
            } else if (matches[4].str() == ")") {
                result.ubIncluded = false;
            } else {
                throw cpp_utils::exceptions::InvalidArgumentException{"incompatible upper bound parenthesis. Expected one of []) but got '%c'", matches[1].str()};
            }

            return result;
        }
    };

    /**
     * @brief alias for Interval<int>
     * 
     */
    using IntInterval = Interval<int>;
    /**
     * @brief alias for Interval<double>
     * 
     */
    using DoubleInterval = Interval<double>;
    

}


#endif