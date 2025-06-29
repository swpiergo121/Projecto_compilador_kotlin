// exp.cpp
#include "exp.h"
#include "visitor.h"

using namespace std;

Program::Program(Body *b)
    : vardecs(nullptr), classDecs(nullptr), funDecs(nullptr), body(b) {}

Program::~Program() {
  delete vardecs;
  delete classDecs;
  delete funDecs;
}

// VarDecList
VarDecList::VarDecList() {}
VarDecList::~VarDecList() {
  for (auto v : vars)
    delete v;
}
void VarDecList::add(VarDec *var) { vars.push_back(var); }

// ClassDecList
ClassDecList::ClassDecList() {}
ClassDecList::~ClassDecList() {
  for (auto c : classes)
    delete c;
}
void ClassDecList::add(ClassDec *cls) { classes.push_back(cls); }

// FunDecList
FunDecList::FunDecList() {}
FunDecList::~FunDecList() {
  for (auto f : functions)
    delete f;
}
void FunDecList::add(FunDec *fn) { functions.push_back(fn); }

// VarDec
VarDec::VarDec(bool isMutable_, const vector<string> &names_,
               const string typeName_, vector<Exp *> &inits_)
    : isMutable(isMutable_), names(names_), typeName(typeName_), inits(inits_) {
}

// ClassDec
ClassDec::ClassDec(const string &name, const vector<Argument> &args,
                   VarDecList *members)
    : name(name), args(args), members(members) {}

// FunDec
FunDec::FunDec(const string &name, const string &retType,
               const vector<Param> &params, Body *body)
    : name(name), retType(retType), params(params), body(body) {}

// Body
Body::Body(VarDecList *vardecs, StatementList *stmts)
    : vardecs(vardecs), stmts(stmts) {}

// StatementList
StatementList::StatementList() {}
StatementList::~StatementList() {
  for (auto s : statements)
    delete s;
}
void StatementList::add(Stm *stmt) { statements.push_back(stmt); }

// Stm base
Stm::~Stm() {}

// AssignStatement
AssignStatement::AssignStatement(Exp *target, Exp *expr)
    : target(target), expr(expr) {}

AssignStatement::~AssignStatement() {
  delete target;
  delete expr;
}
// PrintStatement
PrintStatement::PrintStatement(Exp *expr) : expr(expr) {}

// ReturnStatement
ReturnStatement::ReturnStatement(Exp *expr) : expr(expr) {}

// IfStatement
IfStatement::IfStatement(Exp *cond, Body *thenBranch, Body *elseBranch)
    : cond(cond), thenBranch(thenBranch), elseBranch(elseBranch) {}

// WhileStatement
WhileStatement::WhileStatement(Exp *cond, Body *body)
    : cond(cond), body(body) {}

// ForStatement
ForStatement::ForStatement(const string &varName, Exp *iterable, Body *body)
    : varName(varName), iterable(iterable), body(body) {}

// BinaryExp
BinaryExp::BinaryExp(Exp *left, Exp *right, BinaryOp op)
    : left(left), right(right), op(op) {}

// IFExp
IFExp::IFExp(Exp *cond, Exp *left, Exp *right)
    : cond(cond), left(left), right(right) {}
// NumberExp
NumberExp::NumberExp(int value) : value(value) {}

// BoolExp
BoolExp::BoolExp(bool value) : value(value) {}

// IdentifierExp
IdentifierExp::IdentifierExp(const string &name) : name(name) {}

// FCallExp
FCallExp::FCallExp(const string &name) : name(name) {}
void FCallExp::add(Exp *arg) { args.push_back(arg); }

// ListExp
ListExp::ListExp(bool isMutable) : isMutable(isMutable) {}
void ListExp::add(Exp *elem) { elements.push_back(elem); }

// IndexExp
IndexExp::IndexExp(const string &name, Exp *index) : name(name), index(index) {}

// DotExp
DotExp::DotExp(Exp* object, const std::string &member)
  : object(object), member(member) {}

// LoopExp
LoopExp::LoopExp(Exp *start, Exp *end, Exp *step, bool downTo)
    : start(start), end(end), step(step), downTo(downTo) {}

int StringExp::accept(Visitor *v) { return v->visit(this); }

// Exp base
Exp::~Exp() {}

std::string Exp::binopToChar(int op) {
  switch (op) {
  case PLUS_OP:
    return "+";
  case MINUS_OP:
    return "-";
  case MUL_OP:
    return "*";
  case DIV_OP:
    return "/";
  case LT_OP:
    return "<";
  case GT_OP:
    return ">";
  default:
    return "?";
  }
}

// Expressions
IFExp::~IFExp() {
  delete cond;
  delete left;
  delete right;
}
BinaryExp::~BinaryExp() {
  delete left;
  delete right;
}
NumberExp::~NumberExp() {}
BoolExp::~BoolExp() {}
IdentifierExp::~IdentifierExp() {}
FCallExp::~FCallExp() {
  for (auto a : args)
    delete a;
}
ListExp::~ListExp() {
  for (auto e : elements)
    delete e;
}
IndexExp::~IndexExp() { delete index; }
DotExp::~DotExp() {
  delete object;
}
LoopExp::~LoopExp() {
  delete start;
  delete end;
  if (step)
    delete step;
}

// Statements
PrintStatement::~PrintStatement() { delete expr; }
ReturnStatement::~ReturnStatement() {
  if (expr)
    delete expr;
}
IfStatement::~IfStatement() {
  delete cond;
  delete thenBranch;
  if (elseBranch)
    delete elseBranch;
}
WhileStatement::~WhileStatement() {
  delete cond;
  delete body;
}
ForStatement::~ForStatement() {
  delete iterable;
  delete body;
}

// Declarations
VarDec::~VarDec() {
  if (inits.size() > 0) {
    for (int i = 0; i < inits.size(); i++) {
      delete inits[i];
    }
  }
}
ClassDec::~ClassDec() { delete members; }
FunDec::~FunDec() { delete body; }
Body::~Body() {
  delete vardecs;
  delete stmts;
}
