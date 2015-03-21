/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#include "error.h"

static void LogMsgv(loglevel level, const char *filename, int lineno, const char *format, va_list ap)
{
    /* If an invalid log level is specified, default to error. */
    if (level >= LOGLEVEL_COUNT) {
        level = LOGLEVEL_ERROR;
    }
    
    static char *const levelStrings[] = {
        /* LOGLEVEL_INFO    */ "info",
        /* LOGLEVEL_WARNING */ "warning",
        /* LOGLEVEL_ERROR   */ "error",
        /* LOGLEVEL_COUNT   */ NULL
    };
    
    char *const levelString = levelStrings[(int)level];
    
    /* Prepare a prefix to be prepended to every output line */
    if (lineno > 0) {
        fprintf(stdout, "%s:%d: %s: ", filename, lineno, levelString);
    } else {
        fprintf(stdout, "%s: %s: ", filename, levelString);
    }
    
    /* Generate the error message */
    vfprintf(stdout, format, ap);
}

void LogMsg(loglevel level, const char *filename, int lineno, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    LogMsgv(level, filename, lineno, format, ap);
    va_end(ap);
}

void ErrorMsg(const char *filename, int lineno, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    LogMsgv(LOGLEVEL_ERROR, filename, lineno, format, ap);
    va_end(ap);
}
