#!/bin/bash

PROJECT_NAME=$1
CODE_LEGAL_PROJECT_NAME=$(echo $PROJECT_NAME | sed 's/-/_/g')
TEMPLATE_DIR=${CPPUTEST_HOME}/scripts/templates
ORIGINAL_DIR=$(pwd)

if [ -e ${PROJECT_NAME} ] ; then
	echo "The directory ${PROJECT_NAME} already exists"
	exit 1;
fi

echo "Copy template project"
cp -R ${TEMPLATE_DIR}/ProjectTemplate ${PROJECT_NAME}
find ${PROJECT_NAME} -name \.svn | xargs rm -rf

cd ${PROJECT_NAME}

ls

changeProjectName() {
    echo Change Name $1/Project$2 to ${CODE_LEGAL_PROJECT_NAME}$2
    sed "-e s/Project/${CODE_LEGAL_PROJECT_NAME}/g" $1/Project$2 | tr -d "\r" >$1/${CODE_LEGAL_PROJECT_NAME}$2
    rm $1/Project$2 
}

changeProjectName . Makefile
changeProjectName src/util BuildTime.cpp 
changeProjectName include/util BuildTime.h 
changeProjectName tests/util BuildTimeTest.cpp 
mv ${CODE_LEGAL_PROJECT_NAME}Makefile Makefile

cd ${ORIGINAL_DIR}
echo "You might want to modify the path for CPPUTEST_HOME in the Makefile."
