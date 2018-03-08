## Using the Eclipse-Cygwin Project

This project is for use on the Windows platform. These are the steps required for using it. (If you only wish to compile the CppUTest libraries, it is recommended that you use the method described at http://cpputest.github.io, rather than setting up Eclipse).

### Preparation
#### 1. Install Cygwin
You need to have Cygwin installed, with at least the Gnu C\C++ compiler, make, autotools and libtool. Please follow the instructions at http://cpputest.github.io to build CppUTest from the Cygwin bash prompt.

#### 2. Set your PATH
Next, you need to add the path to your Cygwin binaries to your Windows system path, e.g.
```dos
C:\<path_to_Cygwin>\Cygwin\bin
```

#### 3. Install Eclipse CDT
Use your existing Eclipse CDT (Juno, Kepler, ...) or unpack the release to your system drive. You may use the 32 bit version as it will work on all systems. Unless you have a specific reason, you do not require the 64 bit version. You may need to install or update your Java JRE and add it to your system path. The JRE needs to match, e.g. 32 bit Eclipse requires the 32 bit JRE.

#### 4. Install the C/C++Unit plugin
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

#### 5. Add the CppUTest plugin
Clone the [CppUTest Eclipse Test Runner](https://github.com/tcmak/CppUTestEclipseJunoTestRunner) and add it to Eclipse following the instructions there.

### Import this project into Eclipse
In Eclipse you can't simply "open" a project - you have to first import it into the workspace:  
 * File->Import...->General->Existing Projects into workspace
 * [Next >]->Select root directory: `<cpputest_dir>/Platforms/Eclipse-Cygwin` 
 * Make sure that `[ ] Copy projects into workspace` is **NOT** ticked.  
 * Click Finish.

### Compile and run tests
Before you can compile CppUTest in Eclipse, you must configure it using Automake. Instructions can be found at http://cpputest.github.io/. Once you have done this, select a configuration (Libraries, Check, CppUTestTests or CppUTestExtTests) via Project->BuildConfiguration->Set Active>. The 'Check' configuration will build the libraries along with all tests and run the tests.

### Working with the C/C++ Unit plugin
Make sure your unit test executable has been built and exists, e.g. cpputest_build/CppUTestTests.exe.

The first time you run tests using the plugin, you need to select them via  
Run->Run Configurations...->C/C++Unit->CppUTestTest.
Click [Run]. (If Run is greyed out, you need to build the configuration CppUTestTests first)
Once you have run the tests at least once, you should be able to select them by clicking the down triangle next to the green "Run" icon, where it should be listed right at the top. On subsequent runs, the executable will be built automatically, if necessary.

Note that launch configurations have been included with this project; in your own projects, you will need to create these yourself.

This is an example of what a successful test run would look like:  
![Successful Test Run](https://raw.githubusercontent.com/cpputest/cpputest.github.io/master/images/eclipse-testrunner-01.png)  
You can select and rerun individual tests in the upper pane, and error messages for failed tests will appear in the lower pane.
### Troubleshooting
#### Problem: I am  getting no test results...
...but you know your tests should have run, and you did not receive any error message.
##### Possible Reason: 
This can happen if the Cygwin-generated binary can't find the Cygwin DLLs. (If you were to run your test executable at the Windows cmd prompt, you would see a Windows error message box to that effect). Since no error message is displayed in the console via stderr, this leaves the test runner ignorant of what happenend
##### Solution:
You need to add <path_to_Cygwin>/Cygwin/bin to your system PATH variable. Then you must restart Eclipse.
