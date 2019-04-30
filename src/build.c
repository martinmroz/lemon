/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#include "build.h"

#include "action.h"
#include "configlist.h"
#include "error.h"
#include "plink.h"
#include "set.h"
#include "struct.h"
#include "table.h"

void FindRulePrecedences(struct lemon *lemp)
{
    struct rule *rp;
    for(rp=lemp->rule; rp; rp=rp->next){
        if( rp->precsym==0 ){
            int i, j;
            for(i=0; i<rp->nrhs && rp->precsym==0; i++){
                struct symbol *sp = rp->rhs[i];
                if( sp->type==MULTITERMINAL ){
                    for(j=0; j<sp->nsubsym; j++){
                        if( sp->subsym[j]->prec>=0 ){
                            rp->precsym = sp->subsym[j];
                            break;
                        }
                    }
                }else if( sp->prec>=0 ){
                    rp->precsym = rp->rhs[i];
                }
            }
        }
    }
    return;
}

void FindFirstSets(struct lemon *lemp)
{
    int i, j;
    struct rule *rp;
    int progress;

    for(i=0; i<lemp->nsymbol; i++){
        lemp->symbols[i]->lambda = LEMON_FALSE;
    }
    for(i=lemp->nterminal; i<lemp->nsymbol; i++){
        lemp->symbols[i]->firstset = SetNew();
    }

    /* First compute all lambdas */
    do{
        progress = 0;
        for(rp=lemp->rule; rp; rp=rp->next){
            if( rp->lhs->lambda ) continue;
            for(i=0; i<rp->nrhs; i++){
                struct symbol *sp = rp->rhs[i];
                if( sp->type!=TERMINAL || sp->lambda==LEMON_FALSE ) break;
            }
            if( i==rp->nrhs ){
                rp->lhs->lambda = LEMON_TRUE;
                progress = 1;
            }
        }
    }while( progress );

    /* Now compute all first sets */
    do{
        struct symbol *s1, *s2;
        progress = 0;
        for(rp=lemp->rule; rp; rp=rp->next){
            s1 = rp->lhs;
            for(i=0; i<rp->nrhs; i++){
                s2 = rp->rhs[i];
                if( s2->type==TERMINAL ){
                    progress += SetAdd(s1->firstset,s2->index);
                    break;
                }else if( s2->type==MULTITERMINAL ){
                    for(j=0; j<s2->nsubsym; j++){
                        progress += SetAdd(s1->firstset,s2->subsym[j]->index);
                    }
                    break;
                }else if( s1==s2 ){
                    if( s1->lambda==LEMON_FALSE ) break;
                }else{
                    progress += SetUnion(s1->firstset,s2->firstset);
                    if( s2->lambda==LEMON_FALSE ) break;
                }
            }
        }
    }while( progress );
    return;
}

PRIVATE struct state *getstate(struct lemon *lemp); /* forward reference */

void FindStates(struct lemon *lemp)
{
    struct symbol *sp;
    struct rule *rp;

    Configlist_init();

    /* Find the start symbol */
    if( lemp->start ){
        sp = Symbol_find(lemp->start);
        if( sp==0 ){
            ErrorMsg(lemp->filename,0,
                     "The specified start symbol \"%s\" is not \
                     in a nonterminal of the grammar.  \"%s\" will be used as the start \
                     symbol instead.",lemp->start,lemp->rule->lhs->name);
            lemp->errorcnt++;
            sp = lemp->rule->lhs;
        }
    }else{
        sp = lemp->rule->lhs;
    }

    /* Make sure the start symbol doesn't occur on the right-hand side of
     ** any rule.  Report an error if it does.  (YACC would generate a new
     ** start symbol in this case.) */
    for(rp=lemp->rule; rp; rp=rp->next){
        int i;
        for(i=0; i<rp->nrhs; i++){
            if( rp->rhs[i]==sp ){   /* FIX ME:  Deal with multiterminals */
                ErrorMsg(lemp->filename,0,
                         "The start symbol \"%s\" occurs on the \
                         right-hand side of a rule. This will result in a parser which \
                         does not work properly.",sp->name);
                lemp->errorcnt++;
            }
        }
    }

    /* The basis configuration set for the first state
     ** is all rules which have the start symbol as their
     ** left-hand side */
    for(rp=sp->rule; rp; rp=rp->nextlhs){
        struct config *newcfp;
        rp->lhsStart = 1;
        newcfp = Configlist_addbasis(rp,0);
        SetAdd(newcfp->fws,0);
    }

    /* Compute the first state.  All other states will be
     ** computed automatically during the computation of the first one.
     ** The returned pointer to the first state is not used. */
    (void)getstate(lemp);
    return;
}

