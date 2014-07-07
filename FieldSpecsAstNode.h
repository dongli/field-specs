#ifndef __Geodiag_cmor_FieldSpecsAstNode__
#define __Geodiag_cmor_FieldSpecsAstNode__

#include "geodiag_cmor_commons.h"

namespace geodiag_cmor {

enum Spec {
    D_2, D_3, KEEP_VERT_COORD
};

enum Op {
    ADD, SUB, MUL, DIV, NEG, NO_OP
};

class FieldSpecsAstNode {
public:
    virtual ~FieldSpecsAstNode() {}

    virtual void print(std::ostream &os, int depth = 0) const = 0;

    static inline string indent(int depth) {
        return string(depth*2, ' ');
    }
};

class FieldSpecsAstIdNode : public FieldSpecsAstNode {
    string idName;
public:
    FieldSpecsAstIdNode(const string *idName) {
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
    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[ID List]: " << endl;
        for (int i = 0; i < idNodes.size(); ++i) {
            idNodes[i]->print(os, depth+1);
        }
    }

    void addIdNode(FieldSpecsAstIdNode *idNode) {
        idNodes.push_back(idNode);
    }
};

class FieldSpecsAstFuncNode : public FieldSpecsAstNode {
    string funcName;
    FieldSpecsAstIdListNode *argListNode;
public:
    FieldSpecsAstFuncNode(const string funcName,
                          FieldSpecsAstIdListNode *argListNode) {
        this->funcName = funcName;
        this->argListNode = argListNode;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Function]: " << funcName << endl;
        argListNode->print(os, depth+1);
    }
};

class FieldSpecsAstAddNode : public FieldSpecsAstNode {
    FieldSpecsAstNode *leftNode;
    FieldSpecsAstNode *rightNode;
public:
    virtual ~FieldSpecsAstAddNode() {
        delete leftNode;
        delete rightNode;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Add]:" << endl;
        leftNode->print(os, depth+1);
        rightNode->print(os, depth+1);
    }
};

class FieldSpecsAstSubNode : public FieldSpecsAstNode {
    FieldSpecsAstNode *leftNode;
    FieldSpecsAstNode *rightNode;
public:
    virtual ~FieldSpecsAstSubNode() {
        delete leftNode;
        delete rightNode;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Sub]:" << endl;
        leftNode->print(os, depth+1);
        rightNode->print(os, depth+1);
    }
};

class FieldSpecsAstMulNode : public FieldSpecsAstNode {
    FieldSpecsAstNode *leftNode;
    FieldSpecsAstNode *rightNode;
public:
    virtual ~FieldSpecsAstMulNode() {
        delete leftNode;
        delete rightNode;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Mul]:" << endl;
        leftNode->print(os, depth+1);
        rightNode->print(os, depth+1);
    }
};

class FieldSpecsAstDivNode : public FieldSpecsAstNode {
    FieldSpecsAstNode *leftNode;
    FieldSpecsAstNode *rightNode;
public:
    virtual ~FieldSpecsAstDivNode() {
        delete leftNode;
        delete rightNode;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Div]:" << endl;
        leftNode->print(os, depth+1);
        rightNode->print(os, depth+1);
    }
};

class FieldSpecsAstNegNode : public FieldSpecsAstNode {
    FieldSpecsAstNode *node;
public:
    FieldSpecsAstNegNode(FieldSpecsAstNode *node) {
        this->node = node;
    }
    virtual ~FieldSpecsAstNegNode() {
        delete node;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Neg]:" << endl;
        node->print(os, depth+1);
    }
};

class FieldSpecsAstSpecNode : public FieldSpecsAstNode {
    Spec spec;
public:
    FieldSpecsAstSpecNode(Spec spec) {
        this->spec = spec;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Spec]: ";
        switch (spec) {
            case D_2:
                os << "2d";
                break;
            case D_3:
                os << "3d";
                break;
            case KEEP_VERT_COORD:
                os << "keep_vert_coord";
                break;
        }
        os << endl;
    }
};

class FieldSpecsAstNumberNode : public FieldSpecsAstNode {
    double value;
public:
    FieldSpecsAstNumberNode(double value) {
        this->value = value;
    }

    virtual void print(std::ostream &os, int depth = 0) const {
        os << indent(depth) << "[Number]: " << value << endl;
    }
};

class FieldSpecsAstExprNode : public FieldSpecsAstNode {
    Op op;
    FieldSpecsAstNode *left;
    FieldSpecsAstNode *right;
public:
    FieldSpecsAstExprNode(FieldSpecsAstNode *left,
                          Op op = NO_OP,
                          FieldSpecsAstNode *right = NULL) {
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
            case ADD:
                os << "+" << endl;
                break;
            case SUB: case NEG:
                os << "-" << endl;
                break;
            case MUL:
                os << "*" << endl;
                break;
            case DIV:
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
};

} // geodiag_cmor

#endif // __Geodiag_cmor_FieldSpecsAstNode__
