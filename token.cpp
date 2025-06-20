// token.cpp
#include "token.h"
#include <iostream>

using namespace std;

// --- constructores ---

Token::Token(Type type)
  : type(type), text("") {}

Token::Token(Type type, char c)
  : type(type), text(string(1, c)) {}

Token::Token(Type type, const string &source, int first, int last)
  : type(type)
{
    text = source.substr(first, last - first);
}

// --- operadores de salida ---

ostream& operator<<(ostream &outs, const Token &tok) {
    switch (tok.type) {
        case Token::PLUS:outs << "TOKEN(PLUS," << tok.text << ")"; break;
        case Token::MINUS:outs << "TOKEN(MINUS," << tok.text << ")"; break;
        case Token::MUL:outs << "TOKEN(MUL," << tok.text << ")"; break;
        case Token::DIV:outs << "TOKEN(DIV," << tok.text << ")"; break;
        case Token::NUM:outs << "TOKEN(NUM," << tok.text << ")"; break;
        case Token::ID:outs << "TOKEN(ID," << tok.text << ")"; break;
        case Token::ERR:outs << "TOKEN(ERR," << tok.text << ")"; break;
        case Token::END:outs << "TOKEN(END)"; break;

        case Token::TRUE:outs << "TOKEN(TRUE)"; break;
        case Token::FALSE:outs << "TOKEN(FALSE)"; break;

        case Token::FUN:outs << "TOKEN(FUN)"; break;
        case Token::VAR:outs << "TOKEN(VAR)"; break;
        case Token::VAL:outs << "TOKEN(VAL)"; break;
        case Token::IF:outs << "TOKEN(IF)"; break;
        case Token::ELSE:outs << "TOKEN(ELSE)"; break;
        case Token::WHILE:outs << "TOKEN(WHILE)"; break;
        case Token::FOR:outs << "TOKEN(FOR)"; break;
        case Token::RETURN:outs << "TOKEN(RETURN)"; break;
        case Token::PRINT:outs << "TOKEN(PRINT)"; break;

        case Token::GT:outs << "TOKEN(GT)"; break;  
        case Token::LT:outs << "TOKEN(LT)"; break;
        case Token::LE:outs << "TOKEN(LE)"; break;
        case Token::EQ:outs << "TOKEN(EQ)"; break;
        case Token::GE:outs << "TOKEN(GE)"; break;

        case Token::ASSIGN:outs << "TOKEN(ASSIGN)"; break;
        case Token::COLON:outs << "TOKEN(COLON)"; break;

        case Token::PI:outs << "TOKEN(LPAREN)"; break;
        case Token::PD:outs << "TOKEN(RPAREN)"; break;
        case Token::LBRACE:outs << "TOKEN(LBRACE)"; break;
        case Token::RBRACE:outs << "TOKEN(RBRACE)"; break;
        case Token::LBRACK:outs << "TOKEN(LBRACK)"; break;
        case Token::RBRACK:outs << "TOKEN(RBRACK)"; break;

        case Token::COMA:outs << "TOKEN(COMMA)"; break;
        case Token::PC:outs << "TOKEN(SEMICOLON)"; break;

        case Token::DOT:outs << "TOKEN(DOT)"; break;
        case Token::DOTDOT:outs << "TOKEN(DOTDOT)"; break;

        default:outs << "TOKEN(UNKNOWN)"; break;
    }
    return outs;
}

ostream& operator<<(ostream &outs, const Token *tok) {
    return outs << *tok;
}
