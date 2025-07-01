// visitor.cpp
#include "visitor.h"
#include "exp.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>

using namespace std;

//------------------------------------------------------------
// Accept hooks for Exp and Stm (if not already in exp.cpp)
//------------------------------------------------------------

int BinaryExp::accept(Visitor *v) { return v->visit(this); }
int IFExp::accept(Visitor *v) { return v->visit(this); }
int NumberExp::accept(Visitor *v) { return v->visit(this); }
int BoolExp::accept(Visitor *v) { return v->visit(this); }
int IdentifierExp::accept(Visitor *v) { return v->visit(this); }
int FCallExp::accept(Visitor *v) { return v->visit(this); }
int ListExp::accept(Visitor *v) { return v->visit(this); }
int IndexExp::accept(Visitor *v) { return v->visit(this); }
int DotExp::accept(Visitor *v) { return v->visit(this); }
int LoopExp::accept(Visitor *v) { return v->visit(this); }

int AssignStatement::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int PrintStatement::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int IfStatement::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int WhileStatement::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int ForStatement::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int ReturnStatement::accept(Visitor *v) {
  v->visit(this);
  return 0;
}

int VarDec::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int VarDecList::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int ClassDec::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int ClassDecList::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int FunDec::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int FunDecList::accept(Visitor *v) {
  v->visit(this);
  return 0;
}

int StatementList::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int Body::accept(Visitor *v) {
  v->visit(this);
  return 0;
}
int Program::accept(Visitor *v) {
  v->visit(this);
  return 0;
}

//----------------------------------------------------------------------
// PrintVisitor implementations
//----------------------------------------------------------------------

void PrintVisitor::imprimir(Program *p) {
  p->accept(this);
  cout << endl;
}

int PrintVisitor::visit(BinaryExp *e) {
  cout << "---- left: " << endl;
  e->left->accept(this);
  cout << " " << Exp::binopToChar(e->op) << " ";
  cout << "---- right: " << endl;
  e->right->accept(this);
  return 0;
}

int PrintVisitor::visit(IFExp *e) {
  cout << "if(";
  e->cond->accept(this);
  cout << "){";
  e->left->accept(this);
  cout << "}else{";
  e->right->accept(this);
  cout << "}";
  return 0;
}

int PrintVisitor::visit(StringExp *exp) {
  cout << '"' << exp->value << '"';
  return 0;
}

int PrintVisitor::visit(NumberExp *e) {
  cout << e->value;
  return 0;
}

int PrintVisitor::visit(BoolExp *e) {
  cout << (e->value ? "true" : "false");
  return 0;
}
int PrintVisitor::visit(IdentifierExp *e) {
  cout << e->name;
  return 0;
}

int PrintVisitor::visit(FCallExp *e) {
  cout << e->name << "(";
  for (size_t i = 0; i < e->args.size(); ++i) {
    e->args[i]->accept(this);
    if (i + 1 < e->args.size())
      cout << ", ";
  }
  cout << ")";
  return 0;
}

int PrintVisitor::visit(ListExp *e) {
  cout << (e->isMutable ? "mutableListOf(" : "listOf(");
  for (size_t i = 0; i < e->elements.size(); ++i) {
    e->elements[i]->accept(this);
    if (i + 1 < e->elements.size())
      cout << ", ";
  }
  cout << ")";
  return 0;
}

int PrintVisitor::visit(IndexExp *e) {
  // imprime algo como: lista[indice]
  cout << e->name << "[";
  e->index->accept(this);
  cout << "]";
  return 0;
}

int PrintVisitor::visit(DotExp *e) {
  cout << e->id << "." << e->member;
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
  cout << "print(";
  s->expr->accept(this);
  cout << ");" << endl;
}

void PrintVisitor::visit(ReturnStatement *s) {
  cout << "return";
  if (s->expr) {
    cout << " ";
    s->expr->accept(this);
  }
  cout << ";" << endl;
}

void PrintVisitor::visit(IfStatement *s) {
  cout << "if ";
  cout << "--- cond" << endl;
  s->cond->accept(this);
  cout << " {" << endl;
  cout << "--- then branch" << endl;
  s->thenBranch->accept(this);
  cout << "}";
  cout << "--- else branch" << endl;
  if (s->elseBranch) {
    cout << " else {" << endl;
    s->elseBranch->accept(this);
    cout << "}";
  }
  cout << endl;
}

void PrintVisitor::visit(WhileStatement *s) {
  cout << "while ";
  s->cond->accept(this);
  cout << " {" << endl;
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
  cout << (d->isMutable ? "var " : "val ") << d->names[0];
  for (int i = 1; i < d->names.size(); i++) {
    cout << ", " << d->names[i];
  }

  cout << ": " << d->typeName;

  if (d->inits.size() > 0) {
    cout << " = ";
    d->inits[0]->accept(this);
    for (int i = 1; i < d->inits.size(); i++) {
      cout << ", " << d->names[i];
      d->inits[i]->accept(this);
    }
  }

  cout << endl;
}

void PrintVisitor::visit(VarDecList *list) {
  for (auto dec : list->vars)
    dec->accept(this);
}

void PrintVisitor::visit(ClassDec *c) {
  cout << "class " << c->name << "(";
  for (size_t i = 0; i < c->args.size(); ++i) {
    cout << c->args[i].name << ":" << c->args[i].type;
    if (i + 1 < c->args.size())
      cout << ", ";
  }
  cout << ") {" << endl;
  c->members->accept(this);
  cout << "}" << endl;
}

