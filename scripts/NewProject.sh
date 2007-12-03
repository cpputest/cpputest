#!/bin/bash -x
TEMPLATE_DIR=${CPP_U_TEST}/scripts/templates
PROJECT=$1

if [ -e ${PROJECT} ] ; then
	echo "The directory ${PROJECT} already exists"
	exit 1;
fi

echo "Copy template project"
cp -R ${TEMPLATE_DIR}/ProjectTemplate ${PROJECT}
find $1 -name \.svn | xargs rm -rf

echo "Update to the new project name"
sedCommand="-e s/Project/${PROJECT}/g -i .bak"
pwd
ls $PROJECT
sed ${sedCommand} ${PROJECT}/Makefile
for dir in ${PROJECT}/AllTests ${PROJECT}/Project ; do
   sed ${sedCommand} ${dir}/*.*
   sed ${sedCommand} ${dir}/Makefile
done

sed -e s^CppUTestEnvironmentVariable^${CPP_U_TEST}^ -i .2.bak ${PROJECT}/Makefile

mv ${PROJECT}/Project ${PROJECT}/${PROJECT}
cd ${PROJECT}/${PROJECT}
for name in BuildTime.h BuildTime.cpp BuildTimeTest.cpp ; do
	mv Project${name} ${PROJECT}${name}
done
cd ../..

find  ${PROJECT} -name \*.bak | xargs rm -f
make -C ${PROJECT} clean depend all
echo "You might want to modify the path for CPP_U_TEST in the main Makefile."
