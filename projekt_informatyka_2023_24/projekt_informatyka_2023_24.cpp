// projekt_informatyka_2023_24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// AquaPark simulator

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
#include <SFML/System/Clock.hpp>


using namespace std;
using namespace sf;


class WaterPipe
{
    //puste
};



class Tank {
public:
    Tank() {
        // Domyślne wartości
        position = Vector2f(0.0f, 0.0f);
        size = Vector2f(100.0f, 200.0f);
        waterLevel = 0.5f; // 50%

        // Inicjalizacja zbiornika
        initializeShapes();
    }

    Tank(Vector2f position, Vector2f size, float waterLevel) : position(position), size(size), waterLevel(waterLevel) {
        initializeShapes();
    }

    void draw(RenderWindow& window) {
        window.draw(tankShape);
        window.draw(waterShape);
    }

    void updateWaterAnimation(float elapsedTime) {
        // animacja param
        static float waterSpeed = 50.0f; 
        static float waterAmplitude = 30.0f; 

        
        float waterLevelChange = waterSpeed * elapsedTime;
        waterLevel += waterLevelChange;

        
        if (waterLevel < 0.0f) {
            waterLevel = 0.0f;
            waterSpeed = -waterSpeed; //cofanie
        }
        if (waterLevel > 1.0f) {
            waterLevel = 1.0f;
            waterSpeed = -waterSpeed;
        }

        // update
        float waterHeight = size.y * waterLevel;
        float waterPositionY = position.y + size.y - waterHeight;
        waterShape.setSize(Vector2f(size.x - 10.0f, waterHeight - 10.0f));
        waterShape.setPosition(position.x + 5.0f, waterPositionY + 5.0f);
    }


private:
    Vector2f position;
    Vector2f size;
    float waterLevel;
    RectangleShape tankShape;
    RectangleShape waterShape;

    void initializeShapes() {
        // Duzy kwadrat init
        tankShape.setSize(size);
        tankShape.setPosition(position);
        tankShape.setFillColor(Color::Green); 

        // Oblicz pozycję i rozmiar
        float waterHeight = size.y * waterLevel;
        float waterPositionY = position.y + size.y - waterHeight;
        waterShape.setSize(Vector2f(size.x - 10.0f, waterHeight - 10.0f)); 
        waterShape.setPosition(position.x + 5.0f, waterPositionY + 5.0f); 
        waterShape.setFillColor(Color::Blue); //kolor wody
    }
};




class Simulation {
public:
    Simulation(RenderWindow& window) : window(window), tank1(Vector2f(200.0f, 100.0f), Vector2f(150.0f, 300.0f), 0.1f)
     {
     
        backgroundColor = Color::Blue;

        // Font
        if (!font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf")) {
            cerr << "Error loading font" << endl;
        }

        // test
        text.setFont(font);
        text.setCharacterSize(36);
        text.setString("Simulation test test");
        text.setFillColor(Color::White);

        // Tekst w srodku
        FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    }

    void run() {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                
            }

            Clock clock; // zegar init
            float elapsedTime = clock.restart().asSeconds();

            tank1.updateWaterAnimation(elapsedTime);

            // Update, render
            window.clear(backgroundColor); 
            
            tank1.draw(window); //zbiornik 1

            window.draw(text); 
            window.display(); // Symulacja
        }
    }

private:
    RenderWindow& window;
    Color backgroundColor;
    Font font;
    Text text;
    Tank tank1;
    Clock clock;

};


class Options
{
    //tu opcje
};


class Menu {
public:
    Menu(RenderWindow& window) : window(window) {
        if (!font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf")) {
            cerr << "Error loading font" << endl;
        }

        if (!background.loadFromFile("resources/Images/MenuBG.jpg")) {
            cerr << "Error loading Menu Background" << endl;
        }

        backgroundSprite.setTexture(background);
        backgroundSprite.setScale(
            static_cast<float>(window.getSize().x) / backgroundSprite.getTexture()->getSize().x,
            static_cast<float>(window.getSize().y) / backgroundSprite.getTexture()->getSize().y
        );

        backgroundRect.setSize(Vector2f(450.0f, 720.0f));
        backgroundRect.setFillColor(Color(0, 0, 0, 128));
        backgroundRect.setPosition(0.0f, 0.0f);

        title.setString("Menu");
        title.setFont(font);
        title.setCharacterSize(48);
        title.setPosition(50, 100);
        title.setFillColor(Color::Red);

        description.setString("Aquapark simulator");
        description.setFont(font);
        description.setCharacterSize(24);
        description.setPosition(50, 200);

        startButton.setString("Rozpocznij");
        startButton.setFont(font);
        startButton.setCharacterSize(36);
        startButton.setPosition(50, 300);

        optionsButton.setString("Opcje");
        optionsButton.setFont(font);
        optionsButton.setCharacterSize(36);
        optionsButton.setPosition(50, 350);

        exitOption.setString("Wyjscie");
        exitOption.setFont(font);
        exitOption.setCharacterSize(36);
        exitOption.setPosition(50, 400);
    }

    void run() {
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
                            startSimulation();
                        }
                        else if (optionsButton.getGlobalBounds().contains(mousePos)) {
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
    }

private:
    RenderWindow& window;
    Font font;
    Texture background;
    Sprite backgroundSprite;
    RectangleShape backgroundRect;
    Text title;
    Text description;
    Text startButton;
    Text optionsButton;
    Text exitOption;


    void startSimulation() {
        // czysc menu
        window.clear();
        window.display();

        Simulation simulation(window);
        simulation.run(); // start symulacji
    }
};





int main() {
    RenderWindow window(VideoMode(1280, 720), "AquaPark simulator");
    Menu menu(window);
    menu.run();

    return 0;
}