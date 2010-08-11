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
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"
#include "CppUTestExt/MockFailure.h"
#include "TestMockFailure.h"

TEST_GROUP(MockSupportTest)
{
	MockSupport mock;

	MockExpectedFunctionsList *expectationsList;

	void setup()
	{
		mock.setMockFailureReporter(MockFailureReporterForTest::getReporter());
		expectationsList = new MockExpectedFunctionsList;
	}

	void teardown()
	{
		mock.checkExpectations();
		CHECK_NO_MOCK_FAILURE();
		expectationsList->deleteAllExpectationsAndClearList();
		delete expectationsList;
	}

	MockExpectedFunctionCall* addFunctionToExpectationsList(const SimpleString& name)
	{
		MockExpectedFunctionCall* newCall = new MockExpectedFunctionCall;
		newCall->withName(name);
		expectationsList->addExpectedCall(newCall);
		return newCall;
	}
};

TEST(MockSupportTest, clearExpectations)
{
	mock.expectOneCall("func");
	mock.clearExpectations();
	CHECK(! mock.expectedCallsLeft());
}

TEST(MockSupportTest, checkExpectationsDoesntFail)
{
	mock.checkExpectations();
}

TEST(MockSupportTest, checkExpectationsClearsTheExpectations)
{
	addFunctionToExpectationsList("foobar");
	MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), *expectationsList);

	mock.expectOneCall("foobar");
	mock.checkExpectations();

	CHECK(! mock.expectedCallsLeft());
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, exceptACallThatHappens)
{
	mock.expectOneCall("func");
	mock.actualCall("func");
	CHECK(! mock.expectedCallsLeft());
}

TEST(MockSupportTest, exceptACallInceasesExpectedCallsLeft)
{
	mock.expectOneCall("func");
	CHECK(mock.expectedCallsLeft());
	mock.clearExpectations();
}

TEST(MockSupportTest, unexpectedCallHappened)
{
	MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "func", *expectationsList);

	mock.actualCall("func");

	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, ignoreOtherCallsExceptForTheExpectedOne)
{
	mock.expectOneCall("foo");
	mock.ignoreOtherCalls();
	mock.actualCall("bar").withParameter("foo", 1);;

	CHECK_NO_MOCK_FAILURE();

	mock.clearExpectations();
}

TEST(MockSupportTest, ignoreOtherCallsDoesntIgnoreMultipleCallsOfTheSameFunction)
{
	mock.expectOneCall("foo");
	mock.ignoreOtherCalls();
	mock.actualCall("bar");
	mock.actualCall("foo");
	mock.actualCall("foo");

	addFunctionToExpectationsList("foo")->callWasMade();
	MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, ignoreOtherStillFailsIfExpectedOneDidntHappen)
{
	mock.expectOneCall("foo");
	mock.ignoreOtherCalls();
	mock.checkExpectations();

	addFunctionToExpectationsList("foo");

	MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), *expectationsList);
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectMultipleCallsThatHappen)
{
	mock.expectOneCall("foo");
	mock.expectOneCall("foo");
	mock.actualCall("foo");
	mock.actualCall("foo");
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOneCallHoweverMultipleHappened)
{
	addFunctionToExpectationsList("foo")->callWasMade();
	addFunctionToExpectationsList("foo")->callWasMade();
	MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);

	mock.expectOneCall("foo");
	mock.expectOneCall("foo");
	mock.actualCall("foo");
	mock.actualCall("foo");
	mock.actualCall("foo");

	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndValue)
{
	mock.expectOneCall("foo").withParameter("parameter", 10);
	mock.actualCall("foo").withParameter("parameter", 10);
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOneDoubleParameterAndValue)
{
	mock.expectOneCall("foo").withParameter("parameter", 1.0);
	mock.actualCall("foo").withParameter("parameter", 1.0);
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOneStringParameterAndValue)
{
	mock.expectOneCall("foo").withParameter("parameter", "string");
	mock.actualCall("foo").withParameter("parameter", "string");
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOnePointerParameterAndValue)
{
	mock.expectOneCall("foo").withParameter("parameter", (void*) 0x01);
	mock.actualCall("foo").withParameter("parameter", (void*) 0x01);
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectOneStringParameterAndValueFails)
{
	MockNamedValue parameter("parameter");
	parameter.setValue("different");
	addFunctionToExpectationsList("foo")->withParameter("parameter", "string");
	MockUnexpectedParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

	mock.expectOneCall("foo").withParameter("parameter", "string");
	mock.actualCall("foo").withParameter("parameter", "different");

	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndFailsDueToParameterName)
{
	MockNamedValue parameter("different");
	parameter.setValue(10);
	addFunctionToExpectationsList("foo")->withParameter("parameter", 10);
	MockUnexpectedParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

	mock.expectOneCall("foo").withParameter("parameter", 10);
	mock.actualCall("foo").withParameter("different", 10);

	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndFailsDueToValue)
{
	MockNamedValue parameter("parameter");
	parameter.setValue(8);
	addFunctionToExpectationsList("foo")->withParameter("parameter", 10);
	MockUnexpectedParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

	mock.expectOneCall("foo").withParameter("parameter", 10);
	mock.actualCall("foo").withParameter("parameter", 8);

	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndFailsDueToTypes)
{
	MockNamedValue parameter("parameter");
	parameter.setValue("heh");
	addFunctionToExpectationsList("foo")->withParameter("parameter", 10);
	MockUnexpectedParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, *expectationsList);

	mock.expectOneCall("foo").withParameter("parameter", 10);
	mock.actualCall("foo").withParameter("parameter", "heh");

	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectMultipleCallsWithDifferentParametersThatHappenOutOfOrder)
{
	mock.expectOneCall("foo").withParameter("p1", 1);
	mock.expectOneCall("foo").withParameter("p1", 2);
	mock.actualCall("foo").withParameter("p1", 2);
	mock.actualCall("foo").withParameter("p1", 1);
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, expectMultipleCallsWithMultipleDifferentParametersThatHappenOutOfOrder)
{
	mock.expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 20);

	mock.actualCall("foo").withParameter("p1", 1).withParameter("p2", 20);
	mock.actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, twiceCalledWithSameParameters)
{
	mock.expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, calledWithoutParameters)
{
	addFunctionToExpectationsList("foo")->withParameter("p1", 1);
	MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);

	mock.expectOneCall("foo").withParameter("p1", 1);
	mock.actualCall("foo");
	mock.checkExpectations();

	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

}

