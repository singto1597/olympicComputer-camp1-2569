#!/bin/sh
set -e
g++ -std=c++17 -O2 -Wall -o cursed_seal grader.cpp cursed_seal.cpp
for i in 1 2; do
  ./cursed_seal < sample/sample$i.in > /tmp/cursed_seal_$i.out
  if diff -bB /tmp/cursed_seal_$i.out sample/sample$i.out > /dev/null; then echo "sample $i: OK"; else echo "sample $i: DIFFERENT"; fi
done
