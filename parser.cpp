#include <iostream>
#include <cstdarg>
#include <vector>
#include "parser.h"

using namespace std;

const static int VA_LIST_TERMINATOR = -1;
const static bool NONTERMINAL = true;
const static bool TERMINAL = false;

static Token *token = nullptr;

Parser::Parser(const string &rawData) : rawData(rawData) {
    scanner = new Scanner(rawData);
}

Parser::~Parser() = default;

const string &Parser::getRawData() const {
    return rawData;
}

void Parser::setRawData(const string &rawData) {
    Parser::rawData = rawData;
}

Node *Parser::parse_program(const int level) const {
    Node *node = new Node("program", NONTERMINAL, level);

    node = consumeNonTerminal(node, parse_vars(level + 1));
    node = consumeNonTerminal(node, parse_block(level + 1));

    return node;
}

Node *Parser::parse_block(const int level) const {
    Node *node = new Node("block", NONTERMINAL, level);

    if (currentToken() == KEYWORD_BEGIN) {
        node = consumeTerminal(node);

        node = consumeNonTerminal(node, parse_vars(level + 1));
        node = consumeNonTerminal(node, parse_stats(level + 1));

        if (currentToken() == KEYWORD_END) {
            node = consumeTerminal(node);

            return node;
        } else {
            reportError(KEYWORD_END, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(KEYWORD_BEGIN, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse_vars(const int level) const {
    Node *node = new Node("vars", NONTERMINAL, level);

    if (currentToken() == KEYWORD_VAR) {
        node = consumeTerminal(node);

        if (currentToken() == IDENTIFIER) {
            node = consumeTerminal(node);

            node = consumeNonTerminal(node, parse_mvars(level + 1));

            return node;
        } else {
            reportError(IDENTIFIER, VA_LIST_TERMINATOR);
        }
    } else {
        return node;
    }
}

Node *Parser::parse_mvars(const int level) const {
    Node *node = new Node("mvars", NONTERMINAL, level);

    if (currentToken() == DELIMITER_PERIOD) {
        node = consumeTerminal(node);

        return node;
    } else if (currentToken() == DELIMITER_COMMA) {
        node = consumeTerminal(node);

        if (currentToken() == IDENTIFIER) {
            node = consumeTerminal(node);

            node = consumeNonTerminal(node, parse_mvars(level + 1));

            return node;
        } else {
            reportError(IDENTIFIER, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(DELIMITER_PERIOD, DELIMITER_COMMA, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse_expr(const int level) const {
    Node *node = new Node("expr", NONTERMINAL, level);

    node = consumeNonTerminal(node, parse_M(level + 1));

    if (currentToken() == OPERATOR_PLUS) {
        node = consumeTerminal(node);

        node = consumeNonTerminal(node, parse_expr(level + 1));

        return node;
    } else if (currentToken() == OPERATOR_MINUS) {
        node = consumeTerminal(node);

        node = consumeNonTerminal(node, parse_expr(level + 1));

        return node;
    } else {
        return node;
    }
}

Node *Parser::parse_M(const int level) const {
    Node *node = new Node("M", NONTERMINAL, level);

    node = consumeNonTerminal(node, parse_F(level + 1));

    if (currentToken() == OPERATOR_PERCENT) {
        node = consumeTerminal(node);

        node = consumeNonTerminal(node, parse_M(level + 1));

        return node;
    } else if (currentToken() == OPERATOR_ASTERISK) {
        node = consumeTerminal(node);

        node = consumeNonTerminal(node, parse_M(level + 1));

        return node;
    } else {
        return node;
    }
}

Node *Parser::parse_F(const int level) const {
    Node *node = new Node("F", NONTERMINAL, level);

    if (currentToken() == DELIMITER_LEFT_PARENTHESIS) {
        node = consumeTerminal(node);

        node = consumeNonTerminal(node, parse_F(level + 1));

        if (currentToken() == DELIMITER_RIGHT_PARENTHESIS) {
            node = consumeTerminal(node);

            return node;
        } else {
            reportError(DELIMITER_RIGHT_PARENTHESIS, VA_LIST_TERMINATOR);
        }
    } else {
        node = consumeNonTerminal(node, parse_R(level + 1));

        return node;
    }
}

Node *Parser::parse_R(const int level) const {
    Node *node = new Node("R", NONTERMINAL, level);

    if (currentToken() == DELIMITER_LEFT_SQUARE_BRACKET) {
        node = consumeTerminal(node);

        node = consumeNonTerminal(node, parse_expr(level + 1));

        if (currentToken() == DELIMITER_RIGHT_SQUARE_BRACKET) {
            node = consumeTerminal(node);

            return node;
        } else {
            reportError(DELIMITER_RIGHT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
        }
    } else if (currentToken() == IDENTIFIER) {
        node = consumeTerminal(node);

        return node;
    } else if (currentToken() == INTEGER) {
        node = consumeTerminal(node);

        return node;
    } else {
        reportError(DELIMITER_LEFT_SQUARE_BRACKET, IDENTIFIER, INTEGER, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse_stats(const int level) const {
    Node *node = new Node("stats", NONTERMINAL, level);

    node = consumeNonTerminal(node, parse_stat(level + 1));
    node = consumeNonTerminal(node, parse_mStat(level + 1));

    return node;
}

Node *Parser::parse_mStat(const int level) const {
    Node *node = new Node("mStat", NONTERMINAL, level);

    if (currentToken() == KEYWORD_INPUT || currentToken() == KEYWORD_OUTPUT || currentToken() == KEYWORD_BEGIN || currentToken() == KEYWORD_CHECK || currentToken() == KEYWORD_LOOP || currentToken() == IDENTIFIER) {
        node = consumeNonTerminal(node, parse_stat(level + 1));
        node = consumeNonTerminal(node, parse_mStat(level + 1));

        return node;
    } else {
        return node;
    }
}

Node *Parser::parse_stat(const int level) const {
    Node *node = new Node("stat", NONTERMINAL, level);

    if (currentToken() == KEYWORD_INPUT) {
        node = consumeNonTerminal(node, parse_in(level + 1));

        return node;
    } else if (currentToken() == KEYWORD_OUTPUT) {
        node = consumeNonTerminal(node, parse_out(level + 1));

        return node;
    } else if (currentToken() == KEYWORD_BEGIN) {
        node = consumeNonTerminal(node, parse_block(level + 1));

        return node;
    } else if (currentToken() == KEYWORD_CHECK) {
        node = consumeNonTerminal(node, parse_if(level + 1));

        return node;
    } else if (currentToken() == KEYWORD_LOOP) {
        node = consumeNonTerminal(node, parse_loop(level + 1));

        return node;
    } else if (currentToken() == IDENTIFIER) {
        node = consumeNonTerminal(node, parse_assign(level + 1));

        return node;
    } else {
        reportError(KEYWORD_INPUT, KEYWORD_OUTPUT, KEYWORD_BEGIN, KEYWORD_CHECK, KEYWORD_LOOP, IDENTIFIER, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse_in(const int level) const {
    Node *node = new Node("in", NONTERMINAL, level);

    if (currentToken() == KEYWORD_INPUT) {
        node = consumeTerminal(node);

        if (currentToken() == IDENTIFIER) {
            node = consumeTerminal(node);

            if (currentToken() == DELIMITER_SEMICOLON) {
                node = consumeTerminal(node);

                return node;
            } else {
                reportError(DELIMITER_SEMICOLON, VA_LIST_TERMINATOR);
            }
        } else {
            reportError(IDENTIFIER, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(KEYWORD_INPUT, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse_out(const int level) const {
    Node *node = new Node("out", NONTERMINAL, level);

    if (currentToken() == KEYWORD_OUTPUT) {
        node = consumeTerminal(node);

        node = consumeNonTerminal(node, parse_expr(level + 1));

        if (currentToken() == DELIMITER_SEMICOLON) {
            node = consumeTerminal(node);

            return node;
        } else {
            reportError(DELIMITER_SEMICOLON, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(KEYWORD_OUTPUT, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse_if(const int level) const {
    Node *node = new Node("if", NONTERMINAL, level);

    if (currentToken() == KEYWORD_CHECK) {
        node = consumeTerminal(node);

        if (currentToken() == DELIMITER_LEFT_SQUARE_BRACKET) {
            node = consumeTerminal(node);

            node = consumeNonTerminal(node, parse_expr(level + 1));
            node = consumeNonTerminal(node, parse_RO(level + 1));
            node = consumeNonTerminal(node, parse_expr(level + 1));

            if (currentToken() == DELIMITER_RIGHT_SQUARE_BRACKET) {
                node = consumeTerminal(node);

                node = consumeNonTerminal(node, parse_stat(level + 1));

                return node;
            } else {
                reportError(DELIMITER_RIGHT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
            }
        } else {
            reportError(DELIMITER_LEFT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(KEYWORD_CHECK, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse_loop(const int level) const {
    Node *node = new Node("loop", NONTERMINAL, level);

    if (currentToken() == KEYWORD_LOOP) {
        node = consumeTerminal(node);

        if (currentToken() == DELIMITER_LEFT_SQUARE_BRACKET) {
            node = consumeTerminal(node);

            node = consumeNonTerminal(node, parse_expr(level + 1));
            node = consumeNonTerminal(node, parse_RO(level + 1));
            node = consumeNonTerminal(node, parse_expr(level + 1));

            if (currentToken() == DELIMITER_RIGHT_SQUARE_BRACKET) {
                node = consumeTerminal(node);

                node = consumeNonTerminal(node, parse_stat(level + 1));

                return node;
            } else {
                reportError(DELIMITER_RIGHT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
            }
        } else {
            reportError(DELIMITER_LEFT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(KEYWORD_LOOP, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse_assign(const int level) const {
    Node *node = new Node("assign", NONTERMINAL, level);

    if (currentToken() == IDENTIFIER) {
        node = consumeTerminal(node);

        if (currentToken() == OPERATOR_COLON) {
            node = consumeTerminal(node);

            node = consumeNonTerminal(node, parse_expr(level + 1));

            if (currentToken() == DELIMITER_SEMICOLON) {
                node = consumeTerminal(node);

                return node;
            } else {
                reportError(DELIMITER_SEMICOLON, VA_LIST_TERMINATOR);
            }
        } else {
            reportError(OPERATOR_COLON, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(IDENTIFIER, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse_RO(const int level) const {
    Node *node = new Node("RO", NONTERMINAL, level);

    if (currentToken() == OPERATOR_LESS_THAN) {
        node = consumeTerminal(node);

        return node;
    } else if (currentToken() == OPERATOR_LESS_THAN_OR_EQUAL_TO) {
        node = consumeTerminal(node);

        return node;
    } else if (currentToken() == OPERATOR_GREATER_THAN) {
        node = consumeTerminal(node);

        return node;
    } else if (currentToken() == OPERATOR_GREATER_THAN_OR_EQUAL_TO) {
        node = consumeTerminal(node);

        return node;
    } else if (currentToken() == OPERATOR_DOUBLE_EQUALS) {
        node = consumeTerminal(node);

        return node;
    } else if (currentToken() == OPERATOR_EXCLAMATION_POINT_EQUALS) {
        node = consumeTerminal(node);

        return node;
    } else {
        reportError(OPERATOR_LESS_THAN, OPERATOR_LESS_THAN_OR_EQUAL_TO, OPERATOR_GREATER_THAN, OPERATOR_GREATER_THAN_OR_EQUAL_TO, OPERATOR_DOUBLE_EQUALS, OPERATOR_EXCLAMATION_POINT_EQUALS, VA_LIST_TERMINATOR);
    }
}

Node *Parser::parse() const {
    token = scanner->getNextToken();

    Node *root = parse_program(ROOT_LEVEL);

    if (currentToken() == END_OF_FILE) {
        cout << "Parsing completed successfully.\n";
    } else {
        reportError(END_OF_FILE, -1);
    }

    return root;
}

const void Parser::reportError(TOKEN_IDENTIFIER expectedToken, ...) const {
    string listOfExpectedTokens;

    va_list va_list;
    va_start(va_list, expectedToken);

    while (expectedToken != VA_LIST_TERMINATOR) {
        listOfExpectedTokens += "\"" + TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP.at(expectedToken) + "\" or ";
        expectedToken = static_cast<TOKEN_IDENTIFIER>(va_arg(va_list, int));
    }

    listOfExpectedTokens = listOfExpectedTokens.substr(0, listOfExpectedTokens.length() - 4);

    cerr << "PARSE ERROR: Got token \"" << TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP.at(currentToken()) << "\" but " << listOfExpectedTokens << " was expected on line \"" << scanner->getCurrentLineNumber() << "\".\n";
    exit(1);
}

const TOKEN_IDENTIFIER Parser::currentToken() const {
    return token->getTokenIdentifier();
}

Node *Parser::consumeTerminal(Node *node) const {
    if (node->getLevel() != ROOT_LEVEL) {
        vector<Node *> children = node->getChildren();
        children.push_back(new Node(token->getValue(), TERMINAL, node->getLevel() + 1));
        node->setChildren(children);
    }

    token = scanner->getNextToken();

    return node;
}

Node *Parser::consumeNonTerminal(Node *originalNode, Node *nonTerminalNode) const {
    vector<Node *> originalNodesChildren = originalNode->getChildren();
    originalNodesChildren.push_back(nonTerminalNode);
    originalNode->setChildren(originalNodesChildren);

    return originalNode;
}