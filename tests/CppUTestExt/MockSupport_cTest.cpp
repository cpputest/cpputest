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

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTestExt/MockSupport_c.h"
#include "MockSupport_cTestCFile.h"
#include "CppUTestExt/OrderedTest.h"

extern "C" {
  static void dummy_function_for_mock_c_test()
  {
  }
  static void dummy_function_for_mock_c_test_two()
  {
  }
}

TEST_GROUP(MockSupport_c)
{
    void teardown() _override
    {
        mock_c()->clear();
    }
};

TEST(MockSupport_c, OrderObserved)
{
    mock_c()->strictOrder();

    mock_c()->expectOneCall("foo1");
    mock_c()->expectOneCall("foo2");
    mock_c()->actualCall("foo1");
    mock_c()->actualCall("foo2");

    mock_c()->checkExpectations();
}

TEST(MockSupport_c, hasReturnValue)
{
    mock_c()->expectOneCall("foo");
    CHECK(mock_c()->actualCall("foo")->hasReturnValue() == 0);
    CHECK(mock_c()->hasReturnValue() == 0);

    mock_c()->expectOneCall("foo2")->andReturnIntValue(1);
    CHECK(mock_c()->actualCall("foo2")->hasReturnValue() != 0);
    CHECK(mock_c()->hasReturnValue() != 0);
}

TEST(MockSupport_c, expectAndActualOneCall)
{
    mock_c()->expectOneCall("boo");
    mock_c()->actualCall("boo");
    mock_c()->checkExpectations();
}

TEST(MockSupport_c, expectAndActualThreeCalls)
{
    mock_c()->expectNCalls(3, "boo");
    mock_c()->actualCall("boo");
    mock_c()->actualCall("boo");
    mock_c()->actualCall("boo");
    mock_c()->checkExpectations();
}

TEST(MockSupport_c, expectNoCall)
{
    mock_c()->expectNoCall("foo");
    mock_c()->expectOneCall("bar");
    mock_c()->actualCall("bar");
    mock_c()->checkExpectations();
}

TEST(MockSupport_c, expectAndActualParameters)
{
    mock_c()->expectOneCall("boo")->withIntParameters("integer", 1)->withDoubleParameters("double", 1.0)->
            withStringParameters("string", "string")->withPointerParameters("pointer", (void*) 1)->
            withFunctionPointerParameters("functionPointer", dummy_function_for_mock_c_test);
    mock_c()->actualCall("boo")->withIntParameters("integer", 1)->withDoubleParameters("double", 1.0)->
            withStringParameters("string", "string")->withPointerParameters("pointer", (void*) 1)->
            withFunctionPointerParameters("functionPointer", dummy_function_for_mock_c_test);
}

extern "C"{

    static int typeNameIsEqual(const void* object1, const void* object2)
    {
        return object1 == object2;
    }

    static const char* typeNameValueToString(const void* PUNUSED(object))
    {
        return "valueToString";
    }

    static void typeCopy(void* dst, const void* src)
    {
        *(int*) dst = *(const int*) src;
    }

}

TEST(MockSupport_c, expectAndActualParametersOnObject)
{
    mock_c()->installComparator("typeName", typeNameIsEqual, typeNameValueToString);
    mock_c()->expectOneCall("boo")->withParameterOfType("typeName", "name", (const void*) 1);
    mock_c()->actualCall("boo")->withParameterOfType("typeName", "name", (const void*) 1);
    mock_c()->checkExpectations();
    mock_c()->removeAllComparatorsAndCopiers();
}

TEST(MockSupport_c, boolParameter)
{
    mock_c()->expectOneCall("foo")->withBoolParameters("p", 1);
    mock_c()->actualCall("foo")->withBoolParameters("p", 1);
}

TEST(MockSupport_c, unsignedIntParameter)
{
    mock_c()->expectOneCall("foo")->withUnsignedIntParameters("p", 1);
    mock_c()->actualCall("foo")->withUnsignedIntParameters("p", 1);
}

TEST(MockSupport_c, longIntParameter)
{
    mock_c()->expectOneCall("foo")->withLongIntParameters("p", 1);
    mock_c()->actualCall("foo")->withLongIntParameters("p", 1);
}

TEST(MockSupport_c, unsignedLongIntParameter)
{
    mock_c()->expectOneCall("foo")->withUnsignedLongIntParameters("p", 1);
    mock_c()->actualCall("foo")->withUnsignedLongIntParameters("p", 1);
}

