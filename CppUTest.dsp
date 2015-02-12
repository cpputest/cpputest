# Microsoft Developer Studio Project File - Name="CppUTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CppUTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CppUTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CppUTest.mak" CFG="CppUTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CppUTest - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CppUTest - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CppUTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\include\Platforms\VisualCpp" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I ".\include\Platforms\VisualCpp" /I ".\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\CppUTest.lib"

!ENDIF 

# Begin Target

# Name "CppUTest - Win32 Release"
# Name "CppUTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SRC\CPPUTEST\CommandLineTestRunner.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\CPPUTEST\JUnitTestOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\CPPUTEST\MemoryLeakWarningPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\CPPUTEST\SimpleString.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\CPPUTEST\TestHarness_c.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\CPPUTEST\TestOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\CPPUTEST\TestPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\CPPUTEST\TestRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\CPPUTEST\TestResult.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\CPPUTEST\Utest.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Platforms\VisualCpp\UtestPlatform.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\CppUTest\CommandLineTestRunner.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\EqualsFailure.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\Failure.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\JunitTestOutput.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\MemoryLeakWarning.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\MemoryLeakWarningPlugin.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\MockTestOutput.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\NullTest.h
# End Source File
# Begin Source File

SOURCE=.\src\Platforms\VisualCpp\Platform.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\RealTestOutput.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\SimpleString.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\SimpleStringExtensions.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\TestHarness.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\TestHarness_c.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\TestInstaller.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\TestOutput.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\TestPlugin.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\TestRegistry.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\TestResult.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\Utest.h
# End Source File
# Begin Source File

SOURCE=.\include\CppUTest\VirtualCall.h
# End Source File
# End Group
# End Target
# End Project
