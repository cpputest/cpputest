#!/bin/bash
printf "" > exit  # so dosbox will run 'external' command 'exit' correctly
printf "" >ALLTESTS.LOG
for test in `ls *.EXE`; do
    bytes=`ls -l ${test} | awk '{print $6}'`
    printf "Running ${test} (${bytes} bytes) inside DOSBox...\n"
    dosbox -conf ../platforms/Dos/dosbox-0.74.conf exit \
    -c "echo *** ${test} (${bytes} bytes) *********************>>ALLTESTS.LOG" \
    -c "${test}>>ALLTESTS.LOG" \
    -noconsole -exit || exit 1
done
printf "\n"
[ "`cat ALLTESTS.LOG`" ] || { printf "There were no tests to run!\n"; exit 1; }
cat ALLTESTS.LOG
[ -z "`cat ALLTESTS.LOG|grep Failure`" ] || { printf "There were failed tests!\n"; exit 1; }
