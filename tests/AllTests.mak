# Microsoft Developer Studio Generated NMAKE File, Based on AllTests.dsp
!IF "$(CFG)" == ""
CFG=AllTests - Win32 Debug
!MESSAGE No configuration specified. Defaulting to AllTests - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "AllTests - Win32 Release" && "$(CFG)" != "AllTests - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AllTests - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\AllTests.exe"

!ELSE 

ALL : "CppUTest - Win32 Release" "$(OUTDIR)\AllTests.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"CppUTest - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AllocationInCFile.obj"
	-@erase "$(INTDIR)\AllocationInCppFile.obj"
	-@erase "$(INTDIR)\AllTests.obj"
	-@erase "$(INTDIR)\CheatSheetTest.obj"
	-@erase "$(INTDIR)\CodeMemoryReportFormatterTest.obj"
	-@erase "$(INTDIR)\CommandLineArgumentsTest.obj"
	-@erase "$(INTDIR)\CommandLineTestRunnerTest.obj"
	-@erase "$(INTDIR)\GMockTest.obj"
	-@erase "$(INTDIR)\GTest1Test.obj"
	-@erase "$(INTDIR)\GTest2ConvertorTest.obj"
	-@erase "$(INTDIR)\JUnitOutputTest.obj"
	-@erase "$(INTDIR)\MemoryLeakDetectorTest.obj"
	-@erase "$(INTDIR)\MemoryLeakOperatorOverloadsTest.obj"
	-@erase "$(INTDIR)\MemoryLeakWarningTest.obj"
	-@erase "$(INTDIR)\MemoryReportAllocatorTest.obj"
	-@erase "$(INTDIR)\MemoryReporterPluginTest.obj"
	-@erase "$(INTDIR)\MemoryReportFormatterTest.obj"
	-@erase "$(INTDIR)\MockActualCallTest.obj"
	-@erase "$(INTDIR)\MockCheatSheetTest.obj"
	-@erase "$(INTDIR)\MockExpectedCallTest.obj"
	-@erase "$(INTDIR)\MockExpectedFunctionsListTest.obj"
	-@erase "$(INTDIR)\MockFailureTest.obj"
	-@erase "$(INTDIR)\MockPluginTest.obj"
	-@erase "$(INTDIR)\MockSupport_cTest.obj"
	-@erase "$(INTDIR)\MockSupport_cTestCFile.obj"
	-@erase "$(INTDIR)\MockSupportTest.obj"
	-@erase "$(INTDIR)\OrderedTestTest.obj"
	-@erase "$(INTDIR)\OrderedTestTest_c.obj"
	-@erase "$(INTDIR)\PluginTest.obj"
	-@erase "$(INTDIR)\PreprocessorTest.obj"
	-@erase "$(INTDIR)\SetPluginTest.obj"
	-@erase "$(INTDIR)\SimpleStringTest.obj"
	-@erase "$(INTDIR)\TestFailureTest.obj"
	-@erase "$(INTDIR)\TestFilterTest.obj"
	-@erase "$(INTDIR)\TestHarness_cTest.obj"
	-@erase "$(INTDIR)\TestHarness_cTestCFile.obj"
	-@erase "$(INTDIR)\TestInstallerTest.obj"
	-@erase "$(INTDIR)\TestMemoryAllocatorTest.obj"
	-@erase "$(INTDIR)\TestOutputTest.obj"
	-@erase "$(INTDIR)\TestRegistryTest.obj"
	-@erase "$(INTDIR)\TestResultTest.obj"
	-@erase "$(INTDIR)\UtestTest.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\AllTests.exe"
	-@erase "$(OUTDIR)\AllTests.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\include" /I "..\include\Platforms\VisualCpp" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "CPPUTEST_MEM_LEAK_DETECTION_DISABLED" /D "CPPUTEST_USE_LONG_LONG" /Fp"$(INTDIR)\AllTests.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AllTests.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\AllTests.pdb" /machine:I386 /out:"$(OUTDIR)\AllTests.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AllocationInCFile.obj" \
	"$(INTDIR)\AllocationInCppFile.obj" \
	"$(INTDIR)\AllTests.obj" \
	"$(INTDIR)\CheatSheetTest.obj" \
	"$(INTDIR)\CodeMemoryReportFormatterTest.obj" \
	"$(INTDIR)\CommandLineArgumentsTest.obj" \
	"$(INTDIR)\CommandLineTestRunnerTest.obj" \
	"$(INTDIR)\GMockTest.obj" \
	"$(INTDIR)\GTest1Test.obj" \
	"$(INTDIR)\GTest2ConvertorTest.obj" \
	"$(INTDIR)\JUnitOutputTest.obj" \
	"$(INTDIR)\MemoryLeakDetectorTest.obj" \
	"$(INTDIR)\MemoryLeakOperatorOverloadsTest.obj" \
	"$(INTDIR)\MemoryLeakWarningTest.obj" \
	"$(INTDIR)\MemoryReportAllocatorTest.obj" \
	"$(INTDIR)\MemoryReporterPluginTest.obj" \
	"$(INTDIR)\MemoryReportFormatterTest.obj" \
	"$(INTDIR)\MockActualCallTest.obj" \
	"$(INTDIR)\MockCheatSheetTest.obj" \
	"$(INTDIR)\MockExpectedCallTest.obj" \
	"$(INTDIR)\MockExpectedFunctionsListTest.obj" \
	"$(INTDIR)\MockFailureTest.obj" \
	"$(INTDIR)\MockPluginTest.obj" \
	"$(INTDIR)\MockSupport_cTest.obj" \
	"$(INTDIR)\MockSupport_cTestCFile.obj" \
	"$(INTDIR)\MockSupportTest.obj" \
	"$(INTDIR)\OrderedTestTest.obj" \
	"$(INTDIR)\OrderedTestTest_c.obj" \
	"$(INTDIR)\PluginTest.obj" \
	"$(INTDIR)\PreprocessorTest.obj" \
	"$(INTDIR)\SetPluginTest.obj" \
	"$(INTDIR)\SimpleStringTest.obj" \
	"$(INTDIR)\TestFailureTest.obj" \
	"$(INTDIR)\TestFilterTest.obj" \
	"$(INTDIR)\TestHarness_cTest.obj" \
	"$(INTDIR)\TestHarness_cTestCFile.obj" \
	"$(INTDIR)\TestInstallerTest.obj" \
	"$(INTDIR)\TestMemoryAllocatorTest.obj" \
	"$(INTDIR)\TestOutputTest.obj" \
	"$(INTDIR)\TestRegistryTest.obj" \
	"$(INTDIR)\TestResultTest.obj" \
	"$(INTDIR)\UtestTest.obj" \
	"..\Release\CppUTest.lib"

