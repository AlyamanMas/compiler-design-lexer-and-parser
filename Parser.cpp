#include "Parser.h"
#include <iostream>

using namespace std;

// program ::= Program ID "{" declaration-list statement-list "}" "."
shared_ptr<ParseTreeNode> Parser::parseProgram() {
    auto node = make_shared<NonTerminalNode>("program");

    auto programToken = consume(PROGRAM, "Program");
    if (!programToken) return nullptr;
    node->addChild(programToken);

    auto idToken = consume(ID, "ID");
    if (!idToken) return nullptr;
    node->addChild(idToken);

    auto lbrace1 = consume(LBRACE, "{");
    if (!lbrace1) return nullptr;
    node->addChild(lbrace1);

    auto declList = parseDeclarationList();
    if (!declList) return nullptr;
    node->addChild(declList);

    auto stmtList = parseStatementList();
    if (!stmtList) return nullptr;
    node->addChild(stmtList);

    auto rbrace = consume(RBRACE, "}");
    if (!rbrace) return nullptr;
    node->addChild(rbrace);

    auto dot = consume(DOT, ".");
    if (!dot) return nullptr;
    node->addChild(dot);

    return node;
}

// declaration-list ::= declaration declaration-list'
shared_ptr<ParseTreeNode> Parser::parseDeclarationList() {
    auto node = make_shared<NonTerminalNode>("declaration-list");

    auto decl = parseDeclaration();
    if (!decl) return nullptr;
    node->addChild(decl);

    auto declListPrime = parseDeclarationListPrime();
    if (!declListPrime) return nullptr;
    node->addChild(declListPrime);

    return node;
}

// declaration-list' ::= declaration declaration-list' | empty
shared_ptr<ParseTreeNode> Parser::parseDeclarationListPrime() {
    auto node = make_shared<NonTerminalNode>("declaration-list'");

    // Check if we have another declaration (starts with type-specifier: int or float)
    if (match(INT) || match(FLOAT)) {
        auto decl = parseDeclaration();
        if (!decl) return nullptr;
        node->addChild(decl);

        auto declListPrime = parseDeclarationListPrime();
        if (!declListPrime) return nullptr;
        node->addChild(declListPrime);
    } else {
        // Empty production
        node->addChild(make_shared<EpsilonNode>());
    }

    return node;
}

// declaration ::= var-declaration
shared_ptr<ParseTreeNode> Parser::parseDeclaration() {
    auto node = make_shared<NonTerminalNode>("declaration");

    auto varDecl = parseVarDeclaration();
    if (!varDecl) return nullptr;
    node->addChild(varDecl);

    return node;
}

// var-declaration ::= type-specifier ID var-declaration'
shared_ptr<ParseTreeNode> Parser::parseVarDeclaration() {
    auto node = make_shared<NonTerminalNode>("var-declaration");

    auto typeSpec = parseTypeSpecifier();
    if (!typeSpec) return nullptr;
    node->addChild(typeSpec);

    auto idToken = consume(ID, "ID");
    if (!idToken) return nullptr;
    node->addChild(idToken);

    auto varDeclPrime = parseVarDeclarationPrime();
    if (!varDeclPrime) return nullptr;
    node->addChild(varDeclPrime);

    return node;
}

// var-declaration' ::= ";" | "[" NUM "]" ";"
shared_ptr<ParseTreeNode> Parser::parseVarDeclarationPrime() {
    auto node = make_shared<NonTerminalNode>("var-declaration'");

    if (match(SEMI)) {
        auto semi = consume(SEMI, ";");
        node->addChild(semi);
    } else if (match(LBRACKET)) {
        auto lbracket = consume(LBRACKET, "[");
        node->addChild(lbracket);

        auto num = consume(NUM, "NUM");
        if (!num) return nullptr;
        node->addChild(num);

        auto rbracket = consume(RBRACKET, "]");
        if (!rbracket) return nullptr;
        node->addChild(rbracket);

        auto semi = consume(SEMI, ";");
        if (!semi) return nullptr;
        node->addChild(semi);
    } else {
        reportError("Expected ';' or '[' in variable declaration");
        return nullptr;
    }

    return node;
}

