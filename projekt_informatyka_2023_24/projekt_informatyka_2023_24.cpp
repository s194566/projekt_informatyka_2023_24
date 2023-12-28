/*

AquaPark simulator


Temat 2

Wizualizacja procesu przemysłowego napełnianie basenów w aquaparku oraz podgrzewania wody w tych basenach.

1. Animacja kierunku przepływu medium w rurach, animacja poziomu wody, animacja podgrzewania/ oziębiania wody, animacja pracy pompy.
2. 4 zbiorniki i 3 rury.
3. jedna rura prosta / jedna rura zakrzywiona 90 stopni / jeden trójnik
4. Zadawanie wartości początkowych w zakładce opcje
5. Automatyczny process anmiacja


zakres minimalny

01. 4 zbiorniki (baseny)
    a) W symulacji napis symulacja aktywna / symulacja zakończona
    b) 3 rury z wodą o nieregularnym kształcie
02. Stały element graficzny tło (zdjęcie basenów w menu głównym)
03. Sterowanie pompy on/off przez myszkę
04. Wybór sceneri kolory basenów.
05. Po wciśnięcie F1 przechodzimy do strony HELP
06. Kliknięcie close lub escape
07. Popup czy na pewno chcesz wyjść
08. Program zapisuje informacje o wartościach zadanych w pliku txt w folderze resources/saved_params/
09. Struktury danych w kodzie gdzieś są.
10.
11. Program wykorzystuje techniki programowania obiektowego.
12. Graficzne menu


*/

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
#include <SFML/System/Clock.hpp>
//#include <list>
#include <array>
#include <fstream>


using namespace std;
using namespace sf;

class WaterPump {
public:
    WaterPump() {
        init();
    }

    void setPosition(const Vector2f& newPosition) {
        sprite.setPosition(newPosition);
    }

    void setSize(const Vector2f& newSize) {
        sprite.setScale(newSize.x / texture.getSize().x, newSize.y / texture.getSize().y);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

private:
    Texture texture; 
    Sprite sprite;   

    void init() {
        if (!texture.loadFromFile("resources/Images/water-pump.png")) {
            cerr << "Error loading water pump texture" << endl;
        }

        sprite.setTexture(texture);
    }
};

class HeatingMachine {
public:
    HeatingMachine() {
        init();
    }

    void setPosition(const Vector2f& newPosition) {
        sprite.setPosition(newPosition);
    }

    void setSize(const Vector2f& newSize) {
        sprite.setScale(newSize.x / texture.getSize().x, newSize.y / texture.getSize().y);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

private:
    sf::Texture texture; 
    sf::Sprite sprite;   

    void init() {
        if (!texture.loadFromFile("resources/Images/heating-machine.png")) {
            cerr << "Error loading water pump texture" << endl;
        }

        sprite.setTexture(texture);

    }
};

class WaterPipe {
public:
    WaterPipe() {
        init();
    }

    void draw(RenderWindow& window) {
        window.draw(pipeShape);
        window.draw(waterShape);
    }

    bool updateWaterAnimation(float elapsedTime, float waterSpeed) {

        float targetWaterLevel = static_cast<float>(100) / 100.0f;

        float levelDifference = targetWaterLevel - waterLevel;

        float stepSize = waterSpeed * elapsedTime;

        if (abs(levelDifference) > stepSize) {
            waterLevel += (levelDifference > 0.0f) ? stepSize : -stepSize;
        }
        else {
            waterLevel = targetWaterLevel;
            waterSpeed = 0.0f;
            return true;
        }

        float waterHeight = waterPipeSize.y * waterLevel;
        float waterPositionY = waterPipePosition.y + waterPipeSize.y - waterHeight;
        waterShape.setSize(Vector2f(waterPipeSize.x - 0.0f, waterHeight - 0.0f));
        waterShape.setPosition(waterPipePosition.x + 0.0f, waterPositionY + 0.0f);

        return false;
    }

    void setPipePosition(Vector2f position)
    {
        pipeShape.setPosition(position);
    }

    void setWaterPipePosition(Vector2f position)
    {
        waterPipePosition = position;
        updateWaterPosition();
    }

    void setPipeSize(Vector2f size) {
        pipeSize = size;
        pipeShape.setSize(pipeSize);
    }

    void setWaterPipeSize(Vector2f size)
    {
        waterPipeSize = size;
        updateWaterPosition();
    }

    void setPipeColor(Color pipeColor)
    {
        pipeShape.setFillColor(pipeColor);
    }

    void setWaterColor(Color waterColor)
    {
        waterShape.setFillColor(waterColor);
    }

    void setWaterLevel(float NewWaterLevel)
    {
        waterLevel = NewWaterLevel;
    }

private:
    Vector2f pipePosition;
    Vector2f waterPipePosition;
    Vector2f pipeSize;
    Vector2f waterPipeSize;
    float waterLevel;
    RectangleShape pipeShape;
    RectangleShape waterShape;