/* Return a pointer to a state which is described by the configuration
 ** list which has been built from calls to Configlist_add.
 */
PRIVATE void buildshifts(struct lemon *, struct state *); /* Forwd ref */
PRIVATE struct state *getstate(struct lemon *lemp)
{
    struct config *cfp, *bp;
    struct state *stp;

    /* Extract the sorted basis of the new state.  The basis was constructed
     ** by prior calls to "Configlist_addbasis()". */
    Configlist_sortbasis();
    bp = Configlist_basis();

    /* Get a state with the same basis */
    stp = State_find(bp);
    if( stp ){
        /* A state with the same basis already exists!  Copy all the follow-set
         ** propagation links from the state under construction into the
         ** preexisting state, then return a pointer to the preexisting state */
        struct config *x, *y;
        for(x=bp, y=stp->bp; x && y; x=x->bp, y=y->bp){
            Plink_copy(&y->bplp,x->bplp);
            Plink_delete(x->fplp);
            x->fplp = x->bplp = 0;
        }
        cfp = Configlist_return();
        Configlist_eat(cfp);
    }else{
        /* This really is a new state.  Construct all the details */
        Configlist_closure(lemp);    /* Compute the configuration closure */
        Configlist_sort();           /* Sort the configuration closure */
        cfp = Configlist_return();   /* Get a pointer to the config list */
        stp = State_new();           /* A new state structure */
        MemoryCheck(stp);
        stp->bp = bp;                /* Remember the configuration basis */
        stp->cfp = cfp;              /* Remember the configuration closure */
        stp->statenum = lemp->nstate++; /* Every state gets a sequence number */
        stp->ap = 0;                 /* No actions, yet. */
        State_insert(stp,stp->bp);   /* Add to the state table */
        buildshifts(lemp,stp);       /* Recursively compute successor states */
    }
    return stp;
}

/*
 ** Return true if two symbols are the same.
 */
int same_symbol(struct symbol *a, struct symbol *b)
{
    int i;
    if( a==b ) return 1;
    if( a->type!=MULTITERMINAL ) return 0;
    if( b->type!=MULTITERMINAL ) return 0;
    if( a->nsubsym!=b->nsubsym ) return 0;
    for(i=0; i<a->nsubsym; i++){
        if( a->subsym[i]!=b->subsym[i] ) return 0;
    }
    return 1;
}

/* Construct all successor states to the given state.  A "successor"
 ** state is any state which can be reached by a shift action.
 */
PRIVATE void buildshifts(
    struct lemon *lemp,
    struct state *stp)   /* The state from which successors are computed */
{
    struct config *cfp;  /* For looping thru the config closure of "stp" */
    struct config *bcfp; /* For the inner loop on config closure of "stp" */
    struct config *new;  /* */
    struct symbol *sp;   /* Symbol following the dot in configuration "cfp" */
    struct symbol *bsp;  /* Symbol following the dot in configuration "bcfp" */
    struct state *newstp; /* A pointer to a successor state */

    /* Each configuration becomes complete after it contibutes to a successor
     ** state.  Initially, all configurations are incomplete */
    for(cfp=stp->cfp; cfp; cfp=cfp->next) cfp->status = INCOMPLETE;

    /* Loop through all configurations of the state "stp" */
        for(cfp=stp->cfp; cfp; cfp=cfp->next){
            if( cfp->status==COMPLETE ) continue;    /* Already used by inner loop */
            if( cfp->dot>=cfp->rp->nrhs ) continue;  /* Can't shift this config */
            Configlist_reset();                      /* Reset the new config set */
            sp = cfp->rp->rhs[cfp->dot];             /* Symbol after the dot */

            /* For every configuration in the state "stp" which has the symbol "sp"
             ** following its dot, add the same configuration to the basis set under
             ** construction but with the dot shifted one symbol to the right. */
            for(bcfp=cfp; bcfp; bcfp=bcfp->next){
                if( bcfp->status==COMPLETE ) continue;    /* Already used */
                if( bcfp->dot>=bcfp->rp->nrhs ) continue; /* Can't shift this one */
                bsp = bcfp->rp->rhs[bcfp->dot];           /* Get symbol after dot */
                if( !same_symbol(bsp,sp) ) continue;      /* Must be same as for "cfp" */
                bcfp->status = COMPLETE;                  /* Mark this config as used */
                new = Configlist_addbasis(bcfp->rp,bcfp->dot+1);
                Plink_add(&new->bplp,bcfp);
            }

            /* Get a pointer to the state described by the basis configuration set
             ** constructed in the preceding loop */
            newstp = getstate(lemp);

            /* The state "newstp" is reached from the state "stp" by a shift action
             ** on the symbol "sp" */
            if( sp->type==MULTITERMINAL ){
                int i;
                for(i=0; i<sp->nsubsym; i++){
                    Action_add(&stp->ap,SHIFT,sp->subsym[i],(char*)newstp);
                }
            }else{
                Action_add(&stp->ap,SHIFT,sp,(char *)newstp);
            }
        }
}

