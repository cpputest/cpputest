
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
#include "MockFailureTest.h"

TEST_GROUP(MockParameterTest)
{
  void teardown()
  {
    mock().checkExpectations();
  }
};

TEST(MockParameterTest, expectOneUnsignedIntegerParameterAndValue)
{
    unsigned int value = 14400;
    mock().expectOneCall("foo").withParameter("parameter", value);
    mock().actualCall("foo").withParameter("parameter", value);

    mock().checkExpectations();
}

TEST(MockParameterTest, expectOneIntegerParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", 10);
    mock().actualCall("foo").withParameter("parameter", 10);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesIntAndLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (int)1);
    mock().actualCall("foo").withParameter("parameter", (long)1);

    mock().expectOneCall("foo").withParameter("parameter", (long)1);
    mock().actualCall("foo").withParameter("parameter", (int)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesIntAndUnsignedAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (int)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned)1);
    mock().actualCall("foo").withParameter("parameter", (int)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesIntAndUnsignedLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (int)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)1);
    mock().actualCall("foo").withParameter("parameter", (int)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesUnsignedAndLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (unsigned)1);
    mock().actualCall("foo").withParameter("parameter", (long)1);

    mock().expectOneCall("foo").withParameter("parameter", (long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesUnsignedAndUnsignedLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (unsigned)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesLongAndUnsignedLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)1);
    mock().actualCall("foo").withParameter("parameter", (long)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, longAndUnsignedLongWithSameBitRepresentationShouldNotBeTreatedAsEqual)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", (long)-1);
    MockNamedValue parameter("parameter");
    parameter.setValue((unsigned long)-1);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", (long)-1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)-1);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, unsignedLongAndLongWithSameBitRepresentationShouldnotBeTreatedAsEqual)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", (unsigned long)-1);
    MockNamedValue parameter("parameter");
    parameter.setValue((long)-1);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)-1);
    mock().actualCall("foo").withParameter("parameter", (long)-1);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, expectOneDoubleParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", 1.0);
    mock().actualCall("foo").withParameter("parameter", 1.0);

    mock().checkExpectations();
}

TEST(MockParameterTest, expectOneStringParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", "string");
    mock().actualCall("foo").withParameter("parameter", "string");

    mock().checkExpectations();
}

TEST(MockParameterTest, expectOnePointerParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", (void*) 0x01);
    mock().actualCall("foo").withParameter("parameter", (void*) 0x01);

    mock().checkExpectations();
}

TEST(MockParameterTest, expectOneConstPointerParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", (const void*) 0x01);
    mock().actualCall("foo").withParameter("parameter", (const void*) 0x01);

    mock().checkExpectations();
}

TEST(MockParameterTest, expectOneMemBufferParameterAndValue)
{
    unsigned char memBuffer1[] = { 0x12, 0x15, 0xFF };
    unsigned char memBuffer2[] = { 0x12, 0x15, 0xFF };
    mock().expectOneCall("foo").withParameter("parameter", memBuffer1, sizeof(memBuffer1));
    mock().actualCall("foo").withParameter("parameter", memBuffer2, sizeof(memBuffer2));

    mock().checkExpectations();
}

