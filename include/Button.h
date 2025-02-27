//
// Created by Catalin Cojocaru on 26/02/2025.
//

#ifndef BUTTON_H
#define BUTTON_H


#include <SFML/Graphics.hpp>


class Button : public sf::Drawable {
private:
    sf::Font font;
    sf::RectangleShape textBorder;
    sf::Text textBox;
    std::string text;
    float x, y;
    const int padding;

    std::function<void()> onClickAction;

    void init();
    void centerText();
    void resizeBorder();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
public:
    explicit Button(const std::string& text, float x = 0.f, float y = 0.f);

    void onHover(const sf::RenderWindow &window);
    void onClick(const sf::RenderWindow &window, const std::optional<sf::Event> &event);
    void setOnClick(const std::function<void()> &callback);
    void setVisibility(bool visibility);
    void setPosition(float x, float y);
    void setBorderColor(const sf::Color& color);
    void setText(const std::string& text);
    void setTextColor(const sf::Color& color);
    void setTextSize(int size);
};



#endif //BUTTON_H
