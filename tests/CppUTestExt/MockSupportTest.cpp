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
  MockExpectedCallsListForTest expectations;
  MockFailureReporterInstaller failureReporterInstaller;

  void teardown()
  {
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
  }
};

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
    expectations.addFunction("boo")->onObject(objectPtr);

    mock().expectOneCall("boo").onObject(objectPtr);
    mock().actualCall("boo").onObject(objectPtr2);

    MockUnexpectedObjectFailure expectedFailure(mockFailureTest(), "boo", objectPtr2, expectations);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, OnObjectExpectedButNotCalled)
{
    void* objectPtr = (void*) 0x001;
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
    mock().expectNCalls(2000, "SimpleFunction");
    for (int i = 0; i < 2000; i++) {
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