void PrintVisitor::visit(ClassDecList *l) {
  for (auto c : l->classes)
    c->accept(this);
}

void PrintVisitor::visit(FunDec *f) {
  cout << "fun " << f->name << "(";
  for (size_t i = 0; i < f->params.size(); ++i) {
    cout << f->params[i].name << ":" << f->params[i].type;
    if (i + 1 < f->params.size())
      cout << ", ";
  }
  cout << "):" << f->retType << " {" << endl;
  f->body->accept(this);
  cout << "}" << endl;
}

void PrintVisitor::visit(FunDecList *l) {
  for (auto f : l->functions)
    f->accept(this);
}

void PrintVisitor::visit(StatementList *l) {
  for (auto s : l->statements)
    s->accept(this);
}

void PrintVisitor::visit(Body *b) {
  b->vardecs->accept(this);
  b->stmts->accept(this);
}

void PrintVisitor::visit(Program *p) {
  if (p->body)
    p->body->accept(this);
}

//----------------------------------------------------------------------
// EVALVisitor implementations
//----------------------------------------------------------------------

void EVALVisitor::ejecutar(Program *p) {
  // ── 0) Registrar todas las clases (campos e inits) ──
  if (p->classDecs) {
    for (auto *cd : p->classDecs->classes) {
      // 0.a) Campos (ya lo hace tu visit(ClassDec*), pero podemos rellenarlo
      // aquí con seguridad)
      std::vector<std::string> flds;
      for (auto *vd : cd->members->vars)
        for (auto &nm : vd->names)
          flds.push_back(nm);
      classFields_[cd->name] = std::move(flds);

      // 0.b) Inits por defecto
      std::vector<Exp *> inits;
      for (auto *vd : cd->members->vars)
        for (auto *e : vd->inits)
          inits.push_back(e);
      classFieldInits_[cd->name] = std::move(inits);
    }
  }
  // 1) Nuevo scope (global)
  env.add_level();

  // 2) Registra variables globales declaradas antes de 'fun'
  if (p->vardecs) {
    p->vardecs->accept(this);
  }

  // 3) Registra todas las funciones (llenado de fdecs)
  if (p->funDecs) {
    p->funDecs->accept(this);
  }

  p->body->accept(this);

  // 5) Sal del scope global
  env.remove_level();
}

int EVALVisitor::visit(BinaryExp *exp) {
  int v1 = exp->left->accept(this);
  int v2 = exp->right->accept(this);
  switch (exp->op) {
  case PLUS_OP:
    return v1 + v2;
  case MINUS_OP:
    return v1 - v2;
  case MUL_OP:
    return v1 * v2;
  case DIV_OP:
    return v2 != 0 ? v1 / v2 : 0;
  case LT_OP:
    return v1 < v2;
  case GT_OP:
    return v1 > v2;
  case LE_OP:
    return v1 <= v2;
  case EQ_OP:
    return v1 == v2;
  default:
    return 0;
  }
}

int EVALVisitor::visit(StringExp *exp) { return 0; }

int EVALVisitor::visit(NumberExp *exp) { return exp->value; }
int EVALVisitor::visit(BoolExp *exp) { return exp->value; }

int EVALVisitor::visit(IdentifierExp *exp) {
  if (!env.check(exp->name)) {
    cout << "Variable no declarada: " << exp->name << endl;
    return 0;
  }
  int r = env.lookup(exp->name);
  return env.lookup(exp->name);
}

int EVALVisitor::visit(FCallExp *exp) {
  auto itFields = classFields_.find(exp->name);
  if (itFields != classFields_.end()) {
    int objId = nextObjectId++;
    auto &fields = itFields->second;
    auto &inits = classFieldInits_[exp->name];

    for (size_t i = 0; i < fields.size(); ++i) {
      int val;
      if (i < exp->args.size())
        val = exp->args[i]->accept(this);
      else
        val = inits[i]->accept(this);
      objectHeap[objId][fields[i]] = val;
    }
    retval = objId;
    return objId;
  }

  // 1) Built-in: print / println
  if (exp->name == "print" || exp->name == "println") {
    for (auto arg : exp->args) {
      // 1.a) String literal directo
      if (auto se = dynamic_cast<StringExp *>(arg)) {
        cout << se->value;
        continue;
      }
      // 1.b) IndexExp sobre lista de strings
      if (auto ie = dynamic_cast<IndexExp *>(arg)) {
        int listId = env.lookup(ie->name);
        int idx = ie->index->accept(this);
        // si existe en el heap de strings
        if (stringListHeap.count(listId)) {
          cout << stringListHeap[listId][idx];
          continue;
        }
        // si no, cae a imprimir como entero abajo
      }
      // 1.c) cualquier otro caso: interpreta como Int
      cout << arg->accept(this);
    }
    if (exp->name == "println")
      cout << "\n";
    return 0;
  }

  // BUILTIN para arrays de enteros y objetos
  if (exp->name == "arrayOf" || exp->name == "intArrayOf" ||
      exp->name == "longArrayOf" || exp->name == "doubleArrayOf" ||
      exp->name == "booleanArrayOf") {
    std::vector<int> vals;
    for (auto *arg : exp->args)
      vals.push_back(arg->accept(this));
    int id = nextListId++;
    listHeap[id] = std::move(vals);
    return id;
  }

  // 2) Llamada a función del usuario
  auto it = fdecs.find(exp->name);
  if (it == fdecs.end()) {
    cerr << "Error: función no declarada: " << exp->name << endl;
    exit(1);
  }
  FunDec *fn = it->second;

  // 3) Evaluar todos los argumentos como Int
  vector<int> vals;
  for (auto arg : exp->args) {
    vals.push_back(arg->accept(this));
  }

  // 4) Nuevo scope para parámetros
  env.add_level();
  for (size_t i = 0; i < fn->params.size(); ++i) {
    env.add_var(fn->params[i].name, fn->params[i].type);
    env.update(fn->params[i].name, vals[i]);
  }

  // 5) Ejecutar cuerpo de la función
  fn->body->accept(this);
  int result = retval;

  // 6) Salir del scope
  env.remove_level();
  return result;
}

