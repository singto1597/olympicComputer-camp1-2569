#!/bin/sh
set -eu
g++ -std=c++17 -O2 -Wall -Wextra -pedantic grader.cpp n_pens.cpp -o n_pens
