The New*.sh scripts are helpful for creating the initial files for a new class...
	NewClass.sh - for TTDing a new C++ class
	NewInterface.sh - for TDDing a new interface along with its Mock
	NewCModule.sh - for TDDing a C module

Using NewClass for example:
	cd to the directory where your makefile is
	NewClass SomeClass
	
The script gets you ready for TDD and saves a lot of tedious typing
	1) Creates SomeClass.h SomeClass.cpp SomeClassTest.cpp
		with the class and test essentials in place
		(If the SomeClass.* file already exists, no file is generated)
	2) Adds an entry to AllTests.h so
	    This file could be used to create separate test executables each with
	    a subset of tests.  You would do this if tests cannot be run all at once.
	    In this case the tests would have to be put into a library.  AllTests.h
	    (or a subset of AllTests) would be included in your test main, 
	    and then you link with the test library

Run InstallScripts.sh to enable NewClass, NewInterface, NewCModule
support scripts.  Like this:
	./InstallScripts.sh
	
You might have to add the execute privilege to the shell scripts.
Like this:
	chmod *.sh

These scripts are written in bash.

