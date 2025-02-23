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
    sf::Vector2u windowResolution;
    // Default variables
    float dt = 0;
    int score1 = 0;
    int score2 = 0;
    int fps = 0;
    std::atomic<bool> running = true;

    sf::Clock clock;
    sf::RenderWindow window;
    Paddle paddleLeft;
    Paddle paddleRight;
    Ball ball;

    // Text
    sf::Font font;
    sf::Text player1Score;
    sf::Text player2Score;
    sf::Text fpsCounter;

    std::mutex mutex;
    std::thread fpsThread;

    // void update();
    void render();
    void getFps();
    void loadText();

public:
    Game();
    void run();
};
//    // Game loop
//    while (window.isOpen()) {
//        // -------------------- Event loop --------------------
//        while (const std::optional event = window.pollEvent()) {
//            if (event->is<sf::Event::Closed>()) {
//                window.close();
//            }
//        }
//        // ---------------------------------------------------- !
//        // Set deltaTime
//        sf::Time deltaTime = clock.restart();
//        dt = deltaTime.asSeconds();  // Time elapsed since last frame in seconds
//        fpsCounter.setString(std::to_string(fps) + " FPS");
//        player1Score.setString(std::to_string(score1));
//        player2Score.setString(std::to_string(score2));
//
//        // Movement
//        ball.move(dt);
//
//        // ---------------------- Collision detection ----------------------
//        for (Paddle* paddle : paddles) {
//            paddle->move(dt);
//            if (paddle->getShape().getGlobalBounds().findIntersection(ball.getShape().getGlobalBounds())) {
//                if (paddle->getDirection() == "up") {
//                    ball.setMovMultiplier(-ball.getMovMultiplier()[0], 1);
//                } else if (paddle->getDirection() == "down") {
//                    ball.setMovMultiplier(-ball.getMovMultiplier()[0], -1);
//                } else {ball.setMovMultiplier(-ball.getMovMultiplier()[0], ball.getMovMultiplier()[1]);}
//            }
//        }
//
//
//        if (ball.getPosition().y + ball.getRadius() * 2 > RESOLUTION.y ||
//            ball.getPosition().y < 0){
//            ball.setMovMultiplier(ball.getMovMultiplier()[0], -ball.getMovMultiplier()[1]);
//        }
//        // ----------------------------------------------------------------- !
//
//        // Adjust score
//        if (ball.getPosition().x + ball.getRadius() * 2 > RESOLUTION.x) {
//            score2 += 1;
//            ball.setPosition(RESOLUTION.x/2.f, RESOLUTION.y/2.f);
//            ball.setMovMultiplier(ball.getMovMultiplier()[0], 0);
//
//        } else if (ball.getPosition().x < 0) {
//            score1 += 1;
//            ball.setPosition(RESOLUTION.x/2.f, RESOLUTION.y/2.f);
//            ball.setMovMultiplier(ball.getMovMultiplier()[0], 0);
//        }
//
//        // ------------- Draw stuff to the window -------------
//        window.clear();
//
//        window.draw(paddle1.getShape());
//        window.draw(paddle2.getShape());
//        window.draw(ball.getShape());
//        window.draw(fpsCounter);
//        window.draw(player1Score);
//        window.draw(player2Score);
//
//        window.display();
//        // ---------------------------------------------------- !
//    }
//    running = false;
//    fpsThread.join();
//    return 0;
//};



#endif //GAME_H
