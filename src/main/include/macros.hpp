/*
 * macros.h
 *
 *  Created on: Oct 1, 2018
 *      Author: koldar
 */

#ifndef MACROS_H_
#define MACROS_H_

/**
 * @brief declare that a class @c X should extends a class @c Y
 * 
 * @code
 * #include <type_traits>
 * //T typename should extends Foo class
 * template<EXTENDS(T, Foo)>
 * class Bar {
 * };
 * @endcode
 * 
 * @param X the template parameter which needs to extends with class @c Y
 * @param Y the class which neesd to be superclass of class @c X
 * @see https://stackoverflow.com/a/30687399/1887602
 */
#define EXTENDS(X, Y) typename std::enable_if<std::is_base_of<Y, X>::value>::type* = nullptr

/**
 * @brief perform the elvis operator
 * 
 * If X is not null we will return <tt>*X</tt> otherwise we will return @c ELSE
 * 
 * @code
 *  int* a = nullptr;
 *  int c = 3;
 *  int* b = &c;
 *  ELVIS(a, 5); //return 5
 *  ELVIS(b, 5); //return 3
 * @endcode
 * 
 * @param[in] X a pointer to check
 * @param[in] ELSE statement to execute if X is null
 */
#define ELVIS(X, ELSE) (((X) != nullptr) ? *(X) : (ELSE))

#define CAT(x, y) x ## y

#define PASTE(x, y) CAT(x, y)

/**
 * Count the arguments in a variadic macro
 *
 * @code
 * COUNT_ARGS(a) //1
 * COUNT_ARGS(a,b) //2
 * @endcode
 *
 * @param[in] ... params to count
 */
#define COUNT_ARGS(...) _COUNT_ARGS(__VA_ARGS__,_PP_RSEQ_N())
#define _COUNT_ARGS(...) _PP_ARG_N(__VA_ARGS__)
#define _PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N

#define _PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

/**
 * Execute an operation for each element of a list
 *
 * Here's an example
 * @code
 * #define OPERATION_SUM_MAP(context, index, x) (x*x)
 * #define OPERATION_SUM_COMBINE(context, x, y) x + y
 *
 * FOR_EACH(,OPERATION_SUM_MAP, OPERATION_MAP_COMBINE, 1, 2, 3, 4);
 * //outputs (1*1)+(2*2)+(3*3)+(4*4)
 * @endcode
 *
 * @attention
 *  empty variadic is not support
 *
 * @param[in] CONTEXT a macro that will be available to both the mapping macro and the combine macro
 * @param[in] OP a macro telling us what we need to do with a single value in the variadic
 * @param[in] COMB a macro telling us how to combine 2 values previously generated by OP macro
 */
#define FOR_EACH(CONTEXT, OP, COMB, ...) PASTE(_FOR_EACH_, COUNT_ARGS(__VA_ARGS__))(CONTEXT, OP, COMB, __VA_ARGS__)

#define _FOR_EACH_1(CONTEXT, OP, COMB, x) OP(CONTEXT, 1, x)
#define _FOR_EACH_2(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 2, x),  _FOR_EACH_1(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_3(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 3, x),  _FOR_EACH_2(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_4(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 4, x),  _FOR_EACH_3(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_5(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 5, x),  _FOR_EACH_4(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_6(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 6, x),  _FOR_EACH_5(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_7(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 7, x),  _FOR_EACH_6(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_8(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 8, x),  _FOR_EACH_7(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_9(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 9, x),  _FOR_EACH_8(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_10(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 10, x),  _FOR_EACH_9(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_11(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 11, x),  _FOR_EACH_10(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_12(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 12, x),  _FOR_EACH_11(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_13(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 13, x),  _FOR_EACH_12(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_14(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 14, x),  _FOR_EACH_13(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_15(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 15, x),  _FOR_EACH_14(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_16(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 16, x),  _FOR_EACH_15(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_17(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 17, x),  _FOR_EACH_16(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_18(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 18, x),  _FOR_EACH_17(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_19(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 19, x),  _FOR_EACH_18(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_20(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 20, x),  _FOR_EACH_19(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_21(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 21, x),  _FOR_EACH_20(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_22(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 22, x),  _FOR_EACH_21(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_23(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 23, x),  _FOR_EACH_22(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_24(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 24, x),  _FOR_EACH_23(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_25(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 25, x),  _FOR_EACH_24(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_26(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 26, x),  _FOR_EACH_25(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_27(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 27, x),  _FOR_EACH_26(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_28(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 28, x),  _FOR_EACH_27(CONTEXT, OP, COMB, __VA_ARGS__))
#define _FOR_EACH_29(CONTEXT, OP, COMB, x, ...) COMB(CONTEXT, OP(CONTEXT, 29, x),  _FOR_EACH_28(CONTEXT, OP, COMB, __VA_ARGS__))

/**
 * @attention
 *  empty variadic is not support
 *
 * @return the first item of a variadic sequence
 */
#define FIRST_ELEMENT(x, ...) x

/**
 * Executive the statements only if the pointer is not NULL
 *
 * @code
 * int* a = ...
 * DO_IF_NOT_NULL(a) {
 * 	std::cout << " a is " << *a << std::endl;
 * }
 * @endcode
 *
 * @param[in] x the pointer to check
 */
#define DO_IF_NOT_NULL(x) if (x != nullptr)


#endif /* MACROS_H_ */
