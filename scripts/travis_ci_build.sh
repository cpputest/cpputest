#!/bin/bash
# Script run in the travis CI
set -ex

if [ "x$BUILD" = "xautotools" ]; then
    autoreconf -i ..
    ../configure
    echo "CONFIGURATION DONE. Compiling now."
    make check_all

    make dist
    make dist-zip

    if [ "x$CXX" = "xg++" ]; then
      echo "Deploy please"
#      gem install travis_github_deployer
#      cd .. && travis_github_deployer -v || exit 1
    fi;
fi

if [ "x$BUILD" = "xcmake" ]; then
    cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE
    make
    ctest -V

    if [ "x$CXX" != "xg++" ]; then
        cmake .. -DC++11=ON -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE
        make
        ctest -V
    fi
fi

if [ "x$BUILD" = "xautotools_gtest" ]; then
    autoreconf -i ..
    ../configure

    make check_gtest
fi

if [ "x$BUILD" = "xcmake_gtest" ]; then
    wget https://googlemock.googlecode.com/files/gmock-1.6.0.zip
    unzip gmock-1.6.0.zip -d $TRAVIS_BUILD_DIR
    cd $TRAVIS_BUILD_DIR/gmock-1.6.0
    ./configure && make
    cd -

    export GMOCK_HOME=$TRAVIS_BUILD_DIR/gmock-1.6.0
    export GTEST_HOME=$TRAVIS_BUILD_DIR/gmock-1.6.0/gtest
    cmake .. -DGMOCK=ON
    make
    ctest -V
fi

if [ "x$BUILD" = "xtest_report" ]; then
    autoreconf -i ..
    ../configure
    make check
    ./CppUTestTests -ojunit
    ./CppUTestExtTests -ojunit
    cp ../scripts/generate_junit_report_ant.xml .
    ant -f generate_junit_report_ant.xml
fi

if [ "x$BUILD" = "xcmake_coverage" ]; then
    pip install cpp-coveralls --user `whoami`

    cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -DCOVERAGE=ON -DLONGLONG=ON
    make
    ctest

    coveralls -b . -r .. -i "src" -i "include" --gcov-options="-lbc" || true
fi

if [ "x$BUILD" = "xmake_dos" ]; then
    wget ftp://ftp.openwatcom.org/pub/open-watcom-c-linux-1.9 -O /tmp/watcom.zip
    mkdir -p watcom && unzip -aqd watcom /tmp/watcom.zip && chmod -R +x watcom/binl
    export PATH=$PATH:$PWD/watcom/binl
    export WATCOM=$PWD/watcom
    export CPPUTEST_HOME=$TRAVIS_BUILD_DIR
    export CC=wcl
    export CXX=wcl
    $CC --version
    make -f ../platforms/Dos/Makefile || exit 1
    ../platforms/Dos/alltests.sh || exit 1
 fi

