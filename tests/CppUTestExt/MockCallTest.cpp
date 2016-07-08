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
#include "CppUTest/TestTestingFixture.h"
#include "MockFailureReporterForTest.h"

TEST_GROUP(MockCallTest)
{
  void teardown()
  {
    mock().checkExpectations();
  }
};

TEST(MockCallTest, clear)
{
    mock().expectOneCall("func");
    mock().clear();
    CHECK(! mock().expectedCallsLeft());
}

TEST(MockCallTest, checkExpectationsDoesntFail)
{
    mock().checkExpectations();
}

TEST(MockCallTest, expectASingleCallThatHappens)
{
    mock().expectOneCall("func");
    MockCheckedActualCall& actualCall = (MockCheckedActualCall&) mock().actualCall("func");
    actualCall.checkExpectations();
    CHECK(! mock().expectedCallsLeft());
}

TEST(MockCallTest, expectASingleCallThatDoesntHappen)
{
    mock().expectOneCall("func");
    CHECK(mock().expectedCallsLeft());
    mock().clear();
}

TEST(MockCallTest, expectAMultiCallThatHappensTheMinimumTimes)
{
    mock().expectRangeOfCalls(2, 4, "func");
    mock().actualCall("func");
    MockCheckedActualCall& actualCall = (MockCheckedActualCall&) mock().actualCall("func");
    actualCall.checkExpectations();
    CHECK(! mock().expectedCallsLeft());
}

TEST(MockCallTest, expectAMultiCallThatDoesntHappenTheMinimumTimes)
{
    mock().expectRangeOfCalls(2, 4, "func");
    MockCheckedActualCall& actualCall = (MockCheckedActualCall&) mock().actualCall("func");
    actualCall.checkExpectations();
    CHECK(mock().expectedCallsLeft());
    mock().clear();
}

TEST(MockCallTest, checkExpectationsClearsTheExpectations)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foobar");
    MockActualCallsQueueForTest actualCalls;
    MockExpectedCallsNotFulfilledFailure expectedFailure(mockFailureTest(), expectations, actualCalls);

    mock().expectOneCall("foobar");
    mock().checkExpectations();

    CHECK(! mock().expectedCallsLeft());
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectOneCallInScopeButNotHappen)
{

    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("scope::foobar");
    MockActualCallsQueueForTest actualCalls;
    MockExpectedCallsNotFulfilledFailure expectedFailure(mockFailureTest(), expectations, actualCalls);

    mock("scope").expectOneCall("foobar");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

}

TEST(MockCallTest, unexpectedCallHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest emptyExpectations;
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "func", emptyExpectations, actualCalls);

    mock().actualCall("func");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, unexpectedScopeCallHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest emptyExpectations;
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "scope::func", emptyExpectations, actualCalls);

    mock("scope").actualCall("func");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectOneCallInOneScopeButActualCallInAnotherScope)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest emptyExpectations;
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "class::foo", emptyExpectations, actualCalls);

    mock("scope").expectOneCall("foo");
    mock("class").actualCall("foo");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
    mock().clear();
}

TEST(MockCallTest, expectOneCallInScopeButActualCallInGlobal)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest emptyExpectations;
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", emptyExpectations, actualCalls);

    mock("scope").expectOneCall("foo");
    mock().actualCall("foo");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
    mock().clear();
}


TEST(MockCallTest, expectMultipleSingleCallsThatHappen)
{
    mock().expectOneCall("foo");
    mock().expectOneCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectOneCallHoweverMultipleHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction("foo");
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("foo");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);
    actualCalls.addCall(2, expectedCall2);

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", expectedCalls, actualCalls);

    mock().expectOneCall("foo");
    mock().expectOneCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectNoCallThatHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction(0, 0, "lazy");
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectations, actualCalls);

    mock().expectNoCall("lazy");
    mock().actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectNoCallDoesntInfluenceExpectOneCall)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    expectedCalls.addFunction(0, 0, "lazy");
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("influence");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall2);

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectedCalls, actualCalls);

    mock().expectNoCall("lazy");
    mock().expectOneCall("influence");
    mock().actualCall("influence");
    mock().actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectNoCallOnlyFailureOnceWhenMultipleHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction(0, 0, "lazy");
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectations, actualCalls);

    mock().expectNoCall("lazy");
    mock().actualCall("lazy");
    mock().actualCall("lazy");
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, ignoreOtherCallsExceptForTheUnExpectedOne)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction(0, 0, "lazy");
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectations, actualCalls);

    mock().expectNoCall("lazy");
    mock().ignoreOtherCalls();
    mock().actualCall("bar").withParameter("foo", 1);
    mock().actualCall("bar1").withParameter("foo", 1);
    mock().actualCall("bar2").withParameter("foo", 1);
    mock().actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}


