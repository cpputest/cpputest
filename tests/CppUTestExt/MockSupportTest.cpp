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
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockExpectedCall.h"
#include "CppUTestExt/MockFailure.h"
#include "MockFailureTest.h"

TEST_GROUP(MockSupportTest)
{
    MockExpectedCallsList *expectationsList;

    void setup()
    {
        mock().setMockFailureStandardReporter(MockFailureReporterForTest::getReporter());
        expectationsList = new MockExpectedCallsList;
    }

    void teardown()
    {
        mock().checkExpectations();
        CHECK_NO_MOCK_FAILURE();
        expectationsList->deleteAllExpectationsAndClearList();
        delete expectationsList;
        mock().setMockFailureStandardReporter(NULL);
    }

    MockCheckedExpectedCall* addFunctionToExpectationsList(const SimpleString& name)
    {
        MockCheckedExpectedCall* newCall = new MockCheckedExpectedCall;
        newCall->withName(name);
        expectationsList->addExpectedCall(newCall);
        return newCall;
    }

    MockCheckedExpectedCall* addFunctionToExpectationsList(const SimpleString& name, int order)
    {
        MockCheckedExpectedCall* newCall = new MockCheckedExpectedCall;
        newCall->withName(name);
        newCall->withCallOrder(order);
        expectationsList->addExpectedCall(newCall);
        return newCall;
    }

};

TEST(MockSupportTest, clear)
{
    mock().expectOneCall("func");
    mock().clear();
    CHECK(! mock().expectedCallsLeft());
}

TEST(MockSupportTest, checkExpectationsDoesntFail)
{
    mock().checkExpectations();
}

TEST(MockSupportTest, checkExpectationsClearsTheExpectations)
{
    addFunctionToExpectationsList("foobar");
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), *expectationsList);

    mock().expectOneCall("foobar");
    mock().checkExpectations();

    CHECK(! mock().expectedCallsLeft());
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, exceptACallThatHappens)
{
    mock().expectOneCall("func");
    mock().actualCall("func");
    CHECK(! mock().expectedCallsLeft());
}

TEST(MockSupportTest, exceptACallInceasesExpectedCallsLeft)
{
    mock().expectOneCall("func");
    CHECK(mock().expectedCallsLeft());
    mock().clear();
}

TEST(MockSupportTest, unexpectedCallHappened)
{
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "func", *expectationsList);

    mock().actualCall("func");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, ignoreOtherCallsExceptForTheExpectedOne)
{
    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().actualCall("bar").withParameter("foo", 1);;

    CHECK_NO_MOCK_FAILURE();

    mock().clear();
}

