// parser.cpp
#include "parser.h"
#include <stdexcept>
#include <iostream>

using namespace std;

Parser::Parser(Scanner* sc)
  : scanner(sc),
    current(scanner->nextToken()),
    previous(nullptr)
{
    if (current->type == Token::ERR)
        throw runtime_error("Error léxico inicial: " + current->text);
}

Program* Parser::parse() {
    return parseProgram();
}

// --- Helpers ---

bool Parser::match(Token::Type type) {
    if (check(type)) { advance(); return true; }
    return false;
}

bool Parser::match(Token::Type type, const string& lexeme) {
    if (!isAtEnd() && current->type == type && current->text == lexeme) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type type) const {
    return !isAtEnd() && current->type == type;
}

Token* Parser::advance() {
    previous = current;
    current = scanner->nextToken();
    if (current->type == Token::ERR)
        error("Carácter no reconocido: " + current->text);
    return previous;
}

Token* Parser::consume(Token::Type type, const string& message) {
    if (check(type)) return advance();
    error(message);
    return nullptr;
}

bool Parser::isAtEnd() const {
    return current->type == Token::END;
}

void Parser::error(const string& msg) {
    cerr << "[Line ??] Error de sintaxis: " << msg << endl;
    exit(1);
}

// --- Programa principal ---
Program* Parser::parseProgram() {
    // 1) variables globales
    VarDecList* globals  = parseVarDecList();

    // 2) clases globales (si las hubiera)
    ClassDecList* classes = parseClassDecList();

    // 3) TODAS las funciones globales (incluido main)
    FunDecList* funcs     = parseFunDecList();

    // 4) construir el AST
    Program* prog         = new Program(nullptr);
    prog->vardecs         = globals;
    prog->classDecs       = classes;
    prog->funDecs         = funcs;

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

    while (match(Token::VAR) || match(Token::ID, "val")) {
        bool isMutable = (previous->text == "var");
        auto varName = consume(Token::ID, "Se esperaba nombre de variable")->text;
        consume(Token::COLON, "Se esperaba ':' tras nombre de variable");
        std::string typeName = consume(Token::ID, "Se esperaba tipo")->text;
        if (match(Token::LT)) {
            typeName += "<" + consume(Token::ID, "Se esperaba tipo genérico")->text;
            while (match(Token::COMA)) {
                typeName += "," + consume(Token::ID, "Se esperaba tipo genérico")->text;
            }
            consume(Token::GT, "Se esperaba '>' al final de tipo genérico");
            typeName += ">";
        }

        Exp* init = nullptr;
        if (match(Token::ASSIGN)) {
            init = parseCExp();
        }

        consume(Token::PC, "Se esperaba ';' al final de la declaración");
        list->add(new VarDec(isMutable, varName, typeName, init));
    }

    return list;
}


VarDec* Parser::parseVarDec() {
    bool isMutable = match(Token::VAR);
    if (!isMutable && previous && previous->text != "val")
        consume(Token::VAR, "Se esperaba 'var' o 'val'");
    auto name = consume(Token::ID, "Se esperaba identificador")->text;
    consume(Token::COLON, "Se esperaba ':' tras nombre de variable");
    auto typeName = consume(Token::ID, "Se esperaba nombre de tipo")->text;
    Exp* init = nullptr;
    if (match(Token::ASSIGN)) {
        init = parseCExp();
    }
    consume(Token::PC, "Se esperaba ';' al final de VarDec");
    return new VarDec(isMutable, name, typeName, init);
}

// --- Declaraciones de clase ---

ClassDecList* Parser::parseClassDecList() {
    auto list = new ClassDecList();
    // Mientras veamos la palabra “class”
    while (check(Token::ID) && current->text == "class") {
        advance();  // consumimos “class”
        list->add(parseClassDec());
    }
    return list;
}

FunDecList* Parser::parseFunDecList() {
    auto list = new FunDecList();
    FunDec* f;
    while ((f = parseFunDec()) != nullptr) {
        list->add(f);
    }
    return list;
}

ClassDec* Parser::parseClassDec() {
    auto name = consume(Token::ID,"Se esperaba nombre de clase")->text;
    // 2) paréntesis con parámetros
    consume(Token::PI, "Se esperaba '(' tras nombre de clase");
    auto args = parseArguments();
    consume(Token::PD, "Se esperaba ')' tras argumentos de clase");
    // 3) cuerpo de la clase
    consume(Token::LBRACE, "Se esperaba '{' inicio de cuerpo de clase");
    auto members = parseVarDecList();
    consume(Token::RBRACE, "Se esperaba '}' fin de cuerpo de clase");

    return new ClassDec(name, *args, members);
}

