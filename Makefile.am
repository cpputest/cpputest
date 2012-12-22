

lib_LIBRARIES = libCppUTest.a
check_PROGRAMS = CppUTestTests
TESTS = CppUTestTests

libCppUTest_a_CPPFLAGS = -I$(srcdir)/include $(AM_CPPFLAGS) $(CPPUTEST_CPPFLAGS)
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
	src/Platforms/Gcc/UtestPlatform.cpp

CppUTestTests_CPPFLAGS = $(libCppUTest_a_CPPFLAGS)
CppUTestTests_CFLAGS = $(libCppUTest_a_CFLAGS)
CppUTestTests_CXXFLAGS = $(libCppUTest_a_CXXFLAGS)
CppUTestTests_LDADD = libCppUTest.a -lstdc++


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
