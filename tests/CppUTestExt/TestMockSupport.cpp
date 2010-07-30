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

#define CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN(name) CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN_LOCATION(name, __FILE__, __LINE__)
#define CHECK_MOCK_FAILURE_EXPECTED_CALL_HAPPENED(name) CHECK_MOCK_FAILURE_EXPECTED_CALL_HAPPENED_LOCATION(name, __FILE__, __LINE__)
#define CHECK_MOCK_NO_FAILURE() CHECK_MOCK_NO_FAILURE_LOCATION(__FILE__, __LINE__)
#define CHECK_MOCK_FAILURE_UNEXPECTED_ADDITIONAL_CALL(totalExpectations, name) CHECK_MOCK_FAILURE_UNEXPECTED_ADDITIONAL_CALL_LOCATION(totalExpectations, name, __FILE__, __LINE__)
#define CHECK_MOCK_FAILURE_DIFFERENT_PARAMETER_NAME_THAN_EXPECTED(functionName, expectedName, actualName) CHECK_MOCK_FAILURE_DIFFERENT_PARAMETER_NAME_THAN_EXPECTED_LOCATION(functionName, expectedName, actualName, __FILE__, __LINE__)

class MockFailureReporterForTest : public MockFailureReporter
{
public:
	SimpleString mockFailureString;
	virtual void failTest(const MockFailure& failure)
	{
		mockFailureString = failure.getMessage();
	}
};

TEST_GROUP(MockSupportTest)
{
	MockSupport mock;
	MockFailureReporterForTest reporter;

	void setup()
	{
		mock.setMockFailureReporter(&reporter);
	}

	void teardown()
	{
		mock.clearExpectations();
	}

	void MOCK_FAILURES_EQUAL_LOCATION(const SimpleString& expected, const SimpleString& actual, const char* file, int line)
	{
		if (actual != expected)
		{
			SimpleString error = "MockFailures are different.\n";
			error += "Expected MockFailure:\n\t";
			error += expected;
			error += "\nActual MockFailure:\n\t";
			error += actual;
			FAIL_LOCATION(error.asCharString(), file, line);
		}
	}

	void CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN_LOCATION(const char* name, const char* file, int line)
	{
		MockExpectedFunctionsList list;
		MockExpectedFunctionCall call;
		call.withName(name);
		list.addExpectedCall(&call);
		MockExpectedCallsDidntHappenFailure expectedFailure(this, list);
		MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), reporter.mockFailureString, file, line);
	}

	void CHECK_MOCK_FAILURE_EXPECTED_CALL_HAPPENED_LOCATION(const char* name, const char* file, int line)
	{
		MockUnexpectedCallHappenedFailure expectedFailure(this, name);
		MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), reporter.mockFailureString, file, line);
	}

	void CHECK_MOCK_NO_FAILURE_LOCATION(const char* file, int line)
	{
		MOCK_FAILURES_EQUAL_LOCATION("", reporter.mockFailureString, file, line);
	}

	void CHECK_MOCK_FAILURE_UNEXPECTED_ADDITIONAL_CALL_LOCATION(int totalExpectations, const char* name, const char* file, int line)
	{
		MockUnexpectedAdditionalCall expectedFailure(this, totalExpectations, name);
		MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), reporter.mockFailureString, file, line);
	}

#if 0
	void CHECK_MOCK_FAILURE_DIFFERENT_PARAMETER_NAME_THAN_EXPECTED_LOCATION(const char* functionName, const char* expectedName, const char* actualName, const char* file, int line)
	{
		MockFailure expectedFailure(this);
		expectedFailure.setFailureType(MOCK_UNEXPECTED_PARAMETER_NAME);
		expectedFailure.setFunctionName(functionName);
		expectedFailure.setExpectedParameterName(expectedName);
		expectedFailure.setActualParameterName(actualName);
		MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), mock.mockFailureString, file, line);
	}
#endif
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
	mock.expectOneCall("foobar");
	mock.checkExpectations();
	CHECK(! mock.expectedCallsLeft());
}

TEST(MockSupportTest, exceptACallThatHappens)
{
	mock.expectOneCall("func");
	mock.actualCall("func");
	CHECK(! mock.expectedCallsLeft());
}

TEST(MockSupportTest, exceptACallThatDidNotHappens)
{
	mock.expectOneCall("func");
	CHECK(mock.expectedCallsLeft());
	mock.checkExpectations();
	CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN("func");
}

TEST(MockSupportTest, unexpectedCallHappened)
{
	mock.actualCall("func");
	CHECK_MOCK_FAILURE_EXPECTED_CALL_HAPPENED("func");
}

TEST(MockSupportTest, ignoreOtherCallsExceptForTheExpectedOne)
{
	mock.expectOneCall("foo");
	mock.ignoreOtherCalls();
	mock.actualCall("bar");
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, ignoreOtherStillFailsIfExpectedOneDidntHappen)
{
	mock.expectOneCall("foo");
	mock.ignoreOtherCalls();
	mock.checkExpectations();
	CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN("foo");
}

TEST(MockSupportTest, expectOneCallHoweverMultipleHappened)
{
	mock.expectOneCall("foo");
	mock.expectOneCall("foo");
	mock.actualCall("foo");
	mock.actualCall("foo");
	mock.actualCall("foo");
	CHECK_MOCK_FAILURE_UNEXPECTED_ADDITIONAL_CALL(2, "foo");
}

#if 0

TEST(MockSupportTest, expectOneIntegerParameterOfValue)
{
	mock.expectOneCall("foo")->withParameter("parameter", 10);
	mock.actualCall("foo")->withParameter("parameter", 10);
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, expectOneIntegerParameterOfValueAndValuesDueToParameterName)
{
	mock.expectOneCall("foo")->withParameter("parameter", 10);
	mock.actualCall("foo")->withParameter("different", 10);
	CHECK_MOCK_FAILURE_DIFFERENT_PARAMETER_NAME_THAN_EXPECTED("foo", "parameter", "different");
}

TEST(MockSupportTest, usingWithWithoutAnyRecentExpectation)
{
}
#endif
