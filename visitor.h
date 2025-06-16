// visitor.h
#ifndef VISITOR_H
#define VISITOR_H

#include "environment.h"
#include "exp.h"
#include <unordered_map>
#include <string>

// Forward declarations of all AST node types
class Program;
class VarDecList;
class ClassDecList;
class FunDecList;
class VarDec;
class ClassDec;
class FunDec;
class Body;
class StatementList;

class Stm;
class AssignStatement;
class PrintStatement;
class IfStatement;
class WhileStatement;
class ForStatement;
class ReturnStatement;

class Exp;
class BinaryExp;
class IFExp;
class StringExp;
class NumberExp;
class BoolExp;
class IdentifierExp;
class FCallExp;
class ListExp;
class IndexExp;
class DotExp;
class LoopExp;

// Visitor interface
class Visitor {
public:
  // Expressions
  virtual int  visit(BinaryExp     *exp) = 0;
  virtual int  visit(IFExp         *exp) = 0;
  virtual int  visit(StringExp     *exp) = 0;
  virtual int  visit(NumberExp     *exp) = 0;
  virtual int  visit(BoolExp       *exp) = 0;
  virtual int  visit(IdentifierExp *exp) = 0;
  virtual int  visit(FCallExp      *exp) = 0;
  virtual int  visit(ListExp       *exp) = 0;
  virtual int  visit(IndexExp      *exp) = 0;
  virtual int  visit(DotExp        *exp) = 0;
  virtual int  visit(LoopExp       *exp) = 0;

  // Statements / declarations
  virtual void visit(AssignStatement *stm) = 0;
  virtual void visit(PrintStatement  *stm) = 0;
  virtual void visit(IfStatement     *stm) = 0;
  virtual void visit(WhileStatement  *stm) = 0;
  virtual void visit(ForStatement    *stm) = 0;
  virtual void visit(ReturnStatement *stm) = 0;

  virtual void visit(VarDec       *dec) = 0;
  virtual void visit(VarDecList   *list)= 0;
  virtual void visit(ClassDec     *dec) = 0;
  virtual void visit(ClassDecList *list)= 0;
  virtual void visit(FunDec       *dec) = 0;
  virtual void visit(FunDecList   *list)= 0;

  virtual void visit(StatementList *list)= 0;
  virtual void visit(Body          *body)= 0;
  virtual void visit(Program       *prog)= 0;

  virtual ~Visitor() = default;
};

//----------------------------------------------------------------------
// PrintVisitor: “pretty prints” the AST to stdout
//----------------------------------------------------------------------

class PrintVisitor : public Visitor {
public:
  int indent = 0;
  int step   = 2;

  void imprimir(Program *program);

  int  visit(BinaryExp     *exp) override;
  int  visit(IFExp         *exp) override;
  int  visit(StringExp     *exp) override;
  int  visit(NumberExp     *exp) override;
  int  visit(BoolExp       *exp) override;
  int  visit(IdentifierExp *exp) override;
  int  visit(FCallExp      *exp) override;
  int  visit(ListExp       *exp) override;
  int  visit(IndexExp      *exp) override;
  int  visit(DotExp        *exp) override;
  int  visit(LoopExp       *exp) override;

  void visit(AssignStatement *stm) override;
  void visit(PrintStatement  *stm) override;
  void visit(IfStatement     *stm) override;
  void visit(WhileStatement  *stm) override;
  void visit(ForStatement    *stm) override;
  void visit(ReturnStatement *stm) override;

  void visit(VarDec       *dec) override;
  void visit(VarDecList   *list)override;
  void visit(ClassDec     *dec) override;
  void visit(ClassDecList *list)override;
  void visit(FunDec       *dec) override;
  void visit(FunDecList   *list)override;

  void visit(StatementList *list)override;
  void visit(Body          *body)override;
  void visit(Program       *prog)override;
};

//----------------------------------------------------------------------
// EVALVisitor: interprets the AST in-memory (for testing)
//----------------------------------------------------------------------

