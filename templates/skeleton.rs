
#![allow(dead_code)]

use std::io;
use std::rc::Rc;

/* Next is all token values, in a form suitable for use by makeheaders.
 * This section will be null unless lemon is run with the -m switch.
 *
 * These constants (all generated automatically by the parser generator)
 * specify the various kinds of tokens (terminals) that the parser
 * understands.
 *
 * Each symbol here is a terminal symbol in the grammar.
 */
pub const PLUS: usize = 1;
pub const MINUS: usize = 2;
pub const DIVIDE: usize = 3;
pub const TIMES: usize = 4;
pub const INTEGER: usize = 5;

/* The next thing included is series of defines which control
 * various aspects of the generated parser.
 *    YYCODETYPE         is the data type used for storing terminal
 *                       and nonterminal numbers. 
 *    YYNOCODE           is a number of type YYCODETYPE which corresponds
 *                       to no legal terminal or nonterminal number.  This
 *                       number is used to fill in empty slots of the hash
 *                       table.
 *    YYACTIONTYPE       is the data type used for storing terminal
 *                       and nonterminal numbers.
 *    Token            is the data type used for minor tokens given
 *                       directly to the parser from the tokenizer.
 *    YYMINORTYPE        is the data type used for all minor tokens.
 *                       This is typically a union of many types, one of
 *                       which is Token.  The entry in the union
 *                       for base tokens is called "yy0".
 *    YYNSTATE           the combined number of states.
 *    YYNRULE            the number of rules in the grammar
 *    YYERRORSYMBOL      is the code number of the error symbol.
 *                       If None, then do no error processing.
 */
pub type YYCODETYPE = usize;
pub const YYNOCODE: YYCODETYPE = 10;
pub type YYACTIONTYPE = usize;
pub const YYWILDCARD: Option<usize> = None;
pub type Token = i32;
pub struct YYMINORTYPE {
  pub yy0: Token
}
const YYNSTATE: usize = 11;
const YYNRULE: usize = 6;
const YYERRORSYMBOL: Option<usize> = None;

const YY_NO_ACTION: usize = YYNSTATE + YYNRULE + 2;
const YY_ACCEPT_ACTION: usize = YYNSTATE + YYNRULE + 1;
const YY_ERROR_ACTION: usize = YYNSTATE + YYNRULE;

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
 *                                      slots in the YY_ACTION[] table.
 */

/* The action table is constructed as a single large table named YY_ACTION[].
 * Given state S and lookahead X, the action is computed as
 *
 *      YY_ACTION[ YY_SHIFT_OFFSET[S] + X ]
 *
 * If the index value YY_SHIFT_OFFSET[S]+X is out of range or if the value
 * YY_LOOKAHEAD[YY_SHIFT_OFFSET[S]+X] is not equal to X or if YY_SHIFT_OFFSET[S]
 * is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
 * and that YY_DEFAULT[S] should be used instead.
 *
 * The formula above is for computing the action when the lookahead is
 * a terminal symbol.  If the lookahead is a non-terminal (as occurs after
 * a reduce action) then the YY_REDUCE_OFFSET[] array is used in place of
 * the YY_SHIFT_OFFSET[] array and YY_REDUCE_USE_DFLT is used in place of
 * YY_SHIFT_USE_DFLT.
 */

/* The following are the tables generated in this section:
 *
 *  YY_ACTION[]        A single table containing all actions.
 *  YY_LOOKAHEAD[]     A table containing the lookahead for each entry in
 *                     YY_ACTION.  Used to detect hash collisions.
 *  YY_SHIFT_OFFSET[]    For each state, the offset into YY_ACTION for
 *                     shifting terminals.
 *  YY_REDUCE_OFFSET[]   For each state, the offset into YY_ACTION for
 *                     shifting non-terminals after a reduce.
 *  YY_DEFAULT[]       Default action for each state.
 */
