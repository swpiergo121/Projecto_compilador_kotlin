// parser.cpp
#include "parser.h"
#include "exp.h"
#include "token.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Parser::Parser(Scanner *sc)
    : scanner(sc), current(scanner->nextToken()), previous(nullptr) {
  if (current->type == Token::ERR)
    throw runtime_error("Error léxico inicial: " + current->text);
}

Program *Parser::parse() { return parseProgram(); }

// --- Helpers ---

bool Parser::match(Token::Type type) {
  if (check(type)) {
    advance();
    return true;
  }
  return false;
}

bool Parser::match(Token::Type type, const string &lexeme) {
  if (!isAtEnd() && current->type == type && current->text == lexeme) {
    advance();
    return true;
  }
  return false;
}

bool Parser::check(Token::Type type) const {
  return !isAtEnd() && current->type == type;
}

Token *Parser::advance() {
  previous = current;
  current = scanner->nextToken();
  if (current->type == Token::ERR)
    error("Carácter no reconocido: " + current->text);
  return previous;
}

Token *Parser::consume(Token::Type type, const string &message) {
  if (check(type))
    return advance();
  error(message);
  return nullptr;
}

bool Parser::isAtEnd() const { return current->type == Token::END; }

void Parser::error(const string &msg) {
  cerr << "[Line ??] Error de sintaxis: " << msg << endl;
  exit(1);
}

// --- Programa principal ---
Program *Parser::parseProgram() {
  // 1) variables globales
  VarDecList *globals = parseVarDecList();

  // 2) clases globales (si las hubiera)
  ClassDecList *classes = parseClassDecList();

  // 3) TODAS las funciones globales (incluido main)
  FunDecList *funcs = parseFunDecList();

  // 4) construir el AST
  Program *prog = new Program(nullptr);
  prog->vardecs = globals;
  prog->classDecs = classes;
  prog->funDecs = funcs;

  // 5) asignar el body de main
  for (auto f : funcs->functions) {
    if (f->name == "main") {
      prog->body = f->body;
      break;
    }
  }

  return prog;
}

// --- Declaraciones de variables ---
VarDecList* Parser::parseVarDecList() {
    auto list = new VarDecList();
    // mientras venga 'var' o 'val'
    while (match(Token::VAR) || match(Token::VAL)) {
        bool isMutable = (previous->type == Token::VAR);

        // 1) uno o varios nombres separados por ','
        vector<string> names;
        names.push_back(consume(Token::ID, "Se esperaba identificador")->text);
        while (match(Token::COMA)) {
            names.push_back(consume(Token::ID, "Se esperaba identificador")->text);
        }

        // 2) tipo opcional: ':' Type [ '<' Gen (',' Gen)* '>' ]
        string typeName;
        if (match(Token::COLON)) {
            typeName = consume(Token::ID, "Se esperaba nombre de tipo")->text;
            if (match(Token::LT)) {
                typeName += "<" + consume(Token::ID, "Se esperaba tipo genérico")->text;
                while (match(Token::COMA))
                    typeName += "," + consume(Token::ID, "Se esperaba tipo genérico")->text;
                consume(Token::GT, "Se esperaba '>' al final de tipo genérico");
                typeName += ">";
            }
        }

        // 3) inicializador opcional (solo al primero)
        vector<Exp*> inits(names.size(), nullptr);
        if (match(Token::ASSIGN)) {
            inits[0] = parseCExp();
        }

        // 4) anotar la declaración
        list->add(new VarDec(isMutable, names, typeName, inits));
    }
    return list;
}

