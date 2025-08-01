/*
    # Program Name: TrafficLight.hpp
    # Author: Drew White
    # Date Last Updated: 7/31/2025
    # Purpose: This program will create the header for traffic light creatin the getter and setter methods for its contents
*/


#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

enum class LightColor { Red, Yellow, Green };

//creates the traffic light class to use in trafficlight.cpp and main
class TrafficLight {
private:
    sf::CircleShape redLight;
    sf::CircleShape yellowLight;
    sf::CircleShape greenLight;
    LightColor currentColor;
    bool isVertical;

public:
    TrafficLight(float x, float y, bool vertical = false);

    void setColor(LightColor color);
    void draw(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const;
    LightColor getColor() const { return currentColor; }
};