# Microsoft Developer Studio Generated NMAKE File, Based on CppUTest.dsp
!IF "$(CFG)" == ""
CFG=CppUTest - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CppUTest - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CppUTest - Win32 Release" && "$(CFG)" != "CppUTest - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CppUTest - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\CppUTest.lib"


CLEAN :
	-@erase "$(INTDIR)\CodeMemoryReportFormatter.obj"
	-@erase "$(INTDIR)\CommandLineArguments.obj"
	-@erase "$(INTDIR)\CommandLineTestRunner.obj"
	-@erase "$(INTDIR)\JUnitTestOutput.obj"
	-@erase "$(INTDIR)\TeamCityTestOutput.obj"
	-@erase "$(INTDIR)\MemoryLeakDetector.obj"
	-@erase "$(INTDIR)\MemoryLeakWarningPlugin.obj"
	-@erase "$(INTDIR)\MemoryReportAllocator.obj"
	-@erase "$(INTDIR)\MemoryReporterPlugin.obj"
	-@erase "$(INTDIR)\MemoryReportFormatter.obj"
	-@erase "$(INTDIR)\MockActualCall.obj"
	-@erase "$(INTDIR)\MockExpectedCall.obj"
	-@erase "$(INTDIR)\MockExpectedCallsList.obj"
	-@erase "$(INTDIR)\MockFailure.obj"
	-@erase "$(INTDIR)\MockNamedValue.obj"
	-@erase "$(INTDIR)\MockSupport.obj"
	-@erase "$(INTDIR)\MockSupport_c.obj"
	-@erase "$(INTDIR)\MockSupportPlugin.obj"
	-@erase "$(INTDIR)\OrderedTest.obj"
	-@erase "$(INTDIR)\SimpleMutex.obj"
	-@erase "$(INTDIR)\SimpleString.obj"
	-@erase "$(INTDIR)\TestFailure.obj"
	-@erase "$(INTDIR)\TestFilter.obj"
	-@erase "$(INTDIR)\TestHarness_c.obj"
	-@erase "$(INTDIR)\TestMemoryAllocator.obj"
	-@erase "$(INTDIR)\TestOutput.obj"
	-@erase "$(INTDIR)\TestPlugin.obj"
	-@erase "$(INTDIR)\TestRegistry.obj"
	-@erase "$(INTDIR)\TestResult.obj"
	-@erase "$(INTDIR)\Utest.obj"
	-@erase "$(INTDIR)\UtestPlatform.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\CppUTest.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I ".\include\Platforms\VisualCpp" /I ".\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "CPPUTEST_MEM_LEAK_DETECTION_DISABLED" /D "CPPUTEST_USE_LONG_LONG" /Fp"$(INTDIR)\CppUTest.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CppUTest.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\CppUTest.lib" 
LIB32_OBJS= \
	"$(INTDIR)\CodeMemoryReportFormatter.obj" \
	"$(INTDIR)\MemoryReportAllocator.obj" \
	"$(INTDIR)\MemoryReporterPlugin.obj" \
	"$(INTDIR)\MemoryReportFormatter.obj" \
	"$(INTDIR)\MockActualCall.obj" \
	"$(INTDIR)\MockExpectedCall.obj" \
	"$(INTDIR)\MockExpectedCallsList.obj" \
	"$(INTDIR)\MockFailure.obj" \
	"$(INTDIR)\MockNamedValue.obj" \
	"$(INTDIR)\MockSupport.obj" \
	"$(INTDIR)\MockSupportPlugin.obj" \
	"$(INTDIR)\MockSupport_c.obj" \
	"$(INTDIR)\OrderedTest.obj" \
	"$(INTDIR)\CommandLineArguments.obj" \
	"$(INTDIR)\CommandLineTestRunner.obj" \
	"$(INTDIR)\JUnitTestOutput.obj" \
	"$(INTDIR)\TeamCityTestOutput.obj" \
	"$(INTDIR)\MemoryLeakDetector.obj" \
	"$(INTDIR)\MemoryLeakWarningPlugin.obj" \
	"$(INTDIR)\SimpleMutex.obj" \
	"$(INTDIR)\SimpleString.obj" \
	"$(INTDIR)\TestFailure.obj" \
	"$(INTDIR)\TestFilter.obj" \
	"$(INTDIR)\TestHarness_c.obj" \
	"$(INTDIR)\TestMemoryAllocator.obj" \
	"$(INTDIR)\TestOutput.obj" \
	"$(INTDIR)\TestPlugin.obj" \
	"$(INTDIR)\TestRegistry.obj" \
	"$(INTDIR)\TestResult.obj" \
	"$(INTDIR)\Utest.obj" \
	"$(INTDIR)\UtestPlatform.obj"

