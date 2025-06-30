// exp.h
#ifndef EXP_H
#define EXP_H

#include <string>
#include <vector>

using namespace std;

// Forward-declaration del Visitor
class Visitor;

// -----------------------------------------------------------------------------
// Nodo base de expresiones
// -----------------------------------------------------------------------------
class Exp {
public:
  virtual int accept(Visitor *v) = 0;
  virtual ~Exp();
  static std::string binopToChar(int op);
};

enum BinaryOp {
  PLUS_OP,
  MINUS_OP,
  MUL_OP,
  DIV_OP,
  LT_OP,
  GE_OP,
  GT_OP,
  LE_OP,
  EQ_OP
};

class IFExp : public Exp {
public:
  Exp *cond;
  Exp *left;
  Exp *right;
  IFExp(Exp *cond, Exp *left, Exp *right);
  int accept(Visitor *v) override;
  ~IFExp();
};

class BinaryExp : public Exp {
public:
  Exp *left;
  Exp *right;
  BinaryOp op;
  BinaryExp(Exp *left, Exp *right, BinaryOp op);
  int accept(Visitor *v) override;
  ~BinaryExp();
};

class StringExp : public Exp {
public:
  std::string value;
  StringExp(const std::string &value) : value(value) {}
  int accept(Visitor *v) override;
  ~StringExp() override {}
};

class NumberExp : public Exp {
public:
  long long value;
  NumberExp(long long value);
  int accept(Visitor *v) override;
  ~NumberExp();
};

class BoolExp : public Exp {
public:
  bool value;
  BoolExp(bool value);
  int accept(Visitor *v) override;
  ~BoolExp();
};

class IdentifierExp : public Exp {
public:
  std::string name;
  IdentifierExp(const std::string &name);
  int accept(Visitor *v) override;
  ~IdentifierExp();
};

class FCallExp : public Exp {
public:
  std::string name;
  std::vector<Exp *> args;
  FCallExp(const std::string &name);
  void add(Exp *arg);
  int accept(Visitor *v) override;
  ~FCallExp();
};

class ListExp : public Exp {
public:
  bool isMutable;
  std::vector<Exp *> elements;
  ListExp(bool isMutable);
  void add(Exp *elem);
  int accept(Visitor *v) override;
  ~ListExp();
};

class IndexExp : public Exp {
public:
  std::string name;
  Exp *index;
  Exp *listExpr;
  IndexExp(const std::string &name, Exp *index);
  int accept(Visitor *v) override;
  ~IndexExp();
};

class DotExp : public Exp {
public:
  Exp *object;
  std::string member;
  DotExp(Exp *object, const std::string &member);
  int accept(Visitor *v) override;
  ~DotExp();
};

class LoopExp : public Exp {
public:
  Exp *start;
  Exp *end;
  Exp *step; // puede ser nullptr
  bool downTo;
  LoopExp(Exp *start, Exp *end, Exp *step, bool downTo);
  int accept(Visitor *v) override;
  ~LoopExp();
};

// -----------------------------------------------------------------------------
// Nodo base de sentencias
// -----------------------------------------------------------------------------
class Stm {
public:
  virtual int accept(Visitor *v) = 0;
  virtual ~Stm();
};

class AssignStatement : public Stm {
public:
  Exp *target;
  Exp *expr;
  AssignStatement(Exp *target, Exp *expr);
  int accept(Visitor *v) override;
  ~AssignStatement();
};

class PrintStatement : public Stm {
public:
  Exp *expr;
  PrintStatement(Exp *expr);
  int accept(Visitor *v) override;
  ~PrintStatement();
};

class ReturnStatement : public Stm {
public:
  Exp *expr; // puede ser nullptr
  ReturnStatement(Exp *expr);
  int accept(Visitor *v) override;
  ~ReturnStatement();
};

class IfStatement : public Stm {
public:
  Exp *cond;
  class Body *thenBranch;
  class Body *elseBranch; // puede ser nullptr
  IfStatement(Exp *condition, Body *thenBranch, Body *elseBranch);
  int accept(Visitor *v) override;
  ~IfStatement();
};

class WhileStatement : public Stm {
public:
  Exp *cond;
  class Body *body;
  WhileStatement(Exp *condition, Body *body);
  int accept(Visitor *v) override;
  ~WhileStatement();
};

class ForStatement : public Stm {
public:
  std::string varName;
  Exp *iterable;
  class Body *body;
  ForStatement(const std::string &varName, Exp *iterable, Body *body);
  int accept(Visitor *v) override;
  ~ForStatement();
};

// -----------------------------------------------------------------------------
// Declaraciones y listas de declaraciones
// -----------------------------------------------------------------------------
struct Param {
  std::string name;
  std::string type;
};

struct Argument {
  std::string name;
  std::string type;
};

class VarDec {
public:
  bool isMutable;
  vector<string> names;
  string typeName;
  vector<Exp *> inits;
  VarDec(bool isMutable_, const vector<string> &names_, const string typeNames_,
         vector<Exp *> &inits_);
  int accept(Visitor *v);
  ~VarDec();
};

class VarDecList {
public:
  std::vector<VarDec *> vars;
  VarDecList();
  void add(VarDec *var);
  int accept(Visitor *v);
  ~VarDecList();
};

class ClassDec {
public:
  std::string name;
  std::vector<Argument> args;
  VarDecList *members;
  ClassDec(const std::string &name, const std::vector<Argument> &args,
           VarDecList *members);
  int accept(Visitor *v);
  ~ClassDec();
};

class ClassDecList {
public:
  std::vector<ClassDec *> classes;
  ClassDecList();
  void add(ClassDec *cls);
  int accept(Visitor *v);
  ~ClassDecList();
};

class FunDec {
public:
  std::string name;
  std::string retType;
  std::vector<Param> params;
  class Body *body;
  FunDec(const std::string &name, const std::string &retType,
         const std::vector<Param> &params, Body *body);
  int accept(Visitor *v);
  ~FunDec();
};

class FunDecList {
public:
  std::vector<FunDec *> functions;
  FunDecList();
  void add(FunDec *fn);
  int accept(Visitor *v);
  ~FunDecList();
};

class StatementList {
public:
  std::vector<Stm *> statements;
  StatementList();
  void add(Stm *stmt);
  int accept(Visitor *v);
  ~StatementList();
};

// -----------------------------------------------------------------------------
// Body y Program
// -----------------------------------------------------------------------------
class Body {
public:
  VarDecList *vardecs;
  StatementList *stmts;
  Body(VarDecList *vardecs, StatementList *stmts);
  int accept(Visitor *v);
  ~Body();
};

class Program {
public:
  VarDecList *vardecs;
  ClassDecList *classDecs;
  FunDecList *funDecs;
  Body *body;
  Program(Body *body);
  Program();
  int accept(Visitor *v);
  ~Program();
};

#endif // EXP_H