TEST(MockSupportTest, ignoreOtherCallsDoesntIgnoreMultipleCallsOfTheSameFunction)
{
    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().actualCall("bar");
    mock().actualCall("foo");
    mock().actualCall("foo");

    addFunctionToExpectationsList("foo")->callWasMade(1);
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, ignoreOtherStillFailsIfExpectedOneDidntHappen)
{
    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().checkExpectations();

    addFunctionToExpectationsList("foo");

    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), *expectationsList);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectMultipleCallsThatHappen)
{
    mock().expectOneCall("foo");
    mock().expectOneCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, strictOrderObserved)
{
    mock().strictOrder();
    mock().expectOneCall("foo1");
    mock().expectOneCall("foo2");
    mock().actualCall("foo1");
    mock().actualCall("foo2");
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, someStrictOrderObserved)
{
    mock().expectOneCall("foo3").withCallOrder(3);
    mock().expectOneCall("foo1");
    mock().expectOneCall("foo2");
    mock().actualCall("foo2");
    mock().actualCall("foo1");
    mock().actualCall("foo3");
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, strictOrderViolated)
{
    mock().strictOrder();
    addFunctionToExpectationsList("foo1", 1)->callWasMade(1);
    addFunctionToExpectationsList("foo1", 2)->callWasMade(3);
    addFunctionToExpectationsList("foo2", 3)->callWasMade(2);
    MockCallOrderFailure expectedFailure(mockFailureTest(), *expectationsList);
    mock().expectOneCall("foo1");
    mock().expectOneCall("foo1");
    mock().expectOneCall("foo2");
    mock().actualCall("foo1");
    mock().actualCall("foo2");
    mock().actualCall("foo1");
    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, strictOrderViolatedWorksHierarchically)
{
    mock().strictOrder();
    mock("bla").strictOrder();
    addFunctionToExpectationsList("foo1", 1)->callWasMade(2);
    addFunctionToExpectationsList("foo2", 2)->callWasMade(1);
    MockCallOrderFailure expectedFailure(mockFailureTest(), *expectationsList);
    mock("bla").expectOneCall("foo1");
    mock().expectOneCall("foo1");
    mock().expectOneCall("foo2");
    mock("bla").actualCall("foo1");
    mock().actualCall("foo2");
    mock().actualCall("foo1");
    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, strictOrderViolatedWithinAScope)
{
    mock().strictOrder();
    addFunctionToExpectationsList("foo1", 1)->callWasMade(2);
    addFunctionToExpectationsList("foo2", 2)->callWasMade(1);
    MockCallOrderFailure expectedFailure(mockFailureTest(), *expectationsList);
    mock("scope").expectOneCall("foo1");
    mock("scope").expectOneCall("foo2");
    mock("scope").actualCall("foo2");
    mock("scope").actualCall("foo1");
    mock("scope").checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, strictOrderNotViolatedAcrossScopes)
{
    mock("mock1").strictOrder();
    mock("mock2").strictOrder();
    mock("mock1").expectOneCall("foo1");
    mock("mock2").expectOneCall("foo2");

    mock("mock1").actualCall("foo1");
    mock("mock2").actualCall("foo2");

    mock("mock1").checkExpectations();
    mock("mock2").checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, strictOrderViolatedAcrossScopes)
{
    mock("mock1").strictOrder();
    mock("mock2").strictOrder();
    mock("mock1").expectOneCall("foo1");
    mock("mock2").expectOneCall("foo2");

    mock("mock2").actualCall("foo2");
    mock("mock1").actualCall("foo1");

    mock("mock1").checkExpectations();
    mock("mock2").checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, usingNCalls)
{
    mock().strictOrder();
    mock().expectOneCall("foo1");
    mock().expectNCalls(2, "foo2");
    mock().expectOneCall("foo1");

    mock().actualCall("foo1");
    mock().actualCall("foo2");
    mock().actualCall("foo2");
    mock().actualCall("foo1");

    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOneCallHoweverMultipleHappened)
{
    addFunctionToExpectationsList("foo")->callWasMade(1);
    addFunctionToExpectationsList("foo")->callWasMade(2);
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);

    mock().expectOneCall("foo");
    mock().expectOneCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneUnsignedIntegerParameterAndValue)
{
    unsigned int value = 14400;
    mock().expectOneCall("foo").withParameter("parameter", value);
    mock().actualCall("foo").withParameter("parameter", value);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOneIntegerParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", 10);
    mock().actualCall("foo").withParameter("parameter", 10);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, mismatchedIntegerTypesIntAndLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (int)1);
    mock().actualCall("foo").withParameter("parameter", (long)1);

    mock().expectOneCall("foo").withParameter("parameter", (long)1);
    mock().actualCall("foo").withParameter("parameter", (int)1);

    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, mismatchedIntegerTypesIntAndUnsignedAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (int)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned)1);
    mock().actualCall("foo").withParameter("parameter", (int)1);

    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, mismatchedIntegerTypesIntAndUnsignedLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (int)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)1);
    mock().actualCall("foo").withParameter("parameter", (int)1);

    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, mismatchedIntegerTypesUnsignedAndLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (unsigned)1);
    mock().actualCall("foo").withParameter("parameter", (long)1);

    mock().expectOneCall("foo").withParameter("parameter", (long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned)1);

    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, mismatchedIntegerTypesUnsignedAndUnsignedLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (unsigned)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned)1);

    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, mismatchedIntegerTypesLongAndUnsignedLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)1);
    mock().actualCall("foo").withParameter("parameter", (long)1);

    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, longAndUnsignedLongWithSameBitRepresentationShouldNotBeTreatedAsEqual)
{
    MockNamedValue parameter("parameter");
    parameter.setValue((unsigned long)-1);
    addFunctionToExpectationsList("foo")->withParameter("parameter", (long)-1);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

    mock().expectOneCall("foo").withParameter("parameter", (long)-1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)-1);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, unsignedLongAndLongWithSameBitRepresentationShouldnotBeTreatedAsEqual)
{
    MockNamedValue parameter("parameter");
    parameter.setValue((long)-1);
    addFunctionToExpectationsList("foo")->withParameter("parameter", (unsigned long)-1);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)-1);
    mock().actualCall("foo").withParameter("parameter", (long)-1);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneDoubleParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", 1.0);
    mock().actualCall("foo").withParameter("parameter", 1.0);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOneStringParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", "string");
    mock().actualCall("foo").withParameter("parameter", "string");
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOnePointerParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", (void*) 0x01);
    mock().actualCall("foo").withParameter("parameter", (void*) 0x01);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOneConstPointerParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", (const void*) 0x01);
    mock().actualCall("foo").withParameter("parameter", (const void*) 0x01);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOneStringParameterAndValueFails)
{
    MockNamedValue parameter("parameter");
    parameter.setValue("different");
    addFunctionToExpectationsList("foo")->withParameter("parameter", "string");
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

    mock().expectOneCall("foo").withParameter("parameter", "string");
    mock().actualCall("foo").withParameter("parameter", "different");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneUnsignedIntegerParameterAndFailsDueToParameterName)
{
    unsigned int value = 7;
    MockNamedValue parameter("different");
    parameter.setValue(value);
    addFunctionToExpectationsList("foo")->withParameter("parameter", value);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

    mock().expectOneCall("foo").withParameter("parameter", value);
    mock().actualCall("foo").withParameter("different", value);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndFailsDueToParameterName)
{
    MockNamedValue parameter("different");
    parameter.setValue(10);
    addFunctionToExpectationsList("foo")->withParameter("parameter", 10);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

    mock().expectOneCall("foo").withParameter("parameter", 10);
    mock().actualCall("foo").withParameter("different", 10);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneUnsignedIntegerParameterAndFailsDueToValue)
{
    unsigned int actual_value = 8;
    unsigned int expected_value = actual_value + 1;
    MockNamedValue parameter("parameter");

    parameter.setValue(actual_value);
    addFunctionToExpectationsList("foo")->withParameter("parameter", expected_value);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

    mock().expectOneCall("foo").withParameter("parameter", expected_value);
    mock().actualCall("foo").withParameter("parameter", actual_value);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndFailsDueToValue)
{
    MockNamedValue parameter("parameter");
    parameter.setValue(8);
    addFunctionToExpectationsList("foo")->withParameter("parameter", 10);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

    mock().expectOneCall("foo").withParameter("parameter", 10);
    mock().actualCall("foo").withParameter("parameter", 8);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndFailsDueToTypes)
{
    MockNamedValue parameter("parameter");
    parameter.setValue("heh");
    addFunctionToExpectationsList("foo")->withParameter("parameter", 10);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

    mock().expectOneCall("foo").withParameter("parameter", 10);
    mock().actualCall("foo").withParameter("parameter", "heh");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectMultipleCallsWithDifferentParametersThatHappenOutOfOrder)
{
    mock().expectOneCall("foo").withParameter("p1", 1);
    mock().expectOneCall("foo").withParameter("p1", 2);
    mock().actualCall("foo").withParameter("p1", 2);
    mock().actualCall("foo").withParameter("p1", 1);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectMultipleCallsWithMultipleDifferentParametersThatHappenOutOfOrder)
{
    mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 20);

    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 20);
    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, twiceCalledWithSameParameters)
{
    mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, calledWithoutParameters)
{
    addFunctionToExpectationsList("foo")->withParameter("p1", 1);
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);

    mock().expectOneCall("foo").withParameter("p1", 1);
    mock().actualCall("foo");
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

}

