/*
 *  Created by Martin Mroz on 3/21/15.
 *  Copyright (c) 2015 Martin Mroz. All rights reserved.
 */

#ifndef __lemon_plink_h__
#define __lemon_plink_h__

#include "lemon.h"

#include "struct.h"

struct plink *Plink_new(/* void */);
void Plink_add(/* struct plink **, struct config * */);
void Plink_copy(/* struct plink **, struct plink * */);
void Plink_delete(/* struct plink * */);

#endif /* __lemon_plink_h__ */
