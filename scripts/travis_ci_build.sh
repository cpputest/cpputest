#!/bin/bash
# Script run in the travis CI

if [ "x$BUILDTOOL" = "xautotools" ]; then 
	../configure || exit 1
	echo "CONFIGURATION DONE. Compiling now."
	make check_all || exit 1
	i
	if [ $CXX = "gcc" ]; then 
		echo "Release now"; 
	fi; 
fi

if [ "x$BUILDTOOL" = "cmake" ]; then 
	cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE || exit 1
	make || exit 1
	ctest -V || exit 1
fi

if [ "x$BUILDTOOL" = "cmake" ]; then 
	cmake .. -DGMOCK=ON || exit 1
	make || exit 1
	ctest -V || exit 1
fi

if [ "x$BUILDTOOL" = "cmake" ]; then 
	cmake .. -DGMOCK=OFF -DREAL_GTEST=ON || exit 1
	make || exit 1
	ctest -V || exit 1
fi
