/* D language template for the LEMON parser generator.
 * Copyright (GPL) 2008 Serge Vakulenko <serge@vak.ru>
 */
module parser;
private import std.c.string;

/* First off, code is include which follows the "include" declaration
 * in the input file. */
%%

/* Next is all token values, in a form suitable for use by makeheaders.
 * This section will be null unless lemon is run with the -m switch.
 *
 * These constants (all generated automatically by the parser generator)
 * specify the various kinds of tokens (terminals) that the parser
 * understands.
 *
 * Each symbol here is a terminal symbol in the grammar.
 */
%%

/* The next thing included is series of defines which control
 * various aspects of the generated parser.
 *    YYCODETYPE         is the data type used for storing terminal
 *                       and nonterminal numbers.  "ubyte" is
 *                       used if there are fewer than 250 terminals
 *                       and nonterminals.  "int" is used otherwise.
 *    YYNOCODE           is a number of type YYCODETYPE which corresponds
 *                       to no legal terminal or nonterminal number.  This
 *                       number is used to fill in empty slots of the hash
 *                       table.
 *    YYACTIONTYPE       is the data type used for storing terminal
 *                       and nonterminal numbers.  "ubyte" is
 *                       used if there are fewer than 250 rules and
 *                       states combined.  "int" is used otherwise.
 *    token_t            is the data type used for minor tokens given
 *                       directly to the parser from the tokenizer.
 *    YYMINORTYPE        is the data type used for all minor tokens.
 *                       This is typically a union of many types, one of
 *                       which is token_t.  The entry in the union
 *                       for base tokens is called "yy0".
 *    YYNSTATE           the combined number of states.
 *    YYNRULE            the number of rules in the grammar
 *    YYERRORSYMBOL      is the code number of the error symbol.
 *                       If < 0, then do no error processing.
 */
%%

private const int YY_NO_ACTION = YYNSTATE + YYNRULE + 2;
private const int YY_ACCEPT_ACTION = YYNSTATE + YYNRULE + 1;
private const int YY_ERROR_ACTION = YYNSTATE + YYNRULE;

/* Next are that tables used to determine what action to take based on the
 * current state and lookahead token.  These tables are used to implement
 * functions that take a state number and lookahead value and return an
 * action integer.
 *
 * Suppose the action integer is N.  Then the action is determined as
 * follows
 *
 *   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
 *                                      token onto the stack and goto state N.
 *
 *   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
 *
 *   N == YYNSTATE+YYNRULE              A syntax error has occurred.
 *
 *   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
 *
 *   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
 *                                      slots in the yy_action[] table.
 */

/* The action table is constructed as a single large table named yy_action[].
 * Given state S and lookahead X, the action is computed as
 *
 *      yy_action[ yy_shift_ofst[S] + X ]
 *
 * If the index value yy_shift_ofst[S]+X is out of range or if the value
 * yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
 * is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
 * and that yy_default[S] should be used instead.
 *
 * The formula above is for computing the action when the lookahead is
 * a terminal symbol.  If the lookahead is a non-terminal (as occurs after
 * a reduce action) then the yy_reduce_ofst[] array is used in place of
 * the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
 * YY_SHIFT_USE_DFLT.
 */

/* The following are the tables generated in this section:
 *
 *  yy_action[]        A single table containing all actions.
 *  yy_lookahead[]     A table containing the lookahead for each entry in
 *                     yy_action.  Used to detect hash collisions.
 *  yy_shift_ofst[]    For each state, the offset into yy_action for
 *                     shifting terminals.
 *  yy_reduce_ofst[]   For each state, the offset into yy_action for
 *                     shifting non-terminals after a reduce.
 *  yy_default[]       Default action for each state.
 */
%%

private const int YY_SZ_ACTTAB = yy_action.length;

/* The next table maps tokens into fallback tokens.  If a construct
 * like the following:
 *
 *      %fallback ID X Y Z.
 *
 * appears in the grammer, then ID becomes a fallback token for X, Y,
 * and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
 * but it does not parse, the type of the token is changed to ID and
 * the parse is retried before an error is thrown.
 */
