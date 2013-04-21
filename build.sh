wget https://googlemock.googlecode.com/files/gmock-1.6.0.zip
unzip gmock-1.6.0.zip
export GMOCK_HOME=../gmock-1.6.0
export GTEST_HOME=$GMOCK_HOME/gtest
cd gmock-1.6.0
./configure && make
cd ..
mkdir -p cpputest_build && cd cpputest_build
../configure && echo "CONFIGURATION DONE. Compiling now." && make check_all
