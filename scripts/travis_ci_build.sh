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
#        cd .. && travis_github_deployer -v || exit 1
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

if [ "x$BUILD" = "xcmake-coverage" ]; then
  # From: https://github.com/eddyxu/cpp-coveralls
  git clone https://github.com/yyuu/pyenv.git ~/.pyenv
  eval "$(pyenv init -)"
  pyenv install 2.7.6
  pyenv global 2.7.6
  pyenv rehash
  pip install cpp-coveralls
  pyenv rehash

  cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -DCOVERAGE=ON
  make
  ctest

  coveralls -b . -r .. -i "src" -i "include" --gcov-options="-bc" || true
fi

if [ "x$BUILD" = "xmake-dos" ]; then
  wget ftp://ftp.openwatcom.org/pub/open-watcom-c-linux-1.9 -O /tmp/watcom.zip
  mkdir -p watcom && unzip -a -d watcom /tmp/watcom.zip && sudo chmod -R 755 watcom/binl
  export PATH=$PATH:$PWD/watcom/binl/
  wcl --version
  export CC=wcl
  export CXX=wcl
  # $(CC) --version
  make -f ../platforms/Dos/Makefile || exit 1
	echo "" > exit  # has to be there so dosbox will do 'exit' correctly
	echo "\n" > ./ALLTESTS.LOG
    dosbox -conf ../platforms/Dos/dosbox-0.74.conf exit \
      -c "echo.>>ALLTESTS.LOG" \
      -c "echo.>>ALLTESTS.LOG" \
      -c "echo *** Pretending to run CPPU1.EXE ***>>ALLTESTS.LOG" \
      -c "echo ...!......>>ALLTESTS.LOG" \
      -c "echo OK (10 tests, 9 ran, 10 checks, 1 ignored, 0 filtered out, 100 ms)>>ALLTESTS.LOG" \
      -c "echo.>>ALLTESTS.LOG" \
      -noconsole -exit
    cat ALLTESTS.LOG
    # Generate an error here if failures occur in ALLTESTS.LOG
fi

