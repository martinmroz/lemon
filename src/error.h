/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#ifndef __lemon_error_h__
#define __lemon_error_h__

#include "lemon.h"

enum {
    LINENO_NONE = 0
};

typedef enum e_loglevel {
    LOGLEVEL_INFO = 0,
    LOGLEVEL_WARNING,
    LOGLEVEL_ERROR,
    LOGLEVEL_COUNT
} loglevel;

/**
 * Report a message to standard output.
 *
 * @param filename The name of the input file.
 * @param lineno The line number within the input file to which the message corresponds, or LINENO_NONE.
 * @param format The printf-style format string for the error.
 */
void LogMsg(loglevel level, const char *filename, int lineno, const char *format, ...);

/**
 * Report an error message to standard output.
 *
 * @param filename The name of the input file.
 * @param lineno The line number within the input file to which the error corresponds, or LINENO_NONE.
 * @param format The printf-style format string for the error.
 */
void ErrorMsg(const char *filename, int lineno, const char *format, ...);

#endif /* defined(__lemon_error_h__) */
