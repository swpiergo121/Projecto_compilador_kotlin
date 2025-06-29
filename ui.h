#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// --- Constants ---
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const float PADDING = 20.0f;
const float BUTTON_HEIGHT = 40.0f;
const float BUTTON_WIDTH = 150.0f;
const float DROPDOWN_ITEM_HEIGHT = 30.0f;
const int FONT_SIZE = 18;
const int TEXT_BLOCK_LINE_HEIGHT = 24; // Approximate line height for scrolling
const float TEXT_BLOCK_INTERNAL_PADDING = 5.0f; // Padding inside the text block
const float RESULT_HEIGHT_BOX = 100.0f;
const string PREFIX_INPUT = "tests/";

// std::string exec_and_capture(const std::string &cmd) {
//   std::string result = "";
// #ifdef _WIN32
//   // Windows specific: Use _popen and _pclose
//   FILE *pipe = _popen(cmd.c_str(), "r");
//   if (!pipe) {
//     return "ERROR: _popen failed!";
//   }
//   char buffer[128];
//   while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//     result += buffer;
//   }
//   _pclose(pipe);
// #else
//   // Linux/Unix specific: Use popen and pclose
//   FILE *pipe = popen(cmd.c_str(), "r");
//   if (!pipe) {
//     return "ERROR: popen failed!";
//   }
//   char buffer[128];
//   while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//     result += buffer;
//   }
//   pclose(pipe);
// #endif
//   return result;
// }

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

// --- Helper function to check if a point is within a rectangle ---
bool isMouseOver(const sf::FloatRect &rect, const sf::Vector2f &mousePos) {
  return rect.contains(mousePos);
}

class ScrollableTextBlock {
public:
  sf::RectangleShape background;
  sf::Text textDisplay;
  std::string currentText;
  sf::Font &font;
  float scrollOffset;
  bool isActive;
  float maxScrollOffset;

  ScrollableTextBlock(sf::Vector2f position, sf::Vector2f size, sf::Font &f)
      : font(f), scrollOffset(0.0f), isActive(false), maxScrollOffset(0.0f) {
    background.setPosition(position);
    background.setSize(size);
    background.setFillColor(sf::Color(30, 30, 30));
    background.setOutlineThickness(1.0f);
    background.setOutlineColor(sf::Color::White);

    textDisplay.setFont(font);
    textDisplay.setCharacterSize(FONT_SIZE);
    textDisplay.setFillColor(sf::Color::White);
    // Initial position will be set in updateTextDisplay
  }

  virtual void handleEvent(sf::Event event, const sf::Vector2f &mousePos) {};

  void updateTextDisplay() {
    textDisplay.setString(currentText);

    // Calculate total height of the text
    sf::FloatRect textBounds = textDisplay.getLocalBounds();
    float totalTextHeight = textBounds.height;

    // Calculate max scroll offset. Ensure it doesn't go negative.
    // The visible height for text is background.getSize().y - 2 *
    // TEXT_BLOCK_INTERNAL_PADDING
    maxScrollOffset =
        std::max(0.0f, totalTextHeight - (background.getSize().y -
                                          2 * TEXT_BLOCK_INTERNAL_PADDING));

    // Ensure scrollOffset doesn't exceed maxScrollOffset if text shrinks
    scrollOffset = std::min(scrollOffset, maxScrollOffset);

    // The text's position is relative to the window, but we want it to appear
    // relative to the top-left of the background, adjusted by scrollOffset.
    // This position will be used when drawing with the clipped view.
    textDisplay.setPosition(background.getPosition().x +
                                TEXT_BLOCK_INTERNAL_PADDING,
                            background.getPosition().y +
                                TEXT_BLOCK_INTERNAL_PADDING - scrollOffset);
  }

  void setText(string text) { this->currentText = text; }
  string getText() { return currentText; }

  void draw(sf::RenderWindow &window) {
    window.draw(background);

    // Store the original view
    sf::View originalView = window.getView();

    // Create a new view that covers only the text block's internal area
    // The viewport defines the area on the window where the view is drawn
    // (normalized coordinates 0-1) The rectangle defines the coordinate system
    // of the view (world coordinates)
    sf::View textView;
    textView.setViewport(
        sf::FloatRect(background.getPosition().x / window.getSize().x,
                      background.getPosition().y / window.getSize().y,
                      background.getSize().x / window.getSize().x,
                      background.getSize().y / window.getSize().y));

    // Set the center and size of the view to match the internal text area
    // The view's coordinate system starts at the top-left of the background,
    // and its size matches the background's internal size.
    textView.setCenter(
        background.getPosition().x + background.getSize().x / 2.0f,
        background.getPosition().y + background.getSize().y / 2.0f);
    textView.setSize(background.getSize().x, background.getSize().y);

    window.setView(textView);

    // Now, draw the text. Its position is already calculated to be relative
    // to the top-left of the background, adjusted by scrollOffset.
    // Since the view's coordinate system is aligned with the background,
    // the text will be drawn correctly within the clipped area.
    window.draw(textDisplay);

    // Restore the original view
    window.setView(originalView);
  }
};

class ScrollableTextBlockView : public ScrollableTextBlock {
public:
  ScrollableTextBlockView(sf::Vector2f position, sf::Vector2f size, sf::Font &f)
      : ScrollableTextBlock(position, size, f) {}

