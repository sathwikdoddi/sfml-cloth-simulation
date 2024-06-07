#include <SFML/Graphics.hpp>
#include "include/solver.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML Cloth Simulation");
    window.setFramerateLimit(60);

    Solver solver(window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i position = sf::Mouse::getPosition(window);
            solver.renderObjects(&window, 1 / 60.0, position);
        } else solver.renderObjects(&window, 1 / 60.0);

        window.display();
    }

    return 0;
}