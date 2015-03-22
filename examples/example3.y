/* Copyright (GPL) 2004 mchirico@users.sourceforge.net or mchirico@comcast.net
*/
%include {
#include <stdio.h>
#include "ex3def.h"
#include "example3.h"

  void token_destructor(Token t)
  {
    printf ("In token_destructor t.value=%d \n", t.value);
    printf ("In token_destructor t.n=%d \n", t.n);
  }
}

%token_type {Token}
%default_type {Token}
%token_destructor { token_destructor($$); }

%type expr {Token}
%type id {Token}

%left PLUS MINUS.
%left DIVIDE TIMES.

%parse_accept {
  printf("parsing complete!\n\n\n");
}

%syntax_error {
  printf ("Syntax error!\n");
}

program ::= expr(A).			{ printf ("Result.value=%d\n", A.value);
					  printf ("Result.n=%d\n", A.n);
					}
expr(A) ::= expr(B) MINUS  expr(C).	{ A.value = B.value - C.value;
                                          A.n = B.n+1  + C.n+1;
					}
expr(A) ::= expr(B) PLUS  expr(C).	{ A.value = B.value + C.value;
					  A.n = B.n+1  + C.n+1;
					}
expr(A) ::= expr(B) TIMES  expr(C).	{ A.value = B.value * C.value;
					  A.n = B.n+1  + C.n+1;
					}
expr(A) ::= expr(B) DIVIDE expr(C).	{ if (C.value != 0) {
					    A.value = B.value / C.value;
					    A.n = B.n+1 + C.n+1;
					  } else {
					    printf ("divide by zero\n");
					  }
					}
expr(A) ::= NUM(B).			{ A.value = B.value;
					  A.n = B.n+1;
					}
