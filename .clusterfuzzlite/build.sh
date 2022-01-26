#!/bin/bash -eu

$CXX $CXXFLAGS -std=c++17 $SRC/fuzzz/fuzz.cpp $LIB_FUZZING_ENGINE -o $OUT/fuzzzer