// Lista literal: construye un vector<int>, lo guarda en listHeap y devuelve su
// ID
int EVALVisitor::visit(ListExp *exp) {
  // Si el primer elemento es StringExp hacemos lista de strings
  if (!exp->elements.empty() &&
      dynamic_cast<StringExp *>(exp->elements[0]) != nullptr) {
    std::vector<std::string> svals;
    for (auto e : exp->elements) {
      auto se = dynamic_cast<StringExp *>(e);
      svals.push_back(se->value);
    }
    int id = nextListId++;
    stringListHeap[id] = std::move(svals);
    return id;
  }
  // Si no, lista de enteros
  std::vector<int> ivals;
  for (auto e : exp->elements) {
    ivals.push_back(e->accept(this));
  }
  int id = nextListId++;
  listHeap[id] = std::move(ivals);
  return id;
}

int EVALVisitor::visit(IndexExp *exp) {
  // 1) obtenemos el id de la lista
  int listId = env.lookup(exp->name);
  int idx = exp->index->accept(this);

  // 2) si existe en stringListHeap, devolvemos un índice inválido (no se usa
  // aquí)
  if (stringListHeap.count(listId)) {
    auto &svec = stringListHeap[listId];
    if (idx < 0 || idx >= (int)svec.size()) {
      std::cerr << "Error: índice fuera de rango en " << exp->name << ": "
                << idx << "\n";
      exit(1);
    }
    return idx;
  }
  // 3) si está en listHeap, devolvemos el valor int
  auto &vec = listHeap.at(listId);
  if (idx < 0 || idx >= (int)vec.size()) {
    std::cerr << "Error: índice fuera de rango en " << exp->name << ": " << idx
              << "\n";
    exit(1);
  }
  return vec[idx];
}

int EVALVisitor::visit(DotExp *exp) {
  // 1) Evaluar la sub-expresión que produce el objectId
  int objId = env.lookup(exp->id);

  // 2) Buscar el campo en objectHeap[objId]
  auto &objMap = objectHeap[objId];
  auto it = objMap.find(exp->member);
  if (it == objMap.end()) {
    std::cerr << "Error: campo '" << exp->member
              << "' no existe en el objeto\n";
    std::exit(1);
  }
  retval = it->second;
  return it->second;
}

int EVALVisitor::visit(IFExp *exp) {
  if (exp->cond->accept(this))
    return exp->left->accept(this);
  else
    return exp->right->accept(this);
}

int EVALVisitor::visit(LoopExp *exp) {
  // numeric range only
  int start = exp->start->accept(this);
  int end = exp->end->accept(this);
  int step = exp->step ? exp->step->accept(this) : (exp->downTo ? -1 : 1);
  // return start as placeholder
  return start;
}

void EVALVisitor::visit(AssignStatement *stm) {
  // 1) Evaluar RHS
  int val = stm->expr->accept(this);

  // 2) Asignación a variable simple
  if (auto id = dynamic_cast<IdentifierExp *>(stm->target)) {
    const std::string &name = id->name;
    if (!env.check(name)) {
      std::cerr << "Variable no declarada: " << name << "\n";
      return;
    }
    env.update(name, val);
    return;
  }

  // 3) Asignación a array[index]
  if (auto idx = dynamic_cast<IndexExp *>(stm->target)) {
    // asumimos que la variable es un array de enteros en listHeap[id]
    int arrId = env.lookup(idx->name);
    int i = idx->index->accept(this);
    listHeap[arrId][i] = val;
    return;
  }

  // 4) Asignación a struct.field
  if (auto dot = dynamic_cast<DotExp *>(stm->target)) {
    int objId = env.lookup(dot->id);
    objectHeap[objId][dot->member] = val;
    return;
  }
  // 5) Cualquier otro LHS no es válido
  std::cerr << "Asignación inválida en LHS no identificador\n";
}

void EVALVisitor::visit(PrintStatement *stm) {
  Exp *e = stm->expr;

  // 1) String literal directo
  if (auto se = dynamic_cast<StringExp *>(e)) {
    cout << se->value;

    // 2) IndexExp sobre lista de strings
  } else if (auto ie = dynamic_cast<IndexExp *>(e)) {
    int listId = env.lookup(ie->name);
    int idx = ie->index->accept(this);
    auto it = stringListHeap.find(listId);
    if (it != stringListHeap.end()) {
      // imprimimos el elemento string
      cout << it->second[idx];
    } else {
      // cae en lista de ints: usamos el valor numérico
      cout << ie->accept(this);
    }

    // 3) Cualquier otro caso: lo tratamos como Int
  } else {
    cout << e->accept(this);
  }

  cout << endl;
}

void EVALVisitor::visit(ReturnStatement *stm) {
  retval = stm->expr ? stm->expr->accept(this) : 0;
}

void EVALVisitor::visit(IfStatement *stm) {
  if (stm->cond->accept(this))
    stm->thenBranch->accept(this);
  else if (stm->elseBranch)
    stm->elseBranch->accept(this);
}

