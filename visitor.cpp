// visitor.cpp
#include "visitor.h"
#include "exp.h"
#include <iostream>
using namespace std;

//------------------------------------------------------------
// Accept hooks for Exp and Stm (if not already in exp.cpp)
//------------------------------------------------------------

int BinaryExp::accept(Visitor *v)     { return v->visit(this); }
int IFExp::accept(Visitor *v)         { return v->visit(this); }
int NumberExp::accept(Visitor *v)     { return v->visit(this); }
int BoolExp::accept(Visitor *v)       { return v->visit(this); }
int IdentifierExp::accept(Visitor *v) { return v->visit(this); }
int FCallExp::accept(Visitor *v)      { return v->visit(this); }
int ListExp::accept(Visitor *v)       { return v->visit(this); }
int IndexExp::accept(Visitor *v)      { return v->visit(this); }
int DotExp::accept(Visitor *v)        { return v->visit(this); }
int LoopExp::accept(Visitor *v)       { return v->visit(this); }

int AssignStatement::accept(Visitor *v)  { v->visit(this); return 0; }
int PrintStatement::accept(Visitor *v)   { v->visit(this); return 0; }
int IfStatement::accept(Visitor *v)      { v->visit(this); return 0; }
int WhileStatement::accept(Visitor *v)   { v->visit(this); return 0; }
int ForStatement::accept(Visitor *v)     { v->visit(this); return 0; }
int ReturnStatement::accept(Visitor *v)  { v->visit(this); return 0; }

int VarDec::accept(Visitor *v)           { v->visit(this); return 0; }
int VarDecList::accept(Visitor *v)       { v->visit(this); return 0; }
int ClassDec::accept(Visitor *v)         { v->visit(this); return 0; }
int ClassDecList::accept(Visitor *v)     { v->visit(this); return 0; }
int FunDec::accept(Visitor *v)           { v->visit(this); return 0; }
int FunDecList::accept(Visitor *v)       { v->visit(this); return 0; }

int StatementList::accept(Visitor *v)    { v->visit(this); return 0; }
int Body::accept(Visitor *v)             { v->visit(this); return 0; }
int Program::accept(Visitor *v)          { v->visit(this); return 0; }

//----------------------------------------------------------------------
// PrintVisitor implementations
//----------------------------------------------------------------------

void PrintVisitor::imprimir(Program *p) {
  p->accept(this);
  cout << endl;
}

int PrintVisitor::visit(BinaryExp *e) {
  e->left->accept(this);
  cout << " " << Exp::binopToChar(e->op) << " ";
  e->right->accept(this);
  return 0;
}

int PrintVisitor::visit(IFExp *e) {
  cout << "if("; e->cond->accept(this);
  cout << "){";
  e->left->accept(this);
  cout << "}else{";
  e->right->accept(this);
  cout << "}";
  return 0;
}

int PrintVisitor::visit(NumberExp *e)  { cout << e->value; return 0; }
int PrintVisitor::visit(BoolExp *e)    { cout << (e->value ? "true" : "false"); return 0; }
int PrintVisitor::visit(IdentifierExp *e) { cout << e->name; return 0; }

int PrintVisitor::visit(FCallExp *e) {
  cout << e->name << "(";
  for (size_t i = 0; i < e->args.size(); ++i) {
    e->args[i]->accept(this);
    if (i + 1 < e->args.size()) cout << ", ";
  }
  cout << ")";
  return 0;
}

int PrintVisitor::visit(ListExp *e) {
  cout << (e->isMutable ? "mutableListOf(" : "listOf(");
  for (size_t i = 0; i < e->elements.size(); ++i) {
    e->elements[i]->accept(this);
    if (i + 1 < e->elements.size()) cout << ", ";
  }
  cout << ")";
  return 0;
}

int PrintVisitor::visit(IndexExp *e) {
  cout << e->name << "[";
  e->index->accept(this);
  cout << "]";
  return 0;
}

int PrintVisitor::visit(DotExp *e) {
  cout << e->object << "." << e->member;
  return 0;
}

