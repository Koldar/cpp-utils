/**
 * @file
 * 
 * @brief a timer you can use to time things
 * 
 * A cross-platform monotonic wallclock timer. Currently supports nanoseconds resolution.
 * 
 * Reference doco for timers on OSX:
 * @li https://developer.apple.com/library/mac/qa/qa1398/_index.html
 * @li https://developer.apple.com/library/mac/technotes/tn2169/_index.html#//apple_ref/doc/uid/DTS40013172-CH1-TNTAG5000
 * 
 * @author: dharabor
 * @created: September 2012
 *  
 */

#ifndef _CPPUTILS_TIMER_HEADER__
#define _CPPUTILS_TIMER_HEADER__

#ifdef OS_MAC
#   include <mach/mach.h>
#   include <mach/mach_time.h>
#else
#   include <time.h>
#endif
#include "ICleanable.hpp"
#include <iostream>

namespace cpp_utils {

    enum class timeunit_e {
        NANO,
        MICRO,
        MILLI,
        SECOND,
        MINUTE,
        HOUR,
        DAY
    };

    namespace internal {

        double getTimeUnitInSeconds(timeunit_e u);

    }

    /**
     * @brief class containing a timing
     * 
     * this class follows value object design pattern
     * 
     */
    class timing_t {
    public:
        friend std::ostream& operator << (std::ostream& out, const timing_t& t);
        
        friend bool operator == (const timing_t& a, const timing_t& b);
        friend bool operator != (const timing_t& a, const timing_t& b);
        friend bool operator < (const timing_t& a, const timing_t& b);
        friend bool operator <= (const timing_t& a, const timing_t& b);
        friend bool operator > (const timing_t& a, const timing_t& b);
        friend bool operator >= (const timing_t& a, const timing_t& b);

        template <typename NUM>
        friend timing_t operator +(const timing_t& a, NUM n);
        template <typename NUM>
        friend timing_t operator -(const timing_t& a, NUM n);
        template <typename NUM>
        friend timing_t operator *(const timing_t& a, NUM n);
        template <typename NUM>
        friend timing_t operator /(const timing_t& a, NUM n);
    private:
        /**
         * @brief number of unit elapsed
         * 
         */
        double time;
        /**
         * @brief unit of @c time
         * 
         */
        timeunit_e unit;
    public:
        constexpr timing_t(): time{0}, unit{timeunit_e::MICRO} {

        }
        constexpr timing_t(int t): time{static_cast<double>(t)}, unit{timeunit_e::MICRO} {
        }
        constexpr timing_t(unsigned int t): time{static_cast<double>(t)}, unit{timeunit_e::MICRO} {
        }
        constexpr timing_t(long t): time{static_cast<double>(t)}, unit{timeunit_e::MICRO} {
        }
        constexpr timing_t(unsigned long t): time{static_cast<double>(t)}, unit{timeunit_e::MICRO} {
        }
        constexpr timing_t(double t): time{t}, unit{timeunit_e::MICRO} {

        }
        constexpr timing_t(double t, timeunit_e unit): time{t}, unit{unit} {

        }
        timing_t toNanos() const {
            return this->convert(timeunit_e::NANO);
        }
        timing_t toMicros() const {
            return this->convert(timeunit_e::MICRO);
        }
        timing_t toMillis() const {
            return this->convert(timeunit_e::MILLI);
        }
        timing_t toSeconds() const {
            return this->convert(timeunit_e::SECOND);
        }
        timing_t toMinutes() const {
            return this->convert(timeunit_e::MINUTE);
        }
        timing_t toHour() const {
            return this->convert(timeunit_e::HOUR);
        }
        timing_t toDays() const {
            return this->convert(timeunit_e::DAY);
        }
    private:
        timing_t convert(timeunit_e other) const {
            //t * unit: number of seconds
            return timing_t{(this->time * internal::getTimeUnitInSeconds(this->unit))/internal::getTimeUnitInSeconds(other), other};
        }
    };

    template <typename NUM>
    timing_t operator +(const timing_t& a, NUM n) {
        return timing_t{a.time + n, a.unit};
    }

    template <typename NUM>
    timing_t operator -(const timing_t& a, NUM n) {
        return timing_t{a.time - n, a.unit};
    }

    template <typename NUM>
    timing_t operator *(const timing_t& a, NUM n) {
        return timing_t{a.time * n, a.unit};
    }

    template <typename NUM>
    timing_t operator /(const timing_t& a, NUM n) {
        return timing_t{a.time / n, a.unit};
    }



    /**
     * @brief class to time things
     * 
     */
    class Timer : public ICleanable {
    private:
    #ifdef OS_MAC
        uint64_t start_time;
        uint64_t stop_time;
        mach_timebase_info_data_t timebase;
    #else
        timespec stop_time;
        timespec start_time;
    #endif
        bool running;
    public:
        friend std::ostream& operator <<(std::ostream& out, const Timer& t) {
            if (t.isRunning()) {
                out << t.getCurrentElapsedMicroSeconds();
            } else {
                out << "timer stopped";
            }
            return out;
        }
    public:
        /**
         * @brief Construct a new Timer object
         * 
         * @param start if true, we will immediately start the time
         */
        Timer(bool start=false);
    public:
        void cleanup();
    public:
        /**
         * @brief start the timer
         * 
         * Do nothing if the timer is alerady started
         * 
         */
        void start();
        /**
         * @brief stop the timer
         * 
         * Do nothing if the timer is not running
         * 
         */
        void stop();
        /**
         * @brief check if the timer is running
         * 
         * @return true 
         * @return false 
         */
        bool isRunning() const;

        /**
         * @brief get the microseconds detected by the clock **relative** to its start
         * 
         * @pre
         *  @li timer has been stopped
         * 
         * @return timing_t time **relative** to the timer start detected by the clock.
         */
        timing_t getElapsedMicroSeconds() const;
        /**
         * @brief get the microseconds detected by the clock **relative** to its start
         * 
         * @pre
         *  @li timer is still running
         * 
         * @return timing_t time **relative** to the timer start detected by the clock. 
         */
        timing_t getCurrentElapsedMicroSeconds() const;
        /**
         * @brief get the microseconds detected by the clock
         * 
         * @return timing_t **absolute** time detected by the clock. implementation dependet
         */
        timing_t getCurrentMicroSeconds() const;
    };

}

#endif 