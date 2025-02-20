// Pong-Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// (resolution.y/2.f) - height/2.f

#include <iostream>
#include <SFML/Graphics.hpp>

using key = sf::Keyboard::Key;

class Paddle {
private:
    sf::RectangleShape shape;
    float x;
    float speed = 200.f;
public:
    Paddle(float width, float height, float x, float y) {
        this->x = x;
        shape.setSize({width, height});
        shape.setFillColor(sf::Color(255, 255, 255));
        shape.setPosition({x, y});
    }

    void setPosition(float y) {
        shape.setPosition({this->x, y});  // Fix paddle on the x-axis
    }
    sf::RectangleShape getShape() const {return shape;}

    // Movement Handling
    void move(float const dt, int const playerNumber) {
        if (playerNumber == 1) {
            if (isKeyPressed(key::S)) {
                shape.move({0, speed * dt});
            } else if (isKeyPressed(key::W)) {
                shape.move({0, speed * dt * -1.f});
            }
        } else if (playerNumber == 2) {
            if (isKeyPressed(key::Down)) {
                shape.move({0, speed * dt});
            } else if (isKeyPressed(key::Up)) {
                shape.move({0, speed * dt * -1.f});
            }
        }
        else {
            throw std::invalid_argument("Invalid playerNumber argument. Must be either 1 or 2!");
        }
    }
};


class Ball {
private:
    sf::CircleShape shape;
    float ball_mov_speed = 250.f;
public:
    explicit Ball(float x, float y, float radius = 8.f) {
        shape.setRadius(radius);
        shape.setPosition({x, y});
    }

    // Setters / Getters
    void setRadius(float radius) {shape.setRadius(radius);}
    float getRadius() const {return shape.getRadius();}

    void setPosition(float x, float y) {shape.setPosition({x, y});}
    sf::Vector2f getPosition() const {return shape.getPosition();}

    void setMovSpeed (float speed) {ball_mov_speed = speed;}
    float getMovSpeed() const {return ball_mov_speed;}

    sf::CircleShape getShape() const {return shape;}

    // Functionality
    void move(float dx, float dy) {
        shape.move({ball_mov_speed * dx, ball_mov_speed * dy});
    }
};


int main()
{
    // ------------------------------ INIT ------------------------------
    // Variable definitions
    sf::Clock clock;
    constexpr sf::Vector2u RESOLUTION = { 800, 600 };

    // Window
    sf::RenderWindow window(sf::VideoMode(RESOLUTION), "SFML window");

    // Paddle objects
    float constexpr HEIGHT = 80.f;
    float constexpr WIDTH = 10.f;
    float constexpr dx = 30.f;  // Padding from the left and right screen borders
    Paddle paddle1(WIDTH, HEIGHT, dx, RESOLUTION.y/2.f - HEIGHT/2.f);
    Paddle paddle2(WIDTH, HEIGHT, RESOLUTION.x-WIDTH-dx, RESOLUTION.y/2.f - HEIGHT/2.f);

    // Pong ball
    Ball ball(RESOLUTION.x / 2.f, RESOLUTION.y / 2.f);
    // ------------------------------------------------------------------- !


    // Game loop
    while (window.isOpen()) {
        // -------------------- Event loop --------------------
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        // ---------------------------------------------------- !
        window.clear();

        // Set deltaTime
        sf::Time deltaTime = clock.restart();
        const float dt = deltaTime.asSeconds();  // Time elapsed since last frame in seconds

        // Movement
        paddle1.move(dt, 1);
        paddle2.move(dt, 2);
        // ---------------------- Collision detection ----------------------
        // - Figure out the point on the paddles closest to the ball
        // - Check the distance between that point and the center of the ball
        // - If the distance is smaller than the ball radius, multiply BALL_MOV_SPEED by -1 to change its
        // movement direction (temporary logic)
        // ----------------------------------------------------------------- !

        // ------------ Update position of objects ------------
        ball.move(-dt, 0);
        // ---------------------------------------------------- !


        // ------------- Draw stuff to the window -------------
        window.draw(paddle1.getShape());
        window.draw(paddle2.getShape());
        window.draw(ball.getShape());
        // ---------------------------------------------------- !

        window.display();
    }
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.