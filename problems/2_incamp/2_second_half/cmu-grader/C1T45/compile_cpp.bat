@echo off

set problem=C1T45

g++ -std=c++17 -O -Wall -pipe -static -o %problem%.exe grader.cpp %problem%.cpp