class EVALVisitor : public Visitor {
  Environment                     env;
  std::unordered_map<std::string, FunDec*> fdecs;
  int                              retval;
  bool                             retflag;
  // Heap interno para literales de lista
  std::unordered_map<int, std::vector<int>> listHeap;
  std::unordered_map<int, std::vector<std::string>> stringListHeap;
  int nextListId = 1;

public:
  void ejecutar(Program *program);

  int  visit(BinaryExp     *exp) override;
  int  visit(IFExp         *exp) override;
  int  visit(StringExp     *exp) override;
  int  visit(NumberExp     *exp) override;
  int  visit(BoolExp       *exp) override;
  int  visit(IdentifierExp *exp) override;
  int  visit(FCallExp      *exp) override;
  int  visit(ListExp       *exp) override;
  int  visit(IndexExp      *exp) override;
  int  visit(DotExp        *exp) override;
  int  visit(LoopExp       *exp) override;

  void visit(AssignStatement *stm) override;
  void visit(PrintStatement  *stm) override;
  void visit(IfStatement     *stm) override;
  void visit(WhileStatement  *stm) override;
  void visit(ForStatement    *stm) override;
  void visit(ReturnStatement *stm) override;

  void visit(VarDec       *dec) override;
  void visit(VarDecList   *list)override;
  void visit(ClassDec     *dec) override;
  void visit(ClassDecList *list)override;
  void visit(FunDec       *dec) override;
  void visit(FunDecList   *list)override;

  void visit(StatementList *list)override;
  void visit(Body          *body)override;
  void visit(Program       *prog)override;
};

//----------------------------------------------------------------------
// GenCodeVisitor: genera ensamblador x86-64 recorriendo el AST
//----------------------------------------------------------------------

class GenCodeVisitor : public Visitor {
public:
  explicit GenCodeVisitor(std::ostream& out);

  // Lanza la generación: .data, .text, prologue/epilogue y recorre el programa
  void generate(Program* prog);

  // Reimplementación de todos los visit() de Visitor
  // – Expresiones
  int visit(BinaryExp     *exp) override;
  int visit(IFExp         *exp) override;
  int visit(StringExp     *exp) override;
  int visit(NumberExp     *exp) override;
  int visit(BoolExp       *exp) override;
  int visit(IdentifierExp *exp) override;
  int visit(FCallExp      *exp) override;
  int visit(ListExp       *exp) override;
  int visit(IndexExp      *exp) override;
  int visit(DotExp        *exp) override;
  int visit(LoopExp       *exp) override;

  // – Sentencias / declaraciones
  void visit(AssignStatement *stm) override;
  void visit(PrintStatement  *stm) override;
  void visit(IfStatement     *stm) override;
  void visit(WhileStatement  *stm) override;
  void visit(ForStatement    *stm) override;
  void visit(ReturnStatement *stm) override;

  void visit(VarDec       *dec) override;
  void visit(VarDecList   *list)override;
  void visit(ClassDec     *dec) override;
  void visit(ClassDecList *list)override;
  void visit(FunDec       *dec) override;
  void visit(FunDecList   *list)override;

  void visit(StatementList *list)override;
  void visit(Body          *body)override;
  void visit(Program       *prog)override;

private:
  std::ostream& out_;
  std::unordered_map<std::string,int> symtab_;
  int stackSize_ = 0;
  int labelCount_ = 0;
  bool inGlobal_ = false;
  std::vector<VarDec*> globalVars_; 
  bool collectingStrings_ = false;

  std::unordered_map<std::string, std::string> varTypes_;
  std::unordered_map<std::string,std::unordered_map<std::string,int>> structLayouts_;
  std::string newLabel(const std::string& prefix);

  // para strings en listOf:
  std::unordered_map<std::string, std::string> stringLabel_;     // literal → label
  // para longitudes:
  std::unordered_map<std::string, int> listLength_;     // varName → n

};


#endif // VISITOR_H
