#ifndef P3_CODEGENERATOR_H
#define P3_CODEGENERATOR_H


#include "scope.h"
#include "node.h"

class CodeGenerator {
private:
    const Node *parseTree;
    std::vector<std::string> *variables = new std::vector<std::string>();
    std::vector<std::string> *segments = new std::vector<std::string>();
    Scope *currentScope = new Scope();
    int temp = 0;
    int temp2 = 0;
public:
    explicit CodeGenerator(const Node *parseTree);

    ~CodeGenerator();

    const std::string generateCode();

    const std::string descend(const Node *node);

    const std::string generateTempIdentifier();

    std::string allocateStorage();

    const std::string generateTempLabel();

    const std::string getTempLabel();

    std::string erectSegments();
};


#endif //P3_CODEGENERATOR_H
