#!/bin/bash
for TESTS in CPPU1 CPPU2 CPPUEXT1 CPPUEXT2 CPPUEXT3 CPPUEXT4
do
    printf "Running ${TESTS}.EXE inside DOSBox...\n"
    dosbox -conf ../platforms/Dos/dosbox-0.74.conf exit \
    -c "echo.>>ALLTESTS.LOG" \
    -c "echo *** ${TESTS}.EXE *********************************>>ALLTESTS.LOG" \
    -c "${TESTS}.EXE>>ALLTESTS.LOG" \
    -noconsole -exit || exit 1
done
printf "\n" >>ALLTESTS.LOG
cat ALLTESTS.LOG
[ -z "`cat ALLTESTS.LOG|grep Failure`" ] || { printf "There were failed tests\n"; exit 1; }
