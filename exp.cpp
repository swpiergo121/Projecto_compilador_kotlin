// exp.cpp
#include "exp.h"

// Program
Program::Program() : vardecs(nullptr), classDecs(nullptr), funDecs(nullptr) {}
Program::~Program() {
    delete vardecs;
    delete classDecs;
    delete funDecs;
}

// VarDecList
VarDecList::VarDecList() {}
VarDecList::~VarDecList() {
    for (auto v : vars) delete v;
}
void VarDecList::add(VarDec* var) {
    vars.push_back(var);
}

// ClassDecList
ClassDecList::ClassDecList() {}
ClassDecList::~ClassDecList() {
    for (auto c : classes) delete c;
}
void ClassDecList::add(ClassDec* cls) {
    classes.push_back(cls);
}

// FunDecList
FunDecList::FunDecList() {}
FunDecList::~FunDecList() {
    for (auto f : functions) delete f;
}
void FunDecList::add(FunDec* fn) {
    functions.push_back(fn);
}

// VarDec
VarDec::VarDec(bool isMutable, const string& name, const string& typeName, Exp* init)
  : isMutable(isMutable), name(name), typeName(typeName), init(init) {}

// ClassDec
ClassDec::ClassDec(const string& name, const vector<Argument>& args, VarDecList* members)
  : name(name), args(args), members(members) {}

// FunDec
FunDec::FunDec(const string& name, const string& retType, const vector<Param>& params, Body* body)
  : name(name), retType(retType), params(params), body(body) {}

// Body
Body::Body(VarDecList* vardecs, StatementList* stmts)
  : vardecs(vardecs), stmts(stmts) {}

// StatementList
StatementList::StatementList() {}
StatementList::~StatementList() {
    for (auto s : statements) delete s;
}
void StatementList::add(Stm* stmt) {
    statements.push_back(stmt);
}

// Stm base
Stm::~Stm() {}

// AssignStatement
AssignStatement::AssignStatement(const string& target, Exp* expr)
  : target(target), expr(expr) {}

// PrintStatement
PrintStatement::PrintStatement(Exp* expr)
  : expr(expr) {}

// ReturnStatement
ReturnStatement::ReturnStatement(Exp* expr)
  : expr(expr) {}

// IfStatement
IfStatement::IfStatement(Exp* cond, Body* thenBranch, Body* elseBranch)
  : cond(cond), thenBranch(thenBranch), elseBranch(elseBranch) {}

// WhileStatement
WhileStatement::WhileStatement(Exp* cond, Body* body)
  : cond(cond), body(body) {}

// ForStatement
ForStatement::ForStatement(const string& varName, Exp* iterable, Body* body)
  : varName(varName), iterable(iterable), body(body) {}

// Exp base
Exp::~Exp() {}

// BinaryExp
BinaryExp::BinaryExp(Exp* left, Exp* right, int op)
  : left(left), right(right), op(op) {}

// IFExp
IFExp::IFExp(Exp* cond, Exp* thenExp, Exp* elseExp)
  : cond(cond), thenExp(thenExp), elseExp(elseExp) {}

// NumberExp
NumberExp::NumberExp(int value)
  : value(value) {}

// BoolExp
BoolExp::BoolExp(bool value)
  : value(value) {}

// IdentifierExp
IdentifierExp::IdentifierExp(const string& name)
  : name(name) {}

// FCallExp
FCallExp::FCallExp(const string& name)
  : name(name) {}
void FCallExp::add(Exp* arg) {
    args.push_back(arg);
}

// ListExp
ListExp::ListExp(bool isMutable)
  : isMutable(isMutable) {}
void ListExp::add(Exp* elem) {
    elements.push_back(elem);
}

// IndexExp
IndexExp::IndexExp(const string& name, Exp* index)
  : name(name), index(index) {}

// DotExp
DotExp::DotExp(const string& object, const string& member)
  : object(object), member(member) {}

// LoopExp
LoopExp::LoopExp(Exp* start, Exp* end, Exp* step, bool downTo)
  : start(start), end(end), step(step), downTo(downTo) {}
