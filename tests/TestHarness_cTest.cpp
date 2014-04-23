/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/TestHarness_c.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/PlatformSpecificFunctions.h"


extern "C" int setup_teardown_was_called_in_test_group_in_C;
extern "C" int test_was_called_in_test_group_in_C;
int setup_teardown_was_called_in_test_group_in_C = 0;
int test_was_called_in_test_group_in_C = 0;

TEST_GROUP_C(TestGroupInC)
{
	TEST_GROUP_C_SETUP_WRAPPER(TestGroupInC)
	TEST_GROUP_C_TEARDOWN_WRAPPER(TestGroupInC)
};

TEST_GROUP_C_WRAPPER(TestGroupInC, checkThatTheTestHasRun)

/*
 * This test is a bit strange. They use the fact that you can do -r2 for repeating the same run.
 * When you do so, the same statics will be shared and therefore we can test whether the setup/teardown is run
 * correctly.
 */

TEST(TestGroupInC, setupHasBeenCalled)
{
	test_was_called_in_test_group_in_C++;
	/* Increased in setup, decreased in teardown. So at this point it must be 1 also on a multiple run */
	LONGS_EQUAL(1, setup_teardown_was_called_in_test_group_in_C);
}

static bool hasDestructorOfTheDestructorCheckedBeenCalled;

class HasTheDestructorBeenCalledChecker
{
public:
	HasTheDestructorBeenCalledChecker(){}
	~HasTheDestructorBeenCalledChecker() { hasDestructorOfTheDestructorCheckedBeenCalled = true; }
};

TEST_GROUP(TestHarness_c)
{
	TestTestingFixture* fixture;
	TEST_SETUP()
	{
		hasDestructorOfTheDestructorCheckedBeenCalled = false;
		fixture = new TestTestingFixture();
	}
	TEST_TEARDOWN()
	{
		delete fixture;
	}
};

static void _failIntMethod()
{
	HasTheDestructorBeenCalledChecker checker;
	CHECK_EQUAL_C_INT(1, 2);
}

TEST(TestHarness_c, checkInt)
{
	CHECK_EQUAL_C_INT(2, 2);
	fixture->setTestFunction(_failIntMethod);
	fixture->runAllTests();
	fixture->assertPrintContains("expected <1 0x1>\n	but was  <2 0x2>");
	fixture->assertPrintContains("arness_c");
	CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled)
}

static void _failRealMethod()
{
	HasTheDestructorBeenCalledChecker checker;
	CHECK_EQUAL_C_REAL(1.0, 2.0, 0.5);
}

TEST(TestHarness_c, checkReal)
{
	CHECK_EQUAL_C_REAL(1.0, 1.1, 0.5);
	fixture->setTestFunction(_failRealMethod);
	fixture->runAllTests();
	fixture->assertPrintContains("expected <1>\n	but was  <2>");
	fixture->assertPrintContains("arness_c");
	CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled)
}

static void _failCharMethod()
{
	HasTheDestructorBeenCalledChecker checker;
	CHECK_EQUAL_C_CHAR('a', 'c');
}

TEST(TestHarness_c, checkChar)
{
	CHECK_EQUAL_C_CHAR('a', 'a');
	fixture->setTestFunction(_failCharMethod);
	fixture->runAllTests();
	fixture->assertPrintContains("expected <a>\n	but was  <c>");
	fixture->assertPrintContains("arness_c");
	CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled)
}

static void _failStringMethod()
{
	HasTheDestructorBeenCalledChecker checker;
	CHECK_EQUAL_C_STRING("Hello", "Hello World");
}

TEST(TestHarness_c, checkString)
{
	CHECK_EQUAL_C_STRING("Hello", "Hello");
	fixture->setTestFunction(_failStringMethod);
	fixture->runAllTests();

	StringEqualFailure failure(UtestShell::getCurrent(), "file", 1, "Hello", "Hello World");
	fixture->assertPrintContains(failure.getMessage());
	fixture->assertPrintContains("arness_c");
	CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled)
}

static void _failTextMethod()
{
	HasTheDestructorBeenCalledChecker checker;
	FAIL_TEXT_C("Booo");
}

TEST(TestHarness_c, checkFailText)
{
	fixture->setTestFunction(_failTextMethod);
	fixture->runAllTests();
	fixture->assertPrintContains("Booo");
	fixture->assertPrintContains("arness_c");
	CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled)
}

static void _failMethod()
{
	HasTheDestructorBeenCalledChecker checker;
	FAIL_C();
}

