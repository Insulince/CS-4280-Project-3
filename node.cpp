#include "node.h"

using namespace std;

Node::Node(const string value, const bool nonTerminal, const int level, const NONTERMINAL_IDENTIFIER nonTerminalIdentifier) : value(value),
                                                                                                                              nonTerminal(nonTerminal),
                                                                                                                              level(level),
                                                                                                                              nonTerminalIdentifier(nonTerminalIdentifier) {
}

Node::~Node() = default;

const string &Node::getValue() const {
    return value;
}

const bool Node::isNonTerminal() const {
    return nonTerminal;
}

const int Node::getLevel() const {
    return level;
}

const NONTERMINAL_IDENTIFIER Node::getNonTerminalIdentifier() const {
    return nonTerminalIdentifier;
}

const vector<Node *> &Node::getChildren() const {
    return children;
}

const void Node::setChildren(const vector<Node *> &children) {
    Node::children = children;
}

const string Node::toString() const {
    string output;

    // Defaulted to preorder traversal per the P2 Specification.
    output += toStringMe();
    output += toStringChildren();

    return output;
}

const string Node::toStringMe() const {
    const string TAB = "   ";
    string output;

    for (int i = 0; i < getLevel(); i++) {
        output += TAB + TAB;
    }

    if (nonTerminal) {
        output += "<" + value + "> " + to_string(nonTerminalIdentifier) + "\n";
    } else {
        output += value + " " + to_string(nonTerminalIdentifier) + "\n";
    }

    return output;
}

const string Node::toStringChildren() const {
    string output = "";

    for (const Node *node : getChildren()) {
        output += node->toString();
    }

    return output;
}
