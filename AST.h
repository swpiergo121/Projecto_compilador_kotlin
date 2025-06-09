// AST.h
#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

struct ASTNode { virtual ~ASTNode() = default; };

// ----- Types -----
struct TypeNode : ASTNode { virtual ~TypeNode() = default; };
struct CustomTypeNode : TypeNode {
    std::string name;
    CustomTypeNode(const std::string &n): name(n) {}
};

// ----- Declarations -----
struct Expr;
struct Stmt;

struct VarDecl : ASTNode {
    bool isConst;
    std::string name;
    std::unique_ptr<TypeNode> type;
    std::unique_ptr<Expr> init;
    VarDecl(bool isC,
            const std::string &n,
            std::unique_ptr<TypeNode> t,
            std::unique_ptr<Expr> i)
        : isConst(isC), name(n), type(std::move(t)), init(std::move(i)) {}
};

struct FunctionDecl : ASTNode {
    std::string name;
    std::vector<std::pair<std::string, std::unique_ptr<TypeNode>>> params;
    std::unique_ptr<TypeNode> returnType;
    std::vector<std::unique_ptr<ASTNode>> body;
    FunctionDecl(const std::string &n) : name(n) {}
};

struct StructDecl : ASTNode {
    std::string name;
    std::vector<std::pair<std::string, std::unique_ptr<TypeNode>>> fields;
    StructDecl(const std::string &n) : name(n) {}
};

// ----- Statements -----
struct Stmt : ASTNode { virtual ~Stmt() = default; };

struct ExprStmt : Stmt {
    std::unique_ptr<Expr> expr;
    explicit ExprStmt(std::unique_ptr<Expr> e) : expr(std::move(e)) {}
};

struct IfStmt : Stmt {
    std::unique_ptr<Expr> cond;
    std::vector<std::unique_ptr<Stmt>> thenBranch, elseBranch;
    IfStmt(std::unique_ptr<Expr> c,
           std::vector<std::unique_ptr<Stmt>> t,
           std::vector<std::unique_ptr<Stmt>> e)
        : cond(std::move(c)), thenBranch(std::move(t)), elseBranch(std::move(e)) {}
};

struct WhileStmt : Stmt {
    std::unique_ptr<Expr> cond;
    std::vector<std::unique_ptr<Stmt>> body;
    WhileStmt(std::unique_ptr<Expr> c,
              std::vector<std::unique_ptr<Stmt>> b)
        : cond(std::move(c)), body(std::move(b)) {}
};

struct ForRangeStmt : Stmt {
    std::string var;
    std::unique_ptr<Expr> start, end;
    std::vector<std::unique_ptr<Stmt>> body;
    ForRangeStmt(std::string v,
                 std::unique_ptr<Expr> s,
                 std::unique_ptr<Expr> e,
                 std::vector<std::unique_ptr<Stmt>> b)
      : var(std::move(v)),
        start(std::move(s)),
        end(std::move(e)),
        body(std::move(b)) {}
};

struct ForEachStmt : Stmt {
    std::string var;
    std::unique_ptr<Expr> collection;
    std::vector<std::unique_ptr<Stmt>> body;
    ForEachStmt(std::string v,
                std::unique_ptr<Expr> coll,
                std::vector<std::unique_ptr<Stmt>> b)
      : var(std::move(v)),
        collection(std::move(coll)),
        body(std::move(b)) {}
};


struct ReturnStmt : Stmt {
    std::unique_ptr<Expr> expr;
    explicit ReturnStmt(std::unique_ptr<Expr> e) : expr(std::move(e)) {}
};

// ----- Expressions -----
enum class BinaryOp { ADD, SUB, MUL, DIV, MOD, LT, GT, LTEQ, GTEQ, EQ, NEQ };

struct Expr : ASTNode { virtual ~Expr() = default; };

struct IntLiteral : Expr {
    long long value;
    explicit IntLiteral(long long v) : value(v) {}
};

struct VarExpr : Expr {
    std::string name;
    explicit VarExpr(const std::string &n) : name(n) {}
};

struct BinaryExpr : Expr {
    BinaryOp op;
    std::unique_ptr<Expr> left, right;
    BinaryExpr(BinaryOp o,
               std::unique_ptr<Expr> l,
               std::unique_ptr<Expr> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

struct CallExpr : Expr {
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;
    explicit CallExpr(const std::string &c) : callee(c) {}
};

struct ArrayAccessExpr : Expr {
    std::unique_ptr<Expr> array, index;
    ArrayAccessExpr(std::unique_ptr<Expr> a,
                    std::unique_ptr<Expr> i)
        : array(std::move(a)), index(std::move(i)) {}
};

struct FieldAccessExpr : Expr {
    std::unique_ptr<Expr> object;
    std::string field;
    FieldAccessExpr(std::unique_ptr<Expr> o,
                    const std::string &f)
        : object(std::move(o)), field(f) {}
};

struct NewArrayExpr : Expr {
    std::unique_ptr<TypeNode> elemType;
    std::unique_ptr<Expr> size;
    NewArrayExpr(std::unique_ptr<TypeNode> et,
                 std::unique_ptr<Expr> s)
        : elemType(std::move(et)), size(std::move(s)) {}
};

#endif // AST_H