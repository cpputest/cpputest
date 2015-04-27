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
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

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
    if (false) FAIL("")
    else CHECK(true)

    if (true) CHECK(true)
    else FAIL("")
}

IGNORE_TEST(UnitTestMacros, FAILworksInAnIgnoredTest)
{
    FAIL("die!"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _STRCMP_EQUALWithActualIsNULLTestMethod()
{
    STRCMP_EQUAL("ok", NULL);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_EQUALAndActualIsNULL)
{
    runTestWithMethod(_STRCMP_EQUALWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

static void _STRCMP_EQUALWithExpectedIsNULLTestMethod()
{
    STRCMP_EQUAL(NULL, "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_EQUALAndExpectedIsNULL)
{
    runTestWithMethod(_STRCMP_EQUALWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

static void _STRCMP_CONTAINSWithActualIsNULLTestMethod()
{
    STRCMP_CONTAINS("ok", NULL);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_CONTAINSAndActualIsNULL)
{
    runTestWithMethod(_STRCMP_CONTAINSWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

static void _STRCMP_CONTAINSWithExpectedIsNULLTestMethod()
{
    STRCMP_CONTAINS(NULL, "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_CONTAINSAndExpectedIsNULL)
{
    runTestWithMethod(_STRCMP_CONTAINSWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <>");
}

static void _STRNCMP_EQUALWithActualIsNULLTestMethod()
{
    STRNCMP_EQUAL("ok", NULL, 2);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRNCMP_EQUALAndActualIsNULL)
{
    runTestWithMethod(_STRNCMP_EQUALWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

static void _STRNCMP_EQUALWithExpectedIsNULLTestMethod()
{
    STRNCMP_EQUAL(NULL, "ok", 2);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRNCMP_EQUALAndExpectedIsNULL)
{
    runTestWithMethod(_STRNCMP_EQUALWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

static void _STRCMP_NOCASE_EQUALWithActualIsNULLTestMethod()
{
    STRCMP_NOCASE_EQUAL("ok", NULL);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_NOCASE_EQUALAndActualIsNULL)
{
    runTestWithMethod(_STRCMP_NOCASE_EQUALWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

static void _STRCMP_NOCASE_EQUALWithExpectedIsNULLTestMethod()
{
    STRCMP_NOCASE_EQUAL(NULL, "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_NOCASE_EQUALAndExpectedIsNULL)
{
    runTestWithMethod(_STRCMP_NOCASE_EQUALWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

static void _STRCMP_NOCASE_EQUALWithUnequalInputTestMethod()
{
    STRCMP_NOCASE_EQUAL("no", "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_NOCASE_EQUALAndUnequalInput)
{
    runTestWithMethod(_STRCMP_NOCASE_EQUALWithUnequalInputTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <ok>");
}

static void _STRCMP_NOCASE_CONTAINSWithActualIsNULLTestMethod()
{
    STRCMP_NOCASE_CONTAINS("ok", NULL);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_NOCASE_CONTAINSAndActualIsNULL)
{
    runTestWithMethod(_STRCMP_NOCASE_CONTAINSWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

static void _STRCMP_NOCASE_CONTAINSWithExpectedIsNULLTestMethod()
{
    STRCMP_NOCASE_CONTAINS(NULL, "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_NOCASE_CONTAINSAndExpectedIsNULL)
{
    runTestWithMethod(_STRCMP_NOCASE_CONTAINSWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <>");
}

static void _UNSIGNED_LONGS_EQUALTestMethod()
{
    UNSIGNED_LONGS_EQUAL(1, 1);
    UNSIGNED_LONGS_EQUAL(1, 0);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestUNSIGNED_LONGS_EQUAL)
{
    runTestWithMethod(_UNSIGNED_LONGS_EQUALTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0) 0x0>");
}

static void _failingTestMethodWithCHECK()
{
    CHECK(false);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK)
{
    runTestWithMethod(_failingTestMethodWithCHECK);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("false");
}

TEST(UnitTestMacros, CHECKBehavesAsProperMacro)
{
    if (false) CHECK(false)
    else CHECK(true)
}

static void _failingTestMethodWithCHECK_TEXT()
{
    CHECK_TEXT(false, "Failed because it failed");
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_TEXT)
{
    runTestWithMethod(_failingTestMethodWithCHECK_TEXT);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, CHECK_TEXTBehavesAsProperMacro)
{
    if (false) CHECK_TEXT(false, "false")
    else CHECK_TEXT(true, "true")
}

IGNORE_TEST(UnitTestMacros, CHECKWorksInAnIgnoredTest)
{
    CHECK_TEXT(false, "false"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithCHECK_TRUE()
{
    CHECK_TRUE(false);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_TRUE)
{
    runTestWithMethod(_failingTestMethodWithCHECK_TRUE);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE");
}

TEST(UnitTestMacros, CHECK_TRUEBehavesAsProperMacro)
{
    if (false) CHECK_TRUE(false)
    else CHECK_TRUE(true)
}

IGNORE_TEST(UnitTestMacros, CHECK_TRUEWorksInAnIgnoredTest)
{
    CHECK_TRUE(false) // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithCHECK_FALSE()
{
    CHECK_FALSE(true);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_FALSE)
{
    runTestWithMethod(_failingTestMethodWithCHECK_FALSE);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE");
}

TEST(UnitTestMacros, CHECK_FALSEBehavesAsProperMacro)
{
    if (false) CHECK_FALSE(true)
    else CHECK_FALSE(false)
}

IGNORE_TEST(UnitTestMacros, CHECK_FALSEWorksInAnIgnoredTest)
{
    CHECK_FALSE(true) // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithCHECK_EQUAL()
{
    CHECK_EQUAL(1, 2);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_EQUAL)
{
    runTestWithMethod(_failingTestMethodWithCHECK_EQUAL);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
}

static int countInCountingMethod;
static int _countingMethod()
{
    return countInCountingMethod++;
}

TEST(UnitTestMacros, passingCheckEqualWillNotBeEvaluatedMultipleTimesWithCHECK_EQUAL)
{
    countInCountingMethod = 0;
    CHECK_EQUAL(0, _countingMethod());

    LONGS_EQUAL(1, countInCountingMethod);
}

static void _failing_CHECK_EQUAL_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning()
{
    CHECK_EQUAL(12345, _countingMethod());
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, failing_CHECK_EQUAL_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning)
{
    runTestWithMethod(_failing_CHECK_EQUAL_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("WARNING:\n\tThe \"Actual Parameter\" parameter is evaluated multiple times resulting in different values.\n\tThus the value in the error message is probably incorrect.");
}

static void _failing_CHECK_EQUAL_WithExpectedBeingEvaluatesMultipleTimesWillGiveAWarning()
{
    CHECK_EQUAL(_countingMethod(), 12345);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, failing_CHECK_EQUAL_WithExpectedBeingEvaluatesMultipleTimesWillGiveAWarning)
{
    runTestWithMethod(_failing_CHECK_EQUAL_WithExpectedBeingEvaluatesMultipleTimesWillGiveAWarning);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("WARNING:\n\tThe \"Expected Parameter\" parameter is evaluated multiple times resulting in different values.\n\tThus the value in the error message is probably incorrect.");
}

TEST(UnitTestMacros, failing_CHECK_EQUAL_withParamatersThatDontChangeWillNotGiveAnyWarning)
{
    runTestWithMethod(_failingTestMethodWithCHECK_EQUAL);
    CHECK( ! fixture.output_->getOutput().contains("WARNING"));
}

TEST(UnitTestMacros, CHECK_EQUALBehavesAsProperMacro)
{
    if (false) CHECK_EQUAL(1, 2)
    else CHECK_EQUAL(1, 1)
}

IGNORE_TEST(UnitTestMacros, CHECK_EQUALWorksInAnIgnoredTest)
{
    CHECK_EQUAL(1, 2) // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRCMP_CONTAINS()
{
    STRCMP_CONTAINS("hello", "world");
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithSTRCMP_CONTAINS)
{
    runTestWithMethod(_failingTestMethodWithSTRCMP_CONTAINS);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
}

TEST(UnitTestMacros, STRCMP_CONTAINSBehavesAsProperMacro)
{
    if (false) STRCMP_CONTAINS("1", "2")
    else STRCMP_CONTAINS("1", "1")
}

IGNORE_TEST(UnitTestMacros, STRCMP_CONTAINSWorksInAnIgnoredTest)
{
    STRCMP_CONTAINS("Hello", "World") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

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
    if (false) STRCMP_NOCASE_CONTAINS("never", "executed")
    else STRCMP_NOCASE_CONTAINS("hello", "HELLO WORLD")
}

IGNORE_TEST(UnitTestMacros, STRCMP_NO_CASE_CONTAINSWorksInAnIgnoredTest)
{
    STRCMP_NOCASE_CONTAINS("Hello", "World") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithLONGS_EQUAL()
{
    LONGS_EQUAL(1, 0xff);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailurePrintHexOutputForLongInts)
{
    runTestWithMethod(_failingTestMethodWithLONGS_EQUAL);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <  1 0x01>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <255 0xff>");
}

TEST(UnitTestMacros, LONGS_EQUALBehavesAsProperMacro)
{
    if (false) LONGS_EQUAL(1, 2)
    else LONGS_EQUAL(10, 10)
}

IGNORE_TEST(UnitTestMacros, LONGS_EQUALWorksInAnIgnoredTest)
{
    LONGS_EQUAL(11, 22) // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

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
    if (false) BYTES_EQUAL('a', 'b')
    else BYTES_EQUAL('c', 'c')
}

IGNORE_TEST(UnitTestMacros, BYTES_EQUALWorksInAnIgnoredTest)
{
    BYTES_EQUAL('q', 'w') // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithPOINTERS_EQUAL()
{
    POINTERS_EQUAL((void*)0xa5a5, (void*)0xf0f0);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailurePrintHexOutputForPointers)
{
    runTestWithMethod(_failingTestMethodWithPOINTERS_EQUAL);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
}

TEST(UnitTestMacros, POINTERS_EQUALBehavesAsProperMacro)
{
    if (false) POINTERS_EQUAL(0, (void*) 0xbeefbeef)
    else POINTERS_EQUAL((void*)0xdeadbeef, (void*)0xdeadbeef)
}

IGNORE_TEST(UnitTestMacros, POINTERS_EQUALWorksInAnIgnoredTest)
{
    POINTERS_EQUAL((void*) 0xbeef, (void*) 0xdead) // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithDOUBLES_EQUAL()
{
    DOUBLES_EQUAL(0.12, 44.1, 0.3);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithDOUBLES_EQUAL)
{
    runTestWithMethod(_failingTestMethodWithDOUBLES_EQUAL);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("0.12");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("44.1");
}

TEST(UnitTestMacros, DOUBLES_EQUALBehavesAsProperMacro)
{
    if (false) DOUBLES_EQUAL(0.0, 1.1, 0.0005)
    else DOUBLES_EQUAL(0.1, 0.2, 0.2)
}

IGNORE_TEST(UnitTestMacros, DOUBLES_EQUALWorksInAnIgnoredTest)
{
    DOUBLES_EQUAL(100.0, 0.0, 0.2) // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

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
    MEMCMP_EQUAL("THIS", "THIS", 5);
    return 0;
}

TEST(UnitTestMacros, allMacrosFromFunctionThatReturnsAValue)
{
    functionThatReturnsAValue();
}

TEST(UnitTestMacros, NFirstCharsComparison)
{
    STRNCMP_EQUAL("Hello World!", "Hello Peter!", 0);
    STRNCMP_EQUAL("Hello World!", "Hello Peter!", 1);
    STRNCMP_EQUAL("Hello World!", "Hello Peter!", 6);
    STRNCMP_EQUAL("Hello World!", "Hello", 5);
}

static void _compareNFirstCharsWithUpperAndLowercase()
{
    STRNCMP_EQUAL("hello world!", "HELLO WORLD!", 12);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, CompareNFirstCharsWithUpperAndLowercase)
{
    runTestWithMethod(_compareNFirstCharsWithUpperAndLowercase);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello world!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <HELLO WORLD!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

static void _compareNFirstCharsWithDifferenceInTheMiddle()
{
    STRNCMP_EQUAL("Hello World!", "Hello Peter!", 12);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, CompareNFirstCharsWithDifferenceInTheMiddle)
{
    runTestWithMethod(_compareNFirstCharsWithDifferenceInTheMiddle);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Hello World!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Hello Peter!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 6");
}

static void _compareNFirstCharsWithEmptyString()
{
    STRNCMP_EQUAL("", "Not empty string", 5);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, CompareNFirstCharsWithEmptyString)
{
    runTestWithMethod(_compareNFirstCharsWithEmptyString);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

static void _compareNFirstCharsWithLastCharDifferent()
{
    STRNCMP_EQUAL("Not empty string?", "Not empty string!", 17);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, CompareNFirstCharsWithLastCharDifferent)
{
    runTestWithMethod(_compareNFirstCharsWithLastCharDifferent);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Not empty string?>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 16");
}

TEST(UnitTestMacros, MEMCMP_EQUALBehavesAsAProperMacro)
{
    if (false) MEMCMP_EQUAL("TEST", "test", 5)
    else MEMCMP_EQUAL("TEST", "TEST", 5)
}

IGNORE_TEST(UnitTestMacros, MEMCMP_EQUALWorksInAnIgnoredTest)
{
    MEMCMP_EQUAL("TEST", "test", 5); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _MEMCMP_EQUALFailingTestMethodWithUnequalInput()
{
    unsigned char expectedData[] = { 0x00, 0x01, 0x02, 0x03 };
    unsigned char actualData[] = { 0x00, 0x01, 0x03, 0x03 };

    MEMCMP_EQUAL(expectedData, actualData, sizeof(expectedData));
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, MEMCMP_EQUALFailureWithUnequalInput)
{
    runTestWithMethod(_MEMCMP_EQUALFailingTestMethodWithUnequalInput);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 03 03>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 2");
}

static void _MEMCMP_EQUALFailingTestMethodWithNullExpected()
{
    unsigned char actualData[] = { 0x00, 0x01, 0x02, 0x03 };

    MEMCMP_EQUAL(NULL, actualData, sizeof(actualData));
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, MEMCMP_EQUALFailureWithNullExpected)
{
    runTestWithMethod(_MEMCMP_EQUALFailingTestMethodWithNullExpected);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 02 03>");
}

static void _MEMCMP_EQUALFailingTestMethodWithNullActual()
{
    unsigned char expectedData[] = { 0x00, 0x01, 0x02, 0x03 };

    MEMCMP_EQUAL(expectedData, NULL, sizeof(expectedData));
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, MEMCMP_EQUALFailureWithNullActual)
{
    runTestWithMethod(_MEMCMP_EQUALFailingTestMethodWithNullActual);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(UnitTestMacros, MEMCMP_EQUALNullExpectedNullActual)
{
    MEMCMP_EQUAL(NULL, NULL, 0);
    MEMCMP_EQUAL(NULL, NULL, 1024);
}

#if CPPUTEST_USE_STD_CPP_LIB
static void _failingTestMethod_NoThrowWithCHECK_THROWS()
{
    CHECK_THROWS(int, (void) (1+2));
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_THROWS_whenDoesntThrow)
{
    runTestWithMethod(_failingTestMethod_NoThrowWithCHECK_THROWS);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw nothing");
    LONGS_EQUAL(1, fixture.getCheckCount());
}

static void _succeedingTestMethod_CorrectThrowWithCHECK_THROWS()
{
    CHECK_THROWS(int, throw 4);
    lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, SuccessWithCHECK_THROWS)
{
    runTestWithMethod(_succeedingTestMethod_CorrectThrowWithCHECK_THROWS);
    LONGS_EQUAL(1, fixture.getCheckCount());
}

static void _failingTestMethod_WrongThrowWithCHECK_THROWS()
{
    CHECK_THROWS(int, throw 4.3);
    lineOfCodeExecutedAfterCheck = true; // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_THROWS_whenWrongThrow)
{
    runTestWithMethod(_failingTestMethod_WrongThrowWithCHECK_THROWS);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw a different type");
    LONGS_EQUAL(1, fixture.getCheckCount());
}

TEST(UnitTestMacros, MultipleCHECK_THROWS_inOneScope)
{
    CHECK_THROWS(int, throw 4);
    CHECK_THROWS(int, throw 4);
}
#endif

TEST_GROUP(IgnoreTest)
{
    TestTestingFixture fixture;
    IgnoredUtestShell ignoreTest;

    void setup() _override
    {
        fixture.addTest(&ignoreTest);
    }
};

TEST(IgnoreTest, doesIgnoreCount)
{
    fixture.runAllTests();
    LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoreTest, printsIGNORE_TESTwhenVerbose)
{
    fixture.output_->verbose();
    fixture.runAllTests();
    fixture.assertPrintContains("IGNORE_TEST");
}


