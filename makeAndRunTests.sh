#/bin/bash
# Set this variable when you make without this script
export CPPUTEST_HOME=$(pwd)
export ENABLE_EXTENSIONS=Y

make clean all

make -C examples clean all

