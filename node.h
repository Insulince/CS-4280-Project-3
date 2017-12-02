#ifndef P0_NODE_H
#define P0_NODE_H

#include <iostream>
#include <string>
#include <vector>
#include "token.h"
#include "nonterminal.h"

const static int ROOT_LEVEL = 0;

class Node {
private:
    const std::string value;
    const bool nonTerminal;
    const int level;
    const NONTERMINAL_IDENTIFIER nonTerminalIdentifier;
    std::vector<Node *> children;
public:
    Node(const std::string value, const bool nonTerminal, const int level, const NONTERMINAL_IDENTIFIER nonterminalIdentifier);

    ~Node();

    const std::string &getValue() const;

    const bool isNonTerminal() const;

    const int getLevel() const;

    const NONTERMINAL_IDENTIFIER getNonTerminalIdentifier() const;

    const std::vector<Node *> &getChildren() const;

    const void setChildren(const std::vector<Node *> &children);

    const std::string toString() const;

    const std::string toStringMe() const;

    const std::string toStringChildren() const;
};

#endif //P0_NODE_H
