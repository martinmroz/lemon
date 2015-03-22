/*
 * Simple lemon parser example for D language.
 * Copyright (GPL) 2008 Serge Vakulenko <serge@vak.ru>
 */
%token_type {int}

%left PLUS MINUS.
%left DIVIDE TIMES.

%syntax_error {
	writefln ("Syntax error!");
}

program ::= expr(A).			{ writefln ("Result=%d", A); }

expr(A) ::= expr(B) MINUS  expr(C).	{ A = B - C; }
expr(A) ::= expr(B) PLUS   expr(C).	{ A = B + C; }
expr(A) ::= expr(B) TIMES  expr(C).	{ A = B * C; }
expr(A) ::= expr(B) DIVIDE expr(C).	{ if (C != 0) {
					    A = B / C;
					  } else {
					    writefln ("divide by zero");
					  }
					}
expr(A) ::= INTEGER(B).			{ A = B; }

%include {
	private import std.stdio;
	unittest {
		parser_t parser = new parser_t ();

		/* First input: 15 / 5 */
		parser.parse (INTEGER, 15);
		parser.parse (DIVIDE, 0);
		parser.parse (INTEGER, 5);
		parser.parse (0, 0);

		/* Second input: 50 + 125 */
		parser.parse (INTEGER, 50);
		parser.parse (PLUS, 0);
		parser.parse (INTEGER, 125);
		parser.parse (0, 0);

		/* Third input: 50 * 125 + 125 */
		parser.parse (INTEGER, 50);
		parser.parse (TIMES, 0);
		parser.parse (INTEGER, 125);
		parser.parse (PLUS, 0);
		parser.parse (INTEGER, 125);
		parser.parse (0, 0);
	}
	debug (parser) {
		void main()
		{
			/* Empty. */
		}
	}
}
