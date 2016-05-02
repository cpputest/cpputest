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

TEST_GROUP(UnitTestMacros2)
{
    TestTestingFixture fixture;
    void setup()
    {
        TestTestingFixture::setLineOfCodeExecutedAfterCheck(false);
    }

    void runTestWithMethod(void(*method)())
    {
        fixture.setTestFunction(method);
        fixture.runAllTests();
    }
};

#ifdef CPPUTEST_USE_LONG_LONG

static void _LONGLONGS_EQUALTestMethod()
{
    LONGLONGS_EQUAL(1, 1);
    LONGLONGS_EQUAL(1, 0);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros2, TestLONGLONGS_EQUAL)
{
    runTestWithMethod(_LONGLONGS_EQUALTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 0x1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 0x0>");
}

TEST(UnitTestMacros2, LONGLONGS_EQUALBehavesAsProperMacro)
{
    if (false) LONGLONGS_EQUAL(1, 0)
    else LONGLONGS_EQUAL(1, 1)
}

IGNORE_TEST(UnitTestMacros2, LONGLONGS_EQUALWorksInAnIgnoredTest)
{
    LONGLONGS_EQUAL(1, 0); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _LONGLONGS_EQUAL_TEXTTestMethod()
{
    LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros2, TestLONGLONGS_EQUAL_TEXT)
{
    runTestWithMethod(_LONGLONGS_EQUAL_TEXTTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 0x1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 0x0>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros2, LONGLONGS_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed")
    else LONGLONGS_EQUAL_TEXT(1, 1, "Failed because it failed")
}

IGNORE_TEST(UnitTestMacros2, LONGLONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
    LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _UNSIGNED_LONGLONGS_EQUALTestMethod()
{
    UNSIGNED_LONGLONGS_EQUAL(1, 1);
    UNSIGNED_LONGLONGS_EQUAL(1, 0);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros2, TestUNSIGNED_LONGLONGS_EQUAL)
{
    runTestWithMethod(_UNSIGNED_LONGLONGS_EQUALTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1) 0x1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0) 0x0>");
}

TEST(UnitTestMacros2, UNSIGNED_LONGLONGS_EQUALBehavesAsProperMacro)
{
    if (false) UNSIGNED_LONGLONGS_EQUAL(1, 0)
    else UNSIGNED_LONGLONGS_EQUAL(1, 1)
}

IGNORE_TEST(UnitTestMacros2, UNSIGNED_LONGLONGS_EQUALWorksInAnIgnoredTest)
{
    UNSIGNED_LONGLONGS_EQUAL(1, 0); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

static void _UNSIGNED_LONGLONGS_EQUAL_TEXTTestMethod()
{
    UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
} // LCOV_EXCL_LINE

TEST(UnitTestMacros2, TestUNSIGNED_LONGLONGS_EQUAL_TEXT)
{
    runTestWithMethod(_UNSIGNED_LONGLONGS_EQUAL_TEXTTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1) 0x1>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0) 0x0>");
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestMacros2, UNSIGNED_LONGLONGS_EQUAL_TEXTBehavesAsProperMacro)
{
    if (false) UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed")
    else UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 1, "Failed because it failed")
}

IGNORE_TEST(UnitTestMacros2, UNSIGNED_LONGLONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
    UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed"); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

#else

static void _LONGLONGS_EQUALFailsWithUnsupportedFeatureTestMethod()
{
    LONGLONGS_EQUAL(1, 1);
} // LCOV_EXCL_LINE

static void _UNSIGNED_LONGLONGS_EQUALFailsWithUnsupportedFeatureTestMethod()
{
    UNSIGNED_LONGLONGS_EQUAL(1, 1);
} // LCOV_EXCL_LINE

TEST(UnitTestMacros2, LONGLONGS_EQUALFailsWithUnsupportedFeature)
{
    runTestWithMethod(_LONGLONGS_EQUALFailsWithUnsupportedFeatureTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("\"CPPUTEST_USE_LONG_LONG\" is not supported");
}

TEST(UnitTestMacros2, UNSIGNED_LONGLONGS_EQUALFailsWithUnsupportedFeature)
{
    runTestWithMethod(_UNSIGNED_LONGLONGS_EQUALFailsWithUnsupportedFeatureTestMethod);
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("\"CPPUTEST_USE_LONG_LONG\" is not supported");
}

#endif /* CPPUTEST_USE_LONG_LONG */