"$(OUTDIR)\CppUTest.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\lib\CppUTest.lib" "$(OUTDIR)\CppUTest.bsc"


CLEAN :
	-@erase "$(INTDIR)\CodeMemoryReportFormatter.obj"
	-@erase "$(INTDIR)\CodeMemoryReportFormatter.sbr"
	-@erase "$(INTDIR)\CommandLineArguments.obj"
	-@erase "$(INTDIR)\CommandLineArguments.sbr"
	-@erase "$(INTDIR)\CommandLineTestRunner.obj"
	-@erase "$(INTDIR)\CommandLineTestRunner.sbr"
	-@erase "$(INTDIR)\JUnitTestOutput.obj"
	-@erase "$(INTDIR)\JUnitTestOutput.sbr"
	-@erase "$(INTDIR)\TeamCityTestOutput.obj"
	-@erase "$(INTDIR)\TeamCityTestOutput.sbr"
	-@erase "$(INTDIR)\MemoryLeakDetector.obj"
	-@erase "$(INTDIR)\MemoryLeakDetector.sbr"
	-@erase "$(INTDIR)\MemoryLeakWarningPlugin.obj"
	-@erase "$(INTDIR)\MemoryLeakWarningPlugin.sbr"
	-@erase "$(INTDIR)\MemoryReportAllocator.obj"
	-@erase "$(INTDIR)\MemoryReportAllocator.sbr"
	-@erase "$(INTDIR)\MemoryReporterPlugin.obj"
	-@erase "$(INTDIR)\MemoryReporterPlugin.sbr"
	-@erase "$(INTDIR)\MemoryReportFormatter.obj"
	-@erase "$(INTDIR)\MemoryReportFormatter.sbr"
	-@erase "$(INTDIR)\MockActualCall.obj"
	-@erase "$(INTDIR)\MockActualCall.sbr"
	-@erase "$(INTDIR)\MockExpectedCall.obj"
	-@erase "$(INTDIR)\MockExpectedCall.sbr"
	-@erase "$(INTDIR)\MockExpectedCallsList.obj"
	-@erase "$(INTDIR)\MockExpectedCallsList.sbr"
	-@erase "$(INTDIR)\MockFailure.obj"
	-@erase "$(INTDIR)\MockFailure.sbr"
	-@erase "$(INTDIR)\MockNamedValue.obj"
	-@erase "$(INTDIR)\MockNamedValue.sbr"
	-@erase "$(INTDIR)\MockSupport.obj"
	-@erase "$(INTDIR)\MockSupport.sbr"
	-@erase "$(INTDIR)\MockSupport_c.obj"
	-@erase "$(INTDIR)\MockSupport_c.sbr"
	-@erase "$(INTDIR)\MockSupportPlugin.obj"
	-@erase "$(INTDIR)\MockSupportPlugin.sbr"
	-@erase "$(INTDIR)\OrderedTest.obj"
	-@erase "$(INTDIR)\OrderedTest.sbr"
	-@erase "$(INTDIR)\SimpleMutex.obj"
	-@erase "$(INTDIR)\SimpleMutex.sbr"
	-@erase "$(INTDIR)\SimpleString.obj"
	-@erase "$(INTDIR)\SimpleString.sbr"
	-@erase "$(INTDIR)\TestFailure.obj"
	-@erase "$(INTDIR)\TestFailure.sbr"
	-@erase "$(INTDIR)\TestFilter.obj"
	-@erase "$(INTDIR)\TestFilter.sbr"
	-@erase "$(INTDIR)\TestHarness_c.obj"
	-@erase "$(INTDIR)\TestHarness_c.sbr"
	-@erase "$(INTDIR)\TestMemoryAllocator.obj"
	-@erase "$(INTDIR)\TestMemoryAllocator.sbr"
	-@erase "$(INTDIR)\TestOutput.obj"
	-@erase "$(INTDIR)\TestOutput.sbr"
	-@erase "$(INTDIR)\TestPlugin.obj"
	-@erase "$(INTDIR)\TestPlugin.sbr"
	-@erase "$(INTDIR)\TestRegistry.obj"
	-@erase "$(INTDIR)\TestRegistry.sbr"
	-@erase "$(INTDIR)\TestResult.obj"
	-@erase "$(INTDIR)\TestResult.sbr"
	-@erase "$(INTDIR)\Utest.obj"
	-@erase "$(INTDIR)\Utest.sbr"
	-@erase "$(INTDIR)\UtestPlatform.obj"
	-@erase "$(INTDIR)\UtestPlatform.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\CppUTest.bsc"
	-@erase ".\lib\CppUTest.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /GX /ZI /Od /I ".\include\Platforms\VisualCpp" /I ".\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "CPPUTEST_MEM_LEAK_DETECTION_DISABLED" /D "CPPUTEST_USE_LONG_LONG" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CppUTest.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CodeMemoryReportFormatter.sbr" \
	"$(INTDIR)\MemoryReportAllocator.sbr" \
	"$(INTDIR)\MemoryReporterPlugin.sbr" \
	"$(INTDIR)\MemoryReportFormatter.sbr" \
	"$(INTDIR)\MockActualCall.sbr" \
	"$(INTDIR)\MockExpectedCall.sbr" \
	"$(INTDIR)\MockExpectedCallsList.sbr" \
	"$(INTDIR)\MockFailure.sbr" \
	"$(INTDIR)\MockNamedValue.sbr" \
	"$(INTDIR)\MockSupport.sbr" \
	"$(INTDIR)\MockSupportPlugin.sbr" \
	"$(INTDIR)\MockSupport_c.sbr" \
	"$(INTDIR)\OrderedTest.sbr" \
	"$(INTDIR)\CommandLineArguments.sbr" \
	"$(INTDIR)\CommandLineTestRunner.sbr" \
	"$(INTDIR)\JUnitTestOutput.sbr" \
	"$(INTDIR)\TeamCityTestOutput.sbr" \
	"$(INTDIR)\MemoryLeakDetector.sbr" \
	"$(INTDIR)\MemoryLeakWarningPlugin.sbr" \
	"$(INTDIR)\SimpleMutex.sbr" \
	"$(INTDIR)\SimpleString.sbr" \
	"$(INTDIR)\TestFailure.sbr" \
	"$(INTDIR)\TestFilter.sbr" \
	"$(INTDIR)\TestHarness_c.sbr" \
	"$(INTDIR)\TestMemoryAllocator.sbr" \
	"$(INTDIR)\TestOutput.sbr" \
	"$(INTDIR)\TestPlugin.sbr" \
	"$(INTDIR)\TestRegistry.sbr" \
	"$(INTDIR)\TestResult.sbr" \
	"$(INTDIR)\Utest.sbr" \
	"$(INTDIR)\UtestPlatform.sbr"