private static const YYCODETYPE fallback[] = [
%%
];

/* The following structure represents a single element of the
 * parser's stack.  Information stored includes:
 *
 *   +  The state number for the parser at this level of the stack.
 *
 *   +  The value of the token stored at this level of the stack.
 *      (In other words, the "major" token.)
 *
 *   +  The semantic value stored at this level of the stack.  This is
 *      the information used by the action routines in the grammar.
 *      It is sometimes called the "minor" token.
 */
private struct stack_entry_t {
	int stateno;		/* The state-number */
	int major;		/* The major token value.  This is the code
				 * number for the token at this stack level */
	YYMINORTYPE minor;	/* The user-supplied minor token value.  This
				 * is the value of the token  */
}

/* The state of the parser is completely contained in an instance of
 * the following structure
 */
class parser_t {
	int idx;		/* Index of top element in stack */
	int errcnt;		/* Shifts left before out of the error */
	stack_entry_t[] stack;	/* The parser's stack */

	/*
	 * This function allocates a new parser.
	 */
	this ()
	{
		idx = -1;
		stack = new stack_entry_t [100];
	}

	/*
	 * Deallocate and destroy a parser.  Destructors are all called for
	 * all stack elements before shutting the parser down.
	 */
	~this () {
		while (idx >= 0)
			pop_parser_stack ();
		delete stack;
	}

	debug (lemon) {
		private import std.stdio;
		FILE *trace_file;
		char[] trace_prompt;

		/*
		 * Turn parser tracing on by giving a stream to which to write
		 * the trace and a prompt to preface each trace message.
		 * Tracing is turned off by making either argument NULL
		 *
		 * Inputs:
		 * <ul>
		 * <li> A FILE* to which trace output should be written.
		 *      If NULL, then tracing is turned off.
		 * <li> A prefix string written at the beginning of every
		 *      line of trace output.  If NULL, then tracing is
		 *      turned off.
		 * </ul>
		 *
		 * Outputs:
		 * None.
		 */
		void trace (FILE *fd, char[] prompt)
		{
			trace_file = fd;
			if (trace_file == null)
				trace_prompt = "";
			else
				trace_prompt = prompt;
		}

		/* For tracing shifts, the names of all terminals and nonterminals
		 * are required.  The following table supplies these names */
		private static const char[] token_name[] = [
%%
		];

		/* For tracing reduce actions, the names of all rules are required.
		 */
		private static const char[] rule_name[] = [
%%
		];
	}

	/*
	 * Pop the parser's stack once.
	 *
	 * If there is a destructor routine associated with the token which
	 * is popped from the stack, then call it.
	 *
	 * Return the major token number for the symbol popped.
	 */
	private int pop_parser_stack ()
	{
		stack_entry_t *top;

		if (idx < 0)
			return 0;
		top = &stack [idx];
		debug (lemon) { if (trace_file) {
			fprintf (trace_file, "%sPopping %s\n",
				cast(char*) trace_prompt,
				cast(char*) token_name[top.major]);
		}}
		destructor (top.major, &top.minor);
		idx--;
		return top.major;
	}

	/* The following function deletes the value associated with a
	 * symbol.  The symbol can be either a terminal or nonterminal.
	 * "major" is the symbol code, and "yypminor" is a pointer to
	 * the value.
	 */
	private void destructor (YYCODETYPE major, YYMINORTYPE *pminor)
	{
		switch (major) {
		/* Here is inserted the actions which take place when a
		 * terminal or non-terminal is destroyed.  This can happen
		 * when the symbol is popped from the stack during a
		 * reduce or during error processing or when a parser is
		 * being destroyed before it is finished parsing.
		 *
		 * Note: during a reduce, the only symbols destroyed are those
		 * which appear on the RHS of the rule, but which are not used
		 * inside the C code.
		 */
%%
		default:
			break;   /* If no destructor action specified: do nothing */
		}
	}

