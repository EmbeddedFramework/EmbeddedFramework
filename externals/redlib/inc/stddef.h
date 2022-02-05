//*******************************************************************
// <stddef.h>             Copyright (C) Codemist 2007
//
// Modfied by Code Red Technologies Ltd and NXP
// Copyright 2008-2013 Code Red Technologies Ltd
// Copyright 2013-2014, 2017, 2019 NXP
//*******************************************************************

#ifndef __STDDEF_H_INCLUDED
#define __STDDEF_H_INCLUDED

#include <sys/redlib_version.h>

#ifndef __CODEMIST
#define __CODEMIST
#endif

#include <sys/libconfig.h>

typedef int ptrdiff_t;

#ifndef WCHAR_T_DEFINED
#define WCHAR_T_DEFINED
typedef __WCHAR_TYPE__ wchar_t;
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define offsetof(type, member_designator) \
    ((size_t)((char *)&(((type *)0)->member_designator) - (char *)0))

#endif /* __STDDEF_H_INCLUDED */

/* end of <stddef.h> */
