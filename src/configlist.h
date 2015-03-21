/*
 *  Created by Martin Mroz on 3/21/15.
 *  Copyright (c) 2015 Martin Mroz. All rights reserved.
 */

#ifndef __lemon_configlist_h__
#define __lemon_configlist_h__

#include "lemon.h"

#include "struct.h"

void Configlist_init(/* void */);
struct config *Configlist_add(/* struct rule *, int */);
struct config *Configlist_addbasis(/* struct rule *, int */);
void Configlist_closure(/* void */);
void Configlist_sort(/* void */);
void Configlist_sortbasis(/* void */);
struct config *Configlist_return(/* void */);
struct config *Configlist_basis(/* void */);
void Configlist_eat(/* struct config * */);
void Configlist_reset(/* void */);

#endif /* defined(__lemon_configlist_h__) */
