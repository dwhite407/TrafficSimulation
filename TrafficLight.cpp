/*
    # Program Name: TrafficLight.cpp
    # Author: Drew White
    # Date Last Updated: 7/31/2025
    # This program will initialize and create the traffic lights
*/


#include <fstream>
#include <stdexcept>
#include "TrafficLight.hpp"

using namespace std;

//initial class declaration for the traffic lights
TrafficLight::TrafficLight(float x, float y, bool vertical) {
    float radius = 15.f;
    isVertical = vertical;

    //yellow circle, set as the base for easier math
    yellowLight.setRadius(radius);
    yellowLight.setFillColor(sf::Color(255, 255, 0, 100)); // dim
    yellowLight.setOrigin(radius, radius);
    yellowLight.setPosition(x, y);

    //red and green lights depending on the orientation of the street
    redLight.setRadius(radius);
    redLight.setOrigin(radius, radius);
    redLight.setFillColor(sf::Color(255, 0, 0, 100)); //dim light red

    greenLight.setRadius(radius);
    greenLight.setOrigin(radius, radius);
    greenLight.setFillColor(sf::Color(0, 255, 0, 100)); //dim light green

    if (isVertical) {
        //stack the lights for the veritcal roads
        redLight.setPosition(x, y - (radius * 2.5f));
        greenLight.setPosition(x, y + (radius * 2.5f));
    }
    else {
        //stack the lights for the horizontal roads
        redLight.setPosition(x - (radius * 2.5f), y);
        greenLight.setPosition(x + (radius * 2.5f), y);
    }

    currentColor = LightColor::Red;
}

void TrafficLight::setColor(LightColor color) {
    currentColor = color;

    if (color == LightColor::Red) {
        redLight.setFillColor(sf::Color(255, 50, 50));       //bright red light
        yellowLight.setFillColor(sf::Color(150, 150, 50, 150)); //dim yellow light
        greenLight.setFillColor(sf::Color(20, 120, 20, 150));   //dim green light
    }
    else if (color == LightColor::Yellow) {
        redLight.setFillColor(sf::Color(120, 20, 20, 150));     //dim red light
        yellowLight.setFillColor(sf::Color(255, 255, 100));     //bright yellow light
        greenLight.setFillColor(sf::Color(20, 120, 20, 150));   //dim green light
    }
    else if (color == LightColor::Green) {
        redLight.setFillColor(sf::Color(120, 20, 20, 150));     //dim red light
        yellowLight.setFillColor(sf::Color(150, 150, 50, 150)); //dim yellow light
        greenLight.setFillColor(sf::Color(50, 255, 50));        //bright green light
    }
}

void TrafficLight::draw(sf::RenderWindow& window) const {
    //sets the values between bright and dim lights
    sf::Color brightRed(255, 50, 50);
    sf::Color brightYellow(255, 255, 100);
    sf::Color brightGreen(50, 255, 50);

    sf::Color dimRed(120, 20, 20, 150);
    sf::Color dimYellow(150, 150, 50, 150);
    sf::Color dimGreen(20, 120, 20, 150);

    //draws the glow for the "bright" lights
    if (currentColor == LightColor::Red) {
        sf::CircleShape glow(redLight.getRadius() * 1.3f);
        glow.setFillColor(sf::Color(255, 50, 50, 100)); //translucent glow for the lights
        glow.setOrigin(glow.getRadius(), glow.getRadius());
        glow.setPosition(redLight.getPosition());
        window.draw(glow);
    }
    else if (currentColor == LightColor::Yellow) {
        sf::CircleShape glow(yellowLight.getRadius() * 1.3f);
        glow.setFillColor(sf::Color(255, 255, 100, 100));
        glow.setOrigin(glow.getRadius(), glow.getRadius());
        glow.setPosition(yellowLight.getPosition());
        window.draw(glow);
    }
    else if (currentColor == LightColor::Green) {
        sf::CircleShape glow(greenLight.getRadius() * 1.3f);
        glow.setFillColor(sf::Color(50, 255, 50, 100));
        glow.setOrigin(glow.getRadius(), glow.getRadius());
        glow.setPosition(greenLight.getPosition());
        window.draw(glow);
    }

    //draws the lights with their states
    window.draw(redLight);
    window.draw(yellowLight);
    window.draw(greenLight);
}


sf::Vector2f TrafficLight::getPosition() const {
    return yellowLight.getPosition(); //starts the lights as yellow the origin
}