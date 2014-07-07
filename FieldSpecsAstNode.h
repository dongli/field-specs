#ifndef __Geodiag_cmor_FieldSpecsAstNode__
#define __Geodiag_cmor_FieldSpecsAstNode__

#include "geodiag_cmor_commons.h"

namespace geodiag_cmor {

enum SpecType {
    D_2_SPEC, D_3_SPEC, UNITS_SPEC, KEEP_VERT_COORD_SPEC
};

enum OpType {
    ADD_OP, SUB_OP, MUL_OP, DIV_OP, NEG_OP, NO_OP
};

enum AstNodeType {
    ID_NODE, ID_LIST_NODE, FUNC_NODE, ADD_NODE, SUB_NODE, MUL_NODE, DIV_NODE,
    NEG_NODE, SPEC_NODE, NUMBER_NODE, EXPR_NODE
};

class FieldSpecsAstNode {
protected:
    AstNodeType type;
public:
    virtual ~FieldSpecsAstNode() {}

    virtual AstNodeType getType() const { return type; }

    virtual void print(std::ostream &os, int depth = 0) const = 0;

    static inline string indent(int depth) { return string(depth*2, ' '); }
};

class FieldSpecsAstIdNode : public FieldSpecsAstNode {
    string idName;
public:
    FieldSpecsAstIdNode(const string *idName) {
        type = ID_NODE;
        this->idName = *idName;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[ID]: " << idName << endl;
    }

    const string& getIdName() { return idName; }
};

class FieldSpecsAstIdListNode : public FieldSpecsAstNode {
    vector<FieldSpecsAstIdNode*> idNodes;
public:
    FieldSpecsAstIdListNode() {
        type = ID_LIST_NODE;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[ID List]: " << endl;
        for (int i = 0; i < idNodes.size(); ++i) {
            idNodes[i]->print(os, depth+1);
        }
    }

    void addIdNode(FieldSpecsAstIdNode *idNode) {
        idNodes.push_back(idNode);
    }

    vector<FieldSpecsAstIdNode*>& getIdNodes() {
        return idNodes;
    }
};

class FieldSpecsAstFuncNode : public FieldSpecsAstNode {
    string funcName;
    FieldSpecsAstIdListNode *argListNode;
public:
    FieldSpecsAstFuncNode(const string funcName,
                          FieldSpecsAstIdListNode *argListNode) {
        type = FUNC_NODE;
        this->funcName = funcName;
        this->argListNode = argListNode;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Function]: " << funcName << endl;
        argListNode->print(os, depth+1);
    }

    void getAllIdNames(vector<string> &idNames) {
        for (int i = 0; i < argListNode->getIdNodes().size(); ++i) {
            idNames.push_back(argListNode->getIdNodes()[i]->getIdName());
        }
    }

    void evaluate(const map<string, double> &symTab, double &currVal) const {

    }
};

class FieldSpecsAstSpecNode : public FieldSpecsAstNode {
    SpecType spec;
    string value;
public:
    FieldSpecsAstSpecNode(SpecType spec) {
        type = SPEC_NODE;
        this->spec = spec;
    }
    FieldSpecsAstSpecNode(SpecType spec, const string *value) {
        type = SPEC_NODE;
        this->spec = spec;
        this->value = *value;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Spec]: ";
        switch (spec) {
            case D_2_SPEC:
                os << "2d";
                break;
            case D_3_SPEC:
                os << "3d";
                break;
            case UNITS_SPEC:
                os << "units: " << value;
                break;
            case KEEP_VERT_COORD_SPEC:
                os << "keep_vert_coord";
                break;
        }
        os << endl;
    }

    SpecType getSpec() const { return spec; }

    const string& getValue() const { return value; }
};

class FieldSpecsAstNumberNode : public FieldSpecsAstNode {
    double value;
public:
    FieldSpecsAstNumberNode(double value) {
        type = NUMBER_NODE;
        this->value = value;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Number]: " << value << endl;
    }

    double getValue() const { return value; }
};

