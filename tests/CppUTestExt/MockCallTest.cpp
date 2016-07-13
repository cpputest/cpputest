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

TEST(MockCallTest, exceptACallThatHappens)
{
    mock().expectOneCall("func");
    mock().actualCall("func");
    CHECK(! mock().expectedCallsLeft());
}

TEST(MockCallTest, exceptACallInceasesExpectedCallsLeft)
{
    mock().expectOneCall("func");
    CHECK(mock().expectedCallsLeft());
    mock().clear();
}

TEST(MockCallTest, checkExpectationsClearsTheExpectations)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foobar");
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

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
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

    mock("scope").expectOneCall("foobar");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

}

TEST(MockCallTest, unexpectedCallHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest emptyExpectations;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "func", emptyExpectations);

    mock().actualCall("func");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, unexpectedScopeCallHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest emptyExpectations;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "scope::func", emptyExpectations);

    mock("scope").actualCall("func");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectOneCallInOneScopeButActualCallInAnotherScope)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest emptyExpectations;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "class::foo", emptyExpectations);

    mock("scope").expectOneCall("foo");
    mock("class").actualCall("foo");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
    mock().clear();
}

TEST(MockCallTest, expectOneCallInScopeButActualCallInGlobal)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest emptyExpectations;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", emptyExpectations);

    mock("scope").expectOneCall("foo");
    mock().actualCall("foo");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
    mock().clear();
}


TEST(MockCallTest, expectMultipleCallsThatHappen)
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

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->callWasMade(1);
    expectations.addFunction("foo")->callWasMade(2);
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", expectations);

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
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectations);

    mock().expectNoCall("lazy");
    mock().actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectNoCallDoesntInfluenceExpectOneCall)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("influence")->callWasMade(1);
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectations);

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
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectations);

    mock().expectNoCall("lazy");
    mock().actualCall("lazy");
    mock().actualCall("lazy");
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, ignoreOtherCallsExceptForTheUnExpectedOne)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectations);

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
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "scope::lazy", expectations);

    mock("scope").expectNoCall("lazy");
    mock("scope").actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectNoCallInScopeButActualCallInAnotherScope)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "scope2::lazy", expectations);

    mock("scope1").expectNoCall("lazy");
    mock("scope2").actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectNoCallInScopeButActualCallInGlobal)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "lazy", expectations);

    mock("scope1").expectNoCall("lazy");
    mock().actualCall("lazy");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, ignoreOtherCallsExceptForTheExpectedOne)
{
    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().actualCall("bar").withParameter("foo", 1);;

    mock().clear();
}

TEST(MockCallTest, ignoreOtherCallsDoesntIgnoreMultipleCallsOfTheSameFunction)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->callWasMade(1);
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", expectations);

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
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

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

    mock().expectOneCall("boo").onObject(objectPtr);
    mock().actualCall("boo").onObject(objectPtr2);

    MockUnexpectedObjectFailure expectedFailure(mockFailureTest(), "boo", objectPtr2, expectations);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, OnObjectExpectedButNotCalled)
{
    MockFailureReporterInstaller failureReporterInstaller;

    void* objectPtr = (void*) 0x001;
    MockExpectedCallsListForTest expectations;
    expectations.addFunction("boo")->onObject(objectPtr);
    expectations.addFunction("boo")->onObject(objectPtr);

    mock().expectOneCall("boo").onObject(objectPtr);
    mock().expectOneCall("boo").onObject(objectPtr);
    mock().actualCall("boo");
    mock().actualCall("boo");

    MockExpectedObjectDidntHappenFailure expectedFailure(mockFailureTest(), "boo", expectations);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectMultipleCalls)
{
    mock().expectNCalls(2, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
}

TEST(MockCallTest, shouldntFailTwice)
{
  MockFailureReporterInstaller failureReporterInstaller;

  mock().expectOneCall("foo");
  mock().actualCall("bar");
  mock().checkExpectations();

  CHECK(!MockFailureReporterForTest::getReporter()->mockFailureString.contains("bar"));
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
    LONGS_EQUAL(5, fixture.getCheckCount());
}

