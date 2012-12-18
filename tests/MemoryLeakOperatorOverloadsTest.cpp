#include "CppUTest/TestHarness.h"
#include "CppUTest/TestMemoryAllocator.h"
#include "CppUTest/MemoryLeakDetector.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestTestingFixture.h"
#include "AllocationInCppFile.h"

extern "C"
{
#include "CppUTest/TestHarness_c.h"
#include "AllocationInCFile.h"
}

TEST_GROUP(BasicBehavior)
{
};

TEST(BasicBehavior, CanDeleteNullPointers)
{
	delete (char*) NULL;
	delete [] (char*) NULL;
}

#ifndef CPPUTEST_MEM_LEAK_DETECTION_DISABLED

TEST(BasicBehavior, deleteArrayInvalidatesMemory)
{
	unsigned char* memory = new unsigned char[10];
	PlatformSpecificMemset(memory, 0xAB, 10);
	delete [] memory;
	CHECK(memory[5] != 0xCB);
}

TEST(BasicBehavior, deleteInvalidatesMemory)
{
	unsigned char* memory = new unsigned char;
	*memory = 0xAD;
	delete memory;
	CHECK(*memory != 0xAD);
}

void deleteUnallocatedMemory()
{
	delete (char*) 0x1234678;
	FAIL("Should never come here");
}

TEST(BasicBehavior, deleteWillNotThrowAnExceptionWhenDeletingUnallocatedMemoryButCanStillCauseTestFailures)
{
	/*
	 * Test failure might cause an exception. But according to C++ standard, you aren't allowed
	 * to throw exceptions in the delete function. If you do that, it will call std::terminate.
	 * Therefore, the delete will need to fail without exceptions.
	 */
	MemoryLeakFailure* defaultReporter = MemoryLeakWarningPlugin::getGlobalFailureReporter();
	TestTestingFixture fixture;
	fixture.setTestFunction(deleteUnallocatedMemory);
	fixture.runAllTests();
	LONGS_EQUAL(1, fixture.getFailureCount());
	POINTERS_EQUAL(defaultReporter, MemoryLeakWarningPlugin::getGlobalFailureReporter());
}

#endif

#if CPPUTEST_USE_MALLOC_MACROS

/* This include is added because *sometimes* the cstdlib does an #undef. This should have been prevented */
#if CPPUTEST_USE_STD_CPP_LIB
#include <cstdlib>
#endif

TEST(BasicBehavior, bothMallocAndFreeAreOverloaded)
{
	void* memory = cpputest_malloc_location(sizeof(char), "file", 10);
	free(memory);

	memory = malloc(sizeof(unsigned char));
	cpputest_free_location(memory, "file", 10);
}

#endif

#if CPPUTEST_USE_MEM_LEAK_DETECTION

TEST(BasicBehavior, freeInvalidatesMemory)
{
	unsigned char* memory = (unsigned char*) cpputest_malloc(sizeof(unsigned char));
	*memory = 0xAD;
	cpputest_free(memory);
	CHECK(*memory != 0xAD);
}
#endif

TEST_GROUP(MemoryLeakOverridesToBeUsedInProductionCode)
{
	MemoryLeakDetector* memLeakDetector;
	void setup()
	{
		memLeakDetector = MemoryLeakWarningPlugin::getGlobalDetector();
	}

};

#if CPPUTEST_USE_MALLOC_MACROS

TEST(MemoryLeakOverridesToBeUsedInProductionCode, MallocOverrideIsUsed)
{
	int memLeaks = memLeakDetector->totalMemoryLeaks(mem_leak_period_checking);
	void* memory = malloc(10);
	LONGS_EQUAL(memLeaks+1, memLeakDetector->totalMemoryLeaks(mem_leak_period_checking));
	free (memory);
}

#endif

TEST(MemoryLeakOverridesToBeUsedInProductionCode, UseNativeMallocByTemporarlySwitchingOffMalloc)
{
	int memLeaks = memLeakDetector->totalMemoryLeaks(mem_leak_period_checking);
#if CPPUTEST_USE_MALLOC_MACROS
	#define saved_malloc malloc
	#define saved_free free
	#undef malloc
	#undef free
#endif
	void* memory = malloc(10);
	LONGS_EQUAL(memLeaks, memLeakDetector->totalMemoryLeaks(mem_leak_period_checking));
	free (memory);

#if CPPUTEST_USE_MALLOC_MACROS
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#endif
}

