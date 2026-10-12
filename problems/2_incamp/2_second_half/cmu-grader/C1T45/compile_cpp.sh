#!/bin/bash

problem="C1T45"

g++ -std=c++17 -O -Wall -pipe -static -o "${problem}" grader.cpp "${problem}.cpp"