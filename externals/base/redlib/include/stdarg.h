/*
 * 
 * Use the standard GCC variable argument builtins.
 * 
 */

//*****************************************************************************
//   +--+
//   | ++----+
//   +-++    |
//     |     |
//   +-+--+  |
//   | +--+--+
//   +----+    Copyright 2008, 2010-2011 Code Red Technologies Ltd.
//             Copyright 2017, 2019 NXP
//
//*****************************************************************************

#ifndef _STDARG_H
#define _STDARG_H
#endif

#include <sys/redlib_version.h>

typedef __builtin_va_list va_list;

#define va_start(v,l) __builtin_va_start((v),l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v,l)__builtin_va_arg((v),l)