  void handleEvent(sf::Event event, const sf::Vector2f &mousePos) override {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        isActive = isMouseOver(background.getGlobalBounds(), mousePos);
        if (isActive) {
          std::cout << "Text block activated." << std::endl;
        }
      }
    } else if (event.type == sf::Event::MouseWheelScrolled && isActive) {
      if (isMouseOver(background.getGlobalBounds(), mousePos)) {
        if (event.mouseWheelScroll.delta < 0) { // Scroll down
          scrollOffset =
              std::min(scrollOffset + TEXT_BLOCK_LINE_HEIGHT, maxScrollOffset);
        } else if (event.mouseWheelScroll.delta > 0) { // Scroll up
          scrollOffset = std::max(scrollOffset - TEXT_BLOCK_LINE_HEIGHT, 0.0f);
        }
        updateTextDisplay(); // Update text position after scroll
      }
    }
  }
};

// --- Scrollable Text Block Class ---
class ScrollableTextBlockEdit : public ScrollableTextBlock {
public:
  ScrollableTextBlockEdit(sf::Vector2f position, sf::Vector2f size, sf::Font &f)
      : ScrollableTextBlock(position, size, f) {}

  void handleEvent(sf::Event event, const sf::Vector2f &mousePos) override {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        isActive = isMouseOver(background.getGlobalBounds(), mousePos);
        if (isActive) {
          std::cout << "Text block activated." << std::endl;
        }
      }
    } else if (event.type == sf::Event::TextEntered && isActive) {
      if (event.text.unicode < 128) {     // ASCII characters
        if (event.text.unicode == '\b') { // Backspace
          if (!currentText.empty()) {
            currentText.pop_back();
          }
        } else if (event.text.unicode == '\r') { // Enter key
          currentText += '\n';
        } else {
          currentText += static_cast<char>(event.text.unicode);
        }
        updateTextDisplay();
      }
    } else if (event.type == sf::Event::MouseWheelScrolled && isActive) {
      if (isMouseOver(background.getGlobalBounds(), mousePos)) {
        if (event.mouseWheelScroll.delta < 0) { // Scroll down
          scrollOffset =
              std::min(scrollOffset + TEXT_BLOCK_LINE_HEIGHT, maxScrollOffset);
        } else if (event.mouseWheelScroll.delta > 0) { // Scroll up
          scrollOffset = std::max(scrollOffset - TEXT_BLOCK_LINE_HEIGHT, 0.0f);
        }
        updateTextDisplay(); // Update text position after scroll
      }
    }
  }
};

// --- Button Class ---
class Button {
public:
  sf::RectangleShape shape;
  sf::Text text;
  bool isHovered;

  Button(sf::Vector2f position, sf::Vector2f size, const std::string &label,
         sf::Font &font)
      : isHovered(false) {
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color(70, 70, 70));
    shape.setOutlineThickness(1.0f);
    shape.setOutlineColor(sf::Color::White);

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(FONT_SIZE);
    text.setFillColor(sf::Color::White);

    // Center text on button
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
  }

  void update(const sf::Vector2f &mousePos) {
    isHovered = isMouseOver(shape.getGlobalBounds(), mousePos);
    if (isHovered) {
      shape.setFillColor(sf::Color(100, 100, 100));
    } else {
      shape.setFillColor(sf::Color(70, 70, 70));
    }
  }

  bool isClicked(const sf::Vector2f &mousePos) const {
    return isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left);
  }

  void draw(sf::RenderWindow &window) {
    window.draw(shape);
    window.draw(text);
  }
};

// --- Dropdown Button Class ---
class DropdownButton : public Button {
public:
  std::vector<std::string> items;
  bool isOpen;
  int selectedIndex;
  sf::RectangleShape dropdownBackground;

  DropdownButton(sf::Vector2f position, sf::Vector2f size,
                 const std::string &label, sf::Font &font,
                 const std::vector<std::string> &dropdownItems)
      : Button(position, size, label, font), items(dropdownItems),
        isOpen(false), selectedIndex(-1) {
    dropdownBackground.setFillColor(sf::Color(50, 50, 50));
    dropdownBackground.setOutlineThickness(1.0f);
    dropdownBackground.setOutlineColor(sf::Color::White);
  }

  void handleEvent(sf::Event event, const sf::Vector2f &mousePos) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
      if (isMouseOver(shape.getGlobalBounds(), mousePos)) {
        isOpen = !isOpen;
      } else if (isOpen) {
        // Check if an item was clicked
        for (size_t i = 0; i < items.size(); ++i) {
          sf::FloatRect itemRect(shape.getPosition().x,
                                 shape.getPosition().y + shape.getSize().y +
                                     (i * DROPDOWN_ITEM_HEIGHT),
                                 shape.getSize().x, DROPDOWN_ITEM_HEIGHT);
          if (isMouseOver(itemRect, mousePos)) {
            selectedIndex = i;
            text.setString(
                items[selectedIndex]); // Update button text to selected item
            isOpen = false;            // Close dropdown after selection
            std::cout << "Dropdown selected: " << items[selectedIndex]
                      << std::endl;
            break;
          }
        }
      }
    }
  }

  void draw(sf::RenderWindow &window) {
    Button::draw(window); // Draw the main button

    if (isOpen) {
      dropdownBackground.setPosition(shape.getPosition().x,
                                     shape.getPosition().y + shape.getSize().y);
      dropdownBackground.setSize(
          sf::Vector2f(shape.getSize().x, items.size() * DROPDOWN_ITEM_HEIGHT));
      window.draw(dropdownBackground);

      for (size_t i = 0; i < items.size(); ++i) {
        sf::Text itemText;
        itemText.setFont(*text.getFont());
        itemText.setString(items[i]);
        itemText.setCharacterSize(FONT_SIZE);
        itemText.setFillColor(sf::Color::White);
        itemText.setPosition(shape.getPosition().x + 5,
                             shape.getPosition().y + shape.getSize().y +
                                 (i * DROPDOWN_ITEM_HEIGHT) +
                                 (DROPDOWN_ITEM_HEIGHT - FONT_SIZE) / 2);
        window.draw(itemText);
      }
    }
  }
};
