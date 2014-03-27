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
#include "CppUTestExt/MockActualFunctionCall.h"
#include "CppUTestExt/MockExpectedFunctionsList.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"
#include "CppUTestExt/MockFailure.h"

void MockActualFunctionCall::setName(const SimpleString& name)
{
	functionName_ = name;
}

SimpleString MockActualFunctionCall::getName() const
{
	return functionName_;
}


CheckedMockActualFunctionCall::CheckedMockActualFunctionCall(int callOrder, MockFailureReporter* reporter, const MockExpectedFunctionsList& allExpectations)
	: callOrder_(callOrder), reporter_(reporter), state_(CALL_SUCCEED), _fulfilledExpectation(NULL), allExpectations_(allExpectations)
{
	unfulfilledExpectations_.addUnfilfilledExpectations(allExpectations);
}

CheckedMockActualFunctionCall::~CheckedMockActualFunctionCall()
{
}

void CheckedMockActualFunctionCall::setMockFailureReporter(MockFailureReporter* reporter)
{
	reporter_ = reporter;
}


UtestShell* CheckedMockActualFunctionCall::getTest() const
{
	return reporter_->getTestToFail();
}

void CheckedMockActualFunctionCall::failTest(const MockFailure& failure)
{
	setState(CALL_FAILED);
	reporter_->failTest(failure);
}

void CheckedMockActualFunctionCall::finnalizeCallWhenFulfilled()
{
	if (unfulfilledExpectations_.hasFulfilledExpectations()) {
		_fulfilledExpectation = unfulfilledExpectations_.removeOneFulfilledExpectation();
		callHasSucceeded();
	}
}

void CheckedMockActualFunctionCall::callHasSucceeded()
{
	setState(CALL_SUCCEED);
	unfulfilledExpectations_.resetExpectations();
}

