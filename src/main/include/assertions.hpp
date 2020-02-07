/**
 * @file 
 * @author Massimo Bono
 * @brief a file containing all the assertions as prcessing macrso
 * @version 0.1
 * @date 2020-02-07
 * 
 * I've decided to cerate this file because the assertion in `<cassert>` are somewhat lacking.
 * 
 * @attention
 * All the assertion as enabled only if `NDEBUG` macro is **not** defined!
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _CPPUTILS_ASSERTIONS_HEADER__
#define _CPPUTILS_ASSERTIONS_HEADER__

#include "log.hpp"
#include "macros.hpp"

#define _ASSERTION_FAILED_EXITCODE 90

#ifndef NDEBUG

/**
 * @brief define what happens when an assertion fails
 * 
 */
#define assertionFailed(...) \
    log_error(__VA_ARGS__); \
    exit(_ASSERTION_FAILED_EXITCODE) 

/**
 * @brief  ensure a condition happens in runtime
 * 
 * @note
 * the condition should not have side effects
 * 
 */
#define assertHappens(condition) \
    if (!(condition)) { \
        assertionFailed(TO_STRING(condition), "is false."); \
    } 

/**
 * @brief ensure a condition does not happen in runtime
 * 
 * @note
 * the condition should not have side effects
 * 
 */
#define assertDoNotHappen(condition) \
    if ((condition)) { \
        assertionFailed(TO_STRING(condition), "is true."); \
    } 

/**
 * @brief ensure that 2 quantities are the same
 * 
 */
#define assertEqual(x, y) \
    if ((x) != (y)) { \
        assertionFailed(TO_STRING(x), "!=", TO_STRING(y), " failed. x= ", TO_STRING(x), " y= ", TO_STRING(y)); \
    } \

/**
 * @brief ensure that 2 quantities are **not** the same
 * 
 */
#define assertNotEqual(x, y) \
    if ((x) == (y)) { \
        assertionFailed(TO_STRING(x), "==", TO_STRING(y), " failed. x= ", TO_STRING(x), " y= ", TO_STRING(y)); \
    } \

/**
 * @brief ensure that the value is inside 2 bound
 * 
 * @param lb lowerbound of the range. Should be idempotent
 * @param x value to check. Should be idempotent
 * @param ub upperbound of the range. Should be idempotent
 * @param lb_included true if the lowerbound should be included in the range; false otherwise
 * @param ub_included true if the upperbound should be included in the range; false otherwise
 * 
 */
#define assertInRange(lb, x, ub, lb_included, ub_included) \
    if ((!ub_included) && ((x) == (lb))) { \
        assertionFailed(TO_STRING(x), "==", TO_STRING(lb), "but ", TO_STRING(lb), "is not in range!"); \
    } \
    if ((!ub_included) && ((x) == (ub))) { \
        assertionFailed(TO_STRING(x), "==", TO_STRING(ub), "but ", TO_STRING(ub), "is not in range!"); \
    } \
    if ((x) < (lb)) { \
        assertionFailed(TO_STRING(x), "<", TO_STRING(lb)); \
    } \
    if ((x) > (ub)) { \
        assertionFailed(TO_STRING(x), ">", TO_STRING(ub)); \
    }

#else
#define assertionFailed(...) ;
#define assertHappens(condition) ;
#define assertDoNotHappen(condition) ;

#define assertEqual(x, y) ;
#define assertNotEqual(x, y) ;

#define assertInRange(lb, x, ub, lb_included, ub_included) ;

#endif


#endif