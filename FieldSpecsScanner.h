#ifndef __Geodiag_cmor_FieldSpecsScanner__
#define __Geodiag_cmor_FieldSpecsScanner__

#ifndef YY_DECL
#define YY_DECL \
    token_type geodiag_cmor::FieldSpecsScanner::lex( \
        geodiag_cmor::FieldSpecsParser::semantic_type *yylval, \
        geodiag_cmor::FieldSpecsParser::location_type *yylloc \
    )
#endif

#undef yyFlexLexer
#define yyFlexLexer ExampleFlexLexer
#include "FlexLexer.h"

#include "geodiag_cmor_commons.h"
#include "FieldSpecsParser.h"

typedef geodiag_cmor::FieldSpecsParser::token token;
typedef geodiag_cmor::FieldSpecsParser::token_type token_type;

namespace geodiag_cmor {

class FieldSpecsScanner : public ExampleFlexLexer {
public:
    FieldSpecsScanner(istream *in = 0, ostream *out = 0);
    virtual ~FieldSpecsScanner();

    virtual token_type lex(
        FieldSpecsParser::semantic_type *yylval,
        FieldSpecsParser::location_type *yylloc
    );

    void set_debug(bool b);
};

} // geodiag_cmor

#endif // __Geodiag_cmor_FieldSpecsScanner__
