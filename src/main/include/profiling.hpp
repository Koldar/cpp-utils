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

/**
 * @brief like `PROFILE_TIME` but we print the result on log, with CRITICAL log level
 * 
 * @param[in] description of the timing
 */
#define PROFILE_TIME_AND_PRINT(description) \
    for(bool profile=true; profile;) \
        for ( \
            Timer timer{true} \
            ; \
            profile \
            ; \
            timer.stop(), \
            critical(description, "=", timer.getElapsedMicroSeconds().toHumanReadable()), \
            profile=false \
        )
    

#endif 