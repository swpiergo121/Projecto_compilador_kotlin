// visitor.h
#ifndef VISITOR_H
#define VISITOR_H

#include "environment.h"
#include "exp.h"
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
  virtual int visit(BinaryExp *exp) = 0;
  virtual int visit(IFExp *exp) = 0;
  virtual int visit(StringExp *exp) = 0;
  virtual int visit(NumberExp *exp) = 0;
  virtual int visit(BoolExp *exp) = 0;
  virtual int visit(IdentifierExp *exp) = 0;
  virtual int visit(FCallExp *exp) = 0;
  virtual int visit(ListExp *exp) = 0;
  virtual int visit(IndexExp *exp) = 0;
  virtual int visit(DotExp *exp) = 0;
  virtual int visit(LoopExp *exp) = 0;

  // Statements / declarations
  virtual void visit(AssignStatement *stm) = 0;
  virtual void visit(PrintStatement *stm) = 0;
  virtual void visit(IfStatement *stm) = 0;
  virtual void visit(WhileStatement *stm) = 0;
  virtual void visit(ForStatement *stm) = 0;
  virtual void visit(ReturnStatement *stm) = 0;

  virtual void visit(VarDec *dec) = 0;
  virtual void visit(VarDecList *list) = 0;
  virtual void visit(ClassDec *dec) = 0;
  virtual void visit(ClassDecList *list) = 0;
  virtual void visit(FunDec *dec) = 0;
  virtual void visit(FunDecList *list) = 0;

  virtual void visit(StatementList *list) = 0;
  virtual void visit(Body *body) = 0;
  virtual void visit(Program *prog) = 0;

  virtual ~Visitor() = default;
};

//----------------------------------------------------------------------
// PrintVisitor: “pretty prints” the AST to stdout
//----------------------------------------------------------------------

class PrintVisitor : public Visitor {
public:
  int indent = 0;
  int step = 2;

  void imprimir(Program *program);

  int visit(BinaryExp *exp) override;
  int visit(IFExp *exp) override;
  int visit(StringExp *exp) override;
  int visit(NumberExp *exp) override;
  int visit(BoolExp *exp) override;
  int visit(IdentifierExp *exp) override;
  int visit(FCallExp *exp) override;
  int visit(ListExp *exp) override;
  int visit(IndexExp *exp) override;
  int visit(DotExp *exp) override;
  int visit(LoopExp *exp) override;

  void visit(AssignStatement *stm) override;
  void visit(PrintStatement *stm) override;
  void visit(IfStatement *stm) override;
  void visit(WhileStatement *stm) override;
  void visit(ForStatement *stm) override;
  void visit(ReturnStatement *stm) override;

  void visit(VarDec *dec) override;
  void visit(VarDecList *list) override;
  void visit(ClassDec *dec) override;
  void visit(ClassDecList *list) override;
  void visit(FunDec *dec) override;
  void visit(FunDecList *list) override;

  void visit(StatementList *list) override;
  void visit(Body *body) override;
  void visit(Program *prog) override;
};

//----------------------------------------------------------------------
// EVALVisitor: interprets the AST in-memory (for testing)
//----------------------------------------------------------------------

class EVALVisitor : public Visitor {
  Environment env;
  std::unordered_map<std::string, FunDec *> fdecs;
  int retval;
  // Heap interno para literales de lista
  std::unordered_map<int, std::vector<int>> listHeap;
  std::unordered_map<int, std::vector<std::string>> stringListHeap;
  int nextListId = 1;

  // Para asignar IDs únicos a cada instancia
  int nextObjectId = 1;
  // Para saber el orden de los campos de cada clase
  // class ->
  std::unordered_map<std::string, std::vector<std::string>> classFields_;
  std::unordered_map<std::string, std::vector<Exp *>>
      classFieldInits_; // nombre de clase → expresiones iniciales
  // “Heap” de objetos: objectId -> map(campo -> valor)
  std::unordered_map<int, std::unordered_map<std::string, int>> objectHeap;

public:
  void ejecutar(Program *program);

