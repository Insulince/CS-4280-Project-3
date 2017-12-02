#include<string>
#include "code-generator.h"

using namespace std;

CodeGenerator::CodeGenerator(const Node *parseTree) : parseTree(parseTree) {
}

CodeGenerator::~CodeGenerator() = default;

const string CodeGenerator::generateCode() const {
    return descend(parseTree);
}

const string CodeGenerator::descend(const Node *node) const {
//    cout << "Descending on node with Non Terminal Identifier value \"" << to_string(node->getNonTerminalIdentifier()) << "\" and value \"" << node->getValue() << "\".\n";

    string output;

    if (node->isNonTerminal()) {
        if (node->getNonTerminalIdentifier() == PROGRAM) {
            cout << "NONTERMINAL: PROGRAM\n";
            if (node->getChildren().size() == 2) {
                output += descend(node->getChildren().at(0)); // <vars>
                output += descend(node->getChildren().at(1)); // <block>
            } else {
                cout << "--- Code Generation Error: Processing of \"PROGRAM\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == BLOCK) {
            cout << "NONTERMINAL: BLOCK\n";
            if (node->getChildren().size() == 4) {
                output += descend(node->getChildren().at(0)); // Begin
                output += descend(node->getChildren().at(1)); // <vars>
                output += descend(node->getChildren().at(2)); // <stats>
                output += descend(node->getChildren().at(3)); // End
            } else {
                cout << "--- Code Generation Error: Processing of \"BLOCK\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == VARS) {
            cout << "NONTERMINAL: VARS\n";
            if (node->getChildren().empty()) {
                // $empty
            } else if (node->getChildren().size() == 3 && node->getChildren().at(0)->getValue() == "Var") {
                output += descend(node->getChildren().at(0)); // Var
                output += descend(node->getChildren().at(1)); // $identifier
                output += descend(node->getChildren().at(2)); // <mvars>
            } else {
                cout << "--- Code Generation Error: Processing of \"VARS\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == MVARS) {
            cout << "NONTERMINAL: MVARS\n";
            if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == ".") {
                output += descend(node->getChildren().at(0)); // .
            } else if (node->getChildren().size() == 3 && node->getChildren().at(0)->getValue() == ",") {
                output += descend(node->getChildren().at(0)); // ,
                output += descend(node->getChildren().at(1)); // $identifier
                output += descend(node->getChildren().at(2)); // <mvars>
            } else {
                cout << "--- Code Generation Error: Processing of \"MVARS\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == EXPR) {
            cout << "NONTERMINAL: EXPR\n";
            if (node->getChildren().size() == 3 && node->getChildren().at(1)->getValue() == "+") {
                output += descend(node->getChildren().at(0)); // <M>
                output += descend(node->getChildren().at(1)); // +
                output += descend(node->getChildren().at(2)); // <expr>
            } else if (node->getChildren().size() == 3 && node->getChildren().at(1)->getValue() == "-") {
                output += descend(node->getChildren().at(0)); // <M>
                output += descend(node->getChildren().at(1)); // -
                output += descend(node->getChildren().at(2)); // <expr>
            } else if (node->getChildren().size() == 1) {
                output += descend(node->getChildren().at(0)); // <M>
            } else {
                cout << "--- Code Generation Error: Processing of \"EXPR\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == M) {
            cout << "NONTERMINAL: M\n";
            if (node->getChildren().size() == 3 && node->getChildren().at(1)->getValue() == "%") {
                output += descend(node->getChildren().at(0)); // <F>
                output += descend(node->getChildren().at(1)); // %
                output += descend(node->getChildren().at(2)); // <M>
            } else if (node->getChildren().size() == 3 && node->getChildren().at(1)->getValue() == "*") {
                output += descend(node->getChildren().at(0)); // <F>
                output += descend(node->getChildren().at(1)); // *
                output += descend(node->getChildren().at(2)); // <M>
            } else if (node->getChildren().size() == 1) {
                output += descend(node->getChildren().at(0)); // <F>
            } else {
                cout << "--- Code Generation Error: Processing of \"M\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == F) {
            cout << "NONTERMINAL: F\n";
            if (node->getChildren().size() == 3 && node->getChildren().at(0)->getValue() == "(") {
                output += descend(node->getChildren().at(0)); // (
                output += descend(node->getChildren().at(1)); // <F>
                output += descend(node->getChildren().at(2)); // )
            } else if (node->getChildren().size() == 1) {
                output += descend(node->getChildren().at(0)); // <R>
            } else {
                cout << "--- Code Generation Error: Processing of \"F\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == R) {
            cout << "NONTERMINAL: R\n";
            if (node->getChildren().size() == 3 && node->getChildren().at(0)->getValue() == "[") {
                output += descend(node->getChildren().at(0)); // [
                output += descend(node->getChildren().at(1)); // <expr>
                output += descend(node->getChildren().at(2)); // ]
            } else if (node->getChildren().size() == 1 && isalpha(node->getChildren().at(0)->getValue()[0])) {
                output += descend(node->getChildren().at(0)); // $identifier
            } else if (node->getChildren().size() == 1 && isdigit(node->getChildren().at(0)->getValue()[0])) {
                output += descend(node->getChildren().at(0)); // $number
            } else {
                cout << "--- Code Generation Error: Processing of \"R\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STATS) {
            cout << "NONTERMINAL: STATS\n";
            if (node->getChildren().size() == 2) {
                output += descend(node->getChildren().at(0)); // <stat>
                output += descend(node->getChildren().at(1)); // <mStat>
            } else {
                cout << "--- Code Generation Error: Processing of \"STATS\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == MSTAT) {
            cout << "NONTERMINAL: MSTAT\n";
            if (node->getChildren().empty()) {
                // $empty
            } else if (node->getChildren().size() == 2 && node->getChildren().at(0)->getValue() == "stat") {
                output += descend(node->getChildren().at(0)); // <stat>
                output += descend(node->getChildren().at(1)); // <mStat>
            } else {
                cout << "--- Code Generation Error: Processing of \"MSTAT\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STAT) {
            cout << "NONTERMINAL: STAT\n";
            if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "in") {
                output += descend(node->getChildren().at(0)); // <in>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "out") {
                output += descend(node->getChildren().at(0)); // <out>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "block") {
                output += descend(node->getChildren().at(0)); // <block>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "if") {
                output += descend(node->getChildren().at(0)); // <if>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "loop") {
                output += descend(node->getChildren().at(0)); // <loop>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "assign") {
                output += descend(node->getChildren().at(0)); // <assign>
            } else {
                cout << "--- Code Generation Error: Processing of \"STAT\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IN) {
            cout << "NONTERMINAL: IN\n";
            if (node->getChildren().size() == 3) {
//                output += descend(node->getChildren().at(0)); // Input
//                output += descend(node->getChildren().at(1)); // $identifier
//                output += descend(node->getChildren().at(2)); // ;

                output += "READ " + node->getChildren().at(1)->getValue();
            } else {
                cout << "--- Code Generation Error: Processing of \"IN\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == OUT) {
            cout << "NONTERMINAL: OUT\n";
            if (node->getChildren().size() == 3) {
                output += descend(node->getChildren().at(0)); // Output
                output += descend(node->getChildren().at(1)); // <expr>
                output += descend(node->getChildren().at(2)); // ;
            } else {
                cout << "--- Code Generation Error: Processing of \"OUT\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IF) {
            cout << "NONTERMINAL: IF\n";
            if (node->getChildren().size() == 7) {
                output += descend(node->getChildren().at(0)); // Check
                output += descend(node->getChildren().at(1)); // [
                output += descend(node->getChildren().at(2)); // <expr>
                output += descend(node->getChildren().at(3)); // <RO>
                output += descend(node->getChildren().at(4)); // <expr>
                output += descend(node->getChildren().at(5)); // ]
                output += descend(node->getChildren().at(6)); // <stat>
            } else {
                cout << "--- Code Generation Error: Processing of \"IF\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == LOOP) {
            cout << "NONTERMINAL: LOOP\n";
            if (node->getChildren().size() == 7) {
                output += descend(node->getChildren().at(0)); // Loop
                output += descend(node->getChildren().at(1)); // [
                output += descend(node->getChildren().at(2)); // <expr>
                output += descend(node->getChildren().at(3)); // <RO>
                output += descend(node->getChildren().at(4)); // <expr>
                output += descend(node->getChildren().at(5)); // ]
                output += descend(node->getChildren().at(6)); // <stat>
            } else {
                cout << "--- Code Generation Error: Processing of \"LOOP\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == ASSIGN) {
            cout << "NONTERMINAL: ASSIGN\n";
            if (node->getChildren().size() == 4) {
//                output += descend(node->getChildren().at(0)); // $identifier
//                output += descend(node->getChildren().at(1)); // :
                output += descend(node->getChildren().at(2)); // <expr>
//                output += descend(node->getChildren().at(3)); // ;
//
                output += "STORE " + node->getChildren().at(0)->getValue();
            } else {
                cout << "--- Code Generation Error: Processing of \"ASSIGN\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == RO) {
            cout << "NONTERMINAL: RO\n";
            if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "<") {
                output += descend(node->getChildren().at(0)); // <
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "<=") {
                output += descend(node->getChildren().at(0)); // <=
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == ">") {
                output += descend(node->getChildren().at(0)); // >
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == ">=") {
                output += descend(node->getChildren().at(0)); // >=
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "==") {
                output += descend(node->getChildren().at(0)); // ==
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "!=") {
                output += descend(node->getChildren().at(0)); // !=
            } else {
                cout << "--- Code Generation Error: Processing of \"RO\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == TERMINAL) {
            cout << "NONTERMINAL: TERMINAL\n";
            // This should not be possible to reach, but is included for consistency anyway.
            cout << "--- Code Generation Error: Processing of non-terminal node is marked as terminal.\n";
        } else {
            cout << "--- Code Generation Error: Processing of non-terminal node is unrecognized. Marked as value \"" + to_string(node->getNonTerminalIdentifier()) + "\"\n";
        }
    } else {
        cout << "TERMINAL:    \"" + node->getValue() + "\"\n";
    }

    return output;
}