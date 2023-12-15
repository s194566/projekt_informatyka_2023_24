// AquaPark simulator

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <list>
#include <array>
#include <fstream>


using namespace std;
using namespace sf;


class WaterPipe
{
public:
    WaterPipe()
    {

    }


private:

};

class Tank {
public:
    Tank(Vector2f position, Vector2f size, float waterLevel) : position(position), size(size), waterLevel(waterLevel) {
        init();
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

    void init() {
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


class InputData {
public:
    InputData()
    {
        labelText = "---";
        value = 20;
        position = Vector2f(10.0f, 10.0f);

        initializeElements();
    }

    InputData(const string& labelText, int defaultValue, const Vector2f& position)
        : labelText(labelText), value(defaultValue), position(position) {
        initializeElements();
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

    void initializeElements() {
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

class ParametersTable {
public:
    ParametersTable(const array<int, 8>& dynamicValues, const Vector2f& position, RenderWindow& window)
        : dynamicValues(dynamicValues), position(position), window(window) {
        init();
    }

    void draw() {
        for (size_t i = 0; i < fixedLabels.size(); ++i) {
            const string labelText = fixedLabels[i] + " ";
            drawParameter(labelText, dynamicVal[i], i);
        }
    }

private:
    const array<int, 8>& dynamicValues; 
    Vector2f position; 
    Font font;
    RenderWindow& window;

    const array<int, 8> dynamicVal = { // tymczasowe
        1,2,3,4,5,6,7,8
    };


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

class Simulation {
public:
    Simulation(RenderWindow& window, const std::array<int, 8>& enteredValues)
        : window(window),
        enteredValues(enteredValues),
        parametersTable(enteredValues, Vector2f(10.0f, 10.0f), window),
        Base(Vector2f(200.0f, 650.0f), Vector2f(1000.0f, 50.0f), 1),
        tank1(Vector2f(350.0f, 200.0f), Vector2f(100.0f, 250.0f), 0.5),
        tank2(Vector2f(500.0f, 400.0f), Vector2f(200.0f, 100.0f), 0.5),
        tank3(Vector2f(750.0f, 300.0f), Vector2f(200.0f, 200.0f), 0.5),
        tank4(Vector2f(1000.0f, 200.0f), Vector2f(200.0f, 200.0f), 0.5)
    {
        init();
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::MouseButtonReleased) {
                    handleButtonClick(event);
                }
            }

            Clock clock;
            float elapsedTime = clock.restart().asSeconds();

            tank1.updateWaterAnimation(elapsedTime);
            tank2.updateWaterAnimation(elapsedTime);
            tank3.updateWaterAnimation(elapsedTime);
            tank4.updateWaterAnimation(elapsedTime);

            window.clear(backgroundColor);

            parametersTable.draw();

            tank1.draw(window);
            tank2.draw(window);
            tank3.draw(window);
            tank4.draw(window);

            Base.draw(window);

            window.draw(TitleText);
            window.draw(backButton);

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
    Text TitleText;
    Text backButton;

    Tank tank1;
    Tank tank2;
    Tank tank3;
    Tank tank4;

    Tank Base;

    array<int, 8> enteredValues;
    ParametersTable parametersTable;

    bool simulationShouldClose = false;

    void init() {
        if (!font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf")) {
            cerr << "Error loading font" << endl;
        }

        backgroundColor = Color(0, 0, 255);

        TitleText.setFont(font);
        TitleText.setCharacterSize(24);
        TitleText.setFillColor(Color::White);
        TitleText.setPosition(640.0f, 10.0f);
        TitleText.setString("Symulacja");

        backButton.setFont(font);
        backButton.setCharacterSize(24);
        backButton.setString("cofnij");
        backButton.setPosition(1150.0, 1.0);
    }

    void handleButtonClick(const Event& event) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

        if (backButton.getGlobalBounds().contains(mousePos)) {
            window.clear();
            window.display();
            simulationShouldClose = true;
        }
    }
};

class Options {
public:
    Options(RenderWindow& window, array<int, 8>& enteredValues)
        : window(window), enteredValues(enteredValues),

        // Pool 1
        waterLevelInput1("Poziom wody w basenie 1", enteredValues[0], Vector2f(50, 150)),
        temperatureInput1("Temperatura wody w basenie 1", enteredValues[1], Vector2f(50, 200)),

        // Pool 2
        waterLevelInput2("Poziom wody w basenie 2", enteredValues[2], Vector2f(50, 300)),
        temperatureInput2("Temperatura wody w basenie 2", enteredValues[3], Vector2f(50, 350)),

        // Pool 3
        waterLevelInput3("Poziom wody w basenie 3", enteredValues[4], Vector2f(50, 450)),
        temperatureInput3("Temperatura wody w basenie 3", enteredValues[5], Vector2f(50, 500)),

        // Pool 4
        waterLevelInput4("Poziom wody w basenie 4", enteredValues[6], Vector2f(50, 600)),
        temperatureInput4("Temperatura wody w basenie 4", enteredValues[7], Vector2f(50, 650))

    {
        init();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            

            window.clear();

            window.draw(backgroundSprite);
            window.draw(backgroundRect);

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

private:
    RenderWindow& window;
    Font font;
    Texture background;
    Sprite backgroundSprite;
    RectangleShape backgroundRect;

    array<int, 8> enteredValues; // zamiana z wektora na tablice

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
                window.close();
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
        : window(window) {
        init();
    }

    void run() {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
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
    Menu(RenderWindow& window) : window(window) {
        init();
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
                        else if (paramsButton.getGlobalBounds().contains(mousePos)) {
                            cout << "Params" << endl;
                            startOptions();
                        }
                        else if (helpButton.getGlobalBounds().contains(mousePos)) {
                            cout << "help" << endl;
                            startHelp();
                        }
                        else if (exitOption.getGlobalBounds().contains(mousePos)) {
                            window.close();
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
                Options options(window, enteredValues);
                options.run();
                currentScreenState = ScreenState::Main;  // Powrot do menu
            }
            else if (currentScreenState == ScreenState::Help) {
                
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

    array<int, 8> enteredValues;

    enum class ScreenState {
        Main,
        Options,
        Help
    };

    ScreenState currentScreenState;


    void startOptions() {
        window.clear();
        Options options(window, enteredValues);
        options.run();
    }

    void startSimulation() {
        window.clear();
        Simulation simulation(window, enteredValues);
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
    AquaParkSimulator(RenderWindow& window)
        : window(window), menu(window), options(window, enteredValues), simulation(window, enteredValues) {
        init();
    }

    array<int, 8> readParamsDataFromFile(const std::string& filename) {
        ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            cerr << "Error opening file: " << filename << endl;
        }

        enteredValues.fill(0);  

        int value;
        for (int i = 0; i < enteredValues.size() && inputFile >> value; ++i) {
            enteredValues[i] = value;
        }

        inputFile.close();

        // Drukowanie wartosci z przekazanego pliku do konsoli w celu sprawdzenia
        cout << "Entered Values: ";
        for (const auto& entry : enteredValues) {
            cout << entry << " ";
        }
        cout << endl;

        return enteredValues;
    }

    void run() {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            window.clear();

            if (inMenuScreen) {
                menu.run();
                if (optionsShouldClose) {
                    readParamsDataFromFile("resources/saved_params/params.txt");
                    startOptions();
                    inMenuScreen = false;
                    optionsShouldClose = false;
                }
            }
            else {
                startSimulation();
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

    std::array<int, 8> enteredValues;

    void init() {
        
    }

    void startSimulation() {
        window.clear();
        simulation.run();
    }

    void startOptions() {
        window.clear();
        options.run();
    }

    void startHelp() {
        window.clear();
        // help
    }
};

int main() {
    RenderWindow window(VideoMode(1280, 720), "AquaPark simulator");
    AquaParkSimulator aquaParkSim(window);
    aquaParkSim.readParamsDataFromFile("resources/saved_params/params.txt");
    aquaParkSim.run();

    return 0;
}