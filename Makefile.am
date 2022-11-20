
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
	CppUTest/tests/src/AllocationInCFile.c \
	CppUTest/tests/src/AllocationInCppFile.cpp \
	CppUTest/tests/src/AllocLetTestFree.c \
	CppUTest/tests/src/AllocLetTestFreeTest.cpp \
	CppUTest/tests/src/AllTests.cpp \
	CppUTest/tests/src/CheatSheetTest.cpp \
	CppUTest/tests/src/CompatabilityTests.cpp \
	CppUTest/tests/src/CommandLineArgumentsTest.cpp \
	CppUTest/tests/src/CommandLineTestRunnerTest.cpp \
	CppUTest/tests/src/DummyMemoryLeakDetector.cpp \
	CppUTest/tests/src/JUnitOutputTest.cpp \
	CppUTest/tests/src/MemoryLeakDetectorTest.cpp \
	CppUTest/tests/src/MemoryOperatorOverloadTest.cpp \
	CppUTest/tests/src/MemoryLeakWarningTest.cpp \
	CppUTest/tests/src/PluginTest.cpp \
	CppUTest/tests/src/PreprocessorTest.cpp \
	CppUTest/tests/src/SetPluginTest.cpp \
	CppUTest/tests/src/SimpleStringTest.cpp \
	CppUTest/tests/src/SimpleStringCacheTest.cpp \
	CppUTest/tests/src/SimpleMutexTest.cpp \
	CppUTest/tests/src/TeamCityOutputTest.cpp \
	CppUTest/tests/src/TestFailureNaNTest.cpp \
	CppUTest/tests/src/TestFailureTest.cpp \
	CppUTest/tests/src/TestFilterTest.cpp \
	CppUTest/tests/src/TestHarness_cTest.cpp \
	CppUTest/tests/src/TestHarness_cTestCFile.c \
	CppUTest/tests/src/TestInstallerTest.cpp \
	CppUTest/tests/src/TestMemoryAllocatorTest.cpp \
	CppUTest/tests/src/TestOutputTest.cpp \
	CppUTest/tests/src/TestRegistryTest.cpp \
	CppUTest/tests/src/TestResultTest.cpp \
	CppUTest/tests/src/TestUTestMacro.cpp \
	CppUTest/tests/src/TestUTestStringMacro.cpp \
	CppUTest/tests/src/UtestTest.cpp \
	CppUTest/tests/src/UtestPlatformTest.cpp

CppUTestExtTests_CPPFLAGS = $(lib_libCppUTestExt_a_CPPFLAGS)
CppUTestExtTests_CFLAGS = $(lib_libCppUTestExt_a_CFLAGS)
CppUTestExtTests_CXXFLAGS = $(lib_libCppUTestExt_a_CXXFLAGS)
CppUTestExtTests_LDADD = lib/libCppUTestExt.a lib/libCppUTest.a $(CPPUTEST_LDADD)
CppUTestExtTests_LDFLAGS = $(CppUTestTests_LDFLAGS)

CppUTestExtTests_SOURCES = \
	CppUTestExt/tests/src/AllTests.cpp \
	CppUTestExt/tests/src/CodeMemoryReporterTest.cpp \
	CppUTestExt/tests/src/GMockTest.cpp \
	CppUTestExt/tests/src/GTest1Test.cpp \
	CppUTestExt/tests/src/GTest2ConvertorTest.cpp \
	CppUTestExt/tests/src/IEEE754PluginTest.cpp \
	CppUTestExt/tests/src/IEEE754PluginTest_c.c \
	CppUTestExt/tests/src/MemoryReportAllocatorTest.cpp \
	CppUTestExt/tests/src/MemoryReporterPluginTest.cpp \
	CppUTestExt/tests/src/MemoryReportFormatterTest.cpp \
	CppUTestExt/tests/src/MockActualCallTest.cpp \
	CppUTestExt/tests/src/MockCheatSheetTest.cpp \
	CppUTestExt/tests/src/MockCallTest.cpp \
	CppUTestExt/tests/src/MockComparatorCopierTest.cpp \
	CppUTestExt/tests/src/MockExpectedCallTest.cpp \
	CppUTestExt/tests/src/ExpectedFunctionsListTest.cpp \
	CppUTestExt/tests/src/MockFailureReporterForTest.cpp \
	CppUTestExt/tests/src/MockFailureTest.cpp \
	CppUTestExt/tests/src/MockHierarchyTest.cpp \
	CppUTestExt/tests/src/MockNamedValueTest.cpp \
	CppUTestExt/tests/src/MockParameterTest.cpp \
	CppUTestExt/tests/src/MockPluginTest.cpp \
	CppUTestExt/tests/src/MockSupportTest.cpp \
	CppUTestExt/tests/src/MockSupport_cTest.cpp \
	CppUTestExt/tests/src/MockSupport_cTestCFile.c \
	CppUTestExt/tests/src/MockStrictOrderTest.cpp \
	CppUTestExt/tests/src/MockReturnValueTest.cpp \
	CppUTestExt/tests/src/OrderedTestTest.cpp \
	CppUTestExt/tests/src/OrderedTestTest_c.c \
	CppUTestExt/tests/src/MockFakeLongLong.cpp

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
	CppUTestExt/tests/src/AllTests.cpp
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

remove_gtest_directories:
	rm -rf cpputest_build_gtest15
	rm -rf cpputest_build_gtest16
	rm -rf cpputest_build_gtest17
	rm -rf cpputest_build_gtest18

check_gtest: remove_gtest_directories check_gtest15 check_gtest16 check_gtest17 check_gtest18

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

