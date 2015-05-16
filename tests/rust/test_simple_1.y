/* 
 * Derived from work by M. Chirico.
 * Copyright (GPL) 2004 mchirico@users.sourceforge.net or mchirico@comcast.net
 */
%include {}

%token_type {i32}

%left PLUS MINUS.
%left DIVIDE TIMES.

%syntax_error {
  println!("Syntax error!\n");
}

program ::= expr(A).                { printf ("Result=%d\n", A); }

expr(X) ::= expr(A) MINUS  expr(B).	{ X = A - B; }
expr(X) ::= expr(A) PLUS   expr(B).	{ X = A + B; }
expr(X) ::= expr(A) TIMES  expr(B).	{ X = A * B; }
expr(X) ::= expr(A) DIVIDE expr(B).	{ 
  if (B != 0) {
    X = A / B;
  } else {
    printf ("divide by zero\n");
  }
}

expr(X) ::= INTEGER(A).			        { X = A; }