"$(OUTDIR)\AllTests.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\Release\AllTests.exe
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "CppUTest - Win32 Release" "$(OUTDIR)\AllTests.exe"
   .\Release\AllTests.exe
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\AllTests.exe" "$(OUTDIR)\AllTests.bsc"

!ELSE 

ALL : "CppUTest - Win32 Debug" "$(OUTDIR)\AllTests.exe" "$(OUTDIR)\AllTests.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"CppUTest - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AllocationInCFile.obj"
	-@erase "$(INTDIR)\AllocationInCFile.sbr"
	-@erase "$(INTDIR)\AllocationInCppFile.obj"
	-@erase "$(INTDIR)\AllocationInCppFile.sbr"
	-@erase "$(INTDIR)\AllTests.obj"
	-@erase "$(INTDIR)\AllTests.sbr"
	-@erase "$(INTDIR)\CheatSheetTest.obj"
	-@erase "$(INTDIR)\CheatSheetTest.sbr"
	-@erase "$(INTDIR)\CodeMemoryReportFormatterTest.obj"
	-@erase "$(INTDIR)\CodeMemoryReportFormatterTest.sbr"
	-@erase "$(INTDIR)\CommandLineArgumentsTest.obj"
	-@erase "$(INTDIR)\CommandLineArgumentsTest.sbr"
	-@erase "$(INTDIR)\CommandLineTestRunnerTest.obj"
	-@erase "$(INTDIR)\CommandLineTestRunnerTest.sbr"
	-@erase "$(INTDIR)\GMockTest.obj"
	-@erase "$(INTDIR)\GMockTest.sbr"
	-@erase "$(INTDIR)\GTest1Test.obj"
	-@erase "$(INTDIR)\GTest1Test.sbr"
	-@erase "$(INTDIR)\GTest2ConvertorTest.obj"
	-@erase "$(INTDIR)\GTest2ConvertorTest.sbr"
	-@erase "$(INTDIR)\JUnitOutputTest.obj"
	-@erase "$(INTDIR)\JUnitOutputTest.sbr"
	-@erase "$(INTDIR)\MemoryLeakDetectorTest.obj"
	-@erase "$(INTDIR)\MemoryLeakDetectorTest.sbr"
	-@erase "$(INTDIR)\MemoryLeakOperatorOverloadsTest.obj"
	-@erase "$(INTDIR)\MemoryLeakOperatorOverloadsTest.sbr"
	-@erase "$(INTDIR)\MemoryLeakWarningTest.obj"
	-@erase "$(INTDIR)\MemoryLeakWarningTest.sbr"
	-@erase "$(INTDIR)\MemoryReportAllocatorTest.obj"
	-@erase "$(INTDIR)\MemoryReportAllocatorTest.sbr"
	-@erase "$(INTDIR)\MemoryReporterPluginTest.obj"
	-@erase "$(INTDIR)\MemoryReporterPluginTest.sbr"
	-@erase "$(INTDIR)\MemoryReportFormatterTest.obj"
	-@erase "$(INTDIR)\MemoryReportFormatterTest.sbr"
	-@erase "$(INTDIR)\MockActualCallTest.obj"
	-@erase "$(INTDIR)\MockActualCallTest.sbr"
	-@erase "$(INTDIR)\MockCheatSheetTest.obj"
	-@erase "$(INTDIR)\MockCheatSheetTest.sbr"
	-@erase "$(INTDIR)\MockExpectedCallTest.obj"
	-@erase "$(INTDIR)\MockExpectedCallTest.sbr"
	-@erase "$(INTDIR)\MockExpectedFunctionsListTest.obj"
	-@erase "$(INTDIR)\MockExpectedFunctionsListTest.sbr"
	-@erase "$(INTDIR)\MockFailureTest.obj"
	-@erase "$(INTDIR)\MockFailureTest.sbr"
	-@erase "$(INTDIR)\MockPluginTest.obj"
	-@erase "$(INTDIR)\MockPluginTest.sbr"
	-@erase "$(INTDIR)\MockSupport_cTest.obj"
	-@erase "$(INTDIR)\MockSupport_cTest.sbr"
	-@erase "$(INTDIR)\MockSupport_cTestCFile.obj"
	-@erase "$(INTDIR)\MockSupport_cTestCFile.sbr"
	-@erase "$(INTDIR)\MockSupportTest.obj"
	-@erase "$(INTDIR)\MockSupportTest.sbr"
	-@erase "$(INTDIR)\OrderedTestTest.obj"
	-@erase "$(INTDIR)\OrderedTestTest.sbr"
	-@erase "$(INTDIR)\OrderedTestTest_c.obj"
	-@erase "$(INTDIR)\OrderedTestTest_c.sbr"
	-@erase "$(INTDIR)\PluginTest.obj"
	-@erase "$(INTDIR)\PluginTest.sbr"
	-@erase "$(INTDIR)\PreprocessorTest.obj"
	-@erase "$(INTDIR)\PreprocessorTest.sbr"
	-@erase "$(INTDIR)\SetPluginTest.obj"
	-@erase "$(INTDIR)\SetPluginTest.sbr"
	-@erase "$(INTDIR)\SimpleStringTest.obj"
	-@erase "$(INTDIR)\SimpleStringTest.sbr"
	-@erase "$(INTDIR)\TestFailureTest.obj"
	-@erase "$(INTDIR)\TestFailureTest.sbr"
	-@erase "$(INTDIR)\TestFilterTest.obj"
	-@erase "$(INTDIR)\TestFilterTest.sbr"
	-@erase "$(INTDIR)\TestHarness_cTest.obj"
	-@erase "$(INTDIR)\TestHarness_cTest.sbr"
	-@erase "$(INTDIR)\TestHarness_cTestCFile.obj"
	-@erase "$(INTDIR)\TestHarness_cTestCFile.sbr"
	-@erase "$(INTDIR)\TestInstallerTest.obj"
	-@erase "$(INTDIR)\TestInstallerTest.sbr"
	-@erase "$(INTDIR)\TestMemoryAllocatorTest.obj"
	-@erase "$(INTDIR)\TestMemoryAllocatorTest.sbr"
	-@erase "$(INTDIR)\TestOutputTest.obj"
	-@erase "$(INTDIR)\TestOutputTest.sbr"
	-@erase "$(INTDIR)\TestRegistryTest.obj"
	-@erase "$(INTDIR)\TestRegistryTest.sbr"
	-@erase "$(INTDIR)\TestResultTest.obj"
	-@erase "$(INTDIR)\TestResultTest.sbr"
	-@erase "$(INTDIR)\UtestTest.obj"
	-@erase "$(INTDIR)\UtestTest.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\AllTests.bsc"
	-@erase "$(OUTDIR)\AllTests.exe"
	-@erase "$(OUTDIR)\AllTests.ilk"
	-@erase "$(OUTDIR)\AllTests.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /GX /ZI /Od /I "..\include" /I "..\include\Platforms\VisualCpp" /FI"CppUTest/MemoryLeakDetectorMallocMacros.h" /FI"CppUTest/MemoryLeakDetectorNewMacros.h" /D "_CONSOLE" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "CPPUTEST_MEM_LEAK_DETECTION_DISABLED" /D "CPPUTEST_USE_LONG_LONG" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AllTests.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AllocationInCFile.sbr" \
	"$(INTDIR)\AllocationInCppFile.sbr" \
	"$(INTDIR)\AllTests.sbr" \
	"$(INTDIR)\CheatSheetTest.sbr" \
	"$(INTDIR)\CodeMemoryReportFormatterTest.sbr" \
	"$(INTDIR)\CommandLineArgumentsTest.sbr" \
	"$(INTDIR)\CommandLineTestRunnerTest.sbr" \
	"$(INTDIR)\GMockTest.sbr" \
	"$(INTDIR)\GTest1Test.sbr" \
	"$(INTDIR)\GTest2ConvertorTest.sbr" \
	"$(INTDIR)\JUnitOutputTest.sbr" \
	"$(INTDIR)\MemoryLeakDetectorTest.sbr" \
	"$(INTDIR)\MemoryLeakOperatorOverloadsTest.sbr" \
	"$(INTDIR)\MemoryLeakWarningTest.sbr" \
	"$(INTDIR)\MemoryReportAllocatorTest.sbr" \
	"$(INTDIR)\MemoryReporterPluginTest.sbr" \
	"$(INTDIR)\MemoryReportFormatterTest.sbr" \
	"$(INTDIR)\MockActualCallTest.sbr" \
	"$(INTDIR)\MockCheatSheetTest.sbr" \
	"$(INTDIR)\MockExpectedCallTest.sbr" \
	"$(INTDIR)\MockExpectedFunctionsListTest.sbr" \
	"$(INTDIR)\MockFailureTest.sbr" \
	"$(INTDIR)\MockPluginTest.sbr" \
	"$(INTDIR)\MockSupport_cTest.sbr" \
	"$(INTDIR)\MockSupport_cTestCFile.sbr" \
	"$(INTDIR)\MockSupportTest.sbr" \
	"$(INTDIR)\OrderedTestTest.sbr" \
	"$(INTDIR)\OrderedTestTest_c.sbr" \
	"$(INTDIR)\PluginTest.sbr" \
	"$(INTDIR)\PreprocessorTest.sbr" \
	"$(INTDIR)\SetPluginTest.sbr" \
	"$(INTDIR)\SimpleStringTest.sbr" \
	"$(INTDIR)\TestFailureTest.sbr" \
	"$(INTDIR)\TestFilterTest.sbr" \
	"$(INTDIR)\TestHarness_cTest.sbr" \
	"$(INTDIR)\TestHarness_cTestCFile.sbr" \
	"$(INTDIR)\TestInstallerTest.sbr" \
	"$(INTDIR)\TestMemoryAllocatorTest.sbr" \
	"$(INTDIR)\TestOutputTest.sbr" \
	"$(INTDIR)\TestRegistryTest.sbr" \
	"$(INTDIR)\TestResultTest.sbr" \
	"$(INTDIR)\UtestTest.sbr"

