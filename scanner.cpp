#include "scanner.h"
#include "token.h"
#include <cstring>
#include <iostream>

using namespace std;

Scanner::Scanner(const char *s) : input(s), first(0), current(0) {}

bool is_white_space(char c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token *Scanner::nextToken() {
  Token *token;
  while (current < input.length() && is_white_space(input[current]))
    current++;
  if (current >= input.length())
    return new Token(Token::END);
  char c = input[current];
  first = current;
  if (isdigit(c)) {
    current++;
    while (current < input.length() && isdigit(input[current]))
      current++;
    token = new Token(Token::NUM, input, first, current - first);
  }

  else if (isalpha(c)) {
    current++;
    while (current < input.length() && isalnum(input[current]))
      current++;
    string word = input.substr(first, current - first);
    if (word == "print") {
      token = new Token(Token::PRINT, word, 0, word.length());
    } else if (word == "if") {
      token = new Token(Token::IF, word, 0, word.length());
    } else if (word == "then") {
      token = new Token(Token::THEN, word, 0, word.length());
    } else if (word == "else") {
      token = new Token(Token::ELSE, word, 0, word.length());
    } else if (word == "endif") {
      token = new Token(Token::ENDIF, word, 0, word.length());
    } else if (word == "ifexp") {
      token = new Token(Token::IFEXP, word, 0, word.length());
    } else if (word == "while") {
      token = new Token(Token::WHILE, word, 0, word.length());
    } else if (word == "endwhile") {
      token = new Token(Token::ENDWHILE, word, 0, word.length());
    } else if (word == "do") {
      token = new Token(Token::DO, word, 0, word.length());
    } else if (word == "for") {
      token = new Token(Token::FOR, word, 0, word.length());
    } else if (word == "endfor") {
      token = new Token(Token::ENDFOR, word, 0, word.length());
    } else if (word == "var") {
      token = new Token(Token::VAR, word, 0, word.length());
    } else if (word == "true") {
      token = new Token(Token::TRUE, word, 0, word.length());
    } else if (word == "false") {
      token = new Token(Token::FALSE, word, 0, word.length());
    } else if (word == "fun") {
      token = new Token(Token::FUN, word, 0, word.length());
    } else if (word == "endfun") {
      token = new Token(Token::ENDFUN, word, 0, word.length());
    } else if (word == "return") {
      token = new Token(Token::RETURN, word, 0, word.length());
    } else if (word == "class") {
      token = new Token(Token::CLASS, word, 0, word.length());
    } else {
      token = new Token(Token::ID, word, 0, word.length());
    }
  }

  else if (strchr("+-*/()=;,<{}:", c)) {
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
    case '(':
      token = new Token(Token::PI, c);
      break;
    case ')':
      token = new Token(Token::PD, c);
      break;
    case '{':
      token = new Token(Token::CBL, c);
      break;
    case '}':
      token = new Token(Token::CBR, c);
      break;
    case ':':
      token = new Token(Token::DP, c);
      break;
    case '=':
      if (current + 1 < input.length() && input[current + 1] == '=') {
        token = new Token(Token::EQ, "==", 0, 2);
        current++;
      } else {
        token = new Token(Token::ASSIGN, c);
      }
      break;
    case '<':
      if (current + 1 < input.length() && input[current + 1] == '=') {
        token = new Token(Token::LE, "<=", 0, 2);
        current++;
      } else {
        token = new Token(Token::LT, c);
      }
      break;
    case ';':
      token = new Token(Token::PC, c);
      break;
    default:
      cout << "No debería llegar acá" << endl;
      token = new Token(Token::ERR, c);
    }
    current++;
  } else {
    token = new Token(Token::ERR, c);
    current++;
  }
  return token;
}

void Scanner::reset() {
  first = 0;
  current = 0;
}

Scanner::~Scanner() {}

void test_scanner(Scanner *scanner) {
  Token *current;
  cout << "Iniciando Scanner:" << endl << endl;
  while ((current = scanner->nextToken())->type != Token::END) {
    if (current->type == Token::ERR) {
      cout << "Error en scanner - carácter inválido: " << current->text << endl;
      break;
    } else {
      cout << *current << endl;
    }
    delete current;
  }
  cout << "TOKEN(END)" << endl;
  delete current;
}