class FieldSpecsAstExprNode : public FieldSpecsAstNode {
    OpType op;
    FieldSpecsAstNode *left;
    FieldSpecsAstNode *right;
public:
    FieldSpecsAstExprNode(FieldSpecsAstNode *left,
                          OpType op = NO_OP,
                          FieldSpecsAstNode *right = NULL) {
        type = EXPR_NODE;
        this->left = left;
        this->op = op;
        this->right = right;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Expression]:" << endl;
        os << indent(depth+1) << "[Left]:" << endl;
        left->print(os, depth+2);
        os << indent(depth+1) << "[Operator]: ";
        switch (op) {
            case ADD_OP:
                os << "+" << endl;
                break;
            case SUB_OP: case NEG_OP:
                os << "-" << endl;
                break;
            case MUL_OP:
                os << "*" << endl;
                break;
            case DIV_OP:
                os << "/" << endl;
                break;
            case NO_OP:
                os << "NO OPERATOR" << endl;
                break;
        }
        if (right != NULL) {
            os << indent(depth+1) << "[Right]:" << endl;
            right->print(os, depth+2);
        }
    }

    void getAllIdNames(vector<string> &idNames) {
        if (left->getType() == ID_NODE) {
            FieldSpecsAstIdNode *node = dynamic_cast<FieldSpecsAstIdNode*>(left);
            idNames.push_back(node->getIdName());
        } else if (left->getType() == EXPR_NODE) {
            FieldSpecsAstExprNode *node = dynamic_cast<FieldSpecsAstExprNode*>(left);
            node->getAllIdNames(idNames);
        }
        if (right == NULL) return;
        if (right->getType() == ID_NODE) {
            FieldSpecsAstIdNode *node = dynamic_cast<FieldSpecsAstIdNode*>(right);
            idNames.push_back(node->getIdName());
        } else if (right->getType() == EXPR_NODE) {
            FieldSpecsAstExprNode *node = dynamic_cast<FieldSpecsAstExprNode*>(right);
            node->getAllIdNames(idNames);
        }
    }

    void evaluate(const map<string, double> &symTab, double &currVal) const {
        if (left->getType() == ID_NODE) {
            FieldSpecsAstIdNode *node = dynamic_cast<FieldSpecsAstIdNode*>(left);
            currVal = symTab.at(node->getIdName());
        } else if (left->getType() == NUMBER_NODE) {
            FieldSpecsAstNumberNode *node = dynamic_cast<FieldSpecsAstNumberNode*>(left);
            currVal = node->getValue();
        } else if (left->getType() == EXPR_NODE) {
            FieldSpecsAstExprNode *node = dynamic_cast<FieldSpecsAstExprNode*>(left);
            node->evaluate(symTab, currVal);
        } else {
            cerr << "[Error]: " << "Under construction!" << endl;
        }
        if (right == NULL) {
            if (op == NEG_OP) {
                currVal *= -1;
                return;
            } else if (op == NO_OP) {
                return;
            }
        }
        double rightVal = 0;
        if (right->getType() == ID_NODE) {
            FieldSpecsAstIdNode *node = dynamic_cast<FieldSpecsAstIdNode*>(right);
            rightVal = symTab.at(node->getIdName());
        } else if (right->getType() == NUMBER_NODE) {
            FieldSpecsAstNumberNode *node = dynamic_cast<FieldSpecsAstNumberNode*>(right);
            rightVal = node->getValue();
        } else if (right->getType() == EXPR_NODE) {
            FieldSpecsAstExprNode *node = dynamic_cast<FieldSpecsAstExprNode*>(right);
            node->evaluate(symTab, rightVal);
        } else {
            cerr << "[Error]: " << "Under construction!" << endl;
        }
        switch (op) {
            case ADD_OP:
                currVal += rightVal;
                break;
            case SUB_OP:
                currVal -= rightVal;
                break;
            case MUL_OP:
                currVal *= rightVal;
                break;
            case DIV_OP:
                currVal /= rightVal;
                break;
            default:
                cerr << "[Error]: " << "Unexpected operator!" << endl;
        }
    }
};

} // geodiag_cmor

#endif // __Geodiag_cmor_FieldSpecsAstNode__