void FindLinks(struct lemon *lemp)
{
    int i;
    struct config *cfp, *other;
    struct state *stp;
    struct plink *plp;

    /* Housekeeping detail:
     ** Add to every propagate link a pointer back to the state to
     ** which the link is attached. */
    for(i=0; i<lemp->nstate; i++){
        stp = lemp->sorted[i];
        for(cfp=stp->cfp; cfp; cfp=cfp->next){
            cfp->stp = stp;
        }
    }

    /* Convert all backlinks into forward links.  Only the forward
     ** links are used in the follow-set computation. */
    for(i=0; i<lemp->nstate; i++){
        stp = lemp->sorted[i];
        for(cfp=stp->cfp; cfp; cfp=cfp->next){
            for(plp=cfp->bplp; plp; plp=plp->next){
                other = plp->cfp;
                Plink_add(&other->fplp,cfp);
            }
        }
    }
}

void FindFollowSets(struct lemon *lemp)
{
    int i;
    struct config *cfp;
    struct plink *plp;
    int progress;
    int change;

    for(i=0; i<lemp->nstate; i++){
        for(cfp=lemp->sorted[i]->cfp; cfp; cfp=cfp->next){
            cfp->status = INCOMPLETE;
        }
    }

    do{
        progress = 0;
        for(i=0; i<lemp->nstate; i++){
            for(cfp=lemp->sorted[i]->cfp; cfp; cfp=cfp->next){
                if( cfp->status==COMPLETE ) continue;
                for(plp=cfp->fplp; plp; plp=plp->next){
                    change = SetUnion(plp->cfp->fws,cfp->fws);
                    if( change ){
                        plp->cfp->status = INCOMPLETE;
                        progress = 1;
                    }
                }
                cfp->status = COMPLETE;
            }
        }
    }while( progress );
}

static int resolve_conflict(
    struct action *apx,
    struct action *apy,
    struct symbol *errsym); /* forward reference */

void FindActions(struct lemon *lemp)
{
    int i,j;
    struct config *cfp;
    struct state *stp;
    struct symbol *sp;
    struct rule *rp;

    /* Add all of the reduce actions
     ** A reduce action is added for each element of the followset of
     ** a configuration which has its dot at the extreme right.
     */
    for(i=0; i<lemp->nstate; i++){   /* Loop over all states */
        stp = lemp->sorted[i];
        for(cfp=stp->cfp; cfp; cfp=cfp->next){  /* Loop over all configurations */
            if( cfp->rp->nrhs==cfp->dot ){        /* Is dot at extreme right? */
                for(j=0; j<lemp->nterminal; j++){
                    if( SetFind(cfp->fws,j) ){
                        /* Add a reduce action to the state "stp" which will reduce by the
                         ** rule "cfp->rp" if the lookahead symbol is "lemp->symbols[j]" */
                        Action_add(&stp->ap,REDUCE,lemp->symbols[j],(char *)cfp->rp);
                    }
                }
            }
        }
    }

    /* Add the accepting token */
    if( lemp->start ){
        sp = Symbol_find(lemp->start);
        if( sp==0 ) sp = lemp->rule->lhs;
    }else{
        sp = lemp->rule->lhs;
    }
    /* Add to the first state (which is always the starting state of the
     ** finite state machine) an action to ACCEPT if the lookahead is the
     ** start nonterminal.  */
    Action_add(&lemp->sorted[0]->ap,ACCEPT,sp,0);

    /* Resolve conflicts */
    for(i=0; i<lemp->nstate; i++){
        struct action *ap, *nap;
        struct state *stp;
        stp = lemp->sorted[i];
        /* assert( stp->ap ); */
        stp->ap = Action_sort(stp->ap);
        for(ap=stp->ap; ap && ap->next; ap=ap->next){
            for(nap=ap->next; nap && nap->sp==ap->sp; nap=nap->next){
                /* The two actions "ap" and "nap" have the same lookahead.
                 ** Figure out which one should be used */
                lemp->nconflict += resolve_conflict(ap,nap,lemp->errsym);
            }
        }
    }

    /* Report an error for each rule that can never be reduced. */
    for(rp=lemp->rule; rp; rp=rp->next) rp->canReduce = LEMON_FALSE;
        for(i=0; i<lemp->nstate; i++){
            struct action *ap;
            for(ap=lemp->sorted[i]->ap; ap; ap=ap->next){
                if( ap->type==REDUCE ) ap->x.rp->canReduce = LEMON_TRUE;
            }
        }
    for(rp=lemp->rule; rp; rp=rp->next){
        if( rp->canReduce ) continue;
        ErrorMsg(lemp->filename,rp->ruleline,"This rule can not be reduced.\n");
        lemp->errorcnt++;
    }
}

