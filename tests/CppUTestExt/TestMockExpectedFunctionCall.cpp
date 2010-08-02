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
#include "CppUTestExt/MockExpectedFunctionCall.h"

class TypeForTestingExpectedFunctionCall
{
public:
	TypeForTestingExpectedFunctionCall(int val) : value(val) {};
	int value;
};


class TypeForTestingExpectedFunctionCallComparator : public MockParameterComparator
{
public:
	TypeForTestingExpectedFunctionCallComparator() {}
	virtual ~TypeForTestingExpectedFunctionCallComparator() {};

	virtual bool isEqual(void* object1, void* object2)
	{
		return ((TypeForTestingExpectedFunctionCall*)object1)->value == ((TypeForTestingExpectedFunctionCall*)object2)->value;
	}
	virtual SimpleString valueToString(void* object)
	{
		return StringFrom(((TypeForTestingExpectedFunctionCall*)object)->value);
	}

};

TEST_GROUP(MockParameterComparatorRepository)
{
};

TEST(MockParameterComparatorRepository, getComparatorForNonExistingName)
{
	MockParameterComparatorRepository repository;
	POINTERS_EQUAL(NULL, repository.getComparatorForType("typeName"));
}

TEST(MockParameterComparatorRepository, installComparator)
{
	TypeForTestingExpectedFunctionCallComparator comparator;
	MockParameterComparatorRepository repository;
	repository.installComparator("typeName", comparator);
	POINTERS_EQUAL(&comparator, repository.getComparatorForType("typeName"));
}

TEST(MockParameterComparatorRepository, installMultipleComparator)
{
	TypeForTestingExpectedFunctionCallComparator comparator1, comparator2, comparator3;
	MockParameterComparatorRepository repository;
	repository.installComparator("type1", comparator1);
	repository.installComparator("type2", comparator2);
	repository.installComparator("type3", comparator3);
	POINTERS_EQUAL(&comparator3, repository.getComparatorForType("type3"));
	POINTERS_EQUAL(&comparator2, repository.getComparatorForType("type2"));
	POINTERS_EQUAL(&comparator1, repository.getComparatorForType("type1"));
}

TEST_GROUP(MockExpectedFunctionCall)
{
	MockExpectedFunctionCall* call;
	void setup ()
	{
		call = new MockExpectedFunctionCall;
	}
	void teardown()
	{
		delete call;
	}
};

TEST(MockExpectedFunctionCall, callWithoutParameterSetOrNotFound)
{
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_NONE, call->getParameterType("nonexisting"));
	LONGS_EQUAL(0, call->getParameterValue("nonexisting").intValue_);
	CHECK(!call->hasParameterWithName("nonexisting"));
}

TEST(MockExpectedFunctionCall, callWithIntegerParameter)
{
	call->withParameter("integer", 1);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_INT, call->getParameterType("integer"));
	LONGS_EQUAL(1, call->getParameterValue("integer").intValue_);
	CHECK(call->hasParameterWithName("integer"));
}

TEST(MockExpectedFunctionCall, callWithDoubleParameter)
{
	call->withParameter("double", 1.2);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_DOUBLE, call->getParameterType("double"));
	DOUBLES_EQUAL(1.2, call->getParameterValue("double").doubleValue_, 0.05);
}

TEST(MockExpectedFunctionCall, callWithStringParameter)
{
	call->withParameter("string", "hello world");
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_STRING, call->getParameterType("string"));
	STRCMP_EQUAL("hello world", call->getParameterValue("string").stringValue_);
}

TEST(MockExpectedFunctionCall, callWithPointerParameter)
{
	void* ptr = (void*) 0x123;
	call->withParameter("pointer", ptr);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_POINTER, call->getParameterType("pointer"));
	POINTERS_EQUAL(ptr, call->getParameterValue("pointer").pointerValue_);
}

TEST(MockExpectedFunctionCall, callWithObjectParameter)
{
	void* ptr = (void*) 0x123;
	call->withParameterOfType("class", "object", ptr);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_OBJECT, call->getParameterType("object"));
	POINTERS_EQUAL(ptr, call->getParameterValue("object").objectPointerValue_);
	STRCMP_EQUAL("class", call->getParameterTypeName("object").asCharString());
}

TEST(MockExpectedFunctionCall, callWithObjectParameterUnequalComparison)
{
	TypeForTestingExpectedFunctionCall type(1), unequalType(2);
	MockFunctionParameter parameter ("name", MOCK_FUNCTION_PARAMETER_OBJECT, NULL);
	parameter.value_.objectPointerValue_ = &unequalType;
	call->withParameterOfType("type", "name", &type);
	CHECK (!call->hasParameter(parameter));
}

TEST(MockExpectedFunctionCall, callWithObjectParameterEqualComparisonButFailsWithoutRepository)
{
	TypeForTestingExpectedFunctionCall type(1), equalType(1);
	MockFunctionParameter parameter ("name", MOCK_FUNCTION_PARAMETER_OBJECT, NULL);
	parameter.value_.objectPointerValue_ = &equalType;
	call->withParameterOfType("type", "name", &type);
	CHECK (!call->hasParameter(parameter));
}