    void init() {
        //pipeShape.setSize(pipeSize);
        //pipeShape.setPosition(pipePosition);
        //pipeShape.setFillColor(Color::Green);
    }

    void updateWaterPosition() {
        float waterHeight = waterPipeSize.y * waterLevel;
        float waterPositionY = waterPipePosition.y + waterPipeSize.y - waterHeight;
        waterShape.setSize(Vector2f(waterPipeSize.x - 10.0f, waterHeight - 0.0f));
        waterShape.setPosition(waterPipePosition.x + 5.0f, waterPositionY + 5.0f);
    }
};

class Tank {
public:
    Tank() {
        init();
    }

    void draw(RenderWindow& window)
    {
        window.draw(tankShape);
        window.draw(waterShape);
    }

    bool updateWaterAnimation(float elapsedTime, int targetPercentage, float waterSpeed) {
        
        float targetWaterLevel = static_cast<float>(targetPercentage) / 100.0f;

        float levelDifference = targetWaterLevel - waterLevel;

        float stepSize = waterSpeed * elapsedTime;

        if (abs(levelDifference) > stepSize) {
            waterLevel += (levelDifference > 0.0f) ? stepSize : -stepSize;
        }
        else {
            waterLevel = targetWaterLevel;
            waterSpeed = 0.0f;
            return true;
        }

        // Update
        float waterHeight = size.y * waterLevel;
        float waterPositionY = position.y + size.y - waterHeight;
        waterShape.setSize(sf::Vector2f(size.x - 10.0f, waterHeight - 0.0f));
        waterShape.setPosition(position.x + 5.0f, waterPositionY - 5.0f);

        return false;
    }

    void setPosition(Vector2f newPosition) {
        position = newPosition;
        tankShape.setPosition(position);
        updateWaterPosition();
    }

    void setSize(Vector2f newSize) {
        size = newSize;
        tankShape.setSize(size);
        updateWaterPosition();
    }

    void setWaterLevel(float newWaterLevel) {
        waterLevel = newWaterLevel;
        updateWaterPosition();
    }

private:
    Vector2f position = { 0.0f, 0.0f };
    Vector2f size = { 100.0f, 100.0f };
    float waterLevel = { 0.5f };
    RectangleShape tankShape;
    RectangleShape waterShape;

    void init() {
        tankShape.setSize(size);
        tankShape.setPosition(position);
        tankShape.setFillColor(Color::Green);

        float waterHeight = size.y * waterLevel;
        float waterPositionY = position.y + size.y - waterHeight;
        waterShape.setSize(Vector2f(size.x - 10.0f, waterHeight - 0.0f));
        waterShape.setPosition(position.x + 5.0f, waterPositionY + 5.0f);
        waterShape.setFillColor(Color::Blue);

    }

    void updateWaterPosition() {
        float waterHeight = size.y * waterLevel;
        float waterPositionY = position.y + size.y - waterHeight;
        waterShape.setSize(Vector2f(size.x - 10.0f, waterHeight - 0.0f));
        waterShape.setPosition(position.x + 5.0f, waterPositionY + 5.0f);
    }
};

class InputData {
public:
    InputData() {
        init();
    }

    void draw(RenderWindow& window) {
        window.draw(label);
        window.draw(valueText);
        window.draw(incrementButton);
        window.draw(decrementButton);
    }

    void handleButtonClick(const Vector2f& mousePos) {
        if (incrementButton.getGlobalBounds().contains(mousePos)) {
            increaseValue();
        }
        else if (decrementButton.getGlobalBounds().contains(mousePos)) {
            decreaseValue();
        }
    }

    int getValue() const {
        return value;
    }

    void setValue(int newValue) {
        value = newValue;
        updateValueText();
    }

    void setString(const string& newString) {
        labelText = newString;
        label.setString(labelText);
    }

    void setPosition(Vector2f newPosition) {
        position = newPosition;
        label.setPosition(position.x, position.y);
        valueText.setPosition(position.x + 400, position.y);
        incrementButton.setPosition(position.x + 500, position.y);
        decrementButton.setPosition(position.x + 600, position.y);
    }

private:
    string labelText;
    int value;
    Vector2f position;
    Font font;
    Text label;
    Text valueText;
    Text incrementButton;
    Text decrementButton;

    void init() {
        font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf");

        label.setFont(font);
        label.setCharacterSize(24);
        label.setPosition(position.x, position.y);
        label.setString(labelText);

        valueText.setFont(font);
        valueText.setCharacterSize(24);
        updateValueText();
        valueText.setPosition(position.x + 400, position.y);

        incrementButton.setFont(font);
        incrementButton.setCharacterSize(20);
        incrementButton.setString("Up");
        incrementButton.setPosition(position.x + 500, position.y);

        decrementButton.setFont(font);
        decrementButton.setCharacterSize(20);
        decrementButton.setString("Down");
        decrementButton.setPosition(position.x + 600, position.y);
    }