int PrintVisitor::visit(LoopExp *e) {
  e->start->accept(this);
  cout << (e->downTo ? " downTo " : "..");
  e->end->accept(this);
  if (e->step) {
    cout << " step ";
    e->step->accept(this);
  }
  return 0;
}

void PrintVisitor::visit(AssignStatement *s) {
  cout << s->target << " = ";
  s->expr->accept(this);
  cout << ";" << endl;
}

void PrintVisitor::visit(PrintStatement *s) {
  cout << "print("; s->expr->accept(this); cout << ");" << endl;
}

void PrintVisitor::visit(ReturnStatement *s) {
  cout << "return";
  if (s->expr) { cout << " "; s->expr->accept(this); }
  cout << ";" << endl;
}

void PrintVisitor::visit(IfStatement *s) {
  cout << "if "; s->condition->accept(this); cout << " {" << endl;
    s->thenBranch->accept(this);
  cout << "}";
  if (s->elseBranch) {
    cout << " else {" << endl;
    s->elseBranch->accept(this);
    cout << "}";
  }
  cout << endl;
}

void PrintVisitor::visit(WhileStatement *s) {
  cout << "while "; s->condition->accept(this); cout << " {" << endl;
    s->body->accept(this);
  cout << "}" << endl;
}

void PrintVisitor::visit(ForStatement *s) {
  cout << "for(" << s->varName << " in ";
  s->iterable->accept(this);
  cout << ") {" << endl;
    s->body->accept(this);
  cout << "}" << endl;
}

void PrintVisitor::visit(VarDec *d) {
  cout << (d->isMutable ? "var " : "val ")
       << d->typeName << " " << d->name;
  if (d->init) { cout << " = "; d->init->accept(this); }
  cout << ";" << endl;
}

void PrintVisitor::visit(VarDecList *l) {
  for (auto v : l->vars) v->accept(this);
}

void PrintVisitor::visit(ClassDec *c) {
  cout << "class " << c->name << "(";
  for (size_t i = 0; i < c->args.size(); ++i) {
    cout << c->args[i].name << ":" << c->args[i].type;
    if (i + 1 < c->args.size()) cout << ", ";
  }
  cout << ") {" << endl;
    c->members->accept(this);
  cout << "}" << endl;
}

void PrintVisitor::visit(ClassDecList *l) {
  for (auto c : l->classes) c->accept(this);
}

void PrintVisitor::visit(FunDec *f) {
  cout << "fun " << f->name << "(";
  for (size_t i = 0; i < f->params.size(); ++i) {
    cout << f->params[i].name << ":" << f->params[i].type;
    if (i + 1 < f->params.size()) cout << ", ";
  }
  cout << "):" << f->retType << " {" << endl;
    f->body->accept(this);
  cout << "}" << endl;
}

void PrintVisitor::visit(FunDecList *l) {
  for (auto f : l->functions) f->accept(this);
}

void PrintVisitor::visit(StatementList *l) {
  for (auto s : l->statements) s->accept(this);
}

void PrintVisitor::visit(Body *b) {
  b->vardecs->accept(this);
  b->stmts->accept(this);
}

void PrintVisitor::visit(Program *p) {
  p->vardecs->accept(this);
  p->classDecs->accept(this);
  p->funDecs->accept(this);
}

//----------------------------------------------------------------------
// EVALVisitor implementations
//----------------------------------------------------------------------

void EVALVisitor::ejecutar(Program *p) {
  env.add_level();
  p->vardecs->accept(this);
  p->classDecs->accept(this);
  p->funDecs->accept(this);
  auto it = fdecs.find("main");
  if (it == fdecs.end()) {
    cout << "No hay main." << endl;
    exit(1);
  }
  retflag = false;
  it->second->body->accept(this);
  if (!retflag) {
    cout << "Error: main sin return" << endl;
    exit(1);
  }
}

int EVALVisitor::visit(BinaryExp *exp) {
  int v1 = exp->left->accept(this);
  int v2 = exp->right->accept(this);
  switch (exp->op) {
    case PLUS_OP:  return v1 + v2;
    case MINUS_OP: return v1 - v2;
    case MUL_OP:   return v1 * v2;
    case DIV_OP:   return v2!=0 ? v1/v2 : 0;
    case LT_OP:    return v1 < v2;
    case LE_OP:    return v1 <= v2;
    case EQ_OP:    return v1 == v2;
    default:       return 0;
  }
}

