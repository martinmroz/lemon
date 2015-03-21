/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

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

/** Allocate a new parser action */
struct action *Action_new(void);

/** Sort parser actions */
struct action *Action_sort(struct action *ap);

/** Adds a new parser action to the table. */
void Action_add(struct action **app, enum e_action type, struct symbol *sp, char *arg);

/* Return the number of entries in the yy_action table */
#define acttab_size(X) ((X)->nAction)

/* The value for the N-th entry in yy_action */
#define acttab_yyaction(X,N)  ((X)->aAction[N].action)

/* The value for the N-th entry in yy_lookahead */
#define acttab_yylookahead(X,N)  ((X)->aAction[N].lookahead)

/** Allocates a new action table */
acttab *acttab_alloc(void);

/** Free all memory associated with the given acttab */
void acttab_free(acttab *p);

/** Add a new action to the current transaction set */
void acttab_action(acttab *p, int lookahead, int action);

/**
 * Add the transaction set built up with prior calls to acttab_action()
 * into the current action table.  Then reset the transaction set back
 * to an empty set in preparation for a new round of acttab_action() calls.
 *
 * Return the offset into the action table of the new transaction.
 */
int acttab_insert(acttab *p);

#endif /* __lemon_action_h__ */
