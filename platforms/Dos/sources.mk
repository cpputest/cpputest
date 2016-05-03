CPPU_OBJECTS := \
  $(CPPUTEST_HOME)/src/CppUTest/CommandLineArguments.o \
  $(CPPUTEST_HOME)/src/CppUTest/CommandLineTestRunner.o \
  $(CPPUTEST_HOME)/src/CppUTest/JUnitTestOutput.o \
  $(CPPUTEST_HOME)/src/CppUTest/TeamCityTestOutput.o \
  $(CPPUTEST_HOME)/src/CppUTest/MemoryLeakDetector.o \
  $(CPPUTEST_HOME)/src/CppUTest/MemoryLeakWarningPlugin.o \
  $(CPPUTEST_HOME)/src/CppUTest/SimpleMutex.o \
  $(CPPUTEST_HOME)/src/CppUTest/SimpleString.o \
  $(CPPUTEST_HOME)/src/CppUTest/TeamCityTestOutput.o \
  $(CPPUTEST_HOME)/src/CppUTest/TestFailure.o \
  $(CPPUTEST_HOME)/src/CppUTest/TestFilter.o \
  $(CPPUTEST_HOME)/src/CppUTest/TestHarness_c.o \
  $(CPPUTEST_HOME)/src/CppUTest/TestMemoryAllocator.o \
  $(CPPUTEST_HOME)/src/CppUTest/TestOutput.o \
  $(CPPUTEST_HOME)/src/CppUTest/TestPlugin.o \
  $(CPPUTEST_HOME)/src/CppUTest/TestRegistry.o \
  $(CPPUTEST_HOME)/src/CppUTest/TestTestingFixture.o \
  $(CPPUTEST_HOME)/src/CppUTest/TestResult.o \
  $(CPPUTEST_HOME)/src/CppUTest/Utest.o \
  $(CPPUTEST_HOME)/src/Platforms/Dos/UtestPlatform.o \

CPPUX_OBJECTS := \
  $(CPPUTEST_HOME)/src/CppUTestExt/CodeMemoryReportFormatter.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MemoryReporterPlugin.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/IEEE754ExceptionsPlugin.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MockFailure.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MockSupportPlugin.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MockActualCall.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MockSupport_c.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MemoryReportAllocator.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MockExpectedCall.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MockNamedValue.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/OrderedTest.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MemoryReportFormatter.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MockExpectedCallsList.o \
  $(CPPUTEST_HOME)/src/CppUTestExt/MockSupport.o \

CPPU1_OBJECTS := \
  $(CPPUTEST_HOME)/tests/AllTests.o \
  $(CPPUTEST_HOME)/tests/AllocLetTestFree.o \
  $(CPPUTEST_HOME)/tests/AllocLetTestFreeTest.o \
  $(CPPUTEST_HOME)/tests/CheatSheetTest.o \
  $(CPPUTEST_HOME)/tests/CommandLineArgumentsTest.o \
  $(CPPUTEST_HOME)/tests/CommandLineTestRunnerTest.o \
  $(CPPUTEST_HOME)/tests/JUnitOutputTest.o \
  $(CPPUTEST_HOME)/tests/MemoryLeakWarningTest.o \
  $(CPPUTEST_HOME)/tests/PluginTest.o \
  $(CPPUTEST_HOME)/tests/PreprocessorTest.o \
  $(CPPUTEST_HOME)/tests/SetPluginTest.o \
  $(CPPUTEST_HOME)/tests/SimpleMutexTest.o \
  $(CPPUTEST_HOME)/tests/TeamCityOutputTest.o \
  $(CPPUTEST_HOME)/tests/TestFailureNaNTest.o \
  $(CPPUTEST_HOME)/tests/TestFailureTest.o \
  $(CPPUTEST_HOME)/tests/TestResultTest.o \

CPPU2_OBJECTS := \
  $(CPPUTEST_HOME)/tests/AllTests.o \
  $(CPPUTEST_HOME)/tests/MemoryLeakDetectorTest.o \
  $(CPPUTEST_HOME)/tests/SimpleStringTest.o \
  $(CPPUTEST_HOME)/tests/TestOutputTest.o \
  $(CPPUTEST_HOME)/tests/TestRegistryTest.o \

CPPU3_OBJECTS := \
  $(CPPUTEST_HOME)/tests/AllTests.o \
  $(CPPUTEST_HOME)/tests/AllocationInCFile.o \
  $(CPPUTEST_HOME)/tests/AllocationInCppFile.o \
  $(CPPUTEST_HOME)/tests/MemoryOperatorOverloadTest.o \
  $(CPPUTEST_HOME)/tests/TeamCityOutputTest.o \
  $(CPPUTEST_HOME)/tests/TestFilterTest.o \
  $(CPPUTEST_HOME)/tests/TestHarness_cTest.o \
  $(CPPUTEST_HOME)/tests/TestHarness_cTestCFile.o \
  $(CPPUTEST_HOME)/tests/TestInstallerTest.o \
  $(CPPUTEST_HOME)/tests/TestMemoryAllocatorTest.o \
  $(CPPUTEST_HOME)/tests/TestUTestMacro.o \
  $(CPPUTEST_HOME)/tests/UtestPlatformTest.o \
  $(CPPUTEST_HOME)/tests/UtestTest.o \

CPPU4_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/ExpectedFunctionsListTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/GMockTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/GTest1Test.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/GTest2ConvertorTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MemoryReportAllocatorTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MemoryReportFormatterTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MemoryReporterPluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockActualCallTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockCheatSheetTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockComparatorCopierTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockExpectedCallTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockHierarchyTest.o \

CPPU5_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/CodeMemoryReporterTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/OrderedTestTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockReturnValueTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockNamedValueTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockPluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockSupport_cTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockSupport_cTestCFile.o \

CPPU6_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/ExpectedFunctionsListTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockCallTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockComparatorCopierTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockHierarchyTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockParameterTest.o \
  $(CPPUTEST_HOME)/tests/TestUTestStringMacro.o \

  CPPU7_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/IEEE754PluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/IEEE754PluginTest_c.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockComparatorCopierTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockHierarchyTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockPluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockReturnValueTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockStrictOrderTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockSupportTest.o \
