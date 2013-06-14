#!/bin/bash
# Script run in the travis CI

if [ "x$BUILDTOOL" = "xautotools" ]; then 
	../configure && echo "CONFIGURATION DONE. Compiling now." && 
	make check_all && 
	if [ $CXX = "gcc" ]; then 
		echo "Release now"; 
	fi; 
fi

if [ "x$BUILDTOOL" = "cmake" ]; then 
	cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE && 
	make && 
	ctest -V; 
fi

if [ "x$BUILDTOOL" = "cmake" ]; then 
	cmake .. -DGMOCK=ON && 
	make && 
	ctest -V; 
fi

if [ "x$BUILDTOOL" = "cmake" ]; then 
	cmake .. -DGMOCK=OFF -DREAL_GTEST=ON && 
	make && 
	ctest -V; 
fi
