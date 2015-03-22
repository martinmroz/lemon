/* Copyright (GPL) 2004 mchirico@users.sourceforge.net or mchirico@comcast.net
*/
%include {
#include <stdio.h>
#include "ex2def.h"
#include "example2.h"
}

%token_type {Token}
%default_type {Token}

%type expr {Token}
%type NUM {Token}

%left PLUS MINUS.
%left DIVIDE TIMES.

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
