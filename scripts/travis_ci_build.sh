#!/bin/bash
# Script run in the travis CI

if [ $BUILDTOOL = "autotools" ]; then ../configure && echo "CONFIGURATION DONE. Compiling now." && make check_all; if [ $CXX = "gcc" ]; then echo "Release now"; fi; fi
if [ $BUILDTOOL = "cmake" ]; then cmake .. && make && ctest -V; fi
if [ $BUILDTOOL = "cmake" ]; then cmake .. -DC++11=ON && make && ctest -V; fi
if [ $BUILDTOOL = "cmake" ]; then cmake .. -DGMOCK=ON && make && ctest -V; fi
if [ $BUILDTOOL = "cmake" ]; then cmake .. -DGMOCK=ON -DC++11=ON && make && ctest -V; fi
if [ $BUILDTOOL = "cmake" ]; then cmake .. -DGMOCK=OFF -DREAL_GTEST=ON && make && ctest -V; fi
if [ $BUILDTOOL = "cmake" ]; then cmake .. -DGMOCK=OFF -DREAL_GTEST=ON -DC++11=ON && make && ctest -V; fi
