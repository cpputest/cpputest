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

TEST_GROUP(MockReturnValueTest)
{
  void teardown() _override
  {
      mock().checkExpectations();
      mock().clear();
  }
};

TEST(MockReturnValueTest, hasReturnValue)
{
    CHECK(!mock().hasReturnValue());
    mock().expectOneCall("foo");
    CHECK(!mock().actualCall("foo").hasReturnValue());
    CHECK(!mock().hasReturnValue());

    mock().expectOneCall("foo2").andReturnValue(1);
    CHECK(mock().actualCall("foo2").hasReturnValue());
    CHECK(mock().hasReturnValue());
}

TEST(MockReturnValueTest, UnsignedIntegerReturnValue)
{
    unsigned int expected_value = 7;

    mock().expectOneCall("foo").andReturnValue(expected_value);

    MockActualCall& actual_call = mock().actualCall("foo");

    LONGS_EQUAL(expected_value, actual_call.returnValue().getUnsignedIntValue());
    LONGS_EQUAL(expected_value, actual_call.returnUnsignedIntValue());

    LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedIntValue());
    LONGS_EQUAL(expected_value, mock().unsignedIntReturnValue());
}

TEST(MockReturnValueTest, PositiveIntReturnValueCanBeRetrievedAsUnsignedInt)
{
    int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedIntValue());
}

TEST(MockReturnValueTest, IntReturnValueCanBeRetrievedAsLongInt)
{
    int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongIntValue());
}

TEST(MockReturnValueTest, UnsignedIntReturnValueCanBeRetrievedAsLongInt)
{
    unsigned int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongIntValue());
}

TEST(MockReturnValueTest, PositiveIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
    int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
}

TEST(MockReturnValueTest, PositiveLongIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
    long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
}

TEST(MockReturnValueTest, UnsignedIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
    unsigned int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
}

#ifdef CPPUTEST_USE_LONG_LONG

TEST(MockReturnValueTest, PositiveIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
    int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValueTest, PositiveLongIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
    long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValueTest, PositiveLongLongIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
    long long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValueTest, UnsignedIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
    unsigned int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValueTest, UnsignedLongIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
    unsigned long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValueTest, UnsignedLongLongIntReturnValueCanBeRetrieved)
{
    unsigned long long int expected_value = ULLONG_MAX;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValueTest, PositiveIntReturnValueCanBeRetrievedAsLongLongInt)
{
    int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongLongIntValue());
}

TEST(MockReturnValueTest, PositiveLongIntReturnValueCanBeRetrievedAsLongLongInt)
{
    long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongLongIntValue());
}

TEST(MockReturnValueTest, UnsignedIntReturnValueCanBeRetrievedAsLongLongInt)
{
    unsigned int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongLongIntValue());
}

TEST(MockReturnValueTest, UnsignedLongIntReturnValueCanBeRetrievedAsLongLongInt)
{
    unsigned long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongLongIntValue());
}

TEST(MockReturnValueTest, LongLongIntReturnValueCanBeRetrieved)
{
    long long int expected_value = LLONG_MAX;
    mock().expectOneCall("foo").andReturnValue(expected_value);
    LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongLongIntValue());
}

#endif

TEST(MockReturnValueTest, UnsignedIntegerReturnValueSetsDifferentValues)
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

TEST(MockReturnValueTest, UnsignedIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
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

TEST(MockReturnValueTest, WhenADoubleReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    double default_return_value = 10.7;
    double expected_return_value = default_return_value + 1.3;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);

    DOUBLES_EQUAL(expected_return_value, mock().actualCall("foo").returnDoubleValueOrDefault(default_return_value), 0.05);
    DOUBLES_EQUAL(expected_return_value, mock().returnDoubleValueOrDefault(default_return_value), 0.05);
}

