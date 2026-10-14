#!/bin/sh
set -e
g++ -std=c++17 -O2 -Wall -o hero_caravan grader.cpp hero_caravan.cpp
for i in 1 2; do
  ./hero_caravan < sample/sample$i.in > /tmp/hero_caravan_$i.out
  if diff -bB /tmp/hero_caravan_$i.out sample/sample$i.out > /dev/null; then echo "sample $i: OK"; else echo "sample $i: DIFFERENT"; fi
done
