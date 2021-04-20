#!/bin/bash

#regular compile
g++ -o raytracer src/main.cpp src/Ray.cpp src/Camera.cpp src/RenderObject.cpp src/Sphere.cpp src/Cube.cpp src/Vector3.cpp -lpthread -Ofast

#compile with symbols for debugging purposes
g++ -g -o raytracer_debug src/main.cpp src/Ray.cpp src/Camera.cpp src/renderObject.cpp src/Sphere.cpp src/Cube.cpp src/Vector3.cpp -lpthread 