"$(OUTDIR)\AllTests.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\lib\CppUTest.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\AllTests.pdb" /debug /machine:I386 /out:"$(OUTDIR)\AllTests.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AllocationInCFile.obj" \
	"$(INTDIR)\AllocationInCppFile.obj" \
	"$(INTDIR)\AllTests.obj" \
	"$(INTDIR)\CheatSheetTest.obj" \
	"$(INTDIR)\CodeMemoryReportFormatterTest.obj" \
	"$(INTDIR)\CommandLineArgumentsTest.obj" \
	"$(INTDIR)\CommandLineTestRunnerTest.obj" \
	"$(INTDIR)\GMockTest.obj" \
	"$(INTDIR)\GTest1Test.obj" \
	"$(INTDIR)\GTest2ConvertorTest.obj" \
	"$(INTDIR)\JUnitOutputTest.obj" \
	"$(INTDIR)\MemoryLeakDetectorTest.obj" \
	"$(INTDIR)\MemoryLeakOperatorOverloadsTest.obj" \
	"$(INTDIR)\MemoryLeakWarningTest.obj" \
	"$(INTDIR)\MemoryReportAllocatorTest.obj" \
	"$(INTDIR)\MemoryReporterPluginTest.obj" \
	"$(INTDIR)\MemoryReportFormatterTest.obj" \
	"$(INTDIR)\MockActualCallTest.obj" \
	"$(INTDIR)\MockCheatSheetTest.obj" \
	"$(INTDIR)\MockExpectedCallTest.obj" \
	"$(INTDIR)\MockExpectedFunctionsListTest.obj" \
	"$(INTDIR)\MockFailureTest.obj" \
	"$(INTDIR)\MockPluginTest.obj" \
	"$(INTDIR)\MockSupport_cTest.obj" \
	"$(INTDIR)\MockSupport_cTestCFile.obj" \
	"$(INTDIR)\MockSupportTest.obj" \
	"$(INTDIR)\OrderedTestTest.obj" \
	"$(INTDIR)\OrderedTestTest_c.obj" \
	"$(INTDIR)\PluginTest.obj" \
	"$(INTDIR)\PreprocessorTest.obj" \
	"$(INTDIR)\SetPluginTest.obj" \
	"$(INTDIR)\SimpleStringTest.obj" \
	"$(INTDIR)\TestFailureTest.obj" \
	"$(INTDIR)\TestFilterTest.obj" \
	"$(INTDIR)\TestHarness_cTest.obj" \
	"$(INTDIR)\TestHarness_cTestCFile.obj" \
	"$(INTDIR)\TestInstallerTest.obj" \
	"$(INTDIR)\TestMemoryAllocatorTest.obj" \
	"$(INTDIR)\TestOutputTest.obj" \
	"$(INTDIR)\TestRegistryTest.obj" \
	"$(INTDIR)\TestResultTest.obj" \
	"$(INTDIR)\UtestTest.obj" \
	"..\lib\CppUTest.lib"