TEST(MockReturnValueTest, WhenNoDoubleReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    double default_return_value = 7.7;
    mock().expectOneCall("foo");
    DOUBLES_EQUAL(default_return_value, mock().actualCall("foo").returnDoubleValueOrDefault(default_return_value), 0.05);
    DOUBLES_EQUAL(default_return_value, mock().returnDoubleValueOrDefault(default_return_value), 0.05);
}

TEST(MockReturnValueTest, WhenAUnsignedIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    unsigned int default_return_value = 10;
    unsigned int expected_return_value = default_return_value + 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnUnsignedIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnUnsignedIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoUnsignedIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    unsigned int default_return_value = 10;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnUnsignedIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnUnsignedIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenAUnsignedLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    unsigned long int default_return_value = 121487;
    unsigned long int expected_return_value = default_return_value + 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnUnsignedLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnUnsignedLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoUnsignedLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    unsigned long int default_return_value = 7710144;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnUnsignedLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnUnsignedLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenALongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    long int default_return_value = 748797;
    long int expected_return_value = default_return_value + 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    long int default_return_value = 123659;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnLongIntValueOrDefault(default_return_value));
}

#ifdef CPPUTEST_USE_LONG_LONG

TEST(MockReturnValueTest, WhenAUnsignedLongLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    unsigned long long int default_return_value = ULLONG_MAX;
    unsigned long long int expected_return_value = default_return_value - 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnUnsignedLongLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnUnsignedLongLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoUnsignedLongLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    unsigned long long int default_return_value = ULLONG_MAX;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnUnsignedLongLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnUnsignedLongLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenALongLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    long long int default_return_value = LLONG_MAX;
    long long int expected_return_value = default_return_value - 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnLongLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnLongLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoLongLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    long long int default_return_value = LLONG_MAX;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnLongLongIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnLongLongIntValueOrDefault(default_return_value));
}

#endif

