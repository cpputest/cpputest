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
#include "CppUTestExt/MockCheckedActualCall.h"
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockExpectedCallsList.h"
#include "CppUTestExt/MockFailure.h"
#include "MockFailureReporterForTest.h"

TEST_GROUP(MockCheckedActualCall)
{
    MockExpectedCallsList* emptyList;
    MockExpectedCallsList* list;
    MockFailureReporter* reporter;

    void setup()
    {
        emptyList = new MockExpectedCallsList;
        list = new MockExpectedCallsList;
        reporter = MockFailureReporterForTest::getReporter();
    }

    void teardown()
    {
        CHECK_NO_MOCK_FAILURE();
        delete emptyList;
        delete list;
    }
};

TEST(MockCheckedActualCall, unExpectedCall)
{
    MockCheckedActualCall actualCall(1, reporter, *emptyList);
    actualCall.withName("unexpected");

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "unexpected", *list);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, unExpectedCallWithAParameter)
{
    MockCheckedActualCall actualCall(1, reporter, *emptyList);
    actualCall.withName("unexpected").withParameter("bar", 0);

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "unexpected", *list);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, unExpectedCallWithAnOutputParameter)
{
    MockCheckedActualCall actualCall(1, reporter, *emptyList);
    actualCall.withName("unexpected").withOutputParameter("bar", (void*)0);

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "unexpected", *list);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, unExpectedCallOnObject)
{
    int object;

    MockCheckedActualCall actualCall(1, reporter, *emptyList);
    actualCall.withName("unexpected").onObject(&object);

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "unexpected", *list);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

    CHECK(actualCall.hasFailed()); // Checks that onObject() doesn't "reset" call state
}

TEST(MockCheckedActualCall, actualCallWithNoReturnValueAndMeaninglessCallOrderForCoverage)
{
    MockCheckedActualCall actualCall(1, reporter, *emptyList);
    actualCall.withName("noreturn").withCallOrder(0).returnValue();

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "noreturn", *list);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, unExpectedParameterName)
{
    MockCheckedExpectedCall call1;
    call1.withName("func");
    list->addExpectedCall(&call1);

    MockCheckedActualCall actualCall(1, reporter, *list);
    actualCall.withName("func").withParameter("integer", 1);

    MockNamedValue parameter("integer");
    parameter.setValue(1);

    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "func", parameter, *list);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, multipleSameFunctionsExpectingAndHappenGradually)
{
    MockCheckedExpectedCall* call1 = new MockCheckedExpectedCall();
    MockCheckedExpectedCall* call2 = new MockCheckedExpectedCall();
    call1->withName("func");
    call2->withName("func");
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);

    LONGS_EQUAL(2, list->amountOfUnfulfilledExpectations());

    MockCheckedActualCall actualCall1(1, reporter, *list);
    actualCall1.withName("func");
    actualCall1.checkExpectations();

    LONGS_EQUAL(1, list->amountOfUnfulfilledExpectations());

    MockCheckedActualCall actualCall2(2, reporter, *list);
    actualCall2.withName("func");
    actualCall2.checkExpectations();

    LONGS_EQUAL(0, list->amountOfUnfulfilledExpectations());

    list->deleteAllExpectationsAndClearList();
}

TEST(MockCheckedActualCall, MockIgnoredActualCallWorksAsItShould)
{
    MockIgnoredActualCall actual;
    actual.withName("func");
    actual.withCallOrder(1);

    CHECK(false == actual.returnBoolValue());
    CHECK(true == actual.returnBoolValueOrDefault(true));
    CHECK(false == actual.returnBoolValueOrDefault(false));
    CHECK(0 == actual.returnUnsignedLongIntValue());
    CHECK(0 == actual.returnIntValue());
    CHECK(1ul == actual.returnUnsignedLongIntValueOrDefault(1ul));
    CHECK(1 == actual.returnIntValueOrDefault(1));
    CHECK(0 == actual.returnLongIntValue());
    CHECK(1l == actual.returnLongIntValueOrDefault(1l));
    CHECK(0 == actual.returnUnsignedIntValue());
    CHECK(1u == actual.returnUnsignedIntValueOrDefault(1u));
    DOUBLES_EQUAL(0.0, actual.returnDoubleValue(), 0.0);
    DOUBLES_EQUAL(1.5, actual.returnDoubleValueOrDefault(1.5), 0.0);
    STRCMP_EQUAL("bla", actual.returnStringValueOrDefault("bla"));
    STRCMP_EQUAL("", actual.returnStringValue());
    CHECK(0 == actual.returnPointerValue());
    CHECK((void*) 0x2 == actual.returnPointerValueOrDefault((void*) 0x2));
    CHECK(0 == actual.returnConstPointerValue());
    CHECK((const void*) 0x2 == actual.returnConstPointerValueOrDefault((const void*) 0x2));
    CHECK(0 == actual.returnFunctionPointerValue());
    CHECK((void(*)()) 1 == actual.returnFunctionPointerValueOrDefault((void(*)()) 0x1));
    CHECK_FALSE(actual.hasReturnValue());
    CHECK(actual.returnValue().equals(MockNamedValue("")));
}

