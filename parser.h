// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "token.h"
#include "exp.h"        // tus nodos AST: Program, VarDecList, ClassDecList, FunDecList, etc.
#include <vector>
#include <string>

class Parser {
public:
    explicit Parser(Scanner* scanner);
    Program* parse();      // inicia el análisis sintáctico
    Program* parseProgram();

private:
    Scanner* scanner;
    Token*   current;
    Token*   previous;

    // --- Helpers básicos ---
    bool      match(Token::Type type);
    bool      match(Token::Type type, const std::string& lexeme);
    bool      check(Token::Type type) const;
    Token*    advance();
    Token*    consume(Token::Type type, const std::string& message);
    bool      isAtEnd() const;
    void      error(const std::string& msg);

    // --- Producciones de la gramática ---
    VarDecList* parseVarDecList();
    ClassDecList* parseClassDecList();
    FunDecList* parseFunDecList();

    VarDec* parseVarDec();        // var/val id : Type [ = CExp ]
    ClassDec* parseClassDec();      // class id ( Arguments ) { VarDecList }
    FunDec* parseFunDec();        // fun Type id ( ParamDecList ) [ VarDecList StmtList ] endfun

    std::vector<Param>* parseParamDecList(); // id : Type (, id : Type )*
    std::vector<Argument>* parseArguments();    // val id : Type (, val id : Type )*

    Body* parseBody();          // VarDecList StmtList
    StatementList* parseStmtList();      // Stmt ( ; Stmt )*
    Stm* parseStmt();          // cada tipo de sentencia

    Exp* parseCExp();          // Exp [(<|<=|==) Exp]
    Exp* parseExpression();    // Term ((+|-) Term)*
    Exp* parseTerm();          // Factor ((*|/) Factor)*
    Exp* parseFactor();        // literales, llamadas, paréntesis, if-exp, index, dot, lista

    std::vector<Exp*> parseArgList();       // CExp (, CExp)*
    ListExp* parseListExp();       // listOf(...) | mutableListOf(...)
    LoopExp* parseLoopExp();       // CExp .. CExp [ step CExp ] | downTo

    // para el for:
    // for ( id in (LoopExp | id) ) { Body }
};

#endif // PARSER_H
