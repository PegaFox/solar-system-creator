#ifndef PEGAFOX_MENU_UTILS_HPP
#define PEGAFOX_MENU_UTILS_HPP

#include "utils.hpp"
#include <SFML/Graphics.hpp>

namespace pf {
  template <typename num>
  class Slider {
    public:
      
      std::string caption;

      sf::Vector2<num> range;

      Slider(num *var = nullptr) : var{var} {
        if (var == nullptr) {
          this->var = &value;
        }

        rect.width = 256;
        rect.height = 16; 

        outerBox.setSize(sf::Vector2f(rect.width, rect.height));
        outerBox.setFillColor(sf::Color(100, 100, 100));

        slide.setSize(sf::Vector2f(rect.height/2, rect.height));
        slide.setOrigin(sf::Vector2f(rect.height/4, 0));
        slide.setFillColor(sf::Color(200, 200, 200));

        label.setCharacterSize(rect.height);

        range = sf::Vector2<num>(0, 255);
      }

      void setValue(num *value) {
        var = value;
      }

      void setPosition(const sf::Vector2f& newPos) {
        rect.left = newPos.x;
        rect.top = newPos.y;
        outerBox.setPosition(newPos);
        slide.setPosition(sf::Vector2f(pf::adjustRange(*var, range.x, range.y, rect.left, rect.left+rect.width), rect.top));
        label.setPosition(newPos);
      }

      void setSize(const sf::Vector2f& newSize) {
        rect.width = newSize.x;
        rect.height = newSize.y;
        outerBox.setSize(newSize);
        slide.setSize(sf::Vector2f(newSize.y/2, newSize.y));
        label.setCharacterSize(newSize.y);
      }

      void setFrontColor(const sf::Color& newColor) {
        slide.setFillColor(newColor);
      }

      void setBackColor(const sf::Color& newColor) {
        outerBox.setFillColor(newColor);
      }

      void setFont(const std::string& newFont) {
        font.loadFromFile(newFont);
        label.setFont(font);
      }

      void setTextColor(const sf::Color& newColor) {
        label.setFillColor(newColor);
      }

      void getEvent(const sf::Event& event) {

        if (
          event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left &&
          rect.contains(event.mouseButton.x, event.mouseButton.y)
        ) {

          grabbed = true;

        } else if (event.type == sf::Event::MouseButtonReleased) {

          grabbed = false;

        }
        
      }

      void update(sf::RenderTarget& SCREEN, sf::Window& display) {
        slide.setPosition(sf::Vector2f(pf::adjustRange(*var, range.x, range.y, rect.left, rect.left+rect.width), rect.top));

        if (grabbed) {
          *var = pf::adjustRange(SCREEN.mapPixelToCoords(sf::Mouse::getPosition(display)).x, rect.left, rect.left+rect.width, range.x, range.y);
          if (*var > range.y) *var = range.y;
          if (*var < range.x) *var = range.x;
        }

        SCREEN.draw(outerBox);
        SCREEN.draw(slide);
        label.setString(caption+pf::floatToStr(*var));
        SCREEN.draw(label);
      }

    private:

      num value;
      num *var;

      bool grabbed = false;

      sf::RectangleShape outerBox;
      sf::RectangleShape slide;
      sf::Font font;
      sf::Text label;

      sf::FloatRect rect;
  };

  class Toggle {
    public:

      sf::Color onColor;
      sf::Color offColor;

      Toggle(bool *value = nullptr) : var{value} {

        if (value == nullptr) {
          var = &pressed;
        }

        rect.left = *var * 16;
        rect.width = 16;
        rect.height = 16;

        onColor = sf::Color::Green;
        offColor = sf::Color::Red;

        outerBox.setSize(sf::Vector2f(rect.width*2, rect.height));
        outerBox.setFillColor(*var ? onColor : offColor);

        flip.setSize(sf::Vector2f(rect.width, rect.height));
        flip.setFillColor(sf::Color(200, 200, 200));
        flip.setPosition(sf::Vector2f(rect.left, rect.top));

        label.setCharacterSize(rect.height);
      }

