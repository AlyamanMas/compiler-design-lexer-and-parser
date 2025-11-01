%{
/*
 * Modified Lexical Analyzer for C- Language Parser Integration
 * This version returns token codes for the parser
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Token codes - must match token.h */
enum {
    IF = 256,
    ELSE,
    WHILE,
    INT,
    FLOAT,
    RETURN,
    VOID,
    PROGRAM,
    ID,
    NUM,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    LT,
    LTE,
    GT,
    GTE,
    EQ,
    NEQ,
    ASSIGN,
    SEMI,
    COMMA,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    DOT,
    ENDOFFILE,
    ERROR
};

/* Global variables for tracking position */
int line_num = 1;
int col_num = 1;
int comment_start_line = 0;
int comment_start_col = 0;

/* Store current token value */
char token_lexeme[256];

/* Function to update column number */
void update_col(char* text) {
    col_num += strlen(text);
}

/* Function to print error */
void lex_error(const char* message) {
    fprintf(stderr, "LEXICAL ERROR at Line %d, Col %d: %s\n", line_num, col_num, message);
}

%}

%option noyywrap

/* Regular expression definitions */
LETTER      [a-zA-Z]
DIGIT       [0-9]
WHITESPACE  [ \t\r]

/* Modified ID pattern */
ID_PART1    {LETTER}({LETTER}|{DIGIT})*
ID_PART2    ([.#$_]?({LETTER}|{DIGIT})+)
ID          {ID_PART1}{ID_PART2}?

/* Modified NUM pattern */
NUM_INT     {DIGIT}+
NUM_FRAC    {DIGIT}+"."{DIGIT}*
NUM_BASE    ({NUM_INT}|{NUM_FRAC})
EXPONENT    [eE][+-]?{DIGIT}+
NUM         {NUM_BASE}{EXPONENT}?

/* States for comment handling */
%x COMMENT

%%

    /* Multi-line comment handling */
"/*"                {
                        comment_start_line = line_num;
                        comment_start_col = col_num;
                        update_col(yytext);
                        BEGIN(COMMENT);
                    }

<COMMENT>"*/"       {
                        update_col(yytext);
                        BEGIN(INITIAL);
                    }

<COMMENT>\n         {
                        line_num++;
                        col_num = 1;
                    }

<COMMENT>.          {
                        update_col(yytext);
                    }

<COMMENT><<EOF>>    {
                        lex_error("Unclosed comment");
                        return ERROR;
                    }

    /* Keywords (case-insensitive) */
[eE][lL][sS][eE]    {
                        strcpy(token_lexeme, "else");
                        update_col(yytext);
                        return ELSE;
                    }
[iI][fF]            {
                        strcpy(token_lexeme, "if");
                        update_col(yytext);
                        return IF;
                    }
[iI][nN][tT]        {
                        strcpy(token_lexeme, "int");
                        update_col(yytext);
                        return INT;
                    }
[fF][lL][oO][aA][tT] {
                        strcpy(token_lexeme, "float");
                        update_col(yytext);
                        return FLOAT;
                    }
[pP][rR][oO][gG][rR][aA][mM] {
                        strcpy(token_lexeme, "Program");
                        update_col(yytext);
                        return PROGRAM;
                    }
[rR][eE][tT][uU][rR][nN] {
                        strcpy(token_lexeme, "return");
                        update_col(yytext);
                        return RETURN;
                    }
[vV][oO][iI][dD]    {
                        strcpy(token_lexeme, "void");
                        update_col(yytext);
                        return VOID;
                    }
[wW][hH][iI][lL][eE] {
                        strcpy(token_lexeme, "while");
                        update_col(yytext);
                        return WHILE;
                    }

    /* Special symbols */
"+"                 { strcpy(token_lexeme, "+"); update_col(yytext); return PLUS; }
"-"                 { strcpy(token_lexeme, "-"); update_col(yytext); return MINUS; }
"*"                 { strcpy(token_lexeme, "*"); update_col(yytext); return TIMES; }
"/"                 { strcpy(token_lexeme, "/"); update_col(yytext); return DIVIDE; }
"<"                 { strcpy(token_lexeme, "<"); update_col(yytext); return LT; }
"<="                { strcpy(token_lexeme, "<="); update_col(yytext); return LTE; }
">"                 { strcpy(token_lexeme, ">"); update_col(yytext); return GT; }
">="                { strcpy(token_lexeme, ">="); update_col(yytext); return GTE; }
"=="                { strcpy(token_lexeme, "=="); update_col(yytext); return EQ; }
"!="                { strcpy(token_lexeme, "!="); update_col(yytext); return NEQ; }
"="                 { strcpy(token_lexeme, "="); update_col(yytext); return ASSIGN; }
";"                 { strcpy(token_lexeme, ";"); update_col(yytext); return SEMI; }
","                 { strcpy(token_lexeme, ","); update_col(yytext); return COMMA; }
"("                 { strcpy(token_lexeme, "("); update_col(yytext); return LPAREN; }
")"                 { strcpy(token_lexeme, ")"); update_col(yytext); return RPAREN; }
"["                 { strcpy(token_lexeme, "["); update_col(yytext); return LBRACKET; }
"]"                 { strcpy(token_lexeme, "]"); update_col(yytext); return RBRACKET; }
"{"                 { strcpy(token_lexeme, "{"); update_col(yytext); return LBRACE; }
"}"                 { strcpy(token_lexeme, "}"); update_col(yytext); return RBRACE; }
"."                 { strcpy(token_lexeme, "."); update_col(yytext); return DOT; }

    /* Numbers */
{NUM}               {
                        strcpy(token_lexeme, yytext);
                        update_col(yytext);
                        return NUM;
                    }

    /* Identifiers - must come after keywords */
{ID}                {
                        strcpy(token_lexeme, yytext);
                        update_col(yytext);
                        return ID;
                    }

    /* Whitespace */
{WHITESPACE}+       { update_col(yytext); }

    /* Newline */
\n                  { line_num++; col_num = 1; }

    /* Invalid character error */
.                   {
                        char msg[256];
                        snprintf(msg, sizeof(msg), "Invalid character '%c' (ASCII %d)", yytext[0], yytext[0]);
                        lex_error(msg);
                        update_col(yytext);
                        return ERROR;
                    }

%%
