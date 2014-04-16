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
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockActualCall.h"
#include "CppUTestExt/MockExpectedCall.h"
#include "CppUTestExt/MockFailure.h"

#define MOCK_SUPPORT_SCOPE_PREFIX "!!!$$$MockingSupportScope$$$!!!"

static MockSupport global_mock;
int MockSupport::callOrder_ = 0;
int MockSupport::expectedCallOrder_ = 0;

MockSupport& mock(const SimpleString& mockName, MockFailureReporter* failureReporterForThisCall)
{
	MockSupport& mock_support = (mockName != "") ? *global_mock.getMockSupportScope(mockName) : global_mock;
	mock_support.setActiveReporter(failureReporterForThisCall);
	mock_support.setDefaultComparatorRepository();
	return mock_support;
}

MockSupport::MockSupport()
	: strictOrdering_(false), standardReporter_(&defaultReporter_), ignoreOtherCalls_(false), enabled_(true), lastActualFunctionCall_(NULL), tracing_(false)
{
	setActiveReporter(NULL);
}

MockSupport::~MockSupport()
{
}

void MockSupport::crashOnFailure()
{
	activeReporter_->crashOnFailure();
}

void MockSupport::setMockFailureStandardReporter(MockFailureReporter* reporter)
{
	standardReporter_ = (reporter != NULL) ? reporter : &defaultReporter_;

	if (lastActualFunctionCall_)
		lastActualFunctionCall_->setMockFailureReporter(standardReporter_);

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p)) getMockSupport(p)->setMockFailureStandardReporter(standardReporter_);
}

void MockSupport::setActiveReporter(MockFailureReporter* reporter)
{
	activeReporter_ = (reporter) ? reporter : standardReporter_;
}

void MockSupport::setDefaultComparatorRepository()
{
	MockNamedValue::setDefaultComparatorRepository(&comparatorRepository_);
}

void MockSupport::installComparator(const SimpleString& typeName, MockNamedValueComparator& comparator)
{
	comparatorRepository_.installComparator(typeName, comparator);

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p)) getMockSupport(p)->installComparator(typeName, comparator);
}

void MockSupport::installComparators(const MockNamedValueComparatorRepository& repository)
{
	comparatorRepository_.installComparators(repository);

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p)) getMockSupport(p)->installComparators(repository);
}

void MockSupport::removeAllComparators()
{
	comparatorRepository_.clear();
	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p)) getMockSupport(p)->removeAllComparators();
}

void MockSupport::clear()
{
	delete lastActualFunctionCall_;
	lastActualFunctionCall_ = NULL;

	tracing_ = false;
	MockActualCallTrace::instance().clear();

	expectations_.deleteAllExpectationsAndClearList();
	compositeCalls_.clear();
	ignoreOtherCalls_ = false;
	enabled_ = true;
	callOrder_ = 0;
	expectedCallOrder_ = 0;
	strictOrdering_ = false;

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next()) {
		MockSupport* support = getMockSupport(p);
		if (support) {
			support->clear();
			delete support;
		}
	}
	data_.clear();
}

void MockSupport::strictOrder()
{
	strictOrdering_ = true;
}

MockExpectedCall& MockSupport::expectOneCall(const SimpleString& functionName)
{
	if (!enabled_) return MockIgnoredExpectedCall::instance();

	MockCheckedExpectedCall* call = new MockCheckedExpectedCall;
	call->withName(functionName);
	if (strictOrdering_)
		call->withCallOrder(++expectedCallOrder_);
	expectations_.addExpectedCall(call);
	return *call;
}

MockExpectedCall& MockSupport::expectNCalls(int amount, const SimpleString& functionName)
{
	compositeCalls_.clear();

	for (int i = 0; i < amount; i++)
		compositeCalls_.add(expectOneCall(functionName));
	return compositeCalls_;
}

MockCheckedActualCall* MockSupport::createActualFunctionCall()
{
	lastActualFunctionCall_ = new MockCheckedActualCall(++callOrder_, activeReporter_, expectations_);
	return lastActualFunctionCall_;
}

MockActualCall& MockSupport::actualCall(const SimpleString& functionName)
{
	if (lastActualFunctionCall_) {
		lastActualFunctionCall_->checkExpectations();
		delete lastActualFunctionCall_;
		lastActualFunctionCall_ = NULL;
	}

	if (!enabled_) return MockIgnoredActualCall::instance();
	if (tracing_) return MockActualCallTrace::instance().withName(functionName);


	if (!expectations_.hasExpectationWithName(functionName) && ignoreOtherCalls_) {
		return MockIgnoredActualCall::instance();
	}

	MockCheckedActualCall* call = createActualFunctionCall();
	call->withName(functionName);
	return *call;
}

void MockSupport::ignoreOtherCalls()
{
	ignoreOtherCalls_ = true;

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p)) getMockSupport(p)->ignoreOtherCalls();
}

void MockSupport::disable()
{
	enabled_ = false;

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p)) getMockSupport(p)->disable();
}

void MockSupport::enable()
{
	enabled_ = true;

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p)) getMockSupport(p)->enable();
}

void MockSupport::tracing(bool enabled)
{
	tracing_ = enabled;

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p)) getMockSupport(p)->tracing(enabled);
}

const char* MockSupport::getTraceOutput()
{
	return MockActualCallTrace::instance().getTraceOutput();
}

bool MockSupport::expectedCallsLeft()
{
	int callsLeft = expectations_.hasUnfullfilledExpectations();

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p)) callsLeft += getMockSupport(p)->expectedCallsLeft();

	return callsLeft != 0;
}