TEST(MockCallTest, expectNoCallInScopeThatHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction(0, 0, "scope::lazy");
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "scope::lazy", expectations, actualCalls);

    mock("scope").expectNoCall("lazy");
    mock("scope").actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectNoCallInScopeButActualCallInAnotherScope)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "scope2::lazy", expectations, actualCalls);

    mock("scope1").expectNoCall("lazy");
    mock("scope2").actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectNoCallInScopeButActualCallInGlobal)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectations, actualCalls);

    mock("scope1").expectNoCall("lazy");
    mock().actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, ignoreOtherCallsExceptForTheExpectedOne)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    expectedCalls.addFunction("foo");
    MockActualCallsQueueForTest actualCalls;
    MockExpectedCallsNotFulfilledFailure expectedFailure(mockFailureTest(), expectedCalls, actualCalls);

    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().actualCall("bar").withParameter("foo", 1);
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, ignoreOtherCallsDoesntIgnoreMultipleCallsOfTheSameFunction)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction("foo");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", expectedCalls, actualCalls);

    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().actualCall("bar");
    mock().actualCall("foo");
    mock().actualCall("foo");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, ignoreOtherStillFailsIfExpectedOneDidntHappen)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo");
    MockActualCallsQueueForTest actualCalls;
    MockExpectedCallsNotFulfilledFailure expectedFailure(mockFailureTest(), expectations, actualCalls);

    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, threeExpectedAndActual)
{
    mock().expectOneCall("function1");
    mock().expectOneCall("function2");
    mock().expectOneCall("function3");
    mock().actualCall("function1");
    mock().actualCall("function2");
    mock().actualCall("function3");

    mock().checkExpectations();
}

TEST(MockCallTest, disableEnable)
{
    mock().disable();
    mock().expectOneCall("function");
    mock().actualCall("differenFunction");
    CHECK(! mock().expectedCallsLeft());
    mock().enable();
    mock().expectOneCall("function");
    CHECK(mock().expectedCallsLeft());
    mock().actualCall("function");

    mock().checkExpectations();
}

TEST(MockCallTest, OnObject)
{
    void* objectPtr = (void*) 0x001;
    mock().expectOneCall("boo").onObject(objectPtr);
    mock().actualCall("boo").onObject(objectPtr);
}