	/*
	 * Find the appropriate action for a parser given the terminal
	 * look-ahead token i_lookahead.
	 *
	 * If the look-ahead token is YYNOCODE, then check to see if the action is
	 * independent of the look-ahead.  If it is, return the action, otherwise
	 * return YY_NO_ACTION.
	 */
	private int find_shift_action (YYCODETYPE i_lookahead)
	{
		int i;
		int stateno = stack[idx].stateno;

		if (stateno > YY_SHIFT_MAX ||
		    (i = yy_shift_ofst [stateno]) == YY_SHIFT_USE_DFLT) {
			return yy_default [stateno];
		}
		assert (i_lookahead != YYNOCODE);
		i += i_lookahead;
		if (i >= 0 && i < YY_SZ_ACTTAB && yy_lookahead[i] == i_lookahead) {
			return yy_action [i];
		}
		if (i_lookahead > 0) {
			int i_fallback;            /* Fallback token */

			if (i_lookahead < fallback.length &&
			    (i_fallback = fallback[i_lookahead]) != 0) {
				debug (lemon) { if (trace_file) {
					fprintf (trace_file, "%sFALLBACK %s => %s\n",
						cast(char*) trace_prompt,
						cast(char*) token_name[i_lookahead],
						cast(char*) token_name[i_fallback]);
				}}
				return find_shift_action (i_fallback);
			}
			if (YYWILDCARD >= 0) {
				int j = i - i_lookahead + YYWILDCARD;

				if (j >= 0 && j < YY_SZ_ACTTAB &&
				    yy_lookahead[j] == YYWILDCARD) {
					debug (lemon) { if (trace_file) {
						fprintf (trace_file, "%sWILDCARD %s => %s\n",
							cast(char*) trace_prompt,
							cast(char*) token_name[i_lookahead],
							cast(char*) token_name[YYWILDCARD]);
					}}
					return yy_action[j];
				}
			}
		}
		return yy_default[stateno];
	}

	/*
	 * Find the appropriate action for a parser given the non-terminal
	 * look-ahead token i_lookahead.
	 *
	 * If the look-ahead token is YYNOCODE, then check to see if the action is
	 * independent of the look-ahead.  If it is, return the action, otherwise
	 * return YY_NO_ACTION.
	 */
	private int find_reduce_action (int stateno, YYCODETYPE i_lookahead)
	{
		int i;

		assert (stateno <= YY_REDUCE_MAX);
		i = yy_reduce_ofst [stateno];
		assert (i != YY_REDUCE_USE_DFLT);
		assert (i_lookahead != YYNOCODE);
		i += i_lookahead;
		assert (i >= 0 && i < YY_SZ_ACTTAB);
		assert (yy_lookahead[i] == i_lookahead);
		return yy_action [i];
	}

	/*
	 * The following routine is called if the stack overflows.
	 */
	private void stack_overflow (YYMINORTYPE *pminor)
	{
		idx--;
		debug (lemon) { if (trace_file) {
			fprintf (trace_file, "%sStack Overflow!\n",
				cast(char*) trace_prompt);
		}}
		while (idx >= 0)
			pop_parser_stack ();
		/* Here code is inserted which will execute if the parser
		 * stack every overflows */
%%
	}

	/*
	 * Perform a shift action.
	 */
	private void shift (int new_state, int major, YYMINORTYPE *pminor)
	{
		stack_entry_t *top;

		idx++;
		if (idx >= stack.length) {
			/* Increase the size of the parser stack. */
			stack.length = stack.length * 2 + 100;
			debug (lemon) { if (trace_file) {
				fprintf (trace_file, "%sStack grows to %d entries!\n",
					cast(char*) trace_prompt, stack.length);
			}}
			if (idx >= stack.length) {
				stack_overflow (pminor);
				return;
			}
		}
		top = &stack[idx];
		top.stateno = new_state;
		top.major = major;
		top.minor = *pminor;
		debug (lemon) { if (trace_file && idx > 0) {
			int i;
			fprintf (trace_file, "%sShift %d\n",
				cast(char*) trace_prompt, new_state);
			fprintf (trace_file, "%sStack:",
				cast(char*) trace_prompt);
			for (i=1; i<=idx; i++)
				fprintf (trace_file, " %s",
					cast(char*) token_name [stack[i].major]);
			fprintf (trace_file, "\n");
		}}
	}

