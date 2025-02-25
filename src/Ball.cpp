//
// Created by Catalin Cojocaru on 22/02/2025.
//

#include <SFML/Graphics.hpp>
#include "../include/Ball.h"


Ball::Ball(float x, float y, const float speed, const float radius)
    : speed(speed),
      heading(90),
      xMultiplier(1),
      yMultiplier(0)
{
    shape.setRadius(radius);
    shape.setPosition({x, y});
}

void Ball::setHeading(const float heading) {
    if (heading < 0 || heading > 360) {
        throw std::invalid_argument("Invalid argument range. Heading can range from 0 to 360");
    }
    this->heading = heading;
    xMultiplier = std::cos(heading * static_cast<float>(M_PI) / 180);
    yMultiplier = std::sin(heading * static_cast<float>(M_PI) / 180);
}

// Functionality
void Ball::move(const float dt) {
    shape.move({speed * dt * xMultiplier, speed * dt * yMultiplier});
}