/* Resolve a conflict between the two given actions.  If the
 ** conflict can't be resolve, return non-zero.
 **
 ** NO LONGER TRUE:
 **   To resolve a conflict, first look to see if either action
 **   is on an error rule.  In that case, take the action which
 **   is not associated with the error rule.  If neither or both
 **   actions are associated with an error rule, then try to
 **   use precedence to resolve the conflict.
 **
 ** If either action is a SHIFT, then it must be apx.  This
 ** function won't work if apx->type==REDUCE and apy->type==SHIFT.
 */
static int resolve_conflict(
    struct action *apx,
    struct action *apy,
    struct symbol *errsym)   /* The error symbol (NULL otherwise) */
{
    UNUSED_ARG(errsym);
    struct symbol *spx, *spy;
    int errcnt = 0;
    assert( apx->sp==apy->sp );  /* Otherwise there would be no conflict */
    if( apx->type==SHIFT && apy->type==SHIFT ){
        apy->type = SSCONFLICT;
        errcnt++;
    }
    if( apx->type==SHIFT && apy->type==REDUCE ){
        spx = apx->sp;
        spy = apy->x.rp->precsym;
        if( spy==0 || spx->prec<0 || spy->prec<0 ){
            /* Not enough precedence information. */
            apy->type = SRCONFLICT;
            errcnt++;
        }else if( spx->prec>spy->prec ){    /* Lower precedence wins */
            apy->type = RD_RESOLVED;
        }else if( spx->prec<spy->prec ){
            apx->type = SH_RESOLVED;
        }else if( spx->prec==spy->prec && spx->assoc==RIGHT ){ /* Use operator */
            apy->type = RD_RESOLVED;                             /* associativity */
        }else if( spx->prec==spy->prec && spx->assoc==LEFT ){  /* to break tie */
            apx->type = SH_RESOLVED;
        }else{
            assert( spx->prec==spy->prec && spx->assoc==NONE );
            apy->type = SRCONFLICT;
            errcnt++;
        }
    }else if( apx->type==REDUCE && apy->type==REDUCE ){
        spx = apx->x.rp->precsym;
        spy = apy->x.rp->precsym;
        if( spx==0 || spy==0 || spx->prec<0 ||
           spy->prec<0 || spx->prec==spy->prec ){
            apy->type = RRCONFLICT;
            errcnt++;
        }else if( spx->prec>spy->prec ){
            apy->type = RD_RESOLVED;
        }else if( spx->prec<spy->prec ){
            apx->type = RD_RESOLVED;
        }
    }else{
        assert(
               apx->type==SH_RESOLVED ||
               apx->type==RD_RESOLVED ||
               apx->type==SSCONFLICT ||
               apx->type==SRCONFLICT ||
               apx->type==RRCONFLICT ||
               apy->type==SH_RESOLVED ||
               apy->type==RD_RESOLVED ||
               apy->type==SSCONFLICT ||
               apy->type==SRCONFLICT ||
               apy->type==RRCONFLICT
               );
        /* The REDUCE/SHIFT case cannot happen because SHIFTs come before
         ** REDUCEs on the list.  If we reach this point it must be because
         ** the parser conflict had already been resolved. */
    }
    return errcnt;
}
