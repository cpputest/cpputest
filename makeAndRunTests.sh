#/bin/bash
# Set this variable when you make without this script
export CPP_U_TEST=$(pwd)

make clean all

make -C examples clean all

