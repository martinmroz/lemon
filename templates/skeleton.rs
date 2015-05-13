
#![allow(dead_code)]

use std::io;
use std::rc::Rc;

/* 
 * This section comprises all token values.
 *
 * These constants (all generated automatically by the parser generator)
 * specify the various kinds of tokens (terminals) that the parser
 * understands.
 *
 * Each symbol here is a terminal symbol in the grammar.
 */
pub type TokenMajor = CodeNumber;

pub mod token {
    use super::TokenMajor;
    
    /* Numerical value of each major terminal token. */
    pub const PLUS: TokenMajor = 1;
    pub const MINUS: TokenMajor = 2;
    pub const DIVIDE: TokenMajor = 3;
    pub const TIMES: TokenMajor = 4;
    pub const INTEGER: TokenMajor = 5;
}

/*
 *    CodeNumber             is the data type used for storing terminal and nonterminal numbers. 
 *    CODE_NUMBER_NONE       is a number of type CodeNumber which corresponds
 *                           to no legal terminal or nonterminal number. This
 *                           value is used to fill in empty slots of the hash table.
 *
 *    ActionNumber           is the data type used for storing terminal and nonterminal numbers.
 *    ACTION_NUMBER_WILDCARD is an optional action number Option<ActionNumber> indicating the 
 *                           index of the wildcard action in the table.
 *    ACTION_NUMBER_NONE     is the action number indicating no action to take.
 *    ACTION_NUMBER_ACCEPT   is the action number for the Accept action.
 *    ACTION_NUMBER_ERROR    is the action number for the Error action.
 *
 *    STATE_COUNT            the combined number of states.
 *    RULE_COUNT             the number of rules in the grammar
 *    ERROR_SYMBOL           is the optional code number of the error symbol.
 *                           If None, then do no error processing.
 *
 *    TokenMinor             is the data type used for all minor tokens.
 *                           This is typically a union of many types, one of
 *                           which is Token.  The entry in the union
 *                           for base tokens is called "yy0".
 */
type CodeNumber = usize;
const CODE_NUMBER_NONE: CodeNumber = 10;

type ActionNumber = usize;
const ACTION_NUMBER_WILDCARD: Option<ActionNumber> = None;
const ACTION_NUMBER_NONE: ActionNumber = STATE_COUNT + RULE_COUNT + 2;
const ACTION_NUMBER_ACCEPT: ActionNumber = STATE_COUNT + RULE_COUNT + 1;
const ACTION_NUMBER_ERROR: ActionNumber = STATE_COUNT + RULE_COUNT;

const STATE_COUNT: usize = 11;
const RULE_COUNT: usize = 6;
const ERROR_SYMBOL: Option<usize> = None;

#[allow(non_camel_case_types)]
pub enum TokenMinor {
    yyInvalidToken,
    yy0(i32),
}

/*
 * Implementation of the constructor for an empty minor token. This is exclusively used
 * by the Rust parser template.
 */
impl TokenMinor {
    fn new() -> TokenMinor {
        TokenMinor::yy0(0)
    }
}

/* 
 * Next are that tables used to determine what action to take based on the
 * current state and lookahead token.  These tables are used to implement
 * functions that take a state number and lookahead value and return an
 * action integer.
 *
 * Suppose the action integer is N.  Then the action is determined as follows:
 *
 *   0 <= N < STATE_COUNT                       Shift N.  That is, push the lookahead
 *                                              token onto the stack and goto state N.
 *   STATE_COUNT <= N < ACTION_NUMBER_ERROR     Reduce by rule N-STATE_COUNT.
 *   N == ACTION_NUMBER_ERROR                   A syntax error has occurred.
 *   N == ACTION_NUMBER_ACCEPT                  The parser accepts its input.
 *   N == ACTION_NUMBER_NONE                    No such action.  Denotes unused
 *                                              slots in the ACTION_TABLE[] table.
 */

