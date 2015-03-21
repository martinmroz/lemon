/*
 *  Created by Martin Mroz on 3/21/15.
 *  Copyright (c) 2015 Martin Mroz. All rights reserved.
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#ifndef __WIN32__
#   if defined(_WIN32) || defined(WIN32)
#	define __WIN32__
#   endif
#endif

#ifdef __WIN32__
extern int access();
#else
#include <unistd.h>
#endif

#define PRIVATE static

#ifdef TEST
#define MAXRHS 5       /* Set low to exercise exception code */
#else
#define MAXRHS 1000
#endif

extern int nDefine;      /* Number of -D options on the command line */
extern char **azDefine;  /* Name of the -D macros */

#define LANG_C		0
#define LANG_CPP	1
#define LANG_D		2
extern int language;     /* Output language */
