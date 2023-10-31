// projekt_informatyka_2023_24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


using namespace std;
using namespace sf;


int main()
{
    cout << "Hello World!\n";

    RenderWindow window{ VideoMode{1000,1000}, "Projekt informatyka" };
    Event event;
    while (true)
    {
        window.clear(Color::Black);
        window.pollEvent(event);

        if (event.type == Event::Closed)
        {
            window.close();
            break;
        }

        window.display();
    }



    return 0;
}
