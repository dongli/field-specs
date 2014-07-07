#include "FieldSpecsDriver.h"
#include "FieldSpecsAst.h"

using namespace geodiag_cmor;

int main(void)
{
    geodiag_cmor::FieldSpecsAst ast;
    geodiag_cmor::FieldSpecsDriver driver(&ast, false, false);

    vector<string> inputs;
    inputs.push_back("1");
    inputs.push_back("2.0e+3");
    inputs.push_back("U");
    inputs.push_back("abs(U)");
    inputs.push_back("calc_pt(T, P, Q)");
    inputs.push_back("1+2");
    inputs.push_back("1.0e+3*(X+2)");
    inputs.push_back("2d:keep_vert_coord");

    for (int j = 0; j < inputs.size(); ++j) {
        cout << "Input: " << inputs[j] << endl;
        ast.reset();
        if (driver.parse(inputs[j])) {
            cout << "AST Nodes:" << endl;
            for (int i = 0; i < ast.getNumSectionNode(); ++i) {
                ast.getSectionNodes()[i]->print(cout, 1);
            }
        } else {
            cerr << "[Error]: Failed to parse!" << endl;
        }
    }

    return 0;
}