VarDec *Parser::parseVarDec() {
  // 0) Mutabilidad
  bool isMutable = false;
  if (previous->type == Token::VAR) {
    isMutable = true;
  } else if (previous->type == Token::VAL) {
    isMutable = false;
  } else {
    error("Se esperaba 'var' o 'val'");
  }

  // 1) Leer uno o varios nombres, separados por coma
  vector<string> names;
  names.push_back(
    consume(Token::ID, "Se esperaba identificador")->text
  );
  while (match(Token::COMA)) {
    names.push_back(
      consume(Token::ID, "Se esperaba identificador")->text
    );
  }

  // 2) Tipo opcional: si viene ':', lo consumimos; si no, inferimos
  string typeName = "";
  if (match(Token::COLON)) {
    typeName = consume(Token::ID, "Se esperaba nombre de tipo")->text;
    // Genéricos List<T>?
    if (match(Token::LT)) {
      typeName += "<" + consume(Token::ID, "Se esperaba tipo genérico")->text;
      while (match(Token::COMA)) {
        typeName += "," + consume(Token::ID, "Se esperaba tipo genérico")->text;
      }
      consume(Token::GT, "Se esperaba '>' al final de tipo genérico");
      typeName += ">";
    }
  }

  // 3) Inicializador opcional
  vector<Exp *> inits;
  if (match(Token::ASSIGN)) {
    Exp *init = parseCExp();
    inits.push_back(init);
    while (match(Token::COMA)) {
      init = parseCExp();
      inits.push_back(init);
    }
  }

  return new VarDec(isMutable, names, typeName, inits);
}

// --- Declaraciones de clase ---

ClassDecList *Parser::parseClassDecList() {
  auto list = new ClassDecList();
  // Mientras veamos la palabra “class”
  while (match(Token::ID, "class")) {
    list->add(parseClassDec());
  }
  return list;
}

FunDecList *Parser::parseFunDecList() {
  auto list = new FunDecList();
  FunDec *f;
  while ((f = parseFunDec()) != nullptr) {
    list->add(f);
  }
  return list;
}

ClassDec *Parser::parseClassDec() {
  auto name = consume(Token::ID, "Se esperaba nombre de clase")->text;
  std::vector<Argument>* args = new std::vector<Argument>();
  if (match(Token::PI)) {
    args = parseArguments();
    consume(Token::PD, "Se esperaba ')' tras argumentos de clase");
  }
  // 3) Cuerpo de la clase
  consume(Token::LBRACE, "Se esperaba '{' inicio de cuerpo de clase");

  // 3.a) Leemos N declaraciones var/val, separadas opcionalmente por ';'
  VarDecList *members = new VarDecList();
  while (check(Token::VAR) || check(Token::VAL)) {
    // parseVarDecList consume todas las declaraciones que encuentre
    VarDecList *partial = parseVarDecList();
    // merge partial->vars en members->vars
    for (auto *vd : partial->vars) {
      members->add(vd);
    }
    // si hay punto y coma, lo saltamos
    match(Token::PC);
  }

  // 3.b) Ahora sí el cierre de la clase
  consume(Token::RBRACE, "Se esperaba '}' fin de cuerpo de clase");

  return new ClassDec(name, *args, members);
}

// --- Declaraciones de función ---
FunDec *Parser::parseFunDec() {
  if (!match(Token::FUN))
    return nullptr;
  auto name = consume(Token::ID, "Se esperaba identificador de función")->text;
  consume(Token::PI, "Se esperaba '(' tras nombre de función");
  auto params = parseParamDecList();
  consume(Token::PD, "Se esperaba ')' tras lista de parámetros");
  std::string retType;
  if (match(Token::COLON)) {
    retType = consume(Token::ID, "Se esperaba tipo de retorno")->text;
  }

  Body* body = nullptr;
  if (match(Token::ASSIGN)) {
    Exp* e = parseCExp();
    auto *vdl = new VarDecList();
    auto *sl  = new StatementList();
    sl->add(new ReturnStatement(e));
    body = new Body(vdl, sl);
  }
  else if (match(Token::LBRACE)) {
    body = parseBody();
    consume(Token::RBRACE, "Se esperaba '}' fin de cuerpo de función");
  } else {
    error("Se esperaba '=' o '{' inicio de cuerpo de función");
  }
  
  return new FunDec(name, retType, *params, body);
}

