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

int PrintVisitor::visit(StringExp *exp) {
  cout << '"' << exp->value << '"';
  return 0;
}

int PrintVisitor::visit(NumberExp *e)  { 
  cout << e->value; 
  return 0; }

int PrintVisitor::visit(BoolExp *e) { 
  cout << (e->value ? "true" : "false"); 
  return 0; }
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
  // imprime algo como: lista[indice]
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
  cout << "if "; s->cond->accept(this); cout << " {" << endl;
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
  cout << "while "; s->cond->accept(this); cout << " {" << endl;
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

void PrintVisitor::visit(VarDecList *list) {
  for (auto dec : list->vars)
    dec->accept(this);
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
  if (p->body)
    p->body->accept(this);
}

//----------------------------------------------------------------------
// EVALVisitor implementations
//----------------------------------------------------------------------

void EVALVisitor::ejecutar(Program *p) {
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

  // 4) Ejecuta main
  retflag = false;
  if (p->classDecs) p->classDecs->accept(this);
  if (p->funDecs)   p->funDecs->accept(this);
  p->body->accept(this);

  // 5) Sal del scope global
  env.remove_level();

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
    case GT_OP:    return v1 > v2;
    case LE_OP:    return v1 <= v2;
    case EQ_OP:    return v1 == v2;
    default:       return 0;
  }
}

int EVALVisitor::visit(StringExp *exp) {
  return 0;
}

int EVALVisitor::visit(NumberExp *exp) { return exp->value; }
int EVALVisitor::visit(BoolExp   *exp) { return exp->value; }

int EVALVisitor::visit(IdentifierExp *exp) {
  if (!env.check(exp->name)) {
    cout << "Variable no declarada: " << exp->name << endl;
    return 0;
  }
  int r = env.lookup(exp->name);
  return env.lookup(exp->name);
}

