//*******************************************************************
// <float.h>             Copyright (C) Codemist 2007
//
// Modfied by Code Red Technologies Ltd and NXP
// Copyright (c) 2008-2013 Code Red Technologies Ltd.
// Copyright 2013-2019 NXP
//*******************************************************************

#ifndef __FLOAT_H_INCLUDED
#define __FLOAT_H_INCLUDED

#include <sys/redlib_version.h>

#ifndef __CODEMIST
#define __CODEMIST
#endif


#define FLT_RADIX     2
    /* radix of exponent representation */
#define FLT_ROUNDS    1
    /*
     * The rounding mode for floating-point addition is characterised by the
     * value of FLT_ROUNDS:
     *  -1 : indeterminable.
     *   0 : towards zero.
     *   1 : to nearest.
     *   2 : towards positive infinity.
     *   3 : towards negative infinity.
     *   ? : any other is implementation-defined.
     */

#define FLT_MANT_DIG        24
    /* number of base-FLT_RADIX digits in the floating point mantissa */

/* The values that follow are not achieved under Acorn's FPE version 17  */
/* but they should be correct in due course!                             */

#define FLT_DIG      6
    /* number of decimal digits of precision */

#define FLT_MIN_EXP  (-125)
    /* minimum negative integer such that FLT_RADIX raised to that power */
    /* minus 1 is a normalised floating-point number. */

#define FLT_MIN_10_EXP  (-37)
    /* minimum negative integer such that 10 raised to that power is in the */
    /* range of normalised floating-point numbers. */

#define FLT_MAX_EXP  128
    /* maximum integer such that FLT_RADIX raised to that power minus 1 is a */

#define FLT_MAX_10_EXP  38
    /* maximum integer such that 10 raised to that power is in the range of */
    /* representable finite floating-point numbers. */

#define FLT_MAX  3.40282347e+38F
    /* maximum representable finite floating-point number. */

#define FLT_EPSILON         1.19209290e-7F
    /* minimum positive floating point number x such that 1.0 + x != 1.0 */

#define FLT_MIN  1.17549435e-38F
    /* minimum normalised positive floating-point number. */



#define FLT_MANT_DIG        24
#define DBL_MANT_DIG        53
#define LDBL_MANT_DIG       53
    /* number of base-FLT_RADIX digits in the floating point mantissa */

/* The values that follow are not achieved under Acorn's FPE version 17  */
/* but they should be correct in due course!                             */

#define FLT_DIG      6
#define DBL_DIG      15
#define LDBL_DIG     15
    /* number of decimal digits of precision */

#define FLT_MIN_EXP  (-125)
#define DBL_MIN_EXP  (-1021)
#define LDBL_MIN_EXP (-1021)
    /* minimum negative integer such that FLT_RADIX raised to that power */
    /* minus 1 is a normalised floating-point number. */

#define FLT_MIN_10_EXP  (-37)
#define DBL_MIN_10_EXP  (-307)
#define LDBL_MIN_10_EXP (-307)
    /* minimum negative integer such that 10 raised to that power is in the */
    /* range of normalised floating-point numbers. */

#define FLT_MAX_EXP  128
#define DBL_MAX_EXP  1024
#define LDBL_MAX_EXP 1024
    /* maximum integer such that FLT_RADIX raised to that power minus 1 is a */
#define FLT_MAX_10_EXP  38
#define DBL_MAX_10_EXP  308
#define LDBL_MAX_10_EXP 308
    /* maximum integer such that 10 raised to that power is in the range of */
    /* representable finite floating-point numbers. */

#define FLT_MAX  3.40282347e+38F
#define DBL_MAX  1.79769313486231571e+308
#define LDBL_MAX 1.79769313486231571e+308L
    /* maximum representable finite floating-point number. */

#define FLT_EPSILON         1.19209290e-7F
#define DBL_EPSILON         2.2204460492503131e-16
#define LDBL_EPSILON        2.2204460492503131e-16L
    /* minimum positive floating point number x such that 1.0 + x != 1.0 */

#define FLT_MIN  1.17549435e-38F
#define DBL_MIN  2.22507385850720138e-308
#define LDBL_MIN 2.22507385850720138e-308L
    /* minimum normalised positive floating-point number. */



#define DBL_MANT_DIG        53
#define LDBL_MANT_DIG       53
    /* number of base-FLT_RADIX digits in the floating point mantissa */

#define DBL_DIG      15
#define LDBL_DIG     15
    /* number of decimal digits of precision */

#define DBL_MIN_EXP  (-1021)
#define LDBL_MIN_EXP (-1021)
    /* minimum negative integer such that FLT_RADIX raised to that power */
    /* minus 1 is a normalised floating-point number. */

#define DBL_MIN_10_EXP  (-307)
#define LDBL_MIN_10_EXP (-307)
    /* minimum negative integer such that 10 raised to that power is in the */
    /* range of normalised floating-point numbers. */

#define DBL_MAX_EXP  1024
#define LDBL_MAX_EXP 1024
    /* maximum integer such that FLT_RADIX raised to that power minus 1 is a */

#define DBL_MAX_10_EXP  308
#define LDBL_MAX_10_EXP 308
    /* maximum integer such that 10 raised to that power is in the range of */
    /* representable finite floating-point numbers. */

#define DBL_MAX  1.79769313486231571e+308
#define LDBL_MAX 1.79769313486231571e+308L
    /* maximum representable finite floating-point number. */

#define DBL_EPSILON         2.2204460492503131e-16
#define LDBL_EPSILON        2.2204460492503131e-16L
    /* minimum positive floating point number x such that 1.0 + x != 1.0 */

#define DBL_MIN  2.22507385850720138e-308
#define LDBL_MIN 2.22507385850720138e-308L
    /* minimum normalised positive floating-point number. */


// Set C99 FLT_EVAL_METHOD to value provided by GCC defined macro
#define FLT_EVAL_METHOD	__FLT_EVAL_METHOD__

#endif /* __FLOAT_H_INCLUDED */

/* end of <float.h> */
