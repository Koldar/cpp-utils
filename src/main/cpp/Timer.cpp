#include "Timer.hpp"
#include "math.hpp"
#include "exceptions.hpp"

namespace cpp_utils {

    std::ostream& operator << (std::ostream& out, const timing_t& t) {
        out << t.time;
        switch (t.unit) {
            case timeunit_e::NANO: {
                out << "ns";
                break;
            }
            case timeunit_e::MICRO: {
                out << "us";
                break;
            }
            case timeunit_e::MILLI: {
                out << "ms";
                break;
            }
            case timeunit_e::SECOND: {
                out << "s";
                break;
            }
            case timeunit_e::MINUTE: {
                out << "m";
                break;
            }
            case timeunit_e::HOUR: {
                out << "h";
                break;
            }
            case timeunit_e::DAY: {
                out << " day";
                break;
            }
            default: {
                throw cpp_utils::exceptions::InvalidScenarioException<timeunit_e>{t.unit};
            }
        }
        return out;
    }

    bool operator == (const timing_t& a, const timing_t& b) {
        if (a.unit != b.unit) {
            throw cpp_utils::exceptions::InvalidArgumentException{};
        }
        if (&a == &b) {
            return true;
        }
        return isApproximatelyEqual(a.time, b.time, 1e6);
    }

    bool operator != (const timing_t& a, const timing_t& b) {
        if (a.unit != b.unit) {
            throw cpp_utils::exceptions::InvalidArgumentException{};
        }
        if (&a != &b) {
            return true;
        }
        return !isApproximatelyEqual(a.time, b.time, 1e6);
    }

    bool operator < (const timing_t& a, const timing_t& b) {
        if (a.unit != b.unit) {
            throw cpp_utils::exceptions::InvalidArgumentException{};
        }
        if (&a == &b) {
            return false;
        }
        return isDefinitelyLessThan(a.time, b.time, 1e6);
    }
    bool operator <= (const timing_t& a, const timing_t& b) {
        if (a.unit != b.unit) {
            throw cpp_utils::exceptions::InvalidArgumentException{};
        }
        if (&a == &b) {
            return true;
        }
        return isDefinitelyLessThan(a.time, b.time, 1e6) || isApproximatelyEqual(a.time, b.time, 1e6);
    }

    bool operator > (const timing_t& a, const timing_t& b) {
        if (a.unit != b.unit) {
            throw cpp_utils::exceptions::InvalidArgumentException{};
        }
        if (&a == &b) {
            return false;
        }
        return isDefinitelyGreaterThan(a.time, b.time, 1e6);
    }

    bool operator >= (const timing_t& a, const timing_t& b) {
        if (a.unit != b.unit) {
            throw cpp_utils::exceptions::InvalidArgumentException{};
        }
        if (&a == &b) {
            return true;
        }
        return isDefinitelyGreaterThan(a.time, b.time, 1e6) || isApproximatelyEqual(a.time, b.time, 1e6);
    }


    namespace internal {

        double getTimeUnitInSeconds(timeunit_e u) {
            switch (u) {
                case timeunit_e::NANO: return 1e-9;
                case timeunit_e::MICRO: return 1e-6;
                case timeunit_e::MILLI: return 1e-3;
                case timeunit_e::SECOND: return 1;
                case timeunit_e::MINUTE: return 60;
                case timeunit_e::HOUR: return 60*60;
                case timeunit_e::DAY: return 24*60*60;
                default: {
                    throw cpp_utils::exceptions::InvalidScenarioException<timeunit_e>{u};
                }
            }
        }

    }

    Timer::Timer(bool start): running{false} {
    #ifdef OS_MAC
        start_time = 0;
        stop_time = 0;
        mach_timebase_info(&timebase);
    #else
        start_time.tv_sec = 0;
        start_time.tv_nsec = 0;
        stop_time.tv_sec = 0;
        stop_time.tv_nsec = 0;
    #endif
    }

    bool Timer::isRunning() const {
        return this->running;
    }

    void Timer::start() {
        if (this->isRunning()) {
            return;
        }
    #ifdef OS_MAC
        start_time = mach_absolute_time();
        stop_time = start_time;
    #else
        clock_gettime(CLOCK_MONOTONIC , &start_time);
        stop_time = start_time;
    #endif
        this->running = true;
    }

    void Timer::stop() {
        if (!this->isRunning()) {
            return;
        }
    #ifdef OS_MAC
        stop_time = mach_absolute_time();
    #else
        clock_gettime(CLOCK_MONOTONIC , &stop_time);
    #endif
        this->running = false;
    }

    timing_t Timer::getCurrentMicroSeconds() const {
    #ifdef OS_MAC
        uint64_t raw_time = mach_absolute_time();
        return timing_t{(double)(raw_time * timebase.numer / timebase.denom), timeunit_e::NANO).toMicros();
    #else
        timespec raw_time;
        clock_gettime(CLOCK_MONOTONIC , &raw_time);
        return timing_t{(double)(raw_time.tv_nsec), timeunit_e::NANO}.toMicros();
    #endif
    }

    timing_t Timer::getCurrentElapsedMicroSeconds() const {
        if (!this->isRunning()) {
            error("timer is not running!");
            throw cpp_utils::exceptions::InvalidStateException<Timer>{*this};
        }
    #ifdef OS_MAC
        uint64_t elapsed_time = mach_absolute_time() - start_time;
        return timing_t{(double)(elapsed_time * timebase.numer / timebase.denom), timeunit_e::NANO}.toMicros();
    #else
        timespec stop;
        clock_gettime(CLOCK_MONOTONIC , &stop);

        if ((stop.tv_nsec-start_time.tv_nsec) < 0)
            return timing_t{(double)(1000000000+stop.tv_nsec-start_time.tv_nsec), timeunit_e::NANO}.toMicros();
        else
            return timing_t{(double)(stop.tv_nsec - start_time.tv_nsec), timeunit_e::NANO}.toMicros();
    #endif
    }

    timing_t Timer::getElapsedMicroSeconds() const {
        if (this->isRunning()) {
            throw cpp_utils::exceptions::InvalidStateException<Timer>{"timer is not running!"};
        }
    #ifdef OS_MAC
        uint64_t elapsed_time = stop_time - start_time;
        return timing_t{(double)(elapsed_time * timebase.numer / timebase.denom), timeunit_e::NANO}.toMicros();
        //Nanoseconds nanosecs = AbsoluteToNanoseconds(*(AbsoluteTime*)&elapsed_time);
        //return (double) UnsignedWideToUInt64(nanosecs) ;
    #else
        if ((stop_time.tv_nsec-start_time.tv_nsec) < 0)
            return timing_t{(double)(1000000000+stop_time.tv_nsec-start_time.tv_nsec), timeunit_e::NANO}.toMicros();
        else
            return timing_t{(double)(stop_time.tv_nsec - start_time.tv_nsec), timeunit_e::NANO}.toMicros();
    #endif
    }

    std::string Timer::toHumanREadable() const {

    }

    void Timer::cleanup() {
    #ifdef OS_MAC
        start_time = 0;
        stop_time = 0;
    #else
        start_time.tv_sec = 0;
        start_time.tv_nsec = 0;
        stop_time.tv_sec = 0;
        stop_time.tv_nsec = 0;
    #endif
        this->running = false;
    }


}