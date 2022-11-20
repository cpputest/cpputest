CPPU_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTest/src/CommandLineArguments.o \
  $(CPPUTEST_HOME)/CppUTest/src/CommandLineTestRunner.o \
  $(CPPUTEST_HOME)/CppUTest/src/JUnitTestOutput.o \
  $(CPPUTEST_HOME)/CppUTest/src/TeamCityTestOutput.o \
  $(CPPUTEST_HOME)/CppUTest/src/MemoryLeakDetector.o \
  $(CPPUTEST_HOME)/CppUTest/src/MemoryLeakWarningPlugin.o \
  $(CPPUTEST_HOME)/CppUTest/src/SimpleMutex.o \
  $(CPPUTEST_HOME)/CppUTest/src/SimpleString.o \
  $(CPPUTEST_HOME)/CppUTest/src/SimpleStringInternalCache.o \
  $(CPPUTEST_HOME)/CppUTest/src/TestFailure.o \
  $(CPPUTEST_HOME)/CppUTest/src/TestFilter.o \
  $(CPPUTEST_HOME)/CppUTest/src/TestHarness_c.o \
  $(CPPUTEST_HOME)/CppUTest/src/TestMemoryAllocator.o \
  $(CPPUTEST_HOME)/CppUTest/src/TestOutput.o \
  $(CPPUTEST_HOME)/CppUTest/src/TestPlugin.o \
  $(CPPUTEST_HOME)/CppUTest/src/TestRegistry.o \
  $(CPPUTEST_HOME)/CppUTest/src/TestTestingFixture.o \
  $(CPPUTEST_HOME)/CppUTest/src/TestResult.o \
  $(CPPUTEST_HOME)/CppUTest/src/Utest.o \
  $(CPPUTEST_HOME)/CppUTest/Platforms/Dos/UtestPlatform.o

CPPUX_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/src/CodeMemoryReportFormatter.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MemoryReporterPlugin.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/IEEE754ExceptionsPlugin.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MockFailure.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MockSupportPlugin.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MockActualCall.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MockSupport_c.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MemoryReportAllocator.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MockExpectedCall.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MockNamedValue.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/OrderedTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MemoryReportFormatter.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MockExpectedCallsList.o \
  $(CPPUTEST_HOME)/CppUTestExt/src/MockSupport.o

CPPU1_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllocLetTestFree.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllocLetTestFreeTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/CheatSheetTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/CompatabilityTests.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/CommandLineArgumentsTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/CommandLineTestRunnerTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/JUnitOutputTest.o

CPPU2_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/DummyMemoryLeakDetector.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/MemoryLeakWarningTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/PluginTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/PreprocessorTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/SetPluginTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/SimpleMutexTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TeamCityOutputTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestFailureNaNTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestFailureTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestResultTest.o \

CPPU3_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/MemoryLeakDetectorTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/SimpleStringTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/SimpleStringCacheTest.o

CPPU4_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestOutputTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestRegistryTest.o \

CPPU5_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllocationInCFile.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllocationInCppFile.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/MemoryOperatorOverloadTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TeamCityOutputTest.o

CPPU6_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestFilterTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestHarness_cTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestHarness_cTestCFile.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestInstallerTest.o

CPPU7_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestMemoryAllocatorTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestUTestMacro.o

CPPU8_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTest/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/UtestPlatformTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/UtestTest.o \
  $(CPPUTEST_HOME)/CppUTest/tests/src/TestUTestStringMacro.o \

CPPUX1_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/ExpectedFunctionsListTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/GMockTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/GTest1Test.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/GTest2ConvertorTest.o

CPPUX2_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MemoryReportAllocatorTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MemoryReportFormatterTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MemoryReporterPluginTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockActualCallTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockCheatSheetTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockComparatorCopierTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockExpectedCallTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockHierarchyTest.o

CPPUX3_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/CodeMemoryReporterTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/OrderedTestTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/OrderedTestTest_c.o \

CPPUX4_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockReturnValueTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockNamedValueTest.o 

CPPUX5_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockPluginTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockSupport_cTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockSupport_cTestCFile.o \

CPPUX6_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/ExpectedFunctionsListTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockCallTest.o

CPPUX7_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockComparatorCopierTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockHierarchyTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockParameterTest.o \

CPPUX8_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/IEEE754PluginTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/IEEE754PluginTest_c.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockComparatorCopierTest.o

CPPUX9_OBJECTS := \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/AllTests.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockFailureTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockHierarchyTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockPluginTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockReturnValueTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockStrictOrderTest.o \
  $(CPPUTEST_HOME)/CppUTestExt/tests/src/MockSupportTest.o \