void EVALVisitor::visit(WhileStatement *stm) {
  while (stm->cond->accept(this))
    stm->body->accept(this);
}

void EVALVisitor::visit(ForStatement *stm) {
  // only numeric ranges supported
  if (auto loop = dynamic_cast<LoopExp *>(stm->iterable)) {
    int start = loop->start->accept(this);
    int end = loop->end->accept(this);
    int step = loop->step ? loop->step->accept(this) : (loop->downTo ? -1 : 1);
    env.add_level();
    for (int i = start; loop->downTo ? i >= end : i <= end; i += step) {
      env.add_var(stm->varName, "int");
      env.update(stm->varName, i);
      stm->body->accept(this);
    }
    env.remove_level();
  }
}

void EVALVisitor::visit(VarDec *dec) {
  for (size_t i = 0; i < dec->names.size(); ++i) {
    const string &nm = dec->names[i];
    string tname = dec->typeName;
    // inferir List<Int> si viene de listOf()
    if (tname.empty() && i < dec->inits.size() && dec->inits[i]) {
      if (auto fc = dynamic_cast<FCallExp *>(dec->inits[i])) {
        if (fc->name == "listOf" || fc->name == "mutableListOf")
          tname = "List<Int>";
      }
    }
    // si tiene init, lo evaluamos y registramos con valor
    if (i < dec->inits.size() && dec->inits[i]) {
      int v = dec->inits[i]->accept(this);
      env.add_var(nm, v, tname);
    }
    // si no, valor por defecto 0
    else {
      env.add_var(nm, tname);
    }
  }
}

void EVALVisitor::visit(VarDecList *list) {
  for (auto dec : list->vars)
    dec->accept(this);
}

void EVALVisitor::visit(ClassDec *dec) {
  vector<string> fields;
  for (auto &arg : dec->args)
    fields.push_back(arg.name);
  for (auto *vdl : dec->members->vars)
    for (auto &nm : vdl->names)
      fields.push_back(nm);
  classFields_[dec->name] = move(fields);
}

void EVALVisitor::visit(ClassDecList *cdl) {
  for (auto c : cdl->classes) {
    classFields_[c->name] = vector<string>();
    for (auto arg : c->args)
      classFields_[c->name].push_back(arg.name);
    c->members->accept(this);
  }
}

void EVALVisitor::visit(FunDec *fn) { fdecs[fn->name] = fn; }

void EVALVisitor::visit(FunDecList *list) {
  for (auto f : list->functions) {
    fdecs[f->name] = f;
  }
}

void EVALVisitor::visit(StatementList *list) {
  for (auto s : list->statements)
    s->accept(this);
}

void EVALVisitor::visit(Body *body) {
  env.add_level();
  body->vardecs->accept(this);
  body->stmts->accept(this);
  env.remove_level();
}

void EVALVisitor::visit(Program *prog) { ejecutar(prog); }

//----------------------------------------------------------------------
// Constructor
template <typename T> GenCodeVisitor<T>::GenCodeVisitor(T &out) : out_(out) {}

// Generación principal
template <typename T> void GenCodeVisitor<T>::generate(Program *prog) {
  prog->accept(this);
}

// Etiquetas únicas
template <typename T>
std::string GenCodeVisitor<T>::newLabel(const std::string &prefix) {
  return prefix + std::to_string(labelCount_++);
}

// ── Expresiones ──

