
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

if INCLUDE_GMOCKTESTS
# check_PROGRAMS += GTestTests
endif

TESTS = $(check_PROGRAMS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cpputest.pc

cmakemodulesdir = $(libdir)/CppUTest/cmake/Modules
cmakemodules_DATA = \
  cmake/Modules/CppUTestBuildTimeDiscoverTests.cmake

cmakescriptsdir = $(libdir)/CppUTest/cmake/Scripts
cmakescripts_DATA = \
  cmake/Scripts/CppUTestBuildTimeDiscoverTests.cmake

cmakedir = $(libdir)/CppUTest/cmake
cmake_DATA = \
  build/cmake_package_files/CppUTestConfig.cmake \
  build/cmake_package_files/CppUTestConfigVersion.cmake \
  build/cmake_package_files/CppUTestTargets-relwithdebinfo.cmake \
  build/cmake_package_files/CppUTestTargets.cmake

EXTRA_DIST = \
	cpputest.pc.in \
	$(ALL_FILES_IN_GIT)

lib_libCppUTest_a_CPPFLAGS = $(AM_CPPFLAGS) $(CPPUTEST_CPPFLAGS) $(CPPUTEST_ADDITIONAL_CPPFLAGS)
lib_libCppUTest_a_CFLAGS = $(AM_CFLAGS) $(CPPUTEST_CFLAGS) $(CPPUTEST_ADDITIONAL_CFLAGS)
lib_libCppUTest_a_CXXFLAGS = $(AM_CXXFLAGS) $(CPPUTEST_CXXFLAGS) $(CPPUTEST_ADDITIONAL_CXXFLAGS)

lib_libCppUTest_a_SOURCES = \
	CppUTest/src/CommandLineArguments.cpp \
	CppUTest/src/CommandLineTestRunner.cpp \
	CppUTest/src/JUnitTestOutput.cpp \
	CppUTest/src/MemoryLeakDetector.cpp \
	CppUTest/src/MemoryLeakWarningPlugin.cpp \
	CppUTest/src/SimpleString.cpp \
	CppUTest/src/SimpleStringInternalCache.cpp \
	CppUTest/src/SimpleMutex.cpp \
	CppUTest/src/TeamCityTestOutput.cpp \
	CppUTest/src/TestFailure.cpp \
	CppUTest/src/TestFilter.cpp \
	CppUTest/src/TestHarness_c.cpp \
	CppUTest/src/TestMemoryAllocator.cpp \
	CppUTest/src/TestOutput.cpp \
	CppUTest/src/TestPlugin.cpp \
	CppUTest/src/TestRegistry.cpp \
	CppUTest/src/TestResult.cpp \
	CppUTest/src/TestTestingFixture.cpp \
	CppUTest/src/Utest.cpp \
	CppUTest/Platforms/@CPP_PLATFORM@/UtestPlatform.cpp

include_cpputestdir = $(srcdir)/CppUTest/include

include_cpputest_HEADERS = \
	CppUTest/include/CppUTest/CommandLineArguments.h \
	CppUTest/include/CppUTest/CommandLineTestRunner.h \
	CppUTest/include/CppUTest/CppUTestConfig.h \
	CppUTest/include/CppUTest/JUnitTestOutput.h \
	CppUTest/include/CppUTest/MemoryLeakDetector.h \
	CppUTest/include/CppUTest/MemoryLeakDetectorMallocMacros.h \
	CppUTest/include/CppUTest/MemoryLeakDetectorNewMacros.h \
	CppUTest/include/CppUTest/MemoryLeakWarningPlugin.h \
	CppUTest/include/CppUTest/PlatformSpecificFunctions.h \
	CppUTest/include/CppUTest/PlatformSpecificFunctions_c.h \
	CppUTest/include/CppUTest/SimpleString.h \
	CppUTest/include/CppUTest/SimpleStringInternalCache.h \
	CppUTest/include/CppUTest/SimpleMutex.h \
	CppUTest/include/CppUTest/StandardCLibrary.h \
	CppUTest/include/CppUTest/TeamCityTestOutput.h \
	CppUTest/include/CppUTest/TestFailure.h \
	CppUTest/include/CppUTest/TestFilter.h \
	CppUTest/include/CppUTest/TestHarness.h \
	CppUTest/include/CppUTest/TestHarness_c.h \
	CppUTest/include/CppUTest/TestMemoryAllocator.h \
	CppUTest/include/CppUTest/TestOutput.h \
	CppUTest/include/CppUTest/TestPlugin.h \
	CppUTest/include/CppUTest/TestRegistry.h \
	CppUTest/include/CppUTest/TestResult.h \
	CppUTest/include/CppUTest/TestTestingFixture.h \
	CppUTest/include/CppUTest/Utest.h \
	CppUTest/include/CppUTest/UtestMacros.h \
	generated/CppUTestGeneratedConfig.h

lib_libCppUTestExt_a_CPPFLAGS = $(lib_libCppUTest_a_CPPFLAGS)
lib_libCppUTestExt_a_CFLAGS = $(lib_libCppUTest_a_CFLAGS)
lib_libCppUTestExt_a_CXXFLAGS = $(lib_libCppUTest_a_CXXFLAGS)

lib_libCppUTestExt_a_SOURCES = \
   CppUTestExt/src/CodeMemoryReportFormatter.cpp \
   CppUTestExt/src/GTest.cpp \
   CppUTestExt/src/IEEE754ExceptionsPlugin.cpp \
   CppUTestExt/src/MemoryReportAllocator.cpp \
   CppUTestExt/src/MemoryReporterPlugin.cpp \
   CppUTestExt/src/MemoryReportFormatter.cpp \
   CppUTestExt/src/MockActualCall.cpp \
   CppUTestExt/src/MockExpectedCall.cpp \
   CppUTestExt/src/MockExpectedCallsList.cpp \
   CppUTestExt/src/MockFailure.cpp \
   CppUTestExt/src/MockNamedValue.cpp \
   CppUTestExt/src/MockSupport.cpp \
   CppUTestExt/src/MockSupportPlugin.cpp \
   CppUTestExt/src/MockSupport_c.cpp \
   CppUTestExt/src/OrderedTest.cpp

if INCLUDE_CPPUTEST_EXT
include_cpputestextdir = $(srcdir)/CppUTestExt/include

include_cpputestext_HEADERS = \
	CppUTestExt/include/CppUTestExt/CodeMemoryReportFormatter.h \
	CppUTestExt/include/CppUTestExt/GMock.h \
	CppUTestExt/include/CppUTestExt/GTest.h \
	CppUTestExt/include/CppUTestExt/GTestSupport.h \
	CppUTestExt/include/CppUTestExt/GTestConvertor.h \
	CppUTestExt/include/CppUTestExt/IEEE754ExceptionsPlugin.h \
	CppUTestExt/include/CppUTestExt/MemoryReportAllocator.h \
	CppUTestExt/include/CppUTestExt/MemoryReporterPlugin.h \
	CppUTestExt/include/CppUTestExt/MemoryReportFormatter.h \
	CppUTestExt/include/CppUTestExt/MockActualCall.h \
	CppUTestExt/include/CppUTestExt/MockCheckedActualCall.h \
	CppUTestExt/include/CppUTestExt/MockCheckedExpectedCall.h \
	CppUTestExt/include/CppUTestExt/MockExpectedCall.h \
	CppUTestExt/include/CppUTestExt/MockExpectedCallsList.h \
	CppUTestExt/include/CppUTestExt/MockFailure.h \
	CppUTestExt/include/CppUTestExt/MockNamedValue.h \
	CppUTestExt/include/CppUTestExt/MockSupport.h \
	CppUTestExt/include/CppUTestExt/MockSupportPlugin.h \
	CppUTestExt/include/CppUTestExt/MockSupport_c.h \
	CppUTestExt/include/CppUTestExt/OrderedTest.h

endif

CppUTestTests_CPPFLAGS = $(lib_libCppUTest_a_CPPFLAGS)
CppUTestTests_CFLAGS = $(lib_libCppUTest_a_CFLAGS)
CppUTestTests_CXXFLAGS = $(lib_libCppUTest_a_CXXFLAGS)
CppUTestTests_LDADD = lib/libCppUTest.a $(CPPUTEST_LDADD)
CppUTestTests_LDFLAGS = $(AM_LDFLAGS) $(CPPUTEST_LDFLAGS) $(CPPUTEST_ADDITIONAL_LDFLAGS)

CppUTestTests_SOURCES = \
	tests/CppUTest/AllocationInCFile.c \
	tests/CppUTest/AllocationInCppFile.cpp \
	tests/CppUTest/AllocLetTestFree.c \
	tests/CppUTest/AllocLetTestFreeTest.cpp \
	tests/CppUTest/AllTests.cpp \
	tests/CppUTest/CheatSheetTest.cpp \
	tests/CppUTest/CompatabilityTests.cpp \
	tests/CppUTest/CommandLineArgumentsTest.cpp \
	tests/CppUTest/CommandLineTestRunnerTest.cpp \
	tests/CppUTest/DummyMemoryLeakDetector.cpp \
	tests/CppUTest/JUnitOutputTest.cpp \
	tests/CppUTest/MemoryLeakDetectorTest.cpp \
	tests/CppUTest/MemoryOperatorOverloadTest.cpp \
	tests/CppUTest/MemoryLeakWarningTest.cpp \
	tests/CppUTest/PluginTest.cpp \
	tests/CppUTest/PreprocessorTest.cpp \
	tests/CppUTest/SetPluginTest.cpp \
	tests/CppUTest/SimpleStringTest.cpp \
	tests/CppUTest/SimpleStringCacheTest.cpp \
	tests/CppUTest/SimpleMutexTest.cpp \
	tests/CppUTest/TeamCityOutputTest.cpp \
	tests/CppUTest/TestFailureNaNTest.cpp \
	tests/CppUTest/TestFailureTest.cpp \
	tests/CppUTest/TestFilterTest.cpp \
	tests/CppUTest/TestHarness_cTest.cpp \
	tests/CppUTest/TestHarness_cTestCFile.c \
	tests/CppUTest/TestInstallerTest.cpp \
	tests/CppUTest/TestMemoryAllocatorTest.cpp \
	tests/CppUTest/TestOutputTest.cpp \
	tests/CppUTest/TestRegistryTest.cpp \
	tests/CppUTest/TestResultTest.cpp \
	tests/CppUTest/TestUTestMacro.cpp \
	tests/CppUTest/TestUTestStringMacro.cpp \
	tests/CppUTest/UtestTest.cpp \
	tests/CppUTest/UtestPlatformTest.cpp

CppUTestExtTests_CPPFLAGS = $(lib_libCppUTestExt_a_CPPFLAGS)
CppUTestExtTests_CFLAGS = $(lib_libCppUTestExt_a_CFLAGS)
CppUTestExtTests_CXXFLAGS = $(lib_libCppUTestExt_a_CXXFLAGS)
CppUTestExtTests_LDADD = lib/libCppUTestExt.a lib/libCppUTest.a $(CPPUTEST_LDADD)
CppUTestExtTests_LDFLAGS = $(CppUTestTests_LDFLAGS)

CppUTestExtTests_SOURCES = \
	tests/CppUTestExt/AllTests.cpp \
	tests/CppUTestExt/CodeMemoryReporterTest.cpp \
	tests/CppUTestExt/GMockTest.cpp \
	tests/CppUTestExt/GTest1Test.cpp \
	tests/CppUTestExt/GTest2ConvertorTest.cpp \
	tests/CppUTestExt/IEEE754PluginTest.cpp \
	tests/CppUTestExt/IEEE754PluginTest_c.c \
	tests/CppUTestExt/MemoryReportAllocatorTest.cpp \
	tests/CppUTestExt/MemoryReporterPluginTest.cpp \
	tests/CppUTestExt/MemoryReportFormatterTest.cpp \
	tests/CppUTestExt/MockActualCallTest.cpp \
	tests/CppUTestExt/MockCheatSheetTest.cpp \
	tests/CppUTestExt/MockCallTest.cpp \
	tests/CppUTestExt/MockComparatorCopierTest.cpp \
	tests/CppUTestExt/MockExpectedCallTest.cpp \
	tests/CppUTestExt/ExpectedFunctionsListTest.cpp \
	tests/CppUTestExt/MockFailureReporterForTest.cpp \
	tests/CppUTestExt/MockFailureTest.cpp \
	tests/CppUTestExt/MockHierarchyTest.cpp \
	tests/CppUTestExt/MockNamedValueTest.cpp \
	tests/CppUTestExt/MockParameterTest.cpp \
	tests/CppUTestExt/MockPluginTest.cpp \
	tests/CppUTestExt/MockSupportTest.cpp \
	tests/CppUTestExt/MockSupport_cTest.cpp \
	tests/CppUTestExt/MockSupport_cTestCFile.c \
	tests/CppUTestExt/MockStrictOrderTest.cpp \
	tests/CppUTestExt/MockReturnValueTest.cpp \
	tests/CppUTestExt/OrderedTestTest.cpp \
	tests/CppUTestExt/OrderedTestTest_c.c \
	tests/CppUTestExt/MockFakeLongLong.cpp

DISTCLEANFILES = \
	filename.map.txt \
	generated/CppUTestGeneratedConfig.h

if INCLUDE_GMOCKTESTS

#GTestTests_CPPFLAGS = $(lib_libCppUTestExt_a_CPPFLAGS)
#GTestTests_CFLAGS = $(lib_libCppUTestExt_a_CFLAGS)
#GTestTests_CXXFLAGS = $(lib_libCppUTestExt_a_CXXFLAGS) -DCPPUTEST_USE_MEM_LEAK_DETECTION=0 -DGMOCK_RENAME_MAIN=1 -D_THREAD_SAFE  -DGTEST_HAS_PTHREAD=1
#GTestTests_LDADD = lib/libCppUTestExt.a lib/libCppUTest.a $(CPPUTEST_LDADD)
#GTestTests_LDFLAGS = $(CppUTestTests_LDFLAGS)

#GTestTests_SOURCES = \
	$(GMOCK_HOME)/test/gmock-spec-builders_test.cc \
	tests/CppUTestExt/AllTests.cpp
endif

RUN_CPPUTEST_TESTS = ./$(CPPUTEST_TESTS)
RUN_CPPUTESTEXT_TESTS = ./$(CPPUTESTEXT_TESTS)

valgrind: check
	@if test "x$(CPPUTEST_HAS_VALGRIND)" = xno; then echo "Running the valgrind target without having valgrind. Perhaps install it first?"; exit 1; fi
	valgrind --dsymutil=yes --suppressions=$(srcdir)/valgrind.suppressions --gen-suppressions=all --error-exitcode=1 ./$(CPPUTEST_TESTS)

tdd: $(CPPUTEST_TESTS)$(EXEEXT) $(CPPUTESTEXT_TESTS)$(EXEEXT)
	./$(CPPUTEST_TESTS)
if INCLUDE_CPPUTEST_EXT
	./$(CPPUTESTEXT_TESTS)
endif


cpputest_build_gtest112:
	mkdir -p cpputest_build_gtest112
	cd cpputest_build_gtest112; \
	wget https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip -O gtest-1.12.1.zip  && unzip gtest-1.12.1.zip; \
	cd googletest-release-1.12.1; cmake .; make

cpputest_build_gtest111:
	mkdir -p cpputest_build_gtest111
	cd cpputest_build_gtest111; \
	wget https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip -O gtest-1.11.0.zip  && unzip gtest-1.11.0.zip; \
	cd googletest-release-1.11.0; cmake .; make

cpputest_build_gtest110:
	mkdir -p cpputest_build_gtest110
	cd cpputest_build_gtest110; \
	wget https://github.com/google/googletest/archive/refs/tags/release-1.10.0.zip -O gtest-1.10.0.zip  && unzip gtest-1.10.0.zip; \
	cd googletest-release-1.10.0; cmake .; make

cpputest_build_gtest18:
	mkdir -p cpputest_build_gtest18
	cd cpputest_build_gtest18; \
	wget https://github.com/google/googletest/archive/release-1.8.0.zip -O gtest-1.8.0.zip  && unzip gtest-1.8.0.zip; \
	cd googletest-release-1.8.0; cmake .; make

cpputest_build_gtest17:
	mkdir -p cpputest_build_gtest17
	cd cpputest_build_gtest17; \
	wget https://github.com/google/googletest/archive/release-1.7.0.zip -O gtest-1.7.0.zip  && unzip gtest-1.7.0.zip; \
	wget https://github.com/google/googlemock/archive/release-1.7.0.zip -O gmock-1.7.0.zip  && unzip gmock-1.7.0.zip; \
	mv googletest-release-1.7.0 googlemock-release-1.7.0/gtest; \
	cd googlemock-release-1.7.0; autoreconf -i; ./configure && make

cpputest_build_gtest16:
	mkdir -p cpputest_build_gtest16
	cd cpputest_build_gtest16; \
	wget https://github.com/google/googletest/archive/release-1.6.0.zip -O gtest-1.6.0.zip  && unzip gtest-1.6.0.zip; \
	wget https://github.com/google/googlemock/archive/release-1.6.0.zip -O gmock-1.6.0.zip  && unzip gmock-1.6.0.zip; \
	mv googletest-release-1.6.0 googlemock-release-1.6.0/gtest; \
	cd googlemock-release-1.6.0; autoreconf -i; ./configure CXXFLAGS=-DGTEST_USE_OWN_TR1_TUPLE=1 && make

cpputest_build_gtest15:
	mkdir -p cpputest_build_gtest15
	cd cpputest_build_gtest15; \
	wget https://github.com/google/googletest/archive/release-1.5.0.zip -O gtest-1.5.0.zip  && unzip gtest-1.5.0.zip; \
	wget https://github.com/google/googlemock/archive/release-1.5.0.zip -O gmock-1.5.0.zip  && unzip gmock-1.5.0.zip; \
	mv googletest-release-1.5.0 googlemock-release-1.5.0/gtest; \
	cd googlemock-release-1.5.0; autoreconf -i; ./configure CXXFLAGS=-DGTEST_USE_OWN_TR1_TUPLE=1 && make

check_gtest15: cpputest_build_gtest15
	@echo "Build using gmock 1.5";
	export GMOCK_HOME=`pwd`/cpputest_build_gtest15/googlemock-release-1.5.0; \
	make distclean; $(srcdir)/configure --enable-std-cpp98; make check

check_gtest16: cpputest_build_gtest16
	@echo "Build using gmock 1.6";
	export GMOCK_HOME=`pwd`/cpputest_build_gtest16/googlemock-release-1.6.0; \
	make distclean; $(srcdir)/configure --enable-std-cpp98; make check

check_gtest17: cpputest_build_gtest17
	@echo "Build using gmock 1.7"
	export GMOCK_HOME=`pwd`/cpputest_build_gtest17/googlemock-release-1.7.0; \
	make distclean; $(srcdir)/configure --enable-std-cpp98; make check

check_gtest18: cpputest_build_gtest18
	@echo "Build using gmock 1.8"
	export GMOCK_HOME=`pwd`/cpputest_build_gtest18/googletest-release-1.8.0/googlemock; \
	export GTEST_HOME=`pwd`/cpputest_build_gtest18/googletest-release-1.8.0/googletest; \
	make distclean; $(srcdir)/configure --enable-std-cpp98; make check

check_gtest110: cpputest_build_gtest110
	@echo "Build using gmock 1.10"
	export GMOCK_MOCK=`pwd`/cpputest_build_gtest110/googletest-release-1.10.0; \
	export GTEST_HOME=`pwd`/cpputest_build_gtest110/googletest-release-1.10.0; \
	make distclean; $(srcdir)/configure --enable-std-cpp11; make check

check_gtest111: cpputest_build_gtest111
	@echo "Build using gmock 1.11"
	export GMOCK_MOCK=`pwd`/cpputest_build_gtest111/googletest-release-1.11.0; \
	export GTEST_HOME=`pwd`/cpputest_build_gtest111/googletest-release-1.11.0; \
	make distclean; $(srcdir)/configure --enable-std-cpp11; make check

check_gtest112: cpputest_build_gtest112
	@echo "Build using gmock 1.12"
	export GMOCK_MOCK=`pwd`/cpputest_build_gtest121/googletest-release-1.12.1; \
	export GTEST_HOME=`pwd`/cpputest_build_gtest121/googletest-release-1.12.1; \
	make distclean; $(srcdir)/configure --enable-std-cpp11; make check

remove_gtest_directories:
	rm -rf cpputest_build_gtest15
	rm -rf cpputest_build_gtest16
	rm -rf cpputest_build_gtest17
	rm -rf cpputest_build_gtest18
	rm -rf cpputest_build_gtest110
	rm -rf cpputest_build_gtest111
	rm -rf cpputest_build_gtest112

check_gtest: remove_gtest_directories check_gtest15 check_gtest16 check_gtest17 check_gtest18 check_gtest110 check_gtest111 check_gtest112

check_basic:
	@echo "If dash is available, run the configure with dash to find bash-isms and increase portability"
	make distclean; if test "x$(CPPUTEST_HAS_DASH)" = xyes; then CONFIG_SHELL=dash $(srcdir)/configure; fi

	@echo "Building and valgrinding (skipping this on MacOS due to buggy Valgrind"
	if test "x$(CPPUTEST_ON_MACOSX)" = xno; then \
		make distclean; $(srcdir)/configure; make valgrind; \
	fi

	@echo "Building without extensions"
	make distclean; $(srcdir)/configure --disable-extensions; make check

	@echo "Building with the Std C++ 98 turned on. Compiler acts differently then."
	make distclean; $(srcdir)/configure --enable-std-cpp98; make

	@echo "Building with the Std C++ 11 turned on. Compiler acts differently then."
	make distclean; $(srcdir)/configure --enable-std-cpp11; make

	@echo "Building with the Std C++ 14 turned on. Compiler acts differently then."
	make distclean; $(srcdir)/configure --enable-std-cpp14; make

	@echo "Building with the Std C++ 17 turned on. Compiler acts differently then."
	make distclean; $(srcdir)/configure --enable-std-cpp17; make

	@echo "Building with the Std C++ 20 turned on. Compiler acts differently then."
	make distclean; $(srcdir)/configure --enable-std-cpp20; make

	@echo "Building without the Standard C library"
	make distclean; $(srcdir)/configure --disable-std-c; make

	@echo "Building without the Standard C++ library and without long long"
	make distclean; $(srcdir)/configure --disable-std-cpp --disable-long-long; make check

	@echo "Building without memory leak detection"
	make distclean; $(srcdir)/configure --disable-memory-leak-detection; make check

	@echo "Building with address sanitizer"
	make distclean; $(srcdir)/configure --enable-sanitize-address; make check

	@echo "Building without memory leak detection and without Standard C++"
	make distclean; $(srcdir)/configure --disable-memory-leak-detection --disable-std-cpp; make check

	@echo "Generate a map file while building"
	make distclean; $(srcdir)/configure -enable-generate-map-file; make check
	if [ -s CppUTest.o.map.txt ]; then echo "Generating map file failed. Build failed!"; exit 1; fi

check_special_situations:
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

	@echo "Building with all flags turned off"
	make distclean; $(srcdir)/configure --disable-cpputest-flags CFLAGS="" CXXFLAGS="" CPPFLAGS="-I $(srcdir)/include -I$(srcdir)/include/CppUTestExt/CppUTestGTest -I$(srcdir)/include/CppUTestExt/CppUTestGMock" --disable-dependency-tracking; make check

check_coverage:
	@echo "Compile with coverage (switch to clang for Mac OSX)"
	if test "x$(CPPUTEST_HAS_CLANG)" = xyes && test "x$(CPPUTEST_ON_MACOSX)" = xyes; then \
	   echo "Compiling with clang"; make distclean; $(srcdir)/configure CC="clang" CXX="clang++" --enable-coverage CFLAGS="-O0" CXXFLAGS="-O0"; \
	else \
		make distclean; $(srcdir)/configure -enable-coverage CFLAGS="-O0" CXXFLAGS="-O0";  \
	fi

	make check

	./$(CPPUTEST_TESTS) >> test_output.txt; ./$(CPPUTESTEXT_TESTS) >> test_output.txt
	$(SILENCE)for f in `ls *.gcno` ; do \
		gcov $(CppUTestExtTests_SOURCES) $(CppUTestTests_SOURCES) $(lib_libCppUTest_a_SOURCES) $(lib_libCppUTestExt_a_SOURCES) -o $$f 1>>gcov_output.txt 2>>gcov_error.txt; \
	done
	$(srcdir)/scripts/filterGcov.sh gcov_output.txt gcov_error.txt gcov_report.txt test_output.txt
	cat gcov_report.txt
	if test "x$(CPPUTEST_HAS_LCOV)" = xyes; then lcov -c -d . -o temp.info; lcov -r temp.info /usr\* -o coverage.info; genhtml -o test_coverage coverage.info; fi

remove_coverage_output:
	rm -f gcov_output.txt gcov_error.txt gcov_report.txt test_output.txt gcov_report.txt.html temp.info coverage.info
	rm -rf test_coverage

check_examples:
	@echo "Using the old Makefile and examples"
	make distclean
	$(MAKE) -C $(srcdir) -f Makefile_using_MakefileWorker extensions
	$(MAKE) -C $(srcdir)/examples all clean

	@echo "Compiling and running the examples. This will use the old Makefile"
	make distclean; $(srcdir)/configure; make; $(MAKE) -C $(srcdir)/examples all clean CPPUTEST_LIB_LINK_DIR="`pwd`/lib"

check_all: check_basic check_special_situations check_coverage remove_coverage_output check_examples check_gtest
	@echo "Last... one normal build and test"
	make distclean; $(srcdir)/configure; make check;
	@echo "Check running tests repeatedly"
	$(RUN_CPPUTEST_TESTS) - r; $(RUN_CPPUTESTEXT_TESTS) -r
	@echo "Check running tests in separate process (CppUTestExtTests TEST_GROUP(TestOrderedTestMacro) would have to fail)"
	$(RUN_CPPUTEST_TESTS) -p;

# Mac OSX adds an annoying directory when packaging. This can be prevented by setting COPYFILE_DISABLE=1
# However, I've not figured out how to set it automatically and neither google nor stackoverflow knew the answer.
# The automake mailing list is still thinking about it, and thus, I've added this check so that I don't forget to do it :)
# More info: http://superuser.com/questions/61185/why-do-i-get-files-like-foo-in-my-tarball-on-os-x
dist-hook:
	if test "x$(CPPUTEST_ON_MACOSX)" = "xyes"; then \
		if test ! "x$(COPYFILE_DISABLE)" = "x1"; then \
			echo ""; \
			echo 'ERROR: When packaging on MacOSX, please run "COPYFILE_DISABLE=1 make dist"'; \
			echo ""; \
			exit 1; \
		fi \
	fi

distclean-local:
	test -z "generated" || rmdir generated

