#!/bin/bash
# Script run in the travis CI
set -ex

FILE="./test-suite.log"

if [ -f $FILE ]; then
    cat $FILE
else
    echo "$FILE not found."
fi
