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
            cout << "PROCESSING NONTERMINAL: PROGRAM\n";
            if (node->getChildren().size() == 2) {
                // Child 0: <vars>
                // Child 1: <block>
            } else {
                cout << "--- Code Generation Error: Processing of \"PROGRAM\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == BLOCK) {
            cout << "PROCESSING NONTERMINAL: BLOCK\n";
            if (node->getChildren().size() == 4) {
                // Child 0: Begin
                // Child 1: <vars>
                // Child 2: <stats>
                // Child 3: End
            } else {
                cout << "--- Code Generation Error: Processing of \"BLOCK\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == VARS) {
            cout << "PROCESSING NONTERMINAL: VARS\n";
            if (node->getChildren().empty()) {
                // $empty
            } else if (node->getChildren().size() == 3 && node->getChildren().at(0)->getValue() == "Var") {
                // Child 0: Var
                // Child 1: $identifier
                // Child 2: <mvars>
            } else {
                cout << "--- Code Generation Error: Processing of \"VARS\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == MVARS) {
            cout << "PROCESSING NONTERMINAL: MVARS\n";
            if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == ".") {
                // Child 0: .
            } else if (node->getChildren().size() == 3 && node->getChildren().at(0)->getValue() == ",") {
                // Child 0: ,
                // Child 1: $identifier
                // Child 2: <mvars>
            } else {
                cout << "--- Code Generation Error: Processing of \"MVARS\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == EXPR) {
            cout << "PROCESSING NONTERMINAL: EXPR\n";
            if (node->getChildren().size() == 3 && node->getChildren().at(1)->getValue() == "+") {
                // Child 0: <M>
                // Child 1: +
                // Child 2: <expr>
            } else if (node->getChildren().size() == 3 && node->getChildren().at(1)->getValue() == "-") {
                // Child 0: <M>
                // Child 1: -
                // Child 2: <expr>
            } else if (node->getChildren().size() == 1) {
                // Child 0: <M>
            } else {
                cout << "--- Code Generation Error: Processing of \"EXPR\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == M) {
            cout << "PROCESSING NONTERMINAL: M\n";
            if (node->getChildren().size() == 3 && node->getChildren().at(1)->getValue() == "%") {
                // Child 0: <F>
                // Child 1: %
                // Child 2: <M>
            } else if (node->getChildren().size() == 3 && node->getChildren().at(1)->getValue() == "*") {
                // Child 0: <F>
                // Child 1: *
                // Child 2: <M>
            } else if (node->getChildren().size() == 1) {
                // Child 0: <F>
            } else {
                cout << "--- Code Generation Error: Processing of \"M\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == F) {
            cout << "PROCESSING NONTERMINAL: F\n";
            if (node->getChildren().size() == 3 && node->getChildren().at(0)->getValue() == "(") {
                // Child 0: (
                // Child 1: <F>
                // Child 2: )
            } else if (node->getChildren().size() == 1) {
                // Child 0: <R>
            } else {
                cout << "--- Code Generation Error: Processing of \"F\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == R) {
            cout << "PROCESSING NONTERMINAL: R\n";
            if (node->getChildren().size() == 3 && node->getChildren().at(0)->getValue() == "[") {
                // Child 0: [
                // Child 1: <expr>
                // Child 2: ]

                output += descend(node->getChildren().at(1)) + "\n";
            } else if (node->getChildren().size() == 1 && isalpha(node->getChildren().at(0)->getValue()[0])) {
                // Child 0: $identifier

                output += "LOAD " + node->getChildren().at(0)->getValue() + "\n";
            } else if (node->getChildren().size() == 1 && isdigit(node->getChildren().at(0)->getValue()[0])) {
                // Child 0: $number

                output += "LOAD " + node->getChildren().at(0)->getValue() + "\n";
            } else {
                cout << "--- Code Generation Error: Processing of \"R\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STATS) {
            cout << "PROCESSING NONTERMINAL: STATS\n";
            if (node->getChildren().size() == 2) {
                // Child 0: <stat>
                // Child 1: <mStat>
            } else {
                cout << "--- Code Generation Error: Processing of \"STATS\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == MSTAT) {
            cout << "PROCESSING NONTERMINAL: MSTAT\n";
            if (node->getChildren().empty()) {
                // $empty
            } else if (node->getChildren().size() == 2 && node->getChildren().at(0)->getValue() == "stat") {
                // Child 0: <stat>
                // Child 1: <mStat>
            } else {
                cout << "--- Code Generation Error: Processing of \"MSTAT\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STAT) {
            cout << "PROCESSING NONTERMINAL: STAT\n";
            if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "in") {
                // Child 0: <in>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "out") {
                // Child 0: <out>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "block") {
                // Child 0: <block>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "if") {
                // Child 0: <if>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "loop") {
                // Child 0: <loop>
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "assign") {
                // Child 0: <assign>
            } else {
                cout << "--- Code Generation Error: Processing of \"STAT\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IN) {
            cout << "PROCESSING NONTERMINAL: IN\n";
            if (node->getChildren().size() == 3) {
                // Child 0: Input
                // Child 1: $identifier
                // Child 2: ;

                output += "READ " + node->getChildren().at(1)->getValue() + "\n";
            } else {
                cout << "--- Code Generation Error: Processing of \"IN\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == OUT) {
            cout << "PROCESSING NONTERMINAL: OUT\n";
            if (node->getChildren().size() == 3) {
                // Child 0: Output
                // Child 1: <expr>
                // Child 2: ;
            } else {
                cout << "--- Code Generation Error: Processing of \"OUT\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IF) {
            cout << "PROCESSING NONTERMINAL: IF\n";
            if (node->getChildren().size() == 7) {
                // Child 0: Check
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: <stat>
            } else {
                cout << "--- Code Generation Error: Processing of \"IF\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == LOOP) {
            cout << "PROCESSING NONTERMINAL: LOOP\n";
            if (node->getChildren().size() == 7) {
                // Child 0: Loop
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: <stat>
            } else {
                cout << "--- Code Generation Error: Processing of \"LOOP\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == ASSIGN) {
            cout << "PROCESSING NONTERMINAL: ASSIGN\n";
            if (node->getChildren().size() == 4) {
                // Child 0: $identifier
                // Child 1: :
                // Child 2: <expr>
                // Child 3: ;

                output += descend(node->getChildren().at(2)) + "\n";
                output += "STORE " + node->getChildren().at(0)->getValue() + "\n";
            } else {
                cout << "--- Code Generation Error: Processing of \"ASSIGN\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == RO) {
            cout << "PROCESSING NONTERMINAL: RO\n";
            if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "<") {
                // Child 0: <
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "<=") {
                // Child 0: <=
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == ">") {
                // Child 0: >
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == ">=") {
                // Child 0: >=
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "==") {
                // Child 0: ==
            } else if (node->getChildren().size() == 1 && node->getChildren().at(0)->getValue() == "!=") {
                // Child 0: !=
            } else {
                cout << "--- Code Generation Error: Processing of \"RO\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == TERMINAL) {
            cout << "PROCESSING NONTERMINAL: TERMINAL\n";
            // This should not be possible to reach, but is included for consistency anyway.
            cout << "--- Code Generation Error: Processing of non-terminal node is marked as terminal.\n";
        } else {
            cout << "--- Code Generation Error: Processing of non-terminal node is unrecognized. Marked as value \"" + to_string(node->getNonTerminalIdentifier()) + "\"\n";
        }
    } else {
        cout << "PROCESSING TERMINAL:    \"" + node->getValue() + "\"\n";
    }

    return output;
}