TEST(MockExpectedFunctionCall, callWithObjectParameterEqualComparisonButFailsWithoutComparator)
{
	MockParameterComparatorRepository repository;
	call->setComparatorRepository(&repository);

	TypeForTestingExpectedFunctionCall type(1), equalType(1);
	MockFunctionParameter parameter ("name", MOCK_FUNCTION_PARAMETER_OBJECT, NULL);
	parameter.value_.objectPointerValue_ = &equalType;
	call->withParameterOfType("type", "name", &type);
	CHECK (!call->hasParameter(parameter));
}

TEST(MockExpectedFunctionCall, callWithObjectParameterEqualComparison)
{
	TypeForTestingExpectedFunctionCallComparator comparator;
	MockParameterComparatorRepository repository;
	repository.installComparator("type", comparator);

	TypeForTestingExpectedFunctionCall type(1), equalType(1);
	MockFunctionParameter parameter ("name", MOCK_FUNCTION_PARAMETER_OBJECT, NULL);
	parameter.value_.objectPointerValue_ = &equalType;
	parameter.typeName_ = "type";

	call->setComparatorRepository(&repository);
	call->withParameterOfType("type", "name", &type);
	CHECK (call->hasParameter(parameter));
}

TEST(MockExpectedFunctionCall, getParameterValueOfObjectType)
{
	TypeForTestingExpectedFunctionCallComparator comparator;
	MockParameterComparatorRepository repository;
	repository.installComparator("type", comparator);

	TypeForTestingExpectedFunctionCall type(1);
	call->setComparatorRepository(&repository);
	call->withParameterOfType("type", "name", &type);
	POINTERS_EQUAL(&type, call->getParameterValue("name").objectPointerValue_);
	STRCMP_EQUAL("1", call->getParameterValueString("name").asCharString());
}

TEST(MockExpectedFunctionCall, getParameterValueOfObjectTypeWithoutRepository)
{
	TypeForTestingExpectedFunctionCall type(1);
	call->withParameterOfType("type", "name", &type);
	STRCMP_EQUAL("No comparator repository was set.", call->getParameterValueString("name").asCharString());
}

TEST(MockExpectedFunctionCall, getParameterValueOfObjectTypeWithoutComparator)
{
	TypeForTestingExpectedFunctionCall type(1);
	MockParameterComparatorRepository repository;
	call->setComparatorRepository(&repository);
	call->withParameterOfType("type", "name", &type);
	STRCMP_EQUAL("No comparator found for type: \"type\"", call->getParameterValueString("name").asCharString());
}


TEST(MockExpectedFunctionCall, callWithTwoIntegerParameter)
{
	call->withParameter("integer1", 1);
	call->withParameter("integer2", 2);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_INT, call->getParameterType("integer1"));
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_INT, call->getParameterType("integer2"));
	LONGS_EQUAL(1, call->getParameterValue("integer1").intValue_);
	LONGS_EQUAL(2, call->getParameterValue("integer2").intValue_);
}

TEST(MockExpectedFunctionCall, callWithThreeDifferentParameter)
{
	call->withParameter("integer", 1);
	call->withParameter("string", "hello world");
	call->withParameter("double", 0.12);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_INT, call->getParameterType("integer"));
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_STRING, call->getParameterType("string"));
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_DOUBLE, call->getParameterType("double"));
	LONGS_EQUAL(1, call->getParameterValue("integer").intValue_);
	STRCMP_EQUAL("hello world", call->getParameterValue("string").stringValue_);
	DOUBLES_EQUAL(0.12, call->getParameterValue("double").doubleValue_, 0.05);
}

TEST(MockExpectedFunctionCall, withoutANameItsFulfilled)
{
	CHECK(call->isFulfilled());
}

TEST(MockExpectedFunctionCall, withANameItsNotFulfilled)
{
	call->withName("name");
	CHECK(!call->isFulfilled());
}

TEST(MockExpectedFunctionCall, afterSettingCallFulfilledItsFulFilled)
{
	call->withName("name");
	call->callWasMade();
	CHECK(call->isFulfilled());
}

TEST(MockExpectedFunctionCall, calledButNotWithParameterIsNotFulFilled)
{
	call->withName("name")->withParameter("para", 1);
	call->callWasMade();
	CHECK(!call->isFulfilled());
}

TEST(MockExpectedFunctionCall, calledAndParametersAreFulfilled)
{
	call->withName("name")->withParameter("para", 1);
	call->callWasMade();
	call->parameterWasPassed("para");
	CHECK(call->isFulfilled());
}

TEST(MockExpectedFunctionCall, calledButNotAllParametersAreFulfilled)
{
	call->withName("name")->withParameter("para", 1)->withParameter("two", 2);
	call->callWasMade();
	call->parameterWasPassed("para");
	CHECK(! call->isFulfilled());
}
