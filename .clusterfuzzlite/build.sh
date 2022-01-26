#!/bin/bash -eu

echo "SRC: $SRC"
echo "OUT: $OUT"
echo "PWD: $PWD"

$CXX $CXXFLAGS -std=c++17 $SRC/fuzz.cpp -o $OUT/fuzzz/fuzzer
