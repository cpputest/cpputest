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
#include "CppUTestExt/MockActualFunctionCall.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"
#include "CppUTestExt/MockFailure.h"

#define MOCK_SUPPORT_SCOPE_PREFIX "!!!$$$MockingSupportScope$$$!!!"

static MockSupport global_mock;

MockSupport& mock(const SimpleString& mockName)
{
	if (mockName != "")
		return *global_mock.getMockSupportScope(mockName);
	return global_mock;
}


MockSupport::MockSupport()
	: reporter_(&defaultReporter_), ignoreOtherCalls_(false), enabled_(true), lastActualFunctionCall_(NULL), data_(NULL)
{
}

MockSupport::~MockSupport()
{
}

void MockSupport::crashOnFailure()
{
	reporter_->crashOnFailure();
}

void MockSupport::setMockFailureReporter(MockFailureReporter* reporter)
{
	reporter_ = (reporter != NULL) ? reporter : &defaultReporter_;
}

void MockSupport::installComparator(const SimpleString& typeName, MockNamedValueComparator& comparator)
{
	comparatorRepository_.installComparator(typeName, comparator);
}

void MockSupport::removeAllComparators()
{
	comparatorRepository_.clear();
}

void MockSupport::clear()
{
	delete lastActualFunctionCall_;
	lastActualFunctionCall_ = NULL;
	expectations_.deleteAllExpectationsAndClearList();
	ignoreOtherCalls_ = false;

	if (data_) {
		for (MockNamedValueListNode* p = data_->begin(); p; p = p->next())
			if (p->getType() == "MockSupport" && p->getName().contains(MOCK_SUPPORT_SCOPE_PREFIX)) {
				MockSupport* support = (MockSupport*) p->item()->getObjectPointer();
				support->clear();
				delete support;
			}
		data_->clear();
	}
	delete data_;
	data_ = NULL;
}

MockFunctionCall& MockSupport::expectOneCall(const SimpleString& functionName)
{
	if (!enabled_) return MockIgnoredCall::instance();

	MockExpectedFunctionCall* call = new MockExpectedFunctionCall;
	call->setComparatorRepository(&comparatorRepository_);
	call->withName(functionName);
	expectations_.addExpectedCall(call);
	return *call;
}

MockActualFunctionCall* MockSupport::createActualFunctionCall()
{
	if (lastActualFunctionCall_) delete lastActualFunctionCall_;
	lastActualFunctionCall_ = new MockActualFunctionCall(reporter_, expectations_);
	return lastActualFunctionCall_;
}

MockFunctionCall& MockSupport::actualCall(const SimpleString& functionName)
{
	if (!enabled_) return MockIgnoredCall::instance();

	if (lastActualFunctionCall_) lastActualFunctionCall_->checkExpectations();

	if (!expectations_.hasExpectationWithName(functionName) && ignoreOtherCalls_) {
		return MockIgnoredCall::instance();
	}

	MockActualFunctionCall* call = createActualFunctionCall();
	call->setComparatorRepository(&comparatorRepository_);
	call->withName(functionName);
	return *call;
}

void MockSupport::ignoreOtherCalls()
{
	ignoreOtherCalls_ = true;
}

void MockSupport::disable()
{
	enabled_ = false;
}

void MockSupport::enable()
{
	enabled_ = true;
}

bool MockSupport::expectedCallsLeft()
{
	return expectations_.hasUnfullfilledExpectations();
}

void MockSupport::checkExpectations()
{
	if (lastActualFunctionCall_ && !lastActualFunctionCall_->isFulfilled()) {
		if (! lastActualFunctionCall_->hasFailed()) {
			lastActualFunctionCall_->checkExpectations();
		}
	}
	else if (expectedCallsLeft()) {
		MockExpectedCallsDidntHappenFailure failure(reporter_->getTestToFail(), expectations_);
		clear();
		reporter_->failTest(failure);
	}
	clear();
}

bool MockSupport::hasData(const SimpleString& name)
{
	if (data_ == NULL)
		return false;
	return data_->getValueByName(name) != NULL;
}


MockNamedValue* MockSupport::createAndStoreData(const SimpleString& name)
{
	if (data_ == NULL)
		data_ = new MockNamedValueList;

	MockNamedValue* newData = new MockNamedValue(name);
	data_->add(newData);
	return newData;
}

void MockSupport::setData(const SimpleString& name, int value)
{
	MockNamedValue* newData = createAndStoreData(name);
	newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, const char* value)
{
	MockNamedValue* newData = createAndStoreData(name);
	newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, double value)
{
	MockNamedValue* newData = createAndStoreData(name);
	newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, void* value)
{
	MockNamedValue* newData = createAndStoreData(name);
	newData->setValue(value);
}

void MockSupport::setDataObject(const SimpleString& name, const SimpleString& type, void* value)
{
	MockNamedValue* newData = createAndStoreData(name);
	newData->setObjectPointer(type, value);
}

MockNamedValue MockSupport::getData(const SimpleString& name)
{
	if (data_ == NULL)
		return MockNamedValue("");

	MockNamedValue* value = data_->getValueByName(name);
	if (value == NULL)
		return MockNamedValue("");
	return *value;
}

MockSupport* MockSupport::getMockSupportScope(const SimpleString& name)
{
	SimpleString mockingSupportName = MOCK_SUPPORT_SCOPE_PREFIX;
	mockingSupportName += name;

	if (!hasData(mockingSupportName))
		setDataObject(mockingSupportName, "MockSupport", new MockSupport);

	STRCMP_EQUAL("MockSupport", getData(mockingSupportName).getType().asCharString());
	return (MockSupport*) getData(mockingSupportName).getObjectPointer();
}

