#ifndef P2_PARSER_H
#define P2_PARSER_H

#include "token.h"
#include "scanner.h"
#include "node.h"

/* CONTEXT-FREE GRAMMAR:
 * <program>	-> <vars> <block>
 * <block> 		-> Begin <vars> <stats> End
 * <vars> 		-> $empty | Var $identifier <mvars>
 * <mvars> 		-> . | , $identifier <mvars>
 * <expr> 		-> <M> + <expr> | <M> - <expr> | <M>
 * <M> 			-> <F> % <M> | <F> * <M> | <F>
 * <F> 			-> (<F>) | <R>
 * <R>  		-> [<expr>] | $identifier | $number
 * <stats> 		-> <stat> <mStat>
 * <mStat> 		-> $empty | <stat> <mStat>
 * <stat> 		-> <in> | <out> | <block> | <if> | <loop> | <assign>
 * <in> 		-> Input $identifier;
 * <out> 		-> Output <expr>;
 * <if> 		-> Check [<expr> <RO> <expr>] <stat>
 * <loop> 		-> Loop [<expr> <RO> <expr>] <stat>
 * <assign> 	-> $identifier: <expr>;
 * <RO> 		-> < | <= | > | >= | == | !=
 * */

class Parser {
private:
    std::string rawData;
    Scanner *scanner;
public:
    explicit Parser(const std::string &rawData);

    ~Parser();

    const std::string &getRawData() const;

    void setRawData(const std::string &rawData);

    Node *parse() const;

    Node *parse_program(const int level) const;

    Node *parse_block(const int level) const;

    Node *parse_vars(const int level) const;

    Node *parse_mvars(const int level) const;

    Node *parse_expr(const int level) const;

    Node *parse_M(const int level) const;

    Node *parse_F(const int level) const;

    Node *parse_R(const int level) const;

    Node *parse_stats(const int level) const;

    Node *parse_mStat(const int level) const;

    Node *parse_stat(const int level) const;

    Node *parse_in(const int level) const;

    Node *parse_out(const int level) const;

    Node *parse_if(const int level) const;

    Node *parse_loop(const int level) const;

    Node *parse_assign(const int level) const;

    Node *parse_RO(const int level) const;

    const void reportError(TOKEN_IDENTIFIER expectedToken, ...) const;

    const TOKEN_IDENTIFIER currentToken() const;

    Node *consumeTerminal(Node *node) const;

    Node *consumeNonTerminal(Node *originalNode, Node *nonTerminalNode) const;
};

#endif //P2_PARSER_H
