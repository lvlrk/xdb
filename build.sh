#!/bin/sh

rm -f xdb *.o
g++ -c -Wno-enum-compare -Wno-narrowing src/*.cpp
g++ -lraylib -lfmt -o xdb *.o
./xdb "$@"
