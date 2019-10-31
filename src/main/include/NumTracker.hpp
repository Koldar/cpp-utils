#ifndef _CPP_UTILS_NUMTRACKER_HEADER__
#define _CPP_UTILS_NUMTRACKER_HEADER__

#include "ICleanable.hpp"

namespace cpp_utils {

    /**
     * @brief a class that tracks the average of a number
     * 
     * @code
     *  NumTracker tracker;
     *  tracker.update(5);
     *  tracker.update(6);
     * 
     *  //get mean
     *  tracker.getAverage() //5.5
     * @endcode
     * 
     * @tparam type of the parameter. Needs to be convertible into double
     */
    template <typename T>
    class NumTracker: public ICleanable {
        typedef NumTracker<T> This;
    private:
        int count;
        double lastMean;
        T lastValue;
    public:
        constexpr NumTracker(): count{0}, lastMean{0}, lastValue{0} {

        }
        constexpr NumTracker(T n): count{1}, lastMean{static_cast<double>(n)}, lastValue{n} {
        }
        virtual ~NumTracker() {

        }
        NumTracker(const This& o): count{o.count}, lastMean{o.lastMean}, lastValue{o.lastValue} {

        }
        NumTracker(This&& o): count{o.count}, lastMean{o.lastMean}, lastValue{o.lastValue} {
        }
        This& operator=(const This& o) {
            this->count = o.count;
            this->lastMean = o.lastMean;
            this->lastValue = o.lastValue;

            return *this;
        }
        This& operator=(This&& o) {
            this->count = o.count;
            this->lastMean = o.lastMean;
            this->lastValue = o.lastValue;

            return *this;
        }
    public:
        void update(T newValue) {
            this->count += 1;
            this->lastMean = this->lastMean + (newValue - this->lastMean)/(this->count);
            this->lastValue = newValue;
        }
        int getCount() const {
            return this->count;
        }
        T getLastValue() const {
            return this->lastValue;
        }
        double getAverage() const {
            return this->lastMean;
        }
    public:
        virtual void cleanup() {
            this->count = 0;
            this->lastMean = 0;
        }
    };

}

#endif