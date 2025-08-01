/*
    # Program Name: Main.cpp
    # Author: Drew White
    # Date Last Updated: 7/31/2025
    # Purpose: This program will give a basic traffic simulation 
    # The cars will spawn in randomly and have their own random speeds
    # There will be no collision between vehicles, and random vehicles will spawn, red bein gtrucks and blue being cars
    # The traffic lights will cycle off of real time
*/


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "TrafficLight.hpp"
#include "Vehicle.hpp"
#include "Car.hpp"
#include "Truck.hpp"
#include <iostream>
using namespace std;

int main() {
    //renders the window to the size of a 1920 x 1080 monitor, resizin not available as it will distort the image right now
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Traffic Light Simulation");
    window.setFramerateLimit(60); //sets frame rate limit to 60 for smooth simulation

    srand(static_cast<unsigned>(time(0))); //seed for random cases

    //gets the font file for the text in the upper left corner of the screen
    sf::Font font;
    if (!font.loadFromFile("resources/arial.ttf")) {
        cerr << "Error: Could not load arial.ttf! Make sure it is in the correct folder.\n";
    }


    //tried to get the sound to work...tried many different things and just could not seem to get it right. Something I will have to work on in the future.

    /*
    sf::Music music;
    if (!music.openFromFile("sounds/city_new.ogg")) {
        cerr << "Failed to load sound file. Check path & existence.\n";
    }
    
     music.setLoop(true);
     music.setVolume(25.f);
     music.play();
    */

    //text properties
    sf::Text title("Traffic Simulation", font, 40);
    title.setFillColor(sf::Color::White);
    title.setPosition(20.f, 20.f);

    sf::Text vehicleCount("Vehicles: 0", font, 25);
    vehicleCount.setFillColor(sf::Color::Yellow);
    vehicleCount.setPosition(20.f, 70.f);

    //traffic light vectors for all streets
    vector<TrafficLight> lights = {
        TrafficLight(750.f, 500.f, true),
        TrafficLight(1050.f, 500.f, true),
        TrafficLight(902.f, 355.f, false),
        TrafficLight(902.f, 645.f, false)
    };

    //vector to hole vehicles
    vector<Vehicle*> vehicles;

    //shapes for the roads
    sf::RectangleShape horizontalRoad(sf::Vector2f(1920.f, 250.f));
    horizontalRoad.setFillColor(sf::Color(0, 0, 0));
    horizontalRoad.setPosition(0.f, 375.f);

    sf::RectangleShape verticalRoad(sf::Vector2f(250.f, 1080.f));
    verticalRoad.setFillColor(sf::Color(0, 0, 0));
    verticalRoad.setPosition(775.f, 0.f);

    //shapes for the lane dividers on the roads
    vector<sf::RectangleShape> laneDividers;
    for (int i = 0; i < 7; i++) { //horizontal dividers
        sf::RectangleShape divider(sf::Vector2f(50.f, 5.f));
        divider.setFillColor(sf::Color::White);
        divider.setPosition(20.f + (i * 342.f), 500.f);
        laneDividers.push_back(divider);
    }
    for (int j = 0; j < 6; j++) { //vertical dividers
        sf::RectangleShape dividerVertical(sf::Vector2f(5.f, 50.f));
        dividerVertical.setFillColor(sf::Color::White);
        dividerVertical.setPosition(900.f, 20.f + (j * 305.f));
        laneDividers.push_back(dividerVertical);
    }

    //these are the traffic light holder rectangles (initially they were on poles but they looked weird so i switched them
    vector<sf::RectangleShape> poles;
    sf::RectangleShape poleLeft(sf::Vector2f(40.f, 120.f));
    poleLeft.setFillColor(sf::Color(80, 80, 80));
    poleLeft.setPosition(lights[0].getPosition().x - 20.f, lights[0].getPosition().y - 60.f);
    poles.push_back(poleLeft);

    sf::RectangleShape poleRight(sf::Vector2f(40.f, 120.f));
    poleRight.setFillColor(sf::Color(80, 80, 80));
    poleRight.setPosition(lights[1].getPosition().x - 20.f, lights[1].getPosition().y - 60.f);
    poles.push_back(poleRight);

    sf::RectangleShape poleTop(sf::Vector2f(120.f, 40.f));
    poleTop.setFillColor(sf::Color(80, 80, 80));
    poleTop.setPosition(lights[2].getPosition().x - 60.f, lights[2].getPosition().y - 20.f);
    poles.push_back(poleTop);

    sf::RectangleShape poleBottom(sf::Vector2f(120.f, 40.f));
    poleBottom.setFillColor(sf::Color(80, 80, 80));
    poleBottom.setPosition(lights[3].getPosition().x - 60.f, lights[3].getPosition().y - 20.f);
    poles.push_back(poleBottom);

    //variables to get timers for light switching logic and car spawning
    sf::Clock clock;
    float spawnTimer = 0.f;
    float cycleTimer = 0.f;
    float greenDuration = 10.f;
    float yellowDuration = 2.f;
    float allRedDuration = 1.f;
    bool horizontalGreen = true;
    enum class LightPhase { Green, Yellow, AllRed };
    LightPhase currentPhase = LightPhase::Green;

    //this sets for late starts after red light
    static bool lastHorizontalGreen = false;
    static bool lastVerticalGreen = false;

    //simulation loop
    while (window.isOpen()) {
        sf::Event event;
        float deltaTime = clock.restart().asSeconds();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //this will cycle through the red, green and yellow light phases
        //Each green light is 10 seconds, yellow 2, and red for the next 11
        //all four lights will be red for 1 second
        //cars will have delayed starts to act like real traffic
        cycleTimer += deltaTime;
        if (horizontalGreen) {
            if (currentPhase == LightPhase::Green) {
                lights[0].setColor(LightColor::Green);
                lights[1].setColor(LightColor::Green);
                lights[2].setColor(LightColor::Red);
                lights[3].setColor(LightColor::Red);
                if (cycleTimer >= greenDuration) { currentPhase = LightPhase::Yellow; cycleTimer = 0.f; }
            }
            else if (currentPhase == LightPhase::Yellow) {
                lights[0].setColor(LightColor::Yellow);
                lights[1].setColor(LightColor::Yellow);
                lights[2].setColor(LightColor::Red);
                lights[3].setColor(LightColor::Red);
                if (cycleTimer >= yellowDuration) { currentPhase = LightPhase::AllRed; cycleTimer = 0.f; }
            }
            else if (currentPhase == LightPhase::AllRed) {
                lights[0].setColor(LightColor::Red);
                lights[1].setColor(LightColor::Red);
                lights[2].setColor(LightColor::Red);
                lights[3].setColor(LightColor::Red);
                if (cycleTimer >= allRedDuration) { horizontalGreen = false; currentPhase = LightPhase::Green; cycleTimer = 0.f; }
            }
        }
        else {
            if (currentPhase == LightPhase::Green) {
                lights[0].setColor(LightColor::Red);
                lights[1].setColor(LightColor::Red);
                lights[2].setColor(LightColor::Green);
                lights[3].setColor(LightColor::Green);
                if (cycleTimer >= greenDuration) { currentPhase = LightPhase::Yellow; cycleTimer = 0.f; }
            }
            else if (currentPhase == LightPhase::Yellow) {
                lights[0].setColor(LightColor::Red);
                lights[1].setColor(LightColor::Red);
                lights[2].setColor(LightColor::Yellow);
                lights[3].setColor(LightColor::Yellow);
                if (cycleTimer >= yellowDuration) { currentPhase = LightPhase::AllRed; cycleTimer = 0.f; }
            }
            else if (currentPhase == LightPhase::AllRed) {
                lights[0].setColor(LightColor::Red);
                lights[1].setColor(LightColor::Red);
                lights[2].setColor(LightColor::Red);
                lights[3].setColor(LightColor::Red);
                if (cycleTimer >= allRedDuration) { horizontalGreen = true; currentPhase = LightPhase::Green; cycleTimer = 0.f; }
            }
        }

        //spawning logic for all of the vehicles
        spawnTimer += deltaTime;
        if (spawnTimer >= 2.f) {
            int direction = rand() % 4; // 0=left,1=right,2=top,3=bottom
            int type = rand() % 2; // 0=car, 1=truck
            if (direction == 0) { //this will orient the cars in the right direction, as well as spawn them in at a random speed for each direction
                if (type == 0) { Car* c = new Car(-150.f, 555.f); c->setSpeed(80.f + rand() % 41); c->targetSpeed = c->getSpeed(); c->setSpeed(0.f); vehicles.push_back(c); }
                else { Truck* t = new Truck(-200.f, 555.f); t->setSpeed(60.f + rand() % 21); t->targetSpeed = t->getSpeed(); t->setSpeed(0.f); vehicles.push_back(t); }
            }
            else if (direction == 1) {
                if (type == 0) { Car* c = new Car(2000.f, 445.f); c->setSpeed(80.f + rand() % 41); c->targetSpeed = c->getSpeed(); c->setSpeed(0.f); c->reverse(); vehicles.push_back(c); }
                else { Truck* t = new Truck(2050.f, 445.f); t->setSpeed(60.f + rand() % 21); t->targetSpeed = t->getSpeed(); t->setSpeed(0.f); t->reverse(); vehicles.push_back(t); }
            }
            else if (direction == 2) {
                if (type == 0) { Car* c = new Car(840.f, -150.f); c->setSpeed(80.f + rand() % 41); c->targetSpeed = c->getSpeed(); c->setSpeed(0.f); c->setVertical(true); vehicles.push_back(c); }
                else { Truck* t = new Truck(840.f, -200.f); t->setSpeed(60.f + rand() % 21); t->targetSpeed = t->getSpeed(); t->setSpeed(0.f); t->setVertical(true); vehicles.push_back(t); }
            }
            else if (direction == 3) {
                if (type == 0) { Car* c = new Car(965.f, 1100.f); c->setSpeed(80.f + rand() % 41); c->targetSpeed = c->getSpeed(); c->setSpeed(0.f); c->setVertical(false); vehicles.push_back(c); }
                else { Truck* t = new Truck(965.f, 1150.f); t->setSpeed(60.f + rand() % 21); t->targetSpeed = t->getSpeed(); t->setSpeed(0.f); t->setVertical(false); vehicles.push_back(t); }
            }
            spawnTimer = 0.f;
        }

        //this is the "stop" lines for the cars...essentially saying where the cars will stop in the program
        float stopLineWest = lights[0].getPosition().x - 80.f;
        float stopLineEast = lights[1].getPosition().x + 80.f;
        float stopLineNorth = lights[2].getPosition().y - 90.f;
        float stopLineSouth = lights[3].getPosition().y + 80.f;

        //this will determine where the cars will stop according to their position (north east south or west)
        for (auto& v : vehicles) {
            if (v->isVertical()) {
                if (v->isReversed()) { if (lights[2].getColor() != LightColor::Green && v->getY() <= stopLineSouth && v->getY() > stopLineSouth - 100.f) v->stop(); else v->resume(-100.f); }
                else { if (lights[3].getColor() != LightColor::Green && v->getY() >= stopLineNorth && v->getY() < stopLineNorth + 100.f) v->stop(); else v->resume(100.f); }
            }
            else {
                if (v->isReversed()) { if (lights[0].getColor() != LightColor::Green && v->getX() <= stopLineEast && v->getX() > stopLineEast - 100.f) v->stop(); else v->resume(-100.f); }
                else { if (lights[1].getColor() != LightColor::Green && v->getX() >= stopLineWest && v->getX() < stopLineWest + 100.f) v->stop(); else v->resume(100.f); }
            }
        }
        //delay logic for late start from cars in behind
        if (horizontalGreen && !lastHorizontalGreen) {
            float delay = 0.f; for (auto& v : vehicles) if (!v->isVertical()) { v->startDelay = delay; v->waitingForGreen = true; delay += 0.3f; }
        }
        lastHorizontalGreen = horizontalGreen;
        if (!horizontalGreen && !lastVerticalGreen) {
            float delay = 0.f; for (auto& v : vehicles) if (v->isVertical()) { v->startDelay = delay; v->waitingForGreen = true; delay += 0.3f; }
        }
        lastVerticalGreen = !horizontalGreen;
        //collision logic for cars
        //no cars will bump into each other
        float carGap = 50.f; //distance the cars will stay from each other
        for (size_t i = 0;i < vehicles.size();i++) {
            Vehicle* current = vehicles[i];
            for (size_t j = 0;j < vehicles.size();j++) {
                if (i == j) continue;
                Vehicle* ahead = vehicles[j];
                if (current->isVertical() && ahead->isVertical() && abs(current->getX() - ahead->getX()) < 10.f) { //logic for distance between vehicles vertically
                    float gap = ahead->getBody().getSize().y + (dynamic_cast<Truck*>(ahead) ? 90.f : 70.f);
                    if (!current->isReversed()) { if (ahead->getY() > current->getY() && ahead->getY() - current->getY() < gap) current->stop(); }
                    else { if (ahead->getY() < current->getY() && current->getY() - ahead->getY() < gap) current->stop(); }
                }
                else if (!current->isVertical() && !ahead->isVertical() && abs(current->getY() - ahead->getY()) < 10.f) { //logic for distance between vehicles horizontally
                    float gap = ahead->getBody().getSize().x + (dynamic_cast<Truck*>(ahead) ? 50.f : 30.f);
                    if (!current->isReversed()) { if (ahead->getX() > current->getX() && ahead->getX() - current->getX() < gap) current->stop(); }
                    else { if (ahead->getX() < current->getX() && current->getX() - ahead->getX() < gap) current->stop(); }
                }
            }
        }
        //loop for the start delay of each car in queue, starting from a slower speed to act as acceleration
        for (auto& v : vehicles) {
            if (v->waitingForGreen) {
                if (v->startDelay > 0.f) { v->startDelay -= deltaTime; v->stop(); }
                else v->waitingForGreen = false;
            }
            if (!v->waitingForGreen && v->getSpeed() < v->targetSpeed) {
                float newSpeed = v->getSpeed() + v->acceleration * deltaTime;
                if (newSpeed > v->targetSpeed) newSpeed = v->targetSpeed;
                v->setSpeed(newSpeed);
            }
            v->move(deltaTime);
        }

        //clears up the vehicles when they exit the screen to save memory and run smoother
        vehicles.erase(remove_if(vehicles.begin(), vehicles.end(), [](Vehicle* v) {
            float x = v->getX(), y = v->getY();
            if (x < -200.f || x>2120.f || y < -200.f || y>1280.f) { delete v; return true; } return false;
            }), vehicles.end());

        //updates the text for amount of vehicles
        vehicleCount.setString("Vehicles: " + to_string(vehicles.size()));

        //draws everything on the screen
        window.clear(sf::Color(01, 32, 20));
        window.draw(horizontalRoad);
        window.draw(verticalRoad);
        for (auto& d : laneDividers) window.draw(d);
        for (auto& v : vehicles) v->draw(window);
        for (auto& p : poles) window.draw(p);
        for (auto& l : lights) l.draw(window);
        window.draw(title);
        window.draw(vehicleCount);
        window.display();
    }

    return 0;
}
