#/bin/bash
# Set this variable when you make without this script
export CPP_U_TEST=$(pwd)
 
make clean all gcov
make -C examples clean all gcov

