@echo off

set problem=C1T43

g++ -std=c++17 -O -Wall -pipe -static -o %problem%.exe grader.cpp %problem%.cpp