TEST(MockSupportTest, ignoreOtherParameters)
{
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters();
    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, ignoreOtherParametersButStillPassAll)
{
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters();
    mock().actualCall("foo").withParameter("p1", 1);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, ignoreOtherParametersButExpectedParameterDidntHappen)
{
    addFunctionToExpectationsList("foo")->withParameter("p1", 1).ignoreOtherParameters();
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);

    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters();
    mock().actualCall("foo").withParameter("p2", 2).withParameter("p3", 3).withParameter("p4", 4);
    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, ignoreOtherParametersMultipleCalls)
{
    mock().expectOneCall("foo").ignoreOtherParameters();
    mock().expectOneCall("foo").ignoreOtherParameters();
    mock().actualCall("foo").withParameter("p2", 2).withParameter("p3", 3).withParameter("p4", 4);
    LONGS_EQUAL(1, mock().expectedCallsLeft());
    mock().actualCall("foo").withParameter("p2", 2).withParameter("p3", 3).withParameter("p4", 4);

    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, ignoreOtherParametersMultipleCallsButOneDidntHappen)
{
    MockCheckedExpectedCall* call = addFunctionToExpectationsList("boo");
    call->ignoreOtherParameters();
    call->callWasMade(1);
    call->parametersWereIgnored();
    call->ignoreOtherParameters();
    addFunctionToExpectationsList("boo")->ignoreOtherParameters();
    mock().expectOneCall("boo").ignoreOtherParameters();
    mock().expectOneCall("boo").ignoreOtherParameters();
    mock().actualCall("boo");
    mock().checkExpectations();
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), *expectationsList);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}


TEST(MockSupportTest, newCallStartsWhileNotAllParametersWerePassed)
{
    addFunctionToExpectationsList("foo")->withParameter("p1", 1);
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);

    mock().expectOneCall("foo").withParameter("p1", 1);
    mock().actualCall("foo");
    mock().actualCall("foo").withParameter("p1", 1);;

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, threeExpectedAndActual)
{
    mock().expectOneCall("function1");
    mock().expectOneCall("function2");
    mock().expectOneCall("function3");
    mock().actualCall("function1");
    mock().actualCall("function2");
    mock().actualCall("function3");
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

class MyTypeForTesting
{
public:
    MyTypeForTesting(int val) : value(val){}
    int value;
};

class MyTypeForTestingComparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(const void* object1, const void* object2)
    {
        return ((MyTypeForTesting*)object1)->value == ((MyTypeForTesting*)object2)->value;
    }
    virtual SimpleString valueToString(const void* object)
    {
        return StringFrom(((MyTypeForTesting*)object)->value);
    }
};


TEST(MockSupportTest, customObjectParameterFailsWhenNotHavingAComparisonRepository)
{
    MyTypeForTesting object(1);
    mock().expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    MockNoWayToCompareCustomTypeFailure expectedFailure(mockFailureTest(), "MyTypeForTesting");
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, customObjectParameterSucceeds)
{
    MyTypeForTesting object(1);
    MyTypeForTestingComparator comparator;
    mock().installComparator("MyTypeForTesting", comparator);
    mock().expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
    mock().removeAllComparators();
}

