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
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockFailure.h"
#include "MockFailureReporterForTest.h"

class TypeForTestingExpectedFunctionCall
{
public:
    TypeForTestingExpectedFunctionCall(int val)
    {
        value = new int(val);
    }
    virtual ~TypeForTestingExpectedFunctionCall()
    {
        delete value;
    }
    int *value;
};

class TypeForTestingExpectedFunctionCallComparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(const void* object1, const void* object2)
    {
        const TypeForTestingExpectedFunctionCall* obj1 = (const TypeForTestingExpectedFunctionCall*) object1;
        const TypeForTestingExpectedFunctionCall* obj2 = (const TypeForTestingExpectedFunctionCall*) object2;
        return *(obj1->value) == *(obj2->value);
    }
    virtual SimpleString valueToString(const void* object)
    {
        const TypeForTestingExpectedFunctionCall* obj = (const TypeForTestingExpectedFunctionCall*) object;
        return StringFrom(*(obj->value));
    }
};

class TypeForTestingExpectedFunctionCallCopier : public MockNamedValueCopier
{
public:
    virtual void copy(void* dst_, const void* src_)
    {
        TypeForTestingExpectedFunctionCall* dst = (TypeForTestingExpectedFunctionCall*) dst_;
        const TypeForTestingExpectedFunctionCall* src = (const TypeForTestingExpectedFunctionCall*) src_;
        *(dst->value) = *(src->value);
    }
};

TEST_GROUP(MockNamedValueHandlerRepository)
{
    void teardown()
    {
        CHECK_NO_MOCK_FAILURE();
    }
};

TEST(MockNamedValueHandlerRepository, getComparatorForNonExistingName)
{
    MockNamedValueComparatorsAndCopiersRepository repository;
    POINTERS_EQUAL(NULL, repository.getComparatorForType("typeName"));
}

TEST(MockNamedValueHandlerRepository, installComparator)
{
    TypeForTestingExpectedFunctionCallComparator comparator;
    MockNamedValueComparatorsAndCopiersRepository repository;
    repository.installComparator("typeName", comparator);
    POINTERS_EQUAL(&comparator, repository.getComparatorForType("typeName"));
}

TEST(MockNamedValueHandlerRepository, installMultipleComparators)
{
    TypeForTestingExpectedFunctionCallComparator comparator1, comparator2, comparator3;
    MockNamedValueComparatorsAndCopiersRepository repository;
    repository.installComparator("type1", comparator1);
    repository.installComparator("type2", comparator2);
    repository.installComparator("type3", comparator3);
    POINTERS_EQUAL(&comparator3, repository.getComparatorForType("type3"));
    POINTERS_EQUAL(&comparator2, repository.getComparatorForType("type2"));
    POINTERS_EQUAL(&comparator1, repository.getComparatorForType("type1"));
}

TEST(MockNamedValueHandlerRepository, getCopierForNonExistingName)
{
    MockNamedValueComparatorsAndCopiersRepository repository;
    POINTERS_EQUAL(NULL, repository.getCopierForType("typeName"));
}

TEST(MockNamedValueHandlerRepository, installCopier)
{
    TypeForTestingExpectedFunctionCallCopier copier;
    MockNamedValueComparatorsAndCopiersRepository repository;
    repository.installCopier("typeName", copier);
    POINTERS_EQUAL(&copier, repository.getCopierForType("typeName"));
}

TEST(MockNamedValueHandlerRepository, installMultipleCopiers)
{
    TypeForTestingExpectedFunctionCallCopier copier1, copier2, copier3;
    MockNamedValueComparatorsAndCopiersRepository repository;
    repository.installCopier("type1", copier1);
    repository.installCopier("type2", copier2);
    repository.installCopier("type3", copier3);
    POINTERS_EQUAL(&copier3, repository.getCopierForType("type3"));
    POINTERS_EQUAL(&copier2, repository.getCopierForType("type2"));
    POINTERS_EQUAL(&copier1, repository.getCopierForType("type1"));
}

