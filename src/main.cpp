// Pong-Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// (resolution.y/2.f) - height/2.f

#include <iostream>

#include "../include/Game.h"


int main() {
    try {
        Game pongGame = Game({1200, 800});
        pongGame.run();
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
