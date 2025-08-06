# Particle Life

Particle life is a project that showcases the phenomenon of emergence in action. First, define some simple rules that govern how particles of different colors interact with each other, then watch as thousands of particles attract and repel each other to form beautiful patterns and structures in a simulation that seem to mirror simple multi cellular organisms. 

# Features 

- Performance
  - Ability to efficiently simulate upwards of 100,000 particles at a high frame rate (dependent on decent computer specs and the parameter d_max being relatively low)
- Display
  - A matrix to visualize how different color particles attract and repel each other
  - A graphical representation of the color distribution
  - FPS counter
  - Many other UI features
- Clickable buttons 
  - Modifying attraction parameters, particle count, color count, and particle distribution
- Sliders
  - Modifying simulation parameters pertaining to the attraction formula and physics
 
All UI elements, buttons, and sliders were made from scratch using C++ and SFML.

# Optimizations

### Dynamic spatial partitioning 

Particles only interact with each other if they are within a distance of d_max, so it makes sense to partition particles so that they don't need to compare themselves with particles farther than d_max away. So, based on the parameter d_max, the space containing particles is partitioned into squares with side length d_max and each particle is only compared with particles in its own partition and the 8 neighboring partitions as particles in those partitions are the only ones that can be within d_max of each other. 
This optimization ensures that when particles are spread out, the time complexity of comparisons is not in O(n^2) time as it would otherwise be allowing for much larger simulations.

### Multithreading

Multithreading has been implemented so that particles and their interactions can be computed in parallel allowing for a 3-5X increase in efficiency for each physics tick simulated.

### 
# How It's Made

### Tech Used: C++, SFML, CMake

This project was my introduction to using C++, SFML, and CMake. I wanted to use SFML as my graphics engine due to its ease of use, good documentation, and availability of learning resources online, and with a learn as I go mentality, I was able to complete the development of this project without the graphics ever being a bottleneck. I have previously worked in C, but this was my first dive into C++. The switch over was painless, and aside from utilizing vectors, maps, and classes (that probably could be structs anyways), most of the programming work has felt very similar to my work on projects in C. Finally, the use of SFML has made it necessary to use CMake so that I can make getting to a working build of the project as quick and painless as possible and learning to use the tool has been relatively simple.

# Inspiration

This project is an implementation of the [Particle Life Engine](https://particle-life.com/) created by [Tom Mohr](https://www.youtube.com/@tom-mohr). In specific, I have used the formula for determining particle interactions, and recreated many of the features present in Tom Mohr's project. I strongly recommend that you take a look at their work as they are a talented developer and laid the groundwork for me to create this project.

# What I've Learned

Making this program has taught me that I can truly make whatever I want with enough time and effort without the need to use any fancy frameworks or libraries to get stuff done. At the beginning of the project, I remember looking for a button class in the SFML documentation because that was what I was used to. But as I learned there was no class made for me, I came to a seemingly obvious conclusion in hindsight, I can just make the class myself with ease. All that I needed is to define the area the button takes up, a variable stating whether it had been pressed, and set up event handling in my main loop to see if the button is clicked. Just like that, in less time than I spent looking for a button class, I had created a button class for myself. This realization inspired me to stop looking for help at every turn and break problems down into their simplest form and see that many seemingly complicated systems are not too hard to program. The ability to create whatever I want from the ground up is what drew me to spend part of my summer on C++ and it has taught me so much about what I can really do with code and the immense power that simple code can have.

# Windows Installation

### Prerequisites

- Have CMake, and Visual Studios 2022 Installed

### Compilation

- Clone the repository to your device
- Run CMake
- Select the location of the repository
- Build binaries at /build (This folder will be created for you)
  <img width="907" height="831" alt="image" src="https://github.com/user-attachments/assets/c3e1af44-d677-42dc-9362-8aa40808be5f" />
- Click on configure and click yes when prompted to create the build directory
  <img width="987" height="910" alt="image" src="https://github.com/user-attachments/assets/0b0c065b-8808-43a5-870e-27754e0dd95d" />
- Click Finish
  <img width="900" height="837" alt="image" src="https://github.com/user-attachments/assets/01107a8f-1b02-4567-b90f-a53a08eda273" />
- The configuration process can be lengthy (upwards of 3 minutes), let it run until completion
- Click Generate
  <img width="907" height="837" alt="image" src="https://github.com/user-attachments/assets/71bccd52-4790-4174-8e10-c677dcf8669d" />
- Click Open Project
  <img width="985" height="911" alt="image" src="https://github.com/user-attachments/assets/6bb2d3b6-a562-4918-ae76-feeb869e00c5" />
- ###### In Visual Studio, click on main and select Set as Startup Project (THIS IS NECESSARY OR ELSE THERE WILL BE AN ERROR)
  <img width="1919" height="1079" alt="Screenshot 2025-07-23 115254" src="https://github.com/user-attachments/assets/3989ee31-5e9e-4e99-9b3a-d7b6371d0eb1" />
- Change from debug to release mode for optimal performance
  <img width="1919" height="1079" alt="Screenshot 2025-07-23 120308" src="https://github.com/user-attachments/assets/bedc902f-d8ee-43d2-93fd-597b1a0ccc4d" />
- Run the program
  <img width="1918" height="1078" alt="image" src="https://github.com/user-attachments/assets/77a32a17-784b-4f92-af80-66e9341b5898" />
  



  




