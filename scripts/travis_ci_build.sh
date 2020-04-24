#!/bin/bash
# Script run in the travis CI
set -ex

if [[ "$CXX" == clang* ]]; then
    export CXXFLAGS="-stdlib=libc++"
fi

if [ "x$CPPUTEST_HOME" = "x" ] ; then
  export CPPUTEST_HOME=$TRAVIS_BUILD_DIR
fi

if [ "x$BUILD" = "xautotools" ]; then
    autoreconf -i ..
    ../configure
    echo "CONFIGURATION DONE. Compiling now."

    if [ "${TRAVIS_EVENT_TYPE}" = "cron" ]; then
        make check_all
    fi


    if [ "x$TRAVIS_OS_NAME" = "xosx" ]; then
        COPYFILE_DISABLE=1 make dist
        COPYFILE_DISABLE=1 make dist-zip
    else
        make dist
        make dist-zip
    fi
fi

if [ "x$BUILD" = "xcmake" ]; then
    BUILD_ARGS=("-DWERROR=ON")

    if [ -n "$CPP_STD" ]; then
        BUILD_ARGS+=("-DCMAKE_CXX_STANDARD=$CPP_STD")
    fi

    cmake --version
    cmake "${BUILD_ARGS[@]}" ..
    make
    ctest -V
fi

if [ "x$BUILD" = "xautotools_gtest" ]; then
    autoreconf -i ..
    ../configure

    make check_gtest
fi

if [ "x$BUILD" = "xcmake_gtest" ]; then
    pwd
    wget https://github.com/google/googletest/archive/release-1.6.0.zip -O gtest-1.6.0.zip  && unzip gtest-1.6.0.zip;
    wget https://github.com/google/googlemock/archive/release-1.6.0.zip -O gmock-1.6.0.zip  && unzip gmock-1.6.0.zip;
    unzip gtest-1.6.0.zip -d $TRAVIS_BUILD_DIR
    unzip gmock-1.6.0.zip -d $TRAVIS_BUILD_DIR
    cd $TRAVIS_BUILD_DIR
    mv googletest-release-1.6.0 googlemock-release-1.6.0/gtest
    cd googlemock-release-1.6.0
    autoreconf -i; ./configure CXXFLAGS=-DGTEST_USE_OWN_TR1_TUPLE=1 && make
    cd -
    export GMOCK_HOME=$TRAVIS_BUILD_DIR/googlemock-release-1.6.0
    export GTEST_HOME=$TRAVIS_BUILD_DIR/googlemock-release-1.6.0/gtest
    cmake . -DGMOCK=ON
    make
    ctest -V
fi

if [ "x$BUILD" = "xtest_report" ]; then
    if [ "x$TRAVIS_OS_NAME" = "xosx" ]; then
        brew install ant
    fi
    autoreconf -i ..
    ../configure
    make check
    ./CppUTestTests -ojunit
    ./CppUTestExtTests -ojunit
    cp ../scripts/generate_junit_report_ant.xml .
    ant -f generate_junit_report_ant.xml
fi

if [ "x$BUILD" = "xcmake_coverage" ]; then
    pip install --user cpp-coveralls

    cmake .. -DCMAKE_BUILD_TYPE=Debug -DC++11=ON -DCOVERAGE=ON -DLONGLONG=ON
    make
    ctest

    coveralls -b . -r .. -i "src" -i "include" --gcov-options="-lbc" || true
fi

if [ "x$BUILD" = "xautotools_cmake_install_test" ]; then
    autoreconf -i ..
    ../configure
    rm -rf install_autotools
    mkdir -p install_autotools
    make DESTDIR=install_autotools install

    cmake ..
    rm -rf install_cmake
    mkdir -p install_cmake
    make DESTDIR=install_cmake install

    export INSTALL_DIFF=`diff -rq install_autotools install_cmake | grep -v CppUTestGeneratedConfig.h | grep -v libCppUTest.a | grep -v libCppUTestExt.a`
    if [ "x$INSTALL_DIFF" != "x" ]; then
        echo "FAILED: CMake install and Autotools install is not the same!\n"
        echo "Difference\n"
        echo "-------------------------------\n"
        echo "$INSTALL_DIFF"
        echo "-------------------------------\n"
        exit 1;
    fi

    # Hack: autotools cannot make CMake package. We cached and copied them. Here we check they are still the same
    for cmakefile in CppUTestConfig.cmake CppUTestConfigVersion.cmake CppUTestTargets-relwithdebinfo.cmake CppUTestTargets.cmake; do
      diff install_autotools/usr/local/lib/CppUTest/cmake/$cmakefile  install_cmake/usr/local/lib/CppUTest/cmake/$cmakefile || exit 1
    done
fi

if [ "x$BUILD" = "xdocker_ubuntu_autotools" ]; then
    $CPPUTEST_HOME/scripts/create_docker_images_and_containers ubuntu
    docker start -i cpputest_ubuntu
fi

if [ "x$BUILD" = "xdocker_ubuntu_gcc10" ]; then
    $CPPUTEST_HOME/scripts/create_docker_images_and_containers gcc10
    docker start -i cpputest_gcc10
fi

if [ "x$BUILD" = "xdocker_dos" ]; then
    $CPPUTEST_HOME/scripts/create_docker_images_and_containers dos
    docker start -i cpputest_dos
fi

if [ "x$BUILD" = "xmake_dos" ]; then
    git clone https://github.com/cpputest/watcom-compiler.git watcom
    export PATH=$PATH:$PWD/watcom/binl
    export WATCOM=$PWD/watcom
    export CC=wcl
    export CXX=wcl
    $CC --version
    make -f $CPPUTEST_HOME/platforms/Dos/Makefile
    $CPPUTEST_HOME/platforms/Dos/alltests.sh
 fi

