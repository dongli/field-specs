#include "FieldSpecsDriver.h"
#include "FieldSpecsScanner.h"
#include "FieldSpecsParser.h"

namespace geodiag_cmor {

FieldSpecsDriver::FieldSpecsDriver(FieldSpecsAst *ast,
                                   bool traceScanning,
                                   bool traceParsing) {
    this->traceScanning = traceScanning;
    this->traceParsing = traceParsing;
    this->ast = ast;
}

bool FieldSpecsDriver::parse(const string &input) {
    istringstream in(input);
    scanner = new FieldSpecsScanner(&in);
    scanner->set_debug(traceScanning);

    parser = new FieldSpecsParser(*this);
    parser->set_debug_level(traceParsing);
    bool ret = parser->parse() == 0;

    delete scanner;
    delete parser;

    return ret;
}

void FieldSpecsDriver::error(const location &loc, const string &msg) {
    cerr << "[Error]: " << loc << ": " << msg << endl;
}

void FieldSpecsDriver::error(const string &msg) {
    cerr << "[Error]: " << msg << endl;
}

}