TEST(MockSupportTest, outputParameterSucceeds)
{
    int param = 1;
    int retval = 2;
    mock().expectOneCall("function").withOutputParameterReturning("parameterName", &retval, sizeof(retval));
    mock().actualCall("function").withOutputParameter("parameterName", &param);
    CHECK_EQUAL(param, 2);
    CHECK_EQUAL(retval, 2);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, noActualCallForOutputParameter)
{
    int output;
    mock().expectOneCall("foo").withOutputParameterReturning("output", &output, sizeof(output));

    addFunctionToExpectationsList("foo")->withOutputParameterReturning("output", &output, sizeof(output));
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), *expectationsList);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, unexpectedOutputParameter)
{
    int param;
    mock().expectOneCall("foo");
    mock().actualCall("foo").withOutputParameter("parameterName", &param);

    addFunctionToExpectationsList("foo")->callWasMade(1);
    MockNamedValue parameter("parameterName");
    parameter.setValue(&param);
    MockUnexpectedOutputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, outputParameterMissing)
{
    int output;
    mock().expectOneCall("foo").withOutputParameterReturning("output", &output, sizeof(output));
    mock().actualCall("foo");

    addFunctionToExpectationsList("foo")->withOutputParameterReturning("output", &output, sizeof(output));
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, twoOutputParameters)
{
    int param1 = 55;
    int retval1 = 1;
    int param2 = 77;
    int retval2 = 2;
    mock().expectOneCall("function").withOutputParameterReturning("parameterName", &retval1, sizeof(retval1)).withParameter("id", 1);
    mock().expectOneCall("function").withOutputParameterReturning("parameterName", &retval2, sizeof(retval2)).withParameter("id", 2);
    mock().actualCall("function").withOutputParameter("parameterName", &param1).withParameter("id", 1);
    mock().actualCall("function").withOutputParameter("parameterName", &param2).withParameter("id", 2);
    CHECK_EQUAL(retval1, param1);
    CHECK_EQUAL(retval2, param2);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, twoInterleavedOutputParameters)
{
    int param1 = 55;
    int retval1 = 1;
    int param2 = 77;
    int retval2 = 2;
    mock().expectOneCall("function").withOutputParameterReturning("parameterName", &retval1, sizeof(retval1)).withParameter("id", 1);
    mock().expectOneCall("function").withOutputParameterReturning("parameterName", &retval2, sizeof(retval2)).withParameter("id", 2);
    mock().actualCall("function").withOutputParameter("parameterName", &param2).withParameter("id", 2);
    mock().actualCall("function").withOutputParameter("parameterName", &param1).withParameter("id", 1);
    CHECK_EQUAL(retval1, param1);
    CHECK_EQUAL(retval2, param2);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, twoDifferentOutputParametersInSameFunctionCallSucceeds)
{
    int param1 = 1;
    int param2 = 1;
    int retval1 = 2;
    int retval2 = 3;
    mock().expectOneCall("foo")
        .withOutputParameterReturning("bar", &retval1, sizeof(retval1))
        .withOutputParameterReturning("foobar", &retval2, sizeof(retval2));
    mock().actualCall("foo")
        .withOutputParameter("bar", &param1)
        .withOutputParameter("foobar", &param2);
    CHECK_EQUAL(2, retval1);
    CHECK_EQUAL(2, param1);
    CHECK_EQUAL(3, retval2);
    CHECK_EQUAL(3, param2);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, outputAndIntParametersOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
{
    int param = 1;
    int retval = 2;
    mock().expectOneCall("foo").withOutputParameterReturning("bar", &retval, sizeof(retval));
    mock().expectOneCall("foo").withIntParameter("bar", 25);
    mock().actualCall("foo").withOutputParameter("bar", &param);
    mock().actualCall("foo").withIntParameter("bar", 25);
    CHECK_EQUAL(2, retval);
    CHECK_EQUAL(2, param);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, twoOutputParameterOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
{
    int param1 = 1;
    int param2 = 1;
    int retval1 = 2;
    int retval2 = 3;
    mock().expectOneCall("foo").withOutputParameterReturning("bar", &retval1, sizeof(retval1));
    mock().expectOneCall("foo").withOutputParameterReturning("bar", &retval2, sizeof(retval2));
    mock().actualCall("foo").withOutputParameter("bar", &param1);
    mock().actualCall("foo").withOutputParameter("bar", &param2);
    CHECK_EQUAL(2, retval1);
    CHECK_EQUAL(2, param1);
    CHECK_EQUAL(3, retval2);
    CHECK_EQUAL(3, param2);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, twoOutputParametersOfSameNameInDifferentFunctionsSucceeds)
{
    int param = 1;
    int retval = 2;
    mock().expectOneCall("foo1").withOutputParameterReturning("bar", &retval, sizeof(retval));
    mock().expectOneCall("foo2").withIntParameter("bar", 25);
    mock().actualCall("foo1").withOutputParameter("bar", &param);
    mock().actualCall("foo2").withIntParameter("bar", 25);
    MyTypeForTestingComparator comparator;
    CHECK_EQUAL(2, retval);
    CHECK_EQUAL(2, param);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, outputAndInputParameter)
{
    int return_value = 5;
    int returned_value = 7;
    mock().expectOneCall("foo").withParameter("bar", 10).withOutputParameterReturning("bar", &return_value, sizeof(return_value));
    mock().actualCall("foo").withParameter("bar", 10).withOutputParameter("bar", &returned_value);

    LONGS_EQUAL(5, returned_value);
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, outputParameterTraced)
{
    mock().tracing(true);

    int param = 1;
    mock().actualCall("someFunc").withOutputParameter("someParameter", &param);
    mock().checkExpectations();
    STRCMP_CONTAINS("Function name:someFunc someParameter:", mock().getTraceOutput());
}

TEST(MockSupportTest, outputParameterWithIgnoredParameters)
{
    int param = 1;
    int retval = 2;

    mock().expectOneCall("foo").withOutputParameterReturning("bar", &param, sizeof(param)).ignoreOtherParameters();
    mock().actualCall("foo").withOutputParameter("bar", &retval).withParameter("other", 1);

    LONGS_EQUAL(param, retval);
}

static bool myTypeIsEqual(const void* object1, const void* object2)
{
    return ((MyTypeForTesting*)object1)->value == ((MyTypeForTesting*)object2)->value;
}

static SimpleString myTypeValueToString(const void* object)
{
    return StringFrom(((MyTypeForTesting*)object)->value);
}

TEST(MockSupportTest, customObjectWithFunctionComparator)
{
    MyTypeForTesting object(1);
    MockFunctionComparator comparator(myTypeIsEqual, myTypeValueToString);
    mock().installComparator("MyTypeForTesting", comparator);
    mock().expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
    mock().removeAllComparators();
}

TEST(MockSupportTest, customObjectWithFunctionComparatorThatFailsCoversValueToString)
{
    MyTypeForTesting object(5);
    MockFunctionComparator comparator(myTypeIsEqual, myTypeValueToString);
    mock().installComparator("MyTypeForTesting", comparator);
    addFunctionToExpectationsList("function")->withParameterOfType("MyTypeForTesting", "parameterName", &object);
    MockExpectedCallsDidntHappenFailure failure(UtestShell::getCurrent(), *expectationsList);
    mock().expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE_LOCATION(failure, __FILE__, __LINE__);
}

TEST(MockSupportTest, disableEnable)
{
    mock().disable();
    mock().expectOneCall("function");
    mock().actualCall("differenFunction");
    CHECK(! mock().expectedCallsLeft());
    mock().enable();
    mock().expectOneCall("function");
    CHECK(mock().expectedCallsLeft());
    mock().actualCall("function");
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, setDataForUnsignedIntegerValues)
{
    unsigned int expected_data = 7;
    mock().setData("data", expected_data);
    LONGS_EQUAL(expected_data, mock().getData("data").getUnsignedIntValue());
}

TEST(MockSupportTest, setDataForIntegerValues)
{
    int expected_data = 10;
    mock().setData("data", expected_data);
    LONGS_EQUAL(expected_data, mock().getData("data").getIntValue());
}

TEST(MockSupportTest, hasDataBeenSet)
{
    CHECK(!mock().hasData("data"));
    mock().setData("data", 10);
    CHECK(mock().hasData("data"));
}

TEST(MockSupportTest, dataCanBeChanged)
{
    mock().setData("data", 10);
    mock().setData("data", 15);
    LONGS_EQUAL(15, mock().getData("data").getIntValue());
}

TEST(MockSupportTest, uninitializedData)
{
    LONGS_EQUAL(0, mock().getData("nonexisting").getIntValue());
    STRCMP_EQUAL("int", mock().getData("nonexisting").getType().asCharString());
}

TEST(MockSupportTest, setMultipleData)
{
    mock().setData("data", 1);
    mock().setData("data2", 10);
    LONGS_EQUAL(1, mock().getData("data").getIntValue());
    LONGS_EQUAL(10, mock().getData("data2").getIntValue());
}

TEST(MockSupportTest, setDataString)
{
    mock().setData("data", "string");
    STRCMP_EQUAL("string", mock().getData("data").getStringValue());
}

TEST(MockSupportTest, setDataDouble)
{
    mock().setData("data", 1.0);
    DOUBLES_EQUAL(1.0, mock().getData("data").getDoubleValue(), 0.05);
}

TEST(MockSupportTest, setDataPointer)
{
    void * ptr = (void*) 0x001;
    mock().setData("data", ptr);
    POINTERS_EQUAL(ptr, mock().getData("data").getPointerValue());
}

TEST(MockSupportTest, setConstDataPointer)
{
    const void * ptr = (const void*) 0x001;
    mock().setData("data", ptr);
    POINTERS_EQUAL(ptr, mock().getData("data").getConstPointerValue());
}

TEST(MockSupportTest, setDataObject)
{
    void * ptr = (void*) 0x001;
    mock().setDataObject("data", "type", ptr);
    POINTERS_EQUAL(ptr, mock().getData("data").getObjectPointer());
    STRCMP_EQUAL("type", mock().getData("data").getType().asCharString());
}

TEST(MockSupportTest, getMockSupportScope)
{
    MockSupport* mock1 = mock().getMockSupportScope("name");
    MockSupport* mock2 = mock().getMockSupportScope("differentName");
    CHECK(!mock().hasData("name"));
    CHECK(mock1 != mock2);
    POINTERS_EQUAL(mock1, mock().getMockSupportScope("name"));
    CHECK(mock1 != &mock());
}

TEST(MockSupportTest, usingTwoMockSupportsByName)
{
    mock("first").expectOneCall("boo");
    LONGS_EQUAL(0, mock("other").expectedCallsLeft());
    LONGS_EQUAL(1, mock("first").expectedCallsLeft());
    mock("first").clear();
}

TEST(MockSupportTest, EnableDisableWorkHierarchically)
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

TEST(MockSupportTest, EnableDisableWorkHierarchicallyWhenSupportIsDynamicallyCreated)
{
    mock().disable();
    mock("first").expectOneCall("boo");
    LONGS_EQUAL(0, mock("first").expectedCallsLeft());

    mock().enable();
    mock("second").expectOneCall("boo");
    LONGS_EQUAL(1, mock("second").expectedCallsLeft());

    mock().clear();
}

TEST(MockSupportTest, ExpectedCallsLeftWorksHierarchically)
{
    mock("first").expectOneCall("foobar");
    LONGS_EQUAL(1, mock().expectedCallsLeft());
    mock().clear();
}

TEST(MockSupportTest, checkExpectationsWorksHierarchically)
{
    mock("first").expectOneCall("foobar");
    mock("second").expectOneCall("helloworld");

    addFunctionToExpectationsList("foobar");
    addFunctionToExpectationsList("helloworld");
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), *expectationsList);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, ignoreOtherCallsWorksHierarchically)
{
    mock("first");
    mock().ignoreOtherCalls();
    mock("first").actualCall("boo");
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, ignoreOtherCallsWorksHierarchicallyWhenDynamicallyCreated)
{
    mock().ignoreOtherCalls();
    mock("first").actualCall("boo");
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, ignoreOtherCallsIgnoresWithAllKindsOfParameters)
{
     mock().ignoreOtherCalls();
     mock().actualCall("boo")
           .withParameter("bar", 1u)
           .withParameter("foo", 1l)
           .withParameter("hey", 1ul)
           .withParameter("duh", 1.0f)
           .withParameter("yoo", (const void*) 0)
           .withParameterOfType("hoo", "int", (const void*) 0)
           .withOutputParameter("gah", (void*) 0)
           ;
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, checkExpectationsWorksHierarchicallyForLastCallNotFinished)
{
    mock("first").expectOneCall("foobar").withParameter("boo", 1);
    mock("first").actualCall("foobar");

    addFunctionToExpectationsList("foobar")->withParameter("boo", 1);
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foobar", *expectationsList);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, reporterIsInheritedInHierarchicalMocks)
{
    mock("differentScope").actualCall("foobar");

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foobar", *expectationsList);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, installComparatorWorksHierarchicalOnBothExistingAndDynamicallyCreatedMockSupports)
{
    MyTypeForTesting object(1);
    MyTypeForTestingComparator comparator;

    mock("existing");
    mock().installComparator("MyTypeForTesting", comparator);
    mock("existing").expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("existing").actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("dynamic").expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("dynamic").actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
    mock().removeAllComparators();
}

TEST(MockSupportTest, installComparatorsWorksHierarchical)
{
    MyTypeForTesting object(1);
    MyTypeForTestingComparator comparator;
    MockNamedValueComparatorRepository repos;
    repos.installComparator("MyTypeForTesting", comparator);

    mock("existing");
    mock().installComparators(repos);
    mock("existing").expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("existing").actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
    mock().removeAllComparators();
}

TEST(MockSupportTest, removeComparatorsWorksHierachically)
{
    MyTypeForTesting object(1);
    MyTypeForTestingComparator comparator;

    mock("scope").installComparator("MyTypeForTesting", comparator);
    mock().removeAllComparators();
    mock("scope").expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("scope").actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    MockNoWayToCompareCustomTypeFailure expectedFailure(mockFailureTest(), "MyTypeForTesting");
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, hasReturnValue)
{
    CHECK(!mock().hasReturnValue());
    mock().expectOneCall("foo");
    CHECK(!mock().actualCall("foo").hasReturnValue());
    CHECK(!mock().hasReturnValue());

    mock().expectOneCall("foo2").andReturnValue(1);
    CHECK(mock().actualCall("foo2").hasReturnValue());
    CHECK(mock().hasReturnValue());
}

TEST(MockSupportTest, UnsignedIntegerReturnValue)
{
    unsigned int expected_value = 7;

    mock().expectOneCall("foo").andReturnValue(expected_value);

    MockActualCall& actual_call = mock().actualCall("foo");

    LONGS_EQUAL(expected_value, actual_call.returnValue().getUnsignedIntValue());
    LONGS_EQUAL(expected_value, actual_call.returnUnsignedIntValue());

    LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedIntValue());
    LONGS_EQUAL(expected_value, mock().unsignedIntReturnValue());
}

TEST(MockSupportTest, PositiveIntReturnValueCanBeRetrievedAsUnsignedInt)
{
    int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedIntValue());
}

