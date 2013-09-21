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

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/PlatformSpecificFunctions.h"

TEST_GROUP(UtestShell)
{
	TestTestingFixture fixture;
};

static void _failMethod()
{
	FAIL("This test fails");
}

static void _passingTestMethod()
{
	CHECK(true);
}

TEST(UtestShell, compareDoubles)
{
	double zero = 0.0;
	double not_a_number = zero / zero;
	CHECK(doubles_equal(1.0, 1.001, 0.01));
	CHECK(!doubles_equal(not_a_number, 1.001, 0.01));
	CHECK(!doubles_equal(1.0, not_a_number, 0.01));
	CHECK(!doubles_equal(1.0, 1.001, not_a_number));
	CHECK(!doubles_equal(1.0, 1.1, 0.05));

	double a = 1.2345678;
	CHECK(doubles_equal(a, a, 0.000000001));
}

IGNORE_TEST(UtestShell, IgnoreTestAccessingFixture)
{
	CHECK(&fixture != 0);
}

TEST(UtestShell, MacrosUsedInSetup)
{
	IGNORE_ALL_LEAKS_IN_TEST();
	fixture.setSetup(_failMethod);
	fixture.setTestFunction(_passingTestMethod);
	fixture.runAllTests();
	LONGS_EQUAL(1, fixture.getFailureCount());
}

TEST(UtestShell, MacrosUsedInTearDown)
{
	IGNORE_ALL_LEAKS_IN_TEST();
	fixture.setTeardown(_failMethod);
	fixture.setTestFunction(_passingTestMethod);
	fixture.runAllTests();
	LONGS_EQUAL(1, fixture.getFailureCount());
}

static int teardownCalled = 0;

static void _teardownMethod()
{
	teardownCalled++;
}

TEST(UtestShell, TeardownCalledAfterTestFailure)
{
	teardownCalled = 0;
	IGNORE_ALL_LEAKS_IN_TEST();
	fixture.setTeardown(_teardownMethod);
	fixture.setTestFunction(_failMethod);
	fixture.runAllTests();
	LONGS_EQUAL(1, fixture.getFailureCount());
	LONGS_EQUAL(1, teardownCalled);
}

static int stopAfterFailure = 0;
static void _stopAfterFailureMethod()
{
	FAIL("fail");
	stopAfterFailure++;
}

TEST(UtestShell, TestStopsAfterTestFailure)
{
	IGNORE_ALL_LEAKS_IN_TEST();
	stopAfterFailure = 0;
	fixture.setTestFunction(_stopAfterFailureMethod);
	fixture.runAllTests();
	CHECK(fixture.hasTestFailed());
	LONGS_EQUAL(1, fixture.getFailureCount());
	LONGS_EQUAL(0, stopAfterFailure);
}

TEST(UtestShell, TestStopsAfterSetupFailure)
{
	stopAfterFailure = 0;
	fixture.setSetup(_stopAfterFailureMethod);
	fixture.setTeardown(_stopAfterFailureMethod);
	fixture.setTestFunction(_failMethod);
	fixture.runAllTests();
	LONGS_EQUAL(2, fixture.getFailureCount());
	LONGS_EQUAL(0, stopAfterFailure);
}

#if CPPUTEST_USE_STD_CPP_LIB

static bool destructorWasCalledOnFailedTest = false;

static void _destructorCalledForLocalObjects()
{
	SetBooleanOnDestructorCall pleaseCallTheDestructor(destructorWasCalledOnFailedTest);
	destructorWasCalledOnFailedTest = false;
	FAIL("fail");
}

TEST(UtestShell, DestructorIsCalledForLocalObjectsWhenTheTestFails)
{
	fixture.setTestFunction(_destructorCalledForLocalObjects);
	fixture.runAllTests();
	CHECK(destructorWasCalledOnFailedTest);
}

#endif

TEST_BASE(MyOwnTest)
{
	MyOwnTest() :
		inTest(false)
	{
	}
	bool inTest;

	void setup()
	{
		CHECK(!inTest);
		inTest = true;
	}
	void teardown()
	{
		CHECK(inTest);
		inTest = false;
	}
};

TEST_GROUP_BASE(UtestMyOwn, MyOwnTest)
{
};

TEST(UtestMyOwn, test)
{
	CHECK(inTest);
}

class NullParameterTest: public UtestShell
{
};

TEST(UtestMyOwn, NullParameters)
{
	NullParameterTest nullTest; /* Bug fix tests for creating a test without a name, fix in SimpleString */
	TestRegistry* reg = TestRegistry::getCurrentRegistry();
	nullTest.shouldRun(reg->getGroupFilter(), reg->getNameFilter());
}

class AllocateAndDeallocateInConstructorAndDestructor
{
	char* memory_;
	char* morememory_;
public:
	AllocateAndDeallocateInConstructorAndDestructor()
	{
		memory_ = new char[100];
		morememory_ = NULL;
	}
	void allocateMoreMemory()
	{
		morememory_ = new char[123];
	}

	~AllocateAndDeallocateInConstructorAndDestructor()
	{
		delete [] memory_;
		delete [] morememory_;
	}
};

TEST_GROUP(CanHaveMemberVariablesInTestGroupThatAllocateMemoryWithoutCausingMemoryLeaks)
{
	AllocateAndDeallocateInConstructorAndDestructor dummy;
};

TEST(CanHaveMemberVariablesInTestGroupThatAllocateMemoryWithoutCausingMemoryLeaks, testInTestGroupName)
{
	dummy.allocateMoreMemory();
}

