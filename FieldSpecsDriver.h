#ifndef __Geodiag_cmor_FieldSpecsDriver__
#define __Geodiag_cmor_FieldSpecsDriver__

#include "geodiag_cmor_commons.h"
#include "FieldSpecsAst.h"

namespace geodiag_cmor {

class FieldSpecsDriver {
    bool traceScanning;
    bool traceParsing;

    class FieldSpecsScanner *scanner;
    class FieldSpecsParser *parser;
    FieldSpecsAst *ast;
public:
    FieldSpecsDriver(FieldSpecsAst *ast, bool traceScanning, bool traceParsing);

    bool parse(const string &input);

    void error(const class location &loc, const string &msg);

    void error(const string &msg);

    class FieldSpecsScanner& getScanner() { return *scanner; }

    FieldSpecsAst& getAst() { return *ast; }
};

} // geodiag_cmor

#endif // __Geodiag_cmor_FieldSpecsDriver__
