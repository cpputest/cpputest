#/bin/bash
# Set this variable when you make without this script
export CPP_U_TEST=$(pwd)/CppUTest
 
make clean depend all
make -C example clean depend all
make -s  test

