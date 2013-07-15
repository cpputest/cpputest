#!/bin/bash
# Script run in the travis CI

if [ "x$BUILDTOOL" = "xautotools" ]; then 
	../configure || exit 1
	echo "CONFIGURATION DONE. Compiling now."
	make check_all || exit 1
	if [ "x$CXX" = "xg++" ]; then 
		echo " " >> ../README
		cd .. && travis_github_deployer || exit 1
	fi; 
fi

if [ "x$BUILDTOOL" = "xcmake" ]; then 
	cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE || exit 1
	make || exit 1
	ctest -V || exit 1
fi

if [ "x$BUILDTOOL" = "xcmake" ]; then 
	cmake .. -DGMOCK=ON || exit 1
	make || exit 1
	ctest -V || exit 1
fi

if [ "x$BUILDTOOL" = "xcmake" ]; then 
	cmake .. -DGMOCK=OFF -DREAL_GTEST=ON || exit 1
	make || exit 1
	ctest -V || exit 1
fi
