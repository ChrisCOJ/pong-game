//
// Created by Catalin Cojocaru on 22/02/2025.
//

#include "../include/Game.h"
#include <future>
#include <iostream>


Game::Game(const sf::Vector2f windowResolution) :
    windowResolution(windowResolution),
    window(sf::VideoMode(static_cast<sf::Vector2u>(windowResolution)), "Pong Game"),
    paddleLeft(15.f, 150.f, 350.f, "WASD"),
    paddleRight(15.f, 150.f, 350.f,"Arrows"),
    ball(windowResolution.x/2, windowResolution.y / 2.f),
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
    paddleLeft.setPosition(60.f, windowResolution.y/2 - paddleLeft.getSize().y/2);
    paddleRight.setPosition(windowResolution.x - 60.f - paddleRight.getSize().x,
                            windowResolution.y/2 - paddleRight.getSize().y/2);
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

void Game::updateObjects() {
    fpsCounter.setString(std::to_string(fps) + " FPS");
    player1Score.setString(std::to_string(score1));
    player2Score.setString(std::to_string(score2));
    ball.move(dt);
}

void Game::run() {
    // Calculates fps every 400 ms
    fpsThread = std::thread(&Game::getFps, this);

    // Game loop
    while (window.isOpen()) {
        // -------------------- Event loop --------------------
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        // ---------------------------------------------------- !
        // Set deltaTime
        sf::Time deltaTime = clock.restart();
        dt = deltaTime.asSeconds();  // Time elapsed since last frame in seconds

        updateObjects();

        // Paddle Movement
        std::array<Paddle*, 2> paddles = { &paddleLeft, &paddleRight };
        for (Paddle* paddle : paddles) {
            if ((paddle->getControlOptions() == "WASD" && isKeyPressed(key::S)) ||
                  (paddle->getControlOptions() == "Arrows" && isKeyPressed(key::Down))) {
                paddle->move(dt, "down", windowResolution.y);
            }
            else if ((paddle->getControlOptions() == "Arrows" && isKeyPressed(key::Up)) ||
                       (paddle->getControlOptions() == "WASD" && isKeyPressed(key::W))) {
                paddle->move(dt, "up", windowResolution.y);
            }

            // Collision detection with the ball
            if (paddle->getShape().getGlobalBounds().findIntersection(ball.getShape().getGlobalBounds())) {}
        }


        if (ball.getPosition().y + ball.getRadius() * 2 > windowResolution.y ||
            ball.getPosition().y < 0){
            ball.setMovMultiplier(ball.getMovMultiplier()[0], -ball.getMovMultiplier()[1]);
        }
        // ----------------------------------------------------------------- !

        // Adjust score
        if (ball.getPosition().x + ball.getRadius() * 2 > windowResolution.x) {
            score1 += 1;
            ball.setPosition(windowResolution.x/2.f, windowResolution.y/2.f);
            ball.setMovMultiplier(ball.getMovMultiplier()[0], 0);

        } else if (ball.getPosition().x < 0) {
            score2 += 1;
            ball.setPosition(windowResolution.x/2.f, windowResolution.y/2.f);
            ball.setMovMultiplier(ball.getMovMultiplier()[0], 0);
        }

        window.clear();
        render(window, objects);
        window.display();
    }

    running = false;
    fpsThread.join();
}
