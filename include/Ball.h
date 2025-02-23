//
// Created by Catalin Cojocaru on 22/02/2025.
//

#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball {
private:
    sf::CircleShape shape;
    float ball_mov_speed = -250.f;
    std::array<float, 2> movMultiplier = {1, 0};  // x and y axis movement multipliers

public:
    explicit Ball(float x, float y, float radius = 8.f);

    // Setters / Getters
    void setRadius(const float radius) {shape.setRadius(radius);}
    float getRadius() const {return shape.getRadius();}

    void setPosition(float x, float y) {shape.setPosition({x, y});}
    sf::Vector2f getPosition() const {return shape.getPosition();}

    void setMovSpeed (const float speed) {ball_mov_speed = speed;}
    float getMovSpeed() const {return ball_mov_speed;}

    std::array<float, 2> getMovMultiplier() const {return movMultiplier;}
    void setMovMultiplier(float x, float y);

    sf::CircleShape getShape() const {return shape;};

    // Functionality
    void move(float dt);
};



#endif //BALL_H
