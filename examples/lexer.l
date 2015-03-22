%{
#include "lexglobal.h"
#include "example5.h"
#include <string.h>
#include <math.h>

int line = 1, col = 1;
%}
%%

[0-9]+|[0-9]*\.[0-9]+	{ col += strlen(yytext);
			  yylval.dval = atof(yytext);
			  return NUM; }

[ \t]			{ col += strlen(yytext); }
			/* ignore but count white space */

[A-Za-z][A-Za-z0-9]*	{ return 0; }
			/* ignore but needed for variables */

"+"			{ return PLUS; }
"-"			{ return MINUS; }
"*"			{ return TIMES; }
"/"			{ return DIVIDE; }

\n			{ col = 0; ++line; return NEWLINE; }

.			{ col += strlen(yytext); return yytext[0]; }
%%
/**
 * reset the line and column count
 */
void reset_lexer(void)
{
  line = 1;
  col  = 1;
}

/**
 * yyerror() is invoked when the lexer or the parser encounter
 * an error. The error message is passed via *s
 */
void yyerror(char *s)
{
  printf("error: %s at line: %d col: %d\n", s, line, col);
}

int yywrap(void)
{
  return 1;
}
