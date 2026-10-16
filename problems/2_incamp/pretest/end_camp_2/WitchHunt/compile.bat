@echo off

set "task=WitchHunt"
set "grader_name=grader"

g++ -std=gnu++20 -Wall -O2 -pipe -static -g -o "%task%.exe" "%grader_name%.cpp" "%task%.cpp"