//
//  set.h
//  lemon
//
//  Created by Martin Mroz on 3/21/15.
//  Copyright (c) 2015 Martin Mroz. All rights reserved.
//

#ifndef __lemon_set_h__
#define __lemon_set_h__

#include "lemon.h"

/********** From the file "set.h" ****************************************/
void  SetSize(/* int N */);             /* All sets will be of size N */
char *SetNew(/* void */);               /* A new set for element 0..N */
void  SetFree(/* char* */);             /* Deallocate a set */

int SetAdd(/* char*,int */);            /* Add element to a set */
int SetUnion(/* char *A,char *B */);    /* A <- A U B, thru element N */

#define SetFind(X,Y) (X[Y])       /* True if Y is in set X */

#endif /* defined(__lemon_set_h__) */