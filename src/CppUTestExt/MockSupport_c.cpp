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

#include "CppUTest/CppUTestConfig.h"
#include "CppUTest/Utest.h"
#include "CppUTest/UtestMacros.h"
#include "CppUTest/PlatformSpecificFunctions_c.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupport_c.h"

class MockFailureReporterTestTerminatorForInCOnlyCode : public TestTerminatorWithoutExceptions
{
public:
	MockFailureReporterTestTerminatorForInCOnlyCode(bool crashOnFailure) : crashOnFailure_(crashOnFailure)
	{
	}

	virtual void exitCurrentTest() const _override
	{
		if (crashOnFailure_)
			UT_CRASH();

		TestTerminatorWithoutExceptions::exitCurrentTest();
	}

	virtual ~MockFailureReporterTestTerminatorForInCOnlyCode()
	{
	}
private:
	bool crashOnFailure_;

};

class MockFailureReporterForInCOnlyCode : public MockFailureReporter
{
public:
	void failTest(const MockFailure& failure) _override
	{
		if (!getTestToFail()->hasFailed())
			getTestToFail()->failWith(failure, MockFailureReporterTestTerminatorForInCOnlyCode(crashOnFailure_));
	}

};

static MockSupport* currentMockSupport = NULL;
static MockExpectedCall* expectedCall = NULL;
static MockActualCall* actualCall = NULL;
static MockFailureReporterForInCOnlyCode failureReporterForC;

class MockCFunctionComparatorNode : public MockNamedValueComparator
{
public:
	MockCFunctionComparatorNode(MockCFunctionComparatorNode* next, MockTypeEqualFunction_c equal, MockTypeValueToStringFunction_c toString)
		: next_(next), equal_(equal), toString_(toString) {}
	virtual ~MockCFunctionComparatorNode() {}

	virtual bool isEqual(const void* object1, const void* object2) _override
	{
		return equal_(object1, object2) != 0;
	}
	virtual SimpleString valueToString(const void* object) _override
	{
		return SimpleString(toString_(object));
	}

	MockCFunctionComparatorNode* next_;
	MockTypeEqualFunction_c equal_;
	MockTypeValueToStringFunction_c toString_;
};

static MockCFunctionComparatorNode* comparatorList_ = NULL;