TEST(MockCheckedActualCall, remainderOfMockActualCallTraceWorksAsItShould)
{
    int value;
    const int const_value = 1;
    const unsigned char mem_buffer[] = { 0xFE, 0x15 };
    void (*function_value)() = (void (*)())0xDEAD;
    MockActualCallTrace actual;
    actual.withName("func");
    actual.withCallOrder(1);
    actual.onObject(&value);

    actual.withBoolParameter("bool", true);
    actual.withUnsignedIntParameter("unsigned_int", (unsigned int) 1);
    actual.withUnsignedLongIntParameter("unsigned_long", (unsigned long)1);
    actual.withLongIntParameter("long_int", (long int) 1);
    actual.withPointerParameter("pointer", &value);
    actual.withConstPointerParameter("const_pointer", &const_value);
    actual.withFunctionPointerParameter("function_pointer", function_value);
    actual.withMemoryBufferParameter("mem_buffer", mem_buffer, sizeof(mem_buffer));
    actual.withParameterOfType("int", "named_type", &const_value);

    SimpleString expectedString("\nFunction name:func");
    expectedString += " withCallOrder:1";
    expectedString += " onObject:0x";
    expectedString += HexStringFrom(&value);
    expectedString += " bool:true";
    expectedString += " unsigned_int:1 (0x1)";
    expectedString += " unsigned_long:1 (0x1)";
    expectedString += " long_int:1 (0x1)";
    expectedString += " pointer:0x";
    expectedString += HexStringFrom(&value);
    expectedString += " const_pointer:0x";
    expectedString += HexStringFrom(&const_value);
    expectedString += " function_pointer:0x";
    expectedString += HexStringFrom(function_value);
    expectedString += " mem_buffer:Size = 2 | HexContents = FE 15";
    expectedString += " int named_type:0x";
    expectedString += HexStringFrom(&const_value);
    STRCMP_EQUAL(expectedString.asCharString(), actual.getTraceOutput());

    CHECK_FALSE(actual.hasReturnValue());
    CHECK(actual.returnValue().equals(MockNamedValue("")));
    CHECK(false == actual.returnBoolValue());
    CHECK(false == actual.returnBoolValueOrDefault(true));
    CHECK(0 == actual.returnLongIntValue());
    CHECK(0 == actual.returnUnsignedLongIntValue());
    CHECK(0 == actual.returnIntValue());
    CHECK(0 == actual.returnUnsignedLongIntValueOrDefault(1ul));
    CHECK(0 == actual.returnIntValueOrDefault(1));
    CHECK(0 == actual.returnLongIntValue());
    CHECK(0 == actual.returnLongIntValueOrDefault(1l));
    CHECK(0 == actual.returnUnsignedIntValue());
    CHECK(0 == actual.returnUnsignedIntValueOrDefault(1u));
    DOUBLES_EQUAL(0.0, actual.returnDoubleValue(), 0.0);
    DOUBLES_EQUAL(0.0, actual.returnDoubleValueOrDefault(1.0), 0.0);
    STRCMP_EQUAL("", actual.returnStringValueOrDefault("bla"));
    STRCMP_EQUAL("", actual.returnStringValue());
    CHECK(0 == actual.returnPointerValue());
    CHECK(0 == actual.returnPointerValueOrDefault((void*) 0x0));
    CHECK(0 == actual.returnConstPointerValue());
    CHECK(0 == actual.returnConstPointerValueOrDefault((const void*) 0x0));
    CHECK(0 == actual.returnFunctionPointerValue());
    CHECK(0 == actual.returnFunctionPointerValueOrDefault((void (*)()) 0x0));
}