TEST(MockSupportTest, IntReturnValueCanBeRetrievedAsLongInt)
{
    int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongIntValue());
}

TEST(MockSupportTest, UnsignedIntReturnValueCanBeRetrievedAsLongInt)
{
    unsigned int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongIntValue());
}

TEST(MockSupportTest, PositiveIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
    int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
}

TEST(MockSupportTest, PositiveLongIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
    long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
}

TEST(MockSupportTest, UnsignedIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
    unsigned int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
}

TEST(MockSupportTest, UnsignedIntegerReturnValueSetsDifferentValues)
{
    unsigned int expected_value = 1;
    unsigned int another_expected_value = 2;

    mock().expectOneCall("foo").andReturnValue(expected_value);
    mock().expectOneCall("foo").andReturnValue(another_expected_value);

    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedIntValue());
    LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedIntValue());
    LONGS_EQUAL(another_expected_value, mock().actualCall("foo").returnValue().getUnsignedIntValue());
    LONGS_EQUAL(another_expected_value, mock().returnValue().getUnsignedIntValue());
}

TEST(MockSupportTest, UnsignedIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
    unsigned int ret_value = 1;
    unsigned int another_ret_value = 2;

    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(ret_value);
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(another_ret_value);

    LONGS_EQUAL(ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getUnsignedIntValue());
    LONGS_EQUAL(ret_value, mock().returnValue().getUnsignedIntValue());
    LONGS_EQUAL(another_ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getUnsignedIntValue());
    LONGS_EQUAL(another_ret_value, mock().returnValue().getUnsignedIntValue());
}

