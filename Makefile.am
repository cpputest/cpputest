
ACLOCAL_AMFLAGS = -I m4

CPPUTEST_TESTS = CppUTestTests
CPPUTESTEXT_TESTS = CppUTestExtTests

EXTRA_LIBRARIES = lib/libCppUTestExt.a
EXTRA_PROGRAMS = CppUTestExtTests

lib_LIBRARIES = lib/libCppUTest.a
check_PROGRAMS = $(CPPUTEST_TESTS)

if INCLUDE_CPPUTEST_EXT
lib_LIBRARIES+= lib/libCppUTestExt.a
check_PROGRAMS += $(CPPUTESTEXT_TESTS)
endif

TESTS = $(check_PROGRAMS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cpputest.pc

EXTRA_DIST = \
	cpputest.pc.in \
	$(ALL_FILES_IN_GIT)

lib_libCppUTest_a_CPPFLAGS = $(AM_CPPFLAGS) $(CPPUTEST_CPPFLAGS) $(CPPUTEST_ADDITIONAL_CPPFLAGS)
lib_libCppUTest_a_CFLAGS = $(AM_CFLAGS) $(CPPUTEST_CFLAGS) $(CPPUTEST_ADDITIONAL_CFLAGS)
lib_libCppUTest_a_CXXFLAGS = $(AM_CXXFLAGS) $(CPPUTEST_CXXFLAGS) $(CPPUTEST_ADDITIONAL_CXXFLAGS)

lib_libCppUTest_a_SOURCES = \
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

include_cpputestdir = $(includedir)/CppUTest

include_cpputest_HEADERS = \
	include/CppUTest/CommandLineArguments.h \
	include/CppUTest/PlatformSpecificFunctions.h \
	include/CppUTest/TestMemoryAllocator.h \
	include/CppUTest/CommandLineTestRunner.h \
	include/CppUTest/PlatformSpecificFunctions_c.h \
	include/CppUTest/TestOutput.h \
	include/CppUTest/CppUTestConfig.h \
	include/CppUTest/SimpleString.h \
	include/CppUTest/TestPlugin.h \
	include/CppUTest/JUnitTestOutput.h \
	include/CppUTest/StandardCLibrary.h \
	include/CppUTest/TestRegistry.h \
	include/CppUTest/MemoryLeakDetector.h \
	include/CppUTest/TestFailure.h \
	include/CppUTest/TestResult.h \
	include/CppUTest/MemoryLeakDetectorMallocMacros.h \
	include/CppUTest/TestFilter.h \
	include/CppUTest/TestTestingFixture.h \
	include/CppUTest/MemoryLeakDetectorNewMacros.h \
	include/CppUTest/TestHarness.h \
	include/CppUTest/Utest.h \
	include/CppUTest/MemoryLeakWarningPlugin.h \
	include/CppUTest/TestHarness_c.h \
	include/CppUTest/UtestMacros.h

lib_libCppUTestExt_a_CPPFLAGS = $(lib_libCppUTest_a_CPPFLAGS)
lib_libCppUTestExt_a_CFLAGS = $(lib_libCppUTest_a_CFLAGS)
lib_libCppUTestExt_a_CXXFLAGS = $(lib_libCppUTest_a_CXXFLAGS)

lib_libCppUTestExt_a_SOURCES = \
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

if INCLUDE_CPPUTEST_EXT
include_cpputestextdir = $(includedir)/CppUTestExt

include_cpputestext_HEADERS = \
	include/CppUTestExt/MemoryReportAllocator.h \
	include/CppUTestExt/MockExpectedFunctionsList.h \
	include/CppUTestExt/MockSupportPlugin.h \
	include/CppUTestExt/MockSupport.h \
	include/CppUTestExt/MockExpectedFunctionCall.h \
	include/CppUTestExt/MemoryReportFormatter.h \
	include/CppUTestExt/MockFailure.h \
	include/CppUTestExt/MockSupport_c.h \
	include/CppUTestExt/GMock.h \
	include/CppUTestExt/MemoryReporterPlugin.h \
	include/CppUTestExt/MockFunctionCall.h \
	include/CppUTestExt/OrderedTest.h \
	include/CppUTestExt/GTestConvertor.h \
	include/CppUTestExt/MockActualFunctionCall.h \
	include/CppUTestExt/MockNamedValue.h
	
endif

CppUTestTests_CPPFLAGS = $(lib_libCppUTest_a_CPPFLAGS)
CppUTestTests_CFLAGS = $(lib_libCppUTest_a_CFLAGS)
CppUTestTests_CXXFLAGS = $(lib_libCppUTest_a_CXXFLAGS) 
CppUTestTests_LDADD = lib/libCppUTest.a $(CPPUTEST_LDADD)
CppUTestTests_LDFLAGS = $(AM_LDFLAGS) $(CPPUTEST_LDFLAGS) $(CPPUTEST_ADDITIONAL_LDFLAGS)

CppUTestTests_SOURCES = \
	tests/AllTests.cpp \
	tests/SetPluginTest.cpp \
	tests/CheatSheetTest.cpp \
	tests/SimpleStringTest.cpp \
	tests/CommandLineArgumentsTest.cpp \
	tests/TestFailureTest.cpp \
	tests/TestFailureNaNTest.cpp \
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

CppUTestExtTests_CPPFLAGS = $(lib_libCppUTestExt_a_CPPFLAGS)
CppUTestExtTests_CFLAGS = $(lib_libCppUTestExt_a_CFLAGS)
CppUTestExtTests_CXXFLAGS = $(lib_libCppUTestExt_a_CXXFLAGS) 
CppUTestExtTests_LDADD = lib/libCppUTestExt.a lib/libCppUTest.a $(CPPUTEST_LDADD)
CppUTestExtTests_LDFLAGS = $(CppUTestTests_LDFLAGS)

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
	

RUN_CPPUTEST_TESTS = ./$(CPPUTEST_TESTS) -r
#RUN_CPPUTEST_EXT_TESTS = ./$(CPPUTEST_EXT_TESTS) -r

valgrind: check
	@if test "x$(CPPUTEST_HAS_VALGRIND)" = xno; then echo "Running the valgrind target without having valgrind. Perhaps install it first?"; exit 1; fi
	valgrind --dsymutil=yes --suppressions=$(srcdir)/valgrind.suppressions --gen-suppressions=all --error-exitcode=1 ./$(CPPUTEST_TESTS)

tdd: $(CPPUTEST_TESTS) $(CPPUTESTEXT_TESTS)
	./$(CPPUTEST_TESTS)
if INCLUDE_CPPUTEST_EXT
	./$(CPPUTESTEXT_TESTS)
endif
	

check_all: 

	@echo "Building and valgrinding"
	make distclean; $(srcdir)/configure; make valgrind
	
	@echo "Building without extensions"
	make distclean; $(srcdir)/configure --disable-extensions; make check

	@echo "Building with the Std C++ 11 turned on. Compiler acts differently then."
	make distclean; $(srcdir)/configure --enable-std-cpp11; make

	@echo "Building without the Standard C library"
	make distclean; $(srcdir)/configure --disable-std-c; make

	@echo "Building without the Standard C++ library"
	make distclean; $(srcdir)/configure --disable-std-cpp; make check

	@echo "Building without memory leak detection"
	make distclean; $(srcdir)/configure --disable-memory-leak-detection; make check

	@echo "Building without memory leak detection and without Standard C++"
	make distclean; $(srcdir)/configure --disable-memory-leak-detection --disable-std-cpp; make check

	@echo "Generate a map file while building"
	make distclean; $(srcdir)/configure -enable-generate-map-file; make check
	if [ -s CppUTest.o.map.txt ]; then echo "Generating map file failed. Build failed!"; exit 1; fi
	
	@echo "Does the system have gcc? $(CPPUTEST_HAS_GCC)"
	if test "x$(CPPUTEST_HAS_GCC)" = xyes; then echo "Compiling with gcc"; make distclean; $(srcdir)/configure CC="gcc" CXX="g++"; make check; fi

	@echo "Does the system have clang and is a Mac? $(CPPUTEST_HAS_CLANG)"
	if test "x$(CPPUTEST_HAS_CLANG)" = xyes && test "x$(CPPUTEST_ON_MACOSX)" = xyes; then \
	   echo "Compiling with clang"; make distclean; $(srcdir)/configure CC="clang" CXX="clang++"; make check; \
	fi

	@echo Testing JUnit output
	make distclean; $(srcdir)/configure; make check
	./$(CPPUTEST_TESTS) -ojunit > junit_run_output
	if [ -s junit_run_output ]; then echo "JUnit run has output. Build failed!"; exit 1; fi
	rm junit_run_output; rm cpputest_*.xml
	
	@echo "Compile with coverage (switch to clang for Mac OSX)"
	if test "x$(CPPUTEST_HAS_CLANG)" = xyes && test "x$(CPPUTEST_ON_MACOSX)" = xyes; then \
	   echo "Compiling with clang"; make distclean; $(srcdir)/configure CC="clang" CXX="clang++" --enable-coverage; \
	else \
		make distclean; $(srcdir)/configure -enable-coverage;  \
	fi
	
	make check
	
	./$(CPPUTEST_TESTS) >> test_output.txt; ./$(CPPUTESTEXT_TESTS) >> test_output.txt 
	$(SILENCE)for f in `ls *.gcno` ; do \
		gcov $(CppUTestExtTests_SOURCES) $(CppUTestTests_SOURCES) $(lib_libCppUTest_a_SOURCES) $(lib_libCppUTestExt_a_SOURCES) -o $$f 1>>gcov_output.txt 2>>gcov_error.txt; \
	done
	$(srcdir)/scripts/filterGcov.sh gcov_output.txt gcov_error.txt gcov_report.txt test_output.txt
	cat gcov_report.txt
	if test "x$(CPPUTEST_HAS_LCOV)" = xyes; then  lcov -c -d . -o coverage.info; genhtml -o test_coverage coverage.info; fi
	rm -f gcov_output.txt gcov_error.txt gcov_report.txt test_output.txt gcov_report.txt.html coverage.info
	rm -rf test_coverage

	@echo "Compiling and running the examples. This will use the old Makefile"
	make distclean; $(srcdir)/configure; make; $(MAKE) -C $(srcdir)/examples all clean CPPUTEST_LIB_LINK_DIR="`pwd`/lib"

	@echo "Build using real gtest"
	make distclean; $(srcdir)/configure --enable-real-gtest; make check

	@echo "Build using gmock"
	make distclean; $(srcdir)/configure --enable-gmock; make check
	
	@echo "Building with all flags turned off"
	make distclean; $(srcdir)/configure --disable-cpputest-flags CFLAGS="" CXXFLAGS="" CPPFLAGS="-I $(srcdir)/include -I$(srcdir)/include/CppUTestExt/CppUTestGTest -I$(srcdir)/include/CppUTestExt/CppUTestGMock" --disable-dependency-tracking; make check
			
	@echo "Last... one normal build and test"
	make distclean; $(srcdir)/configure; make check; $(RUN_CPPUTEST_TESTS); $(RUN_CPPUTEST_EXT_TESTS)
	
	


