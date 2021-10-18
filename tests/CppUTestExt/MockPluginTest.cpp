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
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupportPlugin.h"
#include "MockFailureReporterForTest.h"
#include "CppUTest/TestTestingFixture.h"

TEST_GROUP(MockPlugin)
{
    StringBufferTestOutput output;

    UtestShell *test;
    TestResult *result;

    MockSupportPlugin plugin;

    void setup() _override
    {
        test = new UtestShell("group", "name", "file", 1);
        result = new TestResult(output);
    }

    void teardown() _override
    {
        delete test;
        delete result;
        mock().clear();
        mock().removeAllComparatorsAndCopiers();
    }
};

TEST(MockPlugin, checkExpectationsAndClearAtEnd)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foobar");
    MockExpectedCallsDidntHappenFailure expectedFailure(test, expectations);

    mock().expectOneCall("foobar");

    plugin.postTestAction(*test, *result);

    STRCMP_CONTAINS(expectedFailure.getMessage().asCharString(), output.getOutput().asCharString());
    LONGS_EQUAL(0, mock().expectedCallsLeft());
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockPlugin, checkExpectationsWorksAlsoWithHierachicalObjects)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("differentScope::foobar")->onObject((void*) 1);
    MockExpectedObjectDidntHappenFailure expectedFailure(test, "differentScope::foobar", expectations);

    mock("differentScope").expectOneCall("foobar").onObject((void*) 1);
    mock("differentScope").actualCall("foobar");

    plugin.postTestAction(*test, *result);

    STRCMP_CONTAINS(expectedFailure.getMessage().asCharString(), output.getOutput().asCharString());
    CHECK_NO_MOCK_FAILURE();
}

class DummyComparator : public MockNamedValueComparator
{
public:
    bool isEqual(const void* object1, const void* object2) _override
    {
        return object1 == object2;
    }
    SimpleString valueToString(const void*) _override
    {
        return "string";
    }
};

TEST(MockPlugin, installComparatorRecordsTheComparatorButNotInstallsItYet)
{
    MockFailureReporterInstaller failureReporterInstaller;

    DummyComparator comparator;
    plugin.installComparator("myType", comparator);
    mock().expectOneCall("foo").withParameterOfType("myType", "name", NULLPTR);
    mock().actualCall("foo").withParameterOfType("myType", "name", NULLPTR);

    MockNoWayToCompareCustomTypeFailure failure(test, "myType");
    CHECK_EXPECTED_MOCK_FAILURE(failure);

    plugin.clear();
}

class DummyCopier : public MockNamedValueCopier
{
public:
    void copy(void* dst, const void* src) _override
    {
        *(int*)dst = *(const int*)src;
    }
};

TEST(MockPlugin, installCopierRecordsTheCopierButNotInstallsItYet)
{
    MockFailureReporterInstaller failureReporterInstaller;

    DummyCopier copier;
    plugin.installCopier("myType", copier);
    mock().expectOneCall("foo").withOutputParameterOfTypeReturning("myType", "name", NULLPTR);
    mock().actualCall("foo").withOutputParameterOfType("myType", "name", NULLPTR);

    MockNoWayToCopyCustomTypeFailure failure(test, "myType");
    CHECK_EXPECTED_MOCK_FAILURE(failure);

    plugin.clear();
}

TEST(MockPlugin, preTestActionWillEnableMultipleComparatorsToTheGlobalMockSupportSpace)
{
    DummyComparator comparator;
    DummyComparator comparator2;
    plugin.installComparator("myType", comparator);
    plugin.installComparator("myOtherType", comparator2);

    plugin.preTestAction(*test, *result);
    mock().expectOneCall("foo").withParameterOfType("myType", "name", &comparator);
    mock().expectOneCall("foo").withParameterOfType("myOtherType", "name", &comparator);
    mock().actualCall("foo").withParameterOfType("myType", "name", &comparator);
    mock().actualCall("foo").withParameterOfType("myOtherType", "name", &comparator);

    mock().checkExpectations();
    LONGS_EQUAL(0, result->getFailureCount());

    plugin.clear();
}

static void failTwiceFunction_()
{
    mock().expectOneCall("foobar");
    FAIL("This failed");
}

TEST(MockPlugin, shouldNotFailAgainWhenTestAlreadyFailed)
{
    TestTestingFixture fixture;
    fixture.installPlugin(&plugin);
    fixture.setTestFunction(failTwiceFunction_);
    fixture.runAllTests();
    fixture.assertPrintContains("1 failures, 1 tests, 1 ran, 2 checks,");
}
