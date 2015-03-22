/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#include "configlist.h"

#include "error.h"
#include "msort.h"
#include "plink.h"
#include "set.h"
#include "table.h"

/*
 ** Routines to processing a configuration list and building a state
 ** in the LEMON parser generator.
 */

static struct config *freelist = 0;      /* List of free configurations */
static struct config *current = 0;       /* Top of list of configurations */
static struct config **currentend = 0;   /* Last on list of configs */
static struct config *basis = 0;         /* Top of list of basis configs */
static struct config **basisend = 0;     /* End of list of basis configs */

/* Return a pointer to a new configuration */
PRIVATE struct config *newconfig(){
    struct config *new;
    if( freelist==0 ){
        int i;
        int amt = 3;
        freelist = (struct config *)calloc( amt, sizeof(struct config) );
        if( freelist==0 ){
            ErrorMsg("lemon", LINENO_NONE, "Unable to allocate memory for a new configuration.");
            exit(1);
        }
        for(i=0; i<amt-1; i++) freelist[i].next = &freelist[i+1];
        freelist[amt-1].next = 0;
    }
    new = freelist;
    freelist = freelist->next;
    return new;
}

/* The configuration "old" is no longer used */
PRIVATE void deleteconfig(old)
struct config *old;
{
    old->next = freelist;
    freelist = old;
}

/* Initialized the configuration list builder */
void Configlist_init(){
    current = 0;
    currentend = &current;
    basis = 0;
    basisend = &basis;
    Configtable_init();
    return;
}

/* Initialized the configuration list builder */
void Configlist_reset(){
    current = 0;
    currentend = &current;
    basis = 0;
    basisend = &basis;
    Configtable_clear(0);
    return;
}

/* Add another configuration to the configuration list */
struct config *Configlist_add(rp,dot)
struct rule *rp;    /* The rule */
int dot;            /* Index into the RHS of the rule where the dot goes */
{
    struct config *cfp, model;
    
    assert( currentend!=0 );
    model.rp = rp;
    model.dot = dot;
    cfp = Configtable_find(&model);
    if( cfp==0 ){
        cfp = newconfig();
        cfp->rp = rp;
        cfp->dot = dot;
        cfp->fws = SetNew();
        cfp->stp = 0;
        cfp->fplp = cfp->bplp = 0;
        cfp->next = 0;
        cfp->bp = 0;
        *currentend = cfp;
        currentend = &cfp->next;
        Configtable_insert(cfp);
    }
    return cfp;
}

/* Add a basis configuration to the configuration list */
struct config *Configlist_addbasis(struct rule *rp, int dot)
{
    struct config *cfp, model;
    
    assert( basisend!=0 );
    assert( currentend!=0 );
    model.rp = rp;
    model.dot = dot;
    cfp = Configtable_find(&model);
    if( cfp==0 ){
        cfp = newconfig();
        cfp->rp = rp;
        cfp->dot = dot;
        cfp->fws = SetNew();
        cfp->stp = 0;
        cfp->fplp = cfp->bplp = 0;
        cfp->next = 0;
        cfp->bp = 0;
        *currentend = cfp;
        currentend = &cfp->next;
        *basisend = cfp;
        basisend = &cfp->bp;
        Configtable_insert(cfp);
    }
    return cfp;
}

/* Compute the closure of the configuration list */
void Configlist_closure(struct lemon *lemp)
{
    struct config *cfp, *newcfp;
    struct rule *rp, *newrp;
    struct symbol *sp, *xsp;
    int i, dot;
    
    assert( currentend!=0 );
    for(cfp=current; cfp; cfp=cfp->next){
        rp = cfp->rp;
        dot = cfp->dot;
        if( dot>=rp->nrhs ) continue;
        sp = rp->rhs[dot];
        if( sp->type==NONTERMINAL ){
            if( sp->rule==0 && sp!=lemp->errsym ){
                ErrorMsg(lemp->filename,rp->line,"Nonterminal \"%s\" has no rules.",
                         sp->name);
                lemp->errorcnt++;
            }
            for(newrp=sp->rule; newrp; newrp=newrp->nextlhs){
                newcfp = Configlist_add(newrp,0);
                for(i=dot+1; i<rp->nrhs; i++){
                    xsp = rp->rhs[i];
                    if( xsp->type==TERMINAL ){
                        SetAdd(newcfp->fws,xsp->index);
                        break;
                    }else if( xsp->type==MULTITERMINAL ){
                        int k;
                        for(k=0; k<xsp->nsubsym; k++){
                            SetAdd(newcfp->fws, xsp->subsym[k]->index);
                        }
                        break;
                    }else{
                        SetUnion(newcfp->fws,xsp->firstset);
                        if( xsp->lambda==LEMON_FALSE ) break;
                    }
                }
                if( i==rp->nrhs ) Plink_add(&cfp->fplp,newcfp);
            }
        }
    }
    return;
}

/* Sort the configuration list */
void Configlist_sort(){
    current = (struct config *)msort((char *)current,(char **)&(current->next),(msort_comparator)Configcmp);
    currentend = 0;
    return;
}

/* Sort the basis configuration list */
void Configlist_sortbasis(){
    basis = (struct config *)msort((char *)current,(char **)&(current->bp),(msort_comparator)Configcmp);
    basisend = 0;
    return;
}

/* Return a pointer to the head of the configuration list and
 ** reset the list */
struct config *Configlist_return(){
    struct config *old;
    old = current;
    current = 0;
    currentend = 0;
    return old;
}

/* Return a pointer to the head of the configuration list and
 ** reset the list */
struct config *Configlist_basis(){
    struct config *old;
    old = basis;
    basis = 0;
    basisend = 0;
    return old;
}

/* Free all elements of the given configuration list */
void Configlist_eat(cfp)
struct config *cfp;
{
    struct config *nextcfp;
    for(; cfp; cfp=nextcfp){
        nextcfp = cfp->next;
        assert( cfp->fplp==0 );
        assert( cfp->bplp==0 );
        if( cfp->fws ) SetFree(cfp->fws);
        deleteconfig(cfp);
    }
    return;
}
