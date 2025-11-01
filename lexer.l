%{
/*
 * Lexical Analyzer for C- Language
 * Course Assignment - Compiler Project
 * 
 * This lexer implements:
 * - Modified ID and NUM token definitions
 * - Case-insensitive keyword recognition
 * - Error handling for lexical errors
 * - Line and column tracking
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Global variables for tracking position */
int line_num = 1;
int col_num = 1;
int comment_start_line = 0;
int comment_start_col = 0;

/* Function to update column number */
void update_col(char* text) {
    col_num += strlen(text);
}

/* Function to print token */
void print_token(const char* token_type, const char* lexeme) {
    printf("Line %d, Col %d: %-15s %s\n", line_num, (int)(col_num - strlen(lexeme)), token_type, lexeme);
}

/* Function to print error */
void print_error(const char* message, int line, int col) {
    fprintf(stderr, "LEXICAL ERROR at Line %d, Col %d: %s\n", line, col, message);
}

/* Convert string to lowercase for keyword comparison */
void to_lowercase(char* str, char* result) {
    int i;
    for (i = 0; str[i]; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[i] = '\0';
}

%}

%option noyywrap

/* Regular expression definitions */
LETTER      [a-zA-Z]
DIGIT       [0-9]
WHITESPACE  [ \t\r]

/* Modified ID pattern: letter (letter|digit)* (("." | "#" | "$" | "_")? (letter|digit)+)? */
ID_PART1    {LETTER}({LETTER}|{DIGIT})*
ID_PART2    ([.#$_]?({LETTER}|{DIGIT})+)
ID          {ID_PART1}{ID_PART2}?

/* Modified NUM pattern: (digit+ | digit+ "." digit*) (((E|e) (+|-)? digit+))? */
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
                        print_error("Unclosed comment starting", comment_start_line, comment_start_col);
                        return 1;
                    }

    /* Keywords (case-insensitive) */
[eE][lL][sS][eE]    { print_token("KEYWORD", "else"); update_col(yytext); }
[iI][fF]            { print_token("KEYWORD", "if"); update_col(yytext); }
[iI][nN][tT]        { print_token("KEYWORD", "int"); update_col(yytext); }
[rR][eE][tT][uU][rR][nN] { print_token("KEYWORD", "return"); update_col(yytext); }
[vV][oO][iI][dD]    { print_token("KEYWORD", "void"); update_col(yytext); }
[wW][hH][iI][lL][eE] { print_token("KEYWORD", "while"); update_col(yytext); }

    /* Special symbols */
"+"                 { print_token("SPECIAL_SYMBOL", "+"); update_col(yytext); }
"-"                 { print_token("SPECIAL_SYMBOL", "-"); update_col(yytext); }
"*"                 { print_token("SPECIAL_SYMBOL", "*"); update_col(yytext); }
"/"                 { print_token("SPECIAL_SYMBOL", "/"); update_col(yytext); }
"<"                 { print_token("SPECIAL_SYMBOL", "<"); update_col(yytext); }
"<="                { print_token("SPECIAL_SYMBOL", "<="); update_col(yytext); }
">"                 { print_token("SPECIAL_SYMBOL", ">"); update_col(yytext); }
">="                { print_token("SPECIAL_SYMBOL", ">="); update_col(yytext); }
"=="                { print_token("SPECIAL_SYMBOL", "=="); update_col(yytext); }
"!="                { print_token("SPECIAL_SYMBOL", "!="); update_col(yytext); }
"="                 { print_token("SPECIAL_SYMBOL", "="); update_col(yytext); }
";"                 { print_token("SPECIAL_SYMBOL", ";"); update_col(yytext); }
","                 { print_token("SPECIAL_SYMBOL", ","); update_col(yytext); }
"("                 { print_token("SPECIAL_SYMBOL", "("); update_col(yytext); }
")"                 { print_token("SPECIAL_SYMBOL", ")"); update_col(yytext); }
"["                 { print_token("SPECIAL_SYMBOL", "["); update_col(yytext); }
"]"                 { print_token("SPECIAL_SYMBOL", "]"); update_col(yytext); }
"{"                 { print_token("SPECIAL_SYMBOL", "{"); update_col(yytext); }
"}"                 { print_token("SPECIAL_SYMBOL", "}"); update_col(yytext); }

    /* Numbers */
{NUM}               { print_token("NUM", yytext); update_col(yytext); }

    /* Identifiers */
{ID}                { 
                        char lower[256];
                        to_lowercase(yytext, lower);
                        /* Check if it matches a keyword pattern */
                        if (strcmp(lower, "else") == 0 || strcmp(lower, "if") == 0 || 
                            strcmp(lower, "int") == 0 || strcmp(lower, "return") == 0 || 
                            strcmp(lower, "void") == 0 || strcmp(lower, "while") == 0) {
                            print_token("KEYWORD", lower);
                        } else {
                            print_token("ID", yytext);
                        }
                        update_col(yytext);
                    }

    /* Error handling for malformed identifiers */
{LETTER}({LETTER}|{DIGIT})*[.#$_][^a-zA-Z0-9 \t\n\r] {
                        char msg[256];
                        snprintf(msg, sizeof(msg), "Malformed identifier '%s' - invalid character after separator", yytext);
                        print_error(msg, line_num, col_num);
                        update_col(yytext);
                    }

    /* Error handling for malformed numbers */
{NUM_BASE}[eE][+-]?[^0-9 \t\n\r] {
                        char msg[256];
                        snprintf(msg, sizeof(msg), "Malformed number '%s' - invalid character in exponent", yytext);
                        print_error(msg, line_num, col_num);
                        update_col(yytext);
                    }

{NUM_BASE}[eE][+-]?{DIGIT}+{LETTER}+ {
                        char msg[256];
                        snprintf(msg, sizeof(msg), "Malformed number '%s' - letter found after exponent", yytext);
                        print_error(msg, line_num, col_num);
                        update_col(yytext);
                    }

    /* Whitespace */
{WHITESPACE}+       { update_col(yytext); }

    /* Newline */
\n                  { line_num++; col_num = 1; }

    /* Invalid character error */
.                   {
                        char msg[256];
                        snprintf(msg, sizeof(msg), "Invalid character '%c' (ASCII %d)", yytext[0], yytext[0]);
                        print_error(msg, line_num, col_num);
                        update_col(yytext);
                    }

%%

int main(int argc, char** argv) {
    FILE* input_file = NULL;
    
    printf("=============================================================\n");
    printf("           Lexical Analyzer for C- Language\n");
    printf("=============================================================\n\n");
    
    if (argc > 1) {
        input_file = fopen(argv[1], "r");
        if (!input_file) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
            return 1;
        }
        yyin = input_file;
        printf("Reading from file: %s\n\n", argv[1]);
    } else {
        printf("Reading from standard input (Press Ctrl+D to end):\n\n");
        yyin = stdin;
    }
    
    printf("%-20s %-15s %s\n", "Position", "Token Type", "Lexeme");
    printf("-------------------------------------------------------------\n");
    
    yylex();
    
    if (input_file) {
        fclose(input_file);
    }
    
    printf("\n=============================================================\n");
    printf("                    Scanning Complete\n");
    printf("=============================================================\n");
    
    return 0;
}
