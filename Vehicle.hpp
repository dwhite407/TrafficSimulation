/*
    # Program Name: Vehicle.hpp
    # Author: Drew White
    # Date Last Updated: 7/31/2025
    # Purpose: This program will create the header file for the vehicles. createing getter and setter methods
    # This program will do a lot in terms of flipping, coloring, and assigning vehicle initializations
*/


#pragma once
#include <SFML/Graphics.hpp>
using namespace std;

class Vehicle {
protected:
    sf::RectangleShape body;
    float speed;
    sf::CircleShape headlight1, headlight2; //creating headlight variables for cpp

public:
    Vehicle(float x, float y, float speed, sf::Color color);
    virtual void move(float deltaTime);
    virtual void draw(sf::RenderWindow& window);
    virtual ~Vehicle() {}
    virtual void stop() { speed = 0.f; }
    virtual void resume(float newSpeed) { speed = newSpeed; } //car speeds
    float getX() const { return body.getPosition().x; } //gets the cars body positions
    float getY() const { return body.getPosition().y; }
    bool movingLeft = false;
    bool isReversed() const { return movingLeft; }
    void reverse() {
        movingLeft = true;
        speed = -fabs(speed);
        body.setRotation(180.f);                     //sets the car to face left on the opposite side (reverse)
    }

    bool movingVertical = false;
    void setVertical(bool goingDown); //vertical vehicle checking

    sf::RectangleShape getBody() const { return body; }

    bool isVertical() const { return movingVertical; }

    float startDelay = 0.f;
    bool waitingForGreen = false; //buffer for collison and pausing

    void setSpeed(float newSpeed) { speed = newSpeed; }
    float getSpeed() const { return speed; }

    float targetSpeed;
    float acceleration = 50.f; //acceleration for after red lights

};