    void increaseValue() {
        value++;
        updateValueText();
    }

    void decreaseValue() {
        value--;
        updateValueText();
    }

    void updateValueText() {
        valueText.setString(to_string(value));
    }
};

class Parameters {
public:
    Parameters( const Vector2f& position, RenderWindow& window)
        : position(position), window(window) {
        init();
    }

    void draw(int data[]) {
        for (size_t i = 0; i < fixedLabels.size(); ++i) {
            const string labelText = fixedLabels[i] + " ";
            drawParameter(labelText, data[i], i);
        }
    }

    void loadDataFromFileToSimulation(int data[], const string& filename) {

        ifstream fin(filename);

        if (fin.is_open()) {
            for (int i = 0; i < 8; i++) {
                fin >> data[i];
            }

            fin.close();
            cout << "Data loaded from file." << endl;
        }
        else {
            cout << "Unable to open file for loading." << endl;
        }
    }

    void loadParametersDataInto(int data[], const string& filename) {
        ifstream fin(filename);

        if (fin.is_open()) {
            for (int i = 0; i < 8; i++) {
                fin >> data[i];
            }

            fin.close();
            cout << "Data loaded into array." << endl;
        }
        else {
            cout << "Unable to open file for loading." << endl;
        }
    }

private:
    
    Vector2f position; 
    Font font;
    RenderWindow& window;

    const array<string, 8> fixedLabels = {
        "Poziom wody 1:",
        "Temperatura wody 1:",
        "Poziom wody 2:",
        "Temperatura wody 2:",
        "Poziom wody 3:",
        "Temperatura wody 3:",
        "Poziom wody 4:",
        "Temperatura wody 4:"
    };

    void init() {
        if (!font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf")) {
            cerr << "Error loading font" << endl;
        }
    }

    void drawParameter(const string& labelText, int value, size_t index) {
        RectangleShape backgroundRect(Vector2f(300.0f, 40.0f)); // rozmiar pola
        backgroundRect.setFillColor(Color(192, 192, 192, 200)); // kolor pola
        backgroundRect.setPosition(position.x, position.y + index * 50.0f); // pozycja

        Text parameterText;
        parameterText.setFont(font);
        parameterText.setCharacterSize(20); // font
        parameterText.setPosition(position.x + 5.0f, position.y + index * 50.0f + 5.0f); 
        parameterText.setString(labelText + to_string(value)); 

        window.draw(backgroundRect);
        window.draw(parameterText);
    }

    

};

class Popup {
public:
    Popup(Vector2f position) : position(position), size(Vector2f(400.0f, 200.0f)) {
        init();
    }

    void drawYesNo(RenderWindow& window) {
        window.draw(backgroundRect);
        window.draw(messageText);
        window.draw(yesButtonSprite);
        window.draw(noButtonSprite);
    }

    void drawRestart(RenderWindow& window) {
        window.draw(backgroundRect);
        window.draw(messageText);
        window.draw(restartButtonSprite);
    }

    bool showYesNoPopup(RenderWindow& window) {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                        if (yesButtonSprite.getGlobalBounds().contains(mousePos)) {
                            return true;
                        }
                        else if (noButtonSprite.getGlobalBounds().contains(mousePos)) {
                            return false;
                        }
                    }
                }
            }

            drawYesNo(window);
            window.display();
        }

        return false; // Handle window closure
    }

    bool showRestartPopup(sf::RenderWindow& window) {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                        if (yesButtonSprite.getGlobalBounds().contains(mousePos)) {
                            return true;
                        }
                        else if (noButtonSprite.getGlobalBounds().contains(mousePos)) {
                            return false;
                        }
                    }
                }
            }

            drawRestart(window);
            window.display();
        }

        return false;
    }