	/* The following table contains information about every rule that
	 * is used during the reduce.
	 */
	private struct rule_info_t {
		YYCODETYPE lhs;	/* Symbol on the left-hand side of the rule */
		ubyte nrhs;	/* Number of right-hand side symbols in the rule */
	}
	private static const rule_info_t rule_info[] = [
%%
	];

	/*
	 * Perform a reduce action and the shift that must immediately
	 * follow the reduce.
	 */
	private void reduce (int ruleno)
	{
		int yygoto;			/* The next state */
		int yyact;			/* The next action */
		YYMINORTYPE yygotominor;	/* The LHS of the rule reduced */
		stack_entry_t *yymsp;		/* The top of the parser's stack */
		int yysize;			/* Amount to pop the stack */

		yymsp = &stack[idx];
		debug (lemon) {
			if (trace_file && ruleno >= 0 &&
			    ruleno < rule_name.length) {
				fprintf (trace_file, "%sReduce [%s].\n",
					cast(char*) trace_prompt,
					cast(char*) rule_name[ruleno]);
		}}

		/* Silence complaints from purify about yygotominor being uninitialized
		 * in some cases when it is copied into the stack after the following
		 * switch.  yygotominor is uninitialized when a rule reduces that does
		 * not set the value of its left-hand side nonterminal.  Leaving the
		 * value of the nonterminal uninitialized is utterly harmless as long
		 * as the value is never used.  So really the only thing this code
		 * accomplishes is to quieten purify.
		 *
		 * 2007-01-16:  The wireshark project (www.wireshark.org) reports that
		 * without this code, their parser segfaults.  I'm not sure what there
		 * parser is doing to make this happen.  This is the second bug report
		 * from wireshark this week.  Clearly they are stressing Lemon in ways
		 * that it has not been previously stressed...  (SQLite ticket #2172)
		 */
		std.c.string.memset (&yygotominor, 0, yygotominor.sizeof);

		switch (ruleno) {
		/* Beginning here are the reduction cases.  A typical example
		 * follows:
		 *   case 0:
		 *  #line <lineno> <grammarfile>
		 *     { ... }           // User supplied code
		 *  #line <lineno> <thisfile>
		 *     break;
		 */
%%
		};
		yygoto = rule_info[ruleno].lhs;
		yysize = rule_info[ruleno].nrhs;
		idx -= yysize;
		yyact = find_reduce_action (yymsp[-yysize].stateno, yygoto);
		if (yyact < YYNSTATE) {
			debug (lemon) {
				shift (yyact, yygoto, &yygotominor);
			} else {
				/* If we are not debugging and the reduce action popped
				 * at least one element off the stack, then we can push
				 * the new element back onto the stack here, and skip
				 * the stack overflow test in shift().
				 * That gives a significant speed improvement. */
				if (yysize) {
					idx++;
					yymsp -= yysize-1;
					yymsp.stateno = yyact;
					yymsp.major = yygoto;
					yymsp.minor = yygotominor;
				} else {
					shift (yyact, yygoto, &yygotominor);
				}
			}
		} else {
			assert (yyact == YYNSTATE + YYNRULE + 1);
			accept ();
		}
	}

	/*
	 * The following code executes when the parse fails
	 */
	private void parse_failed ()
	{
		debug (lemon) { if (trace_file) {
			fprintf (trace_file, "%sFail!\n",
				cast(char*) trace_prompt);
		}}
		while (idx >= 0)
			pop_parser_stack ();
		/* Here code is inserted which will be executed whenever the
		 * parser fails */
%%
	}

	/*
	 * The following code executes when a syntax error first occurs.
	 */
	private void syntax_error (int major, YYMINORTYPE minor)
	{
		token_t TOKEN = minor.yy0;
%%
	}

	/*
	 * The following is executed when the parser accepts
	 */
	private void accept ()
	{
		debug (lemon) { if (trace_file) {
			fprintf (trace_file, "%sAccept!\n",
				cast(char*) trace_prompt);
		}}
		while (idx >= 0)
			pop_parser_stack ();
		/* Here code is inserted which will be executed whenever the
		 * parser accepts */
%%
	}

