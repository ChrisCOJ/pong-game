//
// Created by Catalin Cojocaru on 22/02/2025.
//

#include "../include/Game.h"
#include "../include/Button.h"
#include <future>
#include <iostream>

#include "../cmake-build-debug/_deps/sfml-src/src/SFML/Window/InputImpl.hpp"


Game::Game(const sf::Vector2f windowResolution) :
    isPaused(false),
    windowResolution(windowResolution),
    window(sf::VideoMode(static_cast<sf::Vector2u>(windowResolution)), "Pong Game"),
    paddleLeft(15.f, 150.f, 500.f, "WS"),
    paddleRight(15.f, 150.f, 500.f,"Arrows"),
    defaultPaddleLeftPos{60.f, windowResolution.y/2 - paddleLeft.getSize().y/2},
    defaultPaddleRightPos{windowResolution.x - 60.f - paddleRight.getSize().x,
                          windowResolution.y/2 - paddleRight.getSize().y/2},
    paddles{ &paddleLeft, &paddleRight },
    ball(windowResolution.x/2, windowResolution.y / 2.f),
    playAgainButton("Play Again!"),
    player1Score(font),
    player2Score(font),
    fpsCounter(font),
    objects{&paddleLeft.getShape(), &paddleRight.getShape(), &ball.getShape(),
        &player1Score, &player2Score, &fpsCounter}

{
    fpsAveragingList.reserve(20);  // Use 20 dt values to calculate average fps and prevent fluctuations
    if (windowResolution.x < 100 || windowResolution.y < 100) {
        throw std::invalid_argument("Invalid window size, vertical/horizontal axis cannot be smaller than 100 pixels");
    }
    loadText();
    paddleLeft.setPosition(defaultPaddleLeftPos[0], defaultPaddleLeftPos[1]);
    paddleRight.setPosition(defaultPaddleRightPos[0], defaultPaddleRightPos[1]);
}


void Game::loadText() {
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }
    // Text init
    player1Score.setFont(font);
    player1Score.setCharacterSize(32);
    player1Score.setFillColor(sf::Color::White);
    player1Score.setPosition({windowResolution.x/4, 200.f});

    player2Score.setFont(font);
    player2Score.setCharacterSize(32);
    player2Score.setFillColor(sf::Color::White);
    player2Score.setPosition({windowResolution.x - windowResolution.x/4, 200.f});

    fpsCounter.setFont(font);
    fpsCounter.setCharacterSize(20);
    fpsCounter.setFillColor(sf::Color::White);
    fpsCounter.setPosition({windowResolution.x - 100.f, 30.f});
}


