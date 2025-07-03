#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Clipboard.hpp>
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
const float BUTTON_WIDTH = 100.0f;
const float BUTTON_DROPDOWN_WIDTH = 250.0f;
const float DROPDOWN_ITEM_HEIGHT = 30.0f;
const int FONT_SIZE = 18;
const int TEXT_BLOCK_LINE_HEIGHT = 24; // Approximate line height for scrolling
const float TEXT_BLOCK_INTERNAL_PADDING = 5.0f; // Padding inside the text block
const float RESULT_HEIGHT_BOX = 100.0f;
const string PREFIX_INPUT = "tests/";
const float CURSOR_WIDTH = 2.0f;
const float CURSOR_BLINK_RATE = 0.5f; // Seconds
const float MAX_DROPDOWN_VISIBLE_HEIGHT = 7 * BUTTON_HEIGHT;

std::string exec_and_capture(const std::string &cmd) {
  std::string result = "";

  // For Windows: Use _popen and _pclose
  // FILE *pipe = _popen(cmd.c_str(), "r");
  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe) {
    return "ERROR: popen failed!";
  }
  char buffer[128];
  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    result += buffer;
  }
  pclose(pipe);
  // For windows
  // _pclose(pipe);
  return result;
}

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

int findCharacterLocalPos(const sf::Text &text, const sf::Vector2f &localPos) {
  const std::string &str = text.getString();
  if (str.empty()) {
    return 0;
  }

  // Iterate through each character to find the closest one
  for (int i = 0; i <= str.length(); ++i) {
    sf::Vector2f charPos = text.findCharacterPos(i);
    // Check if the click is before or at this character's X position
    // and within its line's Y range.
    if (localPos.y < charPos.y + text.getCharacterSize() &&
        // Check if on the same line
        localPos.y >= charPos.y) {
      if (localPos.x < charPos.x) {
        // Clicked before this character
        return i;
      }
    } else if (localPos.y < charPos.y) { // Clicked on a line above
      // This means we've gone past the line where the click occurred
      // Find the last character of the previous line
      int prevLineEndIndex = i;
      while (prevLineEndIndex > 0 &&
             text.findCharacterPos(prevLineEndIndex - 1).y == charPos.y) {
        prevLineEndIndex--;
      }
      // Return the end of the previous line
      return prevLineEndIndex;
    }
  }
  // Clicked after the last character
  return str.length();
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
  // Cursor related members
  sf::RectangleShape cursorShape;
  int cursorPosition;
  sf::Clock cursorBlinkClock;
  bool showCursor;

  ScrollableTextBlockEdit(sf::Vector2f position, sf::Vector2f size, sf::Font &f)
      : ScrollableTextBlock(position, size, f) {
    cursorPosition = 0;
    showCursor = true;

    // Initialize cursor shape
    cursorShape.setFillColor(sf::Color::White);
    cursorShape.setSize(sf::Vector2f(
        CURSOR_WIDTH, FONT_SIZE + 2)); // Height slightly more than font size
  }

  void handleEvent(sf::Event event, const sf::Vector2f &mousePos) override {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        isActive = isMouseOver(background.getGlobalBounds(), mousePos);
        if (isActive) {
          cursorBlinkClock.restart();
          showCursor = true;

          sf::Vector2f clickPosInTextCoords =
              mousePos - textDisplay.getPosition();
          clickPosInTextCoords.y += scrollOffset;

          // Find the character index closest to the click position
          cursorPosition =
              findCharacterLocalPos(textDisplay, clickPosInTextCoords);
          updateTextDisplayCursor();
        }
      }
    } else if (event.type == sf::Event::TextEntered && isActive) {
      if (event.text.unicode < 128) {     // ASCII characters
        if (event.text.unicode == '\b') { // Backspace
          if (cursorPosition > 0) {
            currentText.erase(cursorPosition - 1, 1);
            cursorPosition--;
          }
        } else if (event.text.unicode == '\r') { // Enter key
          currentText.insert(cursorPosition, "\n");
          cursorPosition++;
        } else {
          currentText.insert(cursorPosition, 1,
                             static_cast<char>(event.text.unicode));
          cursorPosition++;
        }
        updateTextDisplayCursor();
      }
    } else if (event.type == sf::Event::KeyPressed && isActive) {
      // Reset cursor blink on key press
      cursorBlinkClock.restart();
      showCursor = true;

      if (event.key.code == sf::Keyboard::Left) {
        if (cursorPosition > 0) {
          cursorPosition--;
        }
        updateTextDisplayCursor();
      } else if (event.key.code == sf::Keyboard::Right) {
        if (cursorPosition < currentText.length()) {
          cursorPosition++;
        }
        updateTextDisplayCursor();
      } else if (event.key.code == sf::Keyboard::Up) {
        if (cursorPosition == 0) {
          // Already at the very beginning of the text
          updateTextDisplayCursor();
          return;
        }
        // 1. Find the start of the current line
        size_t currentLineStart = currentText.rfind('\n', cursorPosition - 1);
        if (currentLineStart == std::string::npos) {
          currentLineStart = 0; // First line
        } else {
          currentLineStart++; // Move past the newline character
        }

        // 2. Calculate the "column" (offset from the start of the current line)
        int currentColumn = cursorPosition - currentLineStart;

        // 3. Find the start of the previous line
        size_t prevLineEnd = std::string::npos;
        if (currentLineStart > 0) {
          prevLineEnd = currentText.rfind(
              '\n',
              currentLineStart - 2); // -2 to look before current line's start
        }
        size_t prevLineStart =
            (prevLineEnd == std::string::npos) ? 0 : prevLineEnd + 1;

        // 4. Determine the length of the previous line
        size_t prevLineLength;
        if (prevLineEnd ==
            std::string::npos) { // If previous line is the first line
          prevLineLength = currentLineStart - prevLineStart;
        } else {
          prevLineLength =
              (currentLineStart - 1) - prevLineStart; // -1 for the newline char
        }

        // 5. Calculate the new cursor position on the previous line
        cursorPosition =
            prevLineStart + std::min(currentColumn, (int)prevLineLength);
        updateTextDisplayCursor();

      } else if (event.key.code == sf::Keyboard::Down) {
        if (cursorPosition == currentText.length()) {
          // Already at the very end of the text
          updateTextDisplayCursor();
          return;
        }

        // 1. Find the end of the current line (or end of text if no newline)
        size_t currentLineEnd = currentText.find('\n', cursorPosition);
        if (currentLineEnd == std::string::npos) {
          currentLineEnd = currentText.length();
        }

        // 2. Find the start of the current line
        size_t currentLineStart = currentText.rfind('\n', cursorPosition - 1);
        if (currentLineStart == std::string::npos) {
          currentLineStart = 0;
        } else {
          currentLineStart++;
        }

        // 3. Calculate the "column"
        int currentColumn = cursorPosition - currentLineStart;

        // 4. Find the start of the next line
        size_t nextLineStart = currentLineEnd + 1;

        if (nextLineStart >= currentText.length()) {
          // No next line, move to the very end of the text
          cursorPosition = currentText.length();
        } else {
          // 5. Find the end of the next line
          size_t nextLineEnd = currentText.find('\n', nextLineStart);
          if (nextLineEnd == std::string::npos) {
            nextLineEnd = currentText.length();
          }

          // 6. Determine the length of the next line
          size_t nextLineLength = nextLineEnd - nextLineStart;

          // 7. Calculate the new cursor position on the next line
          cursorPosition =
              nextLineStart + std::min(currentColumn, (int)nextLineLength);
        }
        updateTextDisplayCursor();
      } else if (event.key.code == sf::Keyboard::V && event.key.control) {
        sf::String clipboardText = sf::Clipboard::getString();

        currentText.insert(cursorPosition, clipboardText.toAnsiString());

        cursorPosition += clipboardText.getSize();

        updateTextDisplayCursor();
      }
    }

    else if (event.type == sf::Event::MouseWheelScrolled && isActive) {
      if (isMouseOver(background.getGlobalBounds(), mousePos)) {
        if (event.mouseWheelScroll.delta < 0) { // Scroll down
          scrollOffset =
              std::min(scrollOffset + TEXT_BLOCK_LINE_HEIGHT, maxScrollOffset);
        } else if (event.mouseWheelScroll.delta > 0) { // Scroll up
          scrollOffset = std::max(scrollOffset - TEXT_BLOCK_LINE_HEIGHT, 0.0f);
        }
        updateTextDisplayCursor(); // Update text position after scroll
      }
    }
  }

  void draw_with_cursor(sf::RenderWindow &window) {
    this->draw(window);
    if (isActive) {
      if (cursorBlinkClock.getElapsedTime().asSeconds() > CURSOR_BLINK_RATE) {
        showCursor = !showCursor;
        cursorBlinkClock.restart();
      }
      if (showCursor) {
        window.draw(cursorShape);
      }
    }
  }

  void updateTextDisplayCursor() {
    this->updateTextDisplay();
    if (isActive) {
      // Get the local position of the character at cursorPosition
      // This is relative to the textDisplay's origin (0,0)
      sf::Vector2f localCursorPos =
          textDisplay.findCharacterPos(cursorPosition);

      // Convert local position to global window position
      // textDisplay.getPosition() is the top-left of the text block
      cursorShape.setPosition(localCursorPos.x, localCursorPos.y);
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
  sf::View dropdownView;   // View for scrolling
  float scrollOffset;      // Current scroll offset
  float maxDropdownHeight; // Maximum height for the dropdown

  DropdownButton(sf::Vector2f position, sf::Vector2f size,
                 const std::string &label, sf::Font &font,
                 const std::vector<std::string> &dropdownItems,
                 float maxVisibleHeight)
      : Button(position, size, label, font), items(dropdownItems),
        isOpen(false), selectedIndex(-1), scrollOffset(0.0f),
        maxDropdownHeight(maxVisibleHeight) {
    dropdownBackground.setFillColor(sf::Color(50, 50, 50));
    dropdownBackground.setOutlineThickness(1.0f);
    dropdownBackground.setOutlineColor(sf::Color::White);

    // This is to fit the long file names
    // Not dynamic so change if possible
    // TODO
    this->text.setPosition(this->shape.getPosition().x + 40.0f,
                           this->text.getPosition().y);
  }

  void handleEvent(sf::Event event, const sf::Vector2f &mousePos,
                   sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
      if (isMouseOver(shape.getGlobalBounds(), mousePos)) {
        isOpen = !isOpen;
        if (isOpen) {
          scrollOffset = 0.0f; // Reset scroll when opening
        }
      } else if (isOpen) {
        // Convert mouse position to dropdown view coordinates
        sf::Vector2f dropdownMousePos =
            window.mapPixelToCoords(sf::Vector2i(mousePos), dropdownView);

        // Check if an item was clicked within the dropdown view
        for (size_t i = 0; i < items.size(); ++i) {
          sf::FloatRect itemRect(
              0,                        // Relative X position within the view
              i * DROPDOWN_ITEM_HEIGHT, // Relative Y position within the view
              shape.getSize().x, DROPDOWN_ITEM_HEIGHT);
          if (isMouseOver(itemRect, dropdownMousePos)) {
            selectedIndex = i;
            text.setString(
                items[selectedIndex]); // Update button text to selected item
            isOpen = false;            // Close dropdown after selection
            break;
          }
        }
      }
    } else if (event.type == sf::Event::MouseWheelScrolled) {
      if (isOpen &&
          isMouseOver(dropdownBackground.getGlobalBounds(), mousePos)) {
        // Adjust scroll offset based on mouse wheel delta
        scrollOffset += event.mouseWheelScroll.delta * DROPDOWN_ITEM_HEIGHT;

        // Clamp scroll offset to prevent scrolling too far
        float totalDropdownContentHeight = items.size() * DROPDOWN_ITEM_HEIGHT;
        if (totalDropdownContentHeight > maxDropdownHeight) {
          scrollOffset = std::max(
              scrollOffset, -(totalDropdownContentHeight - maxDropdownHeight));
          scrollOffset = std::min(scrollOffset, 0.0f);
        } else {
          scrollOffset = 0.0f; // No scrolling needed if content fits
        }
      }
    }
  }

  void draw(sf::RenderWindow &window) {
    Button::draw(window); // Draw the main button

    if (isOpen) {
      // Set up the dropdown background
      dropdownBackground.setPosition(shape.getPosition().x,
                                     shape.getPosition().y + shape.getSize().y);
      dropdownBackground.setSize(
          sf::Vector2f(shape.getSize().x,
                       std::min(maxDropdownHeight,
                                (float)items.size() * DROPDOWN_ITEM_HEIGHT)));
      window.draw(dropdownBackground);

      // Set up the view for the dropdown content
      dropdownView.setViewport(
          sf::FloatRect(dropdownBackground.getPosition().x / window.getSize().x,
                        dropdownBackground.getPosition().y / window.getSize().y,
                        dropdownBackground.getSize().x / window.getSize().x,
                        dropdownBackground.getSize().y / window.getSize().y));
      dropdownView.setSize(dropdownBackground.getSize());
      dropdownView.setCenter(dropdownBackground.getSize().x / 2.0f,
                             dropdownBackground.getSize().y / 2.0f -
                                 scrollOffset);

      window.setView(dropdownView);

      // Draw dropdown items within the view
      for (size_t i = 0; i < items.size(); ++i) {
        sf::Text itemText;
        itemText.setFont(*text.getFont());
        itemText.setString(items[i]);
        itemText.setCharacterSize(FONT_SIZE);
        itemText.setFillColor(sf::Color::White);
        itemText.setPosition(5, // Relative X position within the view
                             i * DROPDOWN_ITEM_HEIGHT +
                                 (DROPDOWN_ITEM_HEIGHT - FONT_SIZE) /
                                     2); // Relative Y position
        window.draw(itemText);
      }

      window.setView(window.getDefaultView()); // Reset to default view
    }
  }
};
