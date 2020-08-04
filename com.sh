#!/bin/sh
g++ -c -o raytrace.o raytrace.cpp
g++ -c -o material.o material.cpp
g++  -o raytrace raytrace.o material.o
