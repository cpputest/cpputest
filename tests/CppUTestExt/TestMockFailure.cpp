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
#include "CppUTestExt/MockFailure.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"
#include "CppUTestExt/MockExpectedFunctionsList.h"


TEST_GROUP(MockFailureTest)
{
	MockFailureReporter reporter;

};

TEST(MockFailureTest, noErrorFailureSomethingGoneWrong)
{
	MockFailure failure(this);
	STRCMP_EQUAL("Test failed with MockFailure without an error! Something went seriously wrong.", failure.getMessage().asCharString());
}

TEST(MockFailureTest, unexpectedCallHappened)
{
	MockUnexpectedCallHappenedFailure failure(this, "foobar");
	STRCMP_EQUAL("MockFailure: Unexpected call to function: foobar. None expected but still happened.", failure.getMessage().asCharString());
}

IGNORE_TEST(MockFailureTest, expectedCallDidNotHappen)
{
	MockExpectedFunctionCall call;
	call.withName("foobar");
	call.withName("world")->withParameter("boo", 2)->withParameter("hello", "world");
	MockExpectedFunctionsList list;
	list.addExpectedCall(&call);

	MockExpectedCallsDidntHappenFailure failure(this, list);
	STRCMP_EQUAL("MockFailure: Excepted call did not happen.\n"
				 "\tEXPECTED calls that did NOT happen:\n"
				 "\t\tfoobar -> no parameters\n"
				 "\t\tworld -> int boo: <2>, char* hello: <world>\n"
				 "\tACTUAL calls that did happen:\n"
				 "\t\t<none>", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedAdditionalCallFailure)
{
	MockUnexpectedAdditionalCallFailure failure(this, 1, "bar");
	STRCMP_EQUAL("MockFailure: Expected 1 calls to \"bar\" but an additional (therefore unexpected) call happened.", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedParameterNameFailure)
{
	MockUnexpectedParameterNameFailure failure(this, "foo", "bar");
	STRCMP_EQUAL("MockFailure: Function \"foo\" was called with unexpected parameter with name: \"bar\".", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedParameterValueFailure)
{
	MockUnexpectedParameterValueFailure failure(this, "foo", "bar", "actual");
	STRCMP_EQUAL("MockFailure: Function \"foo\" was called with an unexpected parameter value for parameter: \"bar\"\n\tvalue was <actual>", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockExpectedParameterDidntHappenFailure)
{
	MockExpectedParameterDidntHappenFailure failure(this, "foo", "bar", "actual");
	STRCMP_EQUAL("MockFailure: Expected Function \"foo\" to be called with parameter: \"bar\" but it didn't happen.\n\tExpected value was <actual>", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockNoWayToCompareCustomTypeFailure)
{
	MockNoWayToCompareCustomTypeFailure failure(this, "myType");
	STRCMP_EQUAL("MockFailure: No way to compare type <myType>. Please install a ParameterTypeComparator.", failure.getMessage().asCharString());
}
