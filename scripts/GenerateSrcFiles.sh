#!/bin/bash -x
#$1 is the class name
#$2 is the template root file name
#$3 is the kind of file to create (c or cpp)
#$4 is the Mock if a mock version should be created

className=$x

#Test for env var set.
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
if [ -f .dependencies ]
  then
    echo Updating .dependencies
    echo "$oFile: $hFile $srcFile $mockFile" >>.dependencies
    echo "$testOFile: $hFile $testFile $mockFile" >>.dependencies
fi

makefileUpdated=NO
updateMakefile() {
	if [ -f Makefile ]
	  then
		if grep -q " $1" $2
		  then
		    echo " $1 is in Makfile already"
		    cp $2 $3
		  else
		    echo "Adding $1 to Makefile"
		    sed -e "s/OBJS =/&\\\@\ $1/" <$2  | tr "@" "\n" >$3
		    makefileUpdated=YES    
		fi
	fi
}

if grep -q "OBJS =" Makefile
  then
	updateMakefile $oFile Makefile Makefile.temp1
	updateMakefile $testOFile Makefile.temp1 Makefile.temp2
  else
    echo "Can't add .o files, no OBJS symbol to update"
fi
    
if [ "$makefileUpdated" == "YES" ]
  then
    echo "Makefile was updated"
	if [ -f Makefile.bak ] 
	then
	  rm Makefile.bak
	fi
	echo Saving old makefile in Makefile.bak
	mv Makefile Makefile.bak
	mv Makefile.temp2 Makefile
	rm -f Makefile.temp1
fi

AllTestsImport=AllTests.h
echo "Adding IMPORT_TEST_GROUP($className); to $AllTestsImport"
echo "IMPORT_TEST_GROUP($className);" >>$AllTestsImport
