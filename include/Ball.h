//
// Created by Catalin Cojocaru on 22/02/2025.
//

#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball {
private:
    sf::CircleShape shape;
    float speed;
    float heading;
    float xMultiplier;  // Multiply x offset by this when shape.move() is called
    float yMultiplier;  // Multiply y offset by this when shape.move() is called

public:
    explicit Ball(float x, float y, float speed = 500.f, float radius = 8.f);

    // Setters / Getters
    void setRadius(const float radius) {shape.setRadius(radius);}
    float getRadius() const {return shape.getRadius();}

    void setPosition(float x, float y) {shape.setPosition({x, y});}
    sf::Vector2f getPosition() const {return shape.getPosition();}

    void setSpeed (const float speed) {this->speed = speed;}
    float getSpeed() const {return speed;}

    float getHeading() const {return heading;}
    void setHeading(float heading);

    std::string getHeadingDirection() const;

    sf::CircleShape& getShape() {return shape;};

    // Functionality
    void move(float dt);
};



#endif //BALL_H
