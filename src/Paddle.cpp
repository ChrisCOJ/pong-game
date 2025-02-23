//
// Created by Catalin Cojocaru on 22/02/2025.
//

#include "../include/Paddle.h"


Paddle::Paddle(const float width, const float height, const int playerNumber)
    : playerNumber(playerNumber)
{
    shape.setSize({width, height});
    shape.setFillColor(sf::Color(255, 255, 255));
}

void Paddle::move(float dt) {
    if ((playerNumber == 1 && isKeyPressed(key::S)) ||
    (playerNumber == 2 && isKeyPressed(key::Down))) {
        direction = "down";
        shape.move({0, speed * dt});
    } else if ((playerNumber == 1 && isKeyPressed(key::W)) ||
        (playerNumber == 2 && isKeyPressed(key::Up))) {
        direction = "up";
        shape.move({0, speed * dt * -1.f});
        } else if (!isKeyPressed(key::W) && !isKeyPressed(key::Up) && !isKeyPressed(key::Down) && !isKeyPressed(key::S)) {
            direction = "N/A";
        } else if (playerNumber != 1 && playerNumber != 2) {
            throw std::invalid_argument("Invalid playerNumber argument. Must be either 1 or 2!");
        }
}