extern "C" {

MockExpectedCall_c* expectOneCall_c(const char* name);
MockActualCall_c* actualCall_c(const char* name);
void disable_c(void);
void enable_c(void);
void setIntData_c(const char* name, int value);
void setDoubleData_c(const char* name, double value);
void setStringData_c(const char* name, const char* value);
void setPointerData_c(const char* name, void* value);
void setConstPointerData_c(const char* name, const void* value);
void setDataObject_c(const char* name, const char* type, void* value);
MockValue_c getData_c(const char* name);

void checkExpectations_c();
int expectedCallsLeft_c();
void clear_c();

MockExpectedCall_c* withIntParameters_c(const char* name, int value);
MockExpectedCall_c* withUnsignedIntParameters_c(const char* name, unsigned int value);
MockExpectedCall_c* withLongIntParameters_c(const char* name, long int value);
MockExpectedCall_c* withUnsignedLongIntParameters_c(const char* name, unsigned long int value);
MockExpectedCall_c* withDoubleParameters_c(const char* name, double value);
MockExpectedCall_c* withStringParameters_c(const char* name, const char* value);
MockExpectedCall_c* withPointerParameters_c(const char* name, void* value);
MockExpectedCall_c* withConstPointerParameters_c(const char* name, const void* value);
MockExpectedCall_c* withParameterOfType_c(const char* type, const char* name, const void* value);
MockExpectedCall_c* andReturnIntValue_c(int value);
MockExpectedCall_c* andReturnUnsignedIntValue_c(unsigned int value);
MockExpectedCall_c* andReturnDoubleValue_c(double value);
MockExpectedCall_c* andReturnStringValue_c(const char* value);
MockExpectedCall_c* andReturnPointerValue_c(void* value);
MockExpectedCall_c* andReturnConstPointerValue_c(const void* value);
MockValue_c expectedReturnValue_c();

MockActualCall_c* withActualIntParameters_c(const char* name, int value);
MockActualCall_c* withActualUnsignedIntParameters_c(const char* name, unsigned int value);
MockActualCall_c* withActualLongIntParameters_c(const char* name, long int value);
MockActualCall_c* withActualUnsignedLongIntParameters_c(const char* name, unsigned long int value);
MockActualCall_c* withActualDoubleParameters_c(const char* name, double value);
MockActualCall_c* withActualStringParameters_c(const char* name, const char* value);
MockActualCall_c* withActualPointerParameters_c(const char* name, void* value);
MockActualCall_c* withActualConstPointerParameters_c(const char* name, const void* value);
MockActualCall_c* withActualParameterOfType_c(const char* type, const char* name, const void* value);
MockValue_c actualReturnValue_c();


static void installComparator_c (const char* typeName, MockTypeEqualFunction_c isEqual, MockTypeValueToStringFunction_c valueToString)
{
	comparatorList_ = new MockCFunctionComparatorNode(comparatorList_, isEqual, valueToString);
	currentMockSupport->installComparator(typeName, *comparatorList_);
}

static void removeAllComparators_c()
{
	while (comparatorList_) {
		MockCFunctionComparatorNode *next = comparatorList_->next_;
		delete comparatorList_;
		comparatorList_ = next;
	}
	currentMockSupport->removeAllComparators();
}

static MockExpectedCall_c gExpectedCall = {
		withIntParameters_c,
		withUnsignedIntParameters_c,
		withLongIntParameters_c,
		withUnsignedLongIntParameters_c,
		withDoubleParameters_c,
		withStringParameters_c,
		withPointerParameters_c,
		withConstPointerParameters_c,
		withParameterOfType_c,
		andReturnUnsignedIntValue_c,
		andReturnIntValue_c,
		andReturnDoubleValue_c,
		andReturnStringValue_c,
		andReturnPointerValue_c,
		andReturnConstPointerValue_c,
};

static MockActualCall_c gActualCall = {
		withActualIntParameters_c,
		withActualUnsignedIntParameters_c,
		withActualLongIntParameters_c,
		withActualUnsignedLongIntParameters_c,
		withActualDoubleParameters_c,
		withActualStringParameters_c,
		withActualPointerParameters_c,
		withActualConstPointerParameters_c,
		withActualParameterOfType_c,
		actualReturnValue_c
};

static MockSupport_c gMockSupport = {
		expectOneCall_c,
		actualCall_c,
		actualReturnValue_c,
		enable_c,
		disable_c,
		setIntData_c,
		setDoubleData_c,
		setStringData_c,
		setPointerData_c,
		setConstPointerData_c,
		setDataObject_c,
		getData_c,
		checkExpectations_c,
		expectedCallsLeft_c,
		clear_c,
		installComparator_c,
		removeAllComparators_c
};

MockExpectedCall_c* withIntParameters_c(const char* name, int value)
{
	expectedCall = &expectedCall->withParameter(name, value);
	return &gExpectedCall;
}

MockExpectedCall_c* withUnsignedIntParameters_c(const char* name, unsigned int value)
{
	expectedCall = &expectedCall->withParameter(name, value);
	return &gExpectedCall;
}

MockExpectedCall_c* withLongIntParameters_c(const char* name, long int value)
{
	expectedCall = &expectedCall->withParameter(name, value);
	return &gExpectedCall;
}

MockExpectedCall_c* withUnsignedLongIntParameters_c(const char* name, unsigned long int value)
{
	expectedCall = &expectedCall->withParameter(name, value);
	return &gExpectedCall;
}

MockExpectedCall_c* withDoubleParameters_c(const char* name, double value)
{
	expectedCall = &expectedCall->withParameter(name, value);
	return &gExpectedCall;
}

MockExpectedCall_c* withStringParameters_c(const char* name, const char* value)
{
	expectedCall = &expectedCall->withParameter(name, value);
	return &gExpectedCall;
}

MockExpectedCall_c* withPointerParameters_c(const char* name, void* value)
{
	expectedCall = &expectedCall->withParameter(name, value);
	return &gExpectedCall;
}

MockExpectedCall_c* withConstPointerParameters_c(const char* name, const void* value)
{
	expectedCall = &expectedCall->withParameter(name, value);
	return &gExpectedCall;
}

MockExpectedCall_c* withParameterOfType_c(const char* type, const char* name, const void* value)
{
	expectedCall = &expectedCall->withParameterOfType(type, name, value);
	return &gExpectedCall;
}

MockExpectedCall_c* andReturnUnsignedIntValue_c(unsigned int value)
{
	expectedCall = &expectedCall->andReturnValue(value);
	return &gExpectedCall;
}

MockExpectedCall_c* andReturnIntValue_c(int value)
{
	expectedCall = &expectedCall->andReturnValue(value);
	return &gExpectedCall;
}

MockExpectedCall_c* andReturnDoubleValue_c(double value)
{
	expectedCall = &expectedCall->andReturnValue(value);
	return &gExpectedCall;
}

MockExpectedCall_c* andReturnStringValue_c(const char* value)
{
	expectedCall = &expectedCall->andReturnValue(value);
	return &gExpectedCall;
}

MockExpectedCall_c* andReturnPointerValue_c(void* value)
{
	expectedCall = &expectedCall->andReturnValue(value);
	return &gExpectedCall;
}

MockExpectedCall_c* andReturnConstPointerValue_c(const void* value)
{
	expectedCall = &expectedCall->andReturnValue(value);
	return &gExpectedCall;
}

static MockValue_c getMockValueCFromNamedValue(const MockNamedValue& namedValue)
{
	MockValue_c returnValue;
	if (PlatformSpecificStrCmp(namedValue.getType().asCharString(), "int") == 0) {
		returnValue.type = MOCKVALUETYPE_INTEGER;
		returnValue.value.intValue = namedValue.getIntValue();
	}
	else if (PlatformSpecificStrCmp(namedValue.getType().asCharString(), "unsigned int") == 0) {
		returnValue.type = MOCKVALUETYPE_UNSIGNED_INTEGER;
		returnValue.value.unsignedIntValue = namedValue.getUnsignedIntValue();
	}
	else if (PlatformSpecificStrCmp(namedValue.getType().asCharString(), "double") == 0) {
		returnValue.type = MOCKVALUETYPE_DOUBLE;
		returnValue.value.doubleValue = namedValue.getDoubleValue();
	}
	else if (PlatformSpecificStrCmp(namedValue.getType().asCharString(), "const char*") == 0) {
		returnValue.type = MOCKVALUETYPE_STRING;
		returnValue.value.stringValue = namedValue.getStringValue();
	}
	else if (PlatformSpecificStrCmp(namedValue.getType().asCharString(), "void*") == 0) {
		returnValue.type = MOCKVALUETYPE_POINTER;
		returnValue.value.pointerValue = namedValue.getPointerValue();
	}
	else if (PlatformSpecificStrCmp(namedValue.getType().asCharString(), "const void*") == 0) {
		returnValue.type = MOCKVALUETYPE_CONST_POINTER;
		returnValue.value.constPointerValue = namedValue.getConstPointerValue();
	}
	else {
		returnValue.type = MOCKVALUETYPE_OBJECT;
		returnValue.value.objectValue = namedValue.getObjectPointer();
	}
	return returnValue;
}

MockExpectedCall_c* expectOneCall_c(const char* name)
{
	expectedCall = &currentMockSupport->expectOneCall(name);
	return &gExpectedCall;
}

MockActualCall_c* actualCall_c(const char* name)
{
	actualCall = &currentMockSupport->actualCall(name);
	return &gActualCall;
}

MockActualCall_c* withActualIntParameters_c(const char* name, int value)
{
	actualCall = &actualCall->withParameter(name, value);
	return &gActualCall;
}

MockActualCall_c* withActualUnsignedIntParameters_c(const char* name, unsigned int value)
{
	actualCall = &actualCall->withParameter(name, value);
	return &gActualCall;
}

MockActualCall_c* withActualLongIntParameters_c(const char* name, long int value)
{
	actualCall = &actualCall->withParameter(name, value);
	return &gActualCall;
}

MockActualCall_c* withActualUnsignedLongIntParameters_c(const char* name, unsigned long int value)
{
	actualCall = &actualCall->withParameter(name, value);
	return &gActualCall;
}

MockActualCall_c* withActualDoubleParameters_c(const char* name, double value)
{
	actualCall = &actualCall->withParameter(name, value);
	return &gActualCall;
}

MockActualCall_c* withActualStringParameters_c(const char* name, const char* value)
{
	actualCall = &actualCall->withParameter(name, value);
	return &gActualCall;
}

MockActualCall_c* withActualPointerParameters_c(const char* name, void* value)
{
	actualCall = &actualCall->withParameter(name, value);
	return &gActualCall;
}

MockActualCall_c* withActualConstPointerParameters_c(const char* name, const void* value)
{
	actualCall = &actualCall->withParameter(name, value);
	return &gActualCall;
}

MockActualCall_c* withActualParameterOfType_c(const char* type, const char* name, const void* value)
{
	actualCall = &actualCall->withParameterOfType(type, name, value);
	return &gActualCall;
}

MockValue_c actualReturnValue_c()
{
	return getMockValueCFromNamedValue(actualCall->returnValue());
}

void disable_c(void)
{
	currentMockSupport->disable();
}

void enable_c(void)
{
	currentMockSupport->enable();
}

void setIntData_c(const char* name, int value)
{
	return currentMockSupport->setData(name, value);
}

void setDoubleData_c(const char* name, double value)
{
	return currentMockSupport->setData(name, value);
}

void setStringData_c(const char* name, const char* value)
{
	return currentMockSupport->setData(name, value);
}

void setPointerData_c(const char* name, void* value)
{
	return currentMockSupport->setData(name, value);
}

void setConstPointerData_c(const char* name, const void* value)
{
	return currentMockSupport->setData(name, value);
}

void setDataObject_c(const char* name, const char* type, void* value)
{
	return currentMockSupport->setDataObject(name, type, value);
}

MockValue_c getData_c(const char* name)
{
	return getMockValueCFromNamedValue(currentMockSupport->getData(name));
}

void checkExpectations_c()
{
	currentMockSupport->checkExpectations();
}

int expectedCallsLeft_c()
{
	return currentMockSupport->expectedCallsLeft();
}

void clear_c()
{
	currentMockSupport->clear();
}

MockSupport_c* mock_c()
{
	currentMockSupport = &mock("", &failureReporterForC);
	return &gMockSupport;
}

MockSupport_c* mock_scope_c(const char* scope)
{
	currentMockSupport = &mock(scope, &failureReporterForC);
	return &gMockSupport;
}

}
