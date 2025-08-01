/*
	# Program Name: Car.hpp
	# Author: Drew White
	# Date Last Updated: 7/31/2025
	# Purpose: This program is the header for the Car, initializing the class object
*/


#pragma once
#include "Vehicle.hpp"

//initializing car while inheriting Vehicle
class Car : public Vehicle {
public:
    Car(float x, float y) : Vehicle(x, y, 100.f, sf::Color::Blue) {} //sets color of car
};