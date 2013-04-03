
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

static bool lineOfCodeExecutedAfterCheck;

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text) CHECK_TEST_FAILS_PROPER_WITH_TEXT_LOCATION(text, fixture, __FILE__, __LINE__)

static void CHECK_TEST_FAILS_PROPER_WITH_TEXT_LOCATION(const char* text, TestTestingFixture& fixture, const char* file, int line)
{
	if (fixture.getFailureCount() != 1)
		FAIL_LOCATION(StringFromFormat("Expected one test failure, but got %d amount of test failures", fixture.getFailureCount()).asCharString(), file, line);

	STRCMP_CONTAINS_LOCATION(text, fixture.output_->getOutput().asCharString(), file, line);

	if (lineOfCodeExecutedAfterCheck)
		FAIL_LOCATION("The test should jump/throw on failure and not execute the next line. However, the next line was executed.", file, line)

}

TEST_GROUP(UnitTestMacros)
{
	TestTestingFixture fixture;
	void setup()
	{
		lineOfCodeExecutedAfterCheck = false;
	}

	void runTestWithMethod(void(*method)())
	{
		fixture.setTestFunction(method);
		fixture.runAllTests();
	}
};

static void _failingTestMethodWithFAIL()
{
	FAIL("This test fails");
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FAILMakesTheTestFailPrintsTheRightResultAndStopsExecuting)
{
	runTestWithMethod(_failingTestMethodWithFAIL);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("This test fails");
}

TEST(UnitTestMacros, FAILWillPrintTheFileThatItFailed)
{
	runTestWithMethod(_failingTestMethodWithFAIL);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT(__FILE__);
}

TEST(UnitTestMacros, FAILBehavesAsAProperMacro)
{
	if (this == 0) FAIL("")
	else CHECK(true)

	if (this != 0) CHECK(true)
	else FAIL("")
}

IGNORE_TEST(UnitTestMacros, FAILworksInAnIngoredTest)
{
	FAIL("die!");
}

static void _failingTestMethodWithCHECK()
{
	CHECK(false);
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithCHECK)
{
	runTestWithMethod(_failingTestMethodWithCHECK);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("false");
}

TEST(UnitTestMacros, CHECKBehavesAsProperMacro)
{
	if (this == 0) CHECK(false)
	else CHECK(true)
}

static void _failingTestMethodWithCHECK_TEXT()
{
	CHECK_TEXT(false, "Failed because it failed");
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithCHECK_TEXT)
{
	runTestWithMethod(_failingTestMethodWithCHECK_TEXT);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, CHECK_TEXTBehavesAsProperMacro)
{
	if (this == 0) CHECK_TEXT(false, "false")
	else CHECK_TEXT(true, "true")
}

IGNORE_TEST(UnitTestMacros, CHECKWorksInAnIgnoredTest)
{
	CHECK_TEXT(false, "false");
}

static void _failingTestMethodWithCHECK_TRUE()
{
	CHECK_TRUE(false);
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithCHECK_TRUE)
{
	runTestWithMethod(_failingTestMethodWithCHECK_TRUE);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE");
}

TEST(UnitTestMacros, CHECK_TRUEBehavesAsProperMacro)
{
	if (this == 0) CHECK_TRUE(false)
	else CHECK_TRUE(true)
}

IGNORE_TEST(UnitTestMacros, CHECK_TRUEWorksInAnIgnoredTest)
{
	CHECK_TRUE(false)
}

static void _failingTestMethodWithCHECK_FALSE()
{
	CHECK_FALSE(true);
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithCHECK_FALSE)
{
	runTestWithMethod(_failingTestMethodWithCHECK_FALSE);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE");
}

TEST(UnitTestMacros, CHECK_FALSEBehavesAsProperMacro)
{
	if (this == 0) CHECK_FALSE(true)
	else CHECK_FALSE(false)
}

IGNORE_TEST(UnitTestMacros, CHECK_FALSEWorksInAnIgnoredTest)
{
	CHECK_FALSE(true)
}

