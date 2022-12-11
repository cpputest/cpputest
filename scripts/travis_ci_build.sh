#!/bin/bash
# Script run in the travis CI
set -ex

if [ "x$CPPUTEST_HOME" = "x" ] ; then
  export CPPUTEST_HOME=$TRAVIS_BUILD_DIR
fi

if [ "x$BUILD" = "xautotools" ]; then
    autoreconf -i ..
    ../configure
    make tdd
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

    # Hack: autotools cannot make CMake package. We cached and copied them. Here we check they are still the same
    for cmakefile in CppUTestConfig.cmake CppUTestConfigVersion.cmake CppUTestTargets-relwithdebinfo.cmake CppUTestTargets.cmake; do
      cat install_autotools/usr/local/lib/CppUTest/cmake/$cmakefile
      cat install_cmake/usr/local/lib/CppUTest/cmake/$cmakefile
      diff -Bw install_autotools/usr/local/lib/CppUTest/cmake/$cmakefile  install_cmake/usr/local/lib/CppUTest/cmake/$cmakefile || exit 1
    done

    export INSTALL_DIFF=`diff -rwBq install_autotools install_cmake  -X CppUTestGeneratedConfig.h -X libCppUTest.a -X libCppUTestExt.a`
    if [ "x$INSTALL_DIFF" != "x" ]; then
        echo "FAILED: CMake install and Autotools install is not the same!\n"
        echo "Difference\n"
        echo "-------------------------------\n"
        echo "$INSTALL_DIFF"
        echo "-------------------------------\n"
        exit 1;
    fi
fi

if [ "x$BUILD" = "xdocker_ubuntu_autotools" ]; then
    $CPPUTEST_HOME/scripts/create_docker_images_and_containers ubuntu
    docker start -i cpputest_ubuntu
fi

if [ "x$BUILD" = "xdocker_ubuntu_dos" ]; then
    $CPPUTEST_HOME/scripts/create_docker_images_and_containers dos
    docker start -i cpputest_dos
fi

if [ "x$BUILD" = "xautotools_dist" ]; then
    autoreconf -i ..
    ../configure

    if [ "x$TRAVIS_OS_NAME" = "xosx" ]; then
        COPYFILE_DISABLE=1 make dist VERSION=latest
        COPYFILE_DISABLE=1 make dist-zip VERSION=latest
    else
        make dist VERSION=latest
        make dist-zip VERSION=latest
    fi
fi

if [ "x$BUILD" = "xautotools_install_and_test_examples" ]; then
    autoreconf -i ..
    ../configure

    make tdd
    sudo make install
    make -C $CPPUTEST_HOME/examples -f $CPPUTEST_HOME/examples/Makefile_ExamplesWithCppUTestInstalled.mk
fi