private:
    Font font;
    Vector2f position;
    Vector2f size;
    float waterLevel;
    RectangleShape backgroundRect;
    Text messageText;
    Sprite yesButtonSprite;
    Sprite noButtonSprite;
    Sprite restartButtonSprite;
    Texture yesTexture;
    Texture noTexture;
    Texture restartTexture;

    void init() {
        backgroundRect.setSize(size);
        backgroundRect.setFillColor(Color(255, 255, 255, 200));
        backgroundRect.setPosition(position);

        if (!font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf")) {
            cerr << "Error loading font." << endl;
        }

        messageText.setString("Do you want to leave?");
        messageText.setFont(font);
        messageText.setCharacterSize(24); 
        messageText.setFillColor(Color::Black);

        
        float textPosX = position.x + (size.x - messageText.getGlobalBounds().width) / 2;
        float textPosY = position.y + ((size.y - messageText.getGlobalBounds().height) / 2) - 50.0f;
        messageText.setPosition(textPosX, textPosY);

        
        if (!yesTexture.loadFromFile("resources/Images/yes-button.png") ||
            !noTexture.loadFromFile("resources/Images/no-button.png") ||
            !restartTexture.loadFromFile("resources/Images/reset-button.jpg"))
        {
            cerr << "Error loading button textures." << endl;

        }

        yesButtonSprite.setTexture(yesTexture);
        yesButtonSprite.setPosition(position.x + 100.0f, position.y + size.y - 100.0f);
        yesButtonSprite.setScale(100.0f / yesButtonSprite.getLocalBounds().width, 100.0f / yesButtonSprite.getLocalBounds().height);

        noButtonSprite.setTexture(noTexture);
        noButtonSprite.setPosition(position.x + 200.0f, position.y + size.y - 100.0f);
        noButtonSprite.setScale(100.0f / noButtonSprite.getLocalBounds().width, 100.0f / noButtonSprite.getLocalBounds().height);

        restartButtonSprite.setTexture(restartTexture);
        restartButtonSprite.setPosition(position.x + 150.0f, position.y + size.y - 100.0f);
        restartButtonSprite.setScale(100.0f / yesButtonSprite.getLocalBounds().width, 100.0f / yesButtonSprite.getLocalBounds().height);
    }
};

class Simulation {
public:
    Simulation(RenderWindow& window)
        : window(window),
        parameters(Vector2f(10.0f, 60.0f), window),
        exitPopup(Vector2f(440.0f, 260.0f)),
        SimulationEndPopup(Vector2f(440.0f, 260.0f))
    {
        init();
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    cout << "Przycisk close" << endl;
                    if (exitPopup.showYesNoPopup(window)) {
                        window.close();
                    }
                }
                else if (event.type == sf::Event::MouseButtonReleased) {
                    handleButtonClick(event);
                }
            }
            

            Clock clock;
            float elapsedTime = clock.restart().asSeconds();


            bool animationPipe1Completed = false;
            bool animationPipe2Completed = false;
            bool animationPipe3Completed = false;
            bool animationPipe4Completed = false;
            bool animationPipe5Completed = false;
            bool animationPipe6Completed = false;
            bool animationPipe7Completed = false;

            bool tankAnimation1Completed = false;
            bool tankAnimation2Completed = false;
            bool tankAnimation3Completed = false;
            bool tankAnimation4Completed = false;


            // Path 1
            if (!animationPipe2Completed) {
                animationPipe2Completed = pipe2.updateWaterAnimation(elapsedTime, 3000);
            }

            if (animationPipe2Completed && !animationPipe1Completed) {
                animationPipe1Completed = pipe1.updateWaterAnimation(elapsedTime, 3000);
            }

            if (animationPipe1Completed) {
                tankAnimation1Completed = tank1.updateWaterAnimation(elapsedTime, data_list[6], 100);
            }

            // Path 2
            if (!animationPipe6Completed) {
                animationPipe6Completed = pipe6.updateWaterAnimation(elapsedTime, 3000);
            }

            if (animationPipe6Completed) {
                animationPipe5Completed = pipe5.updateWaterAnimation(elapsedTime, 3000);
            }

            if (animationPipe5Completed) {
                animationPipe3Completed = pipe3.updateWaterAnimation(elapsedTime, 3000);
                animationPipe4Completed = pipe4.updateWaterAnimation(elapsedTime, 3000);
            }

            if (animationPipe3Completed && animationPipe4Completed) {
                tankAnimation2Completed = tank2.updateWaterAnimation(elapsedTime, data_list[6], 100);
                tankAnimation3Completed = tank3.updateWaterAnimation(elapsedTime, data_list[6], 100);
            }

            // Path 3
            if (!animationPipe7Completed) {
                animationPipe7Completed = pipe7.updateWaterAnimation(elapsedTime, 3000);
            }
            if (animationPipe7Completed) {
                tankAnimation4Completed = tank4.updateWaterAnimation(elapsedTime, data_list[6], 100);
            }

            if (tankAnimation1Completed && tankAnimation2Completed && tankAnimation3Completed && tankAnimation4Completed) {
                cout << "All animations completed!" << endl;
                SimulationEndPopup.showRestartPopup(window);
                break;
            }



            window.clear(backgroundColor);

            drawTopStrip();

            parameters.draw(data_list);

            //put parameters data into array

            tank1.draw(window);
            tank2.draw(window);
            tank3.draw(window);
            tank4.draw(window);
            base.draw(window);

            // Zamina kolejnosci do poprawnego nakładania warstw

            pipe2.draw(window);
            pipe1.draw(window);
            
            pipe5.draw(window);
            pipe3.draw(window);
            pipe4.draw(window);
            pipe6.draw(window);

            pipe7.draw(window);
          
            pump1.draw(window);
            //pump2.draw(window);
            //pump3.draw(window);

            //HM1.draw(window);
            //HM2.draw(window);
            //HM3.draw(window);
            

            window.display();

            if (simulationShouldClose) {
                return;
            }
        }
    }

