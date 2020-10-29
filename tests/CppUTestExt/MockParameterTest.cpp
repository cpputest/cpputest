
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

TEST_GROUP(MockParameterTest)
{
  void teardown() _override
  {
      mock().checkExpectations();
      mock().clear();
  }
};

TEST(MockParameterTest, expectOneBooleanParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", true);
    mock().actualCall("foo").withParameter("parameter", true);

    mock().checkExpectations();
}

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

#ifdef CPPUTEST_USE_LONG_LONG

TEST(MockParameterTest, expectOneUnsignedLongLongIntegerParameterAndValue)
{
    unsigned long long value = 0xFFFFAAAAFFFFAAAA;
    mock().expectOneCall("foo").withParameter("parameter", value);
    mock().actualCall("foo").withParameter("parameter", value);

    mock().checkExpectations();
}

TEST(MockParameterTest, expectOneLongLongIntegerParameterAndValue)
{
    long long value = 0x7FFFAAAAFFFFAAAA;

    mock().expectOneCall("foo").withParameter("parameter", value);
    mock().actualCall("foo").withParameter("parameter", value);

    mock().checkExpectations();
}

#endif

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

#ifdef CPPUTEST_USE_LONG_LONG

TEST(MockParameterTest, mismatchedIntegerTypesIntAndLongLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (int)1);
    mock().actualCall("foo").withParameter("parameter", (long long)1);

    mock().expectOneCall("foo").withParameter("parameter", (long long)1);
    mock().actualCall("foo").withParameter("parameter", (int)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesIntAndUnsignedLongLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (int)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long long)1);
    mock().actualCall("foo").withParameter("parameter", (int)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesUnsignedAndLongLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (unsigned)1);
    mock().actualCall("foo").withParameter("parameter", (long long)1);

    mock().expectOneCall("foo").withParameter("parameter", (long long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesUnsignedAndUnsignedLongLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (unsigned)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesUnsignedLongAndUnsignedLongLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesLongAndLongLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (long)1);
    mock().actualCall("foo").withParameter("parameter", (long long)1);

    mock().expectOneCall("foo").withParameter("parameter", (long long)1);
    mock().actualCall("foo").withParameter("parameter", (long)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesLongAndUnsignedLongLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long long)1);
    mock().actualCall("foo").withParameter("parameter", (long)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesUnsignedLongAndLongLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (unsigned long)1);
    mock().actualCall("foo").withParameter("parameter", (long long)1);

    mock().expectOneCall("foo").withParameter("parameter", (long long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long)1);

    mock().checkExpectations();
}

TEST(MockParameterTest, mismatchedIntegerTypesLongLongAndUnsignedLongLongAreAllowed)
{
    mock().expectOneCall("foo").withParameter("parameter", (long long)1);
    mock().actualCall("foo").withParameter("parameter", (unsigned long long)1);

    mock().expectOneCall("foo").withParameter("parameter", (unsigned long long)1);
    mock().actualCall("foo").withParameter("parameter", (long long)1);

    mock().checkExpectations();
}

#endif

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

TEST(MockParameterTest, expectOneDoubleParameterAndValueAndTolerance)
{
    mock( ).expectOneCall("foo").withParameter("parameter", 100.0, 5.0);
    mock( ).actualCall("foo").withParameter("parameter", 96.0);

    mock( ).checkExpectations();
}

TEST(MockParameterTest, doubleParameterNotEqualIfOutsideTolerance)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withParameter("parameter", 100.0);
    MockNamedValue parameter("parameter");
    parameter.setValue(106.0);
    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock( ).expectOneCall("foo").withParameter("parameter", 100.0, 5.0);
    mock( ).actualCall("foo").withParameter("parameter", 106.0);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
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

TEST(MockParameterTest, expectOneFunctionPointerParameterAndValue)
{
    mock().expectOneCall("foo").withParameter("parameter", (void(*)()) 0x01);
    mock().actualCall("foo").withParameter("parameter", (void(*)()) 0x01);

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
    call->finalizeActualCallMatch();
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
    mock().actualCall("foo").withParameter("p1", 1);

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, outputParameterSucceeds)
{
    int param = 1;
    int retval = 2;

    mock().expectOneCall("function").withOutputParameterReturning("parameterName", &retval, sizeof(retval));
    mock().actualCall("function").withOutputParameter("parameterName", &param);

    CHECK_EQUAL(param, 2);
    CHECK_EQUAL(retval, 2);
    mock().checkExpectations();
}

TEST(MockParameterTest, unmodifiedOutputParameterSucceeds)
{
    int param = 1;

    mock().expectOneCall("function").withUnmodifiedOutputParameter("parameterName");
    mock().actualCall("function").withOutputParameter("parameterName", &param);

    CHECK_EQUAL(param, 1);
    mock().checkExpectations();
}

TEST(MockParameterTest, noActualCallForOutputParameter)
{
    MockFailureReporterInstaller failureReporterInstaller;

    int output;
    MockExpectedCallsListForTest expectations;
    mock().expectOneCall("foo").withOutputParameterReturning("output", &output, sizeof(output));

    expectations.addFunction("foo")->withOutputParameterReturning("output", &output, sizeof(output));
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, noActualCallForUnmodifiedOutputParameter)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    mock().expectOneCall("foo").withUnmodifiedOutputParameter("output");

    expectations.addFunction("foo")->withUnmodifiedOutputParameter("output");
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, unexpectedOutputParameter)
{
    MockFailureReporterInstaller failureReporterInstaller;

    int param;
    MockExpectedCallsListForTest expectations;
    mock().expectOneCall("foo");
    mock().actualCall("foo").withOutputParameter("parameterName", &param);

    expectations.addFunction("foo");
    MockNamedValue parameter("parameterName");
    parameter.setValue(&param);
    MockUnexpectedOutputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, outputParameterMissing)
{
    MockFailureReporterInstaller failureReporterInstaller;

    int output;
    MockExpectedCallsListForTest expectations;
    mock().expectOneCall("foo").withOutputParameterReturning("output", &output, sizeof(output));
    mock().actualCall("foo");

    expectations.addFunction("foo")->withOutputParameterReturning("output", &output, sizeof(output));
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", expectations);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameterTest, twoOutputParameters)
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
}