// --- Declaraciones de función ---
FunDec* Parser::parseFunDec() {
    if (!match(Token::FUN)) return nullptr;
    auto name   = consume(Token::ID, "Se esperaba identificador de función")->text;
    consume(Token::PI, "Se esperaba '(' tras nombre de función");
    auto params = parseParamDecList();
    consume(Token::PD, "Se esperaba ')' tras lista de parámetros");
    std::string retType;
    if (match(Token::COLON)) {
        retType = consume(Token::ID, "Se esperaba tipo de retorno")->text;
    }

    consume(Token::LBRACE, "Se esperaba '{' inicio de cuerpo de función");
    Body* body = parseBody();
    consume(Token::RBRACE, "Se esperaba '}' fin de cuerpo de función");

    return new FunDec(name, retType, *params, body);
}

vector<Param>* Parser::parseParamDecList() {
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
std::vector<Argument>* Parser::parseArguments() {
    auto args = new std::vector<Argument>();

    // si no hay ningún parámetro, devolvemos lista vacía
    if (!check(Token::ID)) {
        return args;
    }

    do {
        // nombre
        auto aname = consume(Token::ID,"Se esperaba nombre de argumento")->text;
        // dos puntos y tipo
        consume(Token::COLON,"Se esperaba ':' tras nombre de argumento");
        auto atype = consume(Token::ID,"Se esperaba tipo de argumento")->text;

        // soportar genéricos List<...>, Point<...>, etc.
        if (match(Token::LT)) {
            atype += "<" + consume(Token::ID,"Se esperaba tipo genérico")->text;
            while (match(Token::COMA)) {
                atype += "," + consume(Token::ID,"Se esperaba tipo genérico")->text;
            }
            consume(Token::GT,"Se esperaba '>' al final de tipo genérico");
            atype += ">";
        }

        args->push_back({aname, atype});
    } while (match(Token::COMA));

    return args;
}

// --- Cuerpo y sentencias ---

Body* Parser::parseBody() {
    auto vdl = parseVarDecList();
    auto sl  = parseStmtList();
    return new Body(vdl, sl);
}

// --- Cuerpo y sentencias ---

StatementList* Parser::parseStmtList() {
    auto sl = new StatementList();
    while (!check(Token::RBRACE) && !isAtEnd()) {
        sl->add(parseStmt());
    }
    return sl;
}

Stm* Parser::parseStmt() {
    if (check(Token::ID)) {
        auto id = advance()->text;
        if (match(Token::ASSIGN)) {
            Exp* e = parseCExp();
            consume(Token::PC, "Falta ';' en asignación");
            return new AssignStatement(new IdentifierExp(id), e);
        } else {
            error("Después del identificador se esperaba '=' para asignación");
        }
    }
    if (match(Token::PRINT) || match(Token::ID, "println")) {
        consume(Token::PI, "Se esperaba '(' tras print");
        Exp* e = parseCExp();
        consume(Token::PD, "Se esperaba ')' en print");
        consume(Token::PC, "Falta ';' en print");
        return new PrintStatement(e);
    }
    if (match(Token::RETURN)) {
        Exp* e = nullptr;
        if (check(Token::PI)) {
            advance();
            if (!check(Token::PD)) {
                e = parseCExp();
            }
            consume(Token::PD, "Se esperaba ')' tras return");
        } else if (!check(Token::PC)) {
            e = parseCExp();
        }
        consume(Token::PC, "Falta ';' en return");
        return new ReturnStatement(e);
    }
    if (match(Token::IF)) {
        consume(Token::PI, "Se esperaba '(' en if");
        auto cond = parseCExp();
        consume(Token::PD, "Se esperaba ')' en if");
        consume(Token::LBRACE, "Se esperaba '{' tras if");
        auto thenB = parseBody();
        consume(Token::RBRACE, "Se esperaba '}' fin then");
        Body* elseB = nullptr;
        if (match(Token::ELSE)) {
            consume(Token::LBRACE, "Se esperaba '{' tras else");
            elseB = parseBody();
            consume(Token::RBRACE, "Se esperaba '}' fin else");
        }
        return new IfStatement(cond, thenB, elseB);
    }
    if (match(Token::WHILE)) {
        consume(Token::PI, "Se esperaba '(' en while");
        auto cond = parseCExp();
        consume(Token::PD, "Se esperaba ')' en while");
        consume(Token::LBRACE, "Se esperaba '{' tras while");
        auto body = parseBody();
        consume(Token::RBRACE, "Se esperaba '}' fin while");
        return new WhileStatement(cond, body);
    }
    if (match(Token::FOR)) {
        consume(Token::PI, "Se esperaba '(' en for");
        auto iterVar = consume(Token::ID, "Se esperaba identificador en for")->text;
        consume(Token::ID, "Se esperaba 'in' en for");
        Stm* forStmt = nullptr;
        if (check(Token::NUM) || check(Token::PI)) {
            auto loop = parseLoopExp();
            consume(Token::PD, "Se esperaba ')' de for");
            consume(Token::LBRACE, "Se esperaba '{' tras for");
            auto body = parseBody();
            consume(Token::RBRACE, "Se esperaba '}' fin for");
            forStmt = new ForStatement(iterVar, loop, body);
        } else {
            auto listName = consume(Token::ID, "Se esperaba lista en for")->text;
            consume(Token::PD, "Se esperaba ')' tras for");
            consume(Token::LBRACE, "Se esperaba '{' tras for");
            auto body = parseBody();
            consume(Token::RBRACE, "Se esperaba '}' fin for");
            forStmt = new ForStatement(iterVar, new IdentifierExp(listName), body);
        }
        return forStmt;
    }
    error("Sentencia no reconocida");
    return nullptr;
}

// --- Expresiones ---

Exp* Parser::parseCExp() {
    auto left = parseExpression();
    if (match(Token::GT) || match(Token::LT) || match(Token::LE) || match(Token::EQ)) {
        auto op = previous->type == Token::GT ? GT_OP
            : previous->type == Token::LT ? LT_OP
            : previous->type == Token::LE ? LE_OP
            : EQ_OP;
        auto right = parseExpression();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseExpression() {
    auto left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        auto op = previous->type == Token::PLUS ? PLUS_OP : MINUS_OP;
        auto right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    auto left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        auto op = previous->type == Token::MUL ? MUL_OP : DIV_OP;
        auto right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    if (match(Token::TRUE))  return new BoolExp(true);
    if (match(Token::FALSE)) return new BoolExp(false);
    if (match(Token::STRING)) {
        return new StringExp(previous->text);
    }
    if (match(Token::NUM))    return new NumberExp(stoi(previous->text));
    if (match(Token::ID, "listOf") || match(Token::ID, "mutableListOf")) {
        bool mut = previous->text == "mutableListOf";
        consume(Token::PI, "Se esperaba '(' en listOf");
        auto le = new ListExp(mut);
        if (!check(Token::PD)) {
            do {
                le->add(parseCExp());
            } while (match(Token::COMA));
        }
        consume(Token::PD, "Se esperaba ')' en listOf");
        return le;
    }
    if (match(Token::ID)) {
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
    if ( match(Token::IF) ) {
        consume(Token::PI,    "Se esperaba '(' en inline if");
        auto cond     = parseCExp();
        consume(Token::PD,    "Se esperaba ')' tras condición de inline if");
        auto thenExpr = parseCExp();
        consume(Token::ELSE,  "Se esperaba 'else' en inline if");
        auto elseExpr = parseCExp();
        return new IFExp(cond, thenExpr, elseExpr);
    }
    if (match(Token::PI)) {
        auto e = parseCExp();
        consume(Token::PD, "Falta ')'");
        return e;
    }
    error("Factor no reconocido");
    return nullptr;
}

vector<Exp*> Parser::parseArgList() {
    vector<Exp*> elems;
    do {
        elems.push_back(parseCExp());
    } while (match(Token::COMA));
    return elems;
}

LoopExp* Parser::parseLoopExp() {
    auto start = parseCExp();
    bool downTo = match(Token::ID, "downTo");
    if (!downTo) {
        consume(Token::DOTDOT, "Se esperaba '..' o 'downTo'");
    }
    auto end = parseCExp();
    Exp* step = nullptr;
    if (match(Token::ID, "step")) {
        step = parseCExp();
    }
    return new LoopExp(start, end, step, downTo);
}
