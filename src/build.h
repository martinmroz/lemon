/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#ifndef __lemon_build_h__
#define __lemon_build_h__

#include "lemon.h"

void FindRulePrecedences();
void FindFirstSets();
void FindStates();
void FindLinks();
void FindFollowSets();
void FindActions();

#endif /* defined(__lemon_build_h__) */
