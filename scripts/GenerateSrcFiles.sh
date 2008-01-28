#!/bin/bash -x
#$1 is the class name
#$2 is the template root file name
#$3 is the kind of file to create (c or cpp)
#$4 is the Mock if a mock version should be created

className=$1

#Test for env var set.
checkForCppUTestToolsEnvVariable() {
	if [ -z "$CPP_U_TEST" ] ; then
	   echo "CPP_U_TEST not set"
	   exit 1
	fi
	if [ ! -d "$CPP_U_TEST" ] ; then
	   echo "CPP_U_TEST not set to a directory"
	   exit 2
	fi
}

checkForCppUTestToolsEnvVariable

templateRootName=$2
srcSuffix=$3
testSuffix=Test

#CPP_SOURCE_TEMPLATES can point to templates you write
#identify the template files
if [ "$CPP_SOURCE_TEMPLATES" == "" ]
  then
    TEMPLATE_DIR=$CPP_U_TEST/scripts/templates
  else
    TEMPLATE_DIR=$CPP_SOURCE_TEMPLATES
fi
  
templateHFile=$TEMPLATE_DIR/$templateRootName.h
templateSrcFile=$TEMPLATE_DIR/$templateRootName.$srcSuffix
if [ "$4" == "Mock" ] ; then
  templateTestFile=$TEMPLATE_DIR/Interface$testSuffix.cpp
else
  templateTestFile=$TEMPLATE_DIR/$templateRootName$testSuffix.cpp
fi
templateMockFile=$TEMPLATE_DIR/Mock$templateRootName.h

#indentify the class and instance names
instanceName=$(echo $1 | cut -c1 | tr A-Z a-z)$(echo $1 | cut -c 2-) 
className=$(echo $1 | cut -c1 | tr a-z A-Z)$(echo $1 | cut -c 2-)

#identify the files being created
hFile=$className.h
srcFile=$className.$srcSuffix
testFile=$className$testSuffix.cpp
if [ "$4" == "Mock" ] ; then
  mockFile=Mock$className.h
else
  mockFile=
fi

echo Creating $hFile $srcFile $testFile $mockFile
sedCommands="-e s/aClassName/$instanceName/g -e s/ClassName/$className/g"
 
generateFileIfNotAlreadyThere() {
  if [ -e $2 ]
    then 
      echo "${2} already exists, skipping"
    else
      sed $sedCommands $1 | tr -d "\r" >$2
    fi
}

generateFileIfNotAlreadyThere $templateHFile $hFile
generateFileIfNotAlreadyThere $templateSrcFile $srcFile
generateFileIfNotAlreadyThere $templateTestFile $testFile
if [ "$4" == "Mock" ] ; then
  generateFileIfNotAlreadyThere $templateMockFile $mockFile
#  sed $sedCommands $templateMockFile | tr -d "\r" >$mockFile
fi

oFile=$className.o
testOFile=$className$testSuffix.o

AllTestsImport=AllTests.h
echo "Adding IMPORT_TEST_GROUP($className); to $AllTestsImport"
echo "IMPORT_TEST_GROUP($className);" >>$AllTestsImport
