#include<string>
#include "code-generator.h"

using namespace std;

CodeGenerator::CodeGenerator(const Node *parseTree) : parseTree(parseTree) {
}

CodeGenerator::~CodeGenerator() = default;

const string CodeGenerator::generateCode() const {
    return generateCodeOnSubTree(parseTree);
}

const string CodeGenerator::generateCodeOnSubTree(const Node *subTree) const {
    string generatedCode;

    if (subTree->isNonTerminal()) {
        const vector<Node *> &children = subTree->getChildren();

        if (subTree->getNonTerminalIdentifier() == PROGRAM) {
            cout << "PROCESSING NONTERMINAL: PROGRAM\n";
            if (children.size() == 2) {
                // Child 0: <vars>
                // Child 1: <block>
            } else {
                cerr << "Code Generation Error: Processing of \"PROGRAM\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == BLOCK) {
            cout << "PROCESSING NONTERMINAL: BLOCK\n";
            if (children.size() == 4) {
                // Child 0: Begin
                // Child 1: <vars>
                // Child 2: <stats>
                // Child 3: End
            } else {
                cerr << "Code Generation Error: Processing of \"BLOCK\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == VARS) {
            cout << "PROCESSING NONTERMINAL: VARS\n";
            if (children.empty()) {
                // $empty
            } else if (children.size() == 3 && children.at(0)->getValue() == "Var") {
                // Child 0: Var
                // Child 1: $identifier
                // Child 2: <mvars>
            } else {
                cerr << "Code Generation Error: Processing of \"VARS\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == MVARS) {
            cout << "PROCESSING NONTERMINAL: MVARS\n";
            if (children.size() == 1 && children.at(0)->getValue() == ".") {
                // Child 0: .
            } else if (children.size() == 3 && children.at(0)->getValue() == ",") {
                // Child 0: ,
                // Child 1: $identifier
                // Child 2: <mvars>
            } else {
                cerr << "Code Generation Error: Processing of \"MVARS\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == EXPR) {
            cout << "PROCESSING NONTERMINAL: EXPR\n";
            if (children.size() == 3 && children.at(1)->getValue() == "+") {
                // Child 0: <M>
                // Child 1: +
                // Child 2: <expr>
            } else if (children.size() == 3 && children.at(1)->getValue() == "-") {
                // Child 0: <M>
                // Child 1: -
                // Child 2: <expr>
            } else if (children.size() == 1) {
                // Child 0: <M>

                generatedCode += generateCodeOnSubTree(children.at(0));
            } else {
                cerr << "Code Generation Error: Processing of \"EXPR\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == M) {
            cout << "PROCESSING NONTERMINAL: M\n";
            if (children.size() == 3 && children.at(1)->getValue() == "%") {
                // Child 0: <F>
                // Child 1: %
                // Child 2: <M>
            } else if (children.size() == 3 && children.at(1)->getValue() == "*") {
                // Child 0: <F>
                // Child 1: *
                // Child 2: <M>
            } else if (children.size() == 1) {
                // Child 0: <F>
            } else {
                cerr << "Code Generation Error: Processing of \"M\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == F) {
            cout << "PROCESSING NONTERMINAL: F\n";
            if (children.size() == 3 && children.at(0)->getValue() == "(") {
                // Child 0: (
                // Child 1: <F>
                // Child 2: )
            } else if (children.size() == 1) {
                // Child 0: <R>
            } else {
                cerr << "Code Generation Error: Processing of \"F\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == R) {
            cout << "PROCESSING NONTERMINAL: R\n";
            if (children.size() == 3 && children.at(0)->getValue() == "[") {
                // Child 0: [
                // Child 1: <expr>
                // Child 2: ]

                generatedCode += generateCodeOnSubTree(children.at(1));
            } else if (children.size() == 1 && isalpha(children.at(0)->getValue()[0])) {
                // Child 0: $identifier

                generatedCode += "LOAD " + children.at(0)->getValue() + "\n";
            } else if (children.size() == 1 && isdigit(children.at(0)->getValue()[0])) {
                // Child 0: $number

                generatedCode += "LOAD " + children.at(0)->getValue() + "\n";
            } else {
                cerr << "Code Generation Error: Processing of \"R\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == STATS) {
            cout << "PROCESSING NONTERMINAL: STATS\n";
            if (children.size() == 2) {
                // Child 0: <stat>
                // Child 1: <mStat>
            } else {
                cerr << "Code Generation Error: Processing of \"STATS\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == MSTAT) {
            cout << "PROCESSING NONTERMINAL: MSTAT\n";
            if (children.empty()) {
                // $empty
            } else if (children.size() == 2 && children.at(0)->getValue() == "stat") {
                // Child 0: <stat>
                // Child 1: <mStat>
            } else {
                cerr << "Code Generation Error: Processing of \"MSTAT\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == STAT) {
            cout << "PROCESSING NONTERMINAL: STAT\n";
            if (children.size() == 1 && children.at(0)->getValue() == "in") {
                // Child 0: <in>
            } else if (children.size() == 1 && children.at(0)->getValue() == "out") {
                // Child 0: <out>
            } else if (children.size() == 1 && children.at(0)->getValue() == "block") {
                // Child 0: <block>
            } else if (children.size() == 1 && children.at(0)->getValue() == "if") {
                // Child 0: <if>
            } else if (children.size() == 1 && children.at(0)->getValue() == "loop") {
                // Child 0: <loop>
            } else if (children.size() == 1 && children.at(0)->getValue() == "assign") {
                // Child 0: <assign>
            } else {
                cerr << "Code Generation Error: Processing of \"STAT\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == IN) {
            cout << "PROCESSING NONTERMINAL: IN\n";
            if (children.size() == 3) {
                // Child 0: Input
                // Child 1: $identifier
                // Child 2: ;

                generatedCode += "READ " + children.at(1)->getValue() + "\n";
            } else {
                cerr << "Code Generation Error: Processing of \"IN\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == OUT) {
            cout << "PROCESSING NONTERMINAL: OUT\n";
            if (children.size() == 3) {
                // Child 0: Output
                // Child 1: <expr>
                // Child 2: ;
            } else {
                cerr << "Code Generation Error: Processing of \"OUT\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == IF) {
            cout << "PROCESSING NONTERMINAL: IF\n";
            if (children.size() == 7) {
                // Child 0: Check
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: <stat>
            } else {
                cerr << "Code Generation Error: Processing of \"IF\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == LOOP) {
            cout << "PROCESSING NONTERMINAL: LOOP\n";
            if (children.size() == 7) {
                // Child 0: Loop
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: <stat>
            } else {
                cerr << "Code Generation Error: Processing of \"LOOP\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == ASSIGN) {
            cout << "PROCESSING NONTERMINAL: ASSIGN\n";
            if (children.size() == 4) {
                // Child 0: $identifier
                // Child 1: :
                // Child 2: <expr>
                // Child 3: ;

                generatedCode += generateCodeOnSubTree(children.at(2));
                generatedCode += "STORE " + children.at(0)->getValue() + "\n";
            } else {
                cerr << "Code Generation Error: Processing of \"ASSIGN\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == RO) {
            cout << "PROCESSING NONTERMINAL: RO\n";
            if (children.size() == 1 && children.at(0)->getValue() == "<") {
                // Child 0: <
            } else if (children.size() == 1 && children.at(0)->getValue() == "<=") {
                // Child 0: <=
            } else if (children.size() == 1 && children.at(0)->getValue() == ">") {
                // Child 0: >
            } else if (children.size() == 1 && children.at(0)->getValue() == ">=") {
                // Child 0: >=
            } else if (children.size() == 1 && children.at(0)->getValue() == "==") {
                // Child 0: ==
            } else if (children.size() == 1 && children.at(0)->getValue() == "!=") {
                // Child 0: !=
            } else {
                cerr << "Code Generation Error: Processing of \"RO\" node is unrecognized.\n";
            }
        } else if (subTree->getNonTerminalIdentifier() == TERMINAL) {
            cout << "PROCESSING NONTERMINAL: TERMINAL\n";
            // This should not be possible to reach, but is included for consistency anyway.
            cerr << "Code Generation Error: Processing of non-terminal node is marked as terminal.\n";
        } else {
            cerr << "Code Generation Error: Processing of non-terminal node is unrecognized. Marked as value \"" + to_string(subTree->getNonTerminalIdentifier()) + "\"\n";
        }
    } else {
        cout << "PROCESSING TERMINAL:    \"" + subTree->getValue() + "\"\n";
    }

    return generatedCode;
}