private:
    RenderWindow& window;
    Color backgroundColor;
    Font font;
    Text simulationStatus;
    Text TitleText;
    Text backButton;

    Tank tank1;
    Tank tank2;
    Tank tank3;
    Tank tank4;
    Tank base;

    WaterPipe pipe1;
    WaterPipe pipe2;
    WaterPipe pipe3;
    WaterPipe pipe4;
    WaterPipe pipe5;
    WaterPipe pipe6;
    WaterPipe pipe7;

    WaterPump pump1;
    WaterPump pump2;
    WaterPump pump3;

    HeatingMachine HM1;
    HeatingMachine HM2;
    HeatingMachine HM3;


    

    Popup exitPopup;
    Popup SimulationEndPopup;
    
    Parameters parameters;

    RectangleShape strip;

    bool simulationShouldClose = false;

    int data_list[8] = { 0 };


 

    void init() {
        if (!font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf")) {
            cerr << "Error loading font" << endl;
        }

        backgroundColor = Color(0, 0, 255);


        // Create a Tank

        base.setPosition(Vector2f(650.0f, 650.0f));
        base.setSize(Vector2f(600.0f, 60.0f));
        base.setWaterLevel(1);
        
        tank1.setPosition(Vector2f(350.0f, 150.0f));
        tank1.setSize(Vector2f(100.0f, 250.0f));
        tank1.setWaterLevel(0);

        tank2.setPosition(Vector2f(500.0f, 200.0f));
        tank2.setSize(Vector2f(200.0f, 150.0f));
        tank2.setWaterLevel(0);

        tank3.setPosition(Vector2f(750.0f, 200.0f));
        tank3.setSize(Vector2f(200.0f, 150.0f));
        tank3.setWaterLevel(0);

        tank4.setPosition(Vector2f(1000.0f, 100.0f));
        tank4.setSize(Vector2f(200.0f, 200.0f));
        tank4.setWaterLevel(0);

        // create WaterPipe

        pipe1.setPipePosition(Vector2f(380.0f, 395.0f));
        pipe1.setPipeSize(Vector2f(40.0f, 265.0f));
        pipe1.setPipeColor(Color::Red);
        pipe1.setWaterPipePosition(Vector2f(390.0f, 395.0f));
        pipe1.setWaterPipeSize(Vector2f(20.0f, 275.0f));
        pipe1.setWaterColor(Color::Green);
        pipe1.setWaterLevel(0);

        pipe2.setPipePosition(Vector2f(380.0f, 660.0f));
        pipe2.setPipeSize(Vector2f(275.0f, 40.0f));
        pipe2.setPipeColor(Color::Red);
        pipe2.setWaterPipePosition(Vector2f(390.0f, 670.0f));
        pipe2.setWaterPipeSize(Vector2f(265.0f, 20.0f));
        pipe2.setWaterColor(Color::Black);
        pipe2.setWaterLevel(0);

        pipe3.setPipePosition(Vector2f(580.0f, 345.0f));
        pipe3.setPipeSize(Vector2f(40.0f, 150.0f));
        pipe3.setPipeColor(Color::Red);
        pipe3.setWaterPipePosition(Vector2f(590.0f, 345.0f));
        pipe3.setWaterPipeSize(Vector2f(20.0f, 160.0f));
        pipe3.setWaterColor(Color::Black);
        pipe3.setWaterLevel(0);

        pipe4.setPipePosition(Vector2f(830.0f, 345.0f));
        pipe4.setPipeSize(Vector2f(40.0f, 150.0f));
        pipe4.setPipeColor(Color::Red);
        pipe4.setWaterPipePosition(Vector2f(840.0f, 345.0f));
        pipe4.setWaterPipeSize(Vector2f(20.0f, 160.0f));
        pipe4.setWaterColor(Color::Black);
        pipe4.setWaterLevel(0);

        pipe5.setPipePosition(Vector2f(580.0f, 495.0f));
        pipe5.setPipeSize(Vector2f(290.0f, 40.0f));
        pipe5.setPipeColor(Color::Red);
        pipe5.setWaterPipePosition(Vector2f(590.0f, 505.0f));
        pipe5.setWaterPipeSize(Vector2f(270.0f, 20.0f));
        pipe5.setWaterColor(Color::Black);
        pipe5.setWaterLevel(0);

        pipe6.setPipePosition(Vector2f(705.0f, 535.0f));
        pipe6.setPipeSize(Vector2f(40.0f, 120.0f));
        pipe6.setPipeColor(Color::Red);
        pipe6.setWaterPipePosition(Vector2f(715.0f, 525.0f));
        pipe6.setWaterPipeSize(Vector2f(20.0f, 130.0f));
        pipe6.setWaterColor(Color::Black);
        pipe6.setWaterLevel(0);

        pipe7.setPipePosition(Vector2f(1080.0f, 295.0f));
        pipe7.setPipeSize(Vector2f(40.0f, 360.0f));
        pipe7.setPipeColor(Color::Red);
        pipe7.setWaterPipePosition(Vector2f(1090.0f, 295.0f));
        pipe7.setWaterPipeSize(Vector2f(20.0f, 360.0f));
        pipe7.setWaterColor(Color::Black);
        pipe7.setWaterLevel(0);

        // create a pump

        pump1.setPosition(Vector2f(605.0f, 600.0f));
        pump1.setSize(Vector2f(100.0f, 100.0f));

        pump2.setPosition(Vector2f(600.0f, 400.0f));
        pump2.setSize(Vector2f(200.0f, 200.0f));

        pump3.setPosition(Vector2f(800.0f, 400.0f));
        pump3.setSize(Vector2f(200.0f, 200.0f));

        // create a heat machine

        HM1.setPosition(Vector2f(500.0f, 500.0f));
        HM1.setSize(Vector2f(200.0f, 200.0f));

        HM2.setPosition(Vector2f(600.0f, 500.0f));
        HM2.setSize(Vector2f(200.0f, 200.0f));

        HM3.setPosition(Vector2f(700.0f, 500.0f));
        HM3.setSize(Vector2f(200.0f, 200.0f));
        
        // Other

        parameters.loadDataFromFileToSimulation(data_list, "resources/saved_params/params.txt");
     
        logParams(data_list);
        
        createTopStrip();

    }

    void handleButtonClick(const Event& event) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

        if (backButton.getGlobalBounds().contains(mousePos)) {
            window.clear();
            window.display();
            simulationShouldClose = true;
        }
    }

    void logParams(int list[8]) const
    {
        for (int i = 0; i < 8; i++) {
            cout << data_list[i] << " ";
        }

        cout << endl;
    }

    void createTopStrip()
    {

        strip.setSize(Vector2f(window.getSize().x, 50.0f));
        strip.setFillColor(Color::Black);  
        strip.setPosition(0.0f, 0.0f);

        TitleText.setFont(font);
        TitleText.setCharacterSize(24);
        TitleText.setFillColor(Color::White);
        TitleText.setPosition(((window.getSize().x)/2)-75.0f, 10.0f);
        TitleText.setString("Symulacja");

        backButton.setFont(font);
        backButton.setCharacterSize(24);
        backButton.setString("cofnij");
        backButton.setPosition(1180.0, 10.0);

        simulationStatus.setFont(font);
        simulationStatus.setCharacterSize(24);
        simulationStatus.setString("Status symulacji Run/Stop");
        simulationStatus.setPosition(10.0, 10.0);

    }

    void drawTopStrip()
    {
        window.draw(strip);
        window.draw(simulationStatus);
        window.draw(TitleText);
        window.draw(backButton);
    }


};

