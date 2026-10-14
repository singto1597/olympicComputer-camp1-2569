#!/bin/sh
set -e
g++ -std=c++17 -O2 -Wall -o twin_scroll grader.cpp twin_scroll.cpp
for i in 1 2; do
  ./twin_scroll < sample/sample$i.in > /tmp/twin_scroll_$i.out
  if diff -bB /tmp/twin_scroll_$i.out sample/sample$i.out > /dev/null; then echo "sample $i: OK"; else echo "sample $i: DIFFERENT"; fi
done
