//
// Created by Catalin Cojocaru on 22/02/2025.
//

#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

using key = sf::Keyboard::Key;

class Paddle {
private:
    float speed;
    sf::RectangleShape shape;
    const std::string controlOptions;
public:
    Paddle(float width, float height, float speed, std::string controlOptions);

    void setPosition(const float x, const float y) {shape.setPosition({x, y});}
    sf::Vector2f getPosition() const {return shape.getPosition();}

    sf::RectangleShape& getShape() {return shape;}

    std::string getControlOptions() const {return controlOptions;}

    void setSped(const float speed) {this->speed = speed;}
    float getSpeed() const {return speed;}

    sf::Vector2f getSize() const {return shape.getSize();}
    void setSize(float width, float height) {shape.setSize({width, height});}

    // Movement Handling
    void move(float dt, const std::string& movDirection, float verticalBounds);

};



#endif //PADDLE_H
