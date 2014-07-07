%{
#include "FieldSpecsScanner.h"

typedef geodiag_cmor::FieldSpecsParser::token token;
typedef geodiag_cmor::FieldSpecsParser::token_type token_type;

#define yyterminate() return token::END

%}

%option c++
%option prefix="FieldSpecs"
%option batch
%option debug
%option noyywrap
%option unput
%option stack

%{
#define YY_USER_ACTION yylloc->columns(yyleng);
%}

%%

%{
    yylloc->step();
%}

"keep_vert_coord" { return token::KEEP_VERT_COORD; }

"2d" { return token::D_2; }

"3d" { return token::D_3; }

[0-9]+("."[0-9]*([eEdD][+-][0-9]+)?)? {
    yylval->doubleValue = atof(yytext);
    return token::NUMBER;
}

[A-Za-z_][A-Za-z0-9_]* {
    yylval->stringValue = new string(yytext, yyleng);
    return token::ID;
}

"+" { return token::ADD; }

"-" { return token::SUB; }

"*" { return token::MUL; }

"/" { return token::DIV; }

[ \t]+ {}

"(" { return token::OPEN_PAREN; }

")" { return token::CLOSE_PAREN; }

"," { return token::COMMA; }

":" { return token::COLON; }

%%

namespace geodiag_cmor {

FieldSpecsScanner::FieldSpecsScanner(istream *in, ostream *out)
    : ExampleFlexLexer(in, out) {
    
}

FieldSpecsScanner::~FieldSpecsScanner() {
}

void FieldSpecsScanner::set_debug(bool b) {
    yy_flex_debug = b;
}

}

#ifdef yylex
#undef yylex
#endif

int ExampleFlexLexer::yylex() {
    std::cerr << "[Error]: We shouldn't in ExampleFlexLexer::yylex()!" << std::endl;
    return 0;
}

int ExampleFlexLexer::yywrap() {
    return 1;
}