static void _failingTestMethodWithCHECK_EQUAL()
{
	CHECK_EQUAL(1, 2);
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithCHECK_EQUAL)
{
	runTestWithMethod(_failingTestMethodWithCHECK_EQUAL);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
}

TEST(UnitTestMacros, CHECK_EQUALBehavesAsProperMacro)
{
	if (this == 0) CHECK_EQUAL(1, 2)
	else CHECK_EQUAL(1, 1)
}

IGNORE_TEST(UnitTestMacros, CHECK_EQUALWorksInAnIgnoredTest)
{
	CHECK_EQUAL(1, 2)
}

static void _failingTestMethodWithSTRCMP_CONTAINS()
{
	STRCMP_CONTAINS("hello", "world");
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithSTRCMP_CONTAINS)
{
	runTestWithMethod(_failingTestMethodWithSTRCMP_CONTAINS);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
}

TEST(UnitTestMacros, STRCMP_CONTAINSBehavesAsProperMacro)
{
	if (this == 0) STRCMP_CONTAINS("1", "2")
	else STRCMP_CONTAINS("1", "1")
}

IGNORE_TEST(UnitTestMacros, STRCMP_CONTAINSWorksInAnIgnoredTest)
{
	STRCMP_CONTAINS("Hello", "World")
}

static void _failingTestMethodWithSTRCMP_NOCASE_CONTAINS()
{
	STRCMP_NOCASE_CONTAINS("hello", "WORLD");
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithSTRCMP_NOCASE_CONTAINS)
{
	runTestWithMethod(_failingTestMethodWithSTRCMP_NOCASE_CONTAINS);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("<WORLD>");
}

TEST(UnitTestMacros, STRCMP_NOCASE_CONTAINSBehavesAsProperMacro)
{
	if (this == 0) STRCMP_NOCASE_CONTAINS("never", "executed")
	else STRCMP_NOCASE_CONTAINS("hello", "HELLO WORLD")
}

IGNORE_TEST(UnitTestMacros, STRCMP_NO_CASE_CONTAINSWorksInAnIgnoredTest)
{
	STRCMP_NOCASE_CONTAINS("Hello", "World")
}

static void _failingTestMethodWithLONGS_EQUAL()
{
	LONGS_EQUAL(1, 0xff);
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailurePrintHexOutputForLongInts)
{
	runTestWithMethod(_failingTestMethodWithLONGS_EQUAL);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <  1 0x01>");
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <255 0xff>");
}

TEST(UnitTestMacros, LONGS_EQUALBehavesAsProperMacro)
{
	if (this == 0) LONGS_EQUAL(1, 2)
	else LONGS_EQUAL(10, 10)
}

IGNORE_TEST(UnitTestMacros, LONGS_EQUALWorksInAnIgnoredTest)
{
	LONGS_EQUAL(11, 22)
}

static void _failingTestMethodWithBYTES_EQUAL()
{
	BYTES_EQUAL('a', 'b');
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithBYTES_EQUAL)
{
	runTestWithMethod(_failingTestMethodWithBYTES_EQUAL);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("<97 0x61>");
}

TEST(UnitTestMacros, BYTES_EQUALBehavesAsProperMacro)
{
	if (this == 0) BYTES_EQUAL('a', 'b')
	else BYTES_EQUAL('c', 'c')
}

IGNORE_TEST(UnitTestMacros, BYTES_EQUALWorksInAnIgnoredTest)
{
	BYTES_EQUAL('q', 'w')
}

static void _failingTestMethodWithPOINTERS_EQUAL()
{
	POINTERS_EQUAL((void*)0xa5a5, (void*)0xf0f0);
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailurePrintHexOutputForPointers)
{
	runTestWithMethod(_failingTestMethodWithPOINTERS_EQUAL);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
}