vector<Param> *Parser::parseParamDecList() {
  auto params = new vector<Param>();
  if (check(Token::ID)) {
    do {
      auto pname = advance()->text;
      consume(Token::COLON, "Se esperaba ':' tras parámetro");
      auto ptype = consume(Token::ID, "Se esperaba tipo de parámetro")->text;
      params->push_back({pname, ptype});
    } while (match(Token::COMA));
  }
  return params;
}

// —————————————
// parseArguments: lista de x:Type [, y:Type]*
// —————————————
std::vector<Argument> *Parser::parseArguments() {
  auto args = new std::vector<Argument>();

  // si no hay ningún parámetro, devolvemos lista vacía
  if (!check(Token::ID)) {
    return args;
  }

  do {
    // nombre
    auto aname = consume(Token::ID, "Se esperaba nombre de argumento")->text;
    // dos puntos y tipo
    consume(Token::COLON, "Se esperaba ':' tras nombre de argumento");
    auto atype = consume(Token::ID, "Se esperaba tipo de argumento")->text;

    // soportar genéricos List<...>, Point<...>, etc.
    if (match(Token::LT)) {
      atype += "<" + consume(Token::ID, "Se esperaba tipo genérico")->text;
      while (match(Token::COMA)) {
        atype += "," + consume(Token::ID, "Se esperaba tipo genérico")->text;
      }
      consume(Token::GT, "Se esperaba '>' al final de tipo genérico");
      atype += ">";
    }

    args->push_back({aname, atype});
  } while (match(Token::COMA));

  return args;
}

// --- Cuerpo y sentencias ---

Body *Parser::parseBody() {
  auto vdl = parseVarDecList();
  auto sl = parseStmtList();
  return new Body(vdl, sl);
}

// --- Cuerpo y sentencias ---

StatementList *Parser::parseStmtList() {
  auto sl = new StatementList();
  while (!check(Token::RBRACE) && !isAtEnd()) {
    sl->add(parseStmt());
  }
  return sl;
}