TEST(MockCallTest, OnObjectFails)
{
    MockFailureReporterInstaller failureReporterInstaller;

    void* objectPtr = (void*) 0x001;
    void* objectPtr2 = (void*) 0x002;
    MockExpectedCallsListForTest expectations;
    expectations.addFunction("boo")->onObject(objectPtr);
    MockActualCallsQueueForTest actualCalls;

    mock().expectOneCall("boo").onObject(objectPtr);
    mock().actualCall("boo").onObject(objectPtr2);

    MockUnexpectedObjectFailure expectedFailure(mockFailureTest(), "boo", objectPtr2, expectations, actualCalls);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, OnObjectExpectedButNotCalled)
{
    MockFailureReporterInstaller failureReporterInstaller;

    void* objectPtr = (void*) 0x001;
    MockExpectedCallsListForTest expectations;
    expectations.addFunction("boo")->onObject(objectPtr);
    expectations.addFunction("boo")->onObject(objectPtr);
    MockActualCallsQueueForTest actualCalls;

    mock().expectOneCall("boo").onObject(objectPtr);
    mock().expectOneCall("boo").onObject(objectPtr);
    mock().actualCall("boo");
    mock().actualCall("boo");

    MockExpectedObjectDidntHappenFailure expectedFailure(mockFailureTest(), "boo", expectations, actualCalls);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectNCalls_Fulfilled)
{
    mock().expectNCalls(2, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAtLeastOneCall_Fulfilled_Minimum)
{
    mock().expectAtLeastOneCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAtLeastOneCall_Fulfilled_MoreThanMinimum)
{
    mock().expectAtLeastOneCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAtLeastNCalls_Fulfilled_Minimum)
{
    mock().expectAtLeastNCalls(3, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAtLeastNCalls_Fulfilled_MoreThanMinimum)
{
    mock().expectAtLeastNCalls(3, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAtMostOneCall_Fulfilled_Minimum)
{
    mock().expectAtMostOneCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAtMostOneCall_Fulfilled_Maximum)
{
    mock().expectAtMostOneCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAtMostNCalls_Fulfilled_Minimum)
{
    mock().expectAtMostNCalls(2, "boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAtMostNCalls_Fulfilled_BetweenMinAndMax)
{
    mock().expectAtMostNCalls(2, "boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAtMostNCalls_Fulfilled_Maximum)
{
    mock().expectAtMostNCalls(2, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAnyCalls_Fulfilled_Minimum)
{
    mock().expectAnyCalls("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectAnyCalls_Fulfilled_MoreThanMinimum)
{
    mock().expectAnyCalls("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectRangeOfCalls_Fulfilled_Minimum)
{
    mock().expectRangeOfCalls(2, 6, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectRangeOfCalls_Fulfilled_BetweenMinAndMax)
{
    mock().expectRangeOfCalls(2, 6, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectRangeOfCalls_Fulfilled_Maximum)
{
    mock().expectRangeOfCalls(2, 6, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectNCalls_NotFulfilled)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction(2, 2, "boo");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);

    MockExpectedCallsNotFulfilledFailure expectedFailure(mockFailureTest(), expectedCalls, actualCalls);

    mock().expectNCalls(2, "boo");
    mock().actualCall("boo");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectAtLeastOneCall_NotFulfilled)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction(1, (unsigned int)-1, "boo");
    MockActualCallsQueueForTest actualCalls;
    MockExpectedCallsNotFulfilledFailure expectedFailure(mockFailureTest(), expectations, actualCalls);

    mock().expectAtLeastOneCall("boo");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectAtLeastNCalls_NotFulfilled)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction(3, (unsigned int)-1, "boo");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);
    actualCalls.addCall(2, expectedCall1);

    MockExpectedCallsNotFulfilledFailure expectedFailure(mockFailureTest(), expectedCalls, actualCalls);

    mock().expectAtLeastNCalls(3, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectAtMostOneCall_NotFulfilled)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction(0, 1, "boo");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "boo", expectedCalls, actualCalls);

    mock().expectAtMostOneCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectAtMostNCalls_NotFulfilled)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction(0, 3, "boo");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);
    actualCalls.addCall(2, expectedCall1);
    actualCalls.addCall(3, expectedCall1);

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "boo", expectedCalls, actualCalls);

    mock().expectAtMostNCalls(3, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectRangeOfCalls_NotFulfilled_BelowMinimum)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction(2, 3, "boo");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);

    MockExpectedCallsNotFulfilledFailure expectedFailure(mockFailureTest(), expectedCalls, actualCalls);

    mock().expectRangeOfCalls(2, 3, "boo");
    mock().actualCall("boo");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectRangeOfCalls_NotFulfilled_AboveMaximum)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction(2, 3, "boo");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);
    actualCalls.addCall(2, expectedCall1);
    actualCalls.addCall(3, expectedCall1);

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "boo", expectedCalls, actualCalls);

    mock().expectRangeOfCalls(2, 3, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectedCallsNotFulfilledMatchBeforeFulfilledOnes)
{
    mock().expectAtLeastOneCall("foo");
    mock().expectAtLeastNCalls(2, "foo");

    mock().actualCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");

    mock().checkExpectations();
}

TEST(MockCallTest, shouldntFailTwice)
{
  MockFailureReporterInstaller failureReporterInstaller;

  mock().strictOrder();
  mock().expectOneCall("foo");
  mock().expectOneCall("boo");
  mock().actualCall("boo");
  mock().actualCall("bar");
  mock().checkExpectations();

  CHECK(!MockFailureReporterForTest::getReporter()->mockFailureString.contains("bar"));
  CHECK(MockFailureReporterForTest::getReporter()->mockFailureString.contains("boo"));
}

TEST(MockCallTest, shouldReturnDefaultWhenThereIsntAnythingToReturn)
{
    CHECK(mock().returnValue().equals(MockNamedValue("")));
}

IGNORE_TEST(MockCallTest, testForPerformanceProfiling)
{
    /* TO fix! */
    mock().expectNCalls(2000, "SimpleFunction");
    for (int i = 0; i < 2000; i++) {
        mock().actualCall("SimpleFunction");
    }
}

static void mocksAreCountedAsChecksTestFunction_()
{
    mock().expectOneCall("foo");
    mock().expectNCalls(3, "bar");
    mock().expectNoCall("lazy");
    mock().clear();
}

TEST(MockCallTest, mockExpectationShouldIncreaseNumberOfChecks)
{
    TestTestingFixture fixture;
    fixture.setTestFunction(mocksAreCountedAsChecksTestFunction_);
    fixture.runAllTests();
    LONGS_EQUAL(3, fixture.getCheckCount());
}

