#!/bin/sh
set -e
g++ -std=c++17 -O2 -Wall -o demon_ring grader.cpp demon_ring.cpp
for i in 1 2; do
  ./demon_ring < sample/sample$i.in > /tmp/demon_ring_$i.out
  if diff -bB /tmp/demon_ring_$i.out sample/sample$i.out > /dev/null; then echo "sample $i: OK"; else echo "sample $i: DIFFERENT"; fi
done