Stm* Parser::parseStmt() {
    // -- 1) print / println (son Token::PRINT pero el texto es "print" o "println")
    if (match(Token::PRINT)) {
        bool isLn = (previous->text == "println");
        consume(Token::PI,  "Se esperaba '(' tras print");
        Exp* e = parseCExp();
        consume(Token::PD,  "Se esperaba ')' en print");
        return new PrintStatement(e);
    }
    // -- 2) asignaciones (índice, campo o simple)
    if (check(Token::ID)) {
        string name = advance()->text;

        // 2.a) foo[expr] = rhs
        if (match(Token::LBRACK)) {
            Exp* idx = parseCExp();
            consume(Token::RBRACK, "Se esperaba ']' en índice");
            consume(Token::ASSIGN, "Se esperaba '=' en asignación de índice");
            Exp* rhs = parseCExp();
            return new AssignStatement(new IndexExp(name, idx), rhs);
        }
        // 2.b) foo.bar = rhs
        else if (match(Token::DOT)) {
            string member = consume(Token::ID, "Se esperaba miembro tras '.'")->text;
            consume(Token::ASSIGN, "Se esperaba '=' en asignación de campo");
            Exp* rhs = parseCExp();
            return new AssignStatement(new DotExp(name, member), rhs);
        }
        // 2.c) foo = rhs
        else if (match(Token::ASSIGN)) {
            Exp* rhs = parseCExp();
            return new AssignStatement(new IdentifierExp(name), rhs);
        }
        else {
            error("Después del identificador se esperaba '=' para asignación");
        }
    }
    // -- 3) return
    if (match(Token::RETURN)) {
        Exp* e = parseCExp();
        return new ReturnStatement(e);
    }
    // -- 4) if / while / for ...
    if (match(Token::IF)) {
      consume(Token::PI, "Se esperaba '(' en if");
      Exp* cond = parseCExp();
      consume(Token::PD, "Se esperaba ')' en if");
      Body* thenB = nullptr;
      if (match(Token::LBRACE)) {
        thenB = parseBody();
        consume(Token::RBRACE, "Se esperaba '}' fin then");
      } else {
        Stm* s = parseStmt();
        auto* vdl = new VarDecList();
        auto* sl  = new StatementList();
        sl->add(s);
        thenB = new Body(vdl, sl);
      }
      Body* elseB = nullptr;
      if (match(Token::ELSE)) {
        if (match(Token::LBRACE)) {
          elseB = parseBody();
          consume(Token::RBRACE, "Se esperaba '}' fin else");
        } else {
          Stm* s = parseStmt();
          auto* vdl = new VarDecList();
          auto* sl  = new StatementList();
          sl->add(s);
          elseB = new Body(vdl, sl);
        }
      }
      return new IfStatement(cond, thenB, elseB);
    }
    if (match(Token::WHILE)) {
        consume(Token::PI, "Se esperaba '(' en while");
        auto cond = parseCExp();
        consume(Token::PD, "Se esperaba ')' tras condición");
        consume(Token::LBRACE, "Se esperaba '{' tras while");
        auto body = parseBody();
        consume(Token::RBRACE, "Se esperaba '}' fin while");
        return new WhileStatement(cond, body);
    }
    if (match(Token::FOR)) {
        consume(Token::PI, "Se esperaba '(' en for");
        string var = consume(Token::ID, "Se esperaba identificador en for")->text;
        consume(Token::ID, "Se esperaba 'in' en for");
        Stm* forSt = nullptr;
        if (check(Token::NUM) || check(Token::PI)) {
            auto loop = parseLoopExp();
            consume(Token::PD, "Se esperaba ')' de for");
            consume(Token::LBRACE, "Se esperaba '{' tras for");
            auto body = parseBody();
            consume(Token::RBRACE, "Se esperaba '}' fin for");
            forSt = new ForStatement(var, loop, body);
        } else {
            auto listName = consume(Token::ID, "Se esperaba lista en for")->text;
            consume(Token::PD, "Se esperaba ')' tras for");
            consume(Token::LBRACE, "Se esperaba '{' tras for");
            auto body = parseBody();
            consume(Token::RBRACE, "Se esperaba '}' fin for");
            forSt = new ForStatement(var, new IdentifierExp(listName), body);
        }
        return forSt;
    }

    error("Sentencia no reconocida");
    return nullptr;
}

// --- Expresiones ---

Exp *Parser::parseCExp() {
  auto left = parseExpression();
  if (match(Token::GT) || match(Token::LT) || match(Token::LE) ||
      match(Token::EQ)) {
    auto op = previous->type == Token::GT   ? GT_OP
              : previous->type == Token::LT ? LT_OP
              : previous->type == Token::LE ? LE_OP
                                            : EQ_OP;
    auto right = parseExpression();
    left = new BinaryExp(left, right, op);
  }
  return left;
}

Exp *Parser::parseExpression() {
  auto left = parseTerm();
  while (match(Token::PLUS) || match(Token::MINUS)) {
    auto op = previous->type == Token::PLUS ? PLUS_OP : MINUS_OP;
    auto right = parseTerm();
    left = new BinaryExp(left, right, op);
  }
  return left;
}

Exp *Parser::parseTerm() {
  auto left = parseFactor();
  while (match(Token::MUL) || match(Token::DIV)) {
    auto op = previous->type == Token::MUL ? MUL_OP : DIV_OP;
    auto right = parseFactor();
    left = new BinaryExp(left, right, op);
  }
  return left;
}

