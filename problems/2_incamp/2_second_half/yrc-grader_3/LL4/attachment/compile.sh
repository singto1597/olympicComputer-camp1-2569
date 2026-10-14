#!/bin/sh
set -e
g++ -std=c++17 -O2 -Wall -o reverse_bridge grader.cpp reverse_bridge.cpp
for i in 1 2; do
  ./reverse_bridge < sample/sample$i.in > /tmp/reverse_bridge_$i.out
  if diff -bB /tmp/reverse_bridge_$i.out sample/sample$i.out > /dev/null; then echo "sample $i: OK"; else echo "sample $i: DIFFERENT"; fi
done
