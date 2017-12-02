#ifndef P3_CODEGENERATOR_H
#define P3_CODEGENERATOR_H


#include "scope.h"
#include "node.h"

class CodeGenerator {
private:
    const Node *parseTree;

    Scope *currentScope = new Scope();
public:
    explicit CodeGenerator(const Node *parseTree);

    ~CodeGenerator();

    const std::string generateCode() const;

    const std::string generateCodeOnSubTree(const Node *subTree) const;
};


#endif //P3_CODEGENERATOR_H