class Options {
public:
    Options(RenderWindow& window)
        : window(window), 
        parameters(Vector2f(10.0f, 10.0f), window),
        exitPopup(Vector2f(440.0f, 260.0f))

    {
        loadDataFromFile("resources/saved_params/params.txt");
        init();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            

            window.clear();

            window.draw(backgroundSprite);
            window.draw(backgroundRect);

            const array<string, 8> fixedLabels = {
            "Poziom wody 1:",
            "Temperatura wody 1:",
            "Poziom wody 2:",
            "Temperatura wody 2:",
            "Poziom wody 3:",
            "Temperatura wody 3:",
            "Poziom wody 4:",
            "Temperatura wody 4:"
            };


            waterLevelInput1.setString(fixedLabels[0]);
            temperatureInput1.setString(fixedLabels[1]);
            waterLevelInput2.setString(fixedLabels[2]);
            temperatureInput2.setString(fixedLabels[3]);
            waterLevelInput3.setString(fixedLabels[4]);
            temperatureInput3.setString(fixedLabels[5]);
            waterLevelInput4.setString(fixedLabels[6]);
            temperatureInput4.setString(fixedLabels[7]);

            waterLevelInput1.setPosition(Vector2f(100.0f, 150.0f));
            temperatureInput1.setPosition(Vector2f(100.0f, 190.0f));
            waterLevelInput2.setPosition(Vector2f(100.0f, 230.0f));
            temperatureInput2.setPosition(Vector2f(100.0f, 270.0f));
            waterLevelInput3.setPosition(Vector2f(100.0f, 310.0f));
            temperatureInput3.setPosition(Vector2f(100.0f, 350.0f));
            waterLevelInput4.setPosition(Vector2f(100.0f, 390.0f));
            temperatureInput4.setPosition(Vector2f(100.0f, 430.0f));


            waterLevelInput1.draw(window);
            temperatureInput1.draw(window);
            waterLevelInput2.draw(window);
            temperatureInput2.draw(window);
            waterLevelInput3.draw(window);
            temperatureInput3.draw(window);
            waterLevelInput4.draw(window);
            temperatureInput4.draw(window);

            window.draw(saveButton);
            window.draw(backButton);

            window.display();

            if (optionsShouldClose) {
                return;  
            }
        }
    }

    void loadDataFromFile(const string& filePath) {
        ifstream inputFile(filePath);

        if (inputFile.is_open()) {
            for (int i = 0; i < 8; ++i) {
                if (!(inputFile >> data_list[i])) {
                    cerr << "Error reading data from file: " << filePath << endl;
                    data_list[i] = 0;
                }
            }

            waterLevelInput1.setValue(data_list[0]);
            temperatureInput1.setValue(data_list[1]);
            waterLevelInput2.setValue(data_list[2]);
            temperatureInput2.setValue(data_list[3]);
            waterLevelInput3.setValue(data_list[4]);
            temperatureInput3.setValue(data_list[5]);
            waterLevelInput4.setValue(data_list[6]);
            temperatureInput4.setValue(data_list[7]);

            inputFile.close();
        }
        else {
            cerr << "Error opening file for reading: " << filePath << endl;
        }
    }

