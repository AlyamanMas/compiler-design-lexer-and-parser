#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ParseTree.h"
#include <memory>
#include <string>
#include <sstream>

class Parser {
private:
    TokenType currentToken;
    std::string currentLexeme;
    int currentLine;
    int currentCol;
    bool hasError;
    std::string errorMessage;

    // Fetch next token from lexer
    void nextToken() {
        int token = yylex();
        if (token == 0) {
            currentToken = ENDOFFILE;
            currentLexeme = "EOF";
        } else {
            currentToken = static_cast<TokenType>(token);
            currentLexeme = token_lexeme;
        }
        currentLine = line_num;
        currentCol = col_num;
    }

    // Match expected token
    bool match(TokenType expected) {
        if (currentToken == expected) {
            return true;
        }
        return false;
    }

    // Consume token and create terminal node
    std::shared_ptr<ParseTreeNode> consume(TokenType expected, const std::string& tokenName) {
        if (currentToken == expected) {
            auto node = std::make_shared<TerminalNode>(tokenName, currentLexeme);
            nextToken();
            return node;
        } else {
            reportError("Expected " + tokenName + " but found '" + currentLexeme + "'");
            return nullptr;
        }
    }

    // Report parsing error
    void reportError(const std::string& message) {
        if (!hasError) {  // Report only the first error
            hasError = true;
            std::ostringstream oss;
            oss << "SYNTAX ERROR at Line " << currentLine << ", Col " << currentCol
                << ": " << message;
            errorMessage = oss.str();
        }
    }

    // Parsing functions for each grammar rule
    std::shared_ptr<ParseTreeNode> parseProgram();
    std::shared_ptr<ParseTreeNode> parseDeclarationList();
    std::shared_ptr<ParseTreeNode> parseDeclarationListPrime();
    std::shared_ptr<ParseTreeNode> parseDeclaration();
    std::shared_ptr<ParseTreeNode> parseVarDeclaration();
    std::shared_ptr<ParseTreeNode> parseVarDeclarationPrime();
    std::shared_ptr<ParseTreeNode> parseTypeSpecifier();
    std::shared_ptr<ParseTreeNode> parseParams();
    std::shared_ptr<ParseTreeNode> parseParamList();
    std::shared_ptr<ParseTreeNode> parseParamListPrime();
    std::shared_ptr<ParseTreeNode> parseParam();
    std::shared_ptr<ParseTreeNode> parseParamPrime();
    std::shared_ptr<ParseTreeNode> parseCompoundStmt();
    std::shared_ptr<ParseTreeNode> parseStatementList();
    std::shared_ptr<ParseTreeNode> parseStatementListPrime();
    std::shared_ptr<ParseTreeNode> parseStatement();
    std::shared_ptr<ParseTreeNode> parseSelectionStmt();
    std::shared_ptr<ParseTreeNode> parseSelectionStmtPrime();
    std::shared_ptr<ParseTreeNode> parseIterationStmt();
    std::shared_ptr<ParseTreeNode> parseAssignmentStmt();
    std::shared_ptr<ParseTreeNode> parseVar();
    std::shared_ptr<ParseTreeNode> parseVarPrime();
    std::shared_ptr<ParseTreeNode> parseExpression();
    std::shared_ptr<ParseTreeNode> parseExpressionPrime();
    std::shared_ptr<ParseTreeNode> parseRelop();
    std::shared_ptr<ParseTreeNode> parseAdditiveExpression();
    std::shared_ptr<ParseTreeNode> parseAdditiveExpressionPrime();
    std::shared_ptr<ParseTreeNode> parseAddop();
    std::shared_ptr<ParseTreeNode> parseTerm();
    std::shared_ptr<ParseTreeNode> parseTermPrime();
    std::shared_ptr<ParseTreeNode> parseMulop();
    std::shared_ptr<ParseTreeNode> parseFactor();

public:
    Parser() : currentToken(ERROR), currentLine(0), currentCol(0), hasError(false) {}

    // Main parse function
    std::shared_ptr<ParseTreeNode> parse() {
        nextToken();  // Get first token
        auto tree = parseProgram();

        if (hasError) {
            return nullptr;
        }

        if (currentToken != ENDOFFILE) {
            reportError("Expected end of file but found '" + currentLexeme + "'");
            return nullptr;
        }

        return tree;
    }

    bool hadError() const { return hasError; }
    std::string getErrorMessage() const { return errorMessage; }
};

#endif /* PARSER_H */
