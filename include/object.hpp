#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

typedef sf::Vector2f Vec2;

struct VerletObject {
    sf::CircleShape obj;
    sf::Clock timer;
    bool immovable;
    Vec2 position_last;
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    VerletObject(Vec2 pos = Vec2(500,500), bool imm = false) {
        timer.restart();
        acceleration = Vec2(0, 0);

        obj.setFillColor(sf::Color::White);
        obj.setRadius(3);
        obj.setOrigin(obj.getRadius(), obj.getRadius());
        obj.setOutlineThickness(2);
        obj.setOutlineColor(sf::Color::Black);
        obj.setPointCount(30);

        immovable = imm;

        obj.setPosition(pos);

        position =  pos;
        position_last = pos;
    }

    void update(float dt) {
        velocity = position - position_last;
        position_last = position;
        position = position + velocity + acceleration * dt * dt;

        acceleration = Vec2();
        obj.setPosition(position);
    }

    void accelerate(Vec2 a) {
        acceleration += a;
    }
};