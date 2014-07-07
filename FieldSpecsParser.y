%{
#include "FieldSpecsAstNode.h"
%}

%require "2.3"

%debug

%start specs

%language "C++"
%defines
%locations
%define api.namespace { geodiag_cmor }
%define parser_class_name { FieldSpecsParser }
%skeleton "lalr1.cc"
%parse-param { class FieldSpecsDriver &driver }
%error-verbose

%union {
    double doubleValue;
    std::string *stringValue;
    class FieldSpecsAstNode *astNode;
}

%token END 0
%token NUMBER ID
%token ADD SUB MUL DIV
%token OPEN_PAREN CLOSE_PAREN COMMA COLON
%token D_2 D_3 UNITS KEEP_VERT_COORD

%left ADD SUB
%left MUL DIV
%nonassoc NEG

%type <doubleValue> NUMBER
%type <stringValue> ID UNITS
%type <astNode> number id argumentList funcionCall expression section

%destructor { delete $$; } ID UNITS
%destructor { delete $$; } number id argumentList funcionCall expression section

%{
#include "FieldSpecsDriver.h"
#include "FieldSpecsScanner.h"

#define yylex driver.getScanner().lex
%}

%%

number: NUMBER { $$ = new FieldSpecsAstNumberNode($1); };

id: ID { $$ = new FieldSpecsAstIdNode($1); };

argumentList: id {
                FieldSpecsAstIdListNode *idListNode = new FieldSpecsAstIdListNode;
                idListNode->addIdNode(dynamic_cast<FieldSpecsAstIdNode*>($1));
                $$ = idListNode;
            }
            | argumentList COMMA id {
                FieldSpecsAstIdListNode *idListNode = dynamic_cast<FieldSpecsAstIdListNode*>($1);
                idListNode->addIdNode(dynamic_cast<FieldSpecsAstIdNode*>($3));
                $$ = idListNode;
            };

funcionCall: id OPEN_PAREN argumentList CLOSE_PAREN {
                FieldSpecsAstIdNode *idNode = dynamic_cast<FieldSpecsAstIdNode*>($1);
                FieldSpecsAstIdListNode *argListNode = dynamic_cast<FieldSpecsAstIdListNode*>($3);
                FieldSpecsAstFuncNode *funcNode = new FieldSpecsAstFuncNode(idNode->getIdName(), argListNode);
                $$ = funcNode;
            };

expression: number {
                $$ = $1;
            }
          | id {
                $$ = $1;
            }
          | funcionCall {
                $$ = $1;
            }
          | expression ADD expression {
                $$ = new FieldSpecsAstExprNode($1, ADD_OP, $3);
            }
          | expression SUB expression {
                $$ = new FieldSpecsAstExprNode($1, SUB_OP, $3);
            }
          | expression MUL expression {
                $$ = new FieldSpecsAstExprNode($1, MUL_OP, $3);
            }
          | expression DIV expression {
                $$ = new FieldSpecsAstExprNode($1, DIV_OP, $3);
            }
          | SUB expression %prec NEG {
                $$ = new FieldSpecsAstExprNode($2, NEG_OP);
            }
          | OPEN_PAREN expression CLOSE_PAREN {
                $$ = $2;
            }
          ;

section: expression { $$ = $1; }
       | D_2 { $$ = new FieldSpecsAstSpecNode(D_2_SPEC); }
       | D_3 { $$ = new FieldSpecsAstSpecNode(D_3_SPEC); }
       | UNITS { $$ = new FieldSpecsAstSpecNode(UNITS_SPEC, $1); }
       | KEEP_VERT_COORD { $$ = new FieldSpecsAstSpecNode(KEEP_VERT_COORD_SPEC); }
       ;

specs:
     | section { driver.getAst().addSectionNode($1); }
     | specs COLON section {
            driver.getAst().addSectionNode($3);
        }
     ;

%%

void geodiag_cmor::FieldSpecsParser::error(const FieldSpecsParser::location_type &loc,
                                           const string &msg) {
    driver.error(loc, msg);
}
