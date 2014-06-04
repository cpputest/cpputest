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

TEST_GROUP(MockSupport_c)
{
};

TEST(MockSupport_c, expectAndActualOneCall)
{
	mock_c()->expectOneCall("boo");
	mock_c()->actualCall("boo");
	mock_c()->checkExpectations();
}

TEST(MockSupport_c, expectAndActualParameters)
{
	mock_c()->expectOneCall("boo")->withIntParameters("integer", 1)->withDoubleParameters("double", 1.0)->
			withStringParameters("string", "string")->withPointerParameters("pointer", (void*) 1);
	mock_c()->actualCall("boo")->withIntParameters("integer", 1)->withDoubleParameters("double", 1.0)->
			withStringParameters("string", "string")->withPointerParameters("pointer", (void*) 1);
}

extern "C"{

	static int typeNameIsEqual(const void* object1, const void* object2)
	{
		return object1 == object2;
	}

	static char* typeNameValueToString(const void* PUNUSED(object))
	{
		return (char*) "valueToString";
	}

}

TEST(MockSupport_c, expectAndActualParametersOnObject)
{
	mock_c()->installComparator("typeName", typeNameIsEqual, typeNameValueToString);
	mock_c()->expectOneCall("boo")->withParameterOfType("typeName", "name", (const void*) 1);
	mock_c()->actualCall("boo")->withParameterOfType("typeName", "name", (const void*) 1);
	mock_c()->checkExpectations();
	mock_c()->removeAllComparators();
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

TEST(MockSupport_c, returnUnsignedIntValue)
{
	unsigned int expected_value = 7;
	mock_c()->expectOneCall("boo")->andReturnUnsignedIntValue(expected_value);
	LONGS_EQUAL(expected_value, mock_c()->actualCall("boo")->returnValue().value.unsignedIntValue);
	LONGS_EQUAL(MOCKVALUETYPE_UNSIGNED_INTEGER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, returnIntValue)
{
	int expected_value = 10;
	mock_c()->expectOneCall("boo")->andReturnIntValue(expected_value);
	LONGS_EQUAL(expected_value, mock_c()->actualCall("boo")->returnValue().value.intValue);
	LONGS_EQUAL(MOCKVALUETYPE_INTEGER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, returnDoubleValue)
{
	mock_c()->expectOneCall("boo")->andReturnDoubleValue(1.0);
	DOUBLES_EQUAL(1.0, mock_c()->actualCall("boo")->returnValue().value.doubleValue, 0.005);
	LONGS_EQUAL(MOCKVALUETYPE_DOUBLE, mock_c()->returnValue().type);
}

TEST(MockSupport_c, returnStringValue)
{
	mock_c()->expectOneCall("boo")->andReturnStringValue("hello world");
	STRCMP_EQUAL("hello world", mock_c()->actualCall("boo")->returnValue().value.stringValue);
	LONGS_EQUAL(MOCKVALUETYPE_STRING, mock_c()->returnValue().type);
}

TEST(MockSupport_c, returnPointerValue)
{
	mock_c()->expectOneCall("boo")->andReturnPointerValue((void*) 10);
	POINTERS_EQUAL((void*) 10, mock_c()->actualCall("boo")->returnValue().value.pointerValue);
	LONGS_EQUAL(MOCKVALUETYPE_POINTER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, returnConstPointerValue)
{
	mock_c()->expectOneCall("boo")->andReturnConstPointerValue((const void*) 10);
	POINTERS_EQUAL((const void*) 10, mock_c()->actualCall("boo")->returnValue().value.constPointerValue);
	LONGS_EQUAL(MOCKVALUETYPE_CONST_POINTER, mock_c()->returnValue().type);
}

TEST(MockSupport_c, MockSupportWithScope)
{
	mock_scope_c("scope")->expectOneCall("boo");
	LONGS_EQUAL(0, mock_scope_c("other")->expectedCallsLeft());
	LONGS_EQUAL(1, mock_scope_c("scope")->expectedCallsLeft());
	mock_scope_c("scope")->actualCall("boo");
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

TEST(MockSupport_c, MockSupportSetDataObject)
{
	mock_c()->setDataObject("name", "type", (void*) 1);
	POINTERS_EQUAL((void*) 1, mock_c()->getData("name").value.objectValue);
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
}

// Silly wrapper because of a test that only fails in Visual C++ due to different
// destructor behaviors
#ifdef _MSC_VER
#define MSC_SWITCHED_TEST(testGroup, testName) IGNORE_TEST(testGroup, testName)
#else
#define MSC_SWITCHED_TEST(testGroup, testName) IGNORE_TEST(testGroup, testName)
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

