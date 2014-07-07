#include "FieldSpecs.h"

int main(void)
{
    geodiag_cmor::FieldSpecsAst ast;
    geodiag_cmor::FieldSpecsDriver driver(&ast, false, false);

    std::vector<std::string> inputs;
    inputs.push_back("1");
    inputs.push_back("2.0e+3");
    inputs.push_back("U");
    inputs.push_back("abs(U)");
    inputs.push_back("calc_pt(T, P, Q)");
    inputs.push_back("1+2");
    inputs.push_back("1.0e+3*(X+2)");
    inputs.push_back("2d:keep_vert_coord");

    for (int j = 0; j < inputs.size(); ++j) {
        std::cout << "--> Input: " << inputs[j] << std::endl;
        ast.reset();
        if (driver.parse(inputs[j])) {
            ast.print(std::cout);
        } else {
            std::cerr << "[Error]: Failed to parse!" << std::endl;
        }
    }

    return 0;
}