bool MockSupport::wasLastCallFulfilled()
{
	if (lastActualFunctionCall_ && !lastActualFunctionCall_->isFulfilled())
		return false;

	for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
		if (getMockSupport(p) && !getMockSupport(p)->wasLastCallFulfilled())
				return false;

	return true;
}

void MockSupport::failTestWithUnexpectedCalls()
{
    MockExpectedFunctionsList expectationsList;
    expectationsList.addExpectations(expectations_);

    for(MockNamedValueListNode *p = data_.begin();p;p = p->next())
		if(getMockSupport(p))
			expectationsList.addExpectations(getMockSupport(p)->expectations_);

    MockExpectedCallsDidntHappenFailure failure(activeReporter_->getTestToFail(), expectationsList);
    clear();
    failTest(failure);
}

void MockSupport::failTestWithOutOfOrderCalls()
{
    MockExpectedFunctionsList expectationsList;
    expectationsList.addExpectations(expectations_);

    for(MockNamedValueListNode *p = data_.begin();p;p = p->next())
		if(getMockSupport(p))
			expectationsList.addExpectations(getMockSupport(p)->expectations_);

    MockCallOrderFailure failure(activeReporter_->getTestToFail(), expectationsList);
    clear();
    failTest(failure);
}

void MockSupport::failTest(MockFailure& failure)
{
	activeReporter_->failTest(failure);
}

void MockSupport::checkExpectationsOfLastCall()
{
    if(lastActualFunctionCall_)
        lastActualFunctionCall_->checkExpectations();

	for(MockNamedValueListNode *p = data_.begin();p;p = p->next())
		if(getMockSupport(p) && getMockSupport(p)->lastActualFunctionCall_)
			getMockSupport(p)->lastActualFunctionCall_->checkExpectations();
}

void MockSupport::checkExpectations()
{
	checkExpectationsOfLastCall();

	if (wasLastCallFulfilled() && expectedCallsLeft())
		failTestWithUnexpectedCalls();

	if (expectations_.hasCallsOutOfOrder())
		failTestWithOutOfOrderCalls();
}

bool MockSupport::hasData(const SimpleString& name)
{
	return data_.getValueByName(name) != NULL;
}

MockNamedValue* MockSupport::retrieveDataFromStore(const SimpleString& name)
{
	MockNamedValue* newData = data_.getValueByName(name);
	if (newData == NULL) {
		newData = new MockNamedValue(name);
		data_.add(newData);
	}
	return newData;
}

void MockSupport::setData(const SimpleString& name, unsigned int value)
{
	MockNamedValue* newData = retrieveDataFromStore(name);
	newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, int value)
{
	MockNamedValue* newData = retrieveDataFromStore(name);
	newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, const char* value)
{
	MockNamedValue* newData = retrieveDataFromStore(name);
	newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, double value)
{
	MockNamedValue* newData = retrieveDataFromStore(name);
	newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, void* value)
{
	MockNamedValue* newData = retrieveDataFromStore(name);
	newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, const void* value)
{
	MockNamedValue* newData = retrieveDataFromStore(name);
	newData->setValue(value);
}

void MockSupport::setDataObject(const SimpleString& name, const SimpleString& type, void* value)
{
	MockNamedValue* newData = retrieveDataFromStore(name);
	newData->setObjectPointer(type, value);
}

MockNamedValue MockSupport::getData(const SimpleString& name)
{
	MockNamedValue* value = data_.getValueByName(name);
	if (value == NULL)
		return MockNamedValue("");
	return *value;
}

MockSupport* MockSupport::clone()
{
	MockSupport* newMock = new MockSupport;
	newMock->setMockFailureStandardReporter(standardReporter_);
	if (ignoreOtherCalls_) newMock->ignoreOtherCalls();

	if (!enabled_) newMock->disable();

	if (strictOrdering_) newMock->strictOrder();

	newMock->tracing(tracing_);
	newMock->installComparators(comparatorRepository_);
	return newMock;
}

MockSupport* MockSupport::getMockSupportScope(const SimpleString& name)
{
	SimpleString mockingSupportName = MOCK_SUPPORT_SCOPE_PREFIX;
	mockingSupportName += name;

	if (hasData(mockingSupportName)) {
		STRCMP_EQUAL("MockSupport", getData(mockingSupportName).getType().asCharString());
		return (MockSupport*) getData(mockingSupportName).getObjectPointer();
	}

	MockSupport *newMock = clone();

	setDataObject(mockingSupportName, "MockSupport", newMock);
	return newMock;
}

MockSupport* MockSupport::getMockSupport(MockNamedValueListNode* node)
{
	if (node->getType() == "MockSupport" && node->getName().contains(MOCK_SUPPORT_SCOPE_PREFIX))
		return  (MockSupport*) node->item()->getObjectPointer();
	return NULL;
}

MockNamedValue MockSupport::returnValue()
{
	if (lastActualFunctionCall_) return lastActualFunctionCall_->returnValue();
	return MockNamedValue("");
}

unsigned int MockSupport::unsignedIntReturnValue()
{
	return returnValue().getUnsignedIntValue();
}

int MockSupport::intReturnValue()
{
	return returnValue().getIntValue();
}

const char* MockSupport::stringReturnValue()
{
	return returnValue().getStringValue();
}

double MockSupport::doubleReturnValue()
{
	return returnValue().getDoubleValue();
}

void* MockSupport::pointerReturnValue()
{
	return returnValue().getPointerValue();
}

const void* MockSupport::constPointerReturnValue()
{
	return returnValue().getConstPointerValue();
}

bool MockSupport::hasReturnValue()
{
	if (lastActualFunctionCall_) return lastActualFunctionCall_->hasReturnValue();
	return false;
}
