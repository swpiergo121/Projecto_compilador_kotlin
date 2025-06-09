#ifndef VISITOR_H
#define VISITOR_H
#include "environment.h"
#include "exp.h"
#include <list>
#include <unordered_map>

class BinaryExp;
class NumberExp;
class BoolExp;
class IFExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class WhileStatement;
class VarDec;
class VarDecList;
class StatementList;
class Body;
class FCallExp;
class FunDec;
class FunDecList;
class ClassDec;
class ClassDecList;
class ReturnStatement;
class Program;

class Visitor {
public:
  virtual int visit(IFExp *exp) = 0;
  virtual int visit(BinaryExp *exp) = 0;
  virtual int visit(NumberExp *exp) = 0;
  virtual int visit(BoolExp *exp) = 0;
  virtual int visit(IdentifierExp *exp) = 0;
  virtual int visit(FCallExp *exp) = 0;
  virtual void visit(ReturnStatement *stm) = 0;
  virtual void visit(FunDec *f) = 0;
  virtual void visit(FunDecList *f) = 0;
  virtual void visit(AssignStatement *stm) = 0;
  virtual void visit(PrintStatement *stm) = 0;
  virtual void visit(IfStatement *stm) = 0;
  virtual void visit(WhileStatement *stm) = 0;
  virtual void visit(VarDec *stm) = 0;
  virtual void visit(VarDecList *stm) = 0;
  virtual void visit(ClassDec *stm) = 0;
  virtual void visit(ClassDecList *stm) = 0;
  virtual void visit(StatementList *stm) = 0;
  virtual void visit(Body *b) = 0;
};

class PrintVisitor : public Visitor {
public:
  int indentacion = 0;
  int spaces = 4;

  void imprimir(Program *program);
  int visit(IFExp *exp) override;
  int visit(BinaryExp *exp) override;
  int visit(NumberExp *exp) override;
  int visit(BoolExp *exp) override;
  int visit(FCallExp *exp) override;
  void visit(ReturnStatement *stm) override;
  void visit(FunDec *f) override;
  void visit(FunDecList *f) override;
  int visit(IdentifierExp *exp) override;
  void visit(AssignStatement *stm) override;
  void visit(PrintStatement *stm) override;
  void visit(IfStatement *stm) override;
  void visit(WhileStatement *stm) override;
  void visit(VarDec *stm) override;
  void visit(VarDecList *stm) override;
  void visit(ClassDec *stm) override;
  void visit(ClassDecList *stm) override;
  void visit(StatementList *stm) override;
  void visit(Body *b) override;
};

class EVALVisitor : public Visitor {
  Environment env;
  unordered_map<string, FunDec *> fdecs;
  unordered_map<string, ClassDec *> cdecs;
  int retval;
  int retcall;

public:
  void ejecutar(Program *program);
  int visit(IFExp *exp) override;
  int visit(BinaryExp *exp) override;
  int visit(NumberExp *exp) override;
  int visit(BoolExp *exp) override;
  int visit(IdentifierExp *exp) override;
  void visit(AssignStatement *stm) override;
  void visit(PrintStatement *stm) override;
  int visit(FCallExp *exp) override;
  void visit(ReturnStatement *stm) override;
  void visit(FunDec *f) override;
  void visit(FunDecList *f) override;
  void visit(IfStatement *stm) override;
  void visit(WhileStatement *stm) override;
  void visit(VarDec *stm) override;
  void visit(VarDecList *stm) override;
  void visit(ClassDec *stm) override;
  void visit(ClassDecList *stm) override;
  void visit(StatementList *stm) override;
  void visit(Body *b) override;
};

#endif // VISITOR_H
