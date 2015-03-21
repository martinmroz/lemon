/*
 *  Created by Martin Mroz on 3/21/15.
 *  Copyright (c) 2015 Martin Mroz. All rights reserved.
 */

#ifndef __lemon_option_h__
#define __lemon_option_h__

#include "lemon.h"

/****** From the file "option.h" ******************************************/
struct s_options {
    enum { OPT_FLAG=1,  OPT_INT,  OPT_DBL,  OPT_STR,
        OPT_FFLAG, OPT_FINT, OPT_FDBL, OPT_FSTR} type;
    char *label;
    char *arg;
    char *message;
};
int    OptInit(/* char**,struct s_options*,FILE* */);
int    OptNArgs(/* void */);
char  *OptArg(/* int */);
void   OptErr(/* int */);
void   OptPrint(/* void */);

#endif /* defined(__lemon_option_h__) */
