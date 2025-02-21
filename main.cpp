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
    float speed = 200.f;
    std::string direction;
    int playerNumber;
public:
    Paddle(float width, float height, float x, float y, const int playerNumber) {
        this->playerNumber = playerNumber;
        shape.setSize({width, height});
        shape.setPosition({x, y});
        shape.setFillColor(sf::Color(255, 255, 255));
    }

    void setPosition(float x, float y) {shape.setPosition({x, y});}  // Fix paddle on the x-axis
    sf::Vector2f getPosition() const {return shape.getPosition();}
    sf::RectangleShape getShape() const {return shape;}
    std::string getDirection() const {return direction;}
    int getPlayerNumber() const {return playerNumber;}

    // Movement Handling
    void move(float const dt) {
        if ((playerNumber == 1 && isKeyPressed(key::S)) ||
            (playerNumber == 2 && isKeyPressed(key::Down))) {
            direction = "down";
            shape.move({0, speed * dt});
        } else if ((playerNumber == 1 && isKeyPressed(key::W)) ||
            (playerNumber == 2 && isKeyPressed(key::Up))) {
            direction = "up";
            shape.move({0, speed * dt * -1.f});
        } else if (!isKeyPressed(key::W) && !isKeyPressed(key::Up) && !isKeyPressed(key::Down) && !isKeyPressed(key::S)) {
            direction = "";
        } else if (playerNumber != 1 && playerNumber != 2) {
            throw std::invalid_argument("Invalid playerNumber argument. Must be either 1 or 2!");
        }
    }

};


class Ball {
private:
    sf::CircleShape shape;
    float ball_mov_speed = -250.f;
    std::array<float, 2> movMultiplier = {1, 0};  // x and y axis movement multipliers
public:
    explicit Ball(float x, float y, const float radius = 8.f) {
        shape.setRadius(radius);
        shape.setPosition({x, y});
    }

    // Setters / Getters
    void setRadius(const float radius) {shape.setRadius(radius);}
    float getRadius() const {return shape.getRadius();}

    void setPosition(float x, float y) {shape.setPosition({x, y});}
    sf::Vector2f getPosition() const {return shape.getPosition();}

    void setMovSpeed (const float speed) {ball_mov_speed = speed;}
    float getMovSpeed() const {return ball_mov_speed;}

    std::array<float, 2> getMovMultiplier() const {return movMultiplier;}
    void setMovMultiplier(const float x, const float y) {
        if (x > 1 || x < -1  || y > 1 || y < -1) {
            throw std::invalid_argument("Invalid argument range. x and y can range from -1 to 1 inclusive");
        }
        movMultiplier[0] = x;
        movMultiplier[1] = y;
    }

    sf::CircleShape getShape() const {return shape;}

    // Functionality
    void move(float const dt) {
        shape.move({movMultiplier[0] * ball_mov_speed * dt, movMultiplier[1] * ball_mov_speed * dt});
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
    Paddle paddle1(WIDTH, HEIGHT, dx, RESOLUTION.y/2.f - HEIGHT/2.f, 1);
    Paddle paddle2(WIDTH, HEIGHT, RESOLUTION.x-WIDTH-dx, RESOLUTION.y/2.f - HEIGHT/2.f, 2);
    std::array<Paddle*, 2> paddles = {&paddle1, &paddle2};

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
        ball.move(dt);

        // ---------------------- Collision detection ----------------------
        for (Paddle* paddle : paddles) {
            paddle->move(dt);
            if (paddle->getShape().getGlobalBounds().findIntersection(ball.getShape().getGlobalBounds())) {
                if (paddle->getDirection() == "up") {
                    ball.setMovMultiplier(-ball.getMovMultiplier()[0], 1);
                } else if (paddle->getDirection() == "down") {
                    ball.setMovMultiplier(-ball.getMovMultiplier()[0], -1);
                } else {ball.setMovMultiplier(-ball.getMovMultiplier()[0], ball.getMovMultiplier()[1]);}
            }
        }


        if (ball.getPosition().y + ball.getRadius() * 2 > RESOLUTION.y ||
            ball.getPosition().y < 0){
            ball.setMovMultiplier(ball.getMovMultiplier()[0], -ball.getMovMultiplier()[1]);
        }
        // ----------------------------------------------------------------- !

        // // Adjust score
        // if (ball.getPosition().x + ball.getRadius() * 2 > RESOLUTION.x) {
        //     return 0;
        // } else if (ball.getPosition().x < 0) {
        //     return 0;
        // }

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