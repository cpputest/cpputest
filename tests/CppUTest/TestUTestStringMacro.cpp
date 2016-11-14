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

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text) fixture.checkTestFailsWithProperTestLocation(text, __FILE__, __LINE__)

TEST_GROUP(UnitTestStringMacros)
{
    TestTestingFixture fixture;
};

static void _STRCMP_EQUALWithActualIsNULLTestMethod()
{
    STRCMP_EQUAL("ok", NULL);
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_EQUALAndActualIsNULL)
{
    fixture.runTestWithMethod(_STRCMP_EQUALWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

static void _STRCMP_EQUALWithExpectedIsNULLTestMethod()
{
    STRCMP_EQUAL(NULL, "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_EQUALAndExpectedIsNULL)
{
    fixture.runTestWithMethod(_STRCMP_EQUALWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

static void _STRCMP_CONTAINSWithActualIsNULLTestMethod()
{
    STRCMP_CONTAINS("ok", NULL);
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_CONTAINSAndActualIsNULL)
{
    fixture.runTestWithMethod(_STRCMP_CONTAINSWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

static void _STRCMP_CONTAINSWithExpectedIsNULLTestMethod()
{
    STRCMP_CONTAINS(NULL, "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_CONTAINSAndExpectedIsNULL)
{
    fixture.runTestWithMethod(_STRCMP_CONTAINSWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <>");
}

static void _STRNCMP_EQUALWithActualIsNULLTestMethod()
{
    STRNCMP_EQUAL("ok", NULL, 2);
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRNCMP_EQUALAndActualIsNULL)
{
    fixture.runTestWithMethod(_STRNCMP_EQUALWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

static void _STRNCMP_EQUALWithExpectedIsNULLTestMethod()
{
    STRNCMP_EQUAL(NULL, "ok", 2);
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRNCMP_EQUALAndExpectedIsNULL)
{
    fixture.runTestWithMethod(_STRNCMP_EQUALWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

static void _STRCMP_NOCASE_EQUALWithActualIsNULLTestMethod()
{
    STRCMP_NOCASE_EQUAL("ok", NULL);
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUALAndActualIsNULL)
{
    fixture.runTestWithMethod(_STRCMP_NOCASE_EQUALWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

static void _STRCMP_NOCASE_EQUALWithExpectedIsNULLTestMethod()
{
    STRCMP_NOCASE_EQUAL(NULL, "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUALAndExpectedIsNULL)
{
    fixture.runTestWithMethod(_STRCMP_NOCASE_EQUALWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

static void _STRCMP_NOCASE_EQUALWithUnequalInputTestMethod()
{
    STRCMP_NOCASE_EQUAL("no", "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUALAndUnequalInput)
{
    fixture.runTestWithMethod(_STRCMP_NOCASE_EQUALWithUnequalInputTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <ok>");
}

static void _STRCMP_NOCASE_CONTAINSWithActualIsNULLTestMethod()
{
    STRCMP_NOCASE_CONTAINS("ok", NULL);
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_CONTAINSAndActualIsNULL)
{
    fixture.runTestWithMethod(_STRCMP_NOCASE_CONTAINSWithActualIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

static void _STRCMP_NOCASE_CONTAINSWithExpectedIsNULLTestMethod()
{
    STRCMP_NOCASE_CONTAINS(NULL, "ok");
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_CONTAINSAndExpectedIsNULL)
{
    fixture.runTestWithMethod(_STRCMP_NOCASE_CONTAINSWithExpectedIsNULLTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <>");
}

static void _failingTestMethodWithSTRCMP_EQUAL()
{
    STRCMP_EQUAL("hello", "hell");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_EQUAL)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRCMP_EQUAL);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
}

TEST(UnitTestStringMacros, STRCMP_EQUALBehavesAsProperMacro)
{
    if (false) STRCMP_EQUAL("1", "2")
    else STRCMP_EQUAL("1", "1")
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_EQUALWorksInAnIgnoredTest)
{
    STRCMP_EQUAL("Hello", "World") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRCMP_EQUAL_TEXT()
{
    STRCMP_EQUAL_TEXT("hello", "hell", "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_EQUAL_TEXT)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRCMP_EQUAL_TEXT);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRCMP_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) STRCMP_EQUAL_TEXT("1", "2", "Failed because it failed")
    else STRCMP_EQUAL_TEXT("1", "1", "Failed because it failed")
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
    STRCMP_EQUAL_TEXT("Hello", "World", "Failed because it failed") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRNCMP_EQUAL()
{
    STRNCMP_EQUAL("hello", "hallo", 5);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRNCMP_EQUAL)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRNCMP_EQUAL);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
}

TEST(UnitTestStringMacros, STRNCMP_EQUALBehavesAsProperMacro)
{
    if (false) STRNCMP_EQUAL("1", "2", 1)
    else STRNCMP_EQUAL("1", "1", 1)
}

IGNORE_TEST(UnitTestStringMacros, STRNCMP_EQUALWorksInAnIgnoredTest)
{
    STRNCMP_EQUAL("Hello", "World", 3) // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRNCMP_EQUAL_TEXT()
{
    STRNCMP_EQUAL_TEXT("hello", "hallo", 5, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRNCMP_EQUAL_TEXT)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRNCMP_EQUAL_TEXT);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRNCMP_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) STRNCMP_EQUAL_TEXT("1", "2", 1, "Failed because it failed")
    else STRNCMP_EQUAL_TEXT("1", "1", 1, "Failed because it failed")
}

IGNORE_TEST(UnitTestStringMacros, STRNCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
    STRNCMP_EQUAL_TEXT("Hello", "World", 3, "Failed because it failed") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRCMP_NOCASE_EQUAL()
{
    STRCMP_NOCASE_EQUAL("hello", "Hell");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUAL)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRCMP_NOCASE_EQUAL);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Hell>");
}

TEST(UnitTestStringMacros, STRCMP_NOCASE_EQUALBehavesAsProperMacro)
{
    if (false) STRCMP_NOCASE_EQUAL("1", "2")
    else STRCMP_NOCASE_EQUAL("1", "1")
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_NOCASE_EQUALWorksInAnIgnoredTest)
{
    STRCMP_NOCASE_EQUAL("Hello", "World") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRCMP_NOCASE_EQUAL_TEXT()
{
    STRCMP_NOCASE_EQUAL_TEXT("hello", "hell", "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUAL_TEXT)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRCMP_NOCASE_EQUAL_TEXT);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRCMP_NOCASE_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) STRCMP_NOCASE_EQUAL_TEXT("1", "2", "Failed because it failed")
    else STRCMP_NOCASE_EQUAL_TEXT("1", "1", "Failed because it failed")
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_NOCASE_EQUAL_TEXTWorksInAnIgnoredTest)
{
    STRCMP_NOCASE_EQUAL_TEXT("Hello", "World", "Failed because it failed") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRCMP_CONTAINS()
{
    STRCMP_CONTAINS("hello", "world");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_CONTAINS)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRCMP_CONTAINS);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
}

TEST(UnitTestStringMacros, STRCMP_CONTAINSBehavesAsProperMacro)
{
    if (false) STRCMP_CONTAINS("1", "2")
    else STRCMP_CONTAINS("1", "1")
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_CONTAINSWorksInAnIgnoredTest)
{
    STRCMP_CONTAINS("Hello", "World") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRCMP_CONTAINS_TEXT()
{
    STRCMP_CONTAINS_TEXT("hello", "world", "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, FailureWithSTRCMP_CONTAINS_TEXT)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRCMP_CONTAINS_TEXT);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRCMP_CONTAINS_TEXTBehavesAsProperMacro)
{
    if (false) STRCMP_CONTAINS_TEXT("1", "2", "Failed because it failed")
    else STRCMP_CONTAINS_TEXT("1", "1", "Failed because it failed")
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_CONTAINS_TEXTWorksInAnIgnoredTest)
{
    STRCMP_CONTAINS_TEXT("Hello", "World", "Failed because it failed") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRCMP_NOCASE_CONTAINS()
{
    STRCMP_NOCASE_CONTAINS("hello", "WORLD");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_CONTAINS)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRCMP_NOCASE_CONTAINS);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <WORLD>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
}

TEST(UnitTestStringMacros, STRCMP_NOCASE_CONTAINSBehavesAsProperMacro)
{
    if (false) STRCMP_NOCASE_CONTAINS("never", "executed")
    else STRCMP_NOCASE_CONTAINS("hello", "HELLO WORLD")
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_NO_CASE_CONTAINSWorksInAnIgnoredTest)
{
    STRCMP_NOCASE_CONTAINS("Hello", "World") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _failingTestMethodWithSTRCMP_NOCASE_CONTAINS_TEXT()
{
    STRCMP_NOCASE_CONTAINS_TEXT("hello", "WORLD", "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_CONTAINS_TEXT)
{
    fixture.runTestWithMethod(_failingTestMethodWithSTRCMP_NOCASE_CONTAINS_TEXT);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <WORLD>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRCMP_NOCASE_CONTAINS_TEXTBehavesAsProperMacro)
{
    if (false) STRCMP_NOCASE_CONTAINS_TEXT("never", "executed", "Failed because it failed")
    else STRCMP_NOCASE_CONTAINS_TEXT("hello", "HELLO WORLD", "Failed because it failed")
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_NO_CASE_CONTAINS_TEXTWorksInAnIgnoredTest)
{
    STRCMP_NOCASE_CONTAINS_TEXT("Hello", "World", "Failed because it failed") // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, NFirstCharsComparison)
{
    STRNCMP_EQUAL("Hello World!", "Hello Peter!", 0);
    STRNCMP_EQUAL("Hello World!", "Hello Peter!", 1);
    STRNCMP_EQUAL("Hello World!", "Hello Peter!", 6);
    STRNCMP_EQUAL("Hello World!", "Hello", 5);
}

static void _compareNFirstCharsWithUpperAndLowercase()
{
    STRNCMP_EQUAL("hello world!", "HELLO WORLD!", 12);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, CompareNFirstCharsWithUpperAndLowercase)
{
    fixture.runTestWithMethod(_compareNFirstCharsWithUpperAndLowercase);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello world!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <HELLO WORLD!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

static void _compareNFirstCharsWithDifferenceInTheMiddle()
{
    STRNCMP_EQUAL("Hello World!", "Hello Peter!", 12);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, CompareNFirstCharsWithDifferenceInTheMiddle)
{
    fixture.runTestWithMethod(_compareNFirstCharsWithDifferenceInTheMiddle);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Hello World!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Hello Peter!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 6");
}

static void _compareNFirstCharsWithEmptyString()
{
    STRNCMP_EQUAL("", "Not empty string", 5);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, CompareNFirstCharsWithEmptyString)
{
    fixture.runTestWithMethod(_compareNFirstCharsWithEmptyString);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

static void _compareNFirstCharsWithLastCharDifferent()
{
    STRNCMP_EQUAL("Not empty string?", "Not empty string!", 17);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestStringMacros, CompareNFirstCharsWithLastCharDifferent)
{
    fixture.runTestWithMethod(_compareNFirstCharsWithLastCharDifferent);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Not empty string?>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string!>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 16");
}

