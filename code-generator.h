#ifndef P3_CODEGENERATOR_H
#define P3_CODEGENERATOR_H

#include "scope.h"
#include "node.h"

class CodeGenerator {
private:
    const Node *parseTree;
    std::vector<std::string> *variables = new std::vector<std::string>();
    std::vector<std::string> *generatedSegments = new std::vector<std::string>();
    Scope *currentScope = new Scope();
    int quantityThrowAwayIdentifiers = 0;
    int quantityThrowAwayLabels = 0;

    const int INITIAL_VARIABLE_VALUE = 0;
    const std::string THROW_AWAY_IDENTIFIER_PREFIX = "I";
    const std::string THROW_AWAY_LABEL_PREFIX = "L";

    const std::string CMD_ADD = "ADD";
    const std::string CMD_BR = "BR";
    const std::string CMD_BRNEG = "BRNEG";
    const std::string CMD_BRZNEG = "BRZNEG";
    const std::string CMD_BRPOS = "BRPOS";
    const std::string CMD_BRZPOS = "BRZPOS";
    const std::string CMD_BRZERO = "BRZERO";
    const std::string CMD_COPY = "COPY";
    const std::string CMD_DIV = "DIV";
    const std::string CMD_MULT = "MULT";
    const std::string CMD_READ = "READ";
    const std::string CMD_WRITE = "WRITE";
    const std::string CMD_STOP = "STOP";
    const std::string CMD_STORE = "STORE";
    const std::string CMD_SUB = "SUB";
    const std::string CMD_NOOP = "NOOP";
    const std::string CMD_LOAD = "LOAD";
    const std::string CMD_PUSH = "PUSH";
    const std::string CMD_POP = "POP";
    const std::string CMD_STACKW = "STACKW";
    const std::string CMD_STACKR = "STACKR";

    const std::string SPACE = " ";
    const std::string COLON = ":";
    const std::string NEW_LINE = "\n";

    const std::string generateCodeForNode(const Node *node);

    const std::string generateSegmentCode() const;

    const std::string generateStorageAllocationCode() const;

    const std::string generateThrowAwayIdentifier();

    const std::string generateThrowAwayLabel();

public:
    explicit CodeGenerator(const Node *parseTree);

    ~CodeGenerator();

    const std::string generateCode();
};

#endif //P3_CODEGENERATOR_H