private:
    RenderWindow& window;
    Font font;
    Texture background;
    Sprite backgroundSprite;
    RectangleShape backgroundRect;

    Popup exitPopup;

    array<int, 8> enteredValues; 

    int data_list[8] = { 0 };
    Parameters parameters;

    InputData waterLevelInput1;
    InputData temperatureInput1;
    InputData waterLevelInput2;
    InputData temperatureInput2;
    InputData waterLevelInput3;
    InputData temperatureInput3;
    InputData waterLevelInput4;
    InputData temperatureInput4;

    Text saveButton;
    Text backButton;

    int id = 0;
    bool optionsShouldClose = false;

    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                cout << "Przycisk close" << endl;
                if (exitPopup.showYesNoPopup(window)) {
                    window.close();
                }
            }
            else if (event.type == Event::MouseButtonReleased) {
                handleButtonClick(event);
            }
        }
    }

    

    void handleButtonClick(const Event& event) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

        waterLevelInput1.handleButtonClick(mousePos);
        temperatureInput1.handleButtonClick(mousePos);
        waterLevelInput2.handleButtonClick(mousePos);
        temperatureInput2.handleButtonClick(mousePos);
        waterLevelInput3.handleButtonClick(mousePos);
        temperatureInput3.handleButtonClick(mousePos);
        waterLevelInput4.handleButtonClick(mousePos);
        temperatureInput4.handleButtonClick(mousePos);

        enteredValues[0] = waterLevelInput1.getValue();
        enteredValues[1] = temperatureInput1.getValue();
        enteredValues[2] = waterLevelInput2.getValue();
        enteredValues[3] = temperatureInput2.getValue();
        enteredValues[4] = waterLevelInput3.getValue();
        enteredValues[5] = temperatureInput3.getValue();
        enteredValues[6] = waterLevelInput4.getValue();
        enteredValues[7] = temperatureInput4.getValue();

        if (saveButton.getGlobalBounds().contains(mousePos)) {
            saveParams(enteredValues);
            window.clear();
            window.display();
            optionsShouldClose = true;
        }

        if (backButton.getGlobalBounds().contains(mousePos)) {
            window.clear();
            window.display();
            optionsShouldClose = true;
        }
    }

    void saveParams(const array<int, 8>& values) {
        cout << "Zapis dane:" << id << endl;

        // Drukowanie do konsoli
        for (const auto& entry : values) {
            cout << entry << " ";
        }

        // Dodanie wartosci do pliku txt
        ofstream outputFile("resources/saved_params/params.txt");

        if (outputFile.is_open()) {
            for (const auto& entry : values) {
                outputFile << entry << " ";
            }
            outputFile << endl;

            outputFile.close();
            cout << "Dane zapisane do pliku." << endl;
        }
        else {
            cerr << "Error opening file for writing: resources/saved_params/params.txt" << endl;
        }

        id += 1;
    }

    void init() {
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

        backgroundRect.setSize(Vector2f(1280.0f, 720.0f));
        backgroundRect.setFillColor(Color(0, 0, 0, 128));
        backgroundRect.setPosition(0.0f, 0.0f);

        saveButton.setFont(font);
        saveButton.setCharacterSize(36);
        saveButton.setString("Zapisz");
        saveButton.setPosition(300.0, 75.0);

        backButton.setFont(font);
        backButton.setCharacterSize(36);
        backButton.setString("cofnij");
        backButton.setPosition(900.0, 75.0);

    }
};

class Help {
public:
    Help(RenderWindow& window)
        :   window(window),
            exitPopup(Vector2f(440.0f, 260.0f)) {
        init();
    }