"$(OUTDIR)\AllTests.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\Debug\AllTests.exe
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "CppUTest - Win32 Debug" "$(OUTDIR)\AllTests.exe" "$(OUTDIR)\AllTests.bsc"
   .\Debug\AllTests.exe -v
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

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
!IF EXISTS("AllTests.dep")
!INCLUDE "AllTests.dep"
!ELSE 
!MESSAGE Warning: cannot find "AllTests.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "AllTests - Win32 Release" || "$(CFG)" == "AllTests - Win32 Debug"
SOURCE=.\CppUTest\AllocationInCFile.c

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\AllocationInCFile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\AllocationInCFile.obj"	"$(INTDIR)\AllocationInCFile.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\AllocationInCppFile.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\AllocationInCppFile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\AllocationInCppFile.obj"	"$(INTDIR)\AllocationInCppFile.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTestExt\AllTests.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\AllTests.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\AllTests.obj"	"$(INTDIR)\AllTests.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTest\CheatSheetTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\CheatSheetTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\CheatSheetTest.obj"	"$(INTDIR)\CheatSheetTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTestExt\CodeMemoryReportFormatterTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\CodeMemoryReportFormatterTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\CodeMemoryReportFormatterTest.obj"	"$(INTDIR)\CodeMemoryReportFormatterTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTest\CommandLineArgumentsTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\CommandLineArgumentsTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\CommandLineArgumentsTest.obj"	"$(INTDIR)\CommandLineArgumentsTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\CommandLineTestRunnerTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\CommandLineTestRunnerTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\CommandLineTestRunnerTest.obj"	"$(INTDIR)\CommandLineTestRunnerTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTestExt\GMockTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\GMockTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\GMockTest.obj"	"$(INTDIR)\GMockTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\GTest1Test.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\GTest1Test.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\GTest1Test.obj"	"$(INTDIR)\GTest1Test.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\GTest2ConvertorTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\GTest2ConvertorTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\GTest2ConvertorTest.obj"	"$(INTDIR)\GTest2ConvertorTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTest\JUnitOutputTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\JUnitOutputTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\JUnitOutputTest.obj"	"$(INTDIR)\JUnitOutputTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\MemoryLeakDetectorTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MemoryLeakDetectorTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MemoryLeakDetectorTest.obj"	"$(INTDIR)\MemoryLeakDetectorTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\MemoryLeakOperatorOverloadsTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MemoryLeakOperatorOverloadsTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MemoryLeakOperatorOverloadsTest.obj"	"$(INTDIR)\MemoryLeakOperatorOverloadsTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\MemoryLeakWarningTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MemoryLeakWarningTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MemoryLeakWarningTest.obj"	"$(INTDIR)\MemoryLeakWarningTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTestExt\MemoryReportAllocatorTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MemoryReportAllocatorTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MemoryReportAllocatorTest.obj"	"$(INTDIR)\MemoryReportAllocatorTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MemoryReporterPluginTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MemoryReporterPluginTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MemoryReporterPluginTest.obj"	"$(INTDIR)\MemoryReporterPluginTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MemoryReportFormatterTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MemoryReportFormatterTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MemoryReportFormatterTest.obj"	"$(INTDIR)\MemoryReportFormatterTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MockActualCallTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MockActualCallTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MockActualCallTest.obj"	"$(INTDIR)\MockActualCallTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MockCheatSheetTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MockCheatSheetTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MockCheatSheetTest.obj"	"$(INTDIR)\MockCheatSheetTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MockExpectedCallTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MockExpectedCallTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MockExpectedCallTest.obj"	"$(INTDIR)\MockExpectedCallTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MockExpectedFunctionsListTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MockExpectedFunctionsListTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MockExpectedFunctionsListTest.obj"	"$(INTDIR)\MockExpectedFunctionsListTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MockFailureTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MockFailureTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MockFailureTest.obj"	"$(INTDIR)\MockFailureTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MockPluginTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MockPluginTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MockPluginTest.obj"	"$(INTDIR)\MockPluginTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MockSupport_cTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MockSupport_cTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MockSupport_cTest.obj"	"$(INTDIR)\MockSupport_cTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MockSupport_cTestCFile.c

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MockSupport_cTestCFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MockSupport_cTestCFile.obj"	"$(INTDIR)\MockSupport_cTestCFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\MockSupportTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\MockSupportTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\MockSupportTest.obj"	"$(INTDIR)\MockSupportTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\OrderedTestTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\OrderedTestTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\OrderedTestTest.obj"	"$(INTDIR)\OrderedTestTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CppUTestExt\OrderedTestTest_c.c

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\OrderedTestTest_c.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\OrderedTestTest_c.obj"	"$(INTDIR)\OrderedTestTest_c.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\PluginTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\PluginTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\PluginTest.obj"	"$(INTDIR)\PluginTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\PreprocessorTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\PreprocessorTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\PreprocessorTest.obj"	"$(INTDIR)\PreprocessorTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\SetPluginTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\SetPluginTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\SetPluginTest.obj"	"$(INTDIR)\SetPluginTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\SimpleStringTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\SimpleStringTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\SimpleStringTest.obj"	"$(INTDIR)\SimpleStringTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\TestFailureTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\TestFailureTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\TestFailureTest.obj"	"$(INTDIR)\TestFailureTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\TestFilterTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\TestFilterTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\TestFilterTest.obj"	"$(INTDIR)\TestFilterTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\TestHarness_cTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\TestHarness_cTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\TestHarness_cTest.obj"	"$(INTDIR)\TestHarness_cTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\TestHarness_cTestCFile.c

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\TestHarness_cTestCFile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\TestHarness_cTestCFile.obj"	"$(INTDIR)\TestHarness_cTestCFile.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\TestInstallerTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\TestInstallerTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\TestInstallerTest.obj"	"$(INTDIR)\TestInstallerTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\TestMemoryAllocatorTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\TestMemoryAllocatorTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\TestMemoryAllocatorTest.obj"	"$(INTDIR)\TestMemoryAllocatorTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\TestOutputTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\TestOutputTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\TestOutputTest.obj"	"$(INTDIR)\TestOutputTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\TestRegistryTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\TestRegistryTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\TestRegistryTest.obj"	"$(INTDIR)\TestRegistryTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\TestResultTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\TestResultTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\TestResultTest.obj"	"$(INTDIR)\TestResultTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppUTest\UtestTest.cpp

