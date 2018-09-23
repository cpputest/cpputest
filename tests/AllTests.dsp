# Microsoft Developer Studio Project File - Name="AllTests" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=AllTests - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AllTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AllTests.mak" CFG="AllTests - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AllTests - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AllTests - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AllTests - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob0 /I "..\include" /I "..\include\Platforms\VisualCpp" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "CPPUTEST_MEM_LEAK_DETECTION_DISABLED" /D "CPPUTEST_USE_LONG_LONG" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:console /incremental:yes /machine:I386
# Begin Special Build Tool
TargetPath=.\Release\AllTests.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=$(TargetPath)
# End Special Build Tool

!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "..\include" /I "..\include\Platforms\VisualCpp" /FI"CppUTest/MemoryLeakDetectorMallocMacros.h" /FI"CppUTest/MemoryLeakDetectorNewMacros.h" /D "_CONSOLE" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "CPPUTEST_MEM_LEAK_DETECTION_DISABLED" /D "CPPUTEST_USE_LONG_LONG" /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\lib\CppUTestd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
TargetPath=.\Debug\AllTests.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=$(TargetPath) -v
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "AllTests - Win32 Release"
# Name "AllTests - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CppUTest\AllocationInCFile.c
# End Source File
# Begin Source File

SOURCE=.\CppUTest\AllocationInCppFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\AllocLetTestFree.c
# End Source File
# Begin Source File

SOURCE=.\CppUTest\AllocLetTestFreeTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\AllTests.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\CheatSheetTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\CodeMemoryReporterTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\CommandLineArgumentsTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\CommandLineTestRunnerTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\ExpectedFunctionsListTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\GMockTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\GTest1Test.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\GTest2ConvertorTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\JUnitOutputTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\MemoryLeakDetectorTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\MemoryLeakWarningTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\MemoryOperatorOverloadTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MemoryReportAllocatorTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MemoryReporterPluginTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MemoryReportFormatterTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockActualCallTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockCheatSheetTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockExpectedCallTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockFailureReporterForTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockFailureReporterForTest.h
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockFailureTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockNamedValueTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockPluginTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockSupport_cTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockSupport_cTestCFile.c
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockSupport_cTestCFile.h
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\MockSupportTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\OrderedTestTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\OrderedTestTest_c.c
# End Source File
# Begin Source File

SOURCE=.\CppUTestExt\OrderedTestTest.h
# End Source File
# Begin Source File

SOURCE=.\CppUTest\PluginTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\PreprocessorTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\SetPluginTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\SimpleMutexTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\SimpleStringTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestFailureNaNTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestFailureTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestFilterTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestHarness_cTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestHarness_cTestCFile.c
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestInstallerTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestMemoryAllocatorTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestOutputTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestRegistryTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestResultTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\TestUtestMacro.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\UtestPlatformTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUTest\UtestTest.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CppUTest\AllocationInCFile.h
# End Source File
# Begin Source File

SOURCE=.\CppUTest\AllocationInCppFile.h
# End Source File
# Begin Source File

SOURCE=.\CppUTest\AllTests.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\CommandLineArguments.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\CppUTestConfig.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\MemoryLeakDetector.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\MemoryLeakDetectorMallocMacros.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\MemoryLeakDetectorNewMacros.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\PlatformSpecificFunctions.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\PlatformSpecificFunctions_c.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\SimpleMutex.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\StandardCLibrary.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\TestFailure.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\TestFilter.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\TestMemoryAllocator.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\TestTestingFixture.h
# End Source File
# Begin Source File

SOURCE=..\include\CppUTest\UtestMacros.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