// type-specifier ::= int | float
shared_ptr<ParseTreeNode> Parser::parseTypeSpecifier() {
    auto node = make_shared<NonTerminalNode>("type-specifier");

    if (match(INT)) {
        auto intToken = consume(INT, "int");
        node->addChild(intToken);
    } else if (match(FLOAT)) {
        auto floatToken = consume(FLOAT, "float");
        node->addChild(floatToken);
    } else {
        reportError("Expected 'int' or 'float'");
        return nullptr;
    }

    return node;
}

// params ::= param-list | "void"
shared_ptr<ParseTreeNode> Parser::parseParams() {
    auto node = make_shared<NonTerminalNode>("params");

    if (match(VOID)) {
        auto voidToken = consume(VOID, "void");
        node->addChild(voidToken);
    } else if (match(INT) || match(FLOAT)) {
        auto paramList = parseParamList();
        if (!paramList) return nullptr;
        node->addChild(paramList);
    } else {
        reportError("Expected parameter list or 'void'");
        return nullptr;
    }

    return node;
}

// param-list ::= param param-list'
shared_ptr<ParseTreeNode> Parser::parseParamList() {
    auto node = make_shared<NonTerminalNode>("param-list");

    auto param = parseParam();
    if (!param) return nullptr;
    node->addChild(param);

    auto paramListPrime = parseParamListPrime();
    if (!paramListPrime) return nullptr;
    node->addChild(paramListPrime);

    return node;
}

// param-list' ::= "," param param-list' | empty
shared_ptr<ParseTreeNode> Parser::parseParamListPrime() {
    auto node = make_shared<NonTerminalNode>("param-list'");

    if (match(COMMA)) {
        auto comma = consume(COMMA, ",");
        node->addChild(comma);

        auto param = parseParam();
        if (!param) return nullptr;
        node->addChild(param);

        auto paramListPrime = parseParamListPrime();
        if (!paramListPrime) return nullptr;
        node->addChild(paramListPrime);
    } else {
        // Empty production
        node->addChild(make_shared<EpsilonNode>());
    }

    return node;
}

// param ::= type-specifier ID param'
shared_ptr<ParseTreeNode> Parser::parseParam() {
    auto node = make_shared<NonTerminalNode>("param");

    auto typeSpec = parseTypeSpecifier();
    if (!typeSpec) return nullptr;
    node->addChild(typeSpec);

    auto idToken = consume(ID, "ID");
    if (!idToken) return nullptr;
    node->addChild(idToken);

    auto paramPrime = parseParamPrime();
    if (!paramPrime) return nullptr;
    node->addChild(paramPrime);

    return node;
}

// param' ::= empty | "[" "]"
shared_ptr<ParseTreeNode> Parser::parseParamPrime() {
    auto node = make_shared<NonTerminalNode>("param'");

    if (match(LBRACKET)) {
        auto lbracket = consume(LBRACKET, "[");
        node->addChild(lbracket);

        auto rbracket = consume(RBRACKET, "]");
        if (!rbracket) return nullptr;
        node->addChild(rbracket);
    } else {
        // Empty production
        node->addChild(make_shared<EpsilonNode>());
    }

    return node;
}

// compound-stmt ::= "{" statement-list "}"
shared_ptr<ParseTreeNode> Parser::parseCompoundStmt() {
    auto node = make_shared<NonTerminalNode>("compound-stmt");

    auto lbrace = consume(LBRACE, "{");
    if (!lbrace) return nullptr;
    node->addChild(lbrace);

    auto stmtList = parseStatementList();
    if (!stmtList) return nullptr;
    node->addChild(stmtList);

    auto rbrace = consume(RBRACE, "}");
    if (!rbrace) return nullptr;
    node->addChild(rbrace);

    return node;
}

