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

TEST_GROUP(MockStrictOrderTest)
{
  void teardown() _override
  {
    mock().clear();
  }
};

TEST(MockStrictOrderTest, OrderObserved)
{
    mock().strictOrder();

    mock().expectOneCall("foo1");
    mock().expectOneCall("foo2");
    mock().actualCall("foo1");
    mock().actualCall("foo2");

    mock().checkExpectations();
}

TEST(MockStrictOrderTest, someOrderObserved)
{
    mock().expectOneCall("foo3").withCallOrder(3);
    mock().expectOneCall("foo1");
    mock().expectOneCall("foo2");
    mock().actualCall("foo2");
    mock().actualCall("foo1");
    mock().actualCall("foo3");

    mock().checkExpectations();
}

TEST(MockStrictOrderTest, orderViolated)
{
    MockFailureReporterInstaller failureReporterInstaller;
    mock().strictOrder();

    MockExpectedCallsListForTest expectations;
    expectations.addFunctionOrdered("foo1", 1)->callWasMade(1);
    expectations.addFunctionOrdered("foo1", 2)->callWasMade(3);
    expectations.addFunctionOrdered("foo2", 3)->callWasMade(2);
    MockCallOrderFailure expectedFailure(mockFailureTest(), expectations);

    mock().expectOneCall("foo1");
    mock().expectOneCall("foo1");
    mock().expectOneCall("foo2");
    mock().actualCall("foo1");
    mock().actualCall("foo2");
    mock().actualCall("foo1");

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockStrictOrderTest, orderViolatedWorksHierarchically)
{
    MockFailureReporterInstaller failureReporterInstaller;
    mock().strictOrder();
    mock("bla").strictOrder();

    MockExpectedCallsListForTest expectations;
    expectations.addFunctionOrdered("foo::foo1", 1)->callWasMade(2);
    expectations.addFunctionOrdered("foo::foo2", 2)->callWasMade(1);
    MockCallOrderFailure expectedFailure(mockFailureTest(), expectations);

    mock("bla").expectOneCall("foo1");
    mock("foo").expectOneCall("foo1");
    mock("foo").expectOneCall("foo2");

    mock("bla").actualCall("foo1");
    mock("foo").actualCall("foo2");
    mock("foo").actualCall("foo1");

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockStrictOrderTest, orderViolatedWorksWithExtraUnexpectedCall)
{
    MockFailureReporterInstaller failureReporterInstaller;
    mock().strictOrder();
    mock("bla").strictOrder();
	mock().ignoreOtherCalls();

    MockExpectedCallsListForTest expectations;
    expectations.addFunctionOrdered("foo::foo1", 1)->callWasMade(2);
    expectations.addFunctionOrdered("foo::foo2", 2)->callWasMade(1);
    MockCallOrderFailure expectedFailure(mockFailureTest(), expectations);

    mock("bla").expectOneCall("foo1");
    mock("foo").expectOneCall("foo1");
    mock("foo").expectOneCall("foo2");

    mock("bla").actualCall("foo1");
    mock("foo").actualCall("foo2");
	mock("foo").actualCall("unexpected1");
    mock("foo").actualCall("foo1");
	mock("foo").actualCall("unexpected2");

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockStrictOrderTest, orderViolatedWithinAScope)
{
    MockFailureReporterInstaller failureReporterInstaller;
    mock().strictOrder();

    MockExpectedCallsListForTest expectations;
    expectations.addFunctionOrdered("scope::foo1", 1)->callWasMade(2);
    expectations.addFunctionOrdered("scope::foo2", 2)->callWasMade(1);
    MockCallOrderFailure expectedFailure(mockFailureTest(), expectations);

    mock("scope").expectOneCall("foo1");
    mock("scope").expectOneCall("foo2");
    mock("scope").actualCall("foo2");
    mock("scope").actualCall("foo1");

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockStrictOrderTest, orderNotViolatedAcrossScopes)
{
    mock("mock1").strictOrder();
    mock("mock2").strictOrder();

    mock("mock1").expectOneCall("foo1");
    mock("mock2").expectOneCall("foo2");
    mock("mock1").actualCall("foo1");
    mock("mock2").actualCall("foo2");

    mock("mock1").checkExpectations();
    mock("mock2").checkExpectations();
}

TEST(MockStrictOrderTest, orderViolatedAcrossScopes)
{
    mock("mock1").strictOrder();
    mock("mock2").strictOrder();

    mock("mock1").expectOneCall("foo1");
    mock("mock2").expectOneCall("foo2");
    mock("mock2").actualCall("foo2");
    mock("mock1").actualCall("foo1");

    mock("mock1").checkExpectations();
    mock("mock2").checkExpectations();
}

TEST(MockStrictOrderTest, orderUsingNCalls)
{
    mock().strictOrder();

    mock().expectOneCall("foo1");
    mock().expectNCalls(2, "foo2");
    mock().expectOneCall("foo1");
    mock().expectNCalls(3, "foo2");

    mock().actualCall("foo1");
    mock().actualCall("foo2");
    mock().actualCall("foo2");
    mock().actualCall("foo1");
    mock().actualCall("foo2");
    mock().actualCall("foo2");
    mock().actualCall("foo2");

    mock().checkExpectations();
}
