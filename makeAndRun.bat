rem ****
rem * for this command line build to work independent of where it was created
rem * 
rem * Generate Project/Export Makefiles
rem *
rem * then do these edits to the generated files
rem * CppUTest.dep - change the relitive path to ....\program files to \program files
rem * AllTests.mak - At the end of the makefile where the depended upon CppUTest is made
rem*       Change cd "\absolute\path\CppUTest" to cd ..
PATH=C:\Program Files\Microsoft Visual Studio\VC98\Bin;%PATH%
rem nmake /f CppUTest.mak CFG="CppUTest - Win32 Debug" all
cd tests
nmake /f AllTests.mak all
cd Debug
AllTests -v
cd ..\..