    void run() {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    cout << "Przycisk close" << endl;
                    if (exitPopup.showYesNoPopup(window)) {
                        window.close();
                    }
                }
                else if (event.type == Event::MouseButtonReleased) {
                    handleButtonClick(event);
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            window.draw(backgroundRect);
            window.draw(helpText);
            window.draw(backButton);
            window.display();

            if (helpShouldClose) {
                return;
            }

        }
    }

private:
    RenderWindow& window;
    Font font;
    Texture background;
    Sprite backgroundSprite;
    RectangleShape backgroundRect;
    Text helpText;
    Text backButton;

    Popup exitPopup;

    bool helpShouldClose = false;


    void init() {
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

        helpText.setString("Tu bedzie opis calego\n projektu oraz pomoc");
        helpText.setFont(font);
        helpText.setCharacterSize(24);
        helpText.setPosition(50, 200);

        backButton.setString("Cofnij");
        backButton.setFont(font);
        backButton.setCharacterSize(36);
        backButton.setPosition(50, 500);
    }

    void handleButtonClick(const Event& event) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));


        if (backButton.getGlobalBounds().contains(mousePos)) {
            window.clear();
            window.display();
            helpShouldClose = true;
        }

    }
};

class Menu {
public:
    Menu(RenderWindow& window) : window(window), exitPopup(Vector2f(440.0f, 260.0f)) {
        init();
    }

    void run() {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    // Display the exit popup
                    cout << "Przycisk close" << endl;
                    if (exitPopup.showYesNoPopup(window)) {
                        window.close();
                    }
                }
                else if (event.type == Event::MouseButtonReleased) {
                    if (event.mouseButton.button == Mouse::Left) {
                        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                        if (startButton.getGlobalBounds().contains(mousePos)) {
                            cout << "Przycisk start" << endl;
                            startSimulation();
                        }
                        else if (paramsButton.getGlobalBounds().contains(mousePos)) {
                            cout << "Przycisk Params" << endl;
                            startOptions();
                        }
                        else if (helpButton.getGlobalBounds().contains(mousePos)) {
                            cout << "Przycisk Help" << endl;
                            startHelp();
                        }
                        else if (exitOption.getGlobalBounds().contains(mousePos)) {
                            cout << "Przycisk wyjscie" << endl;
                            if (exitPopup.showYesNoPopup(window)) {
                                window.close();
                            }
                        }
                    }
                }
            }

            if (currentScreenState == ScreenState::Main) {
                window.clear();
                window.draw(backgroundSprite);
                window.draw(backgroundRect);
                window.draw(title);
                window.draw(description);
                window.draw(startButton);
                window.draw(paramsButton);
                window.draw(helpButton);
                window.draw(exitOption);
                window.display();
            }
            else if (currentScreenState == ScreenState::Options) {
                Options options(window);
                options.run();
                currentScreenState = ScreenState::Main;
            }
            else if (currentScreenState == ScreenState::Help) {
                // Handle the "Help" state
            }
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
    Text paramsButton;
    Text helpButton;
    Text exitOption;

    Popup exitPopup;

    enum class ScreenState {
        Main,
        Options,
        Help
    };

    ScreenState currentScreenState;


    void startOptions() {
        window.clear();
        Options options(window);
        options.run();
    }

    void startSimulation() {
        window.clear();
        Simulation simulation(window);
        simulation.run();
    }

    void startHelp() {
        window.clear();
        Help help(window);
        help.run();

    }


    void init()
    {
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

        paramsButton.setString("Parametry");
        paramsButton.setFont(font);
        paramsButton.setCharacterSize(36);
        paramsButton.setPosition(50, 350);

        helpButton.setString("Pomoc");
        helpButton.setFont(font);
        helpButton.setCharacterSize(36);
        helpButton.setPosition(50, 400);

        exitOption.setString("Wyjscie");
        exitOption.setFont(font);
        exitOption.setCharacterSize(36);
        exitOption.setPosition(50, 450);
    }


};

class AquaParkSimulator {
public:
    AquaParkSimulator(sf::RenderWindow& window)
        : window(window), menu(window), options(window), simulation(window) {
        init();
    }

    void run() {
        while (window.isOpen()) {
            

            //window.clear();

            if (inMenuScreen) {
                menu.run();
                if (optionsShouldClose) {
                    //startOptions();
                    inMenuScreen = false;
                    optionsShouldClose = false;
                }
            }
            else {
                //startSimulation();
            }

            window.display();
        }
    }

private:
    RenderWindow& window;
    Menu menu;
    Options options;
    Simulation simulation;

    bool inMenuScreen = true;
    bool optionsShouldClose = false;

    void init()
    {

    }

    void startSimulation() {
        window.clear();
        simulation.run();
    }

    void startOptions() {
        window.clear();
        options.run();
    }

    //void startHelp() {
    //    window.clear();
    //}
};


int main() {
    RenderWindow window(VideoMode(1280, 720), "AquaPark simulator");
    AquaParkSimulator aquaParkSim(window);
    aquaParkSim.run();

    return 0;
}