//
// Created by Catalin Cojocaru on 22/02/2025.
//

#include <utility>

#include "../include/Paddle.h"


Paddle::Paddle(const float width, const float height, const float speed, const std::string& controlOptions)
    : speed(speed),
      controlOptions(std::move(controlOptions))
{
    shape.setSize({width, height});
    shape.setFillColor(sf::Color(255, 255, 255));
}


void Paddle::move(const float dt, const std::string& movDirection, const float verticalBounds) {
    // Check if the paddle is within it's vertical bounds, then call move
    if (!(shape.getPosition().y < 0)) {
        if (movDirection == "up") {
            shape.move({0, speed * dt * -1.f});  // Negative y offset to move upwards
        }
    }
    if (!(shape.getPosition().y + shape.getSize().y > verticalBounds)) {
        if (movDirection == "down") {
            shape.move({0, speed * dt});  // Positive y offset to move downwards
        }
    }
}