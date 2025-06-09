// parser.cpp
#include "parser.h"
#include <stdexcept>

// --------------------
// Helpers del Parser
// --------------------
Parser::Parser(const std::vector<Token> &tokens)
    : tokens(tokens), current(0) {}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

const Token& Parser::peek() const {
    return tokens[current];
}

const Token& Parser::previous() const {
    return tokens[current - 1];
}

const Token& Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(const std::initializer_list<TokenType>& types) {
    for (auto t : types) {
        if (check(t)) {
            advance();
            return true;
        }
    }
    return false;
}

const Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error("Error en token '" + peek().lexeme + "': " + message);
}

// --------------------
// parseProgram
// --------------------
std::vector<std::unique_ptr<ASTNode>> Parser::parseProgram() {
    std::vector<std::unique_ptr<ASTNode>> program;
    parseVarDecList(program);
    parseClassDecList(program);
    parseFunDecList(program);
    return program;
}

// --------------------
// VarDecList, ClassDecList, FunDecList
// --------------------
void Parser::parseVarDecList(std::vector<std::unique_ptr<ASTNode>>& out) {
    while (match({TokenType::VAR, TokenType::VAL})) {
        out.push_back(parseVarDec());
    }
}

void Parser::parseClassDecList(std::vector<std::unique_ptr<ASTNode>>& out) {
    while (match({TokenType::CLASS})) {
        out.push_back(parseClassDec());
    }
}

void Parser::parseFunDecList(std::vector<std::unique_ptr<ASTNode>>& out) {
    // al menos una función
    do {
        out.push_back(parseFunDec());
    } while (!isAtEnd());
}

// --------------------
// Declaraciones
// --------------------
// VarDec ::= (var|val) id : Type [= Expr] ;
std::unique_ptr<ASTNode> Parser::parseVarDec() {
    bool isConst = previous().type == TokenType::VAL;
    auto nameTok = consume(TokenType::IDENT, "Se esperaba nombre de variable");
    consume(TokenType::COLON, "Se esperaba ':' tras nombre de variable");
    auto typeNode = parseType();
    std::unique_ptr<Expr> init;
    if (match({TokenType::ASSIGN})) {
        init = parseExpression();
    }
    consume(TokenType::SEMICOLON, "Se esperaba ';' tras la declaración");
    return std::make_unique<VarDecl>(
        isConst, nameTok.lexeme, std::move(typeNode), std::move(init)
    );
}

// ClassDec ::= class id() { VarDecList }
std::unique_ptr<ASTNode> Parser::parseClassDec() {
    auto nameTok = consume(TokenType::IDENT, "Se esperaba nombre de struct");
    consume(TokenType::LPAREN, "Se esperaba '(' tras nombre de struct");
    consume(TokenType::RPAREN, "Se esperaba ')'");
    consume(TokenType::LBRACE, "Se esperaba '{' tras struct");
    std::vector<std::unique_ptr<ASTNode>> fields;
    parseVarDecList(fields);
    consume(TokenType::RBRACE, "Se esperaba '}' al final del struct");
    auto sd = std::make_unique<StructDecl>(nameTok.lexeme);
    return sd;
}

// FunDec ::= fun id ( [ParamDecList] ) : Type { Body }
std::unique_ptr<ASTNode> Parser::parseFunDec() {
    auto nameTok = consume(TokenType::IDENT, "Se esperaba nombre de función");
    auto fn = std::make_unique<FunctionDecl>(nameTok.lexeme);

    consume(TokenType::LPAREN, "Se esperaba '(' tras nombre de función");
    if (!check(TokenType::RPAREN)) {
        parseParamDecList(fn->params);
    }
    consume(TokenType::RPAREN, "Se esperaba ')' tras parámetros");

    consume(TokenType::COLON, "Se esperaba ':' antes del tipo de retorno");
    fn->returnType = parseType();

    // parseamos Body = VarDecList + StmtList
    auto stmts = parseBody();
    for (auto &s : stmts) {
        fn->body.push_back(std::move(s));  // unique_ptr<Stmt> → unique_ptr<ASTNode>
    }
    return fn;
}

