#!/bin/bash

checkForCppUTestToolsEnvVariable() {
	if [ -z "$CPPUTEST_HOME" ] ; then
	   echo "CPPUTEST_HOME not set. You must set CPPUTEST_HOME to the top level CppUTest directory"
	   exit 1
	fi
	if [ ! -d "$CPPUTEST_HOME" ] ; then
	   echo "CPPUTEST_HOME not set to a directory.  You must set CPPUTEST_HOME to the top level CppUTest directory"
	   exit 2
	fi
}

checkForCppUTestToolsEnvVariable

printf "" > exit  # so dosbox will run 'external' command 'exit' correctly
printf "" >ALLTESTS.LOG
for test in `ls *.EXE`; do
    kb=`ls -lsk ${test} | awk '{print $1}'`
    printf "Running ${test} (${kb}k) inside DOSBox...\n"
    dosbox -conf $CPPUTEST_HOME/platforms/Dos/dosbox-0.74.conf exit \
    -c "echo *** ${test} (${kb}k) *****************************>>ALLTESTS.LOG" \
    -c "${test}>>ALLTESTS.LOG" \
    -noconsole -exit 1>console_output 2>dosbox_error.log || { printf "error running command!\n"; cat dosbox_error.log;  exit 1; }
done
printf "\n"
[ "`cat ALLTESTS.LOG`" ] || { printf "No tests to run!\n"; exit 1; }
[ "`cat ALLTESTS.LOG|grep checks`" ] || { printf "No test output!\n"; exit 1; }
cat ALLTESTS.LOG
[ -z "`cat ALLTESTS.LOG|grep Failure`" ] || { printf "Failed tests!\n"; exit 1; }
