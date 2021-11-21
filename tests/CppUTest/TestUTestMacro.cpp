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

// Mainly this is for Visual C++, but we'll define it for any system that has the same behavior
// of a 32-bit long on a 64-bit system
#if defined(CPPUTEST_64BIT) && defined(CPPUTEST_64BIT_32BIT_LONGS)
// Forcing the value to be unsigned long long means that there's no sign-extension to perform
#define to_void_pointer(x) ((void *)x##ULL)
#define to_func_pointer(x) ((void (*)())x##ULL)
#else
// Probably not needed, but let's guarantee that the value is an unsigned long
#define to_void_pointer(x) ((void *)x##UL)
#define to_func_pointer(x) ((void (*)())x##UL)
#endif

TEST_GROUP(UnitTestMacros)
{
    TestTestingFixture fixture;
};

static void failingTestMethodWithFAIL_()
{
    FAIL("This test fails");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FAILMakesTheTestFailPrintsTheRightResultAndStopsExecuting)
{
    fixture.runTestWithMethod(failingTestMethodWithFAIL_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("This test fails");
}

TEST(UnitTestMacros, FAILWillPrintTheFileThatItFailed)
{
    fixture.runTestWithMethod(failingTestMethodWithFAIL_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT(__FILE__);
}

TEST(UnitTestMacros, FAILBehavesAsAProperMacro)
{
    if (false) FAIL("");
    else CHECK(true);

    if (true) CHECK(true);
    else FAIL("");
}

IGNORE_TEST(UnitTestMacros, FAILworksInAnIgnoredTest)
{
    FAIL("die!"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void UNSIGNED_LONGS_EQUALTestMethod_()
{
    UNSIGNED_LONGS_EQUAL(1, 1);
    UNSIGNED_LONGS_EQUAL(1, 0);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestUNSIGNED_LONGS_EQUAL)
{
    fixture.runTestWithMethod(UNSIGNED_LONGS_EQUALTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
}

TEST(UnitTestMacros, UNSIGNED_LONGS_EQUALBehavesAsProperMacro)
{
    if (false) UNSIGNED_LONGS_EQUAL(1, 0);
    else UNSIGNED_LONGS_EQUAL(1, 1);
}

IGNORE_TEST(UnitTestMacros, UNSIGNED_LONGS_EQUALWorksInAnIgnoredTest)
{
    UNSIGNED_LONGS_EQUAL(1, 0); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void UNSIGNED_LONGS_EQUAL_TEXTTestMethod_()
{
    UNSIGNED_LONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestUNSIGNED_LONGS_EQUAL_TEXT)
{
    fixture.runTestWithMethod(UNSIGNED_LONGS_EQUAL_TEXTTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, UNSIGNED_LONGS_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) UNSIGNED_LONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
    else UNSIGNED_LONGS_EQUAL_TEXT(1, 1, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, UNSIGNED_LONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
    UNSIGNED_LONGS_EQUAL_TEXT(1, 0, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

#ifdef CPPUTEST_USE_LONG_LONG

static void LONGLONGS_EQUALTestMethod_()
{
    LONGLONGS_EQUAL(1, 1);
    LONGLONGS_EQUAL(1, 0);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestLONGLONGS_EQUAL)
{
    fixture.runTestWithMethod(LONGLONGS_EQUALTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
}

TEST(UnitTestMacros, LONGLONGS_EQUALBehavesAsProperMacro)
{
    if (false) LONGLONGS_EQUAL(1, 0);
    else LONGLONGS_EQUAL(1, 1);
}

IGNORE_TEST(UnitTestMacros, LONGLONGS_EQUALWorksInAnIgnoredTest)
{
    LONGLONGS_EQUAL(1, 0); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void LONGLONGS_EQUAL_TEXTTestMethod_()
{
    LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestLONGLONGS_EQUAL_TEXT)
{
    fixture.runTestWithMethod(LONGLONGS_EQUAL_TEXTTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, LONGLONGS_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
    else LONGLONGS_EQUAL_TEXT(1, 1, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, LONGLONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
    LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void UNSIGNED_LONGLONGS_EQUALTestMethod_()
{
    UNSIGNED_LONGLONGS_EQUAL(1, 1);
    UNSIGNED_LONGLONGS_EQUAL(1, 0);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestUNSIGNED_LONGLONGS_EQUAL)
{
    fixture.runTestWithMethod(UNSIGNED_LONGLONGS_EQUALTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
}

TEST(UnitTestMacros, UNSIGNED_LONGLONGS_EQUALBehavesAsProperMacro)
{
    if (false) UNSIGNED_LONGLONGS_EQUAL(1, 0);
    else UNSIGNED_LONGLONGS_EQUAL(1, 1);
}

IGNORE_TEST(UnitTestMacros, UNSIGNED_LONGLONGS_EQUALWorksInAnIgnoredTest)
{
    UNSIGNED_LONGLONGS_EQUAL(1, 0); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void UNSIGNED_LONGLONGS_EQUAL_TEXTTestMethod_()
{
    UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestUNSIGNED_LONGLONGS_EQUAL_TEXT)
{
    fixture.runTestWithMethod(UNSIGNED_LONGLONGS_EQUAL_TEXTTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, UNSIGNED_LONGLONGS_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
    else UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 1, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, UNSIGNED_LONGLONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
    UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

#else

static void LONGLONGS_EQUALFailsWithUnsupportedFeatureTestMethod_()
{
    LONGLONGS_EQUAL(1, 1);
} // LCOV_EXCL_LINE

static void UNSIGNED_LONGLONGS_EQUALFailsWithUnsupportedFeatureTestMethod()
{
    UNSIGNED_LONGLONGS_EQUAL(1, 1);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, LONGLONGS_EQUALFailsWithUnsupportedFeature)
{
    fixture.runTestWithMethod(LONGLONGS_EQUALFailsWithUnsupportedFeatureTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("\"CPPUTEST_USE_LONG_LONG\" is not supported");
}

TEST(UnitTestMacros, UNSIGNED_LONGLONGS_EQUALFailsWithUnsupportedFeature)
{
    fixture.runTestWithMethod(UNSIGNED_LONGLONGS_EQUALFailsWithUnsupportedFeatureTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("\"CPPUTEST_USE_LONG_LONG\" is not supported");
}

#endif /* CPPUTEST_USE_LONG_LONG */

static void failingTestMethodWithCHECK_()
{
    CHECK(false);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK(false) failed");
}

TEST(UnitTestMacros, CHECKBehavesAsProperMacro)
{
    if (false) CHECK(false);
    else CHECK(true);
}

IGNORE_TEST(UnitTestMacros, CHECKWorksInAnIgnoredTest)
{
    CHECK(false); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithCHECK_TEXT_()
{
    CHECK_TEXT(false, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK(false) failed");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, CHECK_TEXTBehavesAsProperMacro)
{
    if (false) CHECK_TEXT(false, "false");
    else CHECK_TEXT(true, "true");
}

IGNORE_TEST(UnitTestMacros, CHECK_TEXTWorksInAnIgnoredTest)
{
    CHECK_TEXT(false, "false"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithCHECK_TRUE_()
{
    CHECK_TRUE(false);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_TRUE)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_TRUE_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE(false) failed");
}

TEST(UnitTestMacros, CHECK_TRUEBehavesAsProperMacro)
{
    if (false) CHECK_TRUE(false);
    else CHECK_TRUE(true);
}

IGNORE_TEST(UnitTestMacros, CHECK_TRUEWorksInAnIgnoredTest)
{
    CHECK_TRUE(false); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithCHECK_TRUE_TEXT_()
{
    CHECK_TRUE_TEXT(false, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_TRUE_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_TRUE_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE(false) failed");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, CHECK_TRUE_TEXTBehavesAsProperMacro)
{
    if (false) CHECK_TRUE_TEXT(false, "Failed because it failed");
    else CHECK_TRUE_TEXT(true, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, CHECK_TRUE_TEXTWorksInAnIgnoredTest)
{
    CHECK_TRUE_TEXT(false, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithCHECK_FALSE_()
{
    CHECK_FALSE(true);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_FALSE)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_FALSE_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE(true) failed");
}

TEST(UnitTestMacros, CHECK_FALSEBehavesAsProperMacro)
{
    if (false) CHECK_FALSE(true);
    else CHECK_FALSE(false);
}

IGNORE_TEST(UnitTestMacros, CHECK_FALSEWorksInAnIgnoredTest)
{
    CHECK_FALSE(true); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithCHECK_FALSE_TEXT_()
{
    CHECK_FALSE_TEXT(true, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_FALSE_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_FALSE_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE(true)");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, CHECK_FALSE_TEXTBehavesAsProperMacro)
{
    if (false) CHECK_FALSE_TEXT(true, "Failed because it failed");
    else CHECK_FALSE_TEXT(false, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, CHECK_FALSE_TEXTWorksInAnIgnoredTest)
{
    CHECK_FALSE_TEXT(true, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithCHECK_EQUAL_()
{
    CHECK_EQUAL(1, 2);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_EQUAL)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_EQUAL_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <2>");
}

static void failingTestMethodWithCHECK_COMPARE_()
{
    double small = 0.5, big = 0.8;
    CHECK_COMPARE(small, >=, big);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_COMPARE)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_COMPARE_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_COMPARE(0.5 >= 0.8)");
}

TEST(UnitTestMacros, CHECK_COMPAREBehavesAsProperMacro)
{
    if (false) CHECK_COMPARE(1, >, 2);
    else CHECK_COMPARE(1, <, 2);
}

IGNORE_TEST(UnitTestMacros, CHECK_COMPAREWorksInAnIgnoredTest)
{
  CHECK_COMPARE(1, >, 2); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithCHECK_COMPARE_TEXT_()
{
    double small = 0.5, big = 0.8;
    CHECK_COMPARE_TEXT(small, >=, big, "small bigger than big");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_COMPARE_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_COMPARE_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_COMPARE(0.5 >= 0.8)");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("small bigger than big");
}

TEST(UnitTestMacros, CHECK_COMPARE_TEXTBehavesAsProperMacro)
{
  if (false) CHECK_COMPARE_TEXT(1, >, 2, "1 bigger than 2");
  else CHECK_COMPARE_TEXT(1, <, 2, "1 smaller than 2");
}

IGNORE_TEST(UnitTestMacros, CHECK_COMPARE_TEXTWorksInAnIgnoredTest)
{
  CHECK_COMPARE_TEXT(1, >, 2, "1 smaller than 2"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static int countInCountingMethod;
static int countingMethod_()
{
    return countInCountingMethod++;
}

TEST(UnitTestMacros, LONGS_EQUAL_macroExpressionSafety)
{
    LONGS_EQUAL(1, 0.4 + 0.7);
    LONGS_EQUAL(0.4 + 0.7, 1);
    LONGS_EQUAL_TEXT(1, 0.4 + 0.7, "-Wconversion=great");
    LONGS_EQUAL_TEXT(0.4 + 0.7, 1, "-Wconversion=great");
}

TEST(UnitTestMacros, UNSIGNED_LONGS_EQUAL_macroExpressionSafety)
{
    UNSIGNED_LONGS_EQUAL(1, 0.4 + 0.7);
    UNSIGNED_LONGS_EQUAL(0.4 + 0.7, 1);
    UNSIGNED_LONGS_EQUAL_TEXT(1, 0.4 + 0.7, "-Wconversion=great");
    UNSIGNED_LONGS_EQUAL_TEXT(0.4 + 0.7, 1, "-Wconversion=great");
}

TEST(UnitTestMacros, passingCheckEqualWillNotBeEvaluatedMultipleTimesWithCHECK_EQUAL)
{
    countInCountingMethod = 0;
    CHECK_EQUAL(0, countingMethod_());

    LONGS_EQUAL(1, countInCountingMethod);
}

static void failing_CHECK_EQUAL_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning_()
{
    CHECK_EQUAL(12345, countingMethod_());
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, failing_CHECK_EQUAL_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning)
{
    fixture.runTestWithMethod(failing_CHECK_EQUAL_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("WARNING:\n\tThe \"Actual Parameter\" parameter is evaluated multiple times resulting in different values.\n\tThus the value in the error message is probably incorrect.");
}

static void failing_CHECK_EQUAL_WithExpectedBeingEvaluatesMultipleTimesWillGiveAWarning_()
{
    CHECK_EQUAL(countingMethod_(), 12345);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, failing_CHECK_EQUAL_WithExpectedBeingEvaluatesMultipleTimesWillGiveAWarning)
{
    fixture.runTestWithMethod(failing_CHECK_EQUAL_WithExpectedBeingEvaluatesMultipleTimesWillGiveAWarning_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("WARNING:\n\tThe \"Expected Parameter\" parameter is evaluated multiple times resulting in different values.\n\tThus the value in the error message is probably incorrect.");
}

TEST(UnitTestMacros, failing_CHECK_EQUAL_withParamatersThatDontChangeWillNotGiveAnyWarning)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_EQUAL_);
    fixture.assertPrintContainsNot("WARNING");
}

TEST(UnitTestMacros, CHECK_EQUALBehavesAsProperMacro)
{
    if (false) CHECK_EQUAL(1, 2);
    else CHECK_EQUAL(1, 1);
}

IGNORE_TEST(UnitTestMacros, CHECK_EQUALWorksInAnIgnoredTest)
{
    CHECK_EQUAL(1, 2); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithCHECK_EQUAL_TEXT_()
{
    CHECK_EQUAL_TEXT(1, 2, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_EQUAL_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_EQUAL_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <2>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, CHECK_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) CHECK_EQUAL_TEXT(1, 2, "Failed because it failed");
    else CHECK_EQUAL_TEXT(1, 1, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, CHECK_EQUAL_TEXTWorksInAnIgnoredTest)
{
    CHECK_EQUAL_TEXT(1, 2, "Failed because it failed"); // LCOV_EXCL_LINE;
} // LCOV_EXCL_LINE

static void failingTestMethodWithCHECK_EQUAL_ZERO_()
{
    CHECK_EQUAL_ZERO(1);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_EQUAL_ZERO)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_EQUAL_ZERO_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <1>");
}

TEST(UnitTestMacros, passingCheckEqualWillNotBeEvaluatedMultipleTimesWithCHECK_EQUAL_ZERO)
{
    countInCountingMethod = 0;
    CHECK_EQUAL_ZERO(countingMethod_());

    LONGS_EQUAL(1, countInCountingMethod);
}

static void failing_CHECK_EQUAL_ZERO_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning_()
{
    countInCountingMethod = 1;
    CHECK_EQUAL_ZERO(countingMethod_());
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, failing_CHECK_EQUAL_ZERO_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning)
{
    fixture.runTestWithMethod(failing_CHECK_EQUAL_ZERO_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("WARNING:\n\tThe \"Actual Parameter\" parameter is evaluated multiple times resulting in different values.\n\tThus the value in the error message is probably incorrect.");
}

TEST(UnitTestMacros, failing_CHECK_EQUAL_ZERO_withParamatersThatDontChangeWillNotGiveAnyWarning)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_EQUAL_ZERO_);
    fixture.assertPrintContainsNot("WARNING");
}

IGNORE_TEST(UnitTestMacros, CHECK_EQUAL_ZERO_WorksInAnIgnoredTest)
{
    CHECK_EQUAL_ZERO(1); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, CHECK_EQUAL_ZERO_BehavesAsProperMacro)
{
    if (false) CHECK_EQUAL_ZERO(1);
    else CHECK_EQUAL_ZERO(0);
}

static void failingTestMethodWithCHECK_EQUAL_ZERO_TEXT_()
{
    CHECK_EQUAL_ZERO_TEXT(1, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_EQUAL_ZERO_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithCHECK_EQUAL_ZERO_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, CHECK_EQUAL_ZERO_TEXTBehavesAsProperMacro)
{
    if (false) CHECK_EQUAL_ZERO_TEXT(1, "Failed because it failed");
    else CHECK_EQUAL_ZERO_TEXT(0, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, CHECK_EQUAL_ZERO_TEXTWorksInAnIgnoredTest)
{
    CHECK_EQUAL_ZERO_TEXT(1, "Failed because it failed"); // LCOV_EXCL_LINE;
} // LCOV_EXCL_LINE

static void failingTestMethodWithLONGS_EQUAL_()
{
    LONGS_EQUAL(1, 0xff);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithLONGS_EQUALS)
{
    fixture.runTestWithMethod(failingTestMethodWithLONGS_EQUAL_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <  1 (0x1)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <255 (0xff)>");
}

static void failingTestMethodWithLONGS_EQUALWithSymbolicParameters_()
{
#define _MONDAY 1
    int day_of_the_week = _MONDAY+1;
    LONGS_EQUAL(_MONDAY, day_of_the_week);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithLONGS_EQUALShowsSymbolicParameters)
{
    fixture.runTestWithMethod(failingTestMethodWithLONGS_EQUALWithSymbolicParameters_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("LONGS_EQUAL(_MONDAY, day_of_the_week) failed");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <2 (0x2)>");
    CHECK_FALSE(fixture.getOutput().contains("Message: "));
}

TEST(UnitTestMacros, LONGS_EQUALBehavesAsProperMacro)
{
    if (false) LONGS_EQUAL(1, 2);
    else LONGS_EQUAL(10, 10);
}

IGNORE_TEST(UnitTestMacros, LONGS_EQUALWorksInAnIgnoredTest)
{
    LONGS_EQUAL(11, 22); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithLONGS_EQUAL_TEXT_()
{
    LONGS_EQUAL_TEXT(1, 0xff, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithLONGS_EQUALS_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithLONGS_EQUAL_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <  1 (0x1)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <255 (0xff)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, LONGS_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) LONGS_EQUAL_TEXT(1, 2, "Failed because it failed");
    else LONGS_EQUAL_TEXT(10, 10, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, LONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
    LONGS_EQUAL_TEXT(11, 22, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithBYTES_EQUAL_()
{
    BYTES_EQUAL('a', 'b');
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
}

TEST(UnitTestMacros, FailureWithBYTES_EQUAL)
{
    fixture.runTestWithMethod(failingTestMethodWithBYTES_EQUAL_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <97 (0x61)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <98 (0x62)>");
}

TEST(UnitTestMacros, BYTES_EQUALBehavesAsProperMacro)
{
    if (false) BYTES_EQUAL('a', 'b');
    else BYTES_EQUAL('c', 'c');
}

IGNORE_TEST(UnitTestMacros, BYTES_EQUALWorksInAnIgnoredTest)
{
    BYTES_EQUAL('q', 'w'); // LCOV_EXCL_LINE;
} // LCOV_EXCL_LINE

static void failingTestMethodWithBYTES_EQUAL_TEXT_()
{
    BYTES_EQUAL_TEXT('a', 'b', "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
}

TEST(UnitTestMacros, FailureWithBYTES_EQUAL_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithBYTES_EQUAL_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <97 (0x61)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <98 (0x62)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, BYTES_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) BYTES_EQUAL_TEXT('a', 'b', "Failed because it failed");
    else BYTES_EQUAL_TEXT('c', 'c', "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, BYTES_EQUAL_TEXTWorksInAnIgnoredTest)
{
    BYTES_EQUAL_TEXT('q', 'w', "Failed because it failed"); // LCOV_EXCL_LINE;
} // LCOV_EXCL_LINE

static void failingTestMethodWithSIGNED_BYTES_EQUAL_()
{
    SIGNED_BYTES_EQUAL(-1, -2);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
}

TEST(UnitTestMacros, FailureWithSIGNED_BYTES_EQUAL)
{
    fixture.runTestWithMethod(failingTestMethodWithSIGNED_BYTES_EQUAL_);
#if CPPUTEST_CHAR_BIT == 16
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-1 (0xffff)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-2 (0xfffe)>");
#else
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-1 (0xff)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-2 (0xfe)>");
#endif
}

TEST(UnitTestMacros, CHARS_EQUALBehavesAsProperMacro)
{
    if (false) SIGNED_BYTES_EQUAL(-1, -2);
    else SIGNED_BYTES_EQUAL(-3, -3);
}

IGNORE_TEST(UnitTestMacros, CHARS_EQUALWorksInAnIgnoredTest)
{
    SIGNED_BYTES_EQUAL(-7, 19); // LCOV_EXCL_LINE;
} // LCOV_EXCL_LINE

static void failingTestMethodWithSIGNED_BYTES_EQUAL_TEXT_()
{
    SIGNED_BYTES_EQUAL_TEXT(-127, -126, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
}

TEST(UnitTestMacros, FailureWithSIGNED_BYTES_EQUAL_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithSIGNED_BYTES_EQUAL_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-127 (0x81)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-126 (0x82)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, CHARS_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) SIGNED_BYTES_EQUAL_TEXT(-1, -2, "Failed because it failed");
    else SIGNED_BYTES_EQUAL_TEXT(-3, -3, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, SIGNED_BYTES_EQUAL_TEXTWorksInAnIgnoredTest)
{
    SIGNED_BYTES_EQUAL_TEXT(-7, 19, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithPOINTERS_EQUAL_()
{
    POINTERS_EQUAL((void*)0xa5a5, (void*)0xf0f0);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithPOINTERS_EQUAL)
{
    fixture.runTestWithMethod(failingTestMethodWithPOINTERS_EQUAL_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
}

TEST(UnitTestMacros, POINTERS_EQUALBehavesAsProperMacro)
{
    if (false) POINTERS_EQUAL(NULLPTR, to_void_pointer(0xbeefbeef));
    else POINTERS_EQUAL(to_void_pointer(0xdeadbeef), to_void_pointer(0xdeadbeef));
}

IGNORE_TEST(UnitTestMacros, POINTERS_EQUALWorksInAnIgnoredTest)
{
    POINTERS_EQUAL((void*) 0xbeef, (void*) 0xdead); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithPOINTERS_EQUAL_TEXT_()
{
    POINTERS_EQUAL_TEXT((void*)0xa5a5, (void*)0xf0f0, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithPOINTERS_EQUAL_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithPOINTERS_EQUAL_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, POINTERS_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) POINTERS_EQUAL_TEXT(NULLPTR, to_void_pointer(0xbeefbeef), "Failed because it failed");
    else POINTERS_EQUAL_TEXT(to_void_pointer(0xdeadbeef), to_void_pointer(0xdeadbeef), "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, POINTERS_EQUAL_TEXTWorksInAnIgnoredTest)
{
    POINTERS_EQUAL_TEXT((void*) 0xbeef, (void*) 0xdead, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE


static void failingTestMethodWithFUNCTIONPOINTERS_EQUAL_()
{
    FUNCTIONPOINTERS_EQUAL((void (*)())0xa5a5, (void (*)())0xf0f0);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithFUNCTIONPOINTERS_EQUAL)
{
    fixture.runTestWithMethod(failingTestMethodWithFUNCTIONPOINTERS_EQUAL_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
}

TEST(UnitTestMacros, FUNCTIONPOINTERS_EQUALBehavesAsProperMacro)
{
    if (false) FUNCTIONPOINTERS_EQUAL(NULLPTR, to_func_pointer(0xbeefbeef));
    else FUNCTIONPOINTERS_EQUAL(to_func_pointer(0xdeadbeef), to_func_pointer(0xdeadbeef));
}

IGNORE_TEST(UnitTestMacros, FUNCTIONPOINTERS_EQUALWorksInAnIgnoredTest)
{
    FUNCTIONPOINTERS_EQUAL((void (*)())0xbeef, (void (*)())0xdead); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void failingTestMethodWithFUNCTIONPOINTERS_EQUAL_TEXT_()
{
    FUNCTIONPOINTERS_EQUAL_TEXT((void (*)())0xa5a5, (void (*)())0xf0f0, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithFUNCTIONPOINTERS_EQUAL_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithFUNCTIONPOINTERS_EQUAL_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, FUNCTIONPOINTERS_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) FUNCTIONPOINTERS_EQUAL_TEXT(NULLPTR, to_func_pointer(0xbeefbeef), "Failed because it failed");
    else FUNCTIONPOINTERS_EQUAL_TEXT(to_func_pointer(0xdeadbeef), to_func_pointer(0xdeadbeef), "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, FUNCTIONPOINTERS_EQUAL_TEXTWorksInAnIgnoredTest)
{
    FUNCTIONPOINTERS_EQUAL_TEXT((void (*)())0xbeef, (void (*)())0xdead, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE




static void failingTestMethodWithDOUBLES_EQUAL_()
{
    DOUBLES_EQUAL(0.12, 44.1, 0.3);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithDOUBLES_EQUAL)
{
    fixture.runTestWithMethod(failingTestMethodWithDOUBLES_EQUAL_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0.12>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <44.1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("threshold used was <0.3>");
}

TEST(UnitTestMacros, DOUBLES_EQUALBehavesAsProperMacro)
{
    if (false) DOUBLES_EQUAL(0.0, 1.1, 0.0005);
    else DOUBLES_EQUAL(0.1, 0.2, 0.2);
}

IGNORE_TEST(UnitTestMacros, DOUBLES_EQUALWorksInAnIgnoredTest)
{
    DOUBLES_EQUAL(100.0, 0.0, 0.2); // LCOV_EXCL_LINE;
} // LCOV_EXCL_LINE

static void failingTestMethodWithDOUBLES_EQUAL_TEXT_()
{
    DOUBLES_EQUAL_TEXT(0.12, 44.1, 0.3, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithDOUBLES_EQUAL_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithDOUBLES_EQUAL_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0.12>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <44.1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("threshold used was <0.3>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, DOUBLES_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) DOUBLES_EQUAL_TEXT(0.0, 1.1, 0.0005, "Failed because it failed");
    else DOUBLES_EQUAL_TEXT(0.1, 0.2, 0.2, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, DOUBLES_EQUAL_TEXTWorksInAnIgnoredTest)
{
    DOUBLES_EQUAL_TEXT(100.0, 0.0, 0.2, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static bool lineOfCodeExecutedAfterCheck = false;

static void passingTestMethod_()
{
    CHECK(true);
    lineOfCodeExecutedAfterCheck = true;
}

TEST(UnitTestMacros, SuccessPrintsNothing)
{
    fixture.runTestWithMethod(passingTestMethod_);

    LONGS_EQUAL(0, fixture.getFailureCount());
    fixture.assertPrintContains(".\nOK (1 tests");
    CHECK(lineOfCodeExecutedAfterCheck);
}

static void methodThatOnlyPrints_()
{
    UT_PRINT("Hello World!");
}

TEST(UnitTestMacros, PrintPrintsWhateverPrintPrints)
{
    fixture.runTestWithMethod(methodThatOnlyPrints_);

    LONGS_EQUAL(0, fixture.getFailureCount());
    fixture.assertPrintContains("Hello World!");
    fixture.assertPrintContains(__FILE__);
}

static void methodThatOnlyPrintsUsingSimpleStringFromFormat_()
{
    UT_PRINT(StringFromFormat("Hello %s %d", "World!", 2009));
}

TEST(UnitTestMacros, PrintPrintsSimpleStringsForExampleThoseReturnedByFromString)
{
    fixture.runTestWithMethod(methodThatOnlyPrintsUsingSimpleStringFromFormat_);
    fixture.assertPrintContains("Hello World! 2009");
}

static int functionThatReturnsAValue()
{
    CHECK(0 == 0);
    CHECK_TEXT(0 == 0, "Shouldn't fail");
    CHECK_TRUE(0 == 0);
    CHECK_TRUE_TEXT(0 == 0, "Shouldn't fail");
    CHECK_FALSE(0 != 0);
    CHECK_FALSE_TEXT(0 != 0, "Shouldn't fail");
    LONGS_EQUAL(1,1);
    LONGS_EQUAL_TEXT(1, 1, "Shouldn't fail");
    BYTES_EQUAL(0xab,0xab);
    BYTES_EQUAL_TEXT(0xab, 0xab, "Shouldn't fail");
    CHECK_EQUAL(100,100);
    CHECK_EQUAL_TEXT(100, 100, "Shouldn't fail");
    CHECK_EQUAL_ZERO(0);
    CHECK_EQUAL_ZERO_TEXT(0, "Shouldn't fail");
    STRCMP_EQUAL("THIS", "THIS");
    STRCMP_EQUAL_TEXT("THIS", "THIS", "Shouldn't fail");
    DOUBLES_EQUAL(1.0, 1.0, .01);
    DOUBLES_EQUAL_TEXT(1.0, 1.0, .01, "Shouldn't fail");
    POINTERS_EQUAL(NULLPTR, NULLPTR);
    POINTERS_EQUAL_TEXT(NULLPTR, NULLPTR, "Shouldn't fail");
    MEMCMP_EQUAL("THIS", "THIS", 5);
    MEMCMP_EQUAL_TEXT("THIS", "THIS", 5, "Shouldn't fail");
    BITS_EQUAL(0x01, (unsigned char )0x01, 0xFF);
    BITS_EQUAL(0x0001, (unsigned short )0x0001, 0xFFFF);
    BITS_EQUAL(0x00000001, (unsigned long )0x00000001, 0xFFFFFFFF);
    BITS_EQUAL_TEXT(0x01, (unsigned char )0x01, 0xFF, "Shouldn't fail");
    return 0;
}

TEST(UnitTestMacros, allMacrosFromFunctionThatReturnsAValue)
{
    functionThatReturnsAValue();
}

TEST(UnitTestMacros, MEMCMP_EQUALBehavesAsAProperMacro)
{
    if (false) MEMCMP_EQUAL("TEST", "test", 5);
    else MEMCMP_EQUAL("TEST", "TEST", 5);
}

IGNORE_TEST(UnitTestMacros, MEMCMP_EQUALWorksInAnIgnoredTest)
{
    MEMCMP_EQUAL("TEST", "test", 5); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void MEMCMP_EQUALFailingTestMethodWithUnequalInput_()
{
    unsigned char expectedData[] = { 0x00, 0x01, 0x02, 0x03 };
    unsigned char actualData[] = { 0x00, 0x01, 0x03, 0x03 };

    MEMCMP_EQUAL(expectedData, actualData, sizeof(expectedData));
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, MEMCMP_EQUALFailureWithUnequalInput)
{
    fixture.runTestWithMethod(MEMCMP_EQUALFailingTestMethodWithUnequalInput_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 03 03>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 2");
}

static void MEMCMP_EQUALFailingTestMethodWithNullExpected_()
{
    unsigned char actualData[] = { 0x00, 0x01, 0x02, 0x03 };

    MEMCMP_EQUAL(NULLPTR, actualData, sizeof(actualData));
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, MEMCMP_EQUALFailureWithNullExpected)
{
    fixture.runTestWithMethod(MEMCMP_EQUALFailingTestMethodWithNullExpected_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 02 03>");
}

static void MEMCMP_EQUALFailingTestMethodWithNullActual_()
{
    unsigned char expectedData[] = { 0x00, 0x01, 0x02, 0x03 };

    MEMCMP_EQUAL(expectedData, NULLPTR, sizeof(expectedData));
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, MEMCMP_EQUALFailureWithNullActual)
{
    fixture.runTestWithMethod(MEMCMP_EQUALFailingTestMethodWithNullActual_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(UnitTestMacros, MEMCMP_EQUALNullExpectedNullActual)
{
    MEMCMP_EQUAL(NULLPTR, NULLPTR, 0);
    MEMCMP_EQUAL(NULLPTR, NULLPTR, 1024);
}

TEST(UnitTestMacros, MEMCMP_EQUALNullPointerIgnoredInExpectationWhenSize0)
{
	unsigned char actualData[] = { 0x00, 0x01, 0x03, 0x03 };
	MEMCMP_EQUAL(NULLPTR, actualData, 0);
}

TEST(UnitTestMacros, MEMCMP_EQUALNullPointerIgnoredInActualWhenSize0)
{
	unsigned char expectedData[] = { 0x00, 0x01, 0x02, 0x03 };
	MEMCMP_EQUAL(expectedData, NULLPTR, 0);
}

static void failingTestMethodWithMEMCMP_EQUAL_TEXT_()
{
    unsigned char expectedData[] = { 0x00, 0x01, 0x02, 0x03 };
    unsigned char actualData[] = { 0x00, 0x01, 0x03, 0x03 };

    MEMCMP_EQUAL_TEXT(expectedData, actualData, sizeof(expectedData), "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithMEMCMP_EQUAL_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithMEMCMP_EQUAL_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 03 03>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 2");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, MEMCMP_EQUAL_TEXTBehavesAsAProperMacro)
{
    if (false) MEMCMP_EQUAL_TEXT("TEST", "test", 5, "Failed because it failed");
    else MEMCMP_EQUAL_TEXT("TEST", "TEST", 5, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, MEMCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
    MEMCMP_EQUAL_TEXT("TEST", "test", 5, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, BITS_EQUALBehavesAsAProperMacro)
{
    if (false) BITS_EQUAL(0x00, 0xFF, 0xFF);
    else BITS_EQUAL(0x00, 0x00, 0xFF);
}

IGNORE_TEST(UnitTestMacros, BITS_EQUALWorksInAnIgnoredTest)
{
    BITS_EQUAL(0x00, 0xFF, 0xFF); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void BITS_EQUALFailingTestMethodWithUnequalInput_()
{
    BITS_EQUAL(0x00, 0xFF, 0xFF);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, BITS_EQUALFailureWithUnequalInput)
{
    fixture.runTestWithMethod(BITS_EQUALFailingTestMethodWithUnequalInput_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("00000000>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("11111111>");
}

TEST(UnitTestMacros, BITS_EQUALZeroMaskEqual)
{
    BITS_EQUAL(0x00, 0xFF, 0x00);
}

static void failingTestMethodWithBITS_EQUAL_TEXT_()
{
    BITS_EQUAL_TEXT(0x00, 0xFFFFFFFF, 0xFF, "Failed because it failed");
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithBITS_EQUAL_TEXT)
{
    fixture.runTestWithMethod(failingTestMethodWithBITS_EQUAL_TEXT_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <xxxxxxxx xxxxxxxx xxxxxxxx 00000000>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <xxxxxxxx xxxxxxxx xxxxxxxx 11111111>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, BITS_EQUAL_TEXTBehavesAsAProperMacro)
{
    if (false) BITS_EQUAL_TEXT(0x00, 0xFF, 0xFF, "Failed because it failed");
    else BITS_EQUAL_TEXT(0x00, 0x00, 0xFF, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, BITS_EQUAL_TEXTWorksInAnIgnoredTest)
{
    BITS_EQUAL_TEXT(0x00, 0xFF, 0xFF, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

#if defined(__cplusplus) && __cplusplus >= 201103L
enum class ScopedIntEnum {
    A, B
};

static void ENUMS_EQUAL_INTWithScopedIntEnumTestMethod_()
{
    ENUMS_EQUAL_INT(ScopedIntEnum::B, ScopedIntEnum::B);
    ENUMS_EQUAL_INT(ScopedIntEnum::B, ScopedIntEnum::A);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestENUMS_EQUAL_INTWithScopedIntEnum)
{
    fixture.runTestWithMethod(ENUMS_EQUAL_INTWithScopedIntEnumTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
}

TEST(UnitTestMacros, ENUMS_EQUAL_INTWithScopedIntEnumBehavesAsProperMacro)
{
    if (false) ENUMS_EQUAL_INT(ScopedIntEnum::B, ScopedIntEnum::A);
    else ENUMS_EQUAL_INT(ScopedIntEnum::B, ScopedIntEnum::B);
}

IGNORE_TEST(UnitTestMacros, ENUMS_EQUAL_INTWithScopedIntEnumWorksInAnIgnoredTest)
{
    ENUMS_EQUAL_INT(ScopedIntEnum::B, ScopedIntEnum::A); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void ENUMS_EQUAL_INT_TEXTWithScopedIntEnumTestMethod_()
{
    ENUMS_EQUAL_INT_TEXT(ScopedIntEnum::B, ScopedIntEnum::A, "Failed because it failed");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestENUMS_EQUAL_INT_TEXTWithScopedIntEnum)
{
    fixture.runTestWithMethod(ENUMS_EQUAL_INT_TEXTWithScopedIntEnumTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, ENUMS_EQUAL_INT_TEXTWithScopedIntEnumBehavesAsProperMacro)
{
    if (false) ENUMS_EQUAL_INT_TEXT(ScopedIntEnum::B, ScopedIntEnum::A, "Failed because it failed");
    else ENUMS_EQUAL_INT_TEXT(ScopedIntEnum::B, ScopedIntEnum::B, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, ENUMS_EQUAL_EQUAL_INT_TEXTWithScopedIntEnumWorksInAnIgnoredTest)
{
    ENUMS_EQUAL_INT_TEXT(ScopedIntEnum::B, ScopedIntEnum::A, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

enum class ScopedLongEnum : long {
    A, B
};

static void ENUMS_EQUAL_TYPEWithScopedLongEnumTestMethod_()
{
    ENUMS_EQUAL_TYPE(long, ScopedLongEnum::B, ScopedLongEnum::B);
    ENUMS_EQUAL_TYPE(long, ScopedLongEnum::B, ScopedLongEnum::A);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestENUMS_EQUAL_TYPEWithScopedLongEnum)
{
    fixture.runTestWithMethod(ENUMS_EQUAL_TYPEWithScopedLongEnumTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
}

TEST(UnitTestMacros, ENUMS_EQUAL_TYPEWithScopedLongEnumBehavesAsProperMacro)
{
    if (false) ENUMS_EQUAL_TYPE(long, ScopedLongEnum::B, ScopedLongEnum::A);
    else ENUMS_EQUAL_TYPE(long, ScopedLongEnum::B, ScopedLongEnum::B);
}

IGNORE_TEST(UnitTestMacros, ENUMS_EQUAL_TYPEWithScopedLongEnumWorksInAnIgnoredTest)
{
    ENUMS_EQUAL_TYPE(long, ScopedLongEnum::B, ScopedLongEnum::A); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void ENUMS_EQUAL_TYPE_TEXTWithScopedLongEnumTestMethod_()
{
    ENUMS_EQUAL_TYPE_TEXT(long, ScopedLongEnum::B, ScopedLongEnum::A, "Failed because it failed");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestENUMS_EQUAL_TYPE_TEXTWithScopedLongEnum)
{
    fixture.runTestWithMethod(ENUMS_EQUAL_TYPE_TEXTWithScopedLongEnumTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, ENUMS_EQUAL_TYPE_TEXTWithScopedLongEnumBehavesAsProperMacro)
{
    if (false) ENUMS_EQUAL_TYPE_TEXT(long, ScopedLongEnum::B, ScopedLongEnum::A, "Failed because it failed");
    else ENUMS_EQUAL_TYPE_TEXT(long, ScopedLongEnum::B, ScopedLongEnum::B, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, ENUMS_EQUAL_EQUAL_TYPE_TEXTWithScopedLongEnumWorksInAnIgnoredTest)
{
    ENUMS_EQUAL_TYPE_TEXT(long, ScopedLongEnum::B, ScopedLongEnum::A, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

#endif

enum UnscopedEnum {
    UNSCOPED_ENUM_A, UNSCOPED_ENUM_B
};

static void ENUMS_EQUAL_INTWithUnscopedEnumTestMethod_()
{
    ENUMS_EQUAL_INT(UNSCOPED_ENUM_B, UNSCOPED_ENUM_B);
    ENUMS_EQUAL_INT(UNSCOPED_ENUM_B, UNSCOPED_ENUM_A);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestENUMS_EQUAL_INTWithUnscopedEnum)
{
    fixture.runTestWithMethod(ENUMS_EQUAL_INTWithUnscopedEnumTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
}

TEST(UnitTestMacros, ENUMS_EQUAL_INTWithUnscopedEnumBehavesAsProperMacro)
{
    if (false) ENUMS_EQUAL_INT(UNSCOPED_ENUM_B, UNSCOPED_ENUM_A);
    else ENUMS_EQUAL_INT(UNSCOPED_ENUM_B, UNSCOPED_ENUM_B);
}

IGNORE_TEST(UnitTestMacros, ENUMS_EQUAL_INTWithUnscopedEnumWorksInAnIgnoredTest)
{
    ENUMS_EQUAL_INT(UNSCOPED_ENUM_B, UNSCOPED_ENUM_A); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void ENUMS_EQUAL_INT_TEXTWithUnscopedEnumTestMethod_()
{
    ENUMS_EQUAL_INT_TEXT(UNSCOPED_ENUM_B, UNSCOPED_ENUM_A, "Failed because it failed");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, TestENUMS_EQUAL_INT_TEXTWithUnscopedEnum)
{
    fixture.runTestWithMethod(ENUMS_EQUAL_INT_TEXTWithUnscopedEnumTestMethod_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros, ENUMS_EQUAL_INT_TEXTWithUnscopedEnumBehavesAsProperMacro)
{
    if (false) ENUMS_EQUAL_INT_TEXT(UNSCOPED_ENUM_B, UNSCOPED_ENUM_A, "Failed because it failed");
    else ENUMS_EQUAL_INT_TEXT(UNSCOPED_ENUM_B, UNSCOPED_ENUM_B, "Failed because it failed");
}

IGNORE_TEST(UnitTestMacros, ENUMS_EQUAL_EQUAL_INT_TEXTWithUnscopedEnumWorksInAnIgnoredTest)
{
    ENUMS_EQUAL_INT_TEXT(UNSCOPED_ENUM_B, UNSCOPED_ENUM_A, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

#if CPPUTEST_USE_STD_CPP_LIB
static void failingTestMethod_NoThrowWithCHECK_THROWS_()
{
    CHECK_THROWS(int, (void) (1+2));
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_THROWS_whenDoesntThrow)
{
    fixture.runTestWithMethod(failingTestMethod_NoThrowWithCHECK_THROWS_);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw nothing");
    LONGS_EQUAL(1, fixture.getCheckCount());
}

static void succeedingTestMethod_CorrectThrowWithCHECK_THROWS_()
{
    CHECK_THROWS(int, throw 4);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
}

TEST(UnitTestMacros, SuccessWithCHECK_THROWS)
{
    fixture.runTestWithMethod(succeedingTestMethod_CorrectThrowWithCHECK_THROWS_);
    LONGS_EQUAL(1, fixture.getCheckCount());
}

static void failingTestMethod_WrongThrowWithCHECK_THROWS_()
{
    CHECK_THROWS(int, throw 4.3);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(UnitTestMacros, FailureWithCHECK_THROWS_whenWrongThrow)
{
    fixture.runTestWithMethod(failingTestMethod_WrongThrowWithCHECK_THROWS_);
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
