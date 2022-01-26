#!/bin/bash -eu

$CXX $CXXFLAGS -std=c++17 $SRC/fuzzz/fuzz.cpp -o $OUT/fuzzz/fuzzer
