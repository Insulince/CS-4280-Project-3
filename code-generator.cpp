#include<string>
#include "code-generator.h"

using namespace std;

CodeGenerator::CodeGenerator(const Node *parseTree) : parseTree(parseTree) {
}

CodeGenerator::~CodeGenerator() = default;

const string CodeGenerator::generateCode() {
    string output = descend(parseTree);
    output += "STOP\n";
    output += erectSegments();
    output += "STOP\n";
    output += allocateStorage();

    return output;
}

const string CodeGenerator::descend(const Node *node) {
    string output;

    if (node->isNonTerminal()) {
        const vector<Node *> &children = node->getChildren();

        if (node->getNonTerminalIdentifier() == PROGRAM) {
            cout << "PROCESSING NONTERMINAL: PROGRAM\n";
            if (children.size() == 2) {
                // Child 0: <vars>
                // Child 1: <block>

                output += descend(children.at(0));
                output += descend(children.at(1));
            } else {
                cerr << "Code Generation Error: Processing of \"PROGRAM\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == BLOCK) {
            cout << "PROCESSING NONTERMINAL: BLOCK\n";
            if (children.size() == 4) {
                // Child 0: Begin
                // Child 1: <vars>
                // Child 2: <stats>
                // Child 3: End

                output += descend(children.at(1));
                output += descend(children.at(2));
            } else {
                cerr << "Code Generation Error: Processing of \"BLOCK\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == VARS) {
            cout << "PROCESSING NONTERMINAL: VARS\n";
            if (children.empty()) {
                // $empty
            } else if (children.size() == 3 && children.at(0)->getValue() == "Var") {
                // Child 0: Var
                // Child 1: $identifier
                // Child 2: <mvars>

                variables->push_back(children.at(1)->getValue());
                output += descend(children.at(2));
            } else {
                cerr << "Code Generation Error: Processing of \"VARS\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == MVARS) {
            cout << "PROCESSING NONTERMINAL: MVARS\n";
            if (children.size() == 1 && children.at(0)->getValue() == ".") {
                // Child 0: .
            } else if (children.size() == 3 && children.at(0)->getValue() == ",") {
                // Child 0: ,
                // Child 1: $identifier
                // Child 2: <mvars>

                variables->push_back(children.at(1)->getValue());
                output += descend(children.at(2));
            } else {
                cerr << "Code Generation Error: Processing of \"MVARS\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == EXPR) {
            cout << "PROCESSING NONTERMINAL: EXPR\n";
            if (children.size() == 3 && children.at(1)->getValue() == "+") {
                // Child 0: <M>
                // Child 1: +
                // Child 2: <expr>

                output += descend(children.at(0));
                const string tempIdentifier = generateTempIdentifier();
                output += "STORE " + tempIdentifier + "\n";
                output += descend(children.at(2));
                output += "ADD " + tempIdentifier + "\n";
            } else if (children.size() == 3 && children.at(1)->getValue() == "-") {
                // Child 0: <M>
                // Child 1: -
                // Child 2: <expr>

                output += descend(children.at(2));
                const string tempIdentifier = generateTempIdentifier();
                output += "STORE " + tempIdentifier + "\n";
                output += descend(children.at(0));
                output += "SUB " + tempIdentifier + "\n";
            } else if (children.size() == 1) {
                // Child 0: <M>

                output += descend(children.at(0));
            } else {
                cerr << "Code Generation Error: Processing of \"EXPR\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == M) {
            cout << "PROCESSING NONTERMINAL: M\n";
            if (children.size() == 3 && children.at(1)->getValue() == "%") {
                // Child 0: <F>
                // Child 1: %
                // Child 2: <M>

                output += descend(children.at(2));
                const string tempIdentifier = generateTempIdentifier();
                output += "STORE " + tempIdentifier + "\n";
                output += descend(children.at(0));
                output += "DIV " + tempIdentifier + "\n";
            } else if (children.size() == 3 && children.at(1)->getValue() == "*") {
                // Child 0: <F>
                // Child 1: *
                // Child 2: <M>

                output += descend(children.at(0));
                const string tempIdentifier = generateTempIdentifier();
                output += "STORE " + tempIdentifier + "\n";
                output += descend(children.at(2));
                output += "MULT " + tempIdentifier + "\n";
            } else if (children.size() == 1) {
                // Child 0: <F>

                output += descend(children.at(0));
            } else {
                cerr << "Code Generation Error: Processing of \"M\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == F) {
            cout << "PROCESSING NONTERMINAL: F\n";
            if (children.size() == 3 && children.at(0)->getValue() == "(") {
                // Child 0: (
                // Child 1: <F>
                // Child 2: )

                output += descend(children.at(1));
                output += "MULT -1\n";
            } else if (children.size() == 1) {
                // Child 0: <R>

                output += descend(children.at(0));
            } else {
                cerr << "Code Generation Error: Processing of \"F\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == R) {
            cout << "PROCESSING NONTERMINAL: R\n";
            if (children.size() == 3 && children.at(0)->getValue() == "[") {
                // Child 0: [
                // Child 1: <expr>
                // Child 2: ]

                output += descend(children.at(1));
            } else if (children.size() == 1 && isalpha(children.at(0)->getValue()[0])) {
                // Child 0: $identifier

                output += "LOAD " + children.at(0)->getValue() + "\n";
            } else if (children.size() == 1 && isdigit(children.at(0)->getValue()[0])) {
                // Child 0: $number

                output += "LOAD " + children.at(0)->getValue() + "\n";
            } else {
                cerr << "Code Generation Error: Processing of \"R\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STATS) {
            cout << "PROCESSING NONTERMINAL: STATS\n";
            if (children.size() == 2) {
                // Child 0: <stat>
                // Child 1: <mStat>

                output += descend(children.at(0));
                output += descend(children.at(1));
            } else {
                cerr << "Code Generation Error: Processing of \"STATS\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == MSTAT) {
            cout << "PROCESSING NONTERMINAL: MSTAT\n";
            if (children.empty()) {
                // $empty
            } else if (children.size() == 2 && children.at(0)->getValue() == "stat") {
                // Child 0: <stat>
                // Child 1: <mStat>

                output += descend(children.at(0));
                output += descend(children.at(1));
            } else {
                cerr << "Code Generation Error: Processing of \"MSTAT\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STAT) {
            cout << "PROCESSING NONTERMINAL: STAT\n";
            if (children.size() == 1 && children.at(0)->getValue() == "in") {
                // Child 0: <in>

                output += descend(children.at(0));
            } else if (children.size() == 1 && children.at(0)->getValue() == "out") {
                // Child 0: <out>

                output += descend(children.at(0));
            } else if (children.size() == 1 && children.at(0)->getValue() == "block") {
                // Child 0: <block>

                output += descend(children.at(0));
            } else if (children.size() == 1 && children.at(0)->getValue() == "if") {
                // Child 0: <if>

                output += descend(children.at(0));
            } else if (children.size() == 1 && children.at(0)->getValue() == "loop") {
                // Child 0: <loop>

                output += descend(children.at(0));
            } else if (children.size() == 1 && children.at(0)->getValue() == "assign") {
                // Child 0: <assign>

                output += descend(children.at(0));
            } else {
                cerr << "Code Generation Error: Processing of \"STAT\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IN) {
            cout << "PROCESSING NONTERMINAL: IN\n";
            if (children.size() == 3) {
                // Child 0: Input
                // Child 1: $identifier
                // Child 2: ;

                output += "READ " + children.at(1)->getValue() + "\n";
            } else {
                cerr << "Code Generation Error: Processing of \"IN\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == OUT) {
            cout << "PROCESSING NONTERMINAL: OUT\n";
            if (children.size() == 3) {
                // Child 0: Output
                // Child 1: <expr>
                // Child 2: ;

                output += descend(children.at(1));
                const string tempIdentifier = generateTempIdentifier();
                output += "STORE " + tempIdentifier + "\n";
                output += "WRITE " + tempIdentifier + "\n";
            } else {
                cerr << "Code Generation Error: Processing of \"OUT\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IF) {
            cout << "PROCESSING NONTERMINAL: IF\n";
            if (children.size() == 7) {
                // Child 0: Check
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: <stat>

                output += descend(children.at(4));
                const string tempIdentifier = generateTempIdentifier();
                output += "STORE " + tempIdentifier + "\n";
                output += descend(children.at(2));
                output += "SUB " + tempIdentifier + "\n";

                const string toLabel = generateTempLabel();
                output += "BR " + toLabel + "\n";
                const string fromLabel = generateTempLabel();
                const string segment = toLabel + ": NOOP\n"
                                       + descend(children.at(3)) + " " + fromLabel + "\n"
                                       + descend(children.at(6))
                                       + "BR " + fromLabel + "\n";
                segments->push_back(segment);
                output += fromLabel + ": NOOP\n";
            } else {
                cerr << "Code Generation Error: Processing of \"IF\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == LOOP) {
            cout << "PROCESSING NONTERMINAL: LOOP\n";
            if (children.size() == 7) {
                // Child 0: Loop
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: <stat>

                // TODO
            } else {
                cerr << "Code Generation Error: Processing of \"LOOP\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == ASSIGN) {
            cout << "PROCESSING NONTERMINAL: ASSIGN\n";
            if (children.size() == 4) {
                // Child 0: $identifier
                // Child 1: :
                // Child 2: <expr>
                // Child 3: ;

                output += descend(children.at(2));
                output += "STORE " + children.at(0)->getValue() + "\n";
            } else {
                cerr << "Code Generation Error: Processing of \"ASSIGN\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == RO) {
            cout << "PROCESSING NONTERMINAL: RO\n";
            if (children.size() == 1 && children.at(0)->getValue() == "<") {
                // Child 0: <

                output += "BRZPOS";
            } else if (children.size() == 1 && children.at(0)->getValue() == "<=") {
                // Child 0: <=

                output += "BRPOS";
            } else if (children.size() == 1 && children.at(0)->getValue() == ">") {
                // Child 0: >

                output += "BRZNEG";
            } else if (children.size() == 1 && children.at(0)->getValue() == ">=") {
                // Child 0: >=

                output += "BRNEG";
            } else if (children.size() == 1 && children.at(0)->getValue() == "==") {
                // Child 0: ==

                const string temp = generateTempIdentifier();
                output += "STORE " + temp + "\n";
                output += "MULT " + temp + "\n";
                output += "BRPOS";
            } else if (children.size() == 1 && children.at(0)->getValue() == "!=") {
                // Child 0: !=

                output += "BRZERO";
            } else {
                cerr << "Code Generation Error: Processing of \"RO\" node is unrecognized.\n";
            }
        } else if (node->getNonTerminalIdentifier() == TERMINAL) {
            cout << "PROCESSING NONTERMINAL: TERMINAL\n";
            // This should not be possible to reach, but is included for consistency anyway.
            cerr << "Code Generation Error: Processing of non-terminal node is marked as terminal.\n";
        } else {
            cerr << "Code Generation Error: Processing of non-terminal node is unrecognized. Marked as value \"" + to_string(node->getNonTerminalIdentifier()) + "\"\n";
        }
    } else {
        cout << "PROCESSING TERMINAL:    \"" + node->getValue() + "\"\n";
    }

    return output;
}

const string CodeGenerator::generateTempIdentifier() {
    return "T" + to_string(++temp);
}

const string CodeGenerator::generateTempLabel() {
    return "L" + to_string(++temp2);
}

std::string CodeGenerator::allocateStorage() {
    string output;

    for (const string &variable : *variables) {
        output += variable + " 0\n";
    }

    for (int i = 0; i < temp + 1; i++) {
        output += "T" + to_string(i) + " 0\n";
    }

    return output;
}

std::string CodeGenerator::erectSegments() {
    string output;

    for (const string &segment : *segments) {
        output += segment;
    }

    return output;
}
