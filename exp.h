// exp.h
#ifndef EXP_H
#define EXP_H

#include <string>
#include <vector>

// Forward-declaration del Visitor
class Visitor;

// -----------------------------------------------------------------------------
// Nodo base de expresiones
// -----------------------------------------------------------------------------
class Exp {
public:
    virtual int accept(Visitor* v) = 0;
    virtual ~Exp();
    static std::string binopToChar(int op);
};

enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, LT_OP, LE_OP, EQ_OP };

class IFExp : public Exp {
public:
    Exp* cond;
    Exp* left;
    Exp* right;
    IFExp(Exp* cond, Exp* left, Exp* right);
    int accept(Visitor* v) override;
    ~IFExp();
};

class BinaryExp : public Exp {
public:
    Exp*      left;
    Exp*      right;
    BinaryOp  op;
    BinaryExp(Exp* left, Exp* right, BinaryOp op);
    int accept(Visitor* v) override;
    ~BinaryExp();
};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int value);
    int accept(Visitor* v) override;
    ~NumberExp();
};

class BoolExp : public Exp {
public:
    bool value;
    BoolExp(bool value);
    int accept(Visitor* v) override;
    ~BoolExp();
};

class IdentifierExp : public Exp {
public:
    std::string name;
    IdentifierExp(const std::string& name);
    int accept(Visitor* v) override;
    ~IdentifierExp();
};

class FCallExp : public Exp {
public:
    std::string         name;
    std::vector<Exp*>   args;
    FCallExp(const std::string& name);
    void add(Exp* arg);
    int accept(Visitor* v) override;
    ~FCallExp();
};

class ListExp : public Exp {
public:
    bool                isMutable;
    std::vector<Exp*>   elements;
    ListExp(bool isMutable);
    void add(Exp* elem);
    int accept(Visitor* v) override;
    ~ListExp();
};

class IndexExp : public Exp {
public:
    std::string name;
    Exp*        index;
    IndexExp(const std::string& name, Exp* index);
    int accept(Visitor* v) override;
    ~IndexExp();
};

class DotExp : public Exp {
public:
    std::string object;
    std::string member;
    DotExp(const std::string& object, const std::string& member);
    int accept(Visitor* v) override;
    ~DotExp();
};

class LoopExp : public Exp {
public:
    Exp*   start;
    Exp*   end;
    Exp*   step;    // puede ser nullptr
    bool   downTo;
    LoopExp(Exp* start, Exp* end, Exp* step, bool downTo);
    int accept(Visitor* v) override;
    ~LoopExp();
};

// -----------------------------------------------------------------------------
// Nodo base de sentencias
// -----------------------------------------------------------------------------
class Stm {
public:
    virtual int accept(Visitor* v) = 0;
    virtual ~Stm();
};

class AssignStatement : public Stm {
public:
    std::string target;
    Exp*        expr;
    AssignStatement(const std::string& target, Exp* expr);
    int accept(Visitor* v) override;
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp* expr;
    PrintStatement(Exp* expr);
    int accept(Visitor* v) override;
    ~PrintStatement();
};

class ReturnStatement : public Stm {
public:
    Exp* expr;  // puede ser nullptr
    ReturnStatement(Exp* expr);
    int accept(Visitor* v) override;
    ~ReturnStatement();
};

class IfStatement : public Stm {
public:
    Exp*   condition;
    class Body* thenBranch;
    class Body* elseBranch;  // puede ser nullptr
    IfStatement(Exp* condition, Body* thenBranch, Body* elseBranch);
    int accept(Visitor* v) override;
    ~IfStatement();
};

class WhileStatement : public Stm {
public:
    Exp*   condition;
    class Body* body;
    WhileStatement(Exp* condition, Body* body);
    int accept(Visitor* v) override;
    ~WhileStatement();
};

class ForStatement : public Stm {
public:
    std::string varName;
    Exp*        iterable;
    class Body* body;
    ForStatement(const std::string& varName, Exp* iterable, Body* body);
    int accept(Visitor* v) override;
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
    bool              isMutable;
    std::string       name;
    std::string       typeName;
    Exp*              init;    // puede ser nullptr
    VarDec(bool isMutable, const std::string& name, const std::string& typeName, Exp* init);
    int accept(Visitor* v);  // <-- sin override
    ~VarDec();
};

class VarDecList {
public:
    std::vector<VarDec*> vars;
    VarDecList();
    void add(VarDec* var);
    int accept(Visitor* v);  // <-- sin override
    ~VarDecList();
};

class ClassDec {
public:
    std::string             name;
    std::vector<Argument>   args;
    VarDecList*             members;
    ClassDec(const std::string& name, const std::vector<Argument>& args, VarDecList* members);
    int accept(Visitor* v);  // <-- sin override
    ~ClassDec();
};

class ClassDecList {
public:
    std::vector<ClassDec*> classes;
    ClassDecList();
    void add(ClassDec* cls);
    int accept(Visitor* v);  // <-- sin override
    ~ClassDecList();
};

class FunDec {
public:
    std::string           name;
    std::string           retType;
    std::vector<Param>    params;
    class Body*           body;
    FunDec(const std::string& name, const std::string& retType, const std::vector<Param>& params, Body* body);
    int accept(Visitor* v);  // <-- sin override
    ~FunDec();
};

class FunDecList {
public:
    std::vector<FunDec*> functions;
    FunDecList();
    void add(FunDec* fn);
    int accept(Visitor* v);  // <-- sin override
    ~FunDecList();
};

class StatementList {
public:
    std::vector<Stm*> statements;
    StatementList();
    void add(Stm* stmt);
    int accept(Visitor* v);  // <-- sin override
    ~StatementList();
};

// -----------------------------------------------------------------------------
// Body y Program
// -----------------------------------------------------------------------------
class Body {
public:
    VarDecList*     vardecs;
    StatementList*  stmts;
    Body(VarDecList* vardecs, StatementList* stmts);
    int accept(Visitor* v);  // <-- sin override
    ~Body();
};

class Program {
public:
    VarDecList*     vardecs;
    ClassDecList*   classDecs;
    FunDecList*     funDecs;
    Program();
    int accept(Visitor* v);  // <-- sin override
    ~Program();
};

#endif // EXP_H
