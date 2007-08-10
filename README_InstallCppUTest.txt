1. Unzip into <someDirectory>, resulting in
		<someDirectory>/CppUTest/	

   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT	
   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT	
   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT	
	
2. Set the environement variable CPP_U_TEST to 
		<someDirectory>/CppUTest/
		
	Use forward slashes even in windows (windows does not care, but cygwin
	will care if you use it.  Also <someDirectory> should not have spaces in it.
	
	Note: makeCppUTest.sh file (builds the test harness and exercise project)
	will temporarily set this variable.
	
3. Buld CppUTest and examples

3a. For unix/gcc (including cygwin)
Use the command line to execute makeAndRunTests.sh
 > cd <someDirectory>/CppUTest
 > chmod +x *.sh
 > ./makeAndRunTests.sh
 
 NOTE: the builds run their unit tests to verify the build
 NOTE: Older cygwin installs may report memory leaks.

3b. For Microsoft Visual C++ V6
 Double click <someDirectory>/CppUTest/CppUTest.dsw
 Run without debugging, see the test results in the command window
 Exit MS Visual C++
 
 Double click <someDirectory>/CppUTest/example/CppUTestExample.dsw
 Run without debugging, see the test results in the command window
 
 NOTE: To create your own project, you need to hae CppUTest and your project 
 compiled with the same compile and link settings
 
 NOTE: VC6 test runs may report memory leaks.

3c. For Microsoft Visual Studio .NET
 Double click <someDirectory>/CppUTest/CppUTest.dsw
 Allow VS.NET to conver the files by clicking "yes to all"
 Run without debugging, see the test results in the command window
 Exit MS VS.NET
 
 Allow VS.NET to conver the files by clicking "yes to all"
 Run without debugging, see the test results in the command window
 
 NOTE: To create your own project, you need to hae CppUTest and your project 
 compiled with the same compile and link settings
 
 NOTE: VC6 test runs may report memory leaks.

4. Setup the support scripts. These scripts work in various unix systems 
and cygwin.  (these are quite handy)  If you are using windows
install some tool like cygwin or msys so you have scripting
 > cd <someDirectory>/CppUTest/CppSourceTemplates
 > ./InstallScripts.sh

This command adds some symbolic links to /usr/local/bin, so you have 
to run it as root.

	sudo ./InstallScripts.sh
 
