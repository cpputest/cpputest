#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakAllocator.h"
#include "CppUTest/MemoryLeakDetector.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestTestingFixture.h"

#include "AllocationInCppFile.h"
extern "C"
{
#include "AllocationInCFile.h"
}

TEST_GROUP(MemoryLeakOverridesToBeUsedInProductionCode)
{
	MemoryLeakDetector* memLeakDetector;
	void setup()
	{
		memLeakDetector = MemoryLeakWarningPlugin::getGlobalDetector();
	}

};

#if CPPUTEST_USE_MEM_LEAK_DETECTION

TEST(MemoryLeakOverridesToBeUsedInProductionCode, OperatorNewMacroOverloadViaIncludeFileWorks)
{
	char* leak = newAllocation();
	STRCMP_CONTAINS("AllocationInCppFile.cpp", memLeakDetector->report(mem_leak_period_checking));
	delete leak;
}

TEST(MemoryLeakOverridesToBeUsedInProductionCode, OperatorNewArrayMacroOverloadViaIncludeFileWorks)
{
	char* leak = newArrayAllocation();
	STRCMP_CONTAINS("AllocationInCppFile.cpp", memLeakDetector->report(mem_leak_period_checking));
	delete[] leak;
}

TEST(MemoryLeakOverridesToBeUsedInProductionCode, MallocOverrideWorks)
{
	char* leak = mallocAllocation();
	STRCMP_CONTAINS("AllocationInCFile.c", memLeakDetector->report(mem_leak_period_checking));
	freeAllocation(leak);
}

TEST(MemoryLeakOverridesToBeUsedInProductionCode, MallocWithButFreeWithoutLeakDetectionDoesntCrash)
{
	char* leak = mallocAllocation();
	freeAllocationWithoutMacro(leak);
	STRCMP_CONTAINS("Memory leak reports about malloc and free can be caused", memLeakDetector->report(mem_leak_period_checking));
	memLeakDetector->removeMemoryLeakInformationWithoutCheckingOrDeallocating(leak);
}

TEST(MemoryLeakOverridesToBeUsedInProductionCode, OperatorNewOverloadingWithoutMacroWorks)
{
	char* leak = newAllocationWithoutMacro();
	STRCMP_CONTAINS("unknown", memLeakDetector->report(mem_leak_period_checking));
	delete leak;
}

TEST(MemoryLeakOverridesToBeUsedInProductionCode, OperatorNewArrayOverloadingWithoutMacroWorks)
{
	char* leak = newArrayAllocationWithoutMacro();
	STRCMP_CONTAINS("unknown", memLeakDetector->report(mem_leak_period_checking));
	delete[] leak;
}

#else

TEST(MemoryLeakOverridesToBeUsedInProductionCode, MemoryOverridesAreDisabled)
{
	char* leak = newAllocation();
	STRCMP_EQUAL("No memory leaks were detected.", memLeakDetector->report(mem_leak_period_checking));
	delete leak;
}

#endif

TEST_GROUP(OutOfMemoryTestsForOperatorNew)
{
	MemoryLeakAllocator* no_memory_allocator;
	void setup()
	{
		no_memory_allocator = new NullUnknownAllocator;
		MemoryLeakAllocator::setCurrentNewAllocator(no_memory_allocator);
		MemoryLeakAllocator::setCurrentNewArrayAllocator(no_memory_allocator);
	}
	void teardown()
	{
		MemoryLeakAllocator::setCurrentNewAllocatorToDefault();
		MemoryLeakAllocator::setCurrentNewArrayAllocatorToDefault();
		delete no_memory_allocator;
	}
};

#if CPPUTEST_USE_MEM_LEAK_DETECTION

#if CPPUTEST_USE_STD_CPP_LIB

TEST(OutOfMemoryTestsForOperatorNew, FailingNewOperatorThrowsAnExceptionWhenUsingStdCppNew)
{
	try {
		new char;
		FAIL("Should have thrown an exception!")
	}
	catch (std::bad_alloc) {
	}
}

TEST(OutOfMemoryTestsForOperatorNew, FailingNewArrayOperatorThrowsAnExceptionWhenUsingStdCppNew)
{
	try {
		new char[10];
		FAIL("Should have thrown an exception!")
	}
	catch (std::bad_alloc) {
	}
}

#else

TEST(OutOfMemoryTestsForOperatorNew, FailingNewOperatorReturnsNull)
{
	POINTERS_EQUAL(NULL, new char);
}

TEST(OutOfMemoryTestsForOperatorNew, FailingNewArrayOperatorReturnsNull)
{
	POINTERS_EQUAL(NULL, new char[10]);
}

#endif

#undef new

#if CPPUTEST_USE_STD_CPP_LIB

TEST(OutOfMemoryTestsForOperatorNew, FailingNewOperatorThrowsAnExceptionWhenUsingStdCppNewWithoutOverride)
{
	try {
		new char;
		FAIL("Should have thrown an exception!")
	}
	catch (std::bad_alloc) {
	}
}

TEST(OutOfMemoryTestsForOperatorNew, FailingNewArrayOperatorThrowsAnExceptionWhenUsingStdCppNewWithoutOverride)
{
	try {
		new char[10];
		FAIL("Should have thrown an exception!")
	}
	catch (std::bad_alloc) {
	}
}
#else

TEST(OutOfMemoryTestsForOperatorNew, FailingNewOperatorReturnsNullWithoutOverride)
{
	POINTERS_EQUAL(NULL, new char);
}

TEST(OutOfMemoryTestsForOperatorNew, FailingNewArrayOperatorReturnsNullWithoutOverride)
{
	POINTERS_EQUAL(NULL, new char[10]);
}

#endif

#endif
