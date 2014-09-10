##Using the Eclipse Project

This project is for use on the Windows platform. These are the steps required for using it. (If you only wish to compile the CppUTest libraries, it is recommended that you use one ot the methods described at http://cpputest.github.io, rather than setting up Eclips)

###Preparation
####1. Install Cygwin
You need to have Cygwin installed, with at least the Gnu C compiler, make, autotools and libtool. Please follow the instructions in /README.md to build CppUTest from the Cygwin bash prompt.

####2. Set your PATH
Then, you need to add the path to your Cygwin binaries to your system path, e.g.
```dos
C:\<path_to_Cygwin>\Cygwin\bin
```

####3. Install Eclipse CDT
Copy Eclipse CDT (Kepler) to your system drive. You may use the 32 bit version as it will work on all systems, unless you have a specific reason that requires you to use the 64 bit version. You may need to install or update your Java JRE and add it to your system path. The JRE needs to match, e.g. 32 bit Eclipse requires the 32 bit JRE.

####4. Install the Eclipse C/C++ plugin
Open Eclipse. 
Accept the default workspace for now (you may want to set a more appropriate workspace location later on). Then install the "C/C++ Unit Test" plugin:  
  "Help"->"Install New Software"-> work with "Kepler - http://download.eclipse.org/releases/kepler".  
  You will find the plugin under:
```
  [ ] Programming Languages
      [x] C/C++ Unit Testing Support.
```
While you are at it, you might also want to install:  
```
  [ ] Linux Tools  
      [x] Gcov Integration
```

####5. Add the CppUTest plugin
You can find the plugin as well as instructions at https://github.com/tcmak/CppUTestEclipseJunoTestRunner

###Import the project into Eclipse
In Eclipse you can't simply "open" a project - you have to first import it into the workspace:  
 * File->Import...->General->Existing Projects into workspace
 * [Next >]->Select root directory: `<cpputest_dir>/Platforms/Eclipse-Cygwin` 
 * Make sure that `[ ] Copy projects into workspace` is **NOT** ticked.  
 * Click Finish.

###Compile and run tests
Before you can compile CppUTest in Eclipse, you must configure them using Automake. Instructions can be found at http://cpputest.github.io/. Once you have done this, select a configuration (Libraries, Check, CppUTestTests or CppUTestExtTests) via Project->BuildConfiguration->Set Active>. The Check configuration will build the libraries along with all tests and run the tests.

###Working with the C/C++ Unit plugin
Todo.  

###Troubleshooting
####Problem: No Test Results...
...but you know tests should have run. 
#####Possible Reason: 
This can happen if the Cygwin-generated binary can't find the Cygwin DLLs. (If you were to run your test executable at the Windows cmd prompt, you would see a Windows error message box to that effect). Since no error message is displayed in the consol via stderr, this leaves the test runner ignorant of what happenend
#####Solution:
You need to add <path_to_Cygwin>/Cygwin/bin to your system PATH variable. Then you must restart Eclipse.

###Raw Material
1. Unzip CppUTest
2. Install brand new Eclipse CDT
3. Let Eclipse use the default workspace location
4. import Project into workspace (no users of other IDEs will guess this one -- but let's suppose our user is reasonably Eclipse-savvy
5. make: *** No rule to make target `all'. Stop.


![CppUTest test runner plugin in action](https://raw.githubusercontent.com/arstrube/images/master/cpputest/CppUTestEclipsePlugin_CppUTest.png)

Eclipse does not show launch configurations it thinks your current project cannot run.

![error msg](https://raw.githubusercontent.com/arstrube/images/master/cygwin/cyglib_missing.png)
(Apologies for the German, my Windows is limited to this language). The IDE does not know what to do with this. In my setup, Google Test doesn't fare much better; there is a message:
```
Unknown error during parsing Google Test module output: Unexpected test module output.
```
Unfortunately, the executable doesn't even return an error code, so one could only surmise (based on no output at all) that something unexpected happened ;-).

Similar errors exist when Cygwin.dll is not found (a more common case, but the effect is exactly the same.

As far as the test runner is concerned, the tests have run and done nothing.

P.S. Correction - it exits with Code 9009. So we could improve the test runner by checking whether
 1. there is no output AND
 2. exit code is non-zero
 and outputting a message about as helpful as the Google Test one :-)

