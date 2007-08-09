The New*.sh scripts are helpful for creating the initial files for a new class...
	NewClass.sh - for TTDing a new C++ class
	NewInterface.sh - for TDDing a new interface along with its Mock
	NewCModule.sh - for TDDing a C module

To use NewClass for example:
	cd to the directory where your makefile is
	NewClass SomeClass
	
The script gets you ready for TDD and saves a lot of tedius typing
	1) Creates SomeClass.h SomeClass.cpp SomeClassTest.cpp
		with the class and test essentials in place
		(If the SomeClass.* file already exists, no file is generated)
	2) Adds an entry to AllTests.h so the linker can find your test
	3) Makes an entry into the .dependencies file
	4) Updates the makefile (if there is one and it has an "OBJS =" line)

Run InstallScripts.sh to enable NewClass, NewInterface, NewCModule, NewVCModule
support scripts.  Like this:
	./InstallScripts.sh
	
You probably have to add the execute privilege to the shell scripts.
Like this:
	chmod *.sh

These scripts are written in bash.

