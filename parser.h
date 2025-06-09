// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "token.h"
#include <vector>
#include <memory>
#include <string>

class Parser {
public:
    explicit Parser(const std::vector<Token> &tokens);
    std::vector<std::unique_ptr<ASTNode>> parseProgram();

private:
    std::vector<Token> tokens;
    size_t current;

    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;
    const Token& advance();
    bool check(TokenType type) const;
    bool match(const std::initializer_list<TokenType>& types);
    const Token& consume(TokenType type, const std::string &msg);

    void parseVarDecList(std::vector<std::unique_ptr<ASTNode>>& out);
    void parseClassDecList(std::vector<std::unique_ptr<ASTNode>>& out);
    void parseFunDecList(std::vector<std::unique_ptr<ASTNode>>& out);

    std::unique_ptr<ASTNode> parseVarDec();
    std::unique_ptr<ASTNode> parseClassDec();
    std::unique_ptr<ASTNode> parseFunDec();
    std::vector<std::unique_ptr<Stmt>> parseBody();
    void parseParamDecList(std::vector<std::pair<std::string,std::unique_ptr<TypeNode>>>& params);
    std::unique_ptr<TypeNode> parseType();

    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Stmt> parseIfStmt();
    std::unique_ptr<Stmt> parseWhileStmt();
    std::unique_ptr<Stmt> parseForStmt();
    std::unique_ptr<Stmt> parseReturnStmt();
    std::unique_ptr<Stmt> parseExprStmt();

    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parseEquality();
    std::unique_ptr<Expr> parseComparison();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();
    std::unique_ptr<Expr> parseUnary();
    std::unique_ptr<Expr> parsePrimary();
};

#endif // PARSER_H