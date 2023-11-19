// AquaPark simulator

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <list>


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

class ParametersTable {
public:
    ParametersTable(RenderWindow& window, const vector<pair<string, int>>& parameters, const Vector2f& position)
        : window(window), parameters(parameters), position(position) {
        initializeElements();
    }

    void draw() {
        for (size_t i = 0; i < parameters.size(); ++i) {
            const auto& parameter = parameters[i];
            drawParameter(parameter.first, parameter.second, i);
        }
    }

private:
    RenderWindow& window;
    Font font;
    vector<pair<string, int>> parameters;
    Vector2f position;
    int characterSize = 16;  // Rozmiar font
    Color backgroundColor = Color(192, 192, 192, 200);  // kolor

    void initializeElements() {
        if (!font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf")) {
            cerr << "Error loading font" << endl;
        }
    }

    void drawParameter(const string& labelText, int value, size_t index) {
        RectangleShape backgroundRect(Vector2f(300.0f, 40.0f));  // rozmiar pola
        backgroundRect.setFillColor(backgroundColor);
        backgroundRect.setPosition(position.x, position.y + index * 50.0f);

        Text parameterText;
        parameterText.setFont(font);
        parameterText.setCharacterSize(characterSize);
        parameterText.setPosition(position.x + 5.0f, position.y + index * 50.0f + 5.0f);
        parameterText.setString(labelText + ": " + to_string(value));

        window.draw(backgroundRect);
        window.draw(parameterText);
    }
};

class Simulation {
public:
    Simulation(RenderWindow& window, const vector<pair<string, int>>& parameters)
        : window(window), tank1(Vector2f(800.0f, 300.0f), Vector2f(150.0f, 300.0f), 0.1f), parameters(parameters),
        backgroundColor(Color::Blue), font(), TitleText(), parametersTable(window, parameters, Vector2f(10.0f, 10.0f)) {
        initializeElements();
    }

    void run() {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            Clock clock;
            float elapsedTime = clock.restart().asSeconds();

            tank1.updateWaterAnimation(elapsedTime);

            window.clear(backgroundColor);

            tank1.draw(window);
            drawParametersTable();

            window.draw(TitleText);

            window.display();
        }
    }

private:
    RenderWindow& window;
    Color backgroundColor;
    Font font;
    Text TitleText;
    Tank tank1;
    vector<pair<string, int>> parameters;
    ParametersTable parametersTable;

    void initializeElements() {
        if (!font.loadFromFile("resources/Fonts/OpenSans-Bold.ttf")) {
            cerr << "Error loading font" << endl;
        }

        TitleText.setFont(font);
        TitleText.setCharacterSize(24);
        TitleText.setFillColor(Color::White);
        TitleText.setPosition(640.0f, 10.0f);
        TitleText.setString("Symualcja");
    }

    void drawParametersTable() {
        parametersTable.draw();
    }
};


class InputData {
public:
    InputData()
    {
        // Default values
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

class Options {
public:
    Options(RenderWindow& window) : window(window), 

        // Pool 1
        waterLevelInput1("Poziom wody w basenie 1", 20, Vector2f(50, 150)),
        temperatureInput1("Temperatura wody w basenie 1", 20, Vector2f(50, 200)),

        // Pool 2
        waterLevelInput2("Poziom wody w basenie 2", 20, Vector2f(50, 300)),
        temperatureInput2("Temperatura wody w basenie 2", 20, Vector2f(50, 350)),

        // Pool 3
        waterLevelInput3("Poziom wody w basenie 3", 20, Vector2f(50, 450)),
        temperatureInput3("Temperatura wody w basenie 3", 20, Vector2f(50, 500)),

        // Pool 4
        waterLevelInput4("Poziom wody w basenie 4", 20, Vector2f(50, 600)),
        temperatureInput4("Temperatura wody w basenie 4", 20, Vector2f(50, 650))
    
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

        backgroundRect.setSize(Vector2f(1280.0f, 720.0f));
        backgroundRect.setFillColor(Color(0, 0, 0, 128));
        backgroundRect.setPosition(0.0f, 0.0f);

        

        // Save button
        saveButton.setFont(font);
        saveButton.setCharacterSize(36);
        saveButton.setString("Rozpocznij symulacje");
        saveButton.setPosition(300.0, 75.0);
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

            waterLevelInput1.draw(window);
            temperatureInput1.draw(window);
            waterLevelInput2.draw(window);
            temperatureInput2.draw(window);
            waterLevelInput3.draw(window);
            temperatureInput3.draw(window);
            waterLevelInput4.draw(window);
            temperatureInput4.draw(window);

            window.draw(saveButton);

            window.display();
        }
    }

    // Add a method to get entered values
    vector<pair<string, int>> getEnteredValues() const {
        return enteredValues;
    }

private:
    RenderWindow& window;
    Font font;
    Texture background;
    Sprite backgroundSprite;
    RectangleShape backgroundRect;

    vector<pair<string, int>> enteredValues; // Store entered values and their labels

    InputData waterLevelInput1;
    InputData temperatureInput1;
    InputData waterLevelInput2;
    InputData temperatureInput2;
    InputData waterLevelInput3;
    InputData temperatureInput3;
    InputData waterLevelInput4;
    InputData temperatureInput4;

    Text saveButton;

    int id = 0;

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

        if (saveButton.getGlobalBounds().contains(mousePos)) {
            vector<pair<string, int>> enteredValues = saveOptions();
            startSimulation(window, enteredValues);
        }
    }

    vector<pair<string, int>> saveOptions() {
        enteredValues.clear();

        cout << "Zapis dane:" << id << endl;
        // Save water level and temperature values for each pool
        enteredValues.emplace_back("Poziom wody w basenie 1", waterLevelInput1.getValue());
        enteredValues.emplace_back("Temperatura wody w basenie 1", temperatureInput1.getValue());

        enteredValues.emplace_back("Poziom wody w basenie 2", waterLevelInput2.getValue());
        enteredValues.emplace_back("Temperatura wody w basenie 2", temperatureInput2.getValue());

        enteredValues.emplace_back("Poziom wody w basenie 3", waterLevelInput3.getValue());
        enteredValues.emplace_back("Temperatura wody w basenie 3", temperatureInput3.getValue());

        enteredValues.emplace_back("Poziom wody w basenie 4", waterLevelInput4.getValue());
        enteredValues.emplace_back("Temperatura wody w basenie 4", temperatureInput4.getValue());

        // Print the entered values to the console
        for (const auto& entry : enteredValues) {
            cout << entry.first << ": " << entry.second << endl;
        }

        id += 1;

        return enteredValues;
    }


    void startSimulation(RenderWindow& window, const vector<pair<string, int>>& enteredValues) const {
      
        // Pass the entered values to the Simulation constructor
        Simulation simulation(window, enteredValues);
        simulation.run(); // start symulacji
    }
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

        optionsButton.setString("Pomoc");
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
                            startOptions();
                            
                        }
                        else if (optionsButton.getGlobalBounds().contains(mousePos)) {
                            cout << "Tu będzie help" << endl;
                            
                            
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



    void startOptions() {
        window.clear();
        Options options(window);
        options.run();
    }
};




int main() {
    RenderWindow window(VideoMode(1280, 720), "AquaPark simulator");
    Menu menu(window);
    menu.run();

    return 0;
}