// Pong-Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// (resolution.y/2.f) - height/2.f

#include <iostream>
#include <future>
#include <SFML/Graphics.hpp>


constexpr sf::Vector2u RESOLUTION = { 800, 600 };
static std::mutex mutex;
int fps = 0;
float dt = 0.f;
std::atomic running = true;

using key = sf::Keyboard::Key;


class Paddle {
private:
    sf::RectangleShape shape;
    // float x;
    float speed = 200.f;
public:
    Paddle(float width, float height, float x, float y) {
        // this->x = x;
        shape.setSize({width, height});
        shape.setPosition({x, y});
        shape.setFillColor(sf::Color(255, 255, 255));
    }

    void setPosition(float x, float y) {shape.setPosition({x, y});}  // Fix paddle on the x-axis
    sf::Vector2f getPosition() const {return shape.getPosition();}
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
    float ball_mov_speed = -250.f;
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

void getFps() {
    while (running) {
        std::lock_guard lock(mutex);
        if (dt > 0.f) {
            fps = static_cast<int>(1/dt);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
}


int main() {
    // ------------------------------ INIT ------------------------------
    // Variable definitions
    sf::Clock clock;
    // Window
    sf::RenderWindow window(sf::VideoMode(RESOLUTION), "SFML window");

    // Paddle objects
    constexpr float HEIGHT = 80.f;
    float constexpr WIDTH = 10.f;
    float constexpr dx = 30.f;  // Padding from the left and right screen borders
    Paddle paddle1(WIDTH, HEIGHT, dx, RESOLUTION.y/2.f - HEIGHT/2.f);
    Paddle paddle2(WIDTH, HEIGHT, RESOLUTION.x-WIDTH-dx, RESOLUTION.y/2.f - HEIGHT/2.f);

    // Pong ball
    Ball ball(RESOLUTION.x / 2.f, RESOLUTION.y / 2.f);

    // Declare a font
    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }

    sf::Text text(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition({RESOLUTION.x - 130.f, 30.f});
    // ------------------------------------------------------------------- !

    std::thread fpsThread(getFps);

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
        text.setString(std::to_string(fps) + " FPS");

        // Movement
        paddle1.move(dt, 1);
        paddle2.move(dt, 2);
        ball.move(dt, 0);

        // ---------------------- Collision detection ----------------------
        if (paddle1.getShape().getGlobalBounds().findIntersection(ball.getShape().getGlobalBounds()) ||
            paddle2.getShape().getGlobalBounds().findIntersection(ball.getShape().getGlobalBounds())) {
            ball.setMovSpeed(ball.getMovSpeed() * -1);
        }
        if ((ball.getPosition().x + ball.getRadius() * 2 > RESOLUTION.x && ball.getMovSpeed() > 0) ||
            (ball.getPosition().x < 0 && ball.getMovSpeed() < 0)) {

            ball.setMovSpeed(ball.getMovSpeed() * -1);  // Change the ball's movement direction
        }
        // ----------------------------------------------------------------- !

        // ------------- Draw stuff to the window -------------
        window.clear();

        window.draw(paddle1.getShape());
        window.draw(paddle2.getShape());
        window.draw(ball.getShape());
        window.draw(text);

        window.display();
        // ---------------------------------------------------- !
    }
    running = false;
    fpsThread.join();
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.