# Particle Life

Particle life is a project that showcases the phenomenon of emergence in action. First, define some simple rules that govern how particles of different colors interact with each other, then watch as thousands of particles attract and repel each other to form beautiful patterns and structures in a simulation that seem to mirror simple multi cellular organisms. 

# Features 

My implementation of Particle Life has many features allowing you to customize the state of the simulation. 
- Clickable buttons
  - Modifying attraction parameters, particle count, color count, and particle distribution
- Sliders
  - Modifying simulation parameters pertaining to the attraction formula and physics
 

# How It's Made

### Tech Used: C++, SFML, CMake

This project was my introduction to using C++, SFML, and CMake. I wanted to use SFML as my graphics engine due to its ease of use, good documentation, and availability of learning resources online, and with a learn as I go mentality, I was able to complete the development of this project without the graphics ever being a bottleneck. I have previously worked in C, but this was my first dive into C++. The switch over was painless, and aside from utilizing vectors, maps, and classes (that probably could be structs anyways), most of the programming work has felt very similar to my work on projects in C. Finally, the use of SFML has made it necessary to use CMake so that I can make getting to a working build of the project as quick and painless as possible and learning to use the tool has been relatively simple.

# Inspiration

This project is an implementation of the [Particle Life Engine](https://particle-life.com/) created by [Tom Mohr](https://www.youtube.com/@tom-mohr). In specific, I have used the formula for determining particle interactions, and recreated many of the features present in Tom Mohr's project. I strongly recommend that you take a look at their work as they are a talented developer and laid the groundwork for me to create this project.
