#ifndef __Geodiag_cmor_FieldSpecsAst__
#define __Geodiag_cmor_FieldSpecsAst__

#include "geodiag_cmor_commons.h"
#include "FieldSpecsAstNode.h"

namespace geodiag_cmor {

class FieldSpecsAst {
    vector<FieldSpecsAstNode*> sectionNodes;
public:
    void reset() { sectionNodes.clear(); }

    void addSectionNode(FieldSpecsAstNode *sectionNode) {
        sectionNodes.push_back(sectionNode);
    }

    int getNumSectionNode() const { return sectionNodes.size(); }

    const vector<FieldSpecsAstNode*>& getSectionNodes() const {
        return sectionNodes;
    }
};

} // geodiag_cmor

#endif // __Geodiag_cmor_FieldSpecsAst__