	/* The main parser program.
	 * The first argument is the major token number.  The second is
	 * the minor token.
	 */
	void parse (int major, token_t minor)
	{
		YYMINORTYPE minorunion;
		int act;		/* The parser action. */
		int endofinput;		/* True if we are at the end of input */
		int errorhit = 0;	/* True if major has invoked an error */

		/* (re)initialize the parser, if necessary */
		if (idx < 0) {
			if (stack.length <= 0) {
				std.c.string.memset (&minorunion, 0, minorunion.sizeof);
				stack_overflow (&minorunion);
				return;
			}
			idx = 0;
			errcnt = -1;
			stack[0].stateno = 0;
			stack[0].major = 0;
		}
		minorunion.yy0 = minor;
		endofinput = (major==0);

		debug (lemon) { if (trace_file) {
			fprintf (trace_file, "%sInput %s\n",
				cast(char*)trace_prompt,
				cast(char*)token_name[major]);
		}}
		do {
			act = find_shift_action (major);
			if (act < YYNSTATE) {
				/* Impossible to shift the $ token */
				assert (! endofinput);
				shift (act, major, &minorunion);
				errcnt--;
				major = YYNOCODE;

			} else if (act < YYNSTATE + YYNRULE) {
				reduce (act - YYNSTATE);
			} else {
				/* A syntax error has occurred. The response
				 * to an error depends upon whether or not
				 * the grammar defines an error token "ERROR".
				 */
				assert (act == YY_ERROR_ACTION);
				debug (lemon) { if (trace_file) {
					fprintf (trace_file, "%sSyntax Error!\n",
						cast(char*) trace_prompt);
				}}
				if (YYERRORSYMBOL >= 0) {
					/* This is what we do if the grammar
					 * does define ERROR:
					 * 1) Call the %syntax_error function.
					 * 2) Begin popping the stack until we
					 *    enter a state where it is legal
					 *    to shift the error symbol, then
					 *    shift the error symbol.
					 * 3) Set the error count to three.
					 * 4) Begin accepting and shifting new
					 *    tokens.  No new error processing
					 *    will occur until three tokens
					 *    have been shifted successfully.
					 */
					if (errcnt < 0) {
						syntax_error (major, minorunion);
					}
					int mx = stack[idx].major;
					if (mx == YYERRORSYMBOL || errorhit) {
						debug (lemon) { if (trace_file) {
							fprintf (trace_file,
								"%sDiscard input token %s\n",
								cast(char*) trace_prompt,
								cast(char*) token_name[major]);
						}}
						destructor (major, &minorunion);
						major = YYNOCODE;
					} else {
						 while (idx >= 0 &&
						     mx != YYERRORSYMBOL) {
							act = find_reduce_action (
								stack[idx].stateno,
								cast(YYCODETYPE) YYERRORSYMBOL);
							if (act < YYNSTATE)
								break;
							pop_parser_stack ();
						}
						if (idx < 0 || major == 0) {
							destructor (major, &minorunion);
							parse_failed ();
							major = YYNOCODE;
						} else if (mx != YYERRORSYMBOL) {
							YYMINORTYPE u2 = {0};
							shift (act, YYERRORSYMBOL, &u2);
						}
					}
					errcnt = 3;
					errorhit = 1;
				} else {
					/* This is what we do if the grammar
					 * does not define ERROR:
					 * 1) Report an error message, and
					 *    throw away the input token.
					 * 2) If the input token is $, then
					 *    fail the parse.
					 *
					 * As before, subsequent error messages
					 * are suppressed until three input
					 * tokens have been successfully
					 * shifted.
					 */
					if (errcnt <= 0) {
						syntax_error (major, minorunion);
					}
					errcnt = 3;
					destructor (major, &minorunion);
					if (endofinput) {
						parse_failed ();
					}
					major = YYNOCODE;
				}
			}
		} while (major != YYNOCODE && idx >= 0);
	}

} /* End of parser_t. */
