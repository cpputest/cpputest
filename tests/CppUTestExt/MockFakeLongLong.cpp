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
#include "CppUTestExt/MockCheckedActualCall.h"
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockExpectedCallsList.h"
#include "CppUTestExt/MockFailure.h"
#include "MockFailureReporterForTest.h"

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text) fixture.checkTestFailsWithProperTestLocation(text, __FILE__, __LINE__)

TEST_GROUP(FakeLongLongs)
{
    TestTestingFixture fixture;
};

#ifndef CPPUTEST_USE_LONG_LONG

static void _actualCallWithFakeLongLongParameter()
{
    cpputest_longlong value = {0};

    mock().expectOneCall("foo").withParameter("bar", 0);
    mock().actualCall("foo").withParameter("bar", value);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(FakeLongLongs, ActualCallWithFakeLongLongParameterFAILS)
{
    fixture.runTestWithMethod(_actualCallWithFakeLongLongParameter);
    mock().clear();
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Long Long type is not supported");
}

static void _actualCallWithFakeUnsignedLongLongParameter()
{
    cpputest_ulonglong value = {0};

    mock().expectOneCall("foo").withParameter("bar", 0);
    mock().actualCall("foo").withParameter("bar", value);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(FakeLongLongs, ActualCallWithFakeUnsignedLongLongParameterFAILS)
{
    fixture.runTestWithMethod(_actualCallWithFakeUnsignedLongLongParameter);
    mock().clear();
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Unsigned Long Long type is not supported");
}

static void _actualCallWithFakeLongLongReturn()
{
    mock().expectOneCall("foo").andReturnValue(0);
    mock().actualCall("foo").returnLongLongIntValue();
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(FakeLongLongs, ActualCallWithFakeLongLongReturnFAILS)
{
    fixture.runTestWithMethod(_actualCallWithFakeLongLongReturn);
    mock().clear();
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Long Long type is not supported");
}

static void _actualCallWithFakeUnsignedLongLongReturn()
{
    mock().expectOneCall("foo").andReturnValue(0);
    mock().actualCall("foo").returnUnsignedLongLongIntValue();
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(FakeLongLongs, ActualCallWithFakeUnsignedLongLongReturnFAILS)
{
    fixture.runTestWithMethod(_actualCallWithFakeUnsignedLongLongReturn);
    mock().clear();
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Unsigned Long Long type is not supported");
}

static void _expectOneCallWithFakeLongLongParameter()
{
    cpputest_longlong value = {0};

    mock().expectOneCall("foo").withParameter("bar", value);
    mock().actualCall("foo").withParameter("bar", 0);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(FakeLongLongs, ExpectedCallWithFakeLongLongParameterFAILS)
{
    fixture.runTestWithMethod(_expectOneCallWithFakeLongLongParameter);
    mock().clear();
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Long Long type is not supported");
}

static void _expectOneCallWithFakeUnsignedLongLongParameter()
{
    cpputest_ulonglong value = {0};

    mock().expectOneCall("foo").withParameter("bar", value);
    mock().actualCall("foo").withParameter("bar", 0);
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(FakeLongLongs, ExpectedCallWithFakeUnsignedLongLongParameterFAILS)
{
    fixture.runTestWithMethod(_expectOneCallWithFakeUnsignedLongLongParameter);
    mock().clear();
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Unsigned Long Long type is not supported");
}

static void _expectOneCallWithFakeLongLongReturn()
{
    cpputest_longlong value = {0};

    mock().expectOneCall("foo").andReturnValue(value);
    mock().actualCall("foo").returnIntValue();
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(FakeLongLongs, ExpectedCallWithFakeLongLongReturnFAILS)
{
    fixture.runTestWithMethod(_expectOneCallWithFakeLongLongReturn);
    mock().clear();
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Long Long type is not supported");
}

static void _expectOneCallWithFakeUnsignedLongLongReturn()
{
    cpputest_ulonglong value = {0};

    mock().expectOneCall("foo").andReturnValue(value);
    mock().actualCall("foo").returnIntValue();
    TestTestingFixture::lineExecutedAfterCheck(); // LCOV_EXCL_LINE
} // LCOV_EXCL_LINE

TEST(FakeLongLongs, ExpectedCallWithFakeUnsignedLongLongReturnFAILS)
{
    fixture.runTestWithMethod(_expectOneCallWithFakeUnsignedLongLongReturn);
    mock().clear();
    CHECK_TEST_FAILS_PROPER_WITH_TEXT("Unsigned Long Long type is not supported");
}

#endif
