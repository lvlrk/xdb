#!/bin/sh

CXX="g++"
CXXFLAGS="-Wno-enum-compare -Wno-narrowing -O2 -c"
LDFLAGS="-lraylib -lfmt"
TARGET="xdb"

rm -f $TARGET *.o
$CXX $CXXFLAGS src/*.cpp
$CXX $LDFLAGS -o $TARGET *.o
./$TARGET "$@"
