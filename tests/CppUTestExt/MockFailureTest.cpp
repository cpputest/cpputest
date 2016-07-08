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
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockExpectedCallsList.h"
#include "MockFailureReporterForTest.h"

TEST_GROUP(MockFailureTest)
{
    void teardown ()
    {
        CHECK_NO_MOCK_FAILURE();
    }

    void checkUnexpectedNthCallMessage(unsigned int count, const char* expectedOrdinal)
    {
        MockExpectedCallsListForTest expectedCalls;
        MockCheckedExpectedCall* expectedCall = expectedCalls.addFunction(count-1, count-1, "bar");

        MockActualCallsQueueForTest actualCalls;
        for (unsigned int i = 0; i < (count - 1); i++) {
            actualCalls.addCall(i, expectedCall);
        }

        MockUnexpectedCallHappenedFailure failure(UtestShell::getCurrent(), "bar", expectedCalls, actualCalls);

        SimpleString expectedMessage = StringFromFormat("Mock Failure: Unexpected additional (%s) call to function: bar\n\tEXPECTED", expectedOrdinal);
        STRCMP_CONTAINS(expectedMessage.asCharString(), failure.getMessage().asCharString());
    }
};

TEST(MockFailureTest, noErrorFailureSomethingGoneWrong)
{
    MockFailure failure(UtestShell::getCurrent());
    STRCMP_EQUAL("Test failed with MockFailure without an error! Something went seriously wrong.", failure.getMessage().asCharString());
}

TEST(MockFailureTest, unexpectedCallHappened)
{
    MockExpectedCallsListForTest expectedCalls;
    MockActualCallsQueueForTest actualCalls;
    MockUnexpectedCallHappenedFailure failure(UtestShell::getCurrent(), "foobar", expectedCalls, actualCalls);
    STRCMP_EQUAL("Mock Failure: Unexpected call to function: foobar\n"
                 "\tEXPECTED calls that WERE NOT fulfilled:\n"
                 "\t\t<none>\n"
                 "\tEXPECTED calls that WERE fulfilled:\n"
                 "\t\t<none>\n"
                 "\tACTUAL calls that were expected (in call order):\n"
                 "\t\t<none>", failure.getMessage().asCharString());
}

TEST(MockFailureTest, expectedCallDidNotHappen)
{
    MockExpectedCallsListForTest expectedCalls;
    expectedCalls.addFunction("foobar");
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("world");
    expectedCall2->withParameter("boo", 2).withParameter("hello", "world");
    MockCheckedExpectedCall* expectedCall3 = expectedCalls.addFunction("haphaphap");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall3);

    MockExpectedCallsNotFulfilledFailure failure(UtestShell::getCurrent(), expectedCalls, actualCalls);
    STRCMP_EQUAL("Mock Failure: Expected call WAS NOT fulfilled.\n"
                 "\tEXPECTED calls that WERE NOT fulfilled:\n"
                 "\t\tfoobar -> no parameters (expected 1 call, but was called 0 times)\n"
                 "\t\tworld -> int boo: <2 (0x2)>, const char* hello: <world> (expected 1 call, but was called 0 times)\n"
                 "\tEXPECTED calls that WERE fulfilled:\n"
                 "\t\thaphaphap -> no parameters (called 1 time)\n"
                 "\tACTUAL calls that were expected (in call order):\n"
                 "\t\t(1) haphaphap -> no parameters", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedNthAdditionalCallFailure)
{
    checkUnexpectedNthCallMessage(2, "2nd");
    checkUnexpectedNthCallMessage(3, "3rd");
    checkUnexpectedNthCallMessage(4, "4th");
    checkUnexpectedNthCallMessage(11, "11th");
    checkUnexpectedNthCallMessage(12, "12th");
    checkUnexpectedNthCallMessage(13, "13th");
    checkUnexpectedNthCallMessage(14, "14th");
    checkUnexpectedNthCallMessage(21, "21st");
    checkUnexpectedNthCallMessage(22, "22nd");
    checkUnexpectedNthCallMessage(23, "23rd");
}