TEST(MockReturnValueTest, WhenABooleanReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    bool default_return_value = true;
    bool expected_return_value = false;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    CHECK_EQUAL(expected_return_value, mock().actualCall("foo").returnBoolValueOrDefault(default_return_value));
    CHECK_EQUAL(expected_return_value, mock().returnBoolValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoBooleanReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    bool default_return_value = true;
    mock().expectOneCall("foo");
    CHECK_EQUAL(default_return_value, mock().actualCall("foo").returnBoolValueOrDefault(default_return_value));
    CHECK_EQUAL(default_return_value, mock().returnBoolValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenAIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    int default_return_value = 777;
    int expected_return_value = default_return_value + 1;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    LONGS_EQUAL(expected_return_value, mock().actualCall("foo").returnIntValueOrDefault(default_return_value));
    LONGS_EQUAL(expected_return_value, mock().returnIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    int default_return_value = 777;
    mock().expectOneCall("foo");
    LONGS_EQUAL(default_return_value, mock().actualCall("foo").returnIntValueOrDefault(default_return_value));
    LONGS_EQUAL(default_return_value, mock().returnIntValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, BooleanReturnValue)
{
    bool expected_value = true;
    mock().expectOneCall("foo").andReturnValue(true);
    MockActualCall& actual_call = mock().actualCall("foo");

    CHECK_EQUAL(expected_value, actual_call.returnValue().getBoolValue());
    CHECK_EQUAL(expected_value, actual_call.returnBoolValue());

    CHECK_EQUAL(expected_value, mock().returnValue().getBoolValue());
    CHECK_EQUAL(expected_value, mock().boolReturnValue());
}

TEST(MockReturnValueTest, BooleanReturnValueSetsDifferentValues)
{
    bool expected_value = true;
    bool another_expected_value = false;

    mock().expectOneCall("foo").andReturnValue(expected_value);
    mock().expectOneCall("foo").andReturnValue(another_expected_value);

    CHECK_EQUAL(expected_value, mock().actualCall("foo").returnValue().getBoolValue());
    CHECK_EQUAL(expected_value, mock().returnValue().getBoolValue());
    CHECK_EQUAL(another_expected_value, mock().actualCall("foo").returnValue().getBoolValue());
    CHECK_EQUAL(another_expected_value, mock().returnValue().getBoolValue());
}

TEST(MockReturnValueTest, BooleanReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
    bool ret_value = true;
    bool another_ret_value = false;

    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(ret_value);
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(another_ret_value);

    CHECK_EQUAL(ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getBoolValue());
    CHECK_EQUAL(ret_value, mock().returnValue().getBoolValue());
    CHECK_EQUAL(another_ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getBoolValue());
    CHECK_EQUAL(another_ret_value, mock().returnValue().getBoolValue());
}

TEST(MockReturnValueTest, IntegerReturnValue)
{
    int expected_value = 1;
    mock().expectOneCall("foo").andReturnValue(1);
    MockActualCall& actual_call = mock().actualCall("foo");

    LONGS_EQUAL(expected_value, actual_call.returnValue().getIntValue());
    LONGS_EQUAL(expected_value, actual_call.returnIntValue());

    LONGS_EQUAL(expected_value, mock().returnValue().getIntValue());
    LONGS_EQUAL(expected_value, mock().intReturnValue());
}

TEST(MockReturnValueTest, IntegerReturnValueSetsDifferentValues)
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

TEST(MockReturnValueTest, IntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
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

TEST(MockReturnValueTest, LongIntegerReturnValue)
{
    long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);

    MockActualCall& actual_call = mock().actualCall("foo");
    LONGS_EQUAL(expected_value, actual_call.returnValue().getLongIntValue());
    LONGS_EQUAL(expected_value, actual_call.returnLongIntValue());
    LONGS_EQUAL(expected_value, mock().returnValue().getLongIntValue());
    LONGS_EQUAL(expected_value, mock().longIntReturnValue());
}

TEST(MockReturnValueTest, LongIntegerReturnValueSetsDifferentValues)
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

TEST(MockReturnValueTest, LongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
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

TEST(MockReturnValueTest, UnsignedLongIntegerReturnValue)
{
    unsigned long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);

    MockActualCall& actual_call = mock().actualCall("foo");
    LONGS_EQUAL(expected_value, actual_call.returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(expected_value, actual_call.returnUnsignedLongIntValue());
    LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedLongIntValue());
    LONGS_EQUAL(expected_value, mock().unsignedLongIntReturnValue());
}

TEST(MockReturnValueTest, UnsignedLongIntegerReturnValueSetsDifferentValues)
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

TEST(MockReturnValueTest, UnsignedLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
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

#ifdef CPPUTEST_USE_LONG_LONG

TEST(MockReturnValueTest, LongLongIntegerReturnValue)
{
    long long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);

    MockActualCall& actual_call = mock().actualCall("foo");
    LONGLONGS_EQUAL(expected_value, actual_call.returnValue().getLongLongIntValue());
    LONGLONGS_EQUAL(expected_value, actual_call.returnLongLongIntValue());
    LONGLONGS_EQUAL(expected_value, mock().returnValue().getLongLongIntValue());
    LONGLONGS_EQUAL(expected_value, mock().longLongIntReturnValue());
}

TEST(MockReturnValueTest, LongLongIntegerReturnValueSetsDifferentValues)
{
    long long int expected_value = 1;
    long long int another_expected_value = 2;

    mock().expectOneCall("foo").andReturnValue(expected_value);
    mock().expectOneCall("foo").andReturnValue(another_expected_value);

    LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getLongLongIntValue());
    LONGLONGS_EQUAL(expected_value, mock().returnValue().getLongLongIntValue());
    LONGLONGS_EQUAL(another_expected_value, mock().actualCall("foo").returnValue().getLongLongIntValue());
    LONGLONGS_EQUAL(another_expected_value, mock().returnValue().getLongLongIntValue());
}

TEST(MockReturnValueTest, LongLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
    long long int ret_value = 1;
    long long int another_ret_value = 2;

    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(ret_value);
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(another_ret_value);

    LONGLONGS_EQUAL(ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getLongLongIntValue());
    LONGLONGS_EQUAL(ret_value, mock().returnValue().getLongLongIntValue());
    LONGLONGS_EQUAL(another_ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getLongLongIntValue());
    LONGLONGS_EQUAL(another_ret_value, mock().returnValue().getLongLongIntValue());
}

TEST(MockReturnValueTest, UnsignedLongLongIntegerReturnValue)
{
    unsigned long long int expected_value = 7;
    mock().expectOneCall("foo").andReturnValue(expected_value);

    MockActualCall& actual_call = mock().actualCall("foo");
    UNSIGNED_LONGLONGS_EQUAL(expected_value, actual_call.returnValue().getUnsignedLongLongIntValue());
    UNSIGNED_LONGLONGS_EQUAL(expected_value, actual_call.returnUnsignedLongLongIntValue());
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().returnValue().getUnsignedLongLongIntValue());
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().unsignedLongLongIntReturnValue());
}

TEST(MockReturnValueTest, UnsignedLongLongIntegerReturnValueSetsDifferentValues)
{
    unsigned long long int expected_value = 1;
    unsigned long long int another_expected_value = 2;

    mock().expectOneCall("foo").andReturnValue(expected_value);
    mock().expectOneCall("foo").andReturnValue(another_expected_value);

    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock().returnValue().getUnsignedLongLongIntValue());
    UNSIGNED_LONGLONGS_EQUAL(another_expected_value, mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
    UNSIGNED_LONGLONGS_EQUAL(another_expected_value, mock().returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValueTest, UnsignedLongLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
    unsigned long long int ret_value = 1;
    unsigned long long int another_ret_value = 2;

    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(ret_value);
    mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters().andReturnValue(another_ret_value);

    UNSIGNED_LONGLONGS_EQUAL(ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getUnsignedLongLongIntValue());
    UNSIGNED_LONGLONGS_EQUAL(ret_value, mock().returnValue().getUnsignedLongLongIntValue());
    UNSIGNED_LONGLONGS_EQUAL(another_ret_value, mock().actualCall("foo").withParameter("p1", 1).returnValue().getUnsignedLongLongIntValue());
    UNSIGNED_LONGLONGS_EQUAL(another_ret_value, mock().returnValue().getUnsignedLongLongIntValue());
}

#endif

TEST(MockReturnValueTest, MatchingReturnValueOnWhileSignature)
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

TEST(MockReturnValueTest, WhenAStringReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    const char * default_return_value = "default";
    const char * expected_return_value = "expected";
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    STRCMP_EQUAL(expected_return_value, mock().actualCall("foo").returnStringValueOrDefault(default_return_value));
    STRCMP_EQUAL(expected_return_value, mock().returnStringValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoStringReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    const char * default_return_value = "default";
    mock().expectOneCall("foo");
    STRCMP_EQUAL(default_return_value, mock().actualCall("foo").returnStringValueOrDefault(default_return_value));
    STRCMP_EQUAL(default_return_value, mock().returnStringValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, StringReturnValue)
{
    mock().expectOneCall("foo").andReturnValue("hello world");
    MockActualCall& actual_call = mock().actualCall("foo");

    STRCMP_EQUAL("hello world", actual_call.returnValue().getStringValue());
    STRCMP_EQUAL("hello world", actual_call.returnStringValue());
    STRCMP_EQUAL("hello world", mock().stringReturnValue());
}

TEST(MockReturnValueTest, DoubleReturnValue)
{
    double expected_return_value = 7.8;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);

    MockActualCall& actual_call = mock().actualCall("foo");
    DOUBLES_EQUAL(expected_return_value, actual_call.returnValue().getDoubleValue(), 0.05);
    DOUBLES_EQUAL(expected_return_value, actual_call.returnDoubleValue(), 0.05);
    DOUBLES_EQUAL(expected_return_value, mock().doubleReturnValue(), 0.05);
}

TEST(MockReturnValueTest, WhenAConstPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    const void * default_return_value = (void*) 0x7778;
    const void * expected_return_value = (void*) 0x144010;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    POINTERS_EQUAL(expected_return_value, mock().actualCall("foo").returnConstPointerValueOrDefault(default_return_value));
    POINTERS_EQUAL(expected_return_value, mock().returnConstPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoConstPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    const void * default_return_value = (void*) 0x11;
    mock().expectOneCall("foo");
    POINTERS_EQUAL(default_return_value, mock().actualCall("foo").returnConstPointerValueOrDefault(default_return_value));
    POINTERS_EQUAL(default_return_value, mock().returnConstPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenAPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    void * default_return_value = (void*) 0x777;
    void * expected_return_value = (void*) 0x144000;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    POINTERS_EQUAL(expected_return_value, mock().actualCall("foo").returnPointerValueOrDefault(default_return_value));
    POINTERS_EQUAL(expected_return_value, mock().returnPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    void * default_return_value = (void*) 0x10;
    mock().expectOneCall("foo");
    POINTERS_EQUAL(default_return_value, mock().actualCall("foo").returnPointerValueOrDefault(default_return_value));
    POINTERS_EQUAL(default_return_value, mock().returnPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenAFunctionPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
    void (*default_return_value)() = (void(*)()) 0x777;
    void (*expected_return_value)() = (void(*)()) 0x144000;
    mock().expectOneCall("foo").andReturnValue(expected_return_value);
    FUNCTIONPOINTERS_EQUAL(expected_return_value, mock().actualCall("foo").returnFunctionPointerValueOrDefault(default_return_value));
    FUNCTIONPOINTERS_EQUAL(expected_return_value, mock().returnFunctionPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, WhenNoFunctionPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
    void (*default_return_value)() = (void(*)()) 0x10;
    mock().expectOneCall("foo");
    FUNCTIONPOINTERS_EQUAL(default_return_value, mock().actualCall("foo").returnFunctionPointerValueOrDefault(default_return_value));
    FUNCTIONPOINTERS_EQUAL(default_return_value, mock().returnFunctionPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValueTest, PointerReturnValue)
{
    void* ptr = (void*) 0x00107;
    mock().expectOneCall("foo").andReturnValue(ptr);
    MockActualCall& actual_call = mock().actualCall("foo");

    POINTERS_EQUAL(ptr, actual_call.returnValue().getPointerValue());
    POINTERS_EQUAL(ptr, actual_call.returnPointerValue());
    POINTERS_EQUAL(ptr, mock().pointerReturnValue());
}

TEST(MockReturnValueTest, ConstPointerReturnValue)
{
    const void* ptr = (const void*) 0x001074;
    mock().expectOneCall("foo").andReturnValue(ptr);
    MockActualCall& actual_call = mock().actualCall("foo");
    POINTERS_EQUAL(ptr, actual_call.returnValue().getConstPointerValue());
    POINTERS_EQUAL(ptr, actual_call.returnConstPointerValue());
    POINTERS_EQUAL(ptr, mock().constPointerReturnValue());
}

TEST(MockReturnValueTest, FunctionPointerReturnValue)
{
    void (*ptr)() = (void(*)()) 0x00107;
    mock().expectOneCall("foo").andReturnValue(ptr);
    MockActualCall& actual_call = mock().actualCall("foo");

    FUNCTIONPOINTERS_EQUAL(ptr, actual_call.returnValue().getFunctionPointerValue());
    FUNCTIONPOINTERS_EQUAL(ptr, actual_call.returnFunctionPointerValue());
    FUNCTIONPOINTERS_EQUAL(ptr, mock().functionPointerReturnValue());
}

TEST(MockReturnValueTest, whenCallingDisabledOrIgnoredActualCallsThenTheyDontReturnPreviousCallsValues)
{
    mock().expectOneCall("boo").ignoreOtherParameters().andReturnValue(10);
    mock().ignoreOtherCalls();
    mock().actualCall("boo");
    mock().actualCall("An Ignored Call");

    CHECK(!mock().hasReturnValue());
}