// --------------------
// Body, ParamDecList, Type
// --------------------
std::vector<std::unique_ptr<Stmt>> Parser::parseBody() {
    std::vector<std::unique_ptr<Stmt>> stmts;
    while (match({TokenType::VAR, TokenType::VAL})) {
        // VarDecl hereda de Stmt
        auto decl = parseVarDec();
        stmts.push_back(
            std::unique_ptr<Stmt>(static_cast<Stmt*>(decl.release()))
        );
    }
    stmts.push_back(parseStmt());
    while (match({TokenType::SEMICOLON})) {
        stmts.push_back(parseStmt());
    }
    return stmts;
}

// ParamDecList ::= id : Type (',' id : Type)*
void Parser::parseParamDecList(
    std::vector<std::pair<std::string,std::unique_ptr<TypeNode>>>& params
) {
    do {
        auto nameTok = consume(TokenType::IDENT, "Se esperaba nombre de parámetro");
        consume(TokenType::COLON, "Se esperaba ':' tras parámetro");
        auto t = parseType();
        params.emplace_back(nameTok.lexeme, std::move(t));
    } while (match({TokenType::COMMA}));
}

// Type ::= IDENT
std::unique_ptr<TypeNode> Parser::parseType() {
    auto tok = consume(TokenType::IDENT, "Se esperaba un tipo");
    return std::make_unique<CustomTypeNode>(tok.lexeme);
}

// --------------------
// Sentencias
// --------------------
std::unique_ptr<Stmt> Parser::parseStmt() {
    if (match({TokenType::IF}))    return parseIfStmt();
    if (match({TokenType::WHILE})) return parseWhileStmt();
    if (match({TokenType::FOR}))   return parseForStmt();
    if (match({TokenType::RETURN}))return parseReturnStmt();

    return parseExprStmt();
}

// if CExp { Body } [ else { Body } ]
std::unique_ptr<Stmt> Parser::parseIfStmt() {
    auto cond = parseExpression();
    consume(TokenType::LBRACE, "Se esperaba '{' tras condición de if");
    auto thenBranch = parseBody();
    std::vector<std::unique_ptr<Stmt>> elseBranch;
    if (match({TokenType::ELSE})) {
        consume(TokenType::LBRACE, "Se esperaba '{' tras else");
        elseBranch = parseBody();
    }
    return std::make_unique<IfStmt>(
        std::move(cond),
        std::move(thenBranch),
        std::move(elseBranch)
    );
}

// while ( CExp ) { Body }
std::unique_ptr<Stmt> Parser::parseWhileStmt() {
    consume(TokenType::LPAREN, "Se esperaba '(' tras while");
    auto cond = parseExpression();
    consume(TokenType::RPAREN, "Se esperaba ')'");
    auto body = parseBody();
    return std::make_unique<WhileStmt>(std::move(cond), std::move(body));
}

std::unique_ptr<Stmt> Parser::parseForStmt() {
    consume(TokenType::LPAREN, "Se esperaba '(' tras 'for'");
    auto idTok = consume(TokenType::IDENT, "Se esperaba variable de bucle");
    std::string varName = idTok.lexeme;

    consume(TokenType::IN, "Se esperaba 'in' en cabecera de for");

    auto expr1 = parseExpression();

    std::vector<std::unique_ptr<Stmt>> body = parseBody();

    if (match({TokenType::RANGE})) {
        auto expr2 = parseExpression();
        return std::make_unique<ForRangeStmt>(
            varName,
            std::move(expr1),
            std::move(expr2),
            std::move(body)
        );
    }

    return std::make_unique<ForEachStmt>(
        varName,
        std::move(expr1),
        std::move(body)
    );
}

std::unique_ptr<Stmt> Parser::parseReturnStmt() {
    std::unique_ptr<Expr> value;
    if (!check(TokenType::SEMICOLON) && !check(TokenType::RBRACE)) {
        value = parseExpression();
    }
    return std::make_unique<ReturnStmt>(std::move(value));
}

// ExprStmt ::= Expr
std::unique_ptr<Stmt> Parser::parseExprStmt() {
    auto e = parseExpression();
    return std::make_unique<ExprStmt>(std::move(e));
}

