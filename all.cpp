#include "parser.h"
#include "scanner.h"
#include "visitor.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {

  string preffix_input = "inputs/";
  string preffix_out = "outputs/";
  vector<string> paths = {"input1.txt", "input2.txt", "input3.txt",
                          "input4.txt", "input5.txt", "input6.txt",
                          "input7.txt"};

  for (auto path : paths) {
    ifstream infile(preffix_input + path);
    if (!infile.is_open()) {
      cout << "No se pudo abrir el archivo: " << path << endl;
      exit(1);
    }

    string input;
    string line;
    while (getline(infile, line)) {
      input += line + '\n';
    }
    infile.close();

    Scanner scanner(input.c_str());

    string input_copy = input;
    Scanner scanner_test(input_copy.c_str());
    // test_scanner(&scanner_test);
    cout << "---------------------------------------------------" << endl;
    cout << "path: " << path << endl;
    cout << "Scanner exitoso" << endl;
    cout << endl;
    cout << "Iniciando parsing:" << endl;
    Parser parser(&scanner);
    try {
      Program *program = parser.parseProgram();
      cout << "Parsing exitoso" << endl << endl;
      cout << "Iniciando Visitor:" << endl;
      // PrintVisitor printVisitor;
      // EVALVisitor evalVisitor;
      // cout << "IMPRIMIR:" << endl;
      // printVisitor.imprimir(program);
      // cout << endl;
      // cout << "EJECUTAR:" << endl;
      // evalVisitor.ejecutar(program);
      // cout << "Ejecución exitosa" << endl;
      // cout << endl;
      cout << "Generando código assembly:" << endl;
      ofstream outfile(preffix_out + path);
      GenCodeVisitor<ofstream> genVisitor(outfile);
      genVisitor.generate(program);
      cout << endl;
      cout << "Ejecución finalizada con éxito." << endl;
      outfile.close();
      delete program;
    } catch (const exception &e) {
      cout << "Error durante la ejecución: " << e.what() << endl;
      cout << "Pasando a siguiente archivo" << endl;
    }
  }

  return 0;
}