TEST(TestHarness_c, checkFail)
{
	fixture->setTestFunction(_failMethod);
	fixture->runAllTests();
	LONGS_EQUAL(1, fixture->getFailureCount());
	fixture->assertPrintContains("arness_c");
	CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled)
}

static void _CheckMethod()
{
	HasTheDestructorBeenCalledChecker checker;
	CHECK_C(false);
}

TEST(TestHarness_c, checkCheck)
{
	CHECK_C(true);
	fixture->setTestFunction(_CheckMethod);
	fixture->runAllTests();
	LONGS_EQUAL(1, fixture->getFailureCount());
	CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled)
}

#if CPPUTEST_USE_MEM_LEAK_DETECTION

TEST(TestHarness_c, cpputest_malloc_out_of_memory)
{
	cpputest_malloc_set_out_of_memory();
	CHECK(0 == cpputest_malloc(100));

	cpputest_malloc_set_not_out_of_memory();
	void * mem = cpputest_malloc(100);
	CHECK(0 != mem);
	cpputest_free(mem);
}

TEST(TestHarness_c, cpputest_malloc_out_of_memory_after_n_mallocs)
{
	cpputest_malloc_set_out_of_memory_countdown(3);
	void * m1 = cpputest_malloc(10);
	void * m2 = cpputest_malloc(11);
	void * m3 = cpputest_malloc(12);
	CHECK(m1);
	CHECK(m2);
	POINTERS_EQUAL(0, m3);
	cpputest_malloc_set_not_out_of_memory();
	cpputest_free(m1);
	cpputest_free(m2);
}

TEST(TestHarness_c, cpputest_malloc_out_of_memory_after_0_mallocs)
{
	cpputest_malloc_set_out_of_memory_countdown(0);
	void * m1 = cpputest_malloc(10);
	CHECK(m1 == 0);
	cpputest_malloc_set_not_out_of_memory();
}

TEST(TestHarness_c, count_mallocs)
{
	cpputest_malloc_count_reset();
	void * m1 = cpputest_malloc(10);
	void * m2 = cpputest_malloc(11);
	void * m3 = cpputest_malloc(12);
	cpputest_free(m1);
	cpputest_free(m2);
	cpputest_free(m3);
	LONGS_EQUAL(3, cpputest_malloc_get_count());
}

TEST(TestHarness_c, cpputest_calloc)
{
	void * mem = cpputest_calloc(10, 10);
	CHECK(0 != mem);
	cpputest_free(mem);
}

TEST(TestHarness_c, cpputest_realloc_larger)
{
	const char* number_string = "123456789";

	char* mem1 = (char*) cpputest_malloc(10);

	PlatformSpecificStrCpy(mem1, number_string);
	CHECK(mem1 != 0);

	char* mem2 = (char*) cpputest_realloc(mem1, 1000);

	CHECK(mem2 != 0);
	STRCMP_EQUAL(number_string, mem2);

	cpputest_free(mem2);
}

#include "CppUTest/MemoryLeakDetector.h"

TEST(TestHarness_c, macros)
{
#if CPPUTEST_USE_MALLOC_MACROS
	MemoryLeakDetector* memLeakDetector = MemoryLeakWarningPlugin::getGlobalDetector();
	int memLeaks = memLeakDetector->totalMemoryLeaks(mem_leak_period_checking);
#endif
	void* mem1 = malloc(10);
	void* mem2 = calloc(10, 20);
	void* mem3 = realloc(mem2, 100);
#if CPPUTEST_USE_MALLOC_MACROS
	LONGS_EQUAL(memLeaks + 2, memLeakDetector->totalMemoryLeaks(mem_leak_period_checking));
#endif
	free(mem1);
	free(mem3);
#if CPPUTEST_USE_MALLOC_MACROS
	LONGS_EQUAL(memLeaks, memLeakDetector->totalMemoryLeaks(mem_leak_period_checking));
#endif

}

TEST(TestHarness_c, callocInitializedToZero)
{
	char* mem = (char*) calloc(20, sizeof(char));
	for (int i = 0; i < 20; i++)
		CHECK(mem[i] == 0);
	free(mem);
}

TEST(TestHarness_c, callocShouldReturnNULLWhenOutOfMeory)
{
	cpputest_malloc_set_out_of_memory_countdown(0);
	void * m = cpputest_calloc(1, 1);
	CHECK(m == 0);
	cpputest_malloc_set_not_out_of_memory();
}
#endif

