# TrafficSimulation
This is a simple traffic light simulation I built using C++ and SFML. The goal of this project was to simulate cars and trucks navigating an intersection with working traffic lights. It features randomized vehicle spawns, realistic traffic light cycles, queuing behavior, and some visual details like headlights and light poles.

Features
Traffic Light System
Lights change between green, yellow, and red with a realistic all-red buffer phase.
These lights are on a timer, more like inner city lights, opposed the pressure-plated systems.

Vehicle Movement
Cars and trucks spawn randomly from all four directions and random speeds within a range

Queuing and Stop Lines
Vehicles stop before the line and queue up with spacing (larger gap for trucks).

Randomized Vehicle Speeds
Cars and trucks spawn at different speeds to make traffic flow feel more realistic.

Visual Elements
Includes road lanes, poles, working traffic lights, and headlights for vehicles.

Automatic Vehicle Cleanup
Vehicles are deleted from memory once they exit the screen.

Requirements
C++17 or later

SFML 2.6+ (Graphics and Audio modules used)

A compiler like g++ or Visual Studio with SFML properly linked.

How to Build and Run
Clone the repo:

git clone https://github.com/dwhite407/TrafficSimulation.git
Open the project in your IDE.

Make sure SFML is installed and linked.

Run the program, you should see an intersection with vehicles spawning and functioning traffic lights.

Controls
There are no direct user controls; the simulation runs automatically.

Known Issues / Future Improvements
Sometimes the vehicles will stop a little bit in the intersection, safe from wrecks but still a bug.
I could have fixed this but again, it looked more realistic so I stuck with it. 

Sound effects/ambient audio didn’t make it into the final build due to issues with routing. 

Future I would like to add more roads, sidewalks, turns, pressure-plated lights. I would also like to add more vehicles and some cases for emergency vehicles.
I would also like to take a similar approach to Cities Skylines and allow the players to start certain events (such as emergency vehicle, broken lights, etc.)

What I Learned
This project helped me get more comfortable with:

Working with SFML for graphics and timing, managin objects in C++ and working with time and randomizing.
The hardest part of this project was debugging, especially since it  was the first time I have worked with SFML library
I could learn more about how to incorporate the audio and other feature of the library, but overall was very fun and a 
great learning process.

Credits
Built by Drew White as a final project for my Computer Science 2 course.
