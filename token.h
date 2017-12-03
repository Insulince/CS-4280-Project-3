#ifndef P2_TOKEN_H
#define P2_TOKEN_H

#include <map>
#include <string>
#include "util.h"

enum TOKEN_IDENTIFIER {
    END_OF_FILE, //Indicates the end of the file has been reached.
    IDENTIFIER, //Conforms to the regex (JavaScript notation) "/^[a-zA-Z]{1}[a-zA-Z0-9]{0,7}$/".
    INTEGER, //Conforms to the regex (JavaScript notation) "/^[0-9]{1,8}$/".
    KEYWORD_BEGIN, //"Begin"
    KEYWORD_END, //"End"
    KEYWORD_CHECK, //"Check"
    KEYWORD_LOOP, //"Loop"
    KEYWORD_VOID, //"Void"
    KEYWORD_VAR, //"Var"
    KEYWORD_RETURN, //"Return"
    KEYWORD_INPUT, //"Input"
    KEYWORD_OUTPUT, //"Output"
    KEYWORD_PROGRAM, //"Program"
    OPERATOR_EQUALS, //"="
    OPERATOR_LESS_THAN, //"<"
    OPERATOR_LESS_THAN_OR_EQUAL_TO, //"<="
    OPERATOR_GREATER_THAN, //">"
    OPERATOR_GREATER_THAN_OR_EQUAL_TO, //">="
    ILLEGAL_EXCLAMATION_POINT, //"!"
    OPERATOR_EXCLAMATION_POINT_EQUALS, //"!="
    OPERATOR_DOUBLE_EQUALS, //"=="
    OPERATOR_COLON, //":"
    OPERATOR_PLUS, //"+"
    OPERATOR_MINUS, //"-"
    OPERATOR_ASTERISK, //"*"
    OPERATOR_FORWARD_SLASH, //"/"
    OPERATOR_AMPERSAND, //"&"
    OPERATOR_PERCENT, //"%"
    DELIMITER_PERIOD, //"."
    DELIMITER_LEFT_PARENTHESIS, //"("
    DELIMITER_RIGHT_PARENTHESIS, //")"
    DELIMITER_LEFT_CURLY_BRACE, //"{"
    DELIMITER_RIGHT_CURLY_BRACE, //"}"
    DELIMITER_LEFT_SQUARE_BRACKET, //"["
    DELIMITER_RIGHT_SQUARE_BRACKET, //"]"
    DELIMITER_COMMA, //","
    DELIMITER_SEMICOLON //";"
};

