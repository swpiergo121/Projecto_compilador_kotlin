// scanner.cpp
#include "scanner.h"
#include "token.h"
#include <cstring>
#include <iostream>

using namespace std;

Scanner::Scanner(const char *s) : input(s), first(0), current(0) {}

Scanner::~Scanner() {}

void Scanner::reset() {
  first = 0;
  current = 0;
}

static bool is_white_space(char c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token *Scanner::nextToken() {
  // saltar espacios
  while (current < (int)input.size() && is_white_space(input[current]))
    current++;
  // fin de input
  if (current >= (int)input.size())
    return new Token(Token::END);

  first = current;
  char c = input[current];

  // Números
  if (isdigit(c)) {
    current++;
    while (current < (int)input.size() && isdigit(input[current]))
      current++;
    std::string txt = input.substr(first, current - first);
    return new Token(Token::NUM, txt, 0, (int)txt.size());
  }

  // Identificadores y palabras reservadas
  if (isalpha(c)) {
    current++;
    while (current < (int)input.size() &&
           (isalnum(input[current]) || input[current] == '_'))
      current++;
    std::string word = input.substr(first, current - first);

    if (word == "print" || word == "println")
      return new Token(Token::PRINT, word, 0, (int)word.size());
    if (word == "if")
      return new Token(Token::IF, word, 0, (int)word.size());
    if (word == "else")
      return new Token(Token::ELSE, word, 0, (int)word.size());
    if (word == "while")
      return new Token(Token::WHILE, word, 0, (int)word.size());
    if (word == "for")
      return new Token(Token::FOR, word, 0, (int)word.size());
    if (word == "var")
      return new Token(Token::VAR, word, 0, (int)word.size());
    if (word == "val")
      return new Token(Token::VAL, word, 0, (int)word.size());
    if (word == "fun")
      return new Token(Token::FUN, word, 0, (int)word.size());
    if (word == "return")
      return new Token(Token::RETURN, word, 0, (int)word.size());
    if (word == "true")
      return new Token(Token::TRUE, word, 0, (int)word.size());
    if (word == "false")
      return new Token(Token::FALSE, word, 0, (int)word.size());

    return new Token(Token::ID, word, 0, (int)word.size());
  }

  // Símbolos y operadores
  Token *token = nullptr;
  switch (c) {
  case '+':
    token = new Token(Token::PLUS, c);
    break;
  case '-':
    token = new Token(Token::MINUS, c);
    break;
  case '*':
    token = new Token(Token::MUL, c);
    break;
  case '/':
    token = new Token(Token::DIV, c);
    break;
  case ',':
    token = new Token(Token::COMA, c);
    break;
  case ';':
    token = new Token(Token::PC, c);
    break;
  case '(':
    token = new Token(Token::PI, c);
    break;
  case ')':
    token = new Token(Token::PD, c);
    break;
  case '{':
    token = new Token(Token::LBRACE, c);
    break;
  case '}':
    token = new Token(Token::RBRACE, c);
    break;
  case '[':
    token = new Token(Token::LBRACK, c);
    break;
  case ']':
    token = new Token(Token::RBRACK, c);
    break;
  case ':':
    token = new Token(Token::COLON, c);
    break;

  case '.':
    if (current + 1 < (int)input.size() && input[current + 1] == '.') {
      token = new Token(Token::DOTDOT, "..", 0, 2);
      current++;
    } else {
      token = new Token(Token::DOT, ".", 0, 1);
    }
    break;

  case '=':
    if (current + 1 < (int)input.size() && input[current + 1] == '=') {
      token = new Token(Token::EQ, "==", 0, 2);
      current++;
    } else {
      token = new Token(Token::ASSIGN, c);
    }
    break;

  case '>':
    if (current + 1 < (int)input.size() && input[current + 1] == '=') {
      token = new Token(Token::GE, ">=", 0, 2);
      current++;
    } else {
      token = new Token(Token::GT, ">", 0, 1);
    }
    break;

  case '<':
    if (current + 1 < (int)input.size() && input[current + 1] == '=') {
      token = new Token(Token::LE, "<=", 0, 2);
      current++;
    } else {
      token = new Token(Token::LT, "<", 0, 1);
    }
    break;

  case '"': {
    std::string val;

    while (current + 1 < (int)input.size() && input[current + 1] != '"') {
      current++;
      val += input[current];
    }
    if (current + 1 >= (int)input.size()) {
      const char *msg = "Unterminated string literal";
      token = new Token(Token::ERR, msg, 0, (int)strlen(msg));
    } else {
      current++;
      token = new Token(Token::STRING, val, 0, (int)val.size());
    }
    break;
  }

  default:

    token = new Token(Token::ERR, c);
    break;
  }
  current++;
  return token;
}

void test_scanner(Scanner *scanner) {
  Token *tok;
  cout << "Iniciando Scanner:" << endl << endl;
  while ((tok = scanner->nextToken())->type != Token::END) {
    if (tok->type == Token::ERR) {
      cout << "Error en scanner - char inválido: " << tok->text << endl;
      delete tok;
      break;
    }
    cout << *tok << endl;
    delete tok;
  }
  cout << "TOKEN(END)" << endl;
  delete tok;
}
