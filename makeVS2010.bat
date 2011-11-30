rem ****
rem * Command line build - For CppUTest - Run from CppUTest directory
rem * 
rem * this path works on my machine
rem ****PATH=C:\Windows\Microsoft.NET\Framework\v4.0.30319;c:\windows\system32;c:\windows
msbuild /t:rebuild CppUTest_vc10.sln
tests\Debug\AllTests
:end