const std::string END_OF_FILE_TOKEN_NAME = "END_OF_FILE";
const std::string IDENTIFIER_TOKEN_NAME = "IDENTIFIER";
const std::string INTEGER_TOKEN_NAME = "INTEGER";
const std::string KEYWORD_BEGIN_TOKEN_NAME = "KEYWORD_BEGIN";
const std::string KEYWORD_END_TOKEN_NAME = "KEYWORD_END";
const std::string KEYWORD_CHECK_TOKEN_NAME = "KEYWORD_CHECK";
const std::string KEYWORD_LOOP_TOKEN_NAME = "KEYWORD_LOOP";
const std::string KEYWORD_VOID_TOKEN_NAME = "KEYWORD_VOID";
const std::string KEYWORD_VAR_TOKEN_NAME = "KEYWORD_VAR";
const std::string KEYWORD_RETURN_TOKEN_NAME = "KEYWORD_RETURN";
const std::string KEYWORD_INPUT_TOKEN_NAME = "KEYWORD_INPUT";
const std::string KEYWORD_OUTPUT_TOKEN_NAME = "KEYWORD_OUTPUT";
const std::string KEYWORD_PROGRAM_TOKEN_NAME = "KEYWORD_PROGRAM";
const std::string OPERATOR_EQUALS_TOKEN_NAME = "OPERATOR_EQUALS";
const std::string OPERATOR_LESS_THAN_TOKEN_NAME = "OPERATOR_LESS_THAN";
const std::string OPERATOR_LESS_THAN_OR_EQUAL_TO_TOKEN_NAME = "OPERATOR_LESS_THAN_OR_EQUAL_TO";
const std::string OPERATOR_GREATER_THAN_TOKEN_NAME = "OPERATOR_GREATER_THAN";
const std::string OPERATOR_GREATER_THAN_OR_EQUAL_TO_TOKEN_NAME = "OPERATOR_GREATER_THAN_OR_EQUAL_TO";
const std::string ILLEGAL_EXCLAMATION_POINT_TOKEN_NAME = "ILLEGAL_EXCLAMATION_POINT";
const std::string OPERATOR_EXCLAMATION_POINT_EQUALS_TOKEN_NAME = "OPERATOR_EXCLAMATION_POINT_EQUALS";
const std::string OPERATOR_DOUBLE_EQUALS_TOKEN_NAME = "OPERATOR_DOUBLE_EQUALS";
const std::string OPERATOR_COLON_TOKEN_NAME = "OPERATOR_COLON";
const std::string OPERATOR_PLUS_TOKEN_NAME = "OPERATOR_PLUS";
const std::string OPERATOR_MINUS_TOKEN_NAME = "OPERATOR_MINUS";
const std::string OPERATOR_ASTERISK_TOKEN_NAME = "OPERATOR_ASTERISK";
const std::string OPERATOR_FORWARD_SLASH_TOKEN_NAME = "OPERATOR_FORWARD_SLASH";
const std::string OPERATOR_AMPERSAND_TOKEN_NAME = "OPERATOR_AMPERSAND";
const std::string OPERATOR_PERCENT_TOKEN_NAME = "OPERATOR_PERCENT";
const std::string DELIMITER_PERIOD_TOKEN_NAME = "DELIMITER_PERIOD";
const std::string DELIMITER_LEFT_PARENTHESIS_TOKEN_NAME = "DELIMITER_LEFT_PARENTHESIS";
const std::string DELIMITER_RIGHT_PARENTHESIS_TOKEN_NAME = "DELIMITER_RIGHT_PARENTHESIS";
const std::string DELIMITER_LEFT_CURLY_BRACE_TOKEN_NAME = "DELIMITER_LEFT_CURLY_BRACE";
const std::string DELIMITER_RIGHT_CURLY_BRACE_TOKEN_NAME = "DELIMITER_RIGHT_CURLY_BRACE";
const std::string DELIMITER_LEFT_SQUARE_BRACKET_TOKEN_NAME = "DELIMITER_LEFT_SQUARE_BRACKET";
const std::string DELIMITER_RIGHT_SQUARE_BRACKET_TOKEN_NAME = "DELIMITER_RIGHT_SQUARE_BRACKET";
const std::string DELIMITER_COMMA_TOKEN_NAME = "DELIMITER_COMMA";
const std::string DELIMITER_SEMICOLON_TOKEN_NAME = "DELIMITER_SEMICOLON";

const std::map<const TOKEN_IDENTIFIER, const std::string> TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP = {
        {END_OF_FILE,                       END_OF_FILE_TOKEN_NAME},
        {IDENTIFIER,                        IDENTIFIER_TOKEN_NAME},
        {INTEGER,                           INTEGER_TOKEN_NAME},
        {KEYWORD_BEGIN,                     KEYWORD_BEGIN_TOKEN_NAME},
        {KEYWORD_END,                       KEYWORD_END_TOKEN_NAME},
        {KEYWORD_CHECK,                     KEYWORD_CHECK_TOKEN_NAME},
        {KEYWORD_LOOP,                      KEYWORD_LOOP_TOKEN_NAME},
        {KEYWORD_VOID,                      KEYWORD_VOID_TOKEN_NAME},
        {KEYWORD_VAR,                       KEYWORD_VAR_TOKEN_NAME},
        {KEYWORD_RETURN,                    KEYWORD_RETURN_TOKEN_NAME},
        {KEYWORD_INPUT,                     KEYWORD_INPUT_TOKEN_NAME},
        {KEYWORD_OUTPUT,                    KEYWORD_OUTPUT_TOKEN_NAME},
        {KEYWORD_PROGRAM,                   KEYWORD_PROGRAM_TOKEN_NAME},
        {OPERATOR_EQUALS,                   OPERATOR_EQUALS_TOKEN_NAME},
        {OPERATOR_LESS_THAN,                OPERATOR_LESS_THAN_TOKEN_NAME},
        {OPERATOR_LESS_THAN_OR_EQUAL_TO,    OPERATOR_LESS_THAN_OR_EQUAL_TO_TOKEN_NAME},
        {OPERATOR_GREATER_THAN,             OPERATOR_GREATER_THAN_TOKEN_NAME},
        {OPERATOR_GREATER_THAN_OR_EQUAL_TO, OPERATOR_GREATER_THAN_OR_EQUAL_TO_TOKEN_NAME},
        {ILLEGAL_EXCLAMATION_POINT,         ILLEGAL_EXCLAMATION_POINT_TOKEN_NAME},
        {OPERATOR_EXCLAMATION_POINT_EQUALS, OPERATOR_EXCLAMATION_POINT_EQUALS_TOKEN_NAME},
        {OPERATOR_DOUBLE_EQUALS,            OPERATOR_DOUBLE_EQUALS_TOKEN_NAME},
        {OPERATOR_COLON,                    OPERATOR_COLON_TOKEN_NAME},
        {OPERATOR_PLUS,                     OPERATOR_PLUS_TOKEN_NAME},
        {OPERATOR_MINUS,                    OPERATOR_MINUS_TOKEN_NAME},
        {OPERATOR_ASTERISK,                 OPERATOR_ASTERISK_TOKEN_NAME},
        {OPERATOR_FORWARD_SLASH,            OPERATOR_FORWARD_SLASH_TOKEN_NAME},
        {OPERATOR_AMPERSAND,                OPERATOR_AMPERSAND_TOKEN_NAME},
        {OPERATOR_PERCENT,                  OPERATOR_PERCENT_TOKEN_NAME},
        {DELIMITER_PERIOD,                  DELIMITER_PERIOD_TOKEN_NAME},
        {DELIMITER_LEFT_PARENTHESIS,        DELIMITER_LEFT_PARENTHESIS_TOKEN_NAME},
        {DELIMITER_RIGHT_PARENTHESIS,       DELIMITER_RIGHT_PARENTHESIS_TOKEN_NAME},
        {DELIMITER_LEFT_CURLY_BRACE,        DELIMITER_LEFT_CURLY_BRACE_TOKEN_NAME},
        {DELIMITER_RIGHT_CURLY_BRACE,       DELIMITER_RIGHT_CURLY_BRACE_TOKEN_NAME},
        {DELIMITER_LEFT_SQUARE_BRACKET,     DELIMITER_LEFT_SQUARE_BRACKET_TOKEN_NAME},
        {DELIMITER_RIGHT_SQUARE_BRACKET,    DELIMITER_RIGHT_SQUARE_BRACKET_TOKEN_NAME},
        {DELIMITER_COMMA,                   DELIMITER_COMMA_TOKEN_NAME},
        {DELIMITER_SEMICOLON,               DELIMITER_SEMICOLON_TOKEN_NAME}
};

