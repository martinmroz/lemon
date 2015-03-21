/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#ifndef __lemon_set_h__
#define __lemon_set_h__

#include "lemon.h"

/** All sets will be of size N */
void  SetSize(int N);

/** A new set for element 0..N */
char *SetNew(void);

/** Deallocate a set */
void  SetFree(char*);

/** Add element to a set */
int SetAdd(char*,int);

/** A <- A U B, thru element N */
int SetUnion(char *A,char *B);

/** True if Y is in set X */
#define SetFind(X,Y) (X[Y])

#endif /* defined(__lemon_set_h__) */