MockActualFunctionCall& CheckedMockActualFunctionCall::withName(const SimpleString& name)
{
	setName(name);
	setState(CALL_IN_PROGESS);

	unfulfilledExpectations_.onlyKeepUnfulfilledExpectationsRelatedTo(name);
	if (unfulfilledExpectations_.isEmpty()) {
		MockUnexpectedCallHappenedFailure failure(getTest(), name, allExpectations_);
		failTest(failure);
		return *this;
	}

	unfulfilledExpectations_.callWasMade(callOrder_);

	finnalizeCallWhenFulfilled();

	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::withCallOrder(int)
{
	return *this;
}

void CheckedMockActualFunctionCall::checkActualParameter(const MockNamedValue& actualParameter)
{
	unfulfilledExpectations_.onlyKeepUnfulfilledExpectationsWithParameter(actualParameter);

	if (unfulfilledExpectations_.isEmpty()) {
		MockUnexpectedParameterFailure failure(getTest(), getName(), actualParameter, allExpectations_);
		failTest(failure);
		return;
	}

	unfulfilledExpectations_.parameterWasPassed(actualParameter.getName());
	finnalizeCallWhenFulfilled();
}

MockActualFunctionCall& CheckedMockActualFunctionCall::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::withIntParameter(const SimpleString& name, int value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::withDoubleParameter(const SimpleString& name, double value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::withStringParameter(const SimpleString& name, const char* value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::withPointerParameter(const SimpleString& name, void* value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::withConstPointerParameter(const SimpleString& name, const void* value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setObjectPointer(type, value);

	if (actualParameter.getComparator() == NULL) {
		MockNoWayToCompareCustomTypeFailure failure(getTest(), type);
		failTest(failure);
		return *this;
	}
	checkActualParameter(actualParameter);
	return *this;
}

bool CheckedMockActualFunctionCall::isFulfilled() const
{
	return state_ == CALL_SUCCEED;
}

bool CheckedMockActualFunctionCall::hasFailed() const
{
	return state_ == CALL_FAILED;
}

void CheckedMockActualFunctionCall::checkExpectations()
{
	if (state_ != CALL_IN_PROGESS) return;

	if (! unfulfilledExpectations_.hasUnfullfilledExpectations())
		FAIL("Actual call is in progress. Checking expectations. But no unfulfilled expectations. Cannot happen.")

	_fulfilledExpectation = unfulfilledExpectations_.removeOneFulfilledExpectationWithIgnoredParameters();
	if (_fulfilledExpectation) {
		callHasSucceeded();
		return;
	}

	if (unfulfilledExpectations_.hasUnfulfilledExpectationsBecauseOfMissingParameters()) {
		MockExpectedParameterDidntHappenFailure failure(getTest(), getName(), allExpectations_);
		failTest(failure);
	}
	else {
		MockExpectedObjectDidntHappenFailure failure(getTest(), getName(), allExpectations_);
		failTest(failure);
	}
}

const char* CheckedMockActualFunctionCall::stringFromState(ActualCallState state)
{
	switch (state) {
	case CALL_IN_PROGESS: return "In progress";
	case CALL_FAILED: return "Failed";
	case CALL_SUCCEED: return "Succeed";
#ifndef __clang__
	default: ;
#endif
	}
#ifndef __clang__
	return "No valid state info";
#endif
}

void CheckedMockActualFunctionCall::checkStateConsistency(ActualCallState oldState, ActualCallState newState)
{
	if (oldState == newState)
		FAIL(StringFromFormat("State change to the same state: %s.", stringFromState(newState)).asCharString());
	if (oldState == CALL_FAILED)
		FAIL("State was already failed. Cannot change state again.");
}

void CheckedMockActualFunctionCall::setState(ActualCallState state)
{
	checkStateConsistency(state_, state);
	state_ = state;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::andReturnValue(unsigned int)
{
	FAIL("andReturnValue cannot be called on an ActualFunctionCall. Use returnValue instead to get the value.");
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::andReturnValue(int)
{
	FAIL("andReturnValue cannot be called on an ActualFunctionCall. Use returnValue instead to get the value.");
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::andReturnValue(const char*)
{
	FAIL("andReturnValue cannot be called on an ActualFunctionCall. Use returnValue instead to get the value.");
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::andReturnValue(double)
{
	FAIL("andReturnValue cannot be called on an ActualFunctionCall. Use returnValue instead to get the value.");
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::andReturnValue(void*)
{
	FAIL("andReturnValue cannot be called on an ActualFunctionCall. Use returnValue instead to get the value.");
	return *this;
}

MockActualFunctionCall& CheckedMockActualFunctionCall::andReturnValue(const void*)
{
	FAIL("andReturnValue cannot be called on an ActualFunctionCall. Use returnValue instead to get the value.");
	return *this;
}

MockNamedValue CheckedMockActualFunctionCall::returnValue()
{
	checkExpectations();
	if (_fulfilledExpectation)
		return _fulfilledExpectation->returnValue();
	return MockNamedValue("no return value");
}

bool CheckedMockActualFunctionCall::hasReturnValue()
{
	return ! returnValue().getName().isEmpty();
}

MockActualFunctionCall& CheckedMockActualFunctionCall::onObject(void* objectPtr)
{
	unfulfilledExpectations_.onlyKeepUnfulfilledExpectationsOnObject(objectPtr);

	if (unfulfilledExpectations_.isEmpty()) {
		MockUnexpectedObjectFailure failure(getTest(), getName(), objectPtr, allExpectations_);
		failTest(failure);
		return *this;
	}

	unfulfilledExpectations_.wasPassedToObject();

	finnalizeCallWhenFulfilled();
	return *this;
}

MockFunctionCallTrace::MockFunctionCallTrace()
{
}

MockFunctionCallTrace::~MockFunctionCallTrace()
{
}

MockActualFunctionCall& MockFunctionCallTrace::withName(const SimpleString& name)
{
	traceBuffer_ += "\nFunction name: ";
	traceBuffer_ += name;
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::withCallOrder(int callOrder)
{
	traceBuffer_ += "\nwithCallOrder: ";
	traceBuffer_ += StringFrom(callOrder);
	return *this;
}

void MockFunctionCallTrace::addParameterName(const SimpleString& name)
{
	traceBuffer_ += " ";
	traceBuffer_ += name;
	traceBuffer_ += ":";
}

MockActualFunctionCall& MockFunctionCallTrace::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::withIntParameter(const SimpleString& name, int value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::withDoubleParameter(const SimpleString& name, double value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::withStringParameter(const SimpleString& name, const char* value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::withPointerParameter(const SimpleString& name, void* value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::withConstPointerParameter(const SimpleString& name, const void* value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)
{
	traceBuffer_ += " ";
	traceBuffer_ += typeName;
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::ignoreOtherParameters()
{
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::andReturnValue(unsigned int)
{
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::andReturnValue(int)
{
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::andReturnValue(double)
{
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::andReturnValue(const char*)
{
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::andReturnValue(void*)
{
	return *this;
}

MockActualFunctionCall& MockFunctionCallTrace::andReturnValue(const void*)
{
	return *this;
}

bool MockFunctionCallTrace::hasReturnValue()
{
	return false;
}

MockNamedValue MockFunctionCallTrace::returnValue()
{
	return MockNamedValue("");
}

MockActualFunctionCall& MockFunctionCallTrace::onObject(void* objectPtr)
{
	traceBuffer_ += StringFrom(objectPtr);
	return *this;
}

void MockFunctionCallTrace::clear()
{
	traceBuffer_ = "";
}

const char* MockFunctionCallTrace::getTraceOutput()
{
	return traceBuffer_.asCharString();
}

MockFunctionCallTrace& MockFunctionCallTrace::instance()
{
	static MockFunctionCallTrace call;
	return call;
}

MockIgnoredActualCall& MockIgnoredActualCall::instance()
{
    static MockIgnoredActualCall call;
    return call;
}