TEST(MockSupportTest, WhenADoubleReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    double default_return_value = 10.7;
    double expected_return_value = default_return_value + 1.3;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);

    DOUBLES_EQUAL(expected_return_value, mock().actualCall("foo").returnDoubleValueOrDefault(default_return_value), 0.05);
    DOUBLES_EQUAL(expected_return_value, mock().returnDoubleValueOrDefault(default_return_value), 0.05);
}

TEST(MockSupportTest, WhenNoDoubleReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    double default_return_value = 7.7;
    mock().expectOneCall("foo");
    DOUBLES_EQUAL(default_return_value, mock().actualCall("foo").returnDoubleValueOrDefault(default_return_value), 0.05);
    DOUBLES_EQUAL(default_return_value, mock().returnDoubleValueOrDefault(default_return_value), 0.05);
}

TEST(MockSupportTest, WhenAUnsignedIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    unsigned int default_return_value = 10;
    unsigned int expected_return_value = default_return_value + 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnUnsignedIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnUnsignedIntValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenNoUnsignedIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    unsigned int default_return_value = 10;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnUnsignedIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnUnsignedIntValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenAUnsignedLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    unsigned long int default_return_value = 121487;
    unsigned long int expected_return_value = default_return_value + 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnUnsignedLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnUnsignedLongIntValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenNoUnsignedLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    unsigned long int default_return_value = 7710144;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnUnsignedLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnUnsignedLongIntValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenALongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    long int default_return_value = 748797;
    long int expected_return_value = default_return_value + 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnLongIntValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenNoLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    long int default_return_value = 123659;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnLongIntValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenAIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    int default_return_value = 777;
    int expected_return_value = default_return_value + 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnIntValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenNoIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    int default_return_value = 777;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnIntValueOrDefault(default_return_value));
}

TEST(MockSupportTest, IntegerReturnValue)
{
    int expected_value = 1;
    mock().expectOneCall("foo").andReturnValue(1);
    MockActualCall& actual_call = mock().actualCall("foo");

    LONGS_EQUAL(expected_value, actual_call.returnValue().getIntValue());
    LONGS_EQUAL(expected_value, actual_call.returnIntValue());

    LONGS_EQUAL(expected_value, mock().returnValue().getIntValue());
    LONGS_EQUAL(expected_value, mock().intReturnValue());
}

TEST(MockSupportTest, IntegerReturnValueSetsDifferentValues)
{
    int expected_value = 1;
    int another_expected_value = -1;

    mock().expectOneCall("foo").andReturnValue(expected_value);
    mock().expectOneCall("foo").andReturnValue(another_expected_value);

    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getIntValue());
    LONGS_EQUAL(expected_value, mock().returnValue().getIntValue());
    LONGS_EQUAL(another_expected_value, mock().actualCall("foo").returnValue().getIntValue());
    LONGS_EQUAL(another_expected_value, mock().returnValue().getIntValue());
}

TEST(MockSupportTest, IntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
    int ret_value = 1;
    int another_ret_value = -1;

    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(ret_value);
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(another_ret_value);

    LONGS_EQUAL(ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getIntValue());
    LONGS_EQUAL(ret_value, mock().returnValue().getIntValue());
    LONGS_EQUAL(another_ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getIntValue());
    LONGS_EQUAL(another_ret_value, mock().returnValue().getIntValue());
}

TEST(MockSupportTest, LongIntegerReturnValue)
{
    long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);

    MockActualCall& actual_call = mock().actualCall("foo");
    LONGS_EQUAL(expected_value, actual_call.returnValue().getLongIntValue());
    LONGS_EQUAL(expected_value, actual_call.returnLongIntValue());
    LONGS_EQUAL(expected_value, mock().returnValue().getLongIntValue());
    LONGS_EQUAL(expected_value, mock().longIntReturnValue());
}

TEST(MockSupportTest, LongIntegerReturnValueSetsDifferentValues)
{
    long int expected_value = 1;
    long int another_expected_value = 2;

    mock().expectOneCall("foo").andReturnValue(expected_value);
    mock().expectOneCall("foo").andReturnValue(another_expected_value);

    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongIntValue());
    LONGS_EQUAL(expected_value, mock().returnValue().getLongIntValue());
    LONGS_EQUAL(another_expected_value, mock().actualCall("foo").returnValue().getLongIntValue());
    LONGS_EQUAL(another_expected_value, mock().returnValue().getLongIntValue());
}

TEST(MockSupportTest, LongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
    long int ret_value = 1;
    long int another_ret_value = 2;

    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(ret_value);
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(another_ret_value);

    LONGS_EQUAL(ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getLongIntValue());
    LONGS_EQUAL(ret_value, mock().returnValue().getLongIntValue());
    LONGS_EQUAL(another_ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getLongIntValue());
    LONGS_EQUAL(another_ret_value, mock().returnValue().getLongIntValue());
}

TEST(MockSupportTest, UnsignedLongIntegerReturnValue)
{
    unsigned long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);

    MockActualCall& actual_call = mock().actualCall("foo");
    LONGS_EQUAL(expected_value, actual_call.returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(expected_value, actual_call.returnUnsignedLongIntValue());
    LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(expected_value, mock().unsignedLongIntReturnValue());
}

