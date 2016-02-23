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
#include "MockFailureReporterForTest.h"

TEST_GROUP(MockHierarchyTest)
{
};

TEST(MockHierarchyTest, getMockSupportScope)
{
    MockSupport* mock1 = mock().getMockSupportScope("name");
    MockSupport* mock2 = mock().getMockSupportScope("differentName");

    CHECK(!mock().hasData("name"));
    CHECK(mock1 != mock2);
    POINTERS_EQUAL(mock1, mock().getMockSupportScope("name"));
    CHECK(mock1 != &mock());
}

TEST(MockHierarchyTest, usingTwoMockSupportsByName)
{
    mock("first").expectOneCall("boo");

    LONGS_EQUAL(0, mock("other").expectedCallsLeft());
    LONGS_EQUAL(1, mock("first").expectedCallsLeft());

    mock("first").clear();
}

TEST(MockHierarchyTest, EnableDisableWorkHierarchically)
{
    mock("first");

    mock().disable();
    mock("first").expectOneCall("boo");
    LONGS_EQUAL(0, mock("first").expectedCallsLeft());

    mock().enable();
    mock("first").expectOneCall("boo");
    LONGS_EQUAL(1, mock("first").expectedCallsLeft());

    mock("first").clear();
}

TEST(MockHierarchyTest, EnableDisableWorkHierarchicallyWhenSupportIsDynamicallyCreated)
{
    mock().disable();
    mock("first").expectOneCall("boo");
    LONGS_EQUAL(0, mock("first").expectedCallsLeft());

    mock().enable();
    mock("second").expectOneCall("boo");
    LONGS_EQUAL(1, mock("second").expectedCallsLeft());

    mock().clear();
}

TEST(MockHierarchyTest, ExpectedCallsLeftWorksHierarchically)
{
    mock("first").expectOneCall("foobar");
    LONGS_EQUAL(1, mock().expectedCallsLeft());

    mock().clear();
}

TEST(MockHierarchyTest, checkExpectationsWorksHierarchically)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("first::foobar");
    expectations.addFunction("second::helloworld");
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

    mock("first").expectOneCall("foobar");
    mock("second").expectOneCall("helloworld");

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockHierarchyTest, ignoreOtherCallsWorksHierarchically)
{
    mock("first");
    mock().ignoreOtherCalls();
    mock("first").actualCall("boo");

    mock().checkExpectations();
}

TEST(MockHierarchyTest, ignoreOtherCallsWorksHierarchicallyWhenDynamicallyCreated)
{
    mock().ignoreOtherCalls();
    mock("first").actualCall("boo");

    mock().checkExpectations();
}

TEST(MockHierarchyTest, checkExpectationsWorksHierarchicallyForLastCallNotFinished)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("first::foobar")->withParameter("boo", 1);
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "first::foobar", expectations);

    mock("first").expectOneCall("foobar").withParameter("boo", 1);
    mock("first").actualCall("foobar");

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockHierarchyTest, reporterIsInheritedInHierarchicalMocks)
{
    MockFailureReporterInstaller failureReporterInstaller;
    MockExpectedCallsListForTest expectations;

    mock("differentScope").actualCall("foobar");

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "differentScope::foobar", expectations);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