TEST(UnitTestMacros, POINTERS_EQUALBehavesAsProperMacro)
{
	if (this == 0) POINTERS_EQUAL(0, (void*) 0xbeefbeef)
	else POINTERS_EQUAL((void*)0xdeadbeef, (void*)0xdeadbeef)
}

IGNORE_TEST(UnitTestMacros, POINTERS_EQUALWorksInAnIgnoredTest)
{
	POINTERS_EQUAL((void*) 0xbeef, (void*) 0xdead)
}

static void _failingTestMethodWithDOUBLES_EQUAL()
{
	DOUBLES_EQUAL(0.12, 44.1, 0.3);
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithDOUBLES_EQUAL)
{
	runTestWithMethod(_failingTestMethodWithDOUBLES_EQUAL);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("0.12");
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("44.1");
}

TEST(UnitTestMacros, DOUBLES_EQUALBehavesAsProperMacro)
{
	if (this == 0) DOUBLES_EQUAL(0.0, 1.1, 0.0005)
	else DOUBLES_EQUAL(0.1, 0.2, 0.2)
}

IGNORE_TEST(UnitTestMacros, DOUBLES_EQUALWorksInAnIgnoredTest)
{
	DOUBLES_EQUAL(100.0, 0.0, 0.2)
}

static void _passingTestMethod()
{
	CHECK(true);
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, SuccessPrintsNothing)
{
	runTestWithMethod(_passingTestMethod);

	LONGS_EQUAL(0, fixture.getFailureCount());
	fixture.assertPrintContains(".\nOK (1 tests");
	CHECK(lineOfCodeExecutedAfterCheck);
}

static void _methodThatOnlyPrints()
{
	UT_PRINT("Hello World!");
}

TEST(UnitTestMacros, PrintPrintsWhateverPrintPrints)
{
	runTestWithMethod(_methodThatOnlyPrints);

	LONGS_EQUAL(0, fixture.getFailureCount());
	fixture.assertPrintContains("Hello World!");
	fixture.assertPrintContains(__FILE__);
}

static void _methodThatOnlyPrintsUsingSimpleStringFromFormat()
{
	UT_PRINT(StringFromFormat("Hello %s %d", "World!", 2009));
}

TEST(UnitTestMacros, PrintPrintsSimpleStringsForExampleThoseReturnedByFromString)
{
	runTestWithMethod(_methodThatOnlyPrintsUsingSimpleStringFromFormat);
	fixture.assertPrintContains("Hello World! 2009");
}

static int functionThatReturnsAValue()
{
	CHECK(0 == 0);
	LONGS_EQUAL(1,1);
	BYTES_EQUAL(0xab,0xab);
	CHECK_EQUAL(100,100);
	STRCMP_EQUAL("THIS", "THIS");
	DOUBLES_EQUAL(1.0, 1.0, .01);
	POINTERS_EQUAL(0, 0);
	return 0;
}

TEST(UnitTestMacros, allMacrosFromFunctionThatReturnsAValue)
{
	functionThatReturnsAValue();
}

#if CPPUTEST_USE_STD_CPP_LIB
static void _failingTestMethod_NoThrowWithCHECK_THROWS()
{
	CHECK_THROWS(int, (void) (1+2));
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithCHECK_THROWS_whenDoesntThrow)
{
	runTestWithMethod(_failingTestMethod_NoThrowWithCHECK_THROWS);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw nothing");
}

TEST(UnitTestMacros, SuccessWithCHECK_THROWS)
{
	CHECK_THROWS(int, throw 4);
}

static void _failingTestMethod_WrongThrowWithCHECK_THROWS()
{
	CHECK_THROWS(int, throw 4.3);
	lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, FailureWithCHECK_THROWS_whenWrongThrow)
{
	runTestWithMethod(_failingTestMethod_WrongThrowWithCHECK_THROWS);
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
	CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw a different type");
}

TEST(UnitTestMacros, MultipleCHECK_THROWS_inOneScope)
{
	CHECK_THROWS(int, throw 4);
	CHECK_THROWS(int, throw 4);
}
#endif
