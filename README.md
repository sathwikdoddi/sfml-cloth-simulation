# Verlet Objects Cloth Simulation with C++ & SFML
## Program Demo
https://github.com/sathwikdoddi/sfml-cloth-simulation/assets/63931481/193f5d6d-e8d1-431d-904c-b4eb1759a992

## Documentation
Program was made with SFML and C++, there is a solver.hpp file in the include/ folder which imports the object.hpp file containing a struct for a VerletObject. 
These objects are able to hold vectors of links connected to other objects and are linked together by affixing them at set distances. Links are removed by
checking if the mouse slides over a link using vector math from each VerletObject to the mouse compared to a vector drawn from one VerletObject to another
which it is linked to. Inspiration for this project came from Pezza's Work on YT.

If you are on a Mac, you can install SFML with the command
`brew install sfml`
And then you should check where SFML was installed using the command
`brew info sfml`

In order to compile the main file while including the needed dependencies, run the command
`g++ main.cpp -I/PATH/TO/SFML/2.6.1/include -o main -L/PATH/TO/SFML/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system`
