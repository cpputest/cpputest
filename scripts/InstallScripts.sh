#!/bin/bash -x
FirstLetter=${0:0:1}
if [[ $FirstLetter == "/" ]] ;  then
	CPPUTEST_HOME=${0%/scripts/*}
else
	CPPUTEST_HOME="$(pwd)/${0%/scripts/*}"
fi

EXE_DIR=${EXE_DIR:-/usr/local/bin}
test -f ${EXE_DIR} || mkdir -p ${EXE_DIR}

NEW_SCRIPTS="NewCIoDriver NewClass NewInterface NewCModule NewCmiModule NewProject NewLibrary NewPackageDirs NewCInterface NewCFunction NewHelp"


for file in $NEW_SCRIPTS ; do
   rm -f ${EXE_DIR}/${file}
   ln -s ${CPPUTEST_HOME}/scripts/${file}.sh ${EXE_DIR}/${file}
   chmod a+x ${EXE_DIR}/${file}
done