/* TEST... allowing for a new overload in a class */
class NewDummyClass
{
public:
	static bool overloaded_new_called;

#if CPPUTEST_USE_NEW_MACROS
	#undef new
#endif
	void* operator new (size_t size)
#if CPPUTEST_USE_NEW_MACROS
	#include "CppUTest/MemoryLeakDetectorNewMacros.h"
#endif
	{
		overloaded_new_called = true;
		return malloc(size);
	}
	void dummyFunction()
	{
		char* memory = new char;
		delete memory;
	}
};
bool NewDummyClass::overloaded_new_called = false;

TEST(MemoryLeakOverridesToBeUsedInProductionCode, NoSideEffectsFromTurningOffNewMacros)
{
	/*
	 * Interesting effect of wrapping the operator new around the macro is
	 * that the actual new that is called is a different one than expected.
	 *
	 * The overloaded operator new doesn't actually ever get called.
	 *
	 * This might come as a surprise, so it is important to realize!
	 */
	NewDummyClass dummy;
	dummy.dummyFunction();
	// CHECK(dummy.overloaded_new_called);
}

TEST(MemoryLeakOverridesToBeUsedInProductionCode, UseNativeNewByTemporarlySwitchingOffNew)
{
#if CPPUTEST_USE_NEW_MACROS
	#undef new
	#undef delete
#endif
	char* memory = new char[10];
	delete [] memory;
#if CPPUTEST_USE_NEW_MACROS
	#include "CppUTest/MemoryLeakDetectorNewMacros.h"
#endif
}


#if CPPUTEST_USE_MEM_LEAK_DETECTION

TEST(MemoryLeakOverridesToBeUsedInProductionCode, OperatorNewMacroOverloadViaIncludeFileWorks)
{
	char* leak = newAllocation();
	STRCMP_NOCASE_CONTAINS("AllocationInCppFile.cpp", memLeakDetector->report(mem_leak_period_checking));
	delete leak;
}

TEST(MemoryLeakOverridesToBeUsedInProductionCode, OperatorNewArrayMacroOverloadViaIncludeFileWorks)
{
	char* leak = newArrayAllocation();
	STRCMP_NOCASE_CONTAINS("AllocationInCppFile.cpp", memLeakDetector->report(mem_leak_period_checking));
	delete[] leak;
}

TEST(MemoryLeakOverridesToBeUsedInProductionCode, MallocOverrideWorks)
{
	char* leak = mallocAllocation();
	STRCMP_NOCASE_CONTAINS("AllocationInCFile.c", memLeakDetector->report(mem_leak_period_checking));
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
	TestMemoryAllocator* no_memory_allocator;
	void setup()
	{
		no_memory_allocator = new NullUnknownAllocator;
		setCurrentNewAllocator(no_memory_allocator);
		setCurrentNewArrayAllocator(no_memory_allocator);
	}

	void teardown()
	{
		setCurrentNewAllocatorToDefault();
		setCurrentNewArrayAllocatorToDefault();
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
	catch (std::bad_alloc&) {
	}
}

TEST(OutOfMemoryTestsForOperatorNew, FailingNewArrayOperatorThrowsAnExceptionWhenUsingStdCppNew)
{
	try {
		new char[10];
		FAIL("Should have thrown an exception!")
	}
	catch (std::bad_alloc&) {
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
	catch (std::bad_alloc&) {
	}
}

TEST(OutOfMemoryTestsForOperatorNew, FailingNewArrayOperatorThrowsAnExceptionWhenUsingStdCppNewWithoutOverride)
{
	try {
		new char[10];
		FAIL("Should have thrown an exception!")
	}
	catch (std::bad_alloc&) {
	}
}

TEST(OutOfMemoryTestsForOperatorNew, FailingNewOperatorReturnsNullWithoutOverride)
{
	POINTERS_EQUAL(NULL, new (std::nothrow) char);
}

TEST(OutOfMemoryTestsForOperatorNew, FailingNewArrayOperatorReturnsNullWithoutOverride)
{
	POINTERS_EQUAL(NULL, new (std::nothrow) char[10]);
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