int EVALVisitor::visit(NumberExp *exp) { return exp->value; }
int EVALVisitor::visit(BoolExp   *exp) { return exp->value; }

int EVALVisitor::visit(IdentifierExp *exp) {
  if (!env.check(exp->name)) {
    cout << "Variable no declarada: " << exp->name << endl;
    return 0;
  }
  return env.lookup(exp->name);
}

int EVALVisitor::visit(FCallExp *exp) {
  // register function
  auto fn = fdecs.at(exp->name);
  env.add_level();
  for (size_t i = 0; i < exp->args.size(); ++i) {
    int val = exp->args[i]->accept(this);
    env.add_var(fn->params[i].name, fn->params[i].type);
    env.update(fn->params[i].name, val);
  }
  fn->body->accept(this);
  env.remove_level();
  return retval;
}

int EVALVisitor::visit(ListExp *exp) {
  // not implemented
  return 0;
}

int EVALVisitor::visit(IndexExp *exp) {
  // not implemented
  return 0;
}

int EVALVisitor::visit(DotExp *exp) {
  // not implemented
  return 0;
}

int EVALVisitor::visit(LoopExp *exp) {
  // numeric range only
  int start = exp->start->accept(this);
  int end   = exp->end->accept(this);
  int step  = exp->step ? exp->step->accept(this)
                        : (exp->downTo ? -1 : 1);
  // return start as placeholder
  return start;
}

void EVALVisitor::visit(AssignStatement *stm) {
  int val = stm->expr->accept(this);
  if (!env.check(stm->target)) {
    cout << "Variable no declarada: " << stm->target << endl;
    return;
  }
  env.update(stm->target, val);
}

void EVALVisitor::visit(PrintStatement *stm) {
  cout << stm->expr->accept(this) << endl;
}

void EVALVisitor::visit(ReturnStatement *stm) {
  retval  = stm->expr ? stm->expr->accept(this) : 0;
  retflag = true;
}

void EVALVisitor::visit(IfStatement *stm) {
  if (stm->condition->accept(this))
    stm->thenBranch->accept(this);
  else if (stm->elseBranch)
    stm->elseBranch->accept(this);
}

void EVALVisitor::visit(WhileStatement *stm) {
  while (stm->condition->accept(this))
    stm->body->accept(this);
}

void EVALVisitor::visit(ForStatement *stm) {
  // only numeric ranges supported
  if (auto loop = dynamic_cast<LoopExp*>(stm->iterable)) {
    int start = loop->start->accept(this);
    int end   = loop->end->accept(this);
    int step  = loop->step ? loop->step->accept(this)
                          : (loop->downTo ? -1 : 1);
    env.add_level();
    for (int i = start;
         loop->downTo ? i >= end : i <= end;
         i += step)
    {
      env.add_var(stm->varName, "int");
      env.update(stm->varName, i);
      stm->body->accept(this);
    }
    env.remove_level();
  }
}

void EVALVisitor::visit(VarDec *dec) {
  env.add_var(dec->name, dec->typeName);
}

void EVALVisitor::visit(VarDecList *list) {
  for (auto v : list->vars) v->accept(this);
}

void EVALVisitor::visit(ClassDec *dec) {
  // class definitions ignored at runtime
}

void EVALVisitor::visit(ClassDecList *list) {
  for (auto c : list->classes) c->accept(this);
}

void EVALVisitor::visit(FunDec *fn) {
  fdecs[fn->name] = fn;
}

void EVALVisitor::visit(FunDecList *list) {
  for (auto f : list->functions) f->accept(this);
}

void EVALVisitor::visit(StatementList *list) {
  for (auto s : list->statements) s->accept(this);
}

void EVALVisitor::visit(Body *body) {
  env.add_level();
  body->vardecs->accept(this);
  body->stmts->accept(this);
  env.remove_level();
}

void EVALVisitor::visit(Program *prog) {
  ejecutar(prog);
}