"$(OUTDIR)\CppUTest.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"lib\CppUTest.lib" 
LIB32_OBJS= \
	"$(INTDIR)\CodeMemoryReportFormatter.obj" \
	"$(INTDIR)\MemoryReportAllocator.obj" \
	"$(INTDIR)\MemoryReporterPlugin.obj" \
	"$(INTDIR)\MemoryReportFormatter.obj" \
	"$(INTDIR)\MockActualCall.obj" \
	"$(INTDIR)\MockExpectedCall.obj" \
	"$(INTDIR)\MockExpectedCallsList.obj" \
	"$(INTDIR)\MockFailure.obj" \
	"$(INTDIR)\MockNamedValue.obj" \
	"$(INTDIR)\MockSupport.obj" \
	"$(INTDIR)\MockSupportPlugin.obj" \
	"$(INTDIR)\MockSupport_c.obj" \
	"$(INTDIR)\OrderedTest.obj" \
	"$(INTDIR)\CommandLineArguments.obj" \
	"$(INTDIR)\CommandLineTestRunner.obj" \
	"$(INTDIR)\JUnitTestOutput.obj" \
	"$(INTDIR)\TeamCityTestOutput.obj" \
	"$(INTDIR)\MemoryLeakDetector.obj" \
	"$(INTDIR)\MemoryLeakWarningPlugin.obj" \
	"$(INTDIR)\SimpleMutex.obj" \
	"$(INTDIR)\SimpleString.obj" \
	"$(INTDIR)\TestFailure.obj" \
	"$(INTDIR)\TestFilter.obj" \
	"$(INTDIR)\TestHarness_c.obj" \
	"$(INTDIR)\TestMemoryAllocator.obj" \
	"$(INTDIR)\TestOutput.obj" \
	"$(INTDIR)\TestPlugin.obj" \
	"$(INTDIR)\TestRegistry.obj" \
	"$(INTDIR)\TestResult.obj" \
	"$(INTDIR)\Utest.obj" \
	"$(INTDIR)\UtestPlatform.obj"

