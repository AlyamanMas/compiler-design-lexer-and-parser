#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

/* Token type definitions */
typedef enum {
    /* Keywords */
    IF = 256,
    ELSE,
    WHILE,
    INT,
    FLOAT,
    RETURN,
    VOID,
    PROGRAM,

    /* Identifiers and Numbers */
    ID,
    NUM,

    /* Operators */
    PLUS,       // +
    MINUS,      // -
    TIMES,      // *
    DIVIDE,     // /
    LT,         // <
    LTE,        // <=
    GT,         // >
    GTE,        // >=
    EQ,         // ==
    NEQ,        // !=
    ASSIGN,     // =

    /* Delimiters */
    SEMI,       // ;
    COMMA,      // ,
    LPAREN,     // (
    RPAREN,     // )
    LBRACKET,   // [
    RBRACKET,   // ]
    LBRACE,     // {
    RBRACE,     // }
    DOT,        // .

    /* Special tokens */
    ENDOFFILE,
    ERROR
} TokenType;

/* External declarations from lexer */
#ifdef __cplusplus
extern "C" {
#endif

/* Flex-generated declarations */
extern int yylex(void);
extern FILE* yyin;

/* Our custom declarations */
extern char token_lexeme[256];
extern int line_num;
extern int col_num;

#ifdef __cplusplus
}
#endif

#endif /* TOKEN_H */
