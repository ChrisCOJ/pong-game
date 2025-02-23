//
// Created by Catalin Cojocaru on 22/02/2025.
//

#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

using key = sf::Keyboard::Key;

class Paddle {
private:
    float speed = 200.f;
    sf::RectangleShape shape;
    std::string direction;
    int playerNumber;
public:
    Paddle(float width, float height, int playerNumber);

    void setPosition(const float x, const float y) {shape.setPosition({x, y});}
    sf::Vector2f getPosition() const {return shape.getPosition();}

    sf::RectangleShape getShape() const {return shape;}

    std::string getDirection() const {return direction;}

    int getPlayerNumber() const {return playerNumber;}

    sf::Vector2f getSize() const {return shape.getSize();}
    void setSize(float width, float height) {shape.setSize({width, height});}

    // Movement Handling
    void move(float dt);

};



#endif //PADDLE_H
