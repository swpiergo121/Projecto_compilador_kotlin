#include "parser.h"
#include "scanner.h"
#include "visitor.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>

using namespace std;

string get_before_dot(string input) {
  // Find the position of the first occurrence of '.'
  size_t dot_position = input.find('.');

  // If a dot is found, return the substring from the beginning up to the dot's
  // position
  if (dot_position != string::npos) {
    return input.substr(0, dot_position);
  } else {
    // If no dot is found, return the original string
    return input;
  }
}

int main() {
  
  string prefix_input  = "tests/";
  string prefix_output = "outputs/";
  vector<string> paths;

  // --- Listar archivos .txt en tests/ usando dirent.h ---
  DIR *dp = opendir(prefix_input.c_str());
  if (!dp) {
    cerr << "No pude abrir el directorio tests/\n";
    return 1;
  }
  struct dirent *entry;
  while ((entry = readdir(dp)) != nullptr) {
    string name = entry->d_name;
    if (name.size() > 4 && name.substr(name.size()-4) == ".txt") {
      paths.push_back(name);
    }
  }
  closedir(dp);

  sort(paths.begin(), paths.end());

  for (auto path : paths) {
    ifstream infile(prefix_input + path);
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
    test_scanner(&scanner_test);
    cout << "Scanner exitoso" << endl;
    cout << endl;
    cout << "Iniciando parsing:" << endl;
    Parser parser(&scanner);
    try {
      Program *program = parser.parseProgram();
      cout << "Parsing exitoso" << endl << endl;
      cout << "Iniciando Visitor:" << endl;
      PrintVisitor printVisitor;
      EVALVisitor evalVisitor;
      cout << "IMPRIMIR:" << endl;
      printVisitor.imprimir(program);
      cout << endl;
      cout << "EJECUTAR:" << endl;
      evalVisitor.ejecutar(program);
      cout << "Ejecución exitosa" << endl;
      cout << endl;
      cout << "Generando código assembly:" << endl;
      ofstream outfile(prefix_output + get_before_dot(path) + ".s");
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
