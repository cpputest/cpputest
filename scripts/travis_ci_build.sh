#!/bin/bash
# Script run in the travis CI
set -e

if [ "x$BUILDTOOL" = "xautotools" ]; then
    autoreconf -i ..
    ../configure
    echo "CONFIGURATION DONE. Compiling now."
    make check_all

    # Special check for all the gtest versions.
    make check_gtest

    # One more normal build for generating test reports
    ../configure
    make check
    ./CppUTestTests -ojunit
    ./CppUTestExtTests -ojunit
    cp ../scripts/generate_junit_report_ant.xml .
    ant -f generate_junit_report_ant.xml

    make dist
    make dist-zip

    if [ "x$CXX" = "xg++" ]; then
      echo "Deploy please"
#        cd .. && travis_github_deployer -v || exit 1
    fi;
fi

if [ "x$BUILDTOOL" = "xcmake" ]; then
    cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE
    make || exit 1
    ctest -V || exit 1

    if [ "x$CXX" != "xg++" ]; then
        cmake .. -DC++11=ON -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE
        make || exit 1
        ctest -V || exit 1
    fi

    wget https://googlemock.googlecode.com/files/gmock-1.6.0.zip
    unzip gmock-1.6.0.zip -d $TRAVIS_BUILD_DIR
    cd $TRAVIS_BUILD_DIR/gmock-1.6.0
    ./configure && make
    cd -

    export GMOCK_HOME=$TRAVIS_BUILD_DIR/gmock-1.6.0
    export GTEST_HOME=$TRAVIS_BUILD_DIR/gmock-1.6.0/gtest
    cmake .. -DGMOCK=ON || exit 1
    make || exit 1
    ctest -V || exit 1
fi