TEST(MockFailureTest, MockUnexpectedInputParameterFailure)
{
    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction("foo");
    expectedCall1->withParameter("boo", 2);
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("foo");
    expectedCall2->withParameter("boo", 3.3);
    MockCheckedExpectedCall* expectedCall3 = expectedCalls.addFunction("unrelated");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall3);

    MockNamedValue actualParameter("bar");
    actualParameter.setValue(2);

    MockUnexpectedInputParameterFailure failure(UtestShell::getCurrent(), "foo", actualParameter, expectedCalls, actualCalls);
    STRCMP_EQUAL("Mock Failure: Unexpected parameter name to function \"foo\": bar\n"
                 "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
                 "\t\tfoo -> int boo: <2 (0x2)> (expected 1 call, but was called 0 times)\n"
                 "\t\tfoo -> double boo: <3.3> (expected 1 call, but was called 0 times)\n"
                 "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
                 "\t\t<none>\n"
                 "\tACTUAL calls that were expected (in call order) for function: foo\n"
                 "\t\t<none>\n"
                 "\tACTUAL unexpected parameter passed to function: foo\n"
                 "\t\tint bar: <2 (0x2)>", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedOutputParameterFailure)
{
    int out1;
    int out2;
    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction("foo");
    expectedCall1->withOutputParameterReturning("boo", &out1, sizeof(out1));
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("foo");
    expectedCall2->withOutputParameterReturning("boo", &out2, sizeof(out2));
    expectedCalls.addFunction("unrelated");

    MockActualCallsQueueForTest actualCalls;

    MockNamedValue actualParameter("bar");
    actualParameter.setValue((void *)0x123);

    MockUnexpectedOutputParameterFailure failure(UtestShell::getCurrent(), "foo", actualParameter, expectedCalls, actualCalls);
    STRCMP_EQUAL("Mock Failure: Unexpected output parameter name to function \"foo\": bar\n"
                 "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
                 "\t\tfoo -> const void* boo: <output> (expected 1 call, but was called 0 times)\n"
                 "\t\tfoo -> const void* boo: <output> (expected 1 call, but was called 0 times)\n"
                 "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
                 "\t\t<none>\n"
                 "\tACTUAL calls that were expected (in call order) for function: foo\n"
                 "\t\t<none>\n"
                 "\tACTUAL unexpected output parameter passed to function: foo\n"
                 "\t\tvoid* bar", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedParameterValueFailure)
{
    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction("foo");
    expectedCall1->withParameter("boo", 2);
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("foo");
    expectedCall2->withParameter("boo", 10);
    expectedCalls.addFunction("unrelated");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);

    MockNamedValue actualParameter("boo");
    actualParameter.setValue(20);

    MockUnexpectedInputParameterFailure failure(UtestShell::getCurrent(), "foo", actualParameter, expectedCalls, actualCalls);
    STRCMP_EQUAL("Mock Failure: Unexpected parameter value to parameter \"boo\" to function \"foo\": <20 (0x14)>\n"
                 "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
                 "\t\tfoo -> int boo: <10 (0xa)> (expected 1 call, but was called 0 times)\n"
                 "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
                 "\t\tfoo -> int boo: <2 (0x2)> (called 1 time)\n"
                 "\tACTUAL calls that were expected (in call order) for function: foo\n"
                 "\t\t(1) foo -> int boo: <2 (0x2)>\n"
                 "\tACTUAL unexpected parameter passed to function: foo\n"
                 "\t\tint boo: <20 (0x14)>", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockExpectedParameterDidntHappenFailure_AllParametersMissing)
{
    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction("foo");
    expectedCall1->withParameter("bar", 2).withParameter("boo", "str");
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("foo");
    expectedCall2->withParameter("bar", 10).withParameter("boo", "bleh");
    MockCheckedExpectedCall* expectedCall3 = expectedCalls.addFunction("unrelated");
    MockCheckedExpectedCall* expectedCall4 = expectedCalls.addFunction("foo");
    expectedCall4->withParameter("bar", 10).withParameter("boo", "blih");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall4);
    actualCalls.addCall(2, expectedCall3);
    actualCalls.addCall(3, expectedCall2);

    MockExpectedParameterDidntHappenFailure failure(UtestShell::getCurrent(), "foo", expectedCalls, actualCalls);
    STRCMP_EQUAL("Mock Failure: Expected parameter for function \"foo\" did not happen.\n"
                 "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
                 "\t\tfoo -> int bar: <2 (0x2)>, const char* boo: <str> (expected 1 call, but was called 0 times)\n"
                 "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
                 "\t\tfoo -> int bar: <10 (0xa)>, const char* boo: <bleh> (called 1 time)\n"
                 "\t\tfoo -> int bar: <10 (0xa)>, const char* boo: <blih> (called 1 time)\n"
                 "\tACTUAL calls that were expected (in call order) for function: foo\n"
                 "\t\t(1) foo -> int bar: <10 (0xa)>, const char* boo: <blih>\n"
                 "\t\t(3) foo -> int bar: <10 (0xa)>, const char* boo: <bleh>\n"
                 "\tMISSING parameters that didn't happen:\n"
                 "\t\tint bar, const char* boo", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockExpectedParameterDidntHappenFailure_SomeParametersMissing)
{
    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction("foo");
    expectedCall1->withParameter("bar", 2).withParameter("boo", "str1");
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("foo");
    expectedCall2->withParameter("bar", 10).withParameter("boo", "bleh");
    MockCheckedExpectedCall* expectedCall3 = expectedCalls.addFunction("unrelated");
    MockCheckedExpectedCall* expectedCall4 = expectedCalls.addFunction("foo");
    expectedCall4->withParameter("bar", 10).withParameter("boo", "blih");
    MockCheckedExpectedCall* expectedCall5 = expectedCalls.addFunction("foo");
    expectedCall5->withParameter("bar", 2).withParameter("baa", "str2");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall4);
    actualCalls.addCall(2, expectedCall3);
    actualCalls.addCall(3, expectedCall2);

    expectedCall1->inputParameterWasPassed("bar");
    expectedCall5->inputParameterWasPassed("bar");

    MockExpectedParameterDidntHappenFailure failure(UtestShell::getCurrent(), "foo", expectedCalls, actualCalls);
    STRCMP_EQUAL("Mock Failure: Expected parameter for function \"foo\" did not happen.\n"
                 "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
                 "\t\tfoo -> int bar: <2 (0x2)>, const char* boo: <str1> (expected 1 call, but was called 0 times)\n"
                 "\t\tfoo -> int bar: <2 (0x2)>, const char* baa: <str2> (expected 1 call, but was called 0 times)\n"
                 "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
                 "\t\tfoo -> int bar: <10 (0xa)>, const char* boo: <bleh> (called 1 time)\n"
                 "\t\tfoo -> int bar: <10 (0xa)>, const char* boo: <blih> (called 1 time)\n"
                 "\tACTUAL calls that were expected (in call order) for function: foo\n"
                 "\t\t(1) foo -> int bar: <10 (0xa)>, const char* boo: <blih>\n"
                 "\t\t(3) foo -> int bar: <10 (0xa)>, const char* boo: <bleh>\n"
                 "\tMISSING parameters that didn't happen:\n"
                 "\t\tconst char* boo\n"
                 "\t\tconst char* baa", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockExpectedParameterDidntHappenFailure_WithOptionalCalls)
{
    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction(2, 3, "foo");
    expectedCall1->withParameter("bar", 2).withParameter("boo", "str1");
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("foo");
    expectedCall2->withParameter("bar", 10).withParameter("boo", "bleh");
    MockCheckedExpectedCall* expectedCall3 = expectedCalls.addFunction("unrelated");
    MockCheckedExpectedCall* expectedCall4 = expectedCalls.addFunction("foo");
    expectedCall4->withParameter("bar", 10).withParameter("boo", "blih");
    MockCheckedExpectedCall* expectedCall5 = expectedCalls.addFunction("foo");
    expectedCall5->withParameter("bar", 2).withParameter("baa", "str2");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall1);
    actualCalls.addCall(2, expectedCall4);
    actualCalls.addCall(3, expectedCall3);
    actualCalls.addCall(4, expectedCall2);
    actualCalls.addCall(5, expectedCall1);

    expectedCall1->inputParameterWasPassed("bar");
    expectedCall5->inputParameterWasPassed("bar");

    MockExpectedParameterDidntHappenFailure failure(UtestShell::getCurrent(), "foo", expectedCalls, actualCalls);
    STRCMP_EQUAL("Mock Failure: Expected parameter for function \"foo\" did not happen.\n"
                 "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
                 "\t\tfoo -> int bar: <2 (0x2)>, const char* baa: <str2> (expected 1 call, but was called 0 times)\n"
                 "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
                 "\t\tfoo -> int bar: <2 (0x2)>, const char* boo: <str1> (called 2 times)\n"
                 "\t\tfoo -> int bar: <10 (0xa)>, const char* boo: <bleh> (called 1 time)\n"
                 "\t\tfoo -> int bar: <10 (0xa)>, const char* boo: <blih> (called 1 time)\n"
                 "\tACTUAL calls that were expected (in call order) for function: foo\n"
                 "\t\t(1) foo -> int bar: <2 (0x2)>, const char* boo: <str1>\n"
                 "\t\t(2) foo -> int bar: <10 (0xa)>, const char* boo: <blih>\n"
                 "\t\t(4) foo -> int bar: <10 (0xa)>, const char* boo: <bleh>\n"
                 "\t\t(5) foo -> int bar: <2 (0x2)>, const char* boo: <str1>\n"
                 "\tMISSING parameters that didn't happen:\n"
                 "\t\tconst char* boo\n"
                 "\t\tconst char* baa", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockNoWayToCompareCustomTypeFailure)
{
    MockNoWayToCompareCustomTypeFailure failure(UtestShell::getCurrent(), "myType");
    STRCMP_EQUAL("MockFailure: No way to compare type <myType>. Please install a MockNamedValueComparator.", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedObjectFailure)
{
    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction("foo");
    expectedCall1->onObject((void*) 0x02);
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("foo");
    expectedCall2->onObject((void*) 0x03);
    expectedCalls.addFunction("unrelated");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall2);

    MockUnexpectedObjectFailure failure(UtestShell::getCurrent(), "foo", (void*)0x1, expectedCalls, actualCalls);
    STRCMP_EQUAL(StringFromFormat (
                 "MockFailure: Function called on an unexpected object: foo\n"
                 "\tActual object for call has address: <%p>\n"
                 "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
                 "\t\t(object address: %p)::foo -> no parameters (expected 1 call, but was called 0 times)\n"
                 "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
                 "\t\t(object address: %p)::foo -> no parameters (called 1 time)\n"
                 "\tACTUAL calls that were expected (in call order) for function: foo\n"
                 "\t\t(1) (object address: %p)::foo -> no parameters",
                 (void*) 0x01, (void*) 0x02, (void*) 0x03, (void*) 0x03).asCharString(), failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockExpectedObjectDidntHappenFailure)
{
    MockExpectedCallsListForTest expectedCalls;
    MockCheckedExpectedCall* expectedCall1 = expectedCalls.addFunction("foo");
    expectedCall1->onObject((void*) 0x02);
    MockCheckedExpectedCall* expectedCall2 = expectedCalls.addFunction("foo");
    expectedCall2->onObject((void*) 0x03);
    expectedCalls.addFunction("unrelated");

    MockActualCallsQueueForTest actualCalls;
    actualCalls.addCall(1, expectedCall2);

    MockExpectedObjectDidntHappenFailure failure(UtestShell::getCurrent(), "foo", expectedCalls, actualCalls);
    STRCMP_EQUAL(StringFromFormat(
                 "Mock Failure: Expected call on object for function \"foo\" but it did not happen.\n"
                 "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
                 "\t\t(object address: %p)::foo -> no parameters (expected 1 call, but was called 0 times)\n"
                 "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
                 "\t\t(object address: %p)::foo -> no parameters (called 1 time)\n"
                 "\tACTUAL calls that were expected (in call order) for function: foo\n"
                 "\t\t(1) (object address: %p)::foo -> no parameters",
                 (void*) 0x2, (void*) 0x3, (void*) 0x3).asCharString(), failure.getMessage().asCharString());
}
