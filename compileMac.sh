#!/bin/bash

g++ -o raytracer_mac  src/main.cpp src/Ray.cpp src/Camera.cpp src/Sphere.cpp src/Cube.cpp src/Vector3.cpp -pthread -std=c++11 -stdlib=libc++