TEST(MockNamedValueHandlerRepository, installMultipleHandlers)
{
    TypeForTestingExpectedFunctionCallCopier copier1, copier2, copier3;
    TypeForTestingExpectedFunctionCallComparator comparator1, comparator2, comparator3;
    MockNamedValueComparatorsAndCopiersRepository repository;
    repository.installCopier("type1", copier1);
    repository.installComparator("type1", comparator1);
    repository.installCopier("type2", copier2);
    repository.installCopier("type3", copier3);
    repository.installComparator("type2", comparator2);
    repository.installComparator("type3", comparator3);
    POINTERS_EQUAL(&comparator3, repository.getComparatorForType("type3"));
    POINTERS_EQUAL(&comparator2, repository.getComparatorForType("type2"));
    POINTERS_EQUAL(&comparator1, repository.getComparatorForType("type1"));
    POINTERS_EQUAL(&copier3, repository.getCopierForType("type3"));
    POINTERS_EQUAL(&copier2, repository.getCopierForType("type2"));
    POINTERS_EQUAL(&copier1, repository.getCopierForType("type1"));
}

TEST_GROUP(MockExpectedCall)
{
    MockCheckedExpectedCall* call;
    void setup()
    {
        call = new MockCheckedExpectedCall(1, 1);
        call->withName("funcName");
    }
    void teardown()
    {
        delete call;
        CHECK_NO_MOCK_FAILURE();
    }
};

TEST(MockExpectedCall, callWithoutParameterSetOrNotFound)
{
    STRCMP_EQUAL("", call->getInputParameterType("nonexisting").asCharString());
    LONGS_EQUAL(0, call->getInputParameter("nonexisting").getIntValue());
    CHECK(!call->hasInputParameterWithName("nonexisting"));
}

