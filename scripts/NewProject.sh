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
    echo Change Project Name $1/Project$2
    sed "-e s/Project/${PROJECT}/g" $1/Project$2 | tr -d "\r" >$1/${PROJECT}$2
    rm $1/Project$2 
}

changeProjectName . Makefile
changeProjectName src/Project BuildTime.cpp 
changeProjectName include/Project BuildTime.h 
changeProjectName tests/Project BuildTimeTest.cpp 
changeProjectName tests/Project NeedsTests.cpp 
mv ${PROJECT}Makefile Makefile

echo "Rename project directories"
for dir in src include tests ; do
    mv ${dir}/Project ${dir}/${PROJECT}
done

cd ${ORIGINAL_DIR}
#find  ${PROJECT} -name \*.bak | xargs rm -f
#make -C ${PROJECT} clean depend all
echo "You might want to modify the path for CPP_U_TEST in the Makefile."