// statement-list ::= statement-list'
shared_ptr<ParseTreeNode> Parser::parseStatementList() {
    auto node = make_shared<NonTerminalNode>("statement-list");

    auto stmtListPrime = parseStatementListPrime();
    if (!stmtListPrime) return nullptr;
    node->addChild(stmtListPrime);

    return node;
}

// statement-list' ::= statement statement-list' | empty
shared_ptr<ParseTreeNode> Parser::parseStatementListPrime() {
    auto node = make_shared<NonTerminalNode>("statement-list'");

    // Check if we have a statement (starts with ID, if, while, or {)
    if (match(ID) || match(IF) || match(WHILE) || match(LBRACE)) {
        auto stmt = parseStatement();
        if (!stmt) return nullptr;
        node->addChild(stmt);

        auto stmtListPrime = parseStatementListPrime();
        if (!stmtListPrime) return nullptr;
        node->addChild(stmtListPrime);
    } else {
        // Empty production
        node->addChild(make_shared<EpsilonNode>());
    }

    return node;
}

// statement ::= assignment-stmt | compound-stmt | selection-stmt | iteration-stmt
shared_ptr<ParseTreeNode> Parser::parseStatement() {
    auto node = make_shared<NonTerminalNode>("statement");

    if (match(ID)) {
        auto assignStmt = parseAssignmentStmt();
        if (!assignStmt) return nullptr;
        node->addChild(assignStmt);
    } else if (match(LBRACE)) {
        auto compStmt = parseCompoundStmt();
        if (!compStmt) return nullptr;
        node->addChild(compStmt);
    } else if (match(IF)) {
        auto selStmt = parseSelectionStmt();
        if (!selStmt) return nullptr;
        node->addChild(selStmt);
    } else if (match(WHILE)) {
        auto iterStmt = parseIterationStmt();
        if (!iterStmt) return nullptr;
        node->addChild(iterStmt);
    } else {
        reportError("Expected statement");
        return nullptr;
    }

    return node;
}

// selection-stmt ::= if "(" expression ")" statement selection-stmt'
shared_ptr<ParseTreeNode> Parser::parseSelectionStmt() {
    auto node = make_shared<NonTerminalNode>("selection-stmt");

    auto ifToken = consume(IF, "if");
    if (!ifToken) return nullptr;
    node->addChild(ifToken);

    auto lparen = consume(LPAREN, "(");
    if (!lparen) return nullptr;
    node->addChild(lparen);

    auto expr = parseExpression();
    if (!expr) return nullptr;
    node->addChild(expr);

    auto rparen = consume(RPAREN, ")");
    if (!rparen) return nullptr;
    node->addChild(rparen);

    auto stmt = parseStatement();
    if (!stmt) return nullptr;
    node->addChild(stmt);

    auto selStmtPrime = parseSelectionStmtPrime();
    if (!selStmtPrime) return nullptr;
    node->addChild(selStmtPrime);

    return node;
}

// selection-stmt' ::= empty | else statement
shared_ptr<ParseTreeNode> Parser::parseSelectionStmtPrime() {
    auto node = make_shared<NonTerminalNode>("selection-stmt'");

    if (match(ELSE)) {
        auto elseToken = consume(ELSE, "else");
        node->addChild(elseToken);

        auto stmt = parseStatement();
        if (!stmt) return nullptr;
        node->addChild(stmt);
    } else {
        // Empty production
        node->addChild(make_shared<EpsilonNode>());
    }

    return node;
}

// iteration-stmt ::= while "(" expression ")" statement
shared_ptr<ParseTreeNode> Parser::parseIterationStmt() {
    auto node = make_shared<NonTerminalNode>("iteration-stmt");

    auto whileToken = consume(WHILE, "while");
    if (!whileToken) return nullptr;
    node->addChild(whileToken);

    auto lparen = consume(LPAREN, "(");
    if (!lparen) return nullptr;
    node->addChild(lparen);

    auto expr = parseExpression();
    if (!expr) return nullptr;
    node->addChild(expr);

    auto rparen = consume(RPAREN, ")");
    if (!rparen) return nullptr;
    node->addChild(rparen);

    auto stmt = parseStatement();
    if (!stmt) return nullptr;
    node->addChild(stmt);

    return node;
}

