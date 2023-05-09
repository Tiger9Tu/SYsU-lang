%{
#include <cctype>
#include <cstdio>
#include <string>
#define YYEOF 0
int yylex();
int main() {
  do {
  } while (yylex() != YYEOF);
}
std::string yyloc = "<stdin>";
int yyrow = 1, spaceflag = 0, newlineflag = 1, yycolumn = 1, yycolpre = 1;
#define YY_USER_ACTION                                                         \
  do {                                                                         \
    yycolumn += yyleng;                                                        \
  } while (0);



void prt(std::string sym){
  std::fprintf(yyout, "%s '%s'", sym.c_str(), yytext);
  std::fprintf(yyout,"\t");
  if (newlineflag == 1){
    std::fprintf(yyout, "[StartOfLine]"); 
    newlineflag = 0;
  }
  std::fprintf(yyout,"\t");
  if (spaceflag == 1){
    std::fprintf(yyout, "[LeadingSpace]");
  }
  std::fprintf(yyout,"Loc=<%s:%d:%d>\n", yyloc.c_str(), yyrow, yycolumn - yyleng);
  spaceflag = 0;
}
%}
%option noyywrap
%%

#.* {
  std::string s(yytext);
  auto l = s.find("\""), r = s.rfind("\"");
  yyloc = s.substr(l + 1, r - l - 1);
  for (int i = 0; i < s.size(); ++i)
    if (std::isdigit(s[i])) {
      for (yyrow = 0; i < s.size() && std::isdigit(s[i]); ++i)
        yyrow = yyrow * 10 + s[i] - '0';
      --yyrow;
      break;
    }
}

\n {
  ++yyrow;
  yycolpre = yycolumn;
  yycolumn = 1;
  newlineflag = 1;
}

[[:space:]] {spaceflag = 1;}


(int)|(return)|(float)|(long)|(do)|(void)|(if)|(else)|(while)|(continue)|(break)|(char) {
  prt(yytext);
  return ~YYEOF;
}

\( {
  prt("l_paren");
  return ~YYEOF;
}
\) {
  prt("r_paren");
  return ~YYEOF;
}
\{ {
  prt("l_brace");
}
\} {
  prt("r_brace");
  return ~YYEOF;
}
; {
  prt("semi");
  return ~YYEOF;
}

const {
  prt("const");
  return ~YYEOF;
}

, {
  prt("comma");
  return ~YYEOF;
}

= {
  prt("equal");
  return ~YYEOF;
}

(ident) {
  prt("identifier");
  return ~YYEOF;
}


intconst {
  prt("numeric_constant");
  return ~YYEOF;
}

\! {
  prt("exclaim");
  return ~YYEOF;
}

\* {
  prt("star");
  return ~YYEOF;
}

\/ {
  prt("slash");
  return ~YYEOF;
}

\% {
  prt("percent");
  return ~YYEOF;
}

\< {
  prt("less");
  return ~YYEOF;
}

\> {
  prt("greater");
  return ~YYEOF;
}

\<\= {
  prt("lessequal");
  return ~YYEOF;
}

\>\= {
  prt("greaterequal");
  return ~YYEOF;
}

\=\= {
  prt("equalequal");
  return ~YYEOF;
}

\!\= {
  prt("exclaimequal");
  return ~YYEOF;
}

\&\& {
  prt("ampamp");
  return ~YYEOF;
}

\|\| {
  prt("pipepipe");
  return ~YYEOF;
}


\[ {
  prt("l_square");
  return ~YYEOF;
}

\] {
  prt("r_square");
  return ~YYEOF;
}


\- {
  prt("minus");
  return ~YYEOF;
}

\+ {
  prt("plus");
  return ~YYEOF;
}

\.\.\. {
  prt("ellipsis");
  return ~YYEOF;
}

0x([0-9|a-f|A-F]+)?((\.)?([0-9|a-f|A-F]+([P|p|E|e][+|-]?[0-9|a-f|A-F]+)?)?)? {
  prt("numeric_constant");
  return ~YYEOF;
}

([0-9]+)?((\.)?([0-9]+([P|p|E|e][+|-]?[0-9]+)?)?)? {
  prt("numeric_constant");
  return ~YYEOF;
}



[a-zA-Z_][a-zA-Z_0-9]* {
  prt("identifier");
  return ~YYEOF;
}

\"([^\\\"]|(\\\\)|(\\\")|(\\n)|(\\t))*\" {
  prt("string_literal");
  return ~YYEOF;
}

<<EOF>> {
  std::fprintf(yyout, "eof ''\t\tLoc=<%s:%d:%d>\n", yyloc.c_str(), yyrow - 1,
               yycolpre - yyleng);
  return YYEOF;
}

. {
  std::fprintf(yyout, "unknown '%s'\t\tLoc=<%s:%d:%d>\n", yytext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
%%