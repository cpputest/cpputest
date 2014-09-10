##Using the Eclipse Project

This project is for use on the Windows platform. These are the steps required for using it.

###Preparation

####1. Install Cygwin
You need to have Cygwin installed, with at least the Gnu C compiler, make, autotools and libtool. Please follow the instructions in /README.md to build CppUTest from the Cygwin bash prompt.

####2. Set your PATH
Then, you need to add the path to your Cygwin binaries to your system path, e.g.
````dos
C:\<path_to_Cygwin>\Cygwin\bin
````

####3. Install Eclipse CDT
Copy Eclipse CDT (Kepler) to your system drive. You may use the 32 bit version as it will work on all systems, unless you have a specific reason that requires you to use the 64 bit version. You may need to install or update your Java JRE and add it to your system path. The JRE needs to match, e.g. 32 bit Eclipse requires the 32 bit JRE.

####4. Install the Eclipse C/C++ plugin
Open Eclipse. 
Accept the default workspace for now (you may want to set a more appropriate workspace location later on). Then install the "C/C++ Unit Test" plugin:  
  "Help"->"Install New Software"-> work with "Kepler - http://download.eclipse.org/releases/kepler".  
  You will find the plugin under:
````
  [ ] Programming Languages
      [x] C/C++ Unit Testing Support.
````
While you are at it, you might also want to install:  
````
  [ ] Linux Tools  
      [x] Gcov Integration
````

####5. Add the CppUTest plugin
Todo.

###Import the project into Eclipse
Todo.

###Compiling and running tests

###Working with the C/C++ Unit plugin

1. Unzip CppUTest
2. Install brand new Eclipse CDT
3. Let Eclipse use the default workspace location
4. import Project into workspace (no users of other IDEs will guess this one -- but let's suppose our user is reasonably Eclipse-savvy
5. make: *** No rule to make target `all'. Stop.

Of course, as we all know, she would have to open a Cygwin prompt (assuming she is using Cygwin), navigate to CppUTest, and issue the command: ./configure.

But then she might as well do "make tdd" as well and the whole Eclipse shebang is really pointless.

IOW we should recommend to users NOT to use Eclipse, but go straight to GNU automake.

Now for potential contributors, explorers, developers, the curious....

I suggest two simple improvements.
    1. Rename buld configuration from "Debug" to "cpputest_build". This will cause Eclipse to use the appropriate folder. Or would, if it was using the Internal Builder.
    2. Instruct Eclipse to look for Makefile in cpputest_build. No idea how to do this.

So, looks like with Eclipse in the root, we have to dump all that build crap in the root????

Our contributor-user will also not get to use this easily:

![CppUTest test runner plugin in action](https://raw.githubusercontent.com/arstrube/images/master/cpputest/CppUTestEclipsePlugin_CppUTest.png)

@bortsov sorry to hear you had this trouble. I use Eclipse Kepler. I installed C/C++ Unit Testing plugin through help->install new software. I installed the CppuTest test runner plugin as per the README (important thing is to pay attention to the folder name/location, otherwise Eclipse will "know' the plugin but not able to find the binaries.

Eclipse does not show launch configurations it thinks your current project cannot run.

I never used the plugin name you mentioned above -- that should not be necessary. In fact, it may lead to some wrong configuration, resulting in the error you mentioned.

What is the exact hierarchy inside your plugns folder?

I just noticed the test runner you installed is not correct. I recommend you uninstall it.

You must install via help->install new software:
Work with: Kepler - http://download.eclipse.org/releases/kepler (or the equivalent for your version)
````
[ ] Programming Languages
    [x] C/C++ Unit Testing Support.
````
And while you are at it, you might also want to install:
````
[ ] Linux Tools
    [x] Gcov Integration.
````
Once you have that, I am pretty sure it will pick up the launch configuration, too.

 A.install Cygwin (include GCC C/C++, Autotools, Libtool, ...?)
 B.add <path_to_Cygwin>/bin to your system path (!)
 C.install Eclipse (copy into any folder)
 1.git clone git@github.com:cpputest/cpputest
 2.git clone git@github.com:tcmak/CppUTestEclipseJunoTestRunner
 3.download Eclipse IDE for C/C++ Developers (Kepler SR2 win32) http://www.eclipse.org/downloads/packages/eclipse-ide-cc-developers/keplersr2 unzip
 4.cd cpputest/cpputest_build
 5.autoreconf .. -i
 6.../configure
 7.make -j4
 8.cd ../../eclipse
 9.eclipse.exe Create new workspace (eclipse first question) Then install "C/C++ Unit Test" plugin: "Help"->"Install New Software"-> work with "Kepler - http://download.eclipse.org/releases/kepler" -> close eclipse
 10.mkdir dropins/CppUTest
 11.cd ../CppUTestEclipseJunoTestRunner/org.eclipse.cdt.testsrunner.cpputest
 12.cp -r bin/org META-INF plugin.properties plugin.xml ../../eclipse/dropins/CppUTest
 13.cd ../../eclipse
 14.Eclipse.exe -clean -console -consoleLog
 15.import existing project -> select platforms/Eclipse-Cygwin folder in CppUTest folder -> Finish
 16.leave "Debug" configuration build executables: CTRL+B -> 17:11:38 **** Build of configuration Debug for project CppUTest **** make all make: *** No rule to make target 'all'. Stop. 17:11:38 Build Finished (took 297ms)
 
What I did wrong?

Hah. The problem is with the IDE after all (and I have seen this with other IDEs also, so I am not specifically blaming Eclipse here).

Had you run the executable in a Windows command shell (not Cygwin bash!) you would have seen:
![error msg](https://raw.githubusercontent.com/arstrube/images/master/cygwin/cyglib_missing.png)
(Apologies for the German, my Windows is limited to this language). The IDE does not know what to do with this. In my setup, Google Test doesn't fare much better; there is a message:
````
Unknown error during parsing Google Test module output: Unexpected test module output.
````
Unfortunately, the executable doesn't even return an error code, so one could only surmise (based on no output at all) that something unexpected happened ;-).

Similar errors exist when Cygwin.dll is not found (a more common case, but the effect is exactly the same.

As far as the test runner is concerned, the tests have run and done nothing.

P.S. Correction - it exits with Code 9009. So we could improve the test runner by checking whether
 1. there is no output AND
 2. exit code is non-zero
 and outputting a message about as helpful as the Google Test one :-)



