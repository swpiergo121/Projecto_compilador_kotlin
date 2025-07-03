#include "ui.h"
#include "parser.h"
#include "scanner.h"
#include "visitor.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "SFML Kotlin Text Editor",
                          sf::Style::Close | sf::Style::Resize);
  window.setFramerateLimit(60);

  sf::Font font;
  if (!font.loadFromFile(
          "assets/arial.ttf")) { // Make sure you have an Arial font
                                 // file or change to another
    std::cerr << "Error loading font!" << std::endl;
    return -1;
  }

  // --- UI Elements ---
  // Text Blocks
  float textBlockWidth = (WINDOW_WIDTH - PADDING * 3) / 2;
  float textBlockHeight = WINDOW_HEIGHT - PADDING * 2 - BUTTON_HEIGHT -
                          PADDING - PADDING - RESULT_HEIGHT_BOX -
                          PADDING; // Adjust for buttons

  ScrollableTextBlockEdit kotlinEditor(
      sf::Vector2f(PADDING, PADDING + BUTTON_HEIGHT + PADDING),
      sf::Vector2f(textBlockWidth, textBlockHeight), font);
  ScrollableTextBlockView assemblyCode(
      sf::Vector2f(PADDING * 2 + textBlockWidth, PADDING),
      sf::Vector2f(textBlockWidth, textBlockHeight + BUTTON_HEIGHT + PADDING),
      font);
  ScrollableTextBlockView resultAssembly(
      sf::Vector2f(PADDING, PADDING + BUTTON_HEIGHT + PADDING +
                                textBlockHeight + PADDING),
      sf::Vector2f(textBlockWidth, RESULT_HEIGHT_BOX), font);

  // Buttons load
  Button buttonLoad(
      sf::Vector2f(PADDING + BUTTON_DROPDOWN_WIDTH + PADDING, PADDING),
      sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), "Load test", font);
  // Buttons run
  Button buttonRun(sf::Vector2f(PADDING + BUTTON_DROPDOWN_WIDTH + PADDING +
                                    BUTTON_WIDTH + PADDING,
                                PADDING),
                   sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), "Run", font);

  // Dropdown Button (Left Top)
  string prefix_input = "tests/";
  vector<string> paths;

  DIR *dp = opendir(prefix_input.c_str());
  if (!dp) {
    cerr << "No pude abrir el directorio tests/\n";
    return 1;
  }
  struct dirent *entry;
  while ((entry = readdir(dp)) != nullptr) {
    string name = entry->d_name;
    if (name.size() > 4 && name.substr(name.size() - 4) == ".txt") {
      paths.push_back(name);
    }
  }
  sort(paths.begin(), paths.end());

  DropdownButton dropdownButton(
      sf::Vector2f(PADDING, PADDING),
      sf::Vector2f(BUTTON_DROPDOWN_WIDTH, BUTTON_HEIGHT), "Tests...", font,
      paths);

  // Button execute
  Button buttonExecute(sf::Vector2f(PADDING + textBlockWidth + PADDING,
                                    resultAssembly.background.getPosition().y),
                       sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), "Execute",
                       font);
  // Button save
  Button buttonSave(
      sf::Vector2f(PADDING + textBlockWidth + PADDING + BUTTON_WIDTH + PADDING,
                   resultAssembly.background.getPosition().y),
      sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), "Save", font);

  while (window.isOpen()) {
    sf::Event event;
    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::Resized) {
        // Update the view to the new size of the window
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
        // Re-calculate positions and sizes if needed for responsive design
        // (This example does not fully implement responsive layout)
      }

      kotlinEditor.handleEvent(event, mousePos);
      resultAssembly.handleEvent(event, mousePos);
      assemblyCode.handleEvent(event, mousePos);
      dropdownButton.handleEvent(event, mousePos);

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        if (buttonLoad.isClicked(mousePos)) {
          if (dropdownButton.selectedIndex == -1) {
            resultAssembly.setText("Pick a file before loading.");
            resultAssembly.updateTextDisplay();
          } else {

            string path = PREFIX_INPUT +
                          dropdownButton.items[dropdownButton.selectedIndex];
            ifstream infile(path);
            if (!infile.is_open()) {
              exit(1);
            }

            string input;
            string line;
            while (getline(infile, line)) {
              input += line + '\n';
            }
            infile.close();

            kotlinEditor.setText(input);
            kotlinEditor.updateTextDisplayCursor();
          }
        }
        if (buttonRun.isClicked(mousePos)) {
          Scanner scanner(kotlinEditor.getText().c_str());
          Parser parser(&scanner, false);
          try {
            Program *program = parser.parseProgram();
            stringstream ss;
            GenCodeVisitor<stringstream> genVisitor(ss);
            genVisitor.generate(program);
            delete program;

            assemblyCode.setText(ss.str());
            assemblyCode.updateTextDisplay();
          } catch (const exception &e) {
            resultAssembly.setText("File couldn't be parsed correctly");
            resultAssembly.updateTextDisplay();
          }
        }
        if (buttonExecute.isClicked(mousePos)) {
          // 1. Save to file
          string asm_path = "assets/temp.s";
          ofstream asm_file(asm_path);
          if (!asm_file.is_open()) {
            resultAssembly.setText(
                "Error: Could not open temp file for writing.");
            resultAssembly.updateTextDisplay();
            asm_file.close();
            continue;
          }
          asm_file << assemblyCode.currentText;
          asm_file.close();

          // 2. Generate .o file
          string obj_path = "assets/temp.o";
          std::string assemble_cmd = "gcc -c " + asm_path + " -o " + obj_path;
          int assemble_result = std::system(assemble_cmd.c_str());
          if (assemble_result != 0) {
            resultAssembly.setText("Error: Assembly failed with code.");
            resultAssembly.updateTextDisplay();
            continue;
          }

          // 3. Link the object file to a.out file
          string out_path = "assets/a.out";
          std::string link_cmd = "gcc " + obj_path + " -o " + out_path;
          int link_result = std::system(link_cmd.c_str());
          if (link_result != 0) {
            resultAssembly.setText("Error: Linking failed with code " +
                                   to_string(link_result));
            resultAssembly.updateTextDisplay();
            // Clean up generated files
            std::remove(asm_path.c_str());
            std::remove(obj_path.c_str());
            continue;
          }
          // 4. Execute the executable
          std::string captured_output = exec_and_capture("./" + out_path);

          // 5. Save to result box
          resultAssembly.setText("Result:\n" + captured_output);
          resultAssembly.updateTextDisplay();

          // 6. Remove files
          std::remove(asm_path.c_str());
          std::remove(obj_path.c_str());
          std::remove(out_path.c_str());
        }
        if (buttonSave.isClicked(mousePos)) {
          // Save file to path
          // 1. Save to file
          string asm_path = "assets/out.s";
          ofstream asm_file(asm_path);
          if (!asm_file.is_open()) {
            resultAssembly.setText(
                "Error: Could not open temp file for writing.");
            resultAssembly.updateTextDisplay();
            asm_file.close();
            resultAssembly.setText("Assembly code saved in:" + asm_path +
                                   ".\n");
            resultAssembly.updateTextDisplay();
            continue;
          }
          asm_file << assemblyCode.currentText;
          asm_file.close();
        }
      }
    }

    // Update button states (hover effect)
    buttonLoad.update(mousePos);
    buttonRun.update(mousePos);
    buttonExecute.update(mousePos);
    buttonSave.update(mousePos);
    dropdownButton.update(mousePos); // Dropdown also inherits Button's update

    window.clear(sf::Color(50, 50, 50)); // Dark grey background

    // Draw elements
    kotlinEditor.draw_with_cursor(window);
    assemblyCode.draw(window);
    resultAssembly.draw(window);
    buttonLoad.draw(window);
    buttonRun.draw(window);
    dropdownButton.draw(window);
    buttonExecute.draw(window);
    buttonSave.draw(window);

    window.display();
  }

  return 0;
}
