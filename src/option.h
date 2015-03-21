/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#ifndef __lemon_option_h__
#define __lemon_option_h__

#include "lemon.h"

struct s_options {
    enum { OPT_FLAG=1,  OPT_INT,  OPT_DBL,  OPT_STR,
        OPT_FFLAG, OPT_FINT, OPT_FDBL, OPT_FSTR} type;
    char *label;
    char *arg;
    char *message;
};
int    OptInit(char**,struct s_options*,FILE*);
int    OptNArgs(void);
char  *OptArg(int);
void   OptErr(int);
void   OptPrint(void);

#endif /* defined(__lemon_option_h__) */
