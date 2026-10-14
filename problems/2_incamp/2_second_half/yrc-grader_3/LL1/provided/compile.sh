#!/bin/sh
set -e
g++ -std=c++17 -O2 -Wall -o jade_path grader.cpp jade_path.cpp
for i in 1 2; do
  ./jade_path < sample/sample$i.in > /tmp/jade_path_$i.out
  if diff -bB /tmp/jade_path_$i.out sample/sample$i.out > /dev/null; then echo "sample $i: OK"; else echo "sample $i: DIFFERENT"; fi
done
