/*
Reference Chapter 6:
"The C++ Programming Language", Special Edition.
Bjarne Stroustrup,Addison-Wesley Pub Co; 3 edition (February 15, 2000)
ISBN: 0201700735
*/
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cctype>

using namespace std;

enum Token_value {
  NAME, NUMBER, END, INC, DEC,
  PLUS='+', MINUS='-', MUL='*', DIV='/',
  PRINT=';', ASSIGN='=', LP='(', RP=')'
};

Token_value curr_tok = PRINT;

double number_value;
string string_value;

double expr(bool);
double term(bool);
double error(const string&);
double prim(bool);
Token_value get_token();

map<string, double> table;

double expr(bool get)
{
  double left = term(get);

  for(;;)
    switch(curr_tok) {
    case PLUS:
      left += term(true);
      break;
    case MINUS:
      left -= term(true);
      break;
    default:
      return left;
    }
}

double term(bool get)
{
  double left = prim(get);

  for(;;)
    switch(curr_tok) {
    case MUL:
      left *= prim(true);
      break;
    case DIV:
      if (double d = prim(true)) {
	left /=d;
	break;
      }
      return error("divide by 0");
    default:
      return left;
    }
}

double prim(bool get)
{
  if(get) get_token();

  switch(curr_tok) {
  case NUMBER:
    {
      double v = number_value;
      get_token();
      return v;
    }
  case NAME:
    {

      double& v = table[string_value];
      get_token();
      if (curr_tok  == ASSIGN)
        v = expr(true);
      if (curr_tok == INC)  //r++;
	v = v+1;
      if (curr_tok == DEC)  //r++;
	v = v-1;
      return v;
    }
  case INC:
    return prim(true)+1;
  case DEC:
    return prim(true)-1;
  case MINUS:
    return -prim(true);
  case LP:
    {
      double e=expr(true);
      if(curr_tok != RP) return error(" ')' expected");
      get_token();
      return e;
    }
  default:
    return error("primary expected");
  }
}

Token_value get_token()
{
  char ch;

  do {
    if(!cin.get(ch)) return curr_tok = END;
  } while(ch!='\n' && isspace(ch));

  switch(ch) {
  case ';':
  case '\n':
    return curr_tok=PRINT;
  case 0:
    return curr_tok=END;
  case '+':    // added for ++name and name++
    {
      char c;
      if ( cin.get(c) && c == '+' )
	return curr_tok=INC;

      cin.putback(c);
      return curr_tok=Token_value(ch);
    }
  case '-':
    {
      char c;
      if ( cin.get(c) && c == '-' )
	return curr_tok=DEC;

      cin.putback(c);
      return curr_tok=Token_value(ch);
    }
  case '*':
  case '/':
  case '(':
  case ')':
  case '=':
    return curr_tok=Token_value(ch);

  case '0':case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
  case '.':
    cin.putback(ch);
    cin >> number_value;
    return curr_tok=NUMBER;
  default:
    if (isalpha(ch)){
      string_value=ch;
      while(cin.get(ch) && isalnum(ch)) string_value.push_back(ch);
      cin.putback(ch);
      return curr_tok=NAME;
    }
    error("bad token");
    return curr_tok=PRINT;
  }
}

int no_of_errors;

double error(const string& s)
{
  no_of_errors++;
  cerr << "error: " << s << '\n';
  return 1;
}

istream* input;

int main(int argc, char* argv[])
{
  switch(argc) {
  case 1:
    input = &cin;
    break;
  case 2:
    input = new istringstream(argv[1]);
    break;
  default:
    error("too man arguments");
    return 1;
  }

  table["pi"] = 3.1415926535897932385;
  table["e"] = 2.718281818284590452354;

  while(cin) {
    get_token();
    if( curr_tok == END) break;
    if( curr_tok == PRINT) continue;
    cout << expr(false) << '\n';
  }

  if(input != &cin)
    delete input;
  return no_of_errors;
}
