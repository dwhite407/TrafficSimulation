/*
    # Program Name: Vehicle.cpp
    # Author: Drew White
    # Date Last Updated: 7/31/2025
    # Purpose: This program will create the base class Vehicle that is then inherited in car and truck
    # This will also draw the headlights, set speeds, sizes, and poisitions
*/


#include "Vehicle.hpp"
#include "Truck.hpp"
#include <iostream>

using namespace std;

//creates the object from the header file
Vehicle::Vehicle(float x, float y, float speed, sf::Color color) {
    this->speed = speed;
    body.setSize(sf::Vector2f(60.f, 30.f));  //default size of the vehicle (car)
    body.setOrigin(30.f, 15.f);
    body.setPosition(x, y);
    body.setFillColor(color);
    body.setRotation(0.f); //facing right technically

    //headlights, which are just small circles
    headlight1.setRadius(4.f);
    headlight1.setFillColor(sf::Color(255, 255, 180)); //yellowish color to act like a car
    headlight2.setRadius(4.f); //small circles
    headlight2.setFillColor(sf::Color(255, 255, 180));
}


//moving the car with deltaTime, which is using random from a set of numbers
void Vehicle::move(float deltaTime) {
    if (movingVertical)
        body.move(0.f, speed * deltaTime);
    else
        body.move(speed * deltaTime, 0.f);
}

//this will draw the vehicles and the headlights (was trying to add wheels but I just could not get them to look good so I removed them
void Vehicle::draw(sf::RenderWindow& window) {
    //draws the body of the vehicle
    window.draw(body);

    //allowing dynamnic size changes
    sf::Vector2f size = body.getSize();
    float halfWidth = size.x / 2.f;
    float halfHeight = size.y / 2.f;
    float angle = body.getRotation();

    //headlight sizing, bigger for trucks since they are bigger cars
    float headlightSize = dynamic_cast<Truck*>(this) ? 8.f : 5.f;
    headlight1.setRadius(headlightSize);
    headlight2.setRadius(headlightSize);
    headlight1.setOrigin(headlightSize / 2.f, headlightSize / 2.f);
    headlight2.setOrigin(headlightSize / 2.f, headlightSize / 2.f);

    //glow on the headlights so it is like they are turned on
    sf::CircleShape glow1(headlightSize * 1.5f);
    sf::CircleShape glow2(headlightSize * 1.5f);

    glow1.setOrigin(glow1.getRadius(), glow1.getRadius()); //origins for the headlight glows
    glow2.setOrigin(glow2.getRadius(), glow2.getRadius());

    glow1.setPosition(headlight1.getPosition());
    glow2.setPosition(headlight2.getPosition());

    //brighter in the center
    glow1.setFillColor(sf::Color(255, 255, 200, 150));
    glow2.setFillColor(sf::Color(255, 255, 200, 150));

    //draws the glow before the headlights for layering
    window.draw(glow1);
    window.draw(glow2);

    //rotats the offsets of headlights (definitely had to use chatGPT for this, took me a very long time even with the help from that)
    auto rotateOffset = [&](sf::Vector2f offset, float angle) -> sf::Vector2f {
        float rad = angle * 3.14159265f / 180.f;
        float cosA = cos(rad);
        float sinA = sin(rad);
        return { offset.x * cosA - offset.y * sinA, offset.x * sinA + offset.y * cosA };
        };

    //gets the corners from the vehicles for the lights
    sf::Vector2f frontTopCorner(halfWidth, -halfHeight);
    sf::Vector2f frontBottomCorner(halfWidth, halfHeight);

    //rotates the offset to match for vertical vehicles
    sf::Vector2f hl1 = rotateOffset(frontTopCorner, angle);
    sf::Vector2f hl2 = rotateOffset(frontBottomCorner, angle);

    //clamps the lights in a little since they were spread apart a little bit (help from chatGPT)
    auto clampInward = [&](sf::Vector2f& point, float amount) {
        sf::Vector2f dir = point - body.getPosition();
        float length = sqrt(dir.x * dir.x + dir.y * dir.y);
        dir /= length; //normalize
        point -= dir * (-amount); //pull inward slightly
        };

    clampInward(hl1, 2.f); //pull in by 2 pixels
    clampInward(hl2, 2.f);

    //applies the positions of the headlights
    headlight1.setPosition(body.getPosition() + hl1);
    headlight2.setPosition(body.getPosition() + hl2);

    //finally draws the headlights 
    window.draw(headlight1);
    window.draw(headlight2);
}


//sets the vertical positions for the vehicles
//this spot took a while because I was resizing the the vehicles and then rotating them so the orientation was off
void Vehicle::setVertical(bool goingDown) {
    movingVertical = true;
    movingLeft = !goingDown;
    speed = goingDown ? fabs(speed) : -fabs(speed);

    //rotates the vehicles 
    body.setRotation(goingDown ? 90.f : -90.f);

}