TEST(MockSupport_c, doubleParameterWithTolerance)
{
    mock_c( )->expectOneCall("foo")->withDoubleParametersAndTolerance("p", 2.0, 0.2);
    mock_c( )->actualCall("foo")->withDoubleParameters("p", 1.9);
}

#ifdef CPPUTEST_USE_LONG_LONG

TEST(MockSupport_c, longLongIntParameter)
{
    mock_c()->expectOneCall("foo")->withLongLongIntParameters("p", 1);
    mock_c()->actualCall("foo")->withLongLongIntParameters("p", 1);
}

TEST(MockSupport_c, unsignedLongLongIntParameter)
{
    mock_c()->expectOneCall("foo")->withUnsignedLongLongIntParameters("p", 1);
    mock_c()->actualCall("foo")->withUnsignedLongLongIntParameters("p", 1);
}

#endif

TEST(MockSupport_c, memoryBufferParameter)
{
    const unsigned char mem_buffer[] = { 1, 2, 3};
    mock_c()->expectOneCall("foo")->withMemoryBufferParameter("out", mem_buffer, sizeof(mem_buffer));
    mock_c()->actualCall("foo")->withMemoryBufferParameter("out", mem_buffer, sizeof(mem_buffer));
    mock_c()->checkExpectations();
}

TEST(MockSupport_c, outputParameters)
{
    int param = 1;
    const int retval = 2;
    mock_c()->expectOneCall("foo")->withOutputParameterReturning("out", &retval, sizeof(retval));
    mock_c()->actualCall("foo")->withOutputParameter("out", &param);
    mock_c()->checkExpectations();
    LONGS_EQUAL(2, param);
    LONGS_EQUAL(2, retval);
}

TEST(MockSupport_c, unmodifiedOutputParameter)
{
    int param = 1;
    mock_c()->expectOneCall("foo")->withUnmodifiedOutputParameter("out");
    mock_c()->actualCall("foo")->withOutputParameter("out", &param);
    mock_c()->checkExpectations();
    LONGS_EQUAL(1, param);
}

TEST(MockSupport_c, outputParameters_differentType)
{
    long param = 1;
    const long retval = 2;
    mock_c()->expectOneCall("foo")->withOutputParameterReturning("out", &retval, sizeof(retval));
    mock_c()->actualCall("foo")->withOutputParameter("out", &param);
    mock_c()->checkExpectations();
    LONGS_EQUAL(2, param);
    LONGS_EQUAL(2, retval);
}

TEST(MockSupport_c, outputParametersOfType)
{
    int param = 1;
    const int retval = 2;
    mock_c()->installCopier("typeName", typeCopy);
    mock_c()->expectOneCall("foo")->withOutputParameterOfTypeReturning("typeName", "out", &retval);
    mock_c()->actualCall("foo")->withOutputParameterOfType("typeName", "out", &param);
    LONGS_EQUAL(2, param);
    LONGS_EQUAL(2, retval);
    mock_c()->checkExpectations();
    mock_c()->removeAllComparatorsAndCopiers();
}

TEST(MockSupport_c, ignoreOtherParameters)
{
    mock_c()->expectOneCall("foo")->withIntParameters("int", 1)->ignoreOtherParameters();
    mock_c()->actualCall("foo")->withIntParameters("int", 1)->withDoubleParameters("double", 0.01);
    mock_c()->checkExpectations();
}

