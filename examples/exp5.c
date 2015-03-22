#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "example5.c"

#define BUFS 1024

/**
 * We have to declare these here - they're not  in any header files
 * we can inclde.  yyparse() is declared with an empty argument list
 * so that it is compatible with the generated C code from bison.
 */
extern FILE *yyin;
typedef struct yy_buffer_state *YY_BUFFER_STATE;

int             yylex( void );
YY_BUFFER_STATE yy_scan_string( const char * );
void            yy_delete_buffer( YY_BUFFER_STATE );

int main(int argc,char** argv)
{
  int n;
  int yv;
  char buf[BUFS+1];
  void *pParser = ParseAlloc (malloc);
  Token t0;

  t0.n = 0;
  t0.value = 0;

  printf ("Enter an expression like 3+5 <return>\n");
  printf ("  Terminate with ^D\n");

  while ( ( n=read(fileno(stdin), buf, BUFS )) >  0)
    {
      buf[n]='\0';
      yy_scan_string(buf);

      /* on EOF yylex will return 0 */
      while( (yv=yylex()) != 0)
        {
          printf (" yylex() %d yylval.dval %g\n", yv, yylval.dval);
          t0.value=yylval.dval;
          Parse (pParser, yv, t0);
        }
    }
  Parse (pParser, 0, t0);
  ParseFree(pParser, free );
  return 0;
}