      void setValue(bool *value) {
        var = value;
      }

      bool getValue() {
        return *var;
      }

      void setPosition(const sf::Vector2f& newPos) {
        rect.left = newPos.x+(*var * rect.width);
        rect.top = newPos.y;
        outerBox.setPosition(newPos);
        flip.setPosition(sf::Vector2f(rect.left, rect.top));
        label.setPosition(newPos);
      }

      void setSize(const sf::Vector2f& newSize) {
        rect.width = newSize.x/2;
        rect.height = newSize.y;
        outerBox.setSize(newSize);
        flip.setSize(sf::Vector2f(newSize.x/2, newSize.y));
        label.setCharacterSize(newSize.y);
      }

      void setFrontColor(sf::Color& newColor) {
        flip.setFillColor(newColor);
      }

      void setFont(const std::string& newFont) {
        font.loadFromFile(newFont);
        label.setFont(font);
      }

      void setCaption(const sf::String& newCaption) {
        label.setString(newCaption);
      }

      void setTextColor(const sf::Color& newColor) {
        label.setFillColor(newColor);
      }

      void update(sf::RenderTarget& SCREEN, sf::Window& display) {
        sf::Vector2f mPos;

        rect.left = outerBox.getPosition().x+(*var * rect.width);
        flip.setPosition(sf::Vector2f(rect.left, rect.top));

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          mPos = SCREEN.mapPixelToCoords(sf::Mouse::getPosition(display));
          if (rect.contains(mPos.x, mPos.y))
            *var = !*var;
            outerBox.setFillColor(*var ? onColor : offColor);
        }
  
        SCREEN.draw(outerBox);
        SCREEN.draw(flip);
        SCREEN.draw(label);
      }

    private:

      bool *var = nullptr;
      bool pressed = false;

      sf::RectangleShape outerBox;
      sf::RectangleShape flip;
      sf::Font font;
      sf::Text label;

