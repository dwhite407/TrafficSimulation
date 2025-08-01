/*
	# Program Name: Truck.hpp
	# Author: Drew White
	# Date Last Updated: 7/31/2025
	# Purpose: This program is the header for the Truck, initializing the class object
*/


#pragma once
#include "Vehicle.hpp"

//initializing Truck class object, inheriting the class from Vehicle
class Truck : public Vehicle {
public:
    Truck(float x, float y) : Vehicle(x, y, 70.f, sf::Color::Red) {
        body.setSize(sf::Vector2f(100.f, 40.f));  //truck is bigger than the car
        body.setOrigin(50.f, 20.f); //changes the origin so its easier to check for collision
    }

};