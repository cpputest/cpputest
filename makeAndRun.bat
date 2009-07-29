PATH=C:\Program Files\Microsoft Visual Studio\VC98\Bin;%PATH%
nmake /f CppUTest.mak CFG="CppUTest - Win32 Debug" all
nmake /f tests\AllTests.mak CFG="AllTests - Win32 Debug" all
tests\Debug\AllTests