int EVALVisitor::visit(FCallExp *exp) {
  // 0) Constructor de struct/clase
  auto itFields = classFields_.find(exp->name);
  if (itFields != classFields_.end()) {
    // 1) Nuevo objectId
    int objId = nextObjectId++;
    // 2) Evaluar cada argumento y asignarlo al campo correspondiente
    auto &fields = itFields->second;
    for (size_t i = 0; i < exp->args.size(); ++i) {
      int val = exp->args[i]->accept(this);
      objectHeap[objId][ fields[i] ] = val;
    }
    // 3) Guardar el objId en el entorno local (por ejemplo, como Int)
    return objId;
  }

  // 1) Built-in: print / println
  if (exp->name == "print" || exp->name == "println") {
    for (auto arg : exp->args) {
      // 1.a) String literal directo
      if (auto se = dynamic_cast<StringExp*>(arg)) {
        cout << se->value;
        continue;
      }
      // 1.b) IndexExp sobre lista de strings
      if (auto ie = dynamic_cast<IndexExp*>(arg)) {
        int listId = env.lookup(ie->name);
        int idx    = ie->index->accept(this);
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
    if (exp->name == "println") cout << "\n";
    return 0;
  }

  // 2) Llamada a función del usuario
  auto it = fdecs.find(exp->name);
  if (it == fdecs.end()) {
    cerr << "Error: función no declarada: " << exp->name << endl;
    exit(1);
  }
  FunDec* fn = it->second;

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
  retflag = false;
  fn->body->accept(this);
  if (!retflag) {
    cerr << "Error: función '" << fn->name << "' sin return" << endl;
    exit(1);
  }
  int result = retval;

  // 6) Salir del scope
  env.remove_level();
  return result;
}

// Lista literal: construye un vector<int>, lo guarda en listHeap y devuelve su ID
int EVALVisitor::visit(ListExp *exp) {
  // Si el primer elemento es StringExp hacemos lista de strings
  if (!exp->elements.empty() &&
      dynamic_cast<StringExp*>(exp->elements[0]) != nullptr)
  {
    std::vector<std::string> svals;
    for (auto e : exp->elements) {
      auto se = dynamic_cast<StringExp*>(e);
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
  int idx    = exp->index->accept(this);

  // 2) si existe en stringListHeap, devolvemos un índice inválido (no se usa aquí)
  if (stringListHeap.count(listId)) {
    auto &svec = stringListHeap[listId];
    if (idx<0 || idx>=(int)svec.size()) {
      std::cerr<<"Error: índice fuera de rango en "<<exp->name<<": "<<idx<<"\n";
      exit(1);
    }
    return idx;
  }
  // 3) si está en listHeap, devolvemos el valor int
  auto &vec = listHeap.at(listId);
  if (idx < 0 || idx >= (int)vec.size()) {
    std::cerr << "Error: índice fuera de rango en " << exp->name << ": " << idx << "\n";
    exit(1);
  }
  return vec[idx];
}

int EVALVisitor::visit(DotExp *exp) {
  // 1) obtenemos el objectId (se guardó como variable entera)
  int objId = env.lookup(exp->object);
  // 2) buscamos el campo
  auto &fields = objectHeap[objId];
  if (!fields.count(exp->member)) {
    std::cerr << "Error: campo '" << exp->member
              << "' no existe en " << exp->object << "\n";
    std::exit(1);
  }
  return fields[exp->member];
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
  int end   = exp->end->accept(this);
  int step  = exp->step ? exp->step->accept(this)
                        : (exp->downTo ? -1 : 1);
  // return start as placeholder
  return start;
}

void EVALVisitor::visit(AssignStatement *stm) {
    int val = stm->expr->accept(this);
    if (auto id = dynamic_cast<IdentifierExp*>(stm->target)) {
        const std::string &name = id->name;

        if (!env.check(name)) {
            cout << "Variable no declarada: " << name << endl;
            return;
        }

        env.update(name, val);
    }
    else {
        std::cerr << "Asignación inválida en LHS no identificador\n";
    }
}


void EVALVisitor::visit(PrintStatement *stm) {
    Exp* e = stm->expr;

    // 1) String literal directo
    if (auto se = dynamic_cast<StringExp*>(e)) {
        cout << se->value;

    // 2) IndexExp sobre lista de strings
    } else if (auto ie = dynamic_cast<IndexExp*>(e)) {
        int listId = env.lookup(ie->name);
        int idx    = ie->index->accept(this);
        auto it    = stringListHeap.find(listId);
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
  retval  = stm->expr ? stm->expr->accept(this) : 0;
  retflag = true;
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
  // 1) la declaro con valor basura o 0
  env.add_var(dec->name, dec->typeName);

  // 2) si tenía inicializador, lo evalúo y actualizo
  if (dec->init) {
    int v = dec->init->accept(this);
    env.update(dec->name, v);
  }
}

void EVALVisitor::visit(VarDecList *list) {
  for (auto dec : list->vars)
    dec->accept(this);
}

void EVALVisitor::visit(ClassDec *dec) {
  // Guarda el orden de los campos tal como aparecen
  std::vector<std::string> fields;
  for (auto var : dec->members->vars) {
    fields.push_back(var->name);
  }
  classFields_[dec->name] = std::move(fields);
}

void EVALVisitor::visit(ClassDecList *list) {
  for (auto c : list->classes) c->accept(this);
}

void EVALVisitor::visit(FunDec *fn) {
  fdecs[fn->name] = fn;
}

void EVALVisitor::visit(FunDecList *list) {
  for (auto f : list->functions) {
    fdecs[f->name] = f;
  }
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

//----------------------------------------------------------------------
// Constructor
GenCodeVisitor::GenCodeVisitor(std::ostream& out)
  : out_(out)
{}

// Generación principal
void GenCodeVisitor::generate(Program* prog) {
  collectingStrings_ = true;
  for (auto d : prog->vardecs->vars)
    if (d->init) d->init->accept(this);
  collectingStrings_ = false;

  // ======== PASADA 2: EMITIR .data ========
  out_ << ".data\n";
  out_ << "print_fmt: .string \"%ld\\n\"\n\n";

  globalVars_ = prog->vardecs->vars;

  // 2.a) Literales de cadena
  for (auto &p : stringLabel_) {
    out_ << p.second << ": .string \"" << p.first << "\"\n";
  }
  if (!stringLabel_.empty()) out_ << "\n";

  // 2.b) Variables globales
  inGlobal_ = true;
  prog->vardecs->accept(this);
  inGlobal_ = false;

  prog->classDecs->accept(this);

  out_ << "\n.text\n";
  // Declaramos cada función:
  for (auto &fn : prog->funDecs->functions) {
    out_ << ".globl " << fn->name << "\n";
  }
  out_ << "\n";

  prog->accept(this);
}


// Etiquetas únicas
std::string GenCodeVisitor::newLabel(const std::string& prefix) {
  return prefix + std::to_string(labelCount_++);
}

// ── Expresiones ──

int GenCodeVisitor::visit(StringExp *e) {
  // 1) genera un label único en .rodata con el literal
  std::string lbl = newLabel("str");
  out_ << ".section .rodata\n"
       << lbl << ": .string \"" << e->value << "\"\n"
       << ".text\n";
  // 2) carga la dirección del string en %rax
  out_ << "  leaq " << lbl << "(%rip), %rax\n";
  return 0;
}

int GenCodeVisitor::visit(NumberExp *e) {
  out_ << "  movq $" << e->value << ", %rax\n";
  return 0;
}

int GenCodeVisitor::visit(BoolExp *e) {
  out_ << "  movq $" << (e->value ? 1 : 0) << ", %rax\n";
  return 0;
}

int GenCodeVisitor::visit(IdentifierExp *e) {
  if (symtab_.count(e->name)) {
    int off = symtab_.at(e->name);
    out_ << "  movq " << off << "(%rbp), %rax\n";
  } else {
    // variable global
    out_ << "  movq " << e->name << "(%rip), %rax\n";
  }
  return 0;
}

int GenCodeVisitor::visit(BinaryExp *e) {
  e->left->accept(this);
  out_ << "  pushq %rax\n";
  e->right->accept(this);
  out_ << "  popq %rcx\n";
  switch (e->op) {
    case PLUS_OP:  
      out_ << "  addq %rcx, %rax\n";   
      break;
    case MINUS_OP: 
      out_ << "  subq %rax, %rcx\n"
              "  movq %rcx, %rax\n";  
      break;
    case MUL_OP:   
      out_ << "  imulq %rcx, %rax\n"; 
      break;
    case DIV_OP:   
      out_ << "  cqto\n"
              "  idivq %rcx\n";      
      break;
    case LT_OP:
      out_ << "  cmpq %rcx, %rax\n"
              "  setl %al\n"
              "  movzbq %al, %rax\n";
      break;
    case LE_OP:
      out_ << "  cmpq %rcx, %rax\n"
              "  setle %al\n"
              "  movzbq %al, %rax\n";
      break;
    case EQ_OP:
      out_ << "  cmpq %rcx, %rax\n"
              "  sete %al\n"
              "  movzbq %al, %rax\n";
      break;
    default:       break;
  }
  return 0;
}

int GenCodeVisitor::visit(IndexExp *e) {
    // 1) Evaluar el índice → %rax
    e->index->accept(this);

    // 2) Cargar la dirección base del array en %rbx
    if (symtab_.count(e->name)) {
        // variable local en la pila
        int off = symtab_.at(e->name);
        out_ << "  lea " << off << "(%rbp), %rbx\n";  // &lista
        out_ << "  movq (%rbx), %rbx\n";              // ptr heap
    } else {
        // variable global
        out_ << "  movq " << e->name << "(%rip), %rbx\n";
    }

    // 3) Convertir índice en desplazamiento de bytes (= idx * 8)
    out_ << "  salq $3, %rax\n";

    // 4) Sumar desplazamiento al puntero base -> %rbx + %rax
    out_ << "  addq %rax, %rbx\n";

    // 5) Cargar el valor/ptr del elemento en %rax
    out_ << "  movq (%rbx), %rax\n";

    return 0;
}

int GenCodeVisitor::visit(DotExp *e) {
  // 1) cargar ptr de la variable (local o global)
  if (symtab_.count(e->object)) {
    int off = symtab_.at(e->object);
    out_ << "  movq " << off << "(%rbp), %rax\n";
  } else {
    out_ << "  movq " << e->object << "(%rip), %rax\n";
  }
  // 2) offset del campo
  auto structType = varTypes_.at(e->object);
  int fldOff = structLayouts_.at(structType).at(e->member);
  // 3) cargar campo
  out_ << "  addq $" << fldOff << ", %rax\n"
       << "  movq (%rax), %rax\n";
  return 0;
}


int GenCodeVisitor::visit(LoopExp *exp) {
    // Generación mínima: evalúa el inicio y devuelve un valor ficticio
    exp->start->accept(this);
    return 0;
}

int GenCodeVisitor::visit(IFExp *e) {
  auto Lelse = newLabel("Lelse");
  auto Lend  = newLabel("Lend");
  // condición
  e->cond->accept(this);
  out_ << "  cmpq $0, %rax\n";
  out_ << "  je " << Lelse << "\n";
  // then
  e->left->accept(this);
  out_ << "  jmp " << Lend << "\n";
  out_ << Lelse << ":\n";
  // else
  e->right->accept(this);
  out_ << Lend << ":\n";
  return 0;
}

int GenCodeVisitor::visit(ListExp *e) {
    size_t n = e->elements.size();

    // ── PRIMERA PASADA: recolección de literales sólo de strings
    if (collectingStrings_) {
        if (n > 0 && dynamic_cast<StringExp*>(e->elements[0])) {
            for (auto *el : e->elements) {
                auto txt = static_cast<StringExp*>(el)->value;
                if (!stringLabel_.count(txt))
                    stringLabel_[txt] = newLabel("str");
            }
        }
        return 0;
    }

    // ── SEGUNDA PASADA: generación real dentro de .text

    // A) Lista de String: malloc de punteros + llenar con labels
    if (n > 0 && dynamic_cast<StringExp*>(e->elements[0])) {
        out_ << "  movq $" << (n * 8) << ", %rdi\n"
             << "  call malloc@PLT\n"
             << "  movq %rax, %rbx\n";
        for (size_t i = 0; i < n; ++i) {
            auto txt = static_cast<StringExp*>(e->elements[i])->value;
            auto lbl = stringLabel_[txt];
            out_ << "  leaq " << lbl << "(%rip), %rax\n"
                 << "  movq %rax, " << (i * 8) << "(%rbx)\n";
        }
        return 0;
    }

    // B) Lista de Int: malloc de 8*n bytes + almacenar cada entero
    out_ << "  movq $" << (n * 8) << ", %rdi\n"
         << "  call malloc@PLT\n"
         << "  movq %rax, %rbx\n";
    for (size_t i = 0; i < n; ++i) {
        e->elements[i]->accept(this);   // → %rax = valor entero
        out_ << "  movq %rax, " << (i * 8) << "(%rbx)\n";
    }
    return 0;
}

int GenCodeVisitor::visit(FCallExp *e) {
  // 0) Constructor de struct/clase si existe layout
  auto it = structLayouts_.find(e->name);
  if (it != structLayouts_.end()) {
    // número de campos
    int nFields = (int)it->second.size();
    // malloc(nFields*8)
    out_ << "  movq $" << (nFields * 8) << ", %rdi\n"
         << "  call malloc@PLT\n"
         << "  movq %rax, %rbx\n";
    // llenar cada campo: args[i] → offset i*8
    for (size_t i = 0; i < e->args.size(); ++i) {
      e->args[i]->accept(this);    // → %rax = valor de arg
      out_ << "  movq %rax, " << (i * 8) << "(%rbx)\n";
    }
    // devolver puntero al objeto en %rax
    out_ << "  movq %rbx, %rax\n";
    return 0;
  }

  // — resto de llamadas (print, funciones…) —
  // 1) Evaluar argumentos en pila
  for (int i = int(e->args.size()) - 1; i >= 0; --i) {
    e->args[i]->accept(this);
    out_ << "  pushq %rax\n";
  }
  // 2) Llamada a función externa/usuario
  out_ << "  call " << e->name << "@PLT\n";
  // 3) Limpiar pila
  if (!e->args.empty())
    out_ << "  addq $" << (e->args.size() * 8) << ", %rsp\n";
  // Resultado en %rax
  return 0;
}

// ── Sentencias ──
void GenCodeVisitor::visit(AssignStatement *s) {
  // ---- paso 1: evaluar la expresión RHS y salvarla en %r12 ----
  s->expr->accept(this);          // deja el valor en %rax
  out_ << "  movq %rax, %r12\n";  // salvar RHS

  // ---- paso 2: distinguir LHS ----
  if (auto id = dynamic_cast<IdentifierExp*>(s->target)) {
    // asignación a variable local
    int off = symtab_.at(id->name);
    out_ << "  movq %r12, " << off << "(%rbp)\n";

  } else if (auto idx = dynamic_cast<IndexExp*>(s->target)) {
    // asignación a array[index]
    // a) cargar la dirección base del array
    int off = symtab_.at(idx->name);
    out_ << "  lea " << off << "(%rbp), %rbx\n";   // &arr
    out_ << "  movq (%rbx), %rbx\n";               // ptr heap

    // b) calcular offset = index * 8
    idx->index->accept(this);                      // índice -> %rax
    out_ << "  salq $3, %rax\n";                   // *8
    out_ << "  addq %rax, %rbx\n";                 // &elemento

    // c) escribir el valor salvado en %r12
    out_ << "  movq %r12, (%rbx)\n";

  } else if (auto dot = dynamic_cast<DotExp*>(s->target)) {
    // asignación a struct.field
    int offObj = symtab_.at(dot->object);
    out_ << "  lea " << offObj << "(%rbp), %rbx\n";  // &obj
    auto structType = varTypes_.at(dot->object);
    int fldOff = structLayouts_.at(structType).at(dot->member);
    out_ << "  addq $" << fldOff << ", %rbx\n";      // &obj.member
    out_ << "  movq %r12, (%rbx)\n";

  } else {
    // otros casos (p.ej. listas, etc.)
  }
}

void GenCodeVisitor::visit(PrintStatement *s) {
  s->expr->accept(this);
  out_ << "  movq %rax, %rsi\n";
  out_ << "  leaq print_fmt(%rip), %rdi\n";
  out_ << "  movl $0, %eax\n";
  out_ << "  call printf@PLT\n";
}

void GenCodeVisitor::visit(IfStatement *s) {
  auto Lelse = newLabel("Lelse");
  auto Lend  = newLabel("Lend");
  s->cond->accept(this);
  out_ << "  cmpq $0, %rax\n";
  out_ << "  je " << Lelse << "\n";
  s->thenBranch->accept(this);
  out_ << "  jmp " << Lend << "\n";
  out_ << Lelse << ":\n";
  if (s->elseBranch) s->elseBranch->accept(this);
  out_ << Lend << ":\n";
}

void GenCodeVisitor::visit(WhileStatement *s) {
  auto Lbegin = newLabel("Lwb");
  auto Lend   = newLabel("Lwe");
  out_ << Lbegin << ":\n";
  s->cond->accept(this);
  out_ << "  cmpq $0, %rax\n";
  out_ << "  je " << Lend << "\n";
  s->body->accept(this);
  out_ << "  jmp " << Lbegin << "\n";
  out_ << Lend << ":\n";
}

void GenCodeVisitor::visit(ForStatement *s) {
  // 1) Crear espacio para la variable índice
  stackSize_ += 8;
  symtab_[s->varName] = -stackSize_;
  // 2) Distinguir rango numérico o lista
  if (auto loop = dynamic_cast<LoopExp*>(s->iterable)) {
    // 2.1) start, end, step
    loop->start->accept(this); // -> %rax
    out_ << "  movq %rax, " << symtab_[s->varName] << "(%rbp)\n";
    loop->end->accept(this);   // -> %rax
    out_ << "  movq %rax, %r11\n";
    if (loop->step) {
      loop->step->accept(this);
    } else {
      out_ << "  movq $1, %rax\n";
      if (loop->downTo)
        out_ << "  negq %rax\n";
    }
    out_ << "  movq %rax, %r10\n";

    // 2.2) Bucle con etiquetas
    auto Lfor = newLabel("Lfor");
    auto Lend = newLabel("Lfe");
    out_ << Lfor << ":\n";
    // cargar i
    out_ << "  movq " << symtab_[s->varName] << "(%rbp), %rcx\n";
    // comparar con end (%r11)
    if (loop->downTo)
      out_ << "  cmpq %r11, %rcx\n  jl " << Lend << "\n";
    else
      out_ << "  cmpq %r11, %rcx\n  jg " << Lend << "\n";
    // cuerpo
    s->body->accept(this);
    // i += step
    out_ << "  movq " << symtab_[s->varName] << "(%rbp), %rax\n";
    out_ << "  addq %r10, %rax\n";
    out_ << "  movq %rax, " << symtab_[s->varName] << "(%rbp)\n";
    out_ << "  jmp " << Lfor << "\n";
    out_ << Lend << ":\n";
  } else {
    // 1) inicializar índice = 0
    out_ << "  movq $0, " << symtab_[s->varName] << "(%rbp)\n";

    // 2) cargar ptr al primer elemento de la lista
    auto id = static_cast<IdentifierExp*>(s->iterable);
    int offList = symtab_.at(id->name);
    out_ << "  lea " << offList << "(%rbp), %rbx\n"   // &varList
         << "  movq (%rbx), %rbx\n";                  // ptr al heap

    // 3) longitud de la lista
    int n = listLength_.at(id->name);

    // 4) etiquetas
    std::string Lfor = newLabel("Lfor");
    std::string Lend = newLabel("Lend");
    out_ << Lfor << ":\n";

    // 5) comparar idx >= n → fin
    out_ << "  movq " << symtab_[s->varName]
         << "(%rbp), %rax\n"
         << "  cmpq $" << n << ", %rax\n"
         << "  jge " << Lend << "\n";

    // 6) cargar elemento actual
    out_ << "  movq " << symtab_[s->varName] << "(%rbp), %rax\n"
         << "  salq $3, %rax\n"                   // *8
         << "  addq %rax, %rbx\n"                 // &elem
         << "  movq (%rbx), %rax\n";              // valor o ptr

    // 7) asignarlo a la variable de iteración
    out_ << "  movq %rax, " << symtab_[s->varName] << "(%rbp)\n";

    // 8) cuerpo del bucle
    s->body->accept(this);

    // 9) incrementar índice y saltar al inicio
    out_ << "  movq " << symtab_[s->varName]
         << "(%rbp), %rax\n"
         << "  addq $1, %rax\n"
         << "  movq %rax, " << symtab_[s->varName] << "(%rbp)\n"
         << "  jmp " << Lfor << "\n"
         << Lend << ":\n";
  }
}

// ── Declaraciones y bloques ──

void GenCodeVisitor::visit(VarDec *d) {
  if (inGlobal_) {
    // --- globales en .data ---
    out_ << d->name << ": .quad ";
    if (auto num = dynamic_cast<NumberExp*>(d->init)) {
      out_ << num->value;
    } else {
      out_ << "0";
    }
    out_ << "\n";
    return;
  }

  // --- variable LOCAL: reservar espacio ---
  stackSize_ += 8;
  symtab_[d->name]   = -stackSize_;
  varTypes_[d->name] = d->typeName;

  // --- si hay inicializador, generar código para ello ---
  if (d->init) {
    // 1) evaluamos la expresión de init, queda en %rax
    d->init->accept(this);

    // 2) almacenamos %rax en el offset correspondiente
    out_ << "  movq %rax, " << symtab_[d->name] << "(%rbp)\n";

    // 3) si fuera lista, también guardamos su longitud
    if (auto le = dynamic_cast<ListExp*>(d->init)) {
      listLength_[d->name] = (int)le->elements.size();
    }
  }
}

void GenCodeVisitor::visit(VarDecList *l) {
  for (auto v : l->vars) v->accept(this);
}

void GenCodeVisitor::visit(FunDec *f) {
  out_ << f->name << ":\n"
       << "  pushq %rbp\n"
       << "  movq %rsp, %rbp\n";

  // … inicialización de globals …

  // 1) Fase de conteo de locals (y registro de tipos)
  symtab_.clear();
  stackSize_ = 0;
  // Llamamos a un VarDecList “dummy” que solo cuente:
  for (auto vd : f->body->vardecs->vars) {
    // modela lo que hace visit(VarDec*) pero solo el ++, sin emitir código
    stackSize_ += 8;
    symtab_[vd->name] = -stackSize_;
    varTypes_[vd->name] = vd->typeName;    // <-- aquí guardamos p → "Point"
  }

  // 2) Reserva de espacio
  if (stackSize_ > 0) {
    out_ << "  subq $" << stackSize_ << ", %rsp\n";
  }

  // 3) Fase de inicializadores
  for (auto vd : f->body->vardecs->vars) {
    if (vd->init) {
      // evalúa el init en %rax
      vd->init->accept(this);
      // almacena en la pila
      out_ << "  movq %rax, " << symtab_[vd->name] << "(%rbp)\n";
    }
  }

  // 4) Cuerpo de sentencias
  f->body->stmts->accept(this);

  // 5) Epílogo
  out_ << "  leave\n"
       << "  ret\n";
}

void GenCodeVisitor::visit(FunDecList *list) {
    for (auto fn : list->functions) {
        fn->accept(this);
    }
}

void GenCodeVisitor::visit(ClassDec *dec) {
  int offset = 0;
  for (auto field : dec->members->vars) {
    structLayouts_[dec->name][field->name] = offset;
    offset += 8;  // asumimos 8 bytes por campo
  }
}

void GenCodeVisitor::visit(ClassDecList *list) {
    for (auto cls : list->classes) {
        cls->accept(this);
    }
}

void GenCodeVisitor::visit(StatementList *l) {
  for (auto s : l->statements) s->accept(this);
}

void GenCodeVisitor::visit(Body *b) {
  b->vardecs->accept(this);
  b->stmts->accept(this);
}

void GenCodeVisitor::visit(Program *p) {
  p->funDecs->accept(this);
}

void GenCodeVisitor::visit(ReturnStatement *s) {
  if (s->expr) s->expr->accept(this);
}


