1. Unzip into <someDirectory>, resulting in
		<someDirectory>/CppUTest/	

   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT	
   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT	
   MAKE SURE <someDirectory> DOES NOT HAVE SPACES IN IT	
	
2. Build CppUTest and examples

2a. For unix/gcc (including cygwin)
 > cd <someDirectory>/CppUTest/cpputest_build
 > ../configure
 > make
 > make tdd   # This is to run the CppUTest unit tests
  
2b. For Microsoft Visual C++ V6
 Double click <someDirectory>/CppUTest/CppUTest.dsw
 Run without debugging, see the test results in the command window
 Exit MS Visual C++
 
 To run the examples: 
 Double click <someDirectory>/CppUTest/example/CppUTestExample.dsw
 Run without debugging, see the test results in the command window
 You should define the environment variable CPP_U_TEST to point to CppUTest
 to run these.
 
 NOTE: To create your own project, you need to have CppUTest and your project 
 compiled with the same compile and link settings

2c. For ARMCC from Keil MDK-ARM (building CppUTest library only)
 > make all -C platforms/armcc
 Please see README and Makefile in platforms/armcc to adjust options, default
 CPU architecture is ARM7TDMI and default CPU execution mode is THUMB.
 
3c. For Microsoft Visual Studio 2008
 Double click <someDirectory>/CppUTest/CppUTest.sln
 
 If Visual studio reports that the solution file was created with a
 newer version of Visual Studio, then try 3d
 
 Then press control-F5 to "Start without debugging"
 
 See CppUTest build and run its tests.
 
3d. For Older Microsoft Visual Studio .NET
 Double click <someDirectory>/CppUTest/CppUTest.dsw
 Allow VS.NET to convert the files by clicking "yes to all"
 Run without debugging, see the test results in the command window
 Exit MS VS.NET
 
 Allow VS.NET to convert the files by clicking "yes to all"
 Run without debugging, see the test results in the command window
 
 NOTE: To create your own project, you need to have CppUTest and your project 
 compiled with the same compile and link settings
 
4. to setup the support scripts. These scripts work in various unix systems 
and cygwin.  (these are quite handy)  If you are using windows
install some tool like cygwin, msys or MKSToolkit to run these scripts.
 > cd <someDirectory>/CppUTest
 > ./scripts/InstallScripts.sh

This command adds some symbolic links to /usr/local/bin, so you have 
to run it as root.

	sudo ./InstallScripts.sh
 
MSYS - http://www.mingw.org/msys.shtml
CYGWIN - http://www.cygwin.com/
MKSToolkit - http://mkstoolkit.com/
