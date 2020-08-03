#!/bin/sh
g++ -c -o raytrace.o raytrace.cpp
g++  -o raytrace raytrace.o material.o