TEST(MockParameterTest, expectOneMemBufferParameterAndValueFailsDueToContents)
{
    MockFailureReporterInstaller failureReporterInstaller;

    unsigned char memBuffer1[] = { 0x12, 0x15, 0xFF };
    unsigned char memBuffer2[] = { 0x12, 0x05, 0xFF };
    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", memBuffer1, sizeof(memBuffer1));
    MockNamedValue parameter("parameter");
    parameter.setMemoryBuffer( memBuffer2, sizeof(memBuffer2) );
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", memBuffer1, sizeof(memBuffer1));
    mock().actualCall("foo").withParameter("parameter", memBuffer2, sizeof(memBuffer2));

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, expectOneMemBufferParameterAndValueFailsDueToSize)
{
    MockFailureReporterInstaller failureReporterInstaller;

    unsigned char memBuffer1[] = { 0x12, 0x15, 0xFF };
    unsigned char memBuffer2[] = { 0x12, 0x15, 0xFF, 0x90 };

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", memBuffer1, sizeof(memBuffer1));
    MockNamedValue parameter("parameter");
    parameter.setMemoryBuffer( memBuffer2, sizeof(memBuffer2) );
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", memBuffer1, sizeof(memBuffer1));
    mock().actualCall("foo").withParameter("parameter", memBuffer2, sizeof(memBuffer2));

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, expectOneStringParameterAndValueFails)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", "string");
    MockNamedValue parameter("parameter");
    parameter.setValue("different");
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", "string");
    mock().actualCall("foo").withParameter("parameter", "different");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, expectOneUnsignedIntegerParameterAndFailsDueToParameterName)
{
    MockFailureReporterInstaller failureReporterInstaller;

    unsigned int value = 7;
    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", value);
    MockNamedValue parameter("different");
    parameter.setValue(value);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", value);
    mock().actualCall("foo").withParameter("different", value);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, expectOneIntegerParameterAndFailsDueToParameterName)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", 10);
    MockNamedValue parameter("different");
    parameter.setValue(10);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", 10);
    mock().actualCall("foo").withParameter("different", 10);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, expectOneUnsignedIntegerParameterAndFailsDueToValue)
{
    MockFailureReporterInstaller failureReporterInstaller;

    unsigned int actual_value = 8;
    unsigned int expected_value = actual_value + 1;
    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", expected_value);
    MockNamedValue parameter("parameter");
    parameter.setValue(actual_value);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", expected_value);
    mock().actualCall("foo").withParameter("parameter", actual_value);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, expectOneIntegerParameterAndFailsDueToValue)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", 10);
    MockNamedValue parameter("parameter");
    parameter.setValue(8);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", 10);
    mock().actualCall("foo").withParameter("parameter", 8);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, expectOneIntegerParameterAndFailsDueToTypes)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", 10);
    MockNamedValue parameter("parameter");
    parameter.setValue("heh");
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withParameter("parameter", 10);
    mock().actualCall("foo").withParameter("parameter", "heh");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, expectMultipleCallsWithDifferentParametersThatHappenOutOfOrder)
{
    mock().expectOneCall("foo").withParameter("p1", 1);
    mock().expectOneCall("foo").withParameter("p1", 2);
    mock().actualCall("foo").withParameter("p1", 2);
    mock().actualCall("foo").withParameter("p1", 1);

    mock().checkExpectations();
}

TEST(MockParameterTest, expectMultipleCallsWithMultipleDifferentParametersThatHappenOutOfOrder)
{
    mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 20);

    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 20);
    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);

    mock().checkExpectations();
}

TEST(MockParameterTest, twiceCalledWithSameParameters)
{
    mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);

    mock().checkExpectations();
}

TEST(MockParameterTest, calledWithoutParameters)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("p1", 1);
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", expectations);

    mock().expectOneCall("foo").withParameter("p1", 1);
    mock().actualCall("foo");

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, ignoreOtherParameters)
{
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters();
    mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);

    mock().checkExpectations();
}

TEST(MockParameterTest, ignoreOtherParametersButStillPassAll)
{
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters();
    mock().actualCall("foo").withParameter("p1", 1);

    mock().checkExpectations();
}

TEST(MockParameterTest, ignoreOtherParametersButExpectedParameterDidntHappen)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("p1", 1).ignoreOtherParameters();
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", expectations);

    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters();
    mock().actualCall("foo").withParameter("p2", 2).withParameter("p3", 3).withParameter("p4", 4);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, ignoreOtherParametersMultipleCalls)
{
    mock().expectOneCall("foo").ignoreOtherParameters();
    mock().expectOneCall("foo").ignoreOtherParameters();
    mock().actualCall("foo").withParameter("p2", 2).withParameter("p3", 3).withParameter("p4", 4);
    LONGS_EQUAL(1, mock().expectedCallsLeft());
    mock().actualCall("foo").withParameter("p2", 2).withParameter("p3", 3).withParameter("p4", 4);

    mock().checkExpectations();
}

TEST(MockParameterTest, ignoreOtherParametersMultipleCallsButOneDidntHappen)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    MockCheckedExpectedCall* call = expectations.addFunction("boo");
    call->ignoreOtherParameters();
    call->callWasMade(1);
    call->parametersWereIgnored();
    call->ignoreOtherParameters();
    expectations.addFunction("boo")->ignoreOtherParameters();
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

    mock().expectOneCall("boo").ignoreOtherParameters();
    mock().expectOneCall("boo").ignoreOtherParameters();
    mock().actualCall("boo");

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}


TEST(MockParameterTest, newCallStartsWhileNotAllParametersWerePassed)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("p1", 1);
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", expectations);

    mock().expectOneCall("foo").withParameter("p1", 1);
    mock().actualCall("foo");
    mock().actualCall("foo").withParameter("p1", 1);;

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

