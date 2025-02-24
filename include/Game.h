//
// Created by Catalin Cojocaru on 22/02/2025.
//

#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "Paddle.h"
#include <SFML/Graphics.hpp>
#include <thread>


class Game {
private:
    sf::Vector2f windowResolution;
    // Default variables
    float dt = 0;
    int score1 = 0;
    int score2 = 0;
    int fps = 0;
    std::atomic<bool> running = true;
    sf::Clock clock;
    std::mutex mutex;
    std::thread fpsThread;
    sf::Font font;

    sf::RenderWindow window;
    Paddle paddleLeft;
    Paddle paddleRight;
    Ball ball;

    // Text
    sf::Text player1Score;
    sf::Text player2Score;
    sf::Text fpsCounter;

    std::vector<sf::Drawable*> objects;

    void updateObjects();

    static void render(sf::RenderWindow& window, const std::vector<sf::Drawable*>& objects);
    void getFps();
    void loadText();

public:
    explicit Game(sf::Vector2f windowResolution);
    void run();
};


#endif //GAME_H