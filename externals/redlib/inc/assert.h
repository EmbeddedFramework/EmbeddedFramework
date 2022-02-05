/*
 * <assert.h>             Copyright (C) Codemist 2007
 *
 *                        A C Norman
 *
 */
//*****************************************************************************
//
// Modfied by Code Red Technologies Ltd and NXP
//
//   +--+
//   | ++----+
//   +-++    |
//     |     |
//   +-+--+  |
//   | +--+--+
//   +----+    Copyright 2010-2011 Code Red Technologies Ltd.
//             Copyright 2017, 2019 NXP
//
//*****************************************************************************

#include <sys/redlib_version.h>

#ifndef __CODEMIST
#define __CODEMIST
#endif

void __assertion_failed(char *_Expr) __attribute__ ((noreturn));

/*
 * It is legal to reset NDEBUG and re-include <assert.h> hence the
 * fact that this bit gets done every time it is included. This
 * implementation follows the Rationale to the C99 standard.
 */

#ifdef NDEBUG
#  define assert(ignore) ((void)0)
#else
#define _ASSERT_STR(x) _ASSERT_VAL(x)
#define _ASSERT_VAL(x) #x
#define assert(expr)   \
  ((expr) ? (void)0 :  \
		  __assertion_failed(__FILE__ ":" _ASSERT_STR(__LINE__) " : " #expr))
#endif
/* end of assert.h */