// assignment-stmt ::= var "=" expression
shared_ptr<ParseTreeNode> Parser::parseAssignmentStmt() {
    auto node = make_shared<NonTerminalNode>("assignment-stmt");

    auto varNode = parseVar();
    if (!varNode) return nullptr;
    node->addChild(varNode);

    auto assign = consume(ASSIGN, "=");
    if (!assign) return nullptr;
    node->addChild(assign);

    auto expr = parseExpression();
    if (!expr) return nullptr;
    node->addChild(expr);

    return node;
}

// var ::= ID var'
shared_ptr<ParseTreeNode> Parser::parseVar() {
    auto node = make_shared<NonTerminalNode>("var");

    auto idToken = consume(ID, "ID");
    if (!idToken) return nullptr;
    node->addChild(idToken);

    auto varPrime = parseVarPrime();
    if (!varPrime) return nullptr;
    node->addChild(varPrime);

    return node;
}

// var' ::= empty | "[" expression "]"
shared_ptr<ParseTreeNode> Parser::parseVarPrime() {
    auto node = make_shared<NonTerminalNode>("var'");

    if (match(LBRACKET)) {
        auto lbracket = consume(LBRACKET, "[");
        node->addChild(lbracket);

        auto expr = parseExpression();
        if (!expr) return nullptr;
        node->addChild(expr);

        auto rbracket = consume(RBRACKET, "]");
        if (!rbracket) return nullptr;
        node->addChild(rbracket);
    } else {
        // Empty production
        node->addChild(make_shared<EpsilonNode>());
    }

    return node;
}

// expression ::= additive-expression expression'
shared_ptr<ParseTreeNode> Parser::parseExpression() {
    auto node = make_shared<NonTerminalNode>("expression");

    auto addExpr = parseAdditiveExpression();
    if (!addExpr) return nullptr;
    node->addChild(addExpr);

    auto exprPrime = parseExpressionPrime();
    if (!exprPrime) return nullptr;
    node->addChild(exprPrime);

    return node;
}

// expression' ::= relop additive-expression expression' | empty
shared_ptr<ParseTreeNode> Parser::parseExpressionPrime() {
    auto node = make_shared<NonTerminalNode>("expression'");

    if (match(LT) || match(LTE) || match(GT) || match(GTE) || match(EQ) || match(NEQ)) {
        auto relop = parseRelop();
        if (!relop) return nullptr;
        node->addChild(relop);

        auto addExpr = parseAdditiveExpression();
        if (!addExpr) return nullptr;
        node->addChild(addExpr);

        auto exprPrime = parseExpressionPrime();
        if (!exprPrime) return nullptr;
        node->addChild(exprPrime);
    } else {
        // Empty production
        node->addChild(make_shared<EpsilonNode>());
    }

    return node;
}

// relop ::= "<" | "<=" | ">" | ">=" | "==" | "!="
shared_ptr<ParseTreeNode> Parser::parseRelop() {
    auto node = make_shared<NonTerminalNode>("relop");

    if (match(LT)) {
        node->addChild(consume(LT, "<"));
    } else if (match(LTE)) {
        node->addChild(consume(LTE, "<="));
    } else if (match(GT)) {
        node->addChild(consume(GT, ">"));
    } else if (match(GTE)) {
        node->addChild(consume(GTE, ">="));
    } else if (match(EQ)) {
        node->addChild(consume(EQ, "=="));
    } else if (match(NEQ)) {
        node->addChild(consume(NEQ, "!="));
    } else {
        reportError("Expected relational operator");
        return nullptr;
    }

    return node;
}