/* 
 * The action table is constructed as a single large table named ACTION_TABLE[].
 * Given state S and lookahead X, the action is computed as
 *
 *      ACTION_TABLE[ SHIFT_OFFSETS[S] + X ]
 *
 * If the index value SHIFT_OFFSETS[S]+X is out of range or if the value
 * LOOKAHEAD_TABLE[SHIFT_OFFSETS[S]+X] is not equal to X or if SHIFT_OFFSETS[S]
 * is equal to SHIFT_USE_DEFAULT, it means that the action is not in the table
 * and that DEFAULT_ACTION_TABLE[S] should be used instead.
 *
 * The formula above is for computing the action when the lookahead is
 * a terminal symbol.  If the lookahead is a non-terminal (as occurs after
 * a reduce action) then the REDUCE_OFFSETS[] array is used in place of
 * the SHIFT_OFFSETS[] array and REDUCE_USE_DEFAULT is used in place of
 * SHIFT_USE_DEFAULT.
 */

/* 
 * The following are the tables generated in this section:
 *
 *  ACTION_TABLE[]          A single table containing all actions.
 *  LOOKAHEAD_TABLE[]       A table containing the lookahead for each entry in
 *                          ACTION_TABLE.  Used to detect hash collisions.
 *  SHIFT_OFFSETS[]         For each state, the offset into ACTION_TABLE for
 *                          shifting terminals.
 *  REDUCE_OFFSETS[]        For each state, the offset into ACTION_TABLE for
 *                          shifting non-terminals after a reduce.
 *  DEFAULT_ACTION_TABLE[]  Default action for each state.
 */
const ACTION_TABLE_LEN: usize = 15;
const ACTION_TABLE: [ActionNumber; ACTION_TABLE_LEN] = [
/*     0 */    11,    4,    2,    3,    1,    3,    1,    6,   18,    5,
/*    10 */    10,    9,    8,   19,    7,
];
const LOOKAHEAD_TABLE: [CodeNumber; 15] = [
/*     0 */     0,    1,    2,    3,    4,    3,    4,    8,    7,    8,
/*    10 */     5,    8,    8,    9,    8,
];
const SHIFT_USE_DEFAULT: i32 = -1;
const SHIFT_MAX: i32 = 7;
const SHIFT_OFFSETS: [i32; 8] = [
/*     0 */     5,    5,    5,    5,    5,    0,    2,    2,
];
const REDUCE_USE_DEFAULT: i32 = -2;
const REDUCE_MAX: i32 = 4;
const REDUCE_OFFSETS: [i32; 5] = [
/*     0 */     1,    4,    6,    3,   -1,
];
const DEFAULT_ACTION_TABLE: [ActionNumber; 11]  = [
/*     0 */    17,   17,   17,   17,   17,   17,   13,   12,   14,   15,
/*    10 */    16,
];

/* 
 * This table maps tokens into fallback tokens.  If a construct like the following:
 *
 *      %fallback ID X Y Z.
 *
 * appears in the grammer, then ID becomes a fallback token for X, Y,
 * and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
 * but it does not parse, the type of the token is changed to ID and
 * the parse is retried before an error is thrown.
 */
const FALLBACK_CODE_TABLE: [CodeNumber; 0] = [
];

/* 
 * The following structure represents a single element of the
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
    
    /* The state number. */
    state_number: i32,      
    
    /* The major token value.  This is the code number for the token at this stack level. */
    major: CodeNumber,
    
    /* The user-supplied minor token value.  This is the value of the token.  */
    minor: Rc<TokenMinor>
}

/* 
 * This table contains information about every rule that is used during the reduce step.
 */
struct RuleInfo {
    
    /* Symbol on the left-hand side of the rule */
    lhs: CodeNumber,
    
    /* Number of right-hand side symbols in the rule */
    rhs_count: u8
}

const RULE_INFO_LIST: [RuleInfo; 6] = [
    RuleInfo { lhs: 7, rhs_count: 1 },
    RuleInfo { lhs: 8, rhs_count: 3 },
    RuleInfo { lhs: 8, rhs_count: 3 },
    RuleInfo { lhs: 8, rhs_count: 3 },
    RuleInfo { lhs: 8, rhs_count: 3 },
    RuleInfo { lhs: 8, rhs_count: 1 },
];

/* 
 * For tracing shifts, the names of all terminals and nonterminals
 * are required, as defined below.
 */
const TOKEN_NAMES: [&'static str; 9] = [
    "$",             "PLUS",          "MINUS",         "DIVIDE",      
    "TIMES",         "INTEGER",       "error",         "program",     
    "expr",        
];

