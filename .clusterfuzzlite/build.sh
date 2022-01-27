#!/bin/bash -eu

$CXX $CXXFLAGS -std=c++17 $SRC/fuzz/fuzzer.cpp $LIB_FUZZING_ENGINE -o $OUT/fuzzer
