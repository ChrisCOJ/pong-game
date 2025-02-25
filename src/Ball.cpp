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
    /// Ball heading in degrees
    /// 0 deg = (↓) | 90 deg = (->) | 180 deg (↑) | 270 deg (<-)
    if (heading < 0 || heading > 360) {
        throw std::invalid_argument("Invalid argument range. Heading can range from 0 to 360");
    }
    this->heading = heading;
    xMultiplier = std::sin(heading * static_cast<float>(M_PI) / 180);
    yMultiplier = std::cos(heading * static_cast<float>(M_PI) / 180);
}

std::string Ball::getHeadingDirection () const {
    if (const float temp = 180 - getHeading(); temp > 0) {
        return "right";
    }
    return "left";
}

// Functionality2
void Ball::move(const float dt) {
    shape.move({speed * dt * xMultiplier, speed * dt * yMultiplier});
}