/* 
 * For tracing reduce actions, the names of all rules are required.
 */
const RULE_NAMES: [&'static str; 6] = [
/*   0 */ "program ::= expr",
/*   1 */ "expr ::= expr MINUS expr",
/*   2 */ "expr ::= expr PLUS expr",
/*   3 */ "expr ::= expr TIMES expr",
/*   4 */ "expr ::= expr DIVIDE expr",
/*   5 */ "expr ::= INTEGER",
];

/* 
 * The state of the parser is completely contained in an instance of
 * the following structure.
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
    
    /*
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
    fn pop_parser_stack(&mut self) -> Option<usize> {
        if let Some(top) = self.stack.pop() {
        
            /* Log the removal of the stack entry from the parse stack. */
            self.trace_log(format!("Popping {}", TOKEN_NAMES[top.major]));
            
            /* Execute the non-terminal destructor for the token and return the major. */
            self.destructor(top.major, top.minor);
            return Some(top.major);
        } else {
            return None;
        }
    }
    
    /* 
     * The following function deletes the value associated with a
     * symbol.  The symbol can be either a terminal or nonterminal.
     * "major" is the symbol code, and "yypminor" is a pointer to the value.
     */
    #[allow(unused_variables)]
    fn destructor(&mut self, major: TokenMajor, minor: Rc<TokenMinor>) {
        match major {
            /* 
             * Here is inserted the actions which take place when a
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
     * If the look-ahead token is CODE_NUMBER_NONE, then check to see if the action is
     * independent of the look-ahead.  If it is, return the action, otherwise
     * return YY_NO_ACTION.
     */
    fn find_shift_action(&mut self, i_lookahead: CodeNumber) -> ActionNumber {
        assert!(self.stack.len() > 0);
        
        /*
         * If SHIFT_OFFSETS[S] is equal to SHIFT_USE_DEFAULT (or the index value is out of range)
         * it means that the action is not in the table and DEFAULT_ACTION_TABLE[S] should be used instead.
         */
        let top_entry_state = self.stack_entry_from_tail(0).state_number;
        if (top_entry_state > SHIFT_MAX) || (SHIFT_OFFSETS[top_entry_state as usize] == SHIFT_USE_DEFAULT) {
            return DEFAULT_ACTION_TABLE[top_entry_state as usize];
        }
        
        /*
         * Given state S and lookahead X, the action is computed as ACTION_TABLE[ SHIFT_OFFSETS[S] + X ].
         */
        assert!(i_lookahead != CODE_NUMBER_NONE);
        let i = SHIFT_OFFSETS[top_entry_state as usize] + (i_lookahead as i32);
        if (i >= 0) && (i < ACTION_TABLE_LEN as i32) && (LOOKAHEAD_TABLE[i as usize] == i_lookahead) {
            return ACTION_TABLE[i as usize];
        }
        
        if i_lookahead > 0 {
            /*
             * If the lookahead token is in the fallback table and it's valid, attempt to find
             * a shift action that corresponds.
             */
            if (i_lookahead < FALLBACK_CODE_TABLE.len()) && (FALLBACK_CODE_TABLE[i_lookahead as usize] != 0) {
                let i_fallback = FALLBACK_CODE_TABLE[i_lookahead];
                self.trace_log(format!("FALLBACK_CODE_TABLE {} => {}", TOKEN_NAMES[i_lookahead], TOKEN_NAMES[i_fallback]));
                return self.find_shift_action(i_fallback);
            }
            
            /*
             * If a wildcard action is specified, look it up in the action table.
             */
            match ACTION_NUMBER_WILDCARD {
                Some(wildcard) => {
                    let j = i - (i_lookahead as i32) + (wildcard as i32);
                    if (j >= 0) && (j < ACTION_TABLE_LEN as i32) && (LOOKAHEAD_TABLE[j as usize] == wildcard) {
                        self.trace_log(format!("WILDCARD {} => {}", TOKEN_NAMES[i_lookahead], TOKEN_NAMES[wildcard]));
                        return ACTION_TABLE[j as usize];
                    }
                } 
                None => {
                    /* No wildcard specified. */
                }
            }
        }
        
        return DEFAULT_ACTION_TABLE[top_entry_state as usize];
    }
    
    /*
     * Find the appropriate action for a parser given the non-terminal
     * look-ahead token i_lookahead.
     *
     * If the look-ahead token is CODE_NUMBER_NONE, then check to see if the action is
     * independent of the look-ahead.  If it is, return the action, otherwise
     * return YY_NO_ACTION.
     */
    fn find_reduce_action (&self, state_number: i32, i_lookahead: CodeNumber) -> ActionNumber {
        assert!(state_number <= REDUCE_MAX);
        let mut i = REDUCE_OFFSETS[state_number as usize];
        assert!(i != REDUCE_USE_DEFAULT);
        assert!(i_lookahead != CODE_NUMBER_NONE);
        i += i_lookahead as i32;
        assert!(i >= 0 && i < (ACTION_TABLE_LEN as i32));
        assert!(LOOKAHEAD_TABLE[i as usize] == i_lookahead);
        return ACTION_TABLE[i as usize];
    }
    
    /*
     * The following routine is called if the stack overflows.
     */
    #[allow(unused_variables)]
    fn stack_overflow (&mut self, pminor: &TokenMinor) {
        self.trace_log("Stack overflow.".to_string());
        while self.stack.len() > 0 {
            self.pop_parser_stack();
        }
        
        /* Here code is inserted which will execute if the parser stack ever overflows. */
    }
    
    /*
     * Perform a shift action.
     */
    fn shift(&mut self, new_state: i32, major: usize, pminor: Rc<TokenMinor>) {
        self.stack.push(
            StackEntry {
                state_number: new_state, 
                major: major, 
                minor: pminor
            }
        );
        
        self.trace_log(format!("Shift {} -- Stack:", new_state));
        
        let mut stack_trace = "".to_string();
        for i in self.stack.iter() {
            let token_name_string = TOKEN_NAMES[i.major].to_string();
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
    #[allow(non_snake_case)]
    fn reduce(&mut self, ruleno: usize) {
         
        /* Log the reduce action to the trace file. */
        if ruleno < RULE_NAMES.len() {
            self.trace_log(format!("Reduce [{}]", RULE_NAMES[ruleno]));
        }
         
        let mut yy_goto_minor = TokenMinor::new();
         
        match ruleno {
            /* 
             * Beginning here are the reduction cases.  A typical example follows:
             *
             *   0 => {
             *     #line <lineno> <grammarfile>
             *     { ... }           // User supplied code
             *     #line <lineno> <thisfile>
             *     break;
             *   }
             */
              
            0 => {
                /* program ::= expr */
                match &*self.stack_entry_from_tail(0).minor {
                    &TokenMinor::yy0(A) => { println!("Result={}", A); }
                    _ => { 
                        assert!(false, "Unable to evaluate nonterminal."); 
                    }
                }
            }
            1 => {
                /* expr ::= expr MINUS expr */
                match (&*self.stack_entry_from_tail(2).minor, &*self.stack_entry_from_tail(0).minor) {
                    (&TokenMinor::yy0(A), &TokenMinor::yy0(B)) => { yy_goto_minor = TokenMinor::yy0(A - B); }
                    _ => {
                        assert!(false, "Unable to evaluate nonterminal.");
                    }
                }
            }
            2 => {
                /* expr ::= expr PLUS expr */
                match (&*self.stack_entry_from_tail(2).minor, &*self.stack_entry_from_tail(0).minor) {
                    (&TokenMinor::yy0(A), &TokenMinor::yy0(B)) => { yy_goto_minor = TokenMinor::yy0(A + B); }
                    _ => {
                        assert!(false, "Unable to evaluate nonterminal.");
                    }
                }
            }
            3 => {
                /* expr ::= expr TIMES expr */
                match (&*self.stack_entry_from_tail(2).minor, &*self.stack_entry_from_tail(0).minor) {
                    (&TokenMinor::yy0(A), &TokenMinor::yy0(B)) => { yy_goto_minor = TokenMinor::yy0(A * B); }
                    _ => {
                        assert!(false, "Unable to evaluate nonterminal.");
                    }
                }
            }
            4 => {
                /* expr ::= expr DIVIDE expr */
                match (&*self.stack_entry_from_tail(2).minor, &*self.stack_entry_from_tail(0).minor) {
                    (&TokenMinor::yy0(A), &TokenMinor::yy0(B)) => {
                        if B != 0 {
                            yy_goto_minor = TokenMinor::yy0(A / B);
                        } else {
                            println!("divide by zero\n");
                        }
                    }
                    _ => {
                        assert!(false, "Unable to evaluate nonterminal.");
                    }
                }
            }
            5 => {
                /* expr ::= INTEGER */
                match &*self.stack_entry_from_tail(0).minor {
                    &TokenMinor::yy0(A) => { yy_goto_minor = TokenMinor::yy0(A); }
                    _ => { 
                        assert!(false, "Unable to evaluate nonterminal."); 
                    }
                }
            }
            _ => {
                /* Default: No Action. */
            }
        }
        
        let yy_goto = RULE_INFO_LIST[ruleno].lhs;
        let yy_size = RULE_INFO_LIST[ruleno].rhs_count as usize;
        let yy_action = self.find_reduce_action(self.stack_entry_from_tail(yy_size).state_number, yy_goto);
         
        /* Pop `yy_size` entries off the stack without invoking the destructor. */
        let new_length = self.stack.len() - yy_size;
        self.stack.truncate(new_length);
         
        /* 
         * If we are not debugging and the reduce action popped
         * at least one element off the stack, then we can shift
         * the new element back onto the stack here 
         */
        if yy_action < STATE_COUNT {
            self.shift(yy_action as i32, yy_goto, Rc::new(yy_goto_minor));
        } else {
            assert!(yy_action == ACTION_NUMBER_ACCEPT);
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
    fn syntax_error (&mut self, major: usize, minor: Rc<TokenMinor>) {
        match *self.stack_entry_from_tail(0).minor {
            TokenMinor::yy0(token) => { 
                println!("Syntax error!");
            }
            _ => { 
                assert!(false, "Unable to evaluate nonterminal."); 
            }
        }
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
    pub fn parse(&mut self, new_major: TokenMajor, minor: Option<TokenMinor>) {
        let mut major = new_major;
        let mut error_hit = false;
        let end_of_input = major == 0;
        let minor_union = match minor {
            Some(minor_value) => Rc::new(minor_value),
            None => Rc::new(TokenMinor::new())
        };
        
        /* Shift the end-of-file token ($) if the stack is empty. */
        if self.stack.len() == 0 {
            self.error_count = -1;
            self.shift(0, 0, Rc::new(TokenMinor::new()));
        }
        
        self.trace_log(format!("Input {}", TOKEN_NAMES[major]));
        
        while (major != CODE_NUMBER_NONE) && (self.stack.len() > 0) {
            let mut action = self.find_shift_action(major);
            
            if action < STATE_COUNT {
                /* It is not possible to shift the $ token */
                assert!(end_of_input == false);
                self.shift(action as i32, major, minor_union.clone());
                self.error_count -= 1;
                major = CODE_NUMBER_NONE;
            } else if action < ACTION_NUMBER_ERROR {
                self.reduce(action - STATE_COUNT);
            } else {
                /*
                 * A syntax error has occurred. The response to an error depends upon whether
                 * or not the grammar defines an error token "ERROR".
                 */
                assert!(action == ACTION_NUMBER_ERROR);
                self.trace_log(format!("Syntax error!"));
                
                match ERROR_SYMBOL {
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
                            self.trace_log(format!("Discard input token {}", TOKEN_NAMES[major]));
                            self.destructor(major, minor_union.clone());
                            major = CODE_NUMBER_NONE;
                        } else {
                            while { self.stack.len() > 0 && top_major != error_symbol } {
                                action = self.find_reduce_action(self.stack_entry_from_tail(0).major as i32, error_symbol);
                                if action < STATE_COUNT {
                                    break;
                                }
                                self.pop_parser_stack();
                            }
                            
                            if (self.stack.len() == 0) || (major == 0) {
                                self.destructor(major, minor_union.clone());
                                self.parse_failed();
                                major = CODE_NUMBER_NONE;
                            } else if top_major != error_symbol {
                                self.shift(action as i32, error_symbol, Rc::new(TokenMinor::new()));
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
                        
                        major = CODE_NUMBER_NONE;
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
