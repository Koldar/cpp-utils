#ifndef _CPP_UTILS_PROFILING_HEADER__
#define _CPP_UTILS_PROFILING_HEADER__

#include "Timer.hpp"

/**
 * @brief automatica time profile
 * 
 * @code
 * timing_t timeElapsed;
 * PROFILE_TIME(timeElapsed) {
 *      stuff_to_time();
 * }
 * info("you took", timeElapsed);
 * @endcode
 * 
 * 
 */
#define PROFILE_TIME(value) \
    for(bool profile=true; profile;) \
        for (Timer timer{true}; profile; value=timer.getCurrentElapsedMicroSeconds(), timer.stop(), profile=false)


    

#endif 