TEST(MockParameterTest, twoInterleavedOutputParameters)
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
}

TEST(MockParameterTest, twoDifferentOutputParametersInSameFunctionCallSucceeds)
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
}

TEST(MockParameterTest, outputAndIntParametersOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
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
}

TEST(MockParameterTest, twoOutputParameterOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
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
}

TEST(MockParameterTest, twoOutputParametersOfSameNameInDifferentFunctionsSucceeds)
{
    int param = 1;
    int retval = 2;

    mock().expectOneCall("foo1").withOutputParameterReturning("bar", &retval, sizeof(retval));
    mock().expectOneCall("foo2").withIntParameter("bar", 25);
    mock().actualCall("foo1").withOutputParameter("bar", &param);
    mock().actualCall("foo2").withIntParameter("bar", 25);

    CHECK_EQUAL(2, retval);
    CHECK_EQUAL(2, param);
    mock().checkExpectations();
}

TEST(MockParameterTest, outputAndInputParameter)
{
    int return_value = 5;
    int returned_value = 7;

    mock().expectOneCall("foo").withParameter("bar", 10).withOutputParameterReturning("bar", &return_value, sizeof(return_value));
    mock().actualCall("foo").withParameter("bar", 10).withOutputParameter("bar", &returned_value);

    LONGS_EQUAL(5, returned_value);
    mock().checkExpectations();
}

TEST(MockParameterTest, outputParameterTraced)
{
    mock().tracing(true);

    int param = 1;
    mock().actualCall("someFunc").withOutputParameter("someParameter", &param);
    mock().checkExpectations();
    STRCMP_CONTAINS("Function name:someFunc someParameter:", mock().getTraceOutput());

    mock().checkExpectations();
}

TEST(MockParameterTest, outputParameterThatIsIgnoredShouldNotFail)
{
    int param;
    mock().expectOneCall("function").ignoreOtherParameters();
    mock().actualCall("function").withOutputParameter("parameterName", &param);

    mock().checkExpectations();
}

TEST(MockParameterTest, outputParameterWithIgnoredParameters)
{
    int param = 1;
    int retval = 2;

    mock().expectOneCall("foo").withOutputParameterReturning("bar", &param, sizeof(param)).ignoreOtherParameters();
    mock().actualCall("foo").withOutputParameter("bar", &retval).withParameter("other", 1);

    LONGS_EQUAL(param, retval);

    mock().checkExpectations();
}

/*
 * This test checks that the proper output parameters are copied when multiple calls to the same
 * function are expected.
 */
TEST(MockParameterTest, properOutputParametersAreCopied)
{
    int expectedValue1 = 1;
    int expectedValue2 = 2;
    mock().expectOneCall("foo").withOutputParameterReturning("param", &expectedValue1, sizeof(expectedValue1)).ignoreOtherParameters();
    mock().expectOneCall("foo").withOutputParameterReturning("param", &expectedValue2, sizeof(expectedValue2));

    int returnedValue1 = 0;
    int returnedValue2 = 0;
    mock().actualCall("foo").withOutputParameter("param", &returnedValue1);
    mock().actualCall("foo").withOutputParameter("param", &returnedValue2).withParameter("optional", 50);

    CHECK_EQUAL_TEXT(expectedValue2, returnedValue1, "Wrong output value in 1st call");
    CHECK_EQUAL_TEXT(expectedValue1, returnedValue2, "Wrong output value in 2nd call");

    mock().checkExpectations();
}

TEST(MockParameterTest, ignoreOtherCallsIgnoresWithAllKindsOfParameters)
{
     mock().ignoreOtherCalls();
     mock().actualCall("boo")
           .withParameter("umm", true)
           .withParameter("bar", 1u)
           .withParameter("foo", 1l)
           .withParameter("hey", 1ul)
#ifdef CPPUTEST_USE_LONG_LONG
           .withParameter("ick", 1ll)
           .withParameter("grr", 1ull)
#endif
           .withParameter("duh", 1.0)
           .withParameter("yoo", (const void*) NULLPTR)
           .withParameter("func", (void(*)()) NULLPTR)
           .withParameter("mem", (const unsigned char*) NULLPTR, 0)
           .withParameterOfType("hoo", "int", (const void*) NULLPTR)
           .withOutputParameter("gah", (void*) NULLPTR)
           .withOutputParameterOfType("goo", "int", (void*) NULLPTR);

    mock().checkExpectations();
}

TEST(MockParameterTest, expectMultipleCallsWithParameters)
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
}

TEST(MockParameterTest, expectMultipleMultipleCallsWithParameters)
{
    mock().expectNCalls(2, "boo").withParameter("double", 1.0).ignoreOtherParameters();
    mock().expectNCalls(2, "boo").withParameter("double", 1.0).ignoreOtherParameters();
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");

    mock().checkExpectations();
}
