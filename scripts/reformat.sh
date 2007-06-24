#!/bin/bash

set -e
find . -name "*.cpp" -o -name "*.h" |
    while read name; do
        tmpfile=${name}.tmp
        astyle --convert-tabs --style=gnu <"${name}" > "${tmpfile}"
        mv "${tmpfile}" "${name}"
    done
    
