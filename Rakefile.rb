static_library_project :CppUTest, File.dirname(__FILE__) do |lib|

  lib.add_api_headers 'include'

  lib.add_configuration :Test,
    sources: ['src/CppUTest/*.cpp', 'src/Platforms/Gcc/*.cpp'],
    toolchain: toolchain(:gcc,
      cppflags: '-DNDEBUG',
      cflags: '-include CppUTest/MemoryLeakDetectorMallocMacros.h -g -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -Wstrict-prototypes',
      cxxflags: '-include CppUTest/MemoryLeakDetectorNewMacros.h -include CppUTest/MemoryLeakDetectorMallocMacros.h -g -Weverything -Wall -Wno-old-style-cast -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -pedantic-errors -Woverloaded-virtual -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -O2 -g'
    ),
    tests: [
      'CppUTestTests:Test'
    ]

  lib.build_framework_package([:Test])
end

static_library_project :CppUTestExt, File.dirname(__FILE__) do |lib|

  lib.add_api_headers 'include'

  lib.add_configuration :Test,
    sources: ['src/CppUTestExt/*.cpp'],
    toolchain: toolchain(:gcc,
      cppflags: '-DNDEBUG',
      cflags: '-include CppUTest/MemoryLeakDetectorMallocMacros.h -g -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -Wstrict-prototypes',
      cxxflags: '-include CppUTest/MemoryLeakDetectorNewMacros.h -include CppUTest/MemoryLeakDetectorMallocMacros.h -g -Weverything -Wall -Wno-old-style-cast -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -pedantic-errors -Woverloaded-virtual -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -O2 -g'
    ),
    tests: [
      'CppUTestExtTests:Test'
    ]
end

test_application_project :CppUTestTests, File.dirname(__FILE__) do |app|

  app.add_configuration :Test,
    sources: [
        'tests/AllTests.cpp',
        'tests/SetPluginTest.cpp',
        'tests/CheatSheetTest.cpp',
        'tests/SimpleStringTest.cpp',
        'tests/CommandLineArgumentsTest.cpp',
        'tests/TestFailureTest.cpp',
        'tests/TestFailureNaNTest.cpp',
        'tests/CommandLineTestRunnerTest.cpp',
        'tests/TestFilterTest.cpp',
        'tests/TestHarness_cTest.cpp',
        'tests/JUnitOutputTest.cpp',
        'tests/TestHarness_cTestCFile.c',
        'tests/MemoryLeakDetectorTest.cpp',
        'tests/TestInstallerTest.cpp',
        'tests/AllocLetTestFree.c',
        'tests/MemoryLeakOperatorOverloadsTest.cpp',
        'tests/TestMemoryAllocatorTest.cpp',
        'tests/MemoryLeakWarningTest.cpp',
        'tests/TestOutputTest.cpp',
        'tests/AllocLetTestFreeTest.cpp',
        'tests/TestRegistryTest.cpp',
        'tests/AllocationInCFile.c',
        'tests/PluginTest.cpp',
        'tests/TestResultTest.cpp',
        'tests/PreprocessorTest.cpp',
        'tests/TestUTestMacro.cpp',
        'tests/AllocationInCppFile.cpp',
        'tests/UtestTest.cpp',
        'tests/SimpleMutexTest.cpp'
      ],
    toolchain: toolchain(:gcc,
      cppflags: '-DNDEBUG',
      cflags: '-include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -Wstrict-prototypes',
      cxxflags: '-include CppUTest/MemoryLeakDetectorNewMacros.h -include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wno-old-style-cast -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -pedantic-errors -Woverloaded-virtual -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -O2 -g',
      ldflags: '-lstdc++'
    ),
    dependencies: [
      'CppUTest:Test'
    ]
end

test_application_project :CppUTestExtTests, File.dirname(__FILE__) do |app|

  app.add_configuration :Test,
    sources: [
        'tests/CppUTestExt/AllTests.cpp',
        'tests/CppUTestExt/CodeMemoryReportFormatterTest.cpp',
        'tests/CppUTestExt/GMockTest.cpp',
        'tests/CppUTestExt/GTest1Test.cpp',
        'tests/CppUTestExt/MemoryReportAllocatorTest.cpp',
        'tests/CppUTestExt/MemoryReporterPluginTest.cpp',
        'tests/CppUTestExt/MemoryReportFormatterTest.cpp',
        'tests/CppUTestExt/MockActualCallTest.cpp',
        'tests/CppUTestExt/MockCheatSheetTest.cpp',
        'tests/CppUTestExt/MockExpectedCallTest.cpp',
        'tests/CppUTestExt/MockExpectedFunctionsListTest.cpp',
        'tests/CppUTestExt/MockFailureTest.cpp',
        'tests/CppUTestExt/MockPluginTest.cpp',
        'tests/CppUTestExt/MockSupportTest.cpp',
        'tests/CppUTestExt/MockSupport_cTestCFile.c',
        'tests/CppUTestExt/MockSupport_cTest.cpp',
        'tests/CppUTestExt/OrderedTestTest.cpp'
      ],
    toolchain: toolchain(:gcc,
      cppflags: '-DNDEBUG',
      cflags: '-include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -Wstrict-prototypes',
      cxxflags: '-include CppUTest/MemoryLeakDetectorNewMacros.h -include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wno-old-style-cast -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -pedantic-errors -Woverloaded-virtual -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -O2 -g',
      ldflags: '-lstdc++',
      include_paths: [
        'include/CppUTestExt/CppUTestGMock',
        'include/CppUTestExt/CppUTestGTest'
      ]
    ),
    dependencies: [
      'CppUTest:Test', 'CppUTestExt:Test'
    ]
end