// --------------------
// Expresiones
// --------------------
std::unique_ptr<Expr> Parser::parseExpression() {
    return parseEquality();
}

std::unique_ptr<Expr> Parser::parseEquality() {
    auto expr = parseComparison();
    while (match({TokenType::EQ, TokenType::NEQ})) {
        auto op = (previous().type == TokenType::EQ ? BinaryOp::EQ : BinaryOp::NEQ);
        auto right = parseComparison();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseComparison() {
    auto expr = parseTerm();
    while (match({TokenType::LT, TokenType::GT, TokenType::LTEQ, TokenType::GTEQ})) {
        TokenType t = previous().type;
        BinaryOp op = (t == TokenType::LT ? BinaryOp::LT
                      : t == TokenType::GT ? BinaryOp::GT
                      : t == TokenType::LTEQ? BinaryOp::LTEQ
                                            : BinaryOp::GTEQ);
        auto right = parseTerm();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseTerm() {
    auto expr = parseFactor();
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        TokenType t = previous().type;
        BinaryOp op = (t == TokenType::PLUS ? BinaryOp::ADD : BinaryOp::SUB);
        auto right = parseFactor();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseFactor() {
    auto expr = parseUnary();
    while (match({TokenType::STAR, TokenType::SLASH, TokenType::PERCENT})) {
        TokenType t = previous().type;
        BinaryOp op = (t == TokenType::STAR ? BinaryOp::MUL
                      : t == TokenType::SLASH? BinaryOp::DIV
                                             : BinaryOp::MOD);
        auto right = parseUnary();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseUnary() {
    if (match({TokenType::MINUS})) {
        auto right = parseUnary();
        auto zero  = std::make_unique<IntLiteral>(0);
        return std::make_unique<BinaryExpr>(BinaryOp::SUB, std::move(zero), std::move(right));
    }
    return parsePrimary();
}

std::unique_ptr<Expr> Parser::parsePrimary() {
    if (match({TokenType::INT_LITERAL})) {
        return std::make_unique<IntLiteral>(std::stoll(previous().lexeme));
    }
    if (match({TokenType::LONG_LITERAL})) {
        return std::make_unique<IntLiteral>(std::stoll(previous().lexeme));
    }
    if (match({TokenType::IDENT})) {
        std::string name = previous().lexeme;
        // llamada
        if (match({TokenType::LPAREN})) {
            auto call = std::make_unique<CallExpr>(name);
            if (!check(TokenType::RPAREN)) {
                do {
                    call->args.push_back(parseExpression());
                } while (match({TokenType::COMMA}));
            }
            consume(TokenType::RPAREN, "Se esperaba ')' en llamada");
            return call;
        }
        if (match({TokenType::LBRACKET})) {
            auto idx = parseExpression();
            consume(TokenType::RBRACKET, "Se esperaba ']' en acceso a arreglo");
            return std::make_unique<ArrayAccessExpr>(
                std::make_unique<VarExpr>(name),
                std::move(idx)
            );
        }
        if (match({TokenType::DOT})) {
            auto fld = consume(TokenType::IDENT, "Se esperaba campo tras '.'");
            return std::make_unique<FieldAccessExpr>(
                std::make_unique<VarExpr>(name),
                fld.lexeme
            );
        }
        return std::make_unique<VarExpr>(name);
    }
    if (match({TokenType::LPAREN})) {
        auto expr = parseExpression();
        consume(TokenType::RPAREN, "Se esperaba ')'");
        return expr;
    }
    if (match({TokenType::ARRAY_KW})) {
        consume(TokenType::LT, "Se esperaba '<' tras 'Array'");
        auto elemType = parseType();
        consume(TokenType::GT, "Se esperaba '>' tras tipo de Array");
        consume(TokenType::LPAREN, "Se esperaba '(' tras Array<...>");
        auto sz = parseExpression();
        consume(TokenType::RPAREN, "Se esperaba ')' tras tamaño de Array");
        return std::make_unique<NewArrayExpr>(std::move(elemType), std::move(sz));
    }

    throw std::runtime_error("Error de sintaxis en expresión en token '" + peek().lexeme + "'");
}