!IF  "$(CFG)" == "AllTests - Win32 Release"


"$(INTDIR)\UtestTest.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"


"$(INTDIR)\UtestTest.obj"	"$(INTDIR)\UtestTest.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

!IF  "$(CFG)" == "AllTests - Win32 Release"

"CppUTest - Win32 Release" : 
   cd "\DEV\05_CPPUTEST\CPPUTEST"
   $(MAKE) /$(MAKEFLAGS) /F .\CppUTest.mak CFG="CppUTest - Win32 Release" 
   cd ".\tests"

"CppUTest - Win32 ReleaseCLEAN" : 
   cd "\DEV\05_CPPUTEST\CPPUTEST"
   $(MAKE) /$(MAKEFLAGS) /F .\CppUTest.mak CFG="CppUTest - Win32 Release" RECURSE=1 CLEAN 
   cd ".\tests"

!ELSEIF  "$(CFG)" == "AllTests - Win32 Debug"

"CppUTest - Win32 Debug" : 
   cd "\DEV\05_CPPUTEST\CPPUTEST"
   $(MAKE) /$(MAKEFLAGS) /F .\CppUTest.mak CFG="CppUTest - Win32 Debug" 
   cd ".\tests"

"CppUTest - Win32 DebugCLEAN" : 
   cd "\DEV\05_CPPUTEST\CPPUTEST"
   $(MAKE) /$(MAKEFLAGS) /F .\CppUTest.mak CFG="CppUTest - Win32 Debug" RECURSE=1 CLEAN 
   cd ".\tests"

!ENDIF 


!ENDIF 

