//
// Created by Catalin Cojocaru on 26/02/2025.
//

#include "../include/Button.h"
#include <iostream>



Button::Button(const std::string& text, const float x, const float y)
    : textBox(font),
      text(text),
      x(x), y(y),
      padding(20)
{
    init();
}


void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(textBorder, states);
    target.draw(textBox, states);
}


void Button::onHover(const sf::RenderWindow &window) {
    const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    const sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

    if (textBorder.getGlobalBounds().contains(worldMousePos)) {
        textBorder.setFillColor(sf::Color(255, 255, 255, 150));
    }
    else {
        textBorder.setFillColor(sf::Color(255, 255, 255, 255));
    }
}


void Button::onClick(const sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    const sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

    if (textBorder.getGlobalBounds().contains(worldMousePos)) {
        if (event->is<sf::Event::MouseButtonPressed>()) {
            textBorder.setFillColor(sf::Color(255, 255, 255, 50));
            if (onClickAction) {
                onClickAction();
            }
        }
        else if (event->is<sf::Event::MouseButtonReleased>()) {
            textBorder.setFillColor(sf::Color(255, 255, 255, 150));
        }
    }
}


void Button::setOnClick(const std::function<void()> &callback) {
    onClickAction = callback;
}


void Button::setVisibility(const bool visibility) {
    if (visibility) {
        textBorder.setFillColor(sf::Color::Transparent);
        textBox.setFillColor(sf::Color::Transparent);
    }
    else {
        textBorder.setFillColor(sf::Color::White);
        textBox.setFillColor(sf::Color::Black);
    }
}



void Button::init() {
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }

    textBox.setFont(font);
    textBox.setCharacterSize(20);
    textBox.setString(text);
    textBox.setFillColor(sf::Color::Black);
    resizeBorder();
    centerText();
    this->textBorder.setPosition({x, y});
    this->textBorder.setFillColor(sf::Color::White);
}


void Button::centerText() {
    textBox.setPosition({x + ((textBox.getLocalBounds().size.x + padding)/2 - textBox.getLocalBounds().size.x/2),
                        y + ((textBox.getLocalBounds().size.y + padding - 10)/2 - textBox.getLocalBounds().size.y/2)});
}


void Button::resizeBorder() {
    textBorder.setSize({textBox.getLocalBounds().size.x + padding, textBox.getLocalBounds().size.y + padding});
}


void Button::setPosition(const float x, const float y) {
    this->x = x;
    this->y = y;
    textBorder.setPosition({x, y});
    centerText();
}


void Button::setBorderColor(const sf::Color& color) {
    textBorder.setFillColor(color);
}


void Button::setTextColor(const sf::Color& color) {
    textBox.setFillColor(color);
}


void Button::setTextSize(const int size) {
    textBox.setCharacterSize(size);
    resizeBorder();
}


void Button::setText(const std::string& text) {
    this->text = text;
    textBox.setString(text);
    resizeBorder();
}