const std::string KEYWORD_LITERAL_BEGIN = "Begin";
const std::string KEYWORD_LITERAL_END = "End";
const std::string KEYWORD_LITERAL_CHECK = "Check";
const std::string KEYWORD_LITERAL_LOOP = "Loop";
const std::string KEYWORD_LITERAL_VOID = "Void";
const std::string KEYWORD_LITERAL_VAR = "Var";
const std::string KEYWORD_LITERAL_RETURN = "Return";
const std::string KEYWORD_LITERAL_INPUT = "Input";
const std::string KEYWORD_LITERAL_OUTPUT = "Output";
const std::string KEYWORD_LITERAL_PROGRAM = "Program";

const std::map<const std::string, const TOKEN_IDENTIFIER> KEYWORD_LITERAL_TO_KEYWORD_TOKEN_IDENTIFIER_MAP = {
        {KEYWORD_LITERAL_BEGIN,   KEYWORD_BEGIN},
        {KEYWORD_LITERAL_END,     KEYWORD_END},
        {KEYWORD_LITERAL_CHECK,   KEYWORD_CHECK},
        {KEYWORD_LITERAL_LOOP,    KEYWORD_LOOP},
        {KEYWORD_LITERAL_VOID,    KEYWORD_VOID},
        {KEYWORD_LITERAL_VAR,     KEYWORD_VAR},
        {KEYWORD_LITERAL_RETURN,  KEYWORD_RETURN},
        {KEYWORD_LITERAL_INPUT,   KEYWORD_INPUT},
        {KEYWORD_LITERAL_OUTPUT,  KEYWORD_OUTPUT},
        {KEYWORD_LITERAL_PROGRAM, KEYWORD_PROGRAM},
};

class Token {
private:
    const TOKEN_IDENTIFIER tokenIdentifier;
    const std::string value;
    const std::string lineNumber;
public:
    Token(const TOKEN_IDENTIFIER tokenIdentifier, const std::string value, const std::string lineNumber) : tokenIdentifier(tokenIdentifier),
                                                                                                           value(value),
                                                                                                           lineNumber(lineNumber) {
    };

    ~Token() = default;

    const TOKEN_IDENTIFIER getTokenIdentifier() const {
        return tokenIdentifier;
    }

    const std::string &getValue() const {
        return value;
    }

    const std::string &getLineNumber() const {
        return lineNumber;
    }

    const std::string toString() const {
        return "Line Number: \"" + getLineNumber() + "\", Token Identifier: \"" + TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP.at(getTokenIdentifier()) + "\", Token Value: \"" + trim(getValue()) + "\"";
    }
};

#endif //P2_TOKEN_H
