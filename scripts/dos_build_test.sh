#!/usr/bin/env bash
set -euo pipefail
# get current dir even if the script is sourced
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

export CPPUTEST_HOME="$DIR/../"
export WATCOM=./watcom
export PATH=$PATH:$WATCOM/binl

if [ ! -d "$WATCOM" ]; then
    git clone https://github.com/cpputest/watcom-compiler.git "$WATCOM"
fi

make -f "$CPPUTEST_HOME/platforms/Dos/Makefile" clean
make -f "$CPPUTEST_HOME/platforms/Dos/Makefile"
make -f "$CPPUTEST_HOME/platforms/Dos/Makefile" test