  int visit(BinaryExp *exp) override;
  int visit(IFExp *exp) override;
  int visit(StringExp *exp) override;
  int visit(NumberExp *exp) override;
  int visit(BoolExp *exp) override;
  int visit(IdentifierExp *exp) override;
  int visit(FCallExp *exp) override;
  int visit(ListExp *exp) override;
  int visit(IndexExp *exp) override;
  int visit(DotExp *exp) override;
  int visit(LoopExp *exp) override;

  void visit(AssignStatement *stm) override;
  void visit(PrintStatement *stm) override;
  void visit(IfStatement *stm) override;
  void visit(WhileStatement *stm) override;
  void visit(ForStatement *stm) override;
  void visit(ReturnStatement *stm) override;

  void visit(VarDec *dec) override;
  void visit(VarDecList *list) override;
  void visit(ClassDec *dec) override;
  void visit(ClassDecList *list) override;
  void visit(FunDec *dec) override;
  void visit(FunDecList *list) override;

  void visit(StatementList *list) override;
  void visit(Body *body) override;
  void visit(Program *prog) override;
};

//----------------------------------------------------------------------
// GenCodeVisitor: genera ensamblador x86-64 recorriendo el AST
//----------------------------------------------------------------------

template <typename T> class GenCodeVisitor : public Visitor {
public:
  GenCodeVisitor(T &out);

  // Lanza la generación: .data, .text, prologue/epilogue y recorre el programa
  void generate(Program *prog);

  // – Expresiones
  int visit(BinaryExp *exp) override;
  int visit(IFExp *exp) override;
  int visit(StringExp *exp) override;
  int visit(NumberExp *exp) override;
  int visit(BoolExp *exp) override;
  int visit(IdentifierExp *exp) override;
  int visit(FCallExp *exp) override;
  int visit(ListExp *exp) override;
  int visit(IndexExp *exp) override;
  int visit(DotExp *exp) override;
  int visit(LoopExp *exp) override;

  // – Sentencias / declaraciones
  void visit(AssignStatement *stm) override;
  void visit(PrintStatement *stm) override;
  void visit(IfStatement *stm) override;
  void visit(WhileStatement *stm) override;
  void visit(ForStatement *stm) override;
  void visit(ReturnStatement *stm) override;

  void visit(VarDec *dec) override;
  void visit(VarDecList *list) override;
  void visit(ClassDec *dec) override;
  void visit(ClassDecList *list) override;
  void visit(FunDec *dec) override;
  void visit(FunDecList *list) override;

  void visit(StatementList *list) override;
  void visit(Body *body) override;
  void visit(Program *prog) override;

private:
  T &out_;
  stringstream data;
  stringstream text;
  int stackSize_ = 0;
  int labelCount_ = 0;
  bool inGlobal_ = false;
  bool collectingStrings_ = false;
  string nombreFuncion = "";
  int stackFor_ = 0;

  // Needs to free the memory of lists

  // Maps for variables
  unordered_map<string, int> memoria;
  std::unordered_map<std::string, int>
      memoriaIndex_; // para los índices de los for
  unordered_map<string, bool> memoriaGlobal;
  std::unordered_map<std::string, std::string> memoriaTypes_;

  // Maps for class declarations
  // class -> field -> offset
  // The offset is for the location in heap
  std::unordered_map<std::string, std::unordered_map<std::string, int>>
      structLayouts_;
  // class -> field -> type
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
      structFieldTypes_;
  // class -> field -> Exp*
  // Saves the default values for the fields
  std::unordered_map<std::string, std::unordered_map<std::string, Exp *>>
      structFieldInits_;
  // class -> arguments
  // Saves the order for the passes arguments for the constructor
  std::unordered_map<std::string, std::vector<std::string>>
      structFieldConstructorsOrder_;

  std::string newLabel(const std::string &prefix);

  // para strings:
  std::unordered_map<std::string, std::string> stringLabel_; // literal -> label
  // para longitudes de listas:
  std::unordered_map<std::string, int> listLength_; // varName -> n

  // Mapa nuevo: nombre de lista global → su ListExp*
  unordered_map<string, ListExp *> globalInits_;

  // Mapa para booleans
  unordered_map<std::string, int> elemSize_;
  unordered_set<std::string> booleanArrs_;
};

#endif // VISITOR_H