".\lib\CppUTest.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("CppUTest.dep")
!INCLUDE "CppUTest.dep"
!ELSE 
!MESSAGE Warning: cannot find "CppUTest.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CppUTest - Win32 Release" || "$(CFG)" == "CppUTest - Win32 Debug"
SOURCE=.\src\CppUTestExt\CodeMemoryReportFormatter.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\CodeMemoryReportFormatter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\CodeMemoryReportFormatter.obj"	"$(INTDIR)\CodeMemoryReportFormatter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\CommandLineArguments.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\CommandLineArguments.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\CommandLineArguments.obj"	"$(INTDIR)\CommandLineArguments.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\CommandLineTestRunner.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\CommandLineTestRunner.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\CommandLineTestRunner.obj"	"$(INTDIR)\CommandLineTestRunner.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\JUnitTestOutput.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\JUnitTestOutput.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\JUnitTestOutput.obj"	"$(INTDIR)\JUnitTestOutput.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\TeamCityTestOutput.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\TeamCityTestOutput.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\TeamCityTestOutput.obj"	"$(INTDIR)\TeamCityTestOutput.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\SRC\CPPUTEST\MemoryLeakDetector.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MemoryLeakDetector.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MemoryLeakDetector.obj"	"$(INTDIR)\MemoryLeakDetector.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\MemoryLeakWarningPlugin.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MemoryLeakWarningPlugin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MemoryLeakWarningPlugin.obj"	"$(INTDIR)\MemoryLeakWarningPlugin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\CppUTestExt\MemoryReportAllocator.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MemoryReportAllocator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MemoryReportAllocator.obj"	"$(INTDIR)\MemoryReportAllocator.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\CppUTestExt\MemoryReporterPlugin.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MemoryReporterPlugin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MemoryReporterPlugin.obj"	"$(INTDIR)\MemoryReporterPlugin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\CppUTestExt\MemoryReportFormatter.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MemoryReportFormatter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MemoryReportFormatter.obj"	"$(INTDIR)\MemoryReportFormatter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=".\src\CppUTestExt\MockActualCall.cpp"

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MockActualCall.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MockActualCall.obj"	"$(INTDIR)\MockActualCall.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=".\src\CppUTestExt\MockExpectedCall.cpp"

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MockExpectedCall.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MockExpectedCall.obj"	"$(INTDIR)\MockExpectedCall.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=".\src\CppUTestExt\MockExpectedCallsList.cpp"

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MockExpectedCallsList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MockExpectedCallsList.obj"	"$(INTDIR)\MockExpectedCallsList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=".\src\CppUTestExt\MockFailure.cpp"

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MockFailure.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MockFailure.obj"	"$(INTDIR)\MockFailure.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=".\src\CppUTestExt\MockNamedValue.cpp"

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MockNamedValue.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MockNamedValue.obj"	"$(INTDIR)\MockNamedValue.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=".\src\CppUTestExt\MockSupport.cpp"

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MockSupport.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MockSupport.obj"	"$(INTDIR)\MockSupport.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=".\src\CppUTestExt\MockSupport_c.cpp"

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MockSupport_c.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MockSupport_c.obj"	"$(INTDIR)\MockSupport_c.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=".\src\CppUTestExt\MockSupportPlugin.cpp"

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\MockSupportPlugin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\MockSupportPlugin.obj"	"$(INTDIR)\MockSupportPlugin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=".\src\CppUTestExt\OrderedTest.cpp"

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\OrderedTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\OrderedTest.obj"	"$(INTDIR)\OrderedTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\SimpleMutex.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\SimpleMutex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\SimpleMutex.obj"	"$(INTDIR)\SimpleMutex.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\SimpleString.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\SimpleString.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\SimpleString.obj"	"$(INTDIR)\SimpleString.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\TestFailure.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\TestFailure.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\TestFailure.obj"	"$(INTDIR)\TestFailure.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\TestFilter.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\TestFilter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\TestFilter.obj"	"$(INTDIR)\TestFilter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\TestHarness_c.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\TestHarness_c.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\TestHarness_c.obj"	"$(INTDIR)\TestHarness_c.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\TestMemoryAllocator.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\TestMemoryAllocator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\TestMemoryAllocator.obj"	"$(INTDIR)\TestMemoryAllocator.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\TestOutput.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\TestOutput.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\TestOutput.obj"	"$(INTDIR)\TestOutput.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\TestPlugin.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\TestPlugin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\TestPlugin.obj"	"$(INTDIR)\TestPlugin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\TestRegistry.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\TestRegistry.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\TestRegistry.obj"	"$(INTDIR)\TestRegistry.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\TestResult.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\TestResult.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\TestResult.obj"	"$(INTDIR)\TestResult.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SRC\CPPUTEST\Utest.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\Utest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\Utest.obj"	"$(INTDIR)\Utest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\Platforms\VisualCpp\UtestPlatform.cpp

!IF  "$(CFG)" == "CppUTest - Win32 Release"


"$(INTDIR)\UtestPlatform.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "CppUTest - Win32 Debug"


"$(INTDIR)\UtestPlatform.obj"	"$(INTDIR)\UtestPlatform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

