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
  $(CPPUTEST_HOME)/tests/CppUTest/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTest/AllocLetTestFree.o \
  $(CPPUTEST_HOME)/tests/CppUTest/AllocLetTestFreeTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/CheatSheetTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/CompatabilityTests.o \
  $(CPPUTEST_HOME)/tests/CppUTest/CommandLineArgumentsTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/CommandLineTestRunnerTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/JUnitOutputTest.o

CPPU2_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTest/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTest/DummyMemoryLeakDetector.o \
  $(CPPUTEST_HOME)/tests/CppUTest/MemoryLeakWarningTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/PluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/PreprocessorTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/SetPluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/SimpleMutexTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TeamCityOutputTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestFailureNaNTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestFailureTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestResultTest.o \

CPPU3_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTest/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTest/MemoryLeakDetectorTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/SimpleStringTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/SimpleStringCacheTest.o

CPPU4_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTest/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestOutputTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestRegistryTest.o \

CPPU5_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTest/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTest/AllocationInCFile.o \
  $(CPPUTEST_HOME)/tests/CppUTest/AllocationInCppFile.o \
  $(CPPUTEST_HOME)/tests/CppUTest/MemoryOperatorOverloadTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TeamCityOutputTest.o

CPPU6_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTest/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestFilterTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestHarness_cTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestHarness_cTestCFile.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestInstallerTest.o

CPPU7_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTest/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestMemoryAllocatorTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestUTestMacro.o

CPPU8_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTest/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTest/UtestPlatformTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/UtestTest.o \
  $(CPPUTEST_HOME)/tests/CppUTest/TestUTestStringMacro.o \

CPPUX1_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/ExpectedFunctionsListTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/GMockTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/GTest1Test.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/GTest2ConvertorTest.o

CPPUX2_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MemoryReportAllocatorTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MemoryReportFormatterTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MemoryReporterPluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockActualCallTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockCheatSheetTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockComparatorCopierTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockExpectedCallTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockHierarchyTest.o

CPPUX3_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/CodeMemoryReporterTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/OrderedTestTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/OrderedTestTest_c.o \

CPPUX4_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockReturnValueTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockNamedValueTest.o 

CPPUX5_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockPluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockSupport_cTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockSupport_cTestCFile.o \

CPPUX6_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/ExpectedFunctionsListTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockCallTest.o

CPPUX7_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockComparatorCopierTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockHierarchyTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockParameterTest.o \

CPPUX8_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/IEEE754PluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/IEEE754PluginTest_c.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockComparatorCopierTest.o

CPPUX9_OBJECTS := \
  $(CPPUTEST_HOME)/tests/CppUTestExt/AllTests.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureReporterForTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockFailureTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockHierarchyTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockPluginTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockReturnValueTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockStrictOrderTest.o \
  $(CPPUTEST_HOME)/tests/CppUTestExt/MockSupportTest.o \