TEST(MockSupport_c, returnBoolValue)
{
    int expected_value = 1;
    mock_c()->expectOneCall("boo")->andReturnBoolValue(expected_value);
    CHECK_EQUAL(expected_value, mock_c()->actualCall("boo")->boolReturnValue());
    CHECK_EQUAL(expected_value, mock_c()->boolReturnValue());
    LONGS_EQUAL(MOCKVALUETYPE_BOOL, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnBoolValueOrDefaultShouldIgnoreTheDefault)
{
    int defaultValue = 1;
    int expectedValue = 0;
    mock_c()->expectOneCall("foo")->andReturnBoolValue(expectedValue);
    LONGS_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnBoolValueOrDefault(defaultValue));
    LONGS_EQUAL(expectedValue, mock_c()->returnBoolValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnBoolValueOrDefaultShouldlUseTheDefaultValue)
{
    int defaultValue = 1;
    mock_c()->expectOneCall("foo");
    LONGS_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnBoolValueOrDefault(defaultValue));
    LONGS_EQUAL(defaultValue, mock_c()->returnBoolValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnIntValue)
{
    int expected_value = -10;
    mock_c()->expectOneCall("boo")->andReturnIntValue(expected_value);
    LONGS_EQUAL(expected_value, mock_c()->actualCall("boo")->intReturnValue());
    LONGS_EQUAL(expected_value, mock_c()->intReturnValue());
    LONGS_EQUAL(MOCKVALUETYPE_INTEGER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnIntValueOrDefaultShouldIgnoreTheDefault)
{
    int defaultValue = -10;
    int expectedValue = defaultValue - 1;
    mock_c()->expectOneCall("foo")->andReturnIntValue(expectedValue);
    LONGS_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnIntValueOrDefault(defaultValue));
    LONGS_EQUAL(expectedValue, mock_c()->returnIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnIntValueOrDefaultShouldlUseTheDefaultValue)
{
    int defaultValue = -10;
    mock_c()->expectOneCall("foo");
    LONGS_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnIntValueOrDefault(defaultValue));
    LONGS_EQUAL(defaultValue, mock_c()->returnIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnUnsignedIntValue)
{
    unsigned int expected_value = 7;
    mock_c()->expectOneCall("boo")->andReturnUnsignedIntValue(expected_value);
    LONGS_EQUAL(expected_value, mock_c()->actualCall("boo")->unsignedIntReturnValue());
    LONGS_EQUAL(expected_value, mock_c()->unsignedIntReturnValue());
    LONGS_EQUAL(MOCKVALUETYPE_UNSIGNED_INTEGER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnUnsignedIntValueOrDefaultShouldIgnoreTheDefault)
{
    unsigned int defaultValue = 10;
    unsigned int expectedValue = defaultValue + 1;
    mock_c()->expectOneCall("foo")->andReturnUnsignedIntValue(expectedValue);
    LONGS_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnUnsignedIntValueOrDefault(defaultValue));
    LONGS_EQUAL(expectedValue, mock_c()->returnUnsignedIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnUnsignedIntValueOrDefaultShouldlUseTheDefaultValue)
{
    unsigned int defaultValue = 10;
    mock_c()->expectOneCall("foo");
    LONGS_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnUnsignedIntValueOrDefault(defaultValue));
    LONGS_EQUAL(defaultValue, mock_c()->returnUnsignedIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnLongIntValue)
{
    long int expected_value = -10L;
    mock_c()->expectOneCall("boo")->andReturnLongIntValue(expected_value);
    LONGS_EQUAL(expected_value, mock_c()->actualCall("boo")->longIntReturnValue());
    LONGS_EQUAL(expected_value, mock_c()->longIntReturnValue());
    LONGS_EQUAL(MOCKVALUETYPE_LONG_INTEGER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnLongIntValueOrDefaultShouldIgnoreTheDefault)
{
    long int defaultValue = -10L;
    long int expectedValue = defaultValue - 1L;
    mock_c()->expectOneCall("foo")->andReturnLongIntValue(expectedValue);
    LONGS_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnLongIntValueOrDefault(defaultValue));
    LONGS_EQUAL(expectedValue, mock_c()->returnLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
    long int defaultValue = -10L;
    mock_c()->expectOneCall("foo");
    LONGS_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnLongIntValueOrDefault(defaultValue));
    LONGS_EQUAL(defaultValue, mock_c()->returnLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnUnsignedLongIntValue)
{
    unsigned long int expected_value = 10;
    mock_c()->expectOneCall("boo")->andReturnUnsignedLongIntValue(expected_value);
    LONGS_EQUAL(expected_value, mock_c()->actualCall("boo")->unsignedLongIntReturnValue());
    LONGS_EQUAL(expected_value, mock_c()->unsignedLongIntReturnValue());
    LONGS_EQUAL(MOCKVALUETYPE_UNSIGNED_LONG_INTEGER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnUnsignedLongIntValueOrDefaultShouldIgnoreTheDefault)
{
    unsigned long int defaultValue = 10L;
    unsigned long int expectedValue = defaultValue + 1L;
    mock_c()->expectOneCall("foo")->andReturnUnsignedLongIntValue(expectedValue);
    LONGS_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnUnsignedLongIntValueOrDefault(defaultValue));
    LONGS_EQUAL(expectedValue, mock_c()->returnUnsignedLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnUnsignedLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
    unsigned long int defaultValue = 10L;
    mock_c()->expectOneCall("foo");
    LONGS_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnUnsignedLongIntValueOrDefault(defaultValue));
    LONGS_EQUAL(defaultValue, mock_c()->returnUnsignedLongIntValueOrDefault(defaultValue));
}

#ifdef CPPUTEST_USE_LONG_LONG

TEST(MockSupport_c, returnLongLongIntValue)
{
    long long int expected_value = -10L;
    mock_c()->expectOneCall("boo")->andReturnLongLongIntValue(expected_value);
    LONGLONGS_EQUAL(expected_value, mock_c()->actualCall("boo")->longLongIntReturnValue());
    LONGLONGS_EQUAL(expected_value, mock_c()->longLongIntReturnValue());
    LONGLONGS_EQUAL(MOCKVALUETYPE_LONG_LONG_INTEGER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnLongLongIntValueOrDefaultShouldIgnoreTheDefault)
{
    long long int defaultValue = -10L;
    long long int expectedValue = defaultValue - 1L;
    mock_c()->expectOneCall("foo")->andReturnLongLongIntValue(expectedValue);
    LONGLONGS_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnLongLongIntValueOrDefault(defaultValue));
    LONGLONGS_EQUAL(expectedValue, mock_c()->returnLongLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnLongLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
    long long int defaultValue = -10L;
    mock_c()->expectOneCall("foo");
    LONGLONGS_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnLongLongIntValueOrDefault(defaultValue));
    LONGLONGS_EQUAL(defaultValue, mock_c()->returnLongLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnUnsignedLongLongIntValue)
{
    unsigned long long int expected_value = 10;
    mock_c()->expectOneCall("boo")->andReturnUnsignedLongLongIntValue(expected_value);
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock_c()->actualCall("boo")->unsignedLongLongIntReturnValue());
    UNSIGNED_LONGLONGS_EQUAL(expected_value, mock_c()->unsignedLongLongIntReturnValue());
    UNSIGNED_LONGLONGS_EQUAL(MOCKVALUETYPE_UNSIGNED_LONG_LONG_INTEGER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnUnsignedLongLongIntValueOrDefaultShouldIgnoreTheDefault)
{
    unsigned long long int defaultValue = 10L;
    unsigned long long int expectedValue = defaultValue + 1L;
    mock_c()->expectOneCall("foo")->andReturnUnsignedLongLongIntValue(expectedValue);
    UNSIGNED_LONGLONGS_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnUnsignedLongLongIntValueOrDefault(defaultValue));
    UNSIGNED_LONGLONGS_EQUAL(expectedValue, mock_c()->returnUnsignedLongLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnUnsignedLongLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
    unsigned long long int defaultValue = 10L;
    mock_c()->expectOneCall("foo");
    UNSIGNED_LONGLONGS_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnUnsignedLongLongIntValueOrDefault(defaultValue));
    UNSIGNED_LONGLONGS_EQUAL(defaultValue, mock_c()->returnUnsignedLongLongIntValueOrDefault(defaultValue));
}

#endif

TEST(MockSupport_c, returnStringValue)
{
    mock_c()->expectOneCall("boo")->andReturnStringValue("hello world");
    STRCMP_EQUAL("hello world", mock_c()->actualCall("boo")->stringReturnValue());
    STRCMP_EQUAL("hello world", mock_c()->stringReturnValue());
    LONGS_EQUAL(MOCKVALUETYPE_STRING, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnStringValueOrDefaultShouldIgnoreTheDefault)
{
    const char defaultValue[] = "bar";
    const char expectedValue[] = "bla";
    mock_c()->expectOneCall("foo")->andReturnStringValue(expectedValue);
    STRCMP_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnStringValueOrDefault(defaultValue));
    STRCMP_EQUAL(expectedValue, mock_c()->returnStringValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnStringValueOrDefaultShouldlUseTheDefaultValue)
{
    const char defaultValue[] = "bar";
    mock_c()->expectOneCall("foo");
    STRCMP_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnStringValueOrDefault(defaultValue));
    STRCMP_EQUAL(defaultValue, mock_c()->returnStringValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnDoubleValue)
{
    mock_c()->expectOneCall("boo")->andReturnDoubleValue(1.0);
    DOUBLES_EQUAL(1.0, mock_c()->actualCall("boo")->doubleReturnValue(), 0.005);
    DOUBLES_EQUAL(1.0, mock_c()->doubleReturnValue(), 0.005);
    LONGS_EQUAL(MOCKVALUETYPE_DOUBLE, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnDoubleValueOrDefaultShouldIgnoreTheDefault)
{
    double defaultValue = 2.2;
    double expectedValue = defaultValue + 0.1;
    mock_c()->expectOneCall("foo")->andReturnDoubleValue(expectedValue);
    DOUBLES_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnDoubleValueOrDefault(defaultValue), 0.005);
    DOUBLES_EQUAL(expectedValue, mock_c()->returnDoubleValueOrDefault(defaultValue), 0.005);
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnDoubleValueOrDefaultShouldlUseTheDefaultValue)
{
    double defaultValue = 2.2;
    mock_c()->expectOneCall("foo");
    DOUBLES_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnDoubleValueOrDefault(defaultValue), 0.005);
    DOUBLES_EQUAL(defaultValue, mock_c()->returnDoubleValueOrDefault(defaultValue), 0.005);
}

TEST(MockSupport_c, returnPointerValue)
{
    mock_c()->expectOneCall("boo")->andReturnPointerValue((void*) 10);
    POINTERS_EQUAL((void*) 10, mock_c()->actualCall("boo")->pointerReturnValue());
    POINTERS_EQUAL((void*) 10, mock_c()->pointerReturnValue());
    LONGS_EQUAL(MOCKVALUETYPE_POINTER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnPointerValueOrDefaultShouldIgnoreTheDefault)
{
    void* defaultValue = (void*) 10;
    void* expectedValue = (void*) 27;
    mock_c()->expectOneCall("foo")->andReturnPointerValue(expectedValue);
    POINTERS_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnPointerValueOrDefault(defaultValue));
    POINTERS_EQUAL(expectedValue, mock_c()->returnPointerValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnPointerValueOrDefaultShouldlUseTheDefaultValue)
{
    void* defaultValue = (void*) 10;
    mock_c()->expectOneCall("foo");
    POINTERS_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnPointerValueOrDefault(defaultValue));
    POINTERS_EQUAL(defaultValue, mock_c()->returnPointerValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnConstPointerValue)
{
    mock_c()->expectOneCall("boo")->andReturnConstPointerValue((const void*) 10);
    POINTERS_EQUAL((const void*) 10, mock_c()->actualCall("boo")->constPointerReturnValue());
    POINTERS_EQUAL((const void*) 10, mock_c()->constPointerReturnValue());
    LONGS_EQUAL(MOCKVALUETYPE_CONST_POINTER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnConstPointerValueOrDefaultShouldIgnoreTheDefault)
{
    const void* defaultValue = (void*) 10;
    const void* expectedValue = (void*) 27;
    mock_c()->expectOneCall("foo")->andReturnConstPointerValue(expectedValue);
    POINTERS_EQUAL(expectedValue, mock_c()->actualCall("foo")->returnConstPointerValueOrDefault(defaultValue));
    POINTERS_EQUAL(expectedValue, mock_c()->returnConstPointerValueOrDefault(defaultValue));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnConstPointerValueOrDefaultShouldlUseTheDefaultValue)
{
    const void* defaultValue = (void*) 10;
    mock_c()->expectOneCall("foo");
    POINTERS_EQUAL(defaultValue, mock_c()->actualCall("foo")->returnConstPointerValueOrDefault(defaultValue));
    POINTERS_EQUAL(defaultValue, mock_c()->returnConstPointerValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnFunctionPointerValue)
{
    mock_c()->expectOneCall("boo")->andReturnFunctionPointerValue(dummy_function_for_mock_c_test);
    FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test, mock_c()->actualCall("boo")->functionPointerReturnValue());
    FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test, mock_c()->functionPointerReturnValue());
    LONGS_EQUAL(MOCKVALUETYPE_FUNCTIONPOINTER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, whenReturnValueIsGivenReturnFunctionPointerValueOrDefaultShouldIgnoreTheDefault)
{
    mock_c()->expectOneCall("foo")->andReturnFunctionPointerValue(dummy_function_for_mock_c_test);
    FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test,  mock_c()->actualCall("foo")->returnFunctionPointerValueOrDefault(dummy_function_for_mock_c_test_two));
    FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test, mock_c()->returnFunctionPointerValueOrDefault(dummy_function_for_mock_c_test_two));
}

TEST(MockSupport_c, whenNoReturnValueIsGivenReturnFunctionPointerValueOrDefaultShouldlUseTheDefaultValue)
{
    mock_c()->expectOneCall("foo");
    FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test_two, mock_c()->actualCall("foo")->returnFunctionPointerValueOrDefault(dummy_function_for_mock_c_test_two));
    FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test_two, mock_c()->returnFunctionPointerValueOrDefault(dummy_function_for_mock_c_test_two));
}

TEST(MockSupport_c, MockSupportWithScope)
{
    mock_scope_c("scope")->expectOneCall("boo");
    LONGS_EQUAL(0, mock_scope_c("other")->expectedCallsLeft());
    LONGS_EQUAL(1, mock_scope_c("scope")->expectedCallsLeft());
    mock_scope_c("scope")->actualCall("boo");
}

TEST(MockSupport_c, MockSupportSetBoolData)
{
    mock_c()->setBoolData("boolean", 1);
    CHECK_EQUAL(1, mock_c()->getData("boolean").value.boolValue);
}

TEST(MockSupport_c, MockSupportSetIntData)
{
    mock_c()->setIntData("integer", 10);
    LONGS_EQUAL(10, mock_c()->getData("integer").value.intValue);
}

TEST(MockSupport_c, MockSupportSetDoubleData)
{
    mock_c()->setDoubleData("double", 1.0);
    DOUBLES_EQUAL(1.00, mock_c()->getData("double").value.doubleValue, 0.05);
}

TEST(MockSupport_c, MockSupportSetStringData)
{
    mock_c()->setStringData("string", "hello world");
    STRCMP_EQUAL("hello world", mock_c()->getData("string").value.stringValue);
}

TEST(MockSupport_c, MockSupportSetPointerData)
{
    mock_c()->setPointerData("pointer", (void*) 1);
    POINTERS_EQUAL((void*) 1, mock_c()->getData("pointer").value.pointerValue);
}

TEST(MockSupport_c, MockSupportSetConstPointerData)
{
    mock_c()->setConstPointerData("constPointer", (const void*) 1);
    POINTERS_EQUAL((const void*) 1, mock_c()->getData("constPointer").value.constPointerValue);
}

TEST(MockSupport_c, MockSupportMemoryBufferData)
{
    mock_c()->setDataObject("name", "const unsigned char*", (void *) 0xDEAD);
    POINTERS_EQUAL(0xDEAD, mock_c()->getData("name").value.memoryBufferValue);
    LONGS_EQUAL(MOCKVALUETYPE_MEMORYBUFFER, mock_c()->getData("name").type);
}

TEST(MockSupport_c, MockSupportSetFunctionPointerData)
{
    mock_c()->setFunctionPointerData("functionPointer", dummy_function_for_mock_c_test);
    FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test, mock_c()->getData("functionPointer").value.functionPointerValue);
}

TEST(MockSupport_c, MockSupportSetDataObject)
{
    mock_c()->setDataObject("name", "type", (void*) 1);
    POINTERS_EQUAL((void*) 1, mock_c()->getData("name").value.objectValue);
}

TEST(MockSupport_c, MockSupportSetDataConstObject)
{
    mock_c()->setDataConstObject("name", "type", (const void*) 5);
    POINTERS_EQUAL((void*) 5, mock_c()->getData("name").value.constObjectValue);
}

TEST(MockSupport_c, WorksInCFile)
{
    all_mock_support_c_calls();
}

static bool destructorWasCalled = false;

static void failedCallToMockC()
{
    SetBooleanOnDestructorCall setOneDestructor(destructorWasCalled);
    mock_c()->actualCall("Not a call");
} // LCOV_EXCL_LINE

// Silly wrapper because of a test that only fails in Visual C++ due to different
// destructor behaviors
#ifdef _MSC_VER
#define MSC_SWITCHED_TEST(testGroup, testName) IGNORE_TEST(testGroup, testName)
#else
#define MSC_SWITCHED_TEST(testGroup, testName) TEST(testGroup, testName)
#endif

MSC_SWITCHED_TEST(MockSupport_c, NoExceptionsAreThrownWhenAMock_cCallFailed)
{
    TestTestingFixture fixture;

    fixture.setTestFunction(failedCallToMockC);
    fixture.runAllTests();

    LONGS_EQUAL(1, fixture.getFailureCount());
    // Odd behavior in Visual C++, destructor still gets called here
    CHECK(!destructorWasCalled);
}

static bool cpputestHasCrashed;

static void crashMethod()
{
    cpputestHasCrashed = true;
}

TEST_ORDERED(MockSupport_c, shouldCrashOnFailure, 21)
{
    cpputestHasCrashed = false;
    TestTestingFixture fixture;
    UtestShell::setCrashMethod(crashMethod);
    mock_c()->crashOnFailure(true);
    fixture.setTestFunction(failedCallToMockC);

    fixture.runAllTests();

    CHECK(cpputestHasCrashed);

    UtestShell::resetCrashMethod();
    mock_c()->crashOnFailure(false);
}

TEST_ORDERED(MockSupport_c, nextTestShouldNotCrashOnFailure, 22)
{
    cpputestHasCrashed = false;
    TestTestingFixture fixture;
    UtestShell::setCrashMethod(crashMethod);
    fixture.setTestFunction(failedCallToMockC);

    fixture.runAllTests();

    CHECK_FALSE(cpputestHasCrashed);

    UtestShell::resetCrashMethod();
}

static void failingCallToMockCWithParameterOfType_()
{
    mock_c()->expectOneCall("bar")->withParameterOfType("typeName", "name", (const void*) 1);
    mock_c()->actualCall("bar")->withParameterOfType("typeName", "name", (const void*) 2);
} // LCOV_EXCL_LINE

TEST(MockSupport_c, failureWithParameterOfTypeCoversValueToString)
{
    TestTestingFixture fixture;
    mock_c()->installComparator("typeName", typeNameIsEqual, typeNameValueToString);
    fixture.setTestFunction(failingCallToMockCWithParameterOfType_);
    fixture.runAllTests();
    fixture.assertPrintContains("typeName name: <valueToString>");
    mock_c()->removeAllComparatorsAndCopiers();
}

static void callToMockCWithOutputParameterOfType_()
{
    int value1 = 7;
    const int value2 = 9;
    mock_c()->expectOneCall("bar")->withOutputParameterOfTypeReturning("intType", "bla", &value2);
    mock_c()->actualCall("bar")->withOutputParameterOfType("intType", "bla", &value1);
    LONGS_EQUAL(value1, value2);
}

TEST(MockSupport_c, successWithOutputParameterOfType)
{
    TestTestingFixture fixture;
    mock_c()->installCopier("intType", typeCopy);
    fixture.setTestFunction(callToMockCWithOutputParameterOfType_);
    fixture.runAllTests();
    LONGS_EQUAL(2, fixture.getCheckCount());
    LONGS_EQUAL(0, fixture.getFailureCount());
    mock_c()->removeAllComparatorsAndCopiers();
}

static void failingCallToMockCWithMemoryBuffer_()
{
    unsigned char memBuffer1[] = { 0x12, 0x15, 0xFF };
    unsigned char memBuffer2[] = { 0x12, 0x05, 0xFF };
    mock_c()->expectOneCall("bar")->withMemoryBufferParameter("name", memBuffer1, sizeof(memBuffer1));
    mock_c()->actualCall("bar")->withMemoryBufferParameter("name", memBuffer2, sizeof(memBuffer2));
} // LCOV_EXCL_LINE

TEST(MockSupport_c, expectOneMemBufferParameterAndValueFailsDueToContents)
{
    TestTestingFixture fixture;
    fixture.setTestFunction(failingCallToMockCWithMemoryBuffer_);
    fixture.runAllTests();
    fixture.assertPrintContains("Unexpected parameter value to parameter \"name\" "
                                "to function \"bar\": <Size = 3 | HexContents = 12 05 FF>");
}

TEST(MockSupport_c, ignoreOtherCalls)
{
    mock_c()->expectOneCall("foo");
    mock_c()->ignoreOtherCalls();
    mock_c()->actualCall("foo");
    mock_c()->actualCall("bar");
    mock_c()->checkExpectations();
}
