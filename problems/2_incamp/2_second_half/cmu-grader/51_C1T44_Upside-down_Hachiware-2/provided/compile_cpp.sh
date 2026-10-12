#!/bin/bash

problem="C1T44"

g++ -std=c++17 -O -Wall -pipe -static -o "${problem}" grader.cpp "${problem}.cpp"