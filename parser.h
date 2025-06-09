#ifndef PARSER_H
#define PARSER_H

#include "exp.h"
#include "scanner.h"

class Parser {
private:
  Scanner *scanner;
  Token *current, *previous;
  bool match(Token::Type ttype);
  bool check(Token::Type ttype);
  bool advance();
  bool isAtEnd();
  list<Stm *> parseStmList();
  Exp *parseCExp();
  Exp *parseExpression();
  Exp *parseTerm();
  Exp *parseFactor();

public:
  Parser(Scanner *scanner);
  Program *parseProgram();
  Stm *parseStatement();
  StatementList *parseStatementList();
  VarDec *parseVarDec();
  VarDecList *parseVarDecList();
  ClassDec *parseClassDec();
  ClassDecList *parseClassDecList();
  Body *parseBody();
  FunDecList *parseFunDecList();
  FunDec *parseFunDec();
};

#endif // PARSER_H
