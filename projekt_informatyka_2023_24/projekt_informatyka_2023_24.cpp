// projekt_informatyka_2023_24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


using namespace std;
using namespace sf;


int main()
{
    cout << "Hello World!\n";

    RenderWindow window{ VideoMode{1000,1000}, "Projekt informatyka" };
    
    CircleShape triangle(80, 3);
    triangle.setPosition(100, 100);
    triangle.setFillColor(Color(0, 255, 255, 200));

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }


        window.clear();
        window.draw(triangle);
        window.display();
    }



    return 0;
}