template <typename T> int GenCodeVisitor<T>::visit(StringExp *e) {
  // 1) genera un label único en .rodata con el literal
  std::string lbl = newLabel("str");
  text << lbl << ": .string \"" << e->value << "\"\n";
  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(NumberExp *e) {
  text << "  movq $" << e->value << ", %rax\n";
  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(BoolExp *e) {
  text << "  movq $" << (e->value ? 1 : 0) << ", %rax\n";
  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(IdentifierExp *e) {
  if (memoria.count(e->name)) {
    int off = memoria.at(e->name);
    text << "  movq " << off << "(%rbp), %rax\n";
  } else {
    // variable global
    text << "  movq " << e->name << "(%rip), %rax\n";
  }
  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(BinaryExp *e) {
  // Evalúa la izquierda, deja en %rax
  e->left->accept(this);
  // Guarda %rax en la pila
  text << "  pushq %rax\n";
  // Evalúa la derecha, deja en %rax
  e->right->accept(this);
  // Recupera izquierda de la pila a %rcx
  text << " movq %rax, %rcx\n popq %rax\n";

  switch (e->op) {
  case PLUS_OP:
    text << "  addq %rcx, %rax\n";
    break;
  case MINUS_OP:
    text << "  subq %rcx, %rax\n"; // %rax = %rax - %rcx
    break;
  case MUL_OP:
    text << "  imulq %rcx, %rax\n";
    break;
  case DIV_OP:
    text << "  cqto\n";
    text << "  idivq %rcx\n"; // divide %rdx:%rax entre %rax (denominador)
    break;
  case LT_OP:
    text << " cmpq %rcx, %rax\n"
         << " movl $0, %eax\n"
         << " setl %al\n"
         << " movzbq %al, %rax\n";
    break;
  case LE_OP:
    text << " cmpq %rcx, %rax\n"
         << " movl $0, %eax\n"
         << " setle %al\n"
         << " movzbq %al, %rax\n";
    break;
  case GT_OP:
    text << " cmpq %rcx, %rax\n"
         << " movl $0, %eax\n"
         << " setg %al\n"
         << " movzbq %al, %rax\n";
    break;
  case GE_OP:
    text << " cmpq %rcx, %rax\n"
         << " movl $0, %eax\n"
         << " setge %al\n"
         << " movzbq %al, %rax\n";
    break;
  case EQ_OP:
    text << "  cmpq %rcx, %rax\n"
            "  sete %al\n"
            "  movzbq %al, %rax\n";
    break;
  default:
    break;
  }
  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(IndexExp *e) {
  // 1) Evaluar el índice → %rax
  e->index->accept(this);

  // 2) Cargar la dirección base del array en %rbx
  if (memoria.count(e->name)) {
    int off = memoria.at(e->name);
    text << "  lea " << off << "(%rbp), %rbx\n"; // &var
    text << "  movq (%rbx), %rbx\n";             // ptr heap
  } else {
    text << "  movq " << e->name << "(%rip), %rbx\n"; // global
  }

  // 3) Determinar tamaño de elemento (por defecto 8)
  int esz = 8;
  auto itSize = elemSize_.find(e->name);
  if (itSize != elemSize_.end())
    esz = itSize->second;

  // 4) Indexación distinta para booleanArrayOf (1 byte) vs resto
  if (booleanArrs_.count(e->name)) {
    // offset = idx * 1
    text << "  lea    (%rbx,%rax,1), %rbx\n";
    text << "  movzbq (%rbx), %rax\n";
  } else {
    if (esz == 8) {
      text << "  salq $3, %rax\n";
    } else {
      text << "  imulq  $" << esz << ", %rax" << esz << "\n";
    }
    text << "  addq   %rax, %rbx\n";
    text << "  movq   (%rbx), %rax\n";
  }

  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(DotExp *exp) {
  // 1) puntero al objeto anidado
  int off = memoria.at(exp->id);
  text << "  movq " << off << "(%rbp), %rax\n";
  text << "  movq %rax, %rbx\n";

  // 2) averiguar el tipo de ese objeto
  std::string objType = memoriaTypes_.at(exp->id);

  // 3) sumar offset del campo en esa clase
  int fldOff = structLayouts_.at(objType).at(exp->member);
  if (fldOff != 0) {
    text << "  addq $" << fldOff << ", %rbx\n";
  }

  // 4) cargar campo
  text << "  movq (%rbx), %rax\n";
  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(LoopExp *exp) {
  // Generación mínima: evalúa el inicio y devuelve un valor ficticio
  exp->start->accept(this);
  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(IFExp *e) {
  auto Lelse = newLabel("Lelse");
  auto Lend = newLabel("Lend");
  // condición
  e->cond->accept(this);
  text << "  cmpq $0, %rax\n";
  text << "  je " << Lelse << "\n";
  // then
  e->left->accept(this);
  text << "  jmp " << Lend << "\n";
  text << Lelse << ":\n";
  // else
  e->right->accept(this);
  text << Lend << ":\n";
  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(ListExp *e) {
  size_t n = e->elements.size();

  // ── PRIMERA PASADA: recolección de literales sólo de strings
  if (collectingStrings_) {
    if (n > 0 && dynamic_cast<StringExp *>(e->elements[0])) {
      for (auto *el : e->elements) {
        auto txt = static_cast<StringExp *>(el)->value;
        if (!stringLabel_.count(txt))
          stringLabel_[txt] = newLabel("str");
      }
    }
    return 0;
  }

  // ── SEGUNDA PASADA: generación real dentro de .text

  // A) Lista de String: malloc de punteros + llenar con labels
  if (n > 0 && dynamic_cast<StringExp *>(e->elements[0])) {
    text << "  movq $" << (n * 8) << ", %rdi\n"
         << "  call malloc@PLT\n"
         << "  movq %rax, %rbx\n";
    for (size_t i = 0; i < n; ++i) {
      auto txt = static_cast<StringExp *>(e->elements[i])->value;
      auto lbl = stringLabel_[txt];
      text << "  leaq " << lbl << "(%rip), %rax\n"
           << "  movq %rax, " << (i * 8) << "(%rbx)\n";
    }
  } else {

    // B) Lista de Int: malloc de 8*n bytes + almacenar cada entero
    text << "  movq $" << (n * 8) << ", %rdi\n"
         << "  call malloc@PLT\n"
         << "  pushq %rax\n";
    for (size_t i = 0; i < n; ++i) {
      text << "  pushq %rax\n";
      e->elements[i]->accept(this); // → %rax = valor entero
      text << "  movq %rax, %rcx\n";
      // Pops to have the rax pointer saved at the start
      text << "  popq %rax\n";
      // Uses rax to access the index values
      text << "  movq %rcx, " << (i * 8) << "(%rax)\n";
    }

    text << "  popq %rax\n";
  }
  return 0;
}

template <typename T> int GenCodeVisitor<T>::visit(FCallExp *e) {
  // 0) Constructor de struct/clase si existe layout
  auto it = structLayouts_.find(e->name);
  if (it != structLayouts_.end()) {
    // número de campos
    // 1. Sum number of variables and number of variables declared
    int nFields = structLayouts_[e->name].size();
    // 2. Reserver memory
    text << "  movq $" << (nFields * 8) << ", %rdi\n"
         << "  call malloc@PLT\n"
         << "  pushq %rax\n";
    // 3. Unload the args, assume the args are only for the constructor,
    // so the args of call and class are equal Then go for variables
    // llenar cada campo: args[i] → offset i*8
    int nFieldsConstructor = structFieldConstructorsOrder_.size();
    for (size_t i = 0; i < nFieldsConstructor; ++i) {
      string fieldName = structFieldConstructorsOrder_[e->name][i];
      int offField = structLayouts_[e->name][fieldName];
      text << "  pushq %rax\n";
      e->args[i]->accept(this); // → %rax = valor de arg
      // Saves result in rcx
      text << "  movq %rax, %rcx\n";
      // Pops to have the rax pointer saved at the start
      text << "  popq %rax\n";
      // Uses rax to access the index values
      text << "  movq %rcx, " << offField << "(%rax)\n";
    }

    // 4. Init variables
    for (auto v : structFieldInits_[e->name]) {
      int offField = structLayouts_[e->name][v.first];
      text << "  pushq %rax\n";
      v.second->accept(this); // → %rax = valor de arg
      // Saves result in rcx
      text << "  movq %rax, %rcx\n";
      // Pops to have the rax pointer saved at the start
      text << "  popq %rax\n";
      // Uses rax to access the index values
      text << "  movq %rcx, " << offField << "(%rax)\n";
    }

    // Pops the rax declared at the start, which is the pointer
    text << "  popq %rax\n";
    // 5. Check if there are problems with fields that start without values
    // TODO
    // 6. Dont' forget to clean!!!
    // TODO
    return 0;
  } else {
    // fun call
    // 1) Evaluar parametros dados
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx",
                                   "%rcx", "%r8",  "%r9"};
    for (int i = 0; i < e->args.size(); i++) {
      e->args[i]->accept(this);
      text << " movq %rax," << argRegs[i] << endl;
    }

    // 2) Llamada a función externa/usuario
    text << "  call " << e->name << "\n";

    // Resultado en %rax
    return 0;
  }
}

// ── Sentencias ──
template <typename T> void GenCodeVisitor<T>::visit(AssignStatement *s) {
  // 1) evaluar RHS -> %rax
  s->expr->accept(this);

  // 2) caso var local
  if (auto id = dynamic_cast<IdentifierExp *>(s->target)) {
    int off = memoria.at(id->name);
    text << "  movq %rax, " << off << "(%rbp)\n";
    return;
  } else if (auto idx = dynamic_cast<IndexExp *>(s->target)) {
    // 3) caso array[index]
    // I don't know whether list inside lists work
    // TODO, test
    // push rax of expr
    text << "  pushq %rax\n";

    int off = memoria.at(idx->name);
    text << "  lea " << off << "(%rbp), %rbx\n"
         << "  movq (%rbx), %rbx\n";
    idx->index->accept(this);
    text << "  salq $3, %rax\n"
         << "  addq %rax, %rbx\n";

    // pop rax of expr to assign
    text << "  popq %rax\n";
    text << "  movq %rax, (%rbx)\n";
    return;
  } else if (auto dot = dynamic_cast<DotExp *>(s->target)) {
    // 4) caso struct.field
    // evaluar objeto para dejar puntero en %rax
    int off = memoria.at(dot->id);

    // offset del campo
    string structType = memoriaTypes_.at(dot->id);
    int fldOff = structLayouts_.at(structType).at(dot->member);

    // escribir el valor
    text << "  movq " << off << "(%rbp)" << ", %rcx\n";
    text << "  movq %rax," << fldOff << " (%rcx)\n";
    return;
  }

  // 5) caso no soportado
  throw std::runtime_error("Asignación no soportada en codegen");
}

template <typename T> void GenCodeVisitor<T>::visit(PrintStatement *s) {
  s->expr->accept(this);
  text << "  movq %rax, %rsi\n";
  text << "  leaq print_fmt(%rip), %rdi\n";
  text << "  movl $0, %eax\n";
  text << "  call printf@PLT\n";
}

template <typename T> void GenCodeVisitor<T>::visit(IfStatement *s) {
  auto Lelse = newLabel("Lelse");
  auto Lend = newLabel("Lend");
  s->cond->accept(this);
  text << "  cmpq $0, %rax\n";
  text << "  je " << Lelse << "\n";
  s->thenBranch->accept(this);
  text << "  jmp " << Lend << "\n";
  text << Lelse << ":\n";
  if (s->elseBranch)
    s->elseBranch->accept(this);
  text << Lend << ":\n";
}

template <typename T> void GenCodeVisitor<T>::visit(WhileStatement *s) {
  auto Lbegin = newLabel("while");
  auto Lend = newLabel("endwhile");
  text << Lbegin << ":\n";
  s->cond->accept(this);
  text << "  cmpq $0, %rax\n";
  text << "  je " << Lend << "\n";
  s->body->accept(this);
  text << "  jmp " << Lbegin << "\n";
  text << Lend << ":\n";
}

template <typename T> void GenCodeVisitor<T>::visit(ForStatement *s) {
  // 1) Crear espacio para la variable de iteración
  stackFor_ += 8;
  memoria[s->varName] = -stackFor_ - stackSize_;
  text << " subq $" << 8 << ", %rsp" << endl;

  // 2) Distinguir rango numérico o lista
  if (auto loop = dynamic_cast<LoopExp *>(s->iterable)) {
    // Not working correctly? Haven't tested completely with the latest
    // modification
    // TODO

    // 2.1) Define the start, the end and the step to make the comparisons
    loop->start->accept(this); // -> %rax
    text << "  movq %rax, " << memoria[s->varName] << "(%rbp)\n";

    // 2.2) Bucle con etiquetas
    auto Lfor = newLabel("for");
    auto Lend = newLabel("endfor");
    text << Lfor << ":\n";
    // cargar end
    loop->end->accept(this); // -> %rax
    text << " movq %rax, %rcx\n ";
    // cargar i
    text << "  movq " << memoria[s->varName] << "(%rbp), %rax\n";

    // compare, code from GE case in binary
    text << "  cmpq %rcx, %rax\n"
         << "  movl $0, %eax\n"
         << "  setge %al\n"
         << "  movzbq %al, %rax\n";
    text << "  cmpq $0, %rax\n";
    text << "  jne " << Lend << "\n";
    // cuerpo
    s->body->accept(this);
    // i += step

    loop->step->accept(this);
    if (loop->downTo) {
      text << "  negq %rax\n";
    }
    text << " movq %rax, %rcx\n ";
    text << "  movq " << memoria[s->varName] << "(%rbp), %rax\n";

    text << "  addq %rcx, %rax\n";
    text << "  movq %rax, " << memoria[s->varName] << "(%rbp)\n";
    text << "  jmp " << Lfor << "\n";
    text << Lend << ":\n";
  } else {
    // Not working in general
    // TODO
    // 1) inicializar índice = 0
    text << "  movq $0, " << memoriaIndex_[s->varName] << "(%rbp)\n";

    // 2) cargar sólo UNA VEZ la dirección base del array en %r14
    auto id = static_cast<IdentifierExp *>(s->iterable);
    if (memoria.count(id->name)) {
      // array/lista local en pila
      int offList = memoria.at(id->name);
      text << "  lea " << offList << "(%rbp), %rax\n" // &local
           << "  movq (%rax), %r14\n";                // ptr heap -> r14
    } else {
      // array global en .data
      text << "  movq " << id->name << "(%rip), %r14\n";
    }

    // 3) longitud
    int n = listLength_.at(id->name);

    // 4) etiquetas
    std::string Lfor = newLabel("Lfor");
    std::string Lend = newLabel("Lend");

    // 5) inicio del loop
    text << Lfor
         << ":\n"
         // cargar índice
         << "  movq " << memoriaIndex_[s->varName] << "(%rbp), %rax\n"
         << "  cmpq $" << n << ", %rax\n"
         << "  jge " << Lend
         << "\n"

         // calcular dirección en %rdx
         << "  movq " << memoriaIndex_[s->varName] << "(%rbp), %rdx\n"
         << "  salq $3, %rdx\n"
         << "  addq %r14, %rdx\n"

         // cargar elemento y guardarlo en varName
         << "  movq (%rdx), %rax\n"
         << "  movq %rax, " << memoria[s->varName] << "(%rbp)\n";

    // 8) cuerpo
    s->body->accept(this);

    // 9) incrementar índice y saltar
    text << "  movq " << memoriaIndex_[s->varName] << "(%rbp), %rax\n"
         << "  addq $1, %rax\n"
         << "  movq %rax, " << memoriaIndex_[s->varName] << "(%rbp)\n"
         << "  jmp " << Lfor << "\n"
         << Lend << ":\n";
  }
}

// ── Declaraciones y bloques ──

template <typename T> void GenCodeVisitor<T>::visit(VarDec *d) {
  for (size_t i = 0; i < d->names.size(); ++i) {
    const auto &name = d->names[i];
    // 1) Guardar el tipo: si hay anotación explícita, la usamos;
    //    si no y viene de un constructor (FCallExp), inferimos el tipo.
    if (!d->typeName.empty()) {
      memoriaTypes_[name] = d->typeName;
    } else if (i < d->inits.size()) {
      if (auto *fc = dynamic_cast<FCallExp *>(d->inits[i])) {
        memoriaTypes_[name] = fc->name; // inferimos "P" de P()
      }
    }

    if (inGlobal_) {
      // 2) Marcar la variable como global
      memoriaGlobal[name] = true;

      // 3) Por defecto, 8 bytes por elemento
      elemSize_[name] = 8;

      // Si se inicializa
      if (i < d->inits.size() && d->inits[i]) {
        if (auto *num = dynamic_cast<NumberExp *>(d->inits[i])) {
          data << name << ": .quad " << num->value << "\n";
          continue; // skip resto (no malloc)
        } else if (auto *str = dynamic_cast<StringExp *>(d->inits[i])) {
          // Si es inicialización con string
          static int strLabelCounter = 0;
          std::string label = "str" + std::to_string(strLabelCounter++);
          data << label << ": .string \"" << str->value << "\"\n";
          data << name << ": .quad " << label << "\n";
          continue; // skip resto
        } else if (auto *le = dynamic_cast<ListExp *>(d->inits[i])) {
          // Si es una lista (ya tienes esto bien)
          listLength_[name] = (int)le->elements.size();
          globalInits_[name] = le;

          bool allBool = true;
          for (auto *el : le->elements) {
            if (!dynamic_cast<BoolExp *>(el)) {
              allBool = false;
              break;
            }
          }
          if (allBool) {
            elemSize_[name] = 1;
            booleanArrs_.insert(name);
          }

          // Create global label to can be used as a pointer towards the list
          data << name << ": .quad " << 0 << "\n";
        }
      }
    } else {
      // variables locales sin cambios
      stackSize_ += 8;
      memoria[name] = -stackSize_ - stackFor_;
    }
  }
}

template <typename T> void GenCodeVisitor<T>::visit(VarDecList *l) {
  for (auto v : l->vars)
    v->accept(this);
}

template <typename T> void GenCodeVisitor<T>::visit(FunDec *f) {
  text << ".globl " << f->name << "\n";
  text << f->name << ":\n";
  text << "  pushq %rbp\n";
  text << "  movq %rsp, %rbp\n\n";

  // If inside, main declare list global variables
  if (f->name == "main") {
    for (auto &pr : memoriaGlobal) {
      const std::string &name = pr.first;

      // Solo inicializar en main si es una lista o array (es decir, está en
      // globalInits_)
      if (globalInits_.count(name)) {
        int n = listLength_.at(name);     // longitud
        auto *le = globalInits_.at(name); // ListExp*
        int esz = elemSize_.at(name);

        // 1. Reservar el heap
        text << "  movq $" << (n * esz) << ", %rdi\n"
             << "  call malloc@PLT\n"
             << "  pushq %rax\n";
        // 2. Assing initial values
        for (size_t i = 0; i < n; ++i) {
          text << "  pushq %rax\n";
          le->elements[i]->accept(this); // → %rax = valor entero
          if (esz == 1) {
            text << "  movq %al, %rcx\n";
          } else {
            text << "  movq %rax, %rcx\n";
          }
          // Pops to have the rax pointer saved at the start
          text << "  popq %rax\n";
          // Uses rax to access the index values
          text << "  movq %rcx, " << (i * 8) << "(%rax)\n";
        }

        text << "  popq %rax\n";

        // 3. Guardar puntero en la etiqueta global
        text << "  movq %rax, " << name << "(%rip)\n\n";
      }
    }
  }

  this->nombreFuncion = f->name;

  memoria.clear();
  stackSize_ = 0;

  static const char *argRegs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
  for (size_t i = 0; i < f->params.size() && i < 6; ++i) {
    auto &p = f->params[i];
    stackSize_ += 8;
    memoria[p.name] = -stackSize_;
    memoriaTypes_[p.name] = p.type;
    text << "  movq " << argRegs[i] << ", " << memoria[p.name] << "(%rbp)\n";
  }
  text << "\n";

  f->body->accept(this);

  text << ".end_" << f->name << ":" << endl;
  text << "leave" << endl;
  text << "ret" << endl;
  this->nombreFuncion = "";
}

template <typename T> void GenCodeVisitor<T>::visit(FunDecList *list) {
  for (auto fn : list->functions) {
    fn->accept(this);
  }
}

template <typename T> void GenCodeVisitor<T>::visit(ClassDec *dec) {
  int offset = 0;
  // Also add the arguments
  // Separate it for the constructor
  for (auto arg : dec->args) {
    // 1) Graba el offset
    structLayouts_[dec->name][arg.name] = offset;
    // 2) Graba el tipo del campo
    structFieldTypes_[dec->name][arg.name] = arg.type;
    // 3) Siguiente campo a +8 bytes
    offset += 8;
  }
  for (auto *var : dec->members->vars) {
    for (int i = 0; i < var->names.size(); i++) {
      // 1) Graba el offset
      structLayouts_[dec->name][var->names[i]] = offset;
      // 2) Graba el tipo del campo
      structFieldTypes_[dec->name][var->names[i]] = var->typeName;
      // 3) Siguiente campo a +8 bytes
      offset += 8;
      // 4) Add initializer
      if (i < var->inits.size() && var->inits[i]) {
        structFieldInits_[dec->name][var->names[i]] = var->inits[i];
      }
    }
  }
}

template <typename T> void GenCodeVisitor<T>::visit(ClassDecList *list) {
  for (auto cls : list->classes) {
    cls->accept(this);
  }
}

template <typename T> void GenCodeVisitor<T>::visit(StatementList *l) {
  for (auto s : l->statements)
    s->accept(this);
}

template <typename T> void GenCodeVisitor<T>::visit(Body *b) {
  // 1. Reserva para las variables
  b->vardecs->accept(this);
  if (stackSize_ > 0) {
    text << "  subq $" << stackSize_ + stackFor_ << ", %rsp\n\n";
  }

  // 2. Inicializa las variables locales con sus valores
  for (auto *d : b->vardecs->vars) {
    for (size_t i = 0; i < d->inits.size(); ++i) {
      if (!d->inits[i])
        continue;
      d->inits[i]->accept(this); // calcula el valor → %rax
      text << "  movq %rax, " << memoria[d->names[i]] << "(%rbp)\n";
    }
  }
  text << "\n";

  // 3. Acepta statements
  b->stmts->accept(this);
}

template <typename T> void GenCodeVisitor<T>::visit(Program *prog) {
  // — PASO 1: recolectar literales de string
  collectingStrings_ = true;
  for (auto d : prog->vardecs->vars) {
    for (auto *init : d->inits)
      if (init)
        init->accept(this);
  }
  collectingStrings_ = false;

  // — PASO 2: sección .data
  data << ".data\n";
  data << "print_fmt: .string \"%ld\\n\"\n\n";

  // 2.b) Variables globales (listas)
  inGlobal_ = true;
  prog->vardecs->accept(this);

  inGlobal_ = false;

  // 2.c) clases/globales si tuvieras más…
  prog->classDecs->accept(this);

  // — PASO 3: emitimos text
  text << "\n.text\n\n";

  // ahora vienen los FunDec
  prog->funDecs->accept(this);

  text << ".section .note.GNU-stack,\"\",@progbits\n";

  out_ << data.str() << text.str();
}

template <typename T> void GenCodeVisitor<T>::visit(ReturnStatement *s) {
  if (s->expr)
    s->expr->accept(this); // valor → %rax
  text << " jmp .end_" << this->nombreFuncion << endl;
}

// Instation to prevent linking issues
template class GenCodeVisitor<std::ostream>;
template class GenCodeVisitor<std::ofstream>;
template class GenCodeVisitor<std::stringstream>;
