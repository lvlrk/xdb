#!/bin/sh

rm -f xdb *.o
g++ -c -O2 -Wno-enum-compare -Wno-narrowing src/*.cpp
g++ -lraylib -lfmt -o xdb *.o
./xdb "$@"
