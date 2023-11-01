// projekt_informatyka_2023_24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// AquaPark simulator

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


using namespace std;
using namespace sf;


int main() {
    RenderWindow window(VideoMode(1280, 720), "AquaPark simulator");

    // Ładowanie potrzebnych plików
    Font font;
    if (!font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf")) {
        cerr << "Error loading font" << endl;
        return EXIT_FAILURE;
    }

    Texture background;
    if (!background.loadFromFile("resources/Images/MenuBG.jpg")) {
        cerr << "Error loading Menu Background" << endl;
        return EXIT_FAILURE;
    }



    // Tło w menu symulatora
    Sprite backgroundSprite(background);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundSprite.getTexture()->getSize().x,
        static_cast<float>(window.getSize().y) / backgroundSprite.getTexture()->getSize().y
    );


    // Ciemne tło pod menu
    RectangleShape backgroundRect;
    backgroundRect.setSize(sf::Vector2f(450.0f, 720.0f));
    backgroundRect.setFillColor(sf::Color(0, 0, 0, 128)); 
    backgroundRect.setPosition(0.0f, 0.0f);


    // title tekst
    Text title("Menu", font, 48);
    title.setPosition(50, 100);
    title.setFillColor(Color::Red);

    // Opis
    Text description("Aquapark simulator ", font, 24);
    description.setPosition(50, 200);

    // Przycisk start
    Text startButton("Rozpocznij", font, 36);
    startButton.setPosition(50, 300);

    // Przycisk opcje
    Text optionsButton("Opcje", font, 36);
    optionsButton.setPosition(50, 350);

    // Przycisk exit
    Text exitOption("Wyjscie", font, 36);
    exitOption.setPosition(50, 400);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                    if (startButton.getGlobalBounds().contains(mousePos)) {
                        cout << "Przycisk start" << endl;
                    }
                    else if (optionsButton.getGlobalBounds().contains(mousePos))
                    {
                        cout << "Przycisk opcje" << endl;
                    }
                    else if (exitOption.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(backgroundRect);
        window.draw(title);
        window.draw(description);
        window.draw(startButton);
        window.draw(optionsButton);
        window.draw(exitOption);
        window.display();
    }

    return 0;
}
