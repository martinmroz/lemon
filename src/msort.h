/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#ifndef __lemon_msort_h__
#define __lemon_msort_h__

#include "lemon.h"

/*
 ** Inputs:
 **   list:      Pointer to a singly-linked list of structures.
 **   next:      Pointer to pointer to the second element of the list.
 **   cmp:       A comparison function.
 **
 ** Return Value:
 **   A pointer to the head of a sorted list containing the elements
 **   orginally in list.
 **
 ** Side effects:
 **   The "next" pointers for elements in list are changed.
 */
char *msort(
            char *list,
            char **next,
            int (*cmp)(const char*,const char*)
            );

#endif /* defined(__lemon_msort_h__) */
