/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#include "lemon.h"

#include "build.h"
#include "error.h"
#include "option.h"
#include "parse.h"
#include "report.h"
#include "set.h"
#include "struct.h"
#include "table.h"

/*
 ** Main program file for the LEMON parser generator.
 */

/* Report an out-of-memory condition and abort.  This function
 ** is used mostly by the "MemoryCheck" macro in struct.h
 */
void memory_error(){
    ErrorMsg("lemon", LINENO_NONE, "Out of memory.  Aborting...\n");
    exit(1);
}

int nDefine = 0;       /* Number of -D options on the command line */
char **azDefine = 0;   /* Name of the -D macros */
int language = LANG_C; /* Output language */

/* This routine is called with the argument to each -D command-line option.
 ** Add the macro defined to the azDefine array.
 */
static void handle_D_option(char *z){
    char **paz;
    nDefine++;
    azDefine = realloc(azDefine, sizeof(azDefine[0])*nDefine);
    if( azDefine==0 ){
        memory_error();
    }
    paz = &azDefine[nDefine-1];
    *paz = malloc( strlen(z)+1 );
    if( *paz==0 ){
        memory_error();
    }
    strcpy(*paz, z);
    for(z=*paz; *z && *z!='='; z++){}
    *z = 0;
}

/* This routine is called with the argument to each -l command-line option.
 ** Set an output language (c, c++, d).
 */
static void handle_l_option(char *z){
    if (strcmp (z, "c") == 0) {
        language = LANG_C;
        
    } else if (strcmp (z, "c++") == 0) {
        language = LANG_CPP;
        
    } else if (strcmp (z, "d") == 0) {
        language = LANG_D;
    }
}


/* The main program.  Parse the command line and do it... */
int main(int argc, char **argv)
{
    static int version = 0;
    static int rpflag = 0;
    static int basisflag = 0;
    static int compress = 0;
    static int quiet = 0;
    static int statistics = 0;
    static int mhflag = 0;
    static struct s_options options[] = {
        {OPT_FLAG, "b", (char*)&basisflag, "Print only the basis in report."},
        {OPT_FLAG, "c", (char*)&compress, "Don't compress the action table."},
        {OPT_FSTR, "D", (char*)handle_D_option, "Define an %ifdef macro."},
        {OPT_FLAG, "g", (char*)&rpflag, "Print grammar without actions."},
        {OPT_FSTR, "l", (char*)handle_l_option, "Set an output language (c, c++, d)."},
        {OPT_FLAG, "m", (char*)&mhflag, "Output a makeheaders compatible file"},
        {OPT_FLAG, "q", (char*)&quiet, "(Quiet) Don't print the report file."},
        {OPT_FLAG, "s", (char*)&statistics,
            "Print parser stats to standard output."},
        {OPT_FLAG, "x", (char*)&version, "Print the version number."},
        {OPT_FLAG,0,0,0}
    };
    int i;
    struct lemon lem;
    
    OptInit(argv,options,stderr);
    if( version ){
        printf("Lemon version 1.0\n");
        exit(0);
    }
    if( OptNArgs()!=1 ){
        ErrorMsg("lemon", LINENO_NONE, "Exactly one filename argument is required.\n");
        exit(1);
    }
    memset(&lem, 0, sizeof(lem));
    lem.errorcnt = 0;
    
    /* Initialize the machine */
    Strsafe_init();
    Symbol_init();
    State_init();
    lem.argv0 = argv[0];
    lem.filename = OptArg(0);
    lem.basisflag = basisflag;
    Symbol_new("$");
    lem.errsym = Symbol_new("error");
    lem.errsym->useCnt = 0;
    
    /* Parse the input file */
    Parse(&lem);
    if( lem.errorcnt ) exit(lem.errorcnt);
        if( lem.nrule==0 ){
            ErrorMsg(lem.filename, LINENO_NONE, "Empty grammar.\n");
            exit(1);
        }
    
    /* Count and index the symbols of the grammar */
    lem.nsymbol = Symbol_count();
    Symbol_new("{default}");
    lem.symbols = Symbol_arrayof();
    for(i=0; i<=lem.nsymbol; i++) lem.symbols[i]->index = i;
        qsort(lem.symbols,lem.nsymbol+1,sizeof(struct symbol*),
              (int(*)())Symbolcmpp);
        for(i=0; i<=lem.nsymbol; i++) lem.symbols[i]->index = i;
            for(i=1; isupper(lem.symbols[i]->name[0]); i++);
    lem.nterminal = i;
    
    /* Generate a reprint of the grammar, if requested on the command line */
    if( rpflag ){
        Reprint(&lem);
    }else{
        /* Initialize the size for all follow and first sets */
        SetSize(lem.nterminal+1);
        
        /* Find the precedence for every production rule (that has one) */
        FindRulePrecedences(&lem);
        
        /* Compute the lambda-nonterminals and the first-sets for every
         ** nonterminal */
        FindFirstSets(&lem);
        
        /* Compute all LR(0) states.  Also record follow-set propagation
         ** links so that the follow-set can be computed later */
        lem.nstate = 0;
        FindStates(&lem);
        lem.sorted = State_arrayof();
        
        /* Tie up loose ends on the propagation links */
        FindLinks(&lem);
        
        /* Compute the follow set of every reducible configuration */
        FindFollowSets(&lem);
        
        /* Compute the action tables */
        FindActions(&lem);
        
        /* Compress the action tables */
        if( compress==0 ) CompressTables(&lem);
        
        /* Reorder and renumber the states so that states with fewer choices
         ** occur at the end. */
        ResortStates(&lem);
        
        /* Generate a report of the parser generated.  (the "y.output" file) */
        if( !quiet ) ReportOutput(&lem);
        
        /* Generate the source code for the parser */
        ReportTable(&lem, mhflag);
        
        /* Produce a header file for use by the scanner.  (This step is
         ** omitted if the "-m" option is used because makeheaders will
         ** generate the file for us.) */
        if (! mhflag && language != LANG_D)
            ReportHeader(&lem);
    }
    if( statistics ){
        LogMsg(LOGLEVEL_INFO, lem.filename, LINENO_NONE,
               "Parser statistics: %d terminals, %d nonterminals, %d rules\n",
               lem.nterminal, lem.nsymbol - lem.nterminal, lem.nrule);
        LogMsg(LOGLEVEL_INFO, lem.filename, LINENO_NONE,
               "                   %d states, %d parser table entries, %d conflicts\n",
               lem.nstate, lem.tablesize, lem.nconflict);
    }
    if( lem.nconflict ){
        LogMsg(LOGLEVEL_WARNING, lem.filename, LINENO_NONE, "%d parsing conflicts.\n", lem.nconflict);
    }
    exit(lem.errorcnt + lem.nconflict);
    return (lem.errorcnt + lem.nconflict);
}
