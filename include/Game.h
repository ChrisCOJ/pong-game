//
// Created by Catalin Cojocaru on 22/02/2025.
//

#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "Paddle.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include "../include/Button.h"


class Game {
private:
    // Default variables
    float dt = 0;
    int score1 = 0;
    int score2 = 0;
    int fps = 0;
    std::atomic<bool> running = true;
    std::chrono::time_point<std::chrono::system_clock> collisionTime;
    sf::Clock clock;
    std::mutex mutex;
    std::thread fpsThread;
    sf::Font font;

    bool isPaused;
    sf::Vector2f windowResolution;
    std::vector<int> fpsAveragingList;
    sf::RenderWindow window;
    Paddle paddleLeft;
    Paddle paddleRight;
    const std::array<float, 2> defaultPaddleLeftPos;
    const std::array<float, 2> defaultPaddleRightPos;
    const std::array<Paddle*, 2> paddles;
    Ball ball;
    Button playAgainButton;

    // Text
    sf::Text player1Score;
    sf::Text player2Score;
    sf::Text fpsCounter;

    std::vector<sf::Drawable*> objects;

    static void render(sf::RenderWindow& window, const std::vector<sf::Drawable*>& objects);
    void getFps();
    void loadText();
    void displayEndScreen(const std::string& side);
    void restartGame();

public:
    explicit Game(sf::Vector2f windowResolution);
    void run();
};


#endif //GAME_H