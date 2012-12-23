
EXTRA_LIBRARIES = libCppUTestExt.a
EXTRA_PROGRAMS = CppUTestExtTests
lib_LIBRARIES = libCppUTest.a $(CPPUTEST_EXT_LIB)
check_PROGRAMS = CppUTestTests $(CPPUTEST_EXT_TESTS)
TESTS = $(check_PROGRAMS)

libCppUTest_a_CPPFLAGS = -I$(srcdir)/include -I$(srcdir)/include/CppUTestExt/CppUTestGTest -I$(srcdir)/include/CppUTestExt/CppUTestGMock $(AM_CPPFLAGS) $(CPPUTEST_CPPFLAGS)
libCppUTest_a_CFLAGS = $(AM_CFLAGS) $(CPPUTEST_CFLAGS)
libCppUTest_a_CXXFLAGS = $(AM_CXXFLAGS) $(CPPUTEST_CXXFLAGS)

libCppUTest_a_SOURCES = \
	src/CppUTest/CommandLineArguments.cpp \
	src/CppUTest/MemoryLeakWarningPlugin.cpp \
	src/CppUTest/TestHarness_c.cpp \
	src/CppUTest/TestRegistry.cpp \
	src/CppUTest/CommandLineTestRunner.cpp \
	src/CppUTest/SimpleString.cpp \
	src/CppUTest/TestMemoryAllocator.cpp \
	src/CppUTest/TestResult.cpp \
	src/CppUTest/JUnitTestOutput.cpp \
	src/CppUTest/TestFailure.cpp \
	src/CppUTest/TestOutput.cpp \
	src/CppUTest/MemoryLeakDetector.cpp \
	src/CppUTest/TestFilter.cpp \
	src/CppUTest/TestPlugin.cpp \
	src/CppUTest/Utest.cpp \
	src/Platforms/$(CPP_PLATFORM)/UtestPlatform.cpp

libCppUTestExt_a_CPPFLAGS = $(libCppUTest_a_CPPFLAGS)
libCppUTestExt_a_CFLAGS = $(libCppUTest_a_CFLAGS)
libCppUTestExt_a_CXXFLAGS = $(libCppUTest_a_CXXFLAGS)

libCppUTestExt_a_SOURCES = \
   src/CppUTestExt/CodeMemoryReportFormatter.cpp \
   src/CppUTestExt/MemoryReporterPlugin.cpp \
   src/CppUTestExt/MockFailure.cpp \
   src/CppUTestExt/MockSupportPlugin.cpp \
   src/CppUTestExt/GTestConvertor.cpp \
   src/CppUTestExt/MockActualFunctionCall.cpp \
   src/CppUTestExt/MockFunctionCall.cpp \
   src/CppUTestExt/MockSupport_c.cpp \
   src/CppUTestExt/MemoryReportAllocator.cpp \
   src/CppUTestExt/MockExpectedFunctionCall.cpp \
   src/CppUTestExt/MockNamedValue.cpp \
   src/CppUTestExt/OrderedTest.cpp \
   src/CppUTestExt/MemoryReportFormatter.cpp \
   src/CppUTestExt/MockExpectedFunctionsList.cpp \
   src/CppUTestExt/MockSupport.cpp

CppUTestTests_CPPFLAGS = $(libCppUTest_a_CPPFLAGS)
CppUTestTests_CFLAGS = $(libCppUTest_a_CFLAGS)
CppUTestTests_CXXFLAGS = $(libCppUTest_a_CXXFLAGS) 
CppUTestTests_LDADD = libCppUTest.a $(AM_LDFLAGS) $(CPPUTEST_LDFLAGS)

CppUTestTests_SOURCES = \
	tests/AllTests.cpp \
	tests/SetPluginTest.cpp \
	tests/CheatSheetTest.cpp \
	tests/SimpleStringTest.cpp \
	tests/CommandLineArgumentsTest.cpp \
	tests/TestFailureTest.cpp \
	tests/CommandLineTestRunnerTest.cpp \
	tests/TestFilterTest.cpp \
	tests/TestHarness_cTest.cpp \
	tests/JUnitOutputTest.cpp \
	tests/TestHarness_cTestCFile.c \
	tests/MemoryLeakDetectorTest.cpp \
	tests/TestInstallerTest.cpp \
	tests/AllocLetTestFree.c \
	tests/MemoryLeakOperatorOverloadsTest.cpp \
	tests/TestMemoryAllocatorTest.cpp \
	tests/MemoryLeakWarningTest.cpp \
	tests/TestOutputTest.cpp \
	tests/AllocLetTestFreeTest.cpp \
	tests/NullTestTest.cpp \
	tests/TestRegistryTest.cpp \
	tests/AllocationInCFile.c \
	tests/PluginTest.cpp \
	tests/TestResultTest.cpp \
	tests/PreprocessorTest.cpp \
	tests/TestUTestMacro.cpp \
	tests/AllocationInCppFile.cpp \
	tests/UtestTest.cpp

CppUTestExtTests_CPPFLAGS = $(libCppUTestExt_a_CPPFLAGS)
CppUTestExtTests_CFLAGS = $(libCppUTestExt_a_CFLAGS)
CppUTestExtTests_CXXFLAGS = $(libCppUTestExt_a_CXXFLAGS) 
CppUTestExtTests_LDADD = libCppUTestExt.a libCppUTest.a $(AM_LDFLAGS) $(CPPUTEST_LDFLAGS)

CppUTestExtTests_SOURCES = \
	tests/CppUTestExt/AllTests.cpp \
	tests/CppUTestExt/TestMockActualFunctionCall.cpp \
	tests/CppUTestExt/TestMockSupport.cpp \
	tests/CppUTestExt/TestCodeMemoryReportFormatter.cpp \
	tests/CppUTestExt/TestMockCheatSheet.cpp \
	tests/CppUTestExt/TestMockSupport_c.cpp \
	tests/CppUTestExt/TestGMock.cpp \
	tests/CppUTestExt/TestMockExpectedFunctionCall.cpp \
	tests/CppUTestExt/TestMockSupport_cCFile.c \
	tests/CppUTestExt/TestGTest.cpp \
	tests/CppUTestExt/TestMockExpectedFunctionsList.cpp \
	tests/CppUTestExt/TestMemoryReportAllocator.cpp \
	tests/CppUTestExt/TestMockFailure.cpp \
	tests/CppUTestExt/TestOrderedTest.cpp \
	tests/CppUTestExt/TestMemoryReportFormatter.cpp \
	tests/CppUTestExt/TestMemoryReporterPlugin.cpp \
	tests/CppUTestExt/TestMockPlugin.cpp
	
check_all: $(check_PROGRAMS)
	
	@echo "Building without extensions"
	make distclean; $(srcdir)/configure --disable-extensions; make check

	@echo "Building without the Standard C library"
	make distclean; $(srcdir)/configure --disable-std-c; make

	@echo "Building without the Standard C++ library"
	make distclean; $(srcdir)/configure --disable-std-cpp; make check

	@echo "Last... one normal build and test"
	make distclean; $(srcdir)/configure; make check

	