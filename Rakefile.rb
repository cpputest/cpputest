static_library_project :CppUTest, File.dirname(__FILE__) do |lib|

	lib.add_configuration :default,
		sources: ['src/CppUTest/*.cpp', 'src/Platforms/Gcc/*.cpp'],
		api_headers: 'include',
		toolchain: gcc(
			cppflags: '-DNDEBUG',
			cflags: '-include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -Wstrict-prototypes',
			cxxflags: '-include CppUTest/MemoryLeakDetectorNewMacros.h -include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -pedantic-errors -Woverloaded-virtual -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -O2 -g'
		),
		tests: [
			'CppUTestTests:default'
		]
end

static_library_project :CppUTestExt, File.dirname(__FILE__) do |lib|

	lib.add_configuration :default,
		sources: ['src/CppUTestExt/*.cpp'],
		api_headers: 'include',
		toolchain: gcc(
			cppflags: '-DNDEBUG',
			cflags: '-include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -Wstrict-prototypes',
			cxxflags: '-include CppUTest/MemoryLeakDetectorNewMacros.h -include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -pedantic-errors -Woverloaded-virtual -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -O2 -g'
		),
		tests: [
			'CppUTestExtTests:default'
		]
end

test_application_project :CppUTestTests, File.dirname(__FILE__) do |app|

	app.add_configuration :default,
		sources: [
        'tests/AllTests.cpp',
        'tests/SetPluginTest.cpp',
        'tests/CheatSheetTest.cpp',
        'tests/SimpleStringTest.cpp',
        'tests/CommandLineArgumentsTest.cpp',
        'tests/TestFailureTest.cpp',
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
        'tests/NullTestTest.cpp',
        'tests/TestRegistryTest.cpp',
        'tests/AllocationInCFile.c',
        'tests/PluginTest.cpp',
        'tests/TestResultTest.cpp',
        'tests/PreprocessorTest.cpp',
        'tests/TestUTestMacro.cpp',
        'tests/AllocationInCppFile.cpp',
        'tests/UtestTest.cpp'
			],
		toolchain: gcc(
			cppflags: '-DNDEBUG',
			cflags: '-include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -Wstrict-prototypes',
			cxxflags: '-include CppUTest/MemoryLeakDetectorNewMacros.h -include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -pedantic-errors -Woverloaded-virtual -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -O2 -g',
			ldflags: '-lstdc++'
		),
		dependencies: [
			'CppUTest:default'
		]
end

test_application_project :CppUTestExtTests, File.dirname(__FILE__) do |app|

	app.add_configuration :default,
		sources: [
        'tests/CppUTestExt/AllTests.cpp',
        'tests/CppUTestExt/TestMockActualFunctionCall.cpp',
        'tests/CppUTestExt/TestMockSupport.cpp',
        'tests/CppUTestExt/TestCodeMemoryReportFormatter.cpp',
        'tests/CppUTestExt/TestMockCheatSheet.cpp',
        'tests/CppUTestExt/TestMockSupport_c.cpp',
        'tests/CppUTestExt/TestGMock.cpp',
        'tests/CppUTestExt/TestMockExpectedFunctionCall.cpp',
        'tests/CppUTestExt/TestMockSupport_cCFile.c',
        'tests/CppUTestExt/TestGTest.cpp',
        'tests/CppUTestExt/TestMockExpectedFunctionsList.cpp',
        'tests/CppUTestExt/TestMemoryReportAllocator.cpp',
        'tests/CppUTestExt/TestMockFailure.cpp',
        'tests/CppUTestExt/TestOrderedTest.cpp',
        'tests/CppUTestExt/TestMemoryReportFormatter.cpp',
        'tests/CppUTestExt/TestMemoryReporterPlugin.cpp',
        'tests/CppUTestExt/TestMockPlugin.cpp'
			],
		toolchain: gcc(
			cppflags: '-DNDEBUG',
			cflags: '-include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -Wstrict-prototypes',
			cxxflags: '-include CppUTest/MemoryLeakDetectorNewMacros.h -include CppUTest/MemoryLeakDetectorMallocMacros.h -Weverything -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wsign-conversion -Wno-padded -Wno-disabled-macro-expansion -Werror -pedantic-errors -Woverloaded-virtual -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -O2 -g',
			ldflags: '-lstdc++',
			include_paths: [
				'include/CppUTestExt/CppUTestGMock',
				'include/CppUTestExt/CppUTestGTest'
			]
		),
		dependencies: [
			'CppUTest:default', 'CppUTestExt:default'
		]
end
