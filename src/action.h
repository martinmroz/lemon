//
//  action.h
//  lemon
//
//  Created by Martin Mroz on 3/21/15.
//  Copyright (c) 2015 Martin Mroz. All rights reserved.
//

#ifndef __lemon_action_h__
#define __lemon_action_h__

#include "lemon.h"

#include "struct.h"

/*
 ** The state of the yy_action table under construction is an instance of
 ** the following structure
 */
typedef struct acttab acttab;
struct acttab {
    int nAction;                 /* Number of used slots in aAction[] */
    int nActionAlloc;            /* Slots allocated for aAction[] */
    struct {
        int lookahead;             /* Value of the lookahead token */
        int action;                /* Action to take on the given lookahead */
    } *aAction,                  /* The yy_action[] table under construction */
    *aLookahead;               /* A single new transaction set */
    int mnLookahead;             /* Minimum aLookahead[].lookahead */
    int mnAction;                /* Action associated with mnLookahead */
    int mxLookahead;             /* Maximum aLookahead[].lookahead */
    int nLookahead;              /* Used slots in aLookahead[] */
    int nLookaheadAlloc;         /* Slots allocated in aLookahead[] */
};

/* Return the number of entries in the yy_action table */
#define acttab_size(X) ((X)->nAction)

/* The value for the N-th entry in yy_action */
#define acttab_yyaction(X,N)  ((X)->aAction[N].action)

/* The value for the N-th entry in yy_lookahead */
#define acttab_yylookahead(X,N)  ((X)->aAction[N].lookahead)

/* Allocate a new parser action */
struct action *Action_new(void);

/* Sort parser actions */
struct action *Action_sort(struct action *ap);

/* Adds a new parser action to the table. */
void Action_add(struct action **app, enum e_action type, struct symbol *sp, char *arg);

void acttab_free(acttab *p);

acttab *acttab_alloc(void);

void acttab_action(acttab *p, int lookahead, int action);

int acttab_insert(acttab *p);

#endif /* __lemon_action_h__ */
