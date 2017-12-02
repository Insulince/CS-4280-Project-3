#ifndef P3_NONTERMINAL_H
#define P3_NONTERMINAL_H

enum NONTERMINAL_IDENTIFIER {
    PROGRAM,
    BLOCK,
    VARS,
    MVARS,
    EXPR,
    M,
    F,
    R,
    STATS,
    MSTAT,
    STAT,
    IN,
    OUT,
    IF,
    LOOP,
    ASSIGN,
    RO,

    TERMINAL // Used for nodes who are not non-terminal.
};

#endif //P3_NONTERMINAL_H
