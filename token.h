// token.h
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <ostream>

class Token {
public:
    enum Type {
        ERR, END,
        ID, NUM,
        STRING,
        TRUE, FALSE,
        FUN, VAR,
        IF, ELSE, WHILE, FOR, RETURN, PRINT,
        PLUS, MINUS, MUL, DIV,
        GT, LT, LE, EQ, GE,        // >, <, <=, ==, >=
        ASSIGN,            // =
        PI, PD,            // '(' , ')'
        LBRACE, RBRACE,    // '{' , '}'
        LBRACK, RBRACK,    // '[' , ']'
        COMA, PC,          // ',' , ';'
        COLON,             // ':'
        DOT, DOTDOT        // '.' , '..'
    };

    Type        type;
    std::string text;

    // constructores
    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string &source, int first, int last);

    // impresión
    friend std::ostream &operator<<(std::ostream &outs, const Token &tok);
    friend std::ostream &operator<<(std::ostream &outs, const Token *tok);
};

#endif // TOKEN_H
