#!/bin/bash
checkForCppUTestEnvVariable() {
	if [ -z "$CPP_U_TEST" ] ; then
	   echo "CPP_U_TEST not set"
	   exit 1
	fi
	if [ ! -d "$CPP_U_TEST" ] ; then
	   echo "CPP_U_TEST not set to a directory"
	   exit 2
	fi
}
