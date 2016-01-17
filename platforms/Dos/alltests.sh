#!/bin/bash
printf "" > exit  # so dosbox will run 'external' command 'exit' correctly
printf "" >ALLTESTS.LOG
for test in `ls *.EXE`; do
    kb=`ls -lsk ${test} | awk '{print $1}'`
    printf "Running ${test} (${kb}k) inside DOSBox...\n"
    dosbox -conf ../platforms/Dos/dosbox-0.74.conf exit \
    -c "echo *** ${test} (${kb}k) *****************************>>ALLTESTS.LOG" \
    -c "${test}>>ALLTESTS.LOG" \
    -noconsole -exit || exit 1
done
printf "\n"
[ "`cat ALLTESTS.LOG`" ] || { printf "No tests to run!\n"; exit 1; }
[ "`cat ALLTESTS.LOG|grep checks`" ] || { printf "No test output!\n"; exit 1; }
cat ALLTESTS.LOG
[ -z "`cat ALLTESTS.LOG|grep Failure`" ] || { printf "Failed tests!\n"; exit 1; }
