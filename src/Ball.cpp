//
// Created by Catalin Cojocaru on 22/02/2025.
//

#include <SFML/Graphics.hpp>
#include "../include/Ball.h"


Ball::Ball(float x, float y, const float radius) {
    shape.setRadius(radius);
    shape.setPosition({x, y});
}

void Ball::setMovMultiplier(const float x, const float y) {
    if (x > 1 || x < -1  || y > 1 || y < -1) {
        throw std::invalid_argument("Invalid argument range. x and y can range from -1 to 1 inclusive");
    }
    movMultiplier[0] = x;
    movMultiplier[1] = y;
}

// Functionality
void Ball::move(float const dt) {
    shape.move({movMultiplier[0] * ball_mov_speed * dt, movMultiplier[1] * ball_mov_speed * dt});
}