TEST(MockSupportTest, UnsignedLongIntegerReturnValueSetsDifferentValues)
{
    unsigned long int expected_value = 1;
    unsigned long int another_expected_value = 2;

    mock().expectOneCall("foo").andReturnValue(expected_value);
    mock().expectOneCall("foo").andReturnValue(another_expected_value);

    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(another_expected_value, mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(another_expected_value, mock().returnValue().getUnsignedLongIntValue());
}

TEST(MockSupportTest, UnsignedLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
    unsigned long int ret_value = 1;
    unsigned long int another_ret_value = 2;

    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(ret_value);
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(another_ret_value);

    LONGS_EQUAL(ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(ret_value, mock().returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(another_ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(another_ret_value, mock().returnValue().getUnsignedLongIntValue());
}

TEST(MockSupportTest, MatchingReturnValueOnWhileSignature)
{
    mock().expectOneCall("foo").withParameter("p1", 1).andReturnValue(1);
    mock().expectOneCall("foo").withParameter("p1", 2).andReturnValue(2);
    mock().expectOneCall("foo").withParameter("p1", 3).andReturnValue(3);
    mock().expectOneCall("foo").ignoreOtherParameters().andReturnValue(4);

    LONGS_EQUAL(3, mock().actualCall("foo").withParameter("p1", 3).returnValue().getIntValue());
    LONGS_EQUAL(4, mock().actualCall("foo").withParameter("p1", 4).returnValue().getIntValue());
    LONGS_EQUAL(1, mock().actualCall("foo").withParameter("p1", 1).returnValue().getIntValue());
    LONGS_EQUAL(2, mock().actualCall("foo").withParameter("p1", 2).returnValue().getIntValue());
}

TEST(MockSupportTest, WhenAStringReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    const char * default_return_value = "default";
    const char * expected_return_value = "expected";
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    STRCMP_EQUAL(expected_return_value, mock().actualCall("foo").returnStringValueOrDefault(default_return_value));
    STRCMP_EQUAL(expected_return_value, mock().returnStringValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenNoStringReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    const char * default_return_value = "default";
    mock().expectOneCall("foo");
    STRCMP_EQUAL(default_return_value, mock().actualCall("foo").returnStringValueOrDefault(default_return_value));
    STRCMP_EQUAL(default_return_value, mock().returnStringValueOrDefault(default_return_value));
}

TEST(MockSupportTest, StringReturnValue)
{
    mock().expectOneCall("foo").andReturnValue("hello world");
    MockActualCall& actual_call = mock().actualCall("foo");

    STRCMP_EQUAL("hello world", actual_call.returnValue().getStringValue());
    STRCMP_EQUAL("hello world", actual_call.returnStringValue());
    STRCMP_EQUAL("hello world", mock().stringReturnValue());
}

TEST(MockSupportTest, DoubleReturnValue)
{
    double expected_return_value = 7.8;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);

    MockActualCall& actual_call = mock().actualCall("foo");
    DOUBLES_EQUAL(expected_return_value, actual_call.returnValue().getDoubleValue(), 0.05);
    DOUBLES_EQUAL(expected_return_value, actual_call.returnDoubleValue(), 0.05);
    DOUBLES_EQUAL(expected_return_value, mock().doubleReturnValue(), 0.05);
}

TEST(MockSupportTest, WhenAConstPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    const void * default_return_value = (void*) 0x7778;
    const void * expected_return_value = (void*) 0x144010;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    POINTERS_EQUAL(expected_return_value, mock().actualCall("foo").returnConstPointerValueOrDefault(default_return_value));
    POINTERS_EQUAL(expected_return_value, mock().returnConstPointerValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenNoConstPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    const void * default_return_value = (void*) 0x11;
    mock().expectOneCall("foo");
    POINTERS_EQUAL(default_return_value, mock().actualCall("foo").returnConstPointerValueOrDefault(default_return_value));
    POINTERS_EQUAL(default_return_value, mock().returnConstPointerValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenAPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    void * default_return_value = (void*) 0x777;
    void * expected_return_value = (void*) 0x144000;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    POINTERS_EQUAL(expected_return_value, mock().actualCall("foo").returnPointerValueOrDefault(default_return_value));
    POINTERS_EQUAL(expected_return_value, mock().returnPointerValueOrDefault(default_return_value));
}

TEST(MockSupportTest, WhenNoPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    void * default_return_value = (void*) 0x10;
    mock().expectOneCall("foo");
    POINTERS_EQUAL(default_return_value, mock().actualCall("foo").returnPointerValueOrDefault(default_return_value));
    POINTERS_EQUAL(default_return_value, mock().returnPointerValueOrDefault(default_return_value));
}

TEST(MockSupportTest, PointerReturnValue)
{
    void* ptr = (void*) 0x00107;
    mock().expectOneCall("foo").andReturnValue(ptr);
    MockActualCall& actual_call = mock().actualCall("foo");

    POINTERS_EQUAL(ptr, actual_call.returnValue().getPointerValue());
    POINTERS_EQUAL(ptr, actual_call.returnPointerValue());
    POINTERS_EQUAL(ptr, mock().pointerReturnValue());
}

TEST(MockSupportTest, ConstPointerReturnValue)
{
    const void* ptr = (const void*) 0x001074;
    mock().expectOneCall("foo").andReturnValue(ptr);
    MockActualCall& actual_call = mock().actualCall("foo");
    POINTERS_EQUAL(ptr, actual_call.returnValue().getConstPointerValue());
    POINTERS_EQUAL(ptr, actual_call.returnConstPointerValue());
    POINTERS_EQUAL(ptr, mock().constPointerReturnValue());
}

TEST(MockSupportTest, OnObject)
{
    void* objectPtr = (void*) 0x001;
    mock().expectOneCall("boo").onObject(objectPtr);
    mock().actualCall("boo").onObject(objectPtr);
}

TEST(MockSupportTest, OnObjectFails)
{
    void* objectPtr = (void*) 0x001;
    void* objectPtr2 = (void*) 0x002;
    addFunctionToExpectationsList("boo")->onObject(objectPtr);

    mock().expectOneCall("boo").onObject(objectPtr);
    mock().actualCall("boo").onObject(objectPtr2);

    MockUnexpectedObjectFailure expectedFailure(mockFailureTest(), "boo", objectPtr2, *expectationsList);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, OnObjectExpectedButNotCalled)
{
    void* objectPtr = (void*) 0x001;
    addFunctionToExpectationsList("boo")->onObject(objectPtr);
    addFunctionToExpectationsList("boo")->onObject(objectPtr);

    mock().expectOneCall("boo").onObject(objectPtr);
    mock().expectOneCall("boo").onObject(objectPtr);
    mock().actualCall("boo");
    mock().actualCall("boo");

    MockExpectedObjectDidntHappenFailure expectedFailure(mockFailureTest(), "boo", *expectationsList);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectMultipleCalls)
{
    mock().expectNCalls(2, "boo");
    mock().actualCall("boo");
    mock().actualCall("boo");
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectMultipleCallsWithParameters)
{
    int expected_int = -7;
    unsigned int expected_uint = 7;

    mock().expectNCalls(2, "boo").withParameter("double", 1.0).withParameter("int", expected_int).
        withParameter("string", "string").withParameter("uint", expected_uint);
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", expected_int).withParameter("string", "string").
        withParameter("uint", expected_uint);
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", expected_int).withParameter("string", "string").
        withParameter("uint", expected_uint);
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectMultipleMultipleCallsWithParameters)
{
    mock().expectNCalls(2, "boo").withParameter("double", 1.0).ignoreOtherParameters();
    mock().expectNCalls(2, "boo").withParameter("double", 1.0).ignoreOtherParameters();
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, whenCallingDisabledOrIgnoredActualCallsThenTheyDontReturnPreviousCallsValues)
{
    mock().expectOneCall("boo").ignoreOtherParameters().andReturnValue(10);
    mock().ignoreOtherCalls();
    mock().actualCall("boo");
    mock().actualCall("An Ignored Call");
    CHECK(!mock().hasReturnValue());
}

TEST(MockSupportTest, tracing)
{
    mock().tracing(true);

    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock("scope").actualCall("foo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock().checkExpectations();

    STRCMP_CONTAINS("boo", mock().getTraceOutput());
    STRCMP_CONTAINS("foo", mock().getTraceOutput());
}

TEST(MockSupportTest, tracingWorksHierarchically)
{
    mock("scope").tracing(true);
    mock().tracing(true);

    mock().actualCall("boo");
    mock("scope").actualCall("foo");
    mock().checkExpectations();

    STRCMP_CONTAINS("boo", mock().getTraceOutput());
    STRCMP_CONTAINS("foo", mock().getTraceOutput());
}

TEST(MockSupportTest, shouldntFailTwice)
{
       mock().expectOneCall("foo");
       mock().actualCall("bar");
       mock().checkExpectations();
       CHECK(!MockFailureReporterForTest::getReporter()->mockFailureString.contains("bar"));
       CLEAR_MOCK_FAILURE();
}

class StubComparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(const void*, const void*)
    {
        return true;
    }
    virtual SimpleString valueToString(const void*)
    {
        return "";
    }
};

class SomeClass
{
    int someDummy_;
};

static void functionWithConstParam(const SomeClass param)
{
    mock().actualCall("functionWithConstParam").withParameterOfType("SomeClass", "param", &param);
}

TEST(MockSupportTest, shouldSupportConstParameters)
{
    StubComparator comparator;
    mock().installComparator("SomeClass", comparator);

    SomeClass param;
    mock().expectOneCall("functionWithConstParam").withParameterOfType("SomeClass", "param", &param);
    functionWithConstParam(param);

    mock().checkExpectations();
}

TEST(MockSupportTest, shouldReturnDefaultWhenThereIsntAnythingToReturn)
{
    CHECK(mock().returnValue().equals(MockNamedValue("")));
}

IGNORE_TEST(MockSupportTest, testForPerformanceProfiling)
{
    /* TO fix! */
    mock().expectNCalls(1000, "SimpleFunction");
    for (int i = 0; i < 1000; i++) {
        mock().actualCall("SimpleFunction");
    }

}

TEST_GROUP(MockSupportTestWithFixture)
{
    TestTestingFixture fixture;
};

static void mocksAreCountedAsChecksTestFunction_()
{
    mock().expectOneCall("foo");
    mock().expectNCalls(3, "bar");
    mock().clear();
}

TEST(MockSupportTestWithFixture, mockExpectationShouldIncreaseNumberOfChecks)
{
    fixture.setTestFunction(mocksAreCountedAsChecksTestFunction_);
    fixture.runAllTests();
    LONGS_EQUAL(4, fixture.getCheckCount());
}

static void CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failedTestMethod_()
{
    MockExpectedCallsList list;
    MockUnexpectedCallHappenedFailure expectedFailure(UtestShell::getCurrent(), "unexpected", list);
    mock().actualCall("boo");
    CHECK_EXPECTED_MOCK_FAILURE_LOCATION(expectedFailure, "file", 1);
}

TEST(MockSupportTestWithFixture, CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failed)
{
    mock().setMockFailureStandardReporter(MockFailureReporterForTest::getReporter());
    fixture.setTestFunction(CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failedTestMethod_);
    fixture.runAllTests();
    fixture.assertPrintContains("MockFailures are different.");
    fixture.assertPrintContains("Expected MockFailure:");
    fixture.assertPrintContains("Mock Failure: Unexpected call to function: unexpected");
    fixture.assertPrintContains("Actual MockFailure:");
    fixture.assertPrintContains("Mock Failure: Unexpected call to function: boo");
}

static void CHECK_NO_MOCK_FAILURE_LOCATION_failedTestMethod_()
{
    mock().actualCall("boo");    
    CHECK_NO_MOCK_FAILURE_LOCATION("file", 1);
}

TEST(MockSupportTestWithFixture, CHECK_NO_MOCK_FAILURE_LOCATION_failed)
{
    mock().setMockFailureStandardReporter(MockFailureReporterForTest::getReporter());
    fixture.setTestFunction(CHECK_NO_MOCK_FAILURE_LOCATION_failedTestMethod_);
    fixture.runAllTests();
    fixture.assertPrintContains("Unexpected mock failure:");
    fixture.assertPrintContains("Mock Failure: Unexpected call to function: boo");
}

static bool cpputestHasCrashed;

static void crashMethod()
{
    cpputestHasCrashed = true;
}

static void crashOnFailureTestFunction_(void)
{
    mock().actualCall("unexpected");
} // LCOV_EXCL_LINE

#include "CppUTestExt/OrderedTest.h"

TEST_ORDERED(MockSupportTestWithFixture, shouldCrashOnFailure, 10)
{
    mock().crashOnFailure(true);
    UtestShell::setCrashMethod(crashMethod);
    fixture.setTestFunction(crashOnFailureTestFunction_);
    
    fixture.runAllTests();
    
    CHECK(cpputestHasCrashed);
    
    mock().crashOnFailure(false);
    UtestShell::resetCrashMethod();
}

TEST_ORDERED(MockSupportTestWithFixture, nextTestShouldNotCrashOnFailure, 11)
{
    cpputestHasCrashed = false;
    UtestShell::setCrashMethod(crashMethod);
    fixture.setTestFunction(crashOnFailureTestFunction_);
    
    fixture.runAllTests();
    
    fixture.assertPrintContains("Unexpected call to function: unexpected");
    CHECK_FALSE(cpputestHasCrashed);
    
    UtestShell::resetCrashMethod();
}
