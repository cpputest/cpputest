
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

#ifndef D_TestMockFailure_h
#define D_TestMockFailure_h

#define CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN(name) CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN_LOCATION(name, __FILE__, __LINE__)
#define CHECK_MOCK_FAILURE_EXPECTED_CALL_HAPPENED(name) CHECK_MOCK_FAILURE_EXPECTED_CALL_HAPPENED_LOCATION(name, __FILE__, __LINE__)
#define CHECK_MOCK_NO_FAILURE_LEFT() CHECK_MOCK_NO_FAILURE_LEFT_LOCATION(__FILE__, __LINE__)
#define CHECK_MOCK_NO_FAILURE() CHECK_MOCK_NO_FAILURE_LEFT_LOCATION(__FILE__, __LINE__)
#define CHECK_MOCK_FAILURE_UNEXPECTED_ADDITIONAL_CALL(totalExpectations, name) CHECK_MOCK_FAILURE_UNEXPECTED_ADDITIONAL_CALL_LOCATION(totalExpectations, name, __FILE__, __LINE__)
#define CHECK_MOCK_FAILURE_UNEXPECTED_PARAMETER_NAME(functionName, parameterName) CHECK_MOCK_FAILURE_UNEXPECTED_PARAMETER_NAME_LOCATION(functionName, parameterName, __FILE__, __LINE__)
#define CHECK_MOCK_FAILURE_UNEXPECTED_PARAMETER_VALUE(functionName, parameterName, actualValue) CHECK_MOCK_FAILURE_UNEXPECTED_PARAMETER_VALUE_LOCATION(functionName, parameterName, actualValue, __FILE__, __LINE__)
#define CHECK_MOCK_FAILURE_EXPECTED_PARAMETER_BUT_DID_NOT_HAPPEN(functionName, parameterName, value) CHECK_MOCK_FAILURE_EXPECTED_PARAMETER_BUT_DID_NOT_HAPPEN_LOCATION(functionName, parameterName, value, __FILE__, __LINE__)

class MockFailureReporterForTest : public MockFailureReporter
{
public:

	SimpleString mockFailureString;
	virtual void failTest(const MockFailure& failure)
	{
		mockFailureString = failure.getMessage();
	}
	static MockFailureReporterForTest* getReporter()
	{
		static MockFailureReporterForTest reporter;
		return &reporter;
	}
};

inline Utest* mockFailureTest()
{
	return MockFailureReporterForTest::getReporter()->getTestToFail();
}

inline SimpleString mockFailureString()
{
	return MockFailureReporterForTest::getReporter()->mockFailureString;
}

inline void MOCK_FAILURES_EQUAL_LOCATION(const SimpleString& expected, const SimpleString& actual, const char* file, int line)
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
	MockFailureReporterForTest::getReporter()->mockFailureString = "";
}

inline void CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN_LOCATION(const char* name, const char* file, int line)
{
	MockExpectedFunctionsList list;
	MockExpectedFunctionCall call;
	call.withName(name);
	list.addExpectedCall(&call);
	MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), list);
	MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), mockFailureString(), file, line);
}

inline void CHECK_MOCK_FAILURE_EXPECTED_CALL_HAPPENED_LOCATION(const char* name, const char* file, int line)
{
	MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), name);
	MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), mockFailureString(), file, line);
}

inline void CHECK_MOCK_NO_FAILURE_LEFT_LOCATION(const char* file, int line)
{
	MOCK_FAILURES_EQUAL_LOCATION("", mockFailureString(), file, line);
}

inline void CHECK_MOCK_FAILURE_UNEXPECTED_ADDITIONAL_CALL_LOCATION(int totalExpectations, const char* name, const char* file, int line)
{
	MockUnexpectedAdditionalCallFailure expectedFailure(mockFailureTest(), totalExpectations, name);
	MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), mockFailureString(), file, line);
}

inline void CHECK_MOCK_FAILURE_UNEXPECTED_PARAMETER_NAME_LOCATION(const char* functionName, const char* parameterName, const char* file, int line)
{
	MockUnexpectedParameterNameFailure expectedFailure(mockFailureTest(), functionName, parameterName);
	MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), mockFailureString(), file, line);
}

inline void CHECK_MOCK_FAILURE_UNEXPECTED_PARAMETER_VALUE_LOCATION(const char* functionName, const char* parameterName, const char* actualValue, const char* file, int line)
{
	MockUnexpectedParameterValueFailure expectedFailure(mockFailureTest(), functionName, parameterName, actualValue);
	MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), mockFailureString(), file, line);
}

inline void CHECK_MOCK_FAILURE_EXPECTED_PARAMETER_BUT_DID_NOT_HAPPEN_LOCATION(const char* functionName, const char* parameterName, const char* value, const char* file, int line)
{
	MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), functionName, parameterName, value);
	MOCK_FAILURES_EQUAL_LOCATION(expectedFailure.getMessage(), mockFailureString(), file, line);
}

#endif