void Game::getFps() {
    int tempTotal = 0;

    while (running) {
        std::lock_guard lock(mutex);
        if (dt > 0.f) {
            int tempFps = static_cast<int>(1/dt);
            fpsAveragingList.emplace_back(tempFps);
        }
        if (fpsAveragingList.size() == 20) {
            for (const int num : fpsAveragingList) {
                tempTotal += num;
            }
            fps = static_cast<int>(tempTotal/fpsAveragingList.size());
            // Remove the first element to maintain a rolling window of 20 fps values
            fpsAveragingList.erase(fpsAveragingList.begin());
            tempTotal = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}


void Game::render(sf::RenderWindow& window, const std::vector<sf::Drawable*>& objects) {
    for (const auto* object : objects) {
        window.draw(*object);
    }
}


void Game::restartGame() {
    isPaused = false;
    score1 = 0;
    score2 = 0;
    paddleLeft.setPosition(defaultPaddleLeftPos[0], defaultPaddleLeftPos[1]);
    paddleRight.setPosition(defaultPaddleRightPos[0], defaultPaddleRightPos[1]);
    objects.erase(std::remove(objects.begin(), objects.end(), &playAgainButton), objects.end());
}


void Game::displayEndScreen(const std::string& side) {
    isPaused = true;
    playAgainButton.setTextSize(30);
    playAgainButton.setOnClick([this] {restartGame();});

    if (side == "left") {
        playAgainButton.setPosition(300.f, windowResolution.y / 2);
    }
    else if (side == "right") {
        playAgainButton.setPosition(windowResolution.x - 300.f, windowResolution.y / 2);
    }
    else {
        throw std::invalid_argument("Invalid side argument. Can be either 'left' or 'right'");
    }

    objects.emplace_back(&playAgainButton);
}


void Game::run() {
    fpsThread = std::thread(&Game::getFps, this);

    // Game loop
    while (window.isOpen()) {

        // Event loop
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>() && sf::priv::InputImpl::isKeyPressed(key::Escape)) {
                isPaused = !isPaused;
            }

            playAgainButton.onClick(window, event);
        }

        playAgainButton.onHover(window);

        // Set deltaTime
        sf::Time deltaTime = clock.restart();
        dt = deltaTime.asSeconds();  // Time elapsed since last frame in seconds

        if (!isPaused) {
            // ---------------------------- Paddle Movement ----------------------------
            for (Paddle* paddle : paddles) {
                if ((paddle->getControlOptions() == "WS" && isKeyPressed(key::S)) ||
                      (paddle->getControlOptions() == "Arrows" && isKeyPressed(key::Down))) {
                    paddle->move(dt, "down", windowResolution.y);
                }
                else if ((paddle->getControlOptions() == "Arrows" && isKeyPressed(key::Up)) ||
                           (paddle->getControlOptions() == "WS" && isKeyPressed(key::W))) {
                    paddle->move(dt, "up", windowResolution.y);
                }
            // -------------------------------------------------------------------------!

                // ---------------------- Handle ball collision with paddles ----------------------
                sf::FloatRect paddleBounds = paddle->getShape().getGlobalBounds();
                sf::FloatRect ballBounds = ball.getShape().getGlobalBounds();

                auto now = std::chrono::system_clock::now();
                auto timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(now - collisionTime);

                if (paddleBounds.findIntersection(ballBounds) && timeDifference.count() >= dt * 5) {
                    ball.setSpeed(ball.getSpeed() + 50.f);  // Increment speed by 50 following collision
                    // relativeDist calculates the exact part of the paddle the ball collides with
                    const float relativeDist = ball.getPosition().y + ball.getRadius() - paddle->getPosition().y;

                    if (ball.getHeadingDirection() == "right") {
                        // Maps the relative position of the ball on the paddle to a valid heading range
                        const float mapPosToHeading = 210.f + (330.f - 210.f)/paddle->getSize().y * relativeDist;
                        ball.setHeading(mapPosToHeading);
                    }
                    else {
                        const float mapPosToHeading = 150.f - (150.f - 30.f)/paddle->getSize().y * relativeDist;
                        ball.setHeading(mapPosToHeading);
                    }

                    collisionTime = std::chrono::system_clock::now();
                }
            }
            // ------------------------------------------------------------------------------------------ !

            // ---------------- Handle ball collision with the ceiling and floor ----------------
            if (ball.getPosition().y + ball.getRadius() * 2 >= windowResolution.y ||
                    ball.getPosition().y <= 0) {
                auto now = std::chrono::system_clock::now();
                auto timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(now - collisionTime);
                if (timeDifference.count() >= dt * 5) {  // If 5 frames have passed since the last collision
                    if (ball.getHeadingDirection() == "right") {
                        ball.setHeading(180 - ball.getHeading());
                    }
                    else {
                        ball.setHeading(360 + 180 - ball.getHeading());
                    }
                    collisionTime = std::chrono::system_clock::now();
                }
            }
            // ---------------------------------------------------------------------------------- !

            // ------------------ Handle ball collision with side walls ------------------
            if (ball.getPosition().x + ball.getRadius() * 2 > windowResolution.x) {
                ball.setSpeed(500.f);
                score1 += 1;
                ball.setPosition(windowResolution.x/2.f, windowResolution.y/2.f);
                ball.setHeading(90);  // 90 deg heading = (->)

            } else if (ball.getPosition().x < 0) {
                ball.setSpeed(500.f);
                score2 += 1;
                ball.setPosition(windowResolution.x/2.f, windowResolution.y/2.f);
                ball.setHeading(270);  // 270 deg heading = (<-)
            }
            // ---------------------------------------------------------------------------- !

            // ------- Pause the game and display an end screen with some buttons at the end of the game -------
            if (score1 == 10) {
                displayEndScreen("left");
            }
            else if (score2 == 10) {
                displayEndScreen("right");
            }
            // ------------------------------------------------------------------------------------------------- !

            ball.move(dt);
        }

        // Update objects
        fpsCounter.setString(std::to_string(fps) + " FPS");
        player1Score.setString(std::to_string(score1));
        player2Score.setString(std::to_string(score2));

        ///////////////////////////////
        window.clear();
        render(window, objects);
        window.display();
        ///////////////////////////////
    }
    // Wait for threads to finish execution
    running = false;
    fpsThread.join();
}