TEST(MockSupportTest, newCallStartsWhileNotAllParametersWerePassed)
{
	addFunctionToExpectationsList("foo")->withParameter("p1", 1);
	MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", *expectationsList);

	mock.expectOneCall("foo").withParameter("p1", 1);
	mock.actualCall("foo");
	mock.actualCall("foo").withParameter("p1", 1);;

	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, threeExpectedAndActual)
{
	mock.expectOneCall("function1");
	mock.expectOneCall("function2");
	mock.expectOneCall("function3");
	mock.actualCall("function1");
	mock.actualCall("function2");
	mock.actualCall("function3");
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
}

class MyTypeForTesting
{
public:
	MyTypeForTesting(int val) : value(val){};
	int value;
};

class MyTypeForTestingComparator : public MockNamedValueComparator
{
public:
	virtual bool isEqual(void* object1, void* object2)
	{
		return ((MyTypeForTesting*)object1)->value == ((MyTypeForTesting*)object2)->value;
	}
	virtual SimpleString valueToString(void* object)
	{
		return StringFrom(((MyTypeForTesting*)object)->value);
	}
};


TEST(MockSupportTest, customObjectParameterFailsWhenNotHavingAComparisonRepository)
{
	MyTypeForTesting object(1);
	mock.expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
	mock.actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

	MockNoWayToCompareCustomTypeFailure expectedFailure(mockFailureTest(), "MyTypeForTesting");
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, customObjectParameterSucceeds)
{
	MyTypeForTesting object(1);
	MyTypeForTestingComparator comparator;
	mock.installComparator("MyTypeForTesting", comparator);
	mock.expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
	mock.actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
	mock.checkExpectations();
	CHECK_NO_MOCK_FAILURE();
	mock.removeAllComparators();
}

TEST(MockSupportTest, disableEnable)
{
	mock.disable();
	mock.expectOneCall("function");
	mock.actualCall("differenFunction");
	CHECK(! mock.expectedCallsLeft());
	mock.enable();
	mock.expectOneCall("function");
	CHECK(mock.expectedCallsLeft());
	mock.actualCall("function");
	CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportTest, setDataForIntegerValues)
{
	mock.setData("data", 10);
	LONGS_EQUAL(10, mock.getData("data").getIntValue());
}

TEST(MockSupportTest, hasDataBeenSet)
{
	CHECK(!mock.hasData("data"));
	mock.setData("data", 10);
	CHECK(mock.hasData("data"));
}

TEST(MockSupportTest, uninitializedData)
{
	LONGS_EQUAL(0, mock.getData("nonexisting").getIntValue());
	STRCMP_EQUAL("int", mock.getData("nonexisting").getType().asCharString());
}
#if 0
TEST(MockSupportTest, setMultipleData)
{
	mock.setData("data", 1);
	mock.setData("data2", 10);
	LONGS_EQUAL(1, mock.getData("data").getIntValue());
	LONGS_EQUAL(10, mock.getData("data2").getIntValue());
}

TEST(MockSupportTest, setDataString)
{
	mock.setData("data", "string");
	STRCMP_EQUAL("string", mock.getData("data").getStringValue());
}

TEST(MockSupportTest, setDataDouble)
{
	mock.setData("data", 1.0);
	DOUBLES_EQUAL(1.0, mock.getData("data").getDoubleValue(), 0.05);
}

TEST(MockSupportTest, setDataPointer)
{
	void * ptr = 0x001;
	mock.setData("data", ptr);
	POINTERS_EQUAL(ptr, mock.getData("data").getPointerValue());
}

TEST(MockSupportTest, setDataObject)
{
	void * ptr = 0x001;
	mock.setDataObject("data", "type", ptr);
	POINTERS_EQUAL(ptr, mock.getData("data").getObjectPointer());
	STRCMP_EQUAL("type", mock.getData("data").getType());
}
#endif
