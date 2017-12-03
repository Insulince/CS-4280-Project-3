#include<string>
#include "code-generator.h"

using namespace std;

CodeGenerator::CodeGenerator(const Node *parseTree) : parseTree(parseTree) {
}

CodeGenerator::~CodeGenerator() = default;

const string CodeGenerator::generateCode() {
    string generatedCode = generateCodeForNode(parseTree);
    generatedCode += CMD_STOP + NEW_LINE;
    generatedCode += generateSegmentCode();
    generatedCode += generateStorageAllocationCode();

    return generatedCode;
}

const string CodeGenerator::generateCodeForNode(const Node *node) {
    string generatedCode;

    if (node->isNonTerminal()) {
        const vector<Node *> &childNodes = node->getChildren();

        if (node->getNonTerminalIdentifier() == PROGRAM) {
            if (childNodes.size() == 2) {
                // Child 0: <vars>
                // Child 1: <block>

                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += generateCodeForNode(childNodes.at(1));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"PROGRAM\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == BLOCK) {
            if (childNodes.size() == 4) {
                // Child 0: Begin
                // Child 1: <vars>
                // Child 2: <stats>
                // Child 3: End

                generatedCode += generateCodeForNode(childNodes.at(1));
                generatedCode += generateCodeForNode(childNodes.at(2));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"BLOCK\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == VARS) {
            if (childNodes.empty()) {
                // $empty
            } else if (childNodes.size() == 3 && childNodes.at(0)->getValue() == "Var") {
                // Child 0: Var
                // Child 1: $identifier
                // Child 2: <mvars>

                variables->push_back(childNodes.at(1)->getValue());
                generatedCode += generateCodeForNode(childNodes.at(2));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"VARS\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == MVARS) {
            if (childNodes.size() == 1 && childNodes.at(0)->getValue() == ".") {
                // Child 0: .
            } else if (childNodes.size() == 3 && childNodes.at(0)->getValue() == ",") {
                // Child 0: ,
                // Child 1: $identifier
                // Child 2: <mvars>

                variables->push_back(childNodes.at(1)->getValue());
                generatedCode += generateCodeForNode(childNodes.at(2));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"MVARS\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == EXPR) {
            if (childNodes.size() == 3 && childNodes.at(1)->getValue() == "+") {
                // Child 0: <M>
                // Child 1: +
                // Child 2: <expr>

                generatedCode += generateCodeForNode(childNodes.at(0));
                const string tempIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + tempIdentifier + NEW_LINE;
                generatedCode += generateCodeForNode(childNodes.at(2));
                generatedCode += CMD_ADD + SPACE + tempIdentifier + NEW_LINE;
            } else if (childNodes.size() == 3 && childNodes.at(1)->getValue() == "-") {
                // Child 0: <M>
                // Child 1: -
                // Child 2: <expr>

                generatedCode += generateCodeForNode(childNodes.at(2));
                const string tempIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + tempIdentifier + NEW_LINE;
                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += CMD_SUB + SPACE + tempIdentifier + NEW_LINE;
            } else if (childNodes.size() == 1) {
                // Child 0: <M>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"EXPR\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == M) {
            if (childNodes.size() == 3 && childNodes.at(1)->getValue() == "%") {
                // Child 0: <F>
                // Child 1: %
                // Child 2: <M>

                generatedCode += generateCodeForNode(childNodes.at(2));
                const string tempIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + tempIdentifier + NEW_LINE;
                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += CMD_DIV + SPACE + tempIdentifier + NEW_LINE;
            } else if (childNodes.size() == 3 && childNodes.at(1)->getValue() == "*") {
                // Child 0: <F>
                // Child 1: *
                // Child 2: <M>

                generatedCode += generateCodeForNode(childNodes.at(0));
                const string tempIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + tempIdentifier + NEW_LINE;
                generatedCode += generateCodeForNode(childNodes.at(2));
                generatedCode += CMD_MULT + SPACE + tempIdentifier + NEW_LINE;
            } else if (childNodes.size() == 1) {
                // Child 0: <F>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"M\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == F) {
            if (childNodes.size() == 3 && childNodes.at(0)->getValue() == "(") {
                // Child 0: (
                // Child 1: <F>
                // Child 2: )

                generatedCode += generateCodeForNode(childNodes.at(1));
                generatedCode += CMD_MULT + " -1\n";
            } else if (childNodes.size() == 1) {
                // Child 0: <R>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"F\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == R) {
            if (childNodes.size() == 3 && childNodes.at(0)->getValue() == "[") {
                // Child 0: [
                // Child 1: <expr>
                // Child 2: ]

                generatedCode += generateCodeForNode(childNodes.at(1));
            } else if (childNodes.size() == 1 && isalpha(childNodes.at(0)->getValue()[0])) {
                // Child 0: $identifier

                generatedCode += CMD_LOAD + SPACE + childNodes.at(0)->getValue() + NEW_LINE;
            } else if (childNodes.size() == 1 && isdigit(childNodes.at(0)->getValue()[0])) {
                // Child 0: $number

                generatedCode += CMD_LOAD + SPACE + childNodes.at(0)->getValue() + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"R\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STATS) {
            if (childNodes.size() == 2) {
                // Child 0: <stat>
                // Child 1: <mStat>

                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += generateCodeForNode(childNodes.at(1));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"STATS\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == MSTAT) {
            if (childNodes.empty()) {
                // $empty
            } else if (childNodes.size() == 2 && childNodes.at(0)->getValue() == "stat") {
                // Child 0: <stat>
                // Child 1: <mStat>

                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += generateCodeForNode(childNodes.at(1));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"MSTAT\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STAT) {
            if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "in") {
                // Child 0: <in>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "out") {
                // Child 0: <out>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "block") {
                // Child 0: <block>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "if") {
                // Child 0: <if>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "loop") {
                // Child 0: <loop>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "assign") {
                // Child 0: <assign>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"STAT\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IN) {
            if (childNodes.size() == 3) {
                // Child 0: Input
                // Child 1: $identifier
                // Child 2: ;

                generatedCode += CMD_READ + SPACE + childNodes.at(1)->getValue() + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"IN\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == OUT) {
            if (childNodes.size() == 3) {
                // Child 0: Output
                // Child 1: <expr>
                // Child 2: ;

                generatedCode += generateCodeForNode(childNodes.at(1));
                const string tempIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + tempIdentifier + NEW_LINE;
                generatedCode += CMD_WRITE + SPACE + tempIdentifier + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"OUT\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IF) {
            if (childNodes.size() == 7) {
                // Child 0: Check
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: <stat>

                const string toLabel = generateThrowAwayLabel();
                generatedCode += CMD_BR + SPACE + toLabel + NEW_LINE;
                const string fromLabel = generateThrowAwayLabel();
                const string tempIdentifier = generateThrowAwayIdentifier();
                const string segment = toLabel + COLON + SPACE + CMD_NOOP + NEW_LINE
                                       + generateCodeForNode(childNodes.at(4))
                                       + CMD_STORE + SPACE + tempIdentifier + NEW_LINE
                                       + generateCodeForNode(childNodes.at(2))
                                       + CMD_SUB + SPACE + tempIdentifier + NEW_LINE
                                       + generateCodeForNode(childNodes.at(3)) + SPACE + fromLabel + NEW_LINE
                                       + generateCodeForNode(childNodes.at(6))
                                       + CMD_BR + SPACE + fromLabel + NEW_LINE;
                generatedSegments->push_back(segment);
                generatedCode += CMD_STOP + NEW_LINE;
                generatedCode += fromLabel + COLON + SPACE + CMD_NOOP + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"IF\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == LOOP) {
            if (childNodes.size() == 7) {
                // Child 0: Loop
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: <stat>

                const string toLabel = generateThrowAwayLabel();
                generatedCode += CMD_BR + SPACE + toLabel + NEW_LINE;
                const string fromLabel = generateThrowAwayLabel();
                const string tempIdentifier = generateThrowAwayIdentifier();
                const string segment = toLabel + COLON + SPACE + CMD_NOOP + NEW_LINE
                                       + generateCodeForNode(childNodes.at(4))
                                       + CMD_STORE + SPACE + tempIdentifier + NEW_LINE
                                       + generateCodeForNode(childNodes.at(2))
                                       + CMD_SUB + SPACE + tempIdentifier + NEW_LINE
                                       + generateCodeForNode(childNodes.at(3)) + SPACE + fromLabel + NEW_LINE
                                       + generateCodeForNode(childNodes.at(6))
                                       + CMD_BR + SPACE + toLabel + NEW_LINE;
                generatedSegments->push_back(segment);
                generatedCode += CMD_STOP + NEW_LINE;
                generatedCode += fromLabel + COLON + SPACE + CMD_NOOP + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"LOOP\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == ASSIGN) {
            if (childNodes.size() == 4) {
                // Child 0: $identifier
                // Child 1: :
                // Child 2: <expr>
                // Child 3: ;

                generatedCode += generateCodeForNode(childNodes.at(2));
                generatedCode += CMD_STORE + SPACE + childNodes.at(0)->getValue() + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"ASSIGN\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == RO) {
            if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "<") {
                // Child 0: <

                generatedCode += CMD_BRZPOS;
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "<=") {
                // Child 0: <=

                generatedCode += CMD_BRPOS;
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == ">") {
                // Child 0: >

                generatedCode += CMD_BRZNEG;
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == ">=") {
                // Child 0: >=

                generatedCode += CMD_BRNEG;
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "==") {
                // Child 0: ==

                const string temp = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + temp + NEW_LINE;
                generatedCode += CMD_MULT + SPACE + temp + NEW_LINE;
                generatedCode += CMD_BRPOS;
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "!=") {
                // Child 0: !=

                generatedCode += CMD_BRZERO;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"RO\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == TERMINAL) {
            // This should not be possible to reach, but is included for consistency anyway.
            cerr << "Code Generator Error: Processing of non-terminal node is marked as terminal.\n";
        } else {
            cerr << "Code Generator Error: Processing of non-terminal node is unrecognized. Marked as value \"" + to_string(node->getNonTerminalIdentifier()) + "\n";
        }
    }

    return generatedCode;
}

const string CodeGenerator::generateThrowAwayIdentifier() {
    return THROW_AWAY_IDENTIFIER_PREFIX + to_string(quantityThrowAwayIdentifiers++);
}

const string CodeGenerator::generateThrowAwayLabel() {
    return THROW_AWAY_LABEL_PREFIX + to_string(quantityThrowAwayLabels++);
}

const string CodeGenerator::generateSegmentCode() const {
    string generatedSegmentCode;

    for (const string &generatedSegment : *generatedSegments) {
        generatedSegmentCode += generatedSegment;
        generatedSegmentCode += CMD_STOP + NEW_LINE;
    }

    return generatedSegmentCode;
}

const string CodeGenerator::generateStorageAllocationCode() const {
    string generatedStorageAllocationCode;

    for (const string &variable : *variables) {
        generatedStorageAllocationCode += variable + SPACE + to_string(INITIAL_VARIABLE_VALUE) + NEW_LINE;
    }

    for (int throwAwayIdentifierIndex = 0; throwAwayIdentifierIndex < quantityThrowAwayIdentifiers; throwAwayIdentifierIndex++) {
        generatedStorageAllocationCode += THROW_AWAY_IDENTIFIER_PREFIX + to_string(throwAwayIdentifierIndex) + SPACE + to_string(INITIAL_VARIABLE_VALUE) + NEW_LINE;
    }

    return generatedStorageAllocationCode;
}