TEST(MockExpectedCall, callWithUnsignedIntegerParameter)
{
    const SimpleString paramName = "paramName";
    unsigned int value = 356;
    call->withParameter(paramName, value);
    STRCMP_EQUAL("unsigned int", call->getInputParameterType(paramName).asCharString());
    LONGS_EQUAL(value, call->getInputParameter(paramName).getUnsignedIntValue());
    CHECK(call->hasInputParameterWithName(paramName));
    STRCMP_EQUAL("funcName -> unsigned int paramName: <356 (0x164)>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithIntegerParameter)
{
    const SimpleString paramName = "paramName";
    int value = 2;
    call->withParameter(paramName, value);
    STRCMP_EQUAL("int", call->getInputParameterType(paramName).asCharString());
    LONGS_EQUAL(value, call->getInputParameter(paramName).getIntValue());
    CHECK(call->hasInputParameterWithName(paramName));
    STRCMP_EQUAL("funcName -> int paramName: <2 (0x2)>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithBooleanParameter)
{
    const SimpleString paramName = "paramName";
    bool value = true;
    call->withParameter(paramName, value);
    STRCMP_EQUAL("bool", call->getInputParameterType(paramName).asCharString());
    CHECK_EQUAL(value, call->getInputParameter(paramName).getBoolValue());
    CHECK(call->hasInputParameterWithName(paramName));
    STRCMP_EQUAL("funcName -> bool paramName: <true>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithUnsignedLongIntegerParameter)
{
    const SimpleString paramName = "paramName";
    unsigned long value = 888;
    call->withParameter(paramName, value);
    STRCMP_EQUAL("unsigned long int", call->getInputParameterType(paramName).asCharString());
    LONGS_EQUAL(value, call->getInputParameter(paramName).getUnsignedLongIntValue());
    CHECK(call->hasInputParameterWithName(paramName));
    STRCMP_EQUAL("funcName -> unsigned long int paramName: <888 (0x378)>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithLongIntegerParameter)
{
    const SimpleString paramName = "paramName";
    long value = 777;
    call->withParameter(paramName, value);
    STRCMP_EQUAL("long int", call->getInputParameterType(paramName).asCharString());
    LONGS_EQUAL(value, call->getInputParameter(paramName).getLongIntValue());
    CHECK(call->hasInputParameterWithName(paramName));
    STRCMP_EQUAL("funcName -> long int paramName: <777 (0x309)>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithDoubleParameter)
{
    const SimpleString paramName = "paramName";
    double value = 1.2;
    call->withParameter(paramName, value);
    STRCMP_EQUAL("double", call->getInputParameterType(paramName).asCharString());
    DOUBLES_EQUAL(value, call->getInputParameter(paramName).getDoubleValue(), 0.05);
    STRCMP_EQUAL("funcName -> double paramName: <1.2>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithStringParameter)
{
    const SimpleString paramName = "paramName";
    const char* value = "hello world";
    call->withParameter(paramName, value);
    STRCMP_EQUAL("const char*", call->getInputParameterType(paramName).asCharString());
    STRCMP_EQUAL(value, call->getInputParameter(paramName).getStringValue());
    STRCMP_EQUAL("funcName -> const char* paramName: <hello world>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithPointerParameter)
{
    const SimpleString paramName = "paramName";
    void* value = (void*) 0x123;
    call->withParameter(paramName, value);
    STRCMP_EQUAL("void*", call->getInputParameterType(paramName).asCharString());
    POINTERS_EQUAL(value, call->getInputParameter(paramName).getPointerValue());
    STRCMP_EQUAL("funcName -> void* paramName: <0x123>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithConstPointerParameter)
{
    const SimpleString paramName = "paramName";
    const void* value = (const void*) 0x345;
    call->withParameter(paramName, value);
    STRCMP_EQUAL("const void*", call->getInputParameterType(paramName).asCharString());
    POINTERS_EQUAL(value, call->getInputParameter(paramName).getConstPointerValue());
    STRCMP_EQUAL("funcName -> const void* paramName: <0x345>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithFunctionPointerParameter)
{
    const SimpleString paramName = "paramName";
    void (*value)() = (void (*)()) 0xdead;
    call->withParameter(paramName, value);
    STRCMP_EQUAL("void (*)()", call->getInputParameterType(paramName).asCharString());
    FUNCTIONPOINTERS_EQUAL(value, call->getInputParameter(paramName).getFunctionPointerValue());
    STRCMP_EQUAL("funcName -> void (*)() paramName: <0xdead>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithMemoryBuffer)
{
    const SimpleString paramName = "paramName";
    const unsigned char value[] = { 0x12, 0xFE, 0xA1 };
    call->withParameter(paramName, value, sizeof(value));
    STRCMP_EQUAL("const unsigned char*", call->getInputParameterType(paramName).asCharString());
    POINTERS_EQUAL( (void*) value, (void*) call->getInputParameter(paramName).getMemoryBuffer() );
    LONGS_EQUAL(sizeof(value),  call->getInputParameter(paramName).getSize());
    STRCMP_EQUAL("funcName -> const unsigned char* paramName: <Size = 3 | HexContents = 12 FE A1>", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithObjectParameter)
{
    const SimpleString paramName = "paramName";
    void* value = (void*) 0x123;
    call->withParameterOfType("ClassName", paramName, value);
    POINTERS_EQUAL(value, call->getInputParameter(paramName).getObjectPointer());
    STRCMP_EQUAL("ClassName", call->getInputParameterType(paramName).asCharString());
    STRCMP_EQUAL("funcName -> ClassName paramName: <No comparator found for type: \"ClassName\">", call->callToString(true).asCharString());
}

TEST(MockExpectedCall, callWithObjectParameterUnequalComparison)
{
    TypeForTestingExpectedFunctionCall type(1), unequalType(2);
    MockNamedValue parameter("name");
    parameter.setObjectPointer("type", &unequalType);
    call->withParameterOfType("type", "name", &type);
    CHECK(!call->hasInputParameter(parameter));
}

TEST(MockExpectedCall, callWithObjectParameterEqualComparisonButFailsWithoutRepository)
{
    TypeForTestingExpectedFunctionCall type(1), equalType(1);
    MockNamedValue parameter("name");
    parameter.setObjectPointer("type", &equalType);
    call->withParameterOfType("type", "name", &type);
    CHECK(!call->hasInputParameter(parameter));
}

TEST(MockExpectedCall, callWithObjectParameterEqualComparisonButFailsWithoutComparator)
{
    MockNamedValueComparatorsAndCopiersRepository repository;
    MockNamedValue::setDefaultComparatorsAndCopiersRepository(&repository);

    TypeForTestingExpectedFunctionCall type(1), equalType(1);
    MockNamedValue parameter("name");
    parameter.setObjectPointer("type", &equalType);
    call->withParameterOfType("type", "name", &type);
    CHECK(!call->hasInputParameter(parameter));
}

TEST(MockExpectedCall, callWithObjectParameterEqualComparison)
{
    TypeForTestingExpectedFunctionCallComparator comparator;
    MockNamedValueComparatorsAndCopiersRepository repository;
    MockNamedValue::setDefaultComparatorsAndCopiersRepository(&repository);
    repository.installComparator("type", comparator);

    TypeForTestingExpectedFunctionCall type(1), equalType(1);
    MockNamedValue parameter("name");
    parameter.setObjectPointer("type", &equalType);

    call->withParameterOfType("type", "name", &type);
    CHECK(call->hasInputParameter(parameter));
}

TEST(MockExpectedCall, getParameterValueOfObjectType)
{
    TypeForTestingExpectedFunctionCallComparator comparator;
    MockNamedValueComparatorsAndCopiersRepository repository;
    MockNamedValue::setDefaultComparatorsAndCopiersRepository(&repository);
    repository.installComparator("type", comparator);

    TypeForTestingExpectedFunctionCall type(1);
    call->withParameterOfType("type", "name", &type);
    POINTERS_EQUAL(&type, call->getInputParameter("name").getObjectPointer());
    STRCMP_EQUAL("1", call->getInputParameterValueString("name").asCharString());
}

TEST(MockExpectedCall, getParameterValueOfObjectTypeWithoutRepository)
{
    TypeForTestingExpectedFunctionCall type(1);
    call->withParameterOfType("type", "name", &type);
    STRCMP_EQUAL("No comparator found for type: \"type\"", call->getInputParameterValueString("name").asCharString());
}

TEST(MockExpectedCall, getParameterValueOfObjectTypeWithoutComparator)
{
    TypeForTestingExpectedFunctionCall type(1);
    MockNamedValueComparatorsAndCopiersRepository repository;
    MockNamedValue::setDefaultComparatorsAndCopiersRepository(&repository);
    call->withParameterOfType("type", "name", &type);
    STRCMP_EQUAL("No comparator found for type: \"type\"", call->getInputParameterValueString("name").asCharString());
}

TEST(MockExpectedCall, callWithTwoUnsignedIntegerParameter)
{
    unsigned int expected_value = 1;
    unsigned int another_expected_value = 2;

    call->withParameter("unsigned-integer1", expected_value);
    call->withParameter("unsigned-integer2", another_expected_value);
    STRCMP_EQUAL("unsigned int", call->getInputParameterType("unsigned-integer1").asCharString());
    STRCMP_EQUAL("unsigned int", call->getInputParameterType("unsigned-integer2").asCharString());
    LONGS_EQUAL(expected_value, call->getInputParameter("unsigned-integer1").getUnsignedIntValue());
    LONGS_EQUAL(another_expected_value, call->getInputParameter("unsigned-integer2").getUnsignedIntValue());
}

TEST(MockExpectedCall, callWithTwoIntegerParameter)
{
    int expected_value = 1;
    int another_expected_value = -1;

    call->withParameter("integer1", expected_value);
    call->withParameter("integer2", another_expected_value);
    STRCMP_EQUAL("int", call->getInputParameterType("integer1").asCharString());
    STRCMP_EQUAL("int", call->getInputParameterType("integer2").asCharString());
    LONGS_EQUAL(expected_value, call->getInputParameter("integer1").getIntValue());
    LONGS_EQUAL(another_expected_value, call->getInputParameter("integer2").getIntValue());
}

TEST(MockExpectedCall, callWithThreeDifferentParameter)
{
    call->withParameter("integer", 1);
    call->withParameter("string", "hello world");
    call->withParameter("double", 0.12);
    STRCMP_EQUAL("int", call->getInputParameterType("integer").asCharString());
    STRCMP_EQUAL("const char*", call->getInputParameterType("string").asCharString());
    STRCMP_EQUAL("double", call->getInputParameterType("double").asCharString());
    LONGS_EQUAL(1, call->getInputParameter("integer").getIntValue());
    STRCMP_EQUAL("hello world", call->getInputParameter("string").getStringValue());
    DOUBLES_EQUAL(0.12, call->getInputParameter("double").getDoubleValue(), 0.05);
}

TEST(MockExpectedCall, singleCallNotMadeIsNotFulfilledButCanMatchActualCalls)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withName("name");
    CHECK(!expectedCall.isFulfilled());
    CHECK(expectedCall.canMatchActualCalls());
}

TEST(MockExpectedCall, singleCallMadeIsFulFilledAndCannotMatchActualCalls)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.callWasMade(1);
    CHECK(expectedCall.isFulfilled());
    CHECK(!expectedCall.canMatchActualCalls());
}

TEST(MockExpectedCall, multiCallNotMadeIsNotFulfilledButCanMatchActualCalls)
{
    MockCheckedExpectedCall expectedCall(2, 3);
    expectedCall.withName("name");
    CHECK(!expectedCall.isFulfilled());
    CHECK(expectedCall.canMatchActualCalls());
}

TEST(MockExpectedCall, multiCallNotMadeMinimumTimesIsNotFulfilledButCanMatchActualCalls)
{
    MockCheckedExpectedCall expectedCall(2, 3);
    expectedCall.withName("name");
    expectedCall.callWasMade(1);
    CHECK(!expectedCall.isFulfilled());
    CHECK(expectedCall.canMatchActualCalls());
}

TEST(MockExpectedCall, multiCallMadeMinimumTimesIsFulfilledAndCanMatchActualCalls)
{
    MockCheckedExpectedCall expectedCall(2, 3);
    expectedCall.withName("name");
    expectedCall.callWasMade(1);
    expectedCall.callWasMade(2);
    CHECK(expectedCall.isFulfilled());
    CHECK(expectedCall.canMatchActualCalls());
}

TEST(MockExpectedCall, multiCallsMadeMaximumTimesIsFulfilledAndCannotMatchActualCalls)
{
    MockCheckedExpectedCall expectedCall(2, 3);
    expectedCall.withName("name");
    expectedCall.callWasMade(1);
    expectedCall.callWasMade(2);
    expectedCall.callWasMade(3);
    CHECK(expectedCall.isFulfilled());
    CHECK(!expectedCall.canMatchActualCalls());
}

TEST(MockExpectedCall, multiCallsMadeMoreThanMaximumTimesIsNotFulfilledAndCannotMatchActualCalls)
{
    MockCheckedExpectedCall expectedCall(2, 3);
    expectedCall.withName("name");
    expectedCall.callWasMade(1);
    expectedCall.callWasMade(2);
    expectedCall.callWasMade(3);
    expectedCall.callWasMade(4);
    CHECK(!expectedCall.isFulfilled());
    CHECK(!expectedCall.canMatchActualCalls());
}

TEST(MockExpectedCall, callsWithoutParameterAlwaysMatch)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    CHECK(expectedCall.isMatchingActualCall());
}

TEST(MockExpectedCall, callsWithParameterNotFulfilledDontMatch)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withParameter("para", 1);
    CHECK(!expectedCall.isMatchingActualCall());
}

TEST(MockExpectedCall, callsWithParameterFulfilledDoMatch)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withParameter("para", 1);
    expectedCall.inputParameterWasPassed("para");
    CHECK(expectedCall.isMatchingActualCall());
}

TEST(MockExpectedCall, callsWithSomeParametersNotFulfilledDontMatch)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withParameter("para", 1).withParameter("two", 2);
    expectedCall.inputParameterWasPassed("para");
    CHECK(!expectedCall.isMatchingActualCall());
}

TEST(MockExpectedCall, toStringForNoParametersSingleCallNotCalled)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withName("name");
    STRCMP_EQUAL("name -> no parameters (expected 1 call, but was called 0 times)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, toStringForNoParametersMultiCallCalledLessThanMinimum)
{
    MockCheckedExpectedCall expectedCall(2, 2);
    expectedCall.withName("name");
    expectedCall.callWasMade(1);
    STRCMP_EQUAL("name -> no parameters (expected 2 calls, but was called 1 time)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, toStringForNoParametersMultiCallCalledMinimum)
{
    MockCheckedExpectedCall expectedCall(2, 3);
    expectedCall.withName("name");
    expectedCall.callWasMade(1);
    expectedCall.callWasMade(2);
    STRCMP_EQUAL("name -> no parameters (called 2 times)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, toStringForIgnoredParameters)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withName("name");
    expectedCall.ignoreOtherParameters();
    STRCMP_EQUAL("name -> all parameters ignored (expected 1 call, but was called 0 times)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, toStringForMultipleInputParameters)
{
    int int_value = 10;
    unsigned int uint_value = 7;

    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withName("name");
    expectedCall.withParameter("string", "value");
    expectedCall.withParameter("integer", int_value);
    expectedCall.withParameter("unsigned-integer", uint_value);
    expectedCall.callWasMade(1);
    STRCMP_EQUAL("name -> const char* string: <value>, int integer: <10 (0xa)>, unsigned int unsigned-integer: <7 (0x7)> "
                 "(called 1 time)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, toStringForMultipleInputAndOutputParameters)
{
    int int_value = 10;
    unsigned int uint_value = 7;
    unsigned char buffer_value[3];

    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withName("name");
    expectedCall.withParameter("string", "value");
    expectedCall.withParameter("integer", int_value);
    expectedCall.withParameter("unsigned-integer", uint_value);
    expectedCall.withOutputParameterReturning("buffer", buffer_value, sizeof(buffer_value));
    expectedCall.callWasMade(1);
    STRCMP_EQUAL("name -> const char* string: <value>, int integer: <10 (0xa)>, unsigned int unsigned-integer: <7 (0x7)>, "
                 "const void* buffer: <output> (called 1 time)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, toStringForMultipleOutputParameters)
{
    unsigned char buffer_value[3];

    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withName("name");
    expectedCall.withOutputParameterReturning("buffer1", buffer_value, sizeof(buffer_value));
    expectedCall.withOutputParameterReturning("buffer2", buffer_value, sizeof(buffer_value));
    expectedCall.callWasMade(1);
    STRCMP_EQUAL("name -> const void* buffer1: <output>, const void* buffer2: <output> (called 1 time)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, toStringForParameterAndIgnored)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withName("name");
    expectedCall.withParameter("string", "value");
    expectedCall.ignoreOtherParameters();
    expectedCall.callWasMade(1);
    STRCMP_EQUAL("name -> const char* string: <value>, other parameters are ignored (called 1 time)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, toStringForCallOrderSingle)
{
    MockCheckedExpectedCall expectedCall(1, 1);
    expectedCall.withName("name");
    expectedCall.withCallOrder(2, 2);
    expectedCall.callWasMade(1);
    STRCMP_EQUAL("name -> expected call order: <2> -> no parameters (called 1 time)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, toStringForCallOrderMultiple)
{
    MockCheckedExpectedCall expectedCall(5, 5);
    expectedCall.withName("name");
    expectedCall.withCallOrder(5, 9);
    expectedCall.callWasMade(5);
    expectedCall.callWasMade(6);
    expectedCall.callWasMade(7);
    expectedCall.callWasMade(8);
    expectedCall.callWasMade(9);
    STRCMP_EQUAL("name -> expected calls order: <5..9> -> no parameters (called 5 times)", expectedCall.callToString(false).asCharString());
}

TEST(MockExpectedCall, callOrderIsFulfilledButWithWrongOrderSingle)
{
    call->withName("name");
    call->withCallOrder(2, 2);
    call->callWasMade(1);
    CHECK(call->isFulfilled());
    CHECK(call->isOutOfOrder());
}

TEST(MockExpectedCall, callOrderIsFulfilledButWithWrongOrderMultipleTooEarly)
{
    MockCheckedExpectedCall expectedCall(3, 3);
    expectedCall.withName("name");
    expectedCall.withCallOrder(10, 12);
    expectedCall.callWasMade(9);
    expectedCall.callWasMade(10);
    expectedCall.callWasMade(11);
    CHECK(expectedCall.isFulfilled());
    CHECK(expectedCall.isOutOfOrder());
}

TEST(MockExpectedCall, callOrderIsFulfilledButWithWrongOrderMultipleTooLate)
{
    MockCheckedExpectedCall expectedCall(3, 3);
    expectedCall.withName("name");
    expectedCall.withCallOrder(10, 12);
    expectedCall.callWasMade(11);
    expectedCall.callWasMade(12);
    expectedCall.callWasMade(13);
    CHECK(expectedCall.isFulfilled());
    CHECK(expectedCall.isOutOfOrder());
}

TEST(MockExpectedCall, callOrderIsFulfilledSingle)
{
    call->withName("name");
    call->withCallOrder(1, 1);
    call->callWasMade(1);
    CHECK(call->isFulfilled());
    CHECK_FALSE(call->isOutOfOrder());
}

TEST(MockExpectedCall, callOrderIsFulfilledMultiple)
{
    MockCheckedExpectedCall expectedCall(4, 4);
    expectedCall.withName("name");
    expectedCall.withCallOrder(150, 153);
    expectedCall.callWasMade(150);
    expectedCall.callWasMade(151);
    expectedCall.callWasMade(152);
    expectedCall.callWasMade(153);
    CHECK(expectedCall.isFulfilled());
    CHECK_FALSE(expectedCall.isOutOfOrder());
}

TEST(MockExpectedCall, hasOutputParameter)
{
    const int value = 1;
    call->withOutputParameterReturning("foo", &value, sizeof(value));
    MockNamedValue foo("foo");
    foo.setValue(&value);
    CHECK(call->hasOutputParameter(foo));
}

TEST(MockExpectedCall, hasNoOutputParameter)
{
    call->withIntParameter("foo", (int)1);
    MockNamedValue foo("foo");
    foo.setValue((int)1);
    CHECK_FALSE(call->hasOutputParameter(foo));
}

TEST(MockExpectedCall, hasOutputParameterOfType)
{
    TypeForTestingExpectedFunctionCall object(6789);
    call->withOutputParameterOfTypeReturning("TypeForTestingExpectedFunctionCall", "foo", &object);
    MockNamedValue foo("foo");
    foo.setObjectPointer("TypeForTestingExpectedFunctionCall", &object);
    CHECK(call->hasOutputParameter(foo));
}

TEST(MockExpectedCall, hasNoOutputParameterOfTypeSameTypeButInput)
{
    TypeForTestingExpectedFunctionCall object(543);
    call->withParameterOfType("TypeForTestingExpectedFunctionCall", "foo", &object);
    MockNamedValue foo("foo");
    foo.setObjectPointer("TypeForTestingExpectedFunctionCall", &object);
    CHECK_FALSE(call->hasOutputParameter(foo));
}

TEST(MockExpectedCall, hasNoOutputParameterOfTypeDifferentType)
{
    TypeForTestingExpectedFunctionCall object(543);
    call->withOutputParameterOfTypeReturning("TypeForTestingExpectedFunctionCall", "foo", &object);
    MockNamedValue foo("foo");
    foo.setObjectPointer("OtherTypeForTestingExpectedFunctionCall", &object);
    CHECK_FALSE(call->hasOutputParameter(foo));
}

TEST_GROUP(MockIgnoredExpectedCall)
{
    MockIgnoredExpectedCall ignored;
};

TEST(MockIgnoredExpectedCall, worksAsItShould)
{
    ignored.withName("func");
    ignored.withCallOrder(1, 1);
    ignored.onObject((void*) 0);
    ignored.withBoolParameter("umm", true);
    ignored.withIntParameter("bla", (int) 1);
    ignored.withUnsignedIntParameter("foo", (unsigned int) 1);
    ignored.withLongIntParameter("hey", (long int) 1);
    ignored.withUnsignedLongIntParameter("bah", (unsigned long int) 1);
    ignored.withDoubleParameter("hah", (double) 1.1f);
    ignored.withStringParameter("goo", "hello");
    ignored.withPointerParameter("pie", (void*) 0);
    ignored.withConstPointerParameter("woo", (const void*) 0);
    ignored.withFunctionPointerParameter("fop", (void(*)()) 0);
    ignored.withMemoryBufferParameter("waa", (const unsigned char*) 0, 0);
    ignored.withParameterOfType( "mytype", "top", (const void*) 0);
    ignored.withOutputParameterReturning("bar", (void*) 0, 1);
    ignored.withOutputParameterOfTypeReturning("mytype", "bar", (const void*) 0);
    ignored.ignoreOtherParameters();
    ignored.andReturnValue(true);
    ignored.andReturnValue((double) 1.0f);
    ignored.andReturnValue((unsigned int) 1);
    ignored.andReturnValue((int) 1);
    ignored.andReturnValue((unsigned long int) 1);
    ignored.andReturnValue((long int) 1);
    ignored.andReturnValue("boo");
    ignored.andReturnValue((void*) 0);
    ignored.andReturnValue((const void*) 0);
    ignored.andReturnValue((void(*)()) 0);
}
