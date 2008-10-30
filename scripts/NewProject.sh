#!/bin/bash

PROJECT=$1
TEMPLATE_DIR=${CPP_U_TEST}/scripts/templates
ORIGINAL_DIR=$(pwd)

if [ -e ${PROJECT} ] ; then
	echo "The directory ${PROJECT} already exists"
	exit 1;
fi

echo "Copy template project"
cp -R ${TEMPLATE_DIR}/ProjectTemplate ${PROJECT}
find ${PROJECT} -name \.svn | xargs rm -rf

cd ${PROJECT}

ls

changeProjectName() {
    echo Change Name $1/Project$2 to ${PROJECT}$2
    sed "-e s/Project/${PROJECT}/g" $1/Project$2 | tr -d "\r" >$1/${PROJECT}$2
    rm $1/Project$2 
}

changeProjectName . Makefile
changeProjectName src/util BuildTime.cpp 
changeProjectName include/util BuildTime.h 
changeProjectName tests/util BuildTimeTest.cpp 
mv ${PROJECT}Makefile Makefile

cd ${ORIGINAL_DIR}
echo "You might want to modify the path for CPP_U_TEST in the Makefile."