const YY_SZ_ACTTAB: usize = 15;
const YY_ACTION: [YYACTIONTYPE; YY_SZ_ACTTAB] = [
 /*     0 */    11,    4,    2,    3,    1,    3,    1,    6,   18,    5,
 /*    10 */    10,    9,    8,   19,    7,
];
const YY_LOOKAHEAD: [YYCODETYPE; 15] = [
 /*     0 */     0,    1,    2,    3,    4,    3,    4,    8,    7,    8,
 /*    10 */     5,    8,    8,    9,    8,
];
const YY_SHIFT_USE_DFLT: i32 = -1;
const YY_SHIFT_MAX: i32 = 7;
const YY_SHIFT_OFFSET: [i32; 8] = [
 /*     0 */     5,    5,    5,    5,    5,    0,    2,    2,
];
const YY_REDUCE_USE_DFLT: i32 = -2;
const YY_REDUCE_MAX: i32 = 4;
const YY_REDUCE_OFFSET: [i32; 5] = [
 /*     0 */     1,    4,    6,    3,   -1,
];
const YY_DEFAULT: [YYACTIONTYPE; 11]  = [
 /*     0 */    17,   17,   17,   17,   17,   17,   13,   12,   14,   15,
 /*    10 */    16,
];

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
const FALLBACK: [YYCODETYPE; 0] = [
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
struct StackEntry {
    
    /* The state-number */
	stateno: i32,		
	
	/* The major token value.  This is the code number for the token at this stack level */
	major: usize,
	
	/* The user-supplied minor token value.  This is the value of the token  */
	minor: Rc<YYMINORTYPE>
}

/* The following table contains information about every rule that
 * is used during the reduce.
 */
struct RuleInfo {
    
    /* Symbol on the left-hand side of the rule */
	lhs: YYCODETYPE,
	
	/* Number of right-hand side symbols in the rule */
	nrhs: u8
}

const RULE_INFO: [RuleInfo; 6] = [
    RuleInfo { lhs: 7, nrhs: 1 },
    RuleInfo { lhs: 8, nrhs: 3 },
    RuleInfo { lhs: 8, nrhs: 3 },
    RuleInfo { lhs: 8, nrhs: 3 },
    RuleInfo { lhs: 8, nrhs: 3 },
    RuleInfo { lhs: 8, nrhs: 1 },
];

/* For tracing shifts, the names of all terminals and nonterminals
 * are required.  The following table supplies these names */
const TOKEN_NAME: [&'static str; 9] = [
 "$",             "PLUS",          "MINUS",         "DIVIDE",      
 "TIMES",         "INTEGER",       "error",         "program",     
 "expr",        
];

/* For tracing reduce actions, the names of all rules are required.
 */
const RULE_NAME: [&'static str; 6] = [
/*   0 */ "program ::= expr",
/*   1 */ "expr ::= expr MINUS expr",
/*   2 */ "expr ::= expr PLUS expr",
/*   3 */ "expr ::= expr TIMES expr",
/*   4 */ "expr ::= expr DIVIDE expr",
/*   5 */ "expr ::= INTEGER",
];

/* The state of the parser is completely contained in an instance of
 * the following structure
 */
pub struct Parser {
    
	/* Shifts left before out of the error. */
	error_count: i32,
	
	/* The parser's stack. */
    stack: Vec<StackEntry>,
    
    /* Debug output file. */
    trace_file: Option<Box<io::Write>>,
    
    /* Debug trace prompt prepended to each trace log message. */
    trace_prompt: Option<String>
}

impl Parser {
    
    /**
	 * Allocate a new parser object.
	 */
    pub fn new() -> Parser {
        return Parser {
            error_count: 0,
            stack: Vec::new(),
            trace_file: None,
            trace_prompt: None
        }
    }
        
    /*
	 * Turn parser tracing on by giving a stream to which to write
	 * the trace and a prompt to preface each trace message.
	 * Tracing is turned off by making either argument NULL
	 *
	 * Inputs:
	 * <ul>
	 * <li> An io::Write to which trace output should be written.
	 * <li> A prefix string written at the beginning of every line of trace output, or None.
	 * </ul>
	 *
	 * Outputs:
	 * None.
	 */
    pub fn trace(&mut self, trace_file: Box<io::Write>, prompt: Option<String>) {
        self.trace_file = Some(trace_file);
        self.trace_prompt = prompt;
	}
	
	/*
	 * Stops displaying trace messages to the specified writer and clears the prompt.
	 */
	pub fn end_trace(&mut self) {
	    self.trace_file = None;
	    self.trace_prompt = None;
	}
    
    /*
	 * Pop the parser's stack once.
	 *
	 * If there is a destructor routine associated with the token which
	 * is popped from the stack, then call it.
	 *
	 * Return the major token number for the symbol popped.
	 */
	fn pop_parser_stack(&mut self) -> usize {
	    if let Some(top) = self.stack.pop() {
		
		    /* Log the removal of the stack entry from the parse stack. */
		    self.trace_log(format!("Popping {}", TOKEN_NAME[top.major]));
    		
    		/* Execute the non-terminal destructor for the token and return the major. */
    		self.destructor(top.major, top.minor);
    		return top.major;
		} else {
		    return 0;
		}
	}
	
	/* The following function deletes the value associated with a
	 * symbol.  The symbol can be either a terminal or nonterminal.
	 * "major" is the symbol code, and "yypminor" is a pointer to
	 * the value.
	 */
	#[allow(unused_variables)]
    fn destructor(&mut self, major: usize, minor: Rc<YYMINORTYPE>) {
		match major {
    		/* Here is inserted the actions which take place when a
    		 * terminal or non-terminal is destroyed.  This can happen
    		 * when the symbol is popped from the stack during a
    		 * reduce or during error processing or when a parser is
    		 * being destroyed before it is finished parsing.
    		 *
    		 * Note: during a reduce, the only symbols destroyed are those
    		 * which appear on the RHS of the rule, but which are not used
    		 * inside the Rust code.
    		 */
		    _ => {
			    /* If no destructor action specified: do nothing */
		    }
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
	fn find_shift_action(&mut self, i_lookahead: YYCODETYPE) -> YYACTIONTYPE {
        assert!(self.stack.len() > 0);
        
        /*
         * If YY_SHIFT_OFFSET[S] is equal to YY_SHIFT_USE_DFLT (or the index value is out of range)
         * it means that the action is not in the table and YY_DEFAULT[S] should be used instead.
         */
        let top_entry_state = self.stack_entry_from_tail(0).stateno;
        if (top_entry_state > YY_SHIFT_MAX) || (YY_SHIFT_OFFSET[top_entry_state as usize] == YY_SHIFT_USE_DFLT) {
            return YY_DEFAULT[top_entry_state as usize];
        }
        
        /*
         * Given state S and lookahead X, the action is computed as YY_ACTION[ YY_SHIFT_OFFSET[S] + X ].
         */
        assert!(i_lookahead != YYNOCODE);
        let i = YY_SHIFT_OFFSET[top_entry_state as usize] + (i_lookahead as i32);
        if (i >= 0) && (i < YY_SZ_ACTTAB as i32) && (YY_LOOKAHEAD[i as usize] == i_lookahead) {
            return YY_ACTION[i as usize];
        }
        
        if i_lookahead > 0 {
            /*
             * If the lookahead token is in the fallback table and it's valid, attempt to find
             * a shift action that corresponds.
             */
            if (i_lookahead < FALLBACK.len()) && (FALLBACK[i_lookahead as usize] != 0) {
                let i_fallback = FALLBACK[i_lookahead];
                self.trace_log(format!("FALLBACK {} => {}", TOKEN_NAME[i_lookahead], TOKEN_NAME[i_fallback]));
                return self.find_shift_action(i_fallback);
            }
            
            /*
             * If a wildcard action is specified, look it up in the action table.
             */
            match YYWILDCARD {
               Some(wildcard) => {
                   let j = i - (i_lookahead as i32) + (wildcard as i32);
                   if (j >= 0) && (j < YY_SZ_ACTTAB as i32) && (YY_LOOKAHEAD[j as usize] == wildcard) {
                       self.trace_log(format!("WILDCARD {} => {}", TOKEN_NAME[i_lookahead], TOKEN_NAME[wildcard]));
                       return YY_ACTION[j as usize];
                   }
               } 
               None => {
                   /* No wildcard specified. */
               }
            }
        }
        
        return YY_DEFAULT[top_entry_state as usize];
	}
	
	/*
	 * Find the appropriate action for a parser given the non-terminal
	 * look-ahead token i_lookahead.
	 *
	 * If the look-ahead token is YYNOCODE, then check to see if the action is
	 * independent of the look-ahead.  If it is, return the action, otherwise
	 * return YY_NO_ACTION.
	 */
    fn find_reduce_action (&self, stateno: i32, i_lookahead: YYCODETYPE) -> YYACTIONTYPE {
        assert!(stateno <= YY_REDUCE_MAX);
        
		let mut i = YY_REDUCE_OFFSET[stateno as usize];
		assert!(i != YY_REDUCE_USE_DFLT);
		assert!(i_lookahead != YYNOCODE);
		i += i_lookahead as i32;
		assert!(i >= 0 && i < (YY_SZ_ACTTAB as i32));
		assert!(YY_LOOKAHEAD[i as usize] == i_lookahead);
		return YY_ACTION[i as usize];
	}
	
	/*
	 * The following routine is called if the stack overflows.
	 */
	#[allow(unused_variables)]
	fn stack_overflow (&mut self, pminor: &YYMINORTYPE) {
        self.trace_log("Stack overflow.".to_string());
		while self.stack.len() > 0 {
			self.pop_parser_stack ();
		}
		
		/* Here code is inserted which will execute if the parser stack ever overflows */
	}
	
	/*
	 * Perform a shift action.
	 */
    fn shift(&mut self, new_state: i32, major: usize, pminor: Rc<YYMINORTYPE>) {
		self.stack.push(
		    StackEntry {
    	        stateno: new_state, 
    	        major: major, 
    	        minor: pminor
    	    }
		);
		
		self.trace_log(format!("Shift {} -- Stack:", new_state));
		
		let mut stack_trace = "".to_string();
		for i in self.stack.iter() {
		    let token_name_string = TOKEN_NAME[i.major].to_string();
		    stack_trace = format!("{} {}", stack_trace, token_name_string);
	    }
	    
	    self.trace_log(stack_trace);
	}
	
	/*
	 * Convenience method for accessing the stack from the end.
	 */
	fn stack_entry_from_tail(&self, offset_from_end: usize) -> &StackEntry {
	    return &self.stack[self.stack.len() - offset_from_end - 1];
	}
	
	/*
     * Perform a reduce action and the shift that must immediately
     * follow the reduce.
     */
     #[allow(unused_variables)]
     fn reduce(&mut self, ruleno: usize) {
         
         /* Log the reduce action to the trace file. */
         if ruleno < RULE_NAME.len() {
             self.trace_log(format!("Reduce [{}]", RULE_NAME[ruleno]));
         }
         
         let mut yygotominor = YYMINORTYPE {
             yy0: 0
         };
          
         match ruleno {
             /* Beginning here are the reduction cases.  A typical example
         	 * follows:
         	 *   case 0:
         	 *  #line <lineno> <grammarfile>
         	 *     { ... }           // User supplied code
         	 *  #line <lineno> <thisfile>
         	 *     break;
         	 */
         	 
         	 0 => {
         	     /* program ::= expr */
         	     { println!("Result={}", self.stack_entry_from_tail(0).minor.yy0); }
         	 }
         	 1 => {
         	     /* expr ::= expr MINUS expr */
         	     { yygotominor.yy0 = self.stack_entry_from_tail(2).minor.yy0 - self.stack_entry_from_tail(0).minor.yy0; }
         	 }
         	 2 => {
         	     /* expr ::= expr PLUS expr */
         	     { yygotominor.yy0 = self.stack_entry_from_tail(2).minor.yy0 + self.stack_entry_from_tail(0).minor.yy0; }
         	 }
         	 3 => {
         	     /* expr ::= expr TIMES expr */
         	     { yygotominor.yy0 = self.stack_entry_from_tail(2).minor.yy0 * self.stack_entry_from_tail(0).minor.yy0; }
         	 }
         	 4 => {
         	    /* expr ::= expr DIVIDE expr */
                if self.stack_entry_from_tail(0).minor.yy0 != 0 {
                    yygotominor.yy0 = self.stack_entry_from_tail(2).minor.yy0 / self.stack_entry_from_tail(0).minor.yy0;
                } else {
                    println!("divide by zero\n");
                }
         	 }
         	 5 => {
         	     /* expr ::= INTEGER */
         	     { yygotominor.yy0 = self.stack_entry_from_tail(0).minor.yy0; }
         	 }
             _ => {
                 /* Default: No Action. */
             }
         }
         
         let yygoto = RULE_INFO[ruleno].lhs;
         let yysize = RULE_INFO[ruleno].nrhs as usize;
         let yyact = self.find_reduce_action(self.stack_entry_from_tail(yysize).stateno, yygoto);
         
         /* Pop `yysize` entries off the stack without invoking the destructor. */
         let new_length = self.stack.len() - yysize;
         self.stack.truncate(new_length);
         
         /* 
          * If we are not debugging and the reduce action popped
		  * at least one element off the stack, then we can shift
		  * the new element back onto the stack here 
		  */
         if yyact < YYNSTATE {
             self.shift(yyact as i32, yygoto, Rc::new(yygotominor));
         } else {
             assert!(yyact == YY_ACCEPT_ACTION);
             self.accept();
         }
     }
	
	/*
 	 * The following code executes when the parse fails.
 	 */
    fn parse_failed(&mut self) {
        self.trace_log(format!("Parse failed."));
 		while self.stack.len() > 0 {
 			self.pop_parser_stack();
		}
 		/* Here code is inserted which will be executed whenever the parser fails */
 	}
 	
 	/*
	 * The following code executes when a syntax error first occurs.
	 */
	#[allow(unused_variables)]
    fn syntax_error (&mut self, major: usize, minor: Rc<YYMINORTYPE>) {
        let token = minor.yy0;
        println!("Syntax error!");
	}
	
	/*
	 * The following is executed when the parser accepts
	 */
	fn accept(&mut self) {
	    self.trace_log(format!("Accept!"));
	    while self.stack.len() > 0 {
 			self.pop_parser_stack();
		}
		/* Here code is inserted which will be executed whenever the parser accepts */
	}
	
	/* The main parser program.
     * The first argument is the major token number.  The second is the minor token.
     */
    pub fn parse(&mut self, new_major: usize, minor: Token) {
        let mut major = new_major;
        let mut error_hit = false;
        let end_of_input = major == 0;
        let minor_union = Rc::new(YYMINORTYPE {
            yy0: minor
        });
        
        /* Shift the end-of-file token ($) if the stack is empty. */
        if self.stack.len() == 0 {
            self.error_count = -1;
            self.shift(0, 0, Rc::new(YYMINORTYPE {
                yy0: 0
            }));
        }
        
        self.trace_log(format!("Input {}", TOKEN_NAME[major]));
        
        while (major != YYNOCODE) && (self.stack.len() > 0) {
            let mut action = self.find_shift_action(major);
            
            if action < YYNSTATE {
                /* It is not possible to shift the $ token */
    			assert!(end_of_input == false);
				self.shift(action as i32, major, minor_union.clone());
				self.error_count -= 1;
				major = YYNOCODE;
            } else if action < (YY_ERROR_ACTION) {
                self.reduce(action - YYNSTATE);
            } else {
                /*
                 * A syntax error has occurred. The response to an error depends upon whether
                 * or not the grammar defines an error token "ERROR".
                 */
                assert!(action == YY_ERROR_ACTION);
                self.trace_log(format!("Syntax error!"));
                
                match YYERRORSYMBOL {
                    Some(error_symbol) => {
                        /* This is what we do if the grammar
    					 * does define ERROR:
    					 * 1) Call the %syntax_error function.
    					 * 2) Begin popping the stack until we enter a state where it is legal to shift the 
    					 *    error symbol, then shift the error symbol.
    					 * 3) Set the error count to three.
    					 * 4) Begin accepting and shifting new tokens.  No new error processing
    					 *    will occur until three tokens have been shifted successfully.
    					 */
    					if self.error_count < 0 {
    					    self.syntax_error(major, minor_union.clone());
    					}
    					
    					let top_major = self.stack_entry_from_tail(0).major;
    					if (top_major == error_symbol) || error_hit {
    					    self.trace_log(format!("Discard input token {}", TOKEN_NAME[major]));
    					    self.destructor(major, minor_union.clone());
    					    major = YYNOCODE;
    					} else {
    					    while { self.stack.len() > 0 && top_major != error_symbol } {
    					        action = self.find_reduce_action(self.stack_entry_from_tail(0).major as i32, error_symbol);
    					        if action < YYNSTATE {
    					            break;
    					        }
    					        self.pop_parser_stack();
    					    }
    					    
    					    if (self.stack.len() == 0) || (major == 0) {
    					        self.destructor(major, minor_union.clone());
    					        self.parse_failed();
    					        major = YYNOCODE;
    					    } else if top_major != error_symbol {
    					        self.shift(action as i32, error_symbol, Rc::new(YYMINORTYPE {
    					            yy0: 0
    					        }));
    					    }
    					    
    					    self.error_count = 3;
    					    error_hit = true;
    					}
                    }
                    None => {
                        /* This is what we do if the grammar does not define ERROR:
    					 * 1) Report an error message, and throw away the input token.
    					 * 2) If the input token is $, then fail the parse.
    					 *
    					 * As before, subsequent error messages are suppressed until three input
    					 * tokens have been successfully shifted.
    					 */
    					if self.error_count <= 0 {
    					    self.syntax_error(major, minor_union.clone());
    					}
    					
    					self.error_count = 3;
    					self.destructor(major, minor_union.clone());
    					if end_of_input {
    					    self.parse_failed();
    					}
    					
    					major = YYNOCODE;
                    }
                }
            }
        }
    }
	
	/*
	 * Logs the provided trace message if the trace file has been set.
	 */
	fn trace_log(&mut self, message: String) {
	    if let Some(ref mut file) = self.trace_file {
	        match self.trace_prompt {
                Some(ref prompt) => {
                    let formatted_message = format!("{}{}\n", prompt, message);
                    assert!(file.write(formatted_message.as_bytes()).is_ok());
                }
                None => {
                    let formatted_message = format!("{}\n", message);
                    assert!(file.write(formatted_message.as_bytes()).is_ok());
                }
            }
	    }
	}
    
}

impl Drop for Parser {
    fn drop(&mut self) {
        while self.stack.len() > 0 {
            /* 
             * Execute the non-terminal destructor on each remaining non-terminal
             * left the stack once the parser leaves scope.
             */
            self.pop_parser_stack();
        }
    }
}
