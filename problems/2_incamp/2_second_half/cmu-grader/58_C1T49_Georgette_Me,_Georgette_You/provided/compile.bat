@echo off

set "task=Georgette"
set "grader_name=grader"

g++ -std=gnu++17 -Wall -O2 -pipe -static -g -o "%task%.exe" "%grader_name%.cpp" "%task%.cpp"