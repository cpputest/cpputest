#!/bin/bash
# run from the builds directory

listGccMacVersions()
{
    port select --list gcc
}

if [ -z "$1" ]; then
    echo "usage: $0 gcc-version\navailable versions:" 1>&2
    listGccMacVersions
    exit 1
fi

gcc_v=$1

setGccMac()
{
    sudo port select --set gcc $1
}

setGccLinux()
{
     echo "Not implemented for Linux"
     exit 1
#    if [ ! -f "/usr/bin/gcc-$1" ] || [ ! -f "/usr/bin/g++-$1" ]; then
#       echo "no such version gcc/g++ installed" 1>&2
#        exit 1
#    fi

#    update-alternatives --set gcc "/usr/bin/gcc-$1"
#    update-alternatives --set g++ "/usr/bin/g++-$1"
}

if [ "$(uname)" == "Darwin" ]; then
    setGccMac $gcc_v
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    setGccLinux $gcc_v
else
    echo "can't determine OS"
    exit 1
fi

pushd ..
cpputest_home=$(pwd)
popd

mkdir -p $gcc_v
cd $gcc_v
../../configure
make
make check
#these links keep compatibility with makefile worker
ln -s $cpputest_home/include include
ln -s $cpputest_home/build build
ln -s $cpputest_home/scripts scripts