      sf::FloatRect rect;

  };

  class Button {
    public:

      sf::Color onColor;
      sf::Color offColor;

      Button(bool *value = nullptr) : var{value} {

        if (value == nullptr) {
          var = &pressed;
        }

        rect.width = 16;
        rect.height = 16;

        onColor = sf::Color(200, 200, 200);
        offColor = sf::Color(100, 100, 100);

        box.setSize(sf::Vector2f(rect.width, rect.height));
        box.setFillColor(*var ? onColor : offColor);

        label.setCharacterSize(rect.height);
      }

      void setValue(bool *value) {
        var = value;
      }

      bool getValue() {
        return *var;
      }

      void setPosition(const sf::Vector2f& newPos) {
        rect.left = newPos.x;
        rect.top = newPos.y;
        box.setPosition(newPos);
        label.setPosition(newPos);
      }

      void setSize(const sf::Vector2f& newSize) {
        rect.width = newSize.x;
        rect.height = newSize.y;
        box.setSize(newSize);
        label.setCharacterSize(newSize.y);
      }

      void setFont(const std::string& newFont) {
        font.loadFromFile(newFont);
        label.setFont(font);
      }

      void setCaption(const sf::String& newCaption) {
        label.setString(newCaption);
      }

      void setTextColor(const sf::Color& newColor) {
        label.setFillColor(newColor);
      }

      void update(sf::RenderTarget& SCREEN, sf::Window& display) {
        sf::Vector2f mPos;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          mPos = SCREEN.mapPixelToCoords(sf::Mouse::getPosition(display));
          if (rect.contains(mPos.x, mPos.y)) {
            *var = true;
            pressed = true;
          } else if (pressed) {
            *var = false;
            pressed = false;
          }
          box.setFillColor(*var ? onColor : offColor);
        } else if (pressed) {
          *var = false;
          pressed = false;
          box.setFillColor(*var ? onColor : offColor);
        }

        SCREEN.draw(box);
        SCREEN.draw(label);
      }

    private:

      bool pressed = false;
      bool *var = nullptr;

      sf::Font font;
      sf::Text label;

      sf::RectangleShape box;

      sf::FloatRect rect;
  };

  class TextBox {
    public:

      bool lineWrap = true;

      std::string caption;

      TextBox(std::string* var = nullptr) : var{var} {

        if (var == nullptr) {
          this->var = &value;
        }

        rect.width = 16;
        rect.height = 16;

        box.setSize(sf::Vector2f(rect.width, rect.height));
        box.setFillColor(sf::Color(100, 100, 100));

        label.setCharacterSize(rect.height);

      }

      void setValue(std::string* var) {
        this->var = var;
      }

      std::string getValue() {
        return *var;
      }

      void setPosition(const sf::Vector2f& newPos) {
        rect.left = newPos.x;
        rect.top = newPos.y;
        box.setPosition(newPos);
        label.setPosition(newPos);
      }

      void setSize(const sf::Vector2f& newSize) {
        rect.width = newSize.x;
        rect.height = newSize.y;
        box.setSize(newSize);
      }

      void setFont(const std::string& newFont) {
        font.loadFromFile(newFont);
        label.setFont(font);
      }

      void setFontSize(const float size) {
        label.setCharacterSize(size);
      }

      void setBoxColor(const sf::Color& newColor) {
        box.setFillColor(newColor);
      }

      void setTextColor(const sf::Color& newColor) {
        label.setFillColor(newColor);
      }

      void getEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

          selected = rect.contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));

        } else if (event.type == sf::Event::TextEntered && selected) {

          if (event.text.unicode == '\b') {
            if (!var->empty()) {
              var->pop_back();
            }
          } else if (event.text.unicode == '\r') {
            var->push_back('\n');
          } else {
            var->push_back(event.text.unicode);
          }

        } else if (event.type == sf::Event::MouseWheelScrolled && selected) {
          scroll += event.mouseWheelScroll.delta;
        }
      }

      void update(sf::RenderTarget& SCREEN) {
        sf::Color labelColor = label.getFillColor();

        if (!selected && var->empty()) { // use inverse of text color for caption

          label.setString(caption);
          
          label.setFillColor(sf::Color(labelColor.toInteger()^0xFFFFFF00));

        } else if (lineWrap) {

          std::string processedValue = *var;

          if (selected) {
            processedValue.push_back('_');
          }

          float lineWidth = 0.0f;

          for (std::string::iterator character = processedValue.begin(); character < processedValue.end(); character++) {

            sf::Glyph glyph = font.getGlyph(*character, label.getCharacterSize(), false);

            lineWidth += glyph.advance;

            if (*character == '\n') {
              lineWidth = 0.0f;
            }

            if (lineWidth > rect.width) {
              processedValue.insert(character, '\n');
              lineWidth = glyph.advance;
            }

          }

          label.setString(processedValue);

        } else {

          std::string processedValue = var->substr(0, var->size() - scroll);

          if (selected && scroll < 2) {
            processedValue.push_back('_');
          }

          float lineWidth = 0.0f;

          for (std::string::iterator character = processedValue.end(); character >= processedValue.begin(); character--) {

            sf::Glyph glyph = font.getGlyph(*character, label.getCharacterSize(), false);

            lineWidth += glyph.advance;

            if (*character == '\n') {
              lineWidth = 0.0f;
            }

            if (lineWidth > rect.width) {
              processedValue.erase(processedValue.begin(), character);
              break;
            }

          }

          label.setString(processedValue);

        }

        SCREEN.draw(box);
        SCREEN.draw(label);
        label.setFillColor(labelColor);
      }

    private:

      uint scroll = 0;

      bool selected = false;

      std::string* var;

      std::string value;

      sf::Font font;
      sf::Text label;

      sf::RectangleShape box;

      sf::FloatRect rect;
  };
}
#endif