// additive-expression ::= term additive-expression'
shared_ptr<ParseTreeNode> Parser::parseAdditiveExpression() {
    auto node = make_shared<NonTerminalNode>("additive-expression");

    auto termNode = parseTerm();
    if (!termNode) return nullptr;
    node->addChild(termNode);

    auto addExprPrime = parseAdditiveExpressionPrime();
    if (!addExprPrime) return nullptr;
    node->addChild(addExprPrime);

    return node;
}

// additive-expression' ::= addop term additive-expression' | empty
shared_ptr<ParseTreeNode> Parser::parseAdditiveExpressionPrime() {
    auto node = make_shared<NonTerminalNode>("additive-expression'");

    if (match(PLUS) || match(MINUS)) {
        auto addop = parseAddop();
        if (!addop) return nullptr;
        node->addChild(addop);

        auto termNode = parseTerm();
        if (!termNode) return nullptr;
        node->addChild(termNode);

        auto addExprPrime = parseAdditiveExpressionPrime();
        if (!addExprPrime) return nullptr;
        node->addChild(addExprPrime);
    } else {
        // Empty production
        node->addChild(make_shared<EpsilonNode>());
    }

    return node;
}

// addop ::= "+" | "-"
shared_ptr<ParseTreeNode> Parser::parseAddop() {
    auto node = make_shared<NonTerminalNode>("addop");

    if (match(PLUS)) {
        node->addChild(consume(PLUS, "+"));
    } else if (match(MINUS)) {
        node->addChild(consume(MINUS, "-"));
    } else {
        reportError("Expected '+' or '-'");
        return nullptr;
    }

    return node;
}

// term ::= factor term'
shared_ptr<ParseTreeNode> Parser::parseTerm() {
    auto node = make_shared<NonTerminalNode>("term");

    auto factorNode = parseFactor();
    if (!factorNode) return nullptr;
    node->addChild(factorNode);

    auto termPrime = parseTermPrime();
    if (!termPrime) return nullptr;
    node->addChild(termPrime);

    return node;
}

// term' ::= mulop factor term' | empty
shared_ptr<ParseTreeNode> Parser::parseTermPrime() {
    auto node = make_shared<NonTerminalNode>("term'");

    if (match(TIMES) || match(DIVIDE)) {
        auto mulop = parseMulop();
        if (!mulop) return nullptr;
        node->addChild(mulop);

        auto factorNode = parseFactor();
        if (!factorNode) return nullptr;
        node->addChild(factorNode);

        auto termPrime = parseTermPrime();
        if (!termPrime) return nullptr;
        node->addChild(termPrime);
    } else {
        // Empty production
        node->addChild(make_shared<EpsilonNode>());
    }

    return node;
}

// mulop ::= "*" | "/"
shared_ptr<ParseTreeNode> Parser::parseMulop() {
    auto node = make_shared<NonTerminalNode>("mulop");

    if (match(TIMES)) {
        node->addChild(consume(TIMES, "*"));
    } else if (match(DIVIDE)) {
        node->addChild(consume(DIVIDE, "/"));
    } else {
        reportError("Expected '*' or '/'");
        return nullptr;
    }

    return node;
}

// factor ::= "(" expression ")" | var | NUM
shared_ptr<ParseTreeNode> Parser::parseFactor() {
    auto node = make_shared<NonTerminalNode>("factor");

    if (match(LPAREN)) {
        auto lparen = consume(LPAREN, "(");
        node->addChild(lparen);

        auto expr = parseExpression();
        if (!expr) return nullptr;
        node->addChild(expr);

        auto rparen = consume(RPAREN, ")");
        if (!rparen) return nullptr;
        node->addChild(rparen);
    } else if (match(ID)) {
        auto varNode = parseVar();
        if (!varNode) return nullptr;
        node->addChild(varNode);
    } else if (match(NUM)) {
        auto num = consume(NUM, "NUM");
        if (!num) return nullptr;
        node->addChild(num);
    } else {
        reportError("Expected '(', identifier, or number");
        return nullptr;
    }

    return node;
}