Exp *Parser::parseFactor() {
  // 1) Literales básicos
  if (match(Token::TRUE)) {
    return new BoolExp(true);
  } else if (match(Token::FALSE)) {
    return new BoolExp(false);
  } else if (match(Token::STRING)) {
    return new StringExp(previous->text);
  } else if (match(Token::NUM)) {
    return new NumberExp(stoi(previous->text));
  }

  // 2) IntArray(size) { it -> ... }
  else if (match(Token::ID, "IntArray")) {
    consume(Token::PI,   "Se esperaba '(' tras IntArray");
    Exp* sizeExp = parseCExp();
    auto *num = dynamic_cast<NumberExp*>(sizeExp);
    if (!num) error("El primer argumento de IntArray debe ser un literal numérico");
    int n = num->value;
    consume(Token::PD,   "Se esperaba ')' tras tamaño de IntArray");

    consume(Token::LBRACE, "Se esperaba '{' tras IntArray(...)");
    Exp* body = parseCExp();
    consume(Token::RBRACE, "Se esperaba '}' al final de la lambda de IntArray");

    auto *le = new ListExp(false);
    for (int i = 0; i < n; ++i) {
      std::function<Exp*(Exp*)> subst = [&](Exp* e)->Exp* {
        if (auto *v = dynamic_cast<IdentifierExp*>(e)) {
          if (v->name == "it") return new NumberExp(i);
          return new IdentifierExp(v->name);
        }
        if (auto *b = dynamic_cast<BinaryExp*>(e)) {
          return new BinaryExp(
            subst(b->left),
            subst(b->right),
            b->op
          );
        }
        if (auto *n2 = dynamic_cast<NumberExp*>(e)) {
          return new NumberExp(n2->value);
        }
        // Si tienes otros Exp, trátalos aquí…
        return nullptr;
      };
      le->add(subst(body));
    }
    return le;
  }

  // 3) DoubleArray(size) { it -> ... }
  else if (match(Token::ID, "DoubleArray")) {
    consume(Token::PI,   "Se esperaba '(' tras DoubleArray");
    Exp* sizeExp = parseCExp();
    auto *num = dynamic_cast<NumberExp*>(sizeExp);
    if (!num) error("El primer argumento de DoubleArray debe ser un literal numérico");
    int n = num->value;
    consume(Token::PD,   "Se esperaba ')' tras tamaño de DoubleArray");

    consume(Token::LBRACE, "Se esperaba '{' tras DoubleArray(...)");
    Exp* body = parseCExp();
    consume(Token::RBRACE, "Se esperaba '}' al final de la lambda de DoubleArray");

    auto *le = new ListExp(false);
    for (int i = 0; i < n; ++i) {
      std::function<Exp*(Exp*)> subst = [&](Exp* e)->Exp* {
        if (auto *v = dynamic_cast<IdentifierExp*>(e)) {
          if (v->name == "it") return new NumberExp(i);
          return new IdentifierExp(v->name);
        }
        if (auto *b = dynamic_cast<BinaryExp*>(e)) {
          return new BinaryExp(
            subst(b->left),
            subst(b->right),
            b->op
          );
        }
        if (auto *n2 = dynamic_cast<NumberExp*>(e)) {
          return new NumberExp(n2->value);
        }
        return nullptr;
      };
      le->add(subst(body));
    }
    return le;
  }

  // 4) Array<T>(n) { lambda }  → siempre con 'it' implícito
  else if (match(Token::ID, "Array")) {
    // 1) Saltar genéricos <T>
    if (match(Token::LT)) {
      while (!check(Token::GT) && !isAtEnd()) advance();
      consume(Token::GT, "Se esperaba '>' tras parámetro genérico de Array");
    }

    // 2) Leer tamaño n
    consume(Token::PI, "Se esperaba '(' tras Array");
    Exp* sizeExp = parseCExp();
    auto *num = dynamic_cast<NumberExp*>(sizeExp);
    if (!num) error("El primer argumento de Array debe ser un literal numérico");
    int n = num->value;
    consume(Token::PD, "Se esperaba ')' tras tamaño de Array");

    // 3) Leer lambda { it * ... }
    consume(Token::LBRACE, "Se esperaba '{' tras Array(...)");
    Exp* body = parseCExp();
    consume(Token::RBRACE, "Se esperaba '}' al final de la lambda de Array");

    // 4) Expande a ListExp con 'it' implícito
    auto *le = new ListExp(false);
    for (int i = 0; i < n; ++i) {
      std::function<Exp*(Exp*)> subst = [&](Exp* e)->Exp* {
        if (auto *v = dynamic_cast<IdentifierExp*>(e)) {
          if (v->name == "it") return new NumberExp(i);
          return new IdentifierExp(v->name);
        }
        if (auto *b = dynamic_cast<BinaryExp*>(e)) {
          return new BinaryExp(
            subst(b->left),
            subst(b->right),
            b->op
          );
        }
        if (auto *n2 = dynamic_cast<NumberExp*>(e)) {
          return new NumberExp(n2->value);
        }
        return nullptr;
      };
      le->add(subst(body));
    }
    return le;
  }

  // 4) Fábricas de lista/array sin lambda: listOf, arrayOf, intArrayOf, doubleArrayOf, ...
  // mutableListOf (mutable)
  else if (match(Token::ID, "mutableListOf")) {
    consume(Token::PI, "Se esperaba '(' en mutableListOf");
    auto le = new ListExp(true);            // mutable!
    if (!check(Token::PD)) {
      do { le->add(parseCExp()); } while (match(Token::COMA));
    }
    consume(Token::PD, "Se esperaba ')' en mutableListOf");
    return le;
  }
  // 3) el resto de fábricas (arrays y demás)
  else if (
      match(Token::ID, "arrayOf")      || match(Token::ID, "intArrayOf")
    || match(Token::ID, "longArrayOf")  || match(Token::ID, "doubleArrayOf")
    || match(Token::ID, "booleanArrayOf") || match(Token::ID, "listOf")
  ) {
    std::string fn = previous->text;
    consume(Token::PI, "Se esperaba '(' en " + fn);
    auto le = new ListExp(false);          // aquí siempre false
    if (!check(Token::PD)) {
      do { le->add(parseCExp()); } while (match(Token::COMA));
    }
    consume(Token::PD, "Se esperaba ')' en " + fn);
    return le;
  }

  // 5) Identificador / llamada / index / member
  else if (match(Token::ID)) {
    auto name = previous->text;
    if (match(Token::PI)) {
      auto call = new FCallExp(name);
      if (!check(Token::PD)) {
        do {
          call->add(parseCExp());
        } while (match(Token::COMA));
      }
      consume(Token::PD, "Se esperaba ')' en llamada");
      return call;
    }
    if (match(Token::LBRACK)) {
      auto idx = parseCExp();
      consume(Token::RBRACK, "Se esperaba ']' en index");
      return new IndexExp(name, idx);
    }
    if (match(Token::DOT)) {
      auto member = consume(Token::ID, "Se esperaba miembro tras '.'")->text;
      return new DotExp(name, member);
    }
    return new IdentifierExp(name);
  }

  // 6) Inline if
  else if (match(Token::IF)) {
    consume(Token::PI, "Se esperaba '(' en inline if");
    auto cond = parseCExp();
    consume(Token::PD, "Se esperaba ')' tras condición de inline if");
    auto thenExpr = parseCExp();
    consume(Token::ELSE, "Se esperaba 'else' en inline if");
    auto elseExpr = parseCExp();
    return new IFExp(cond, thenExpr, elseExpr);
  }

  // 7) Paréntesis
  else if (match(Token::PI)) {
    auto e = parseCExp();
    consume(Token::PD, "Falta ')'");
    return e;
  }

  error("Factor no reconocido");
  return nullptr;
}

vector<Exp *> Parser::parseArgList() {
  vector<Exp *> elems;
  do {
    elems.push_back(parseCExp());
  } while (match(Token::COMA));
  return elems;
}

LoopExp *Parser::parseLoopExp() {
  auto start = parseCExp();
  bool downTo = match(Token::ID, "downTo");
  if (!downTo) {
    consume(Token::DOTDOT, "Se esperaba '..' o 'downTo'");
  }
  auto end = parseCExp();
  Exp *step = nullptr;
  if (match(Token::ID, "step")) {
    step = parseCExp();
  }
  return new LoopExp(start, end, step, downTo);
}
