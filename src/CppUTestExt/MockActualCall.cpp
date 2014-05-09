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
#include "CppUTestExt/MockCheckedActualCall.h"
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockFailure.h"

MockActualCall::MockActualCall()
{
}

MockActualCall::~MockActualCall()
{
}

void MockCheckedActualCall::setName(const SimpleString& name)
{
	functionName_ = name;
}

SimpleString MockCheckedActualCall::getName() const
{
	return functionName_;
}


MockCheckedActualCall::MockCheckedActualCall(int callOrder, MockFailureReporter* reporter, const MockExpectedCallsList& allExpectations)
	: callOrder_(callOrder), reporter_(reporter), state_(CALL_SUCCEED), _fulfilledExpectation(NULL), allExpectations_(allExpectations)
{
	unfulfilledExpectations_.addUnfilfilledExpectations(allExpectations);
}

MockCheckedActualCall::~MockCheckedActualCall()
{
}

void MockCheckedActualCall::setMockFailureReporter(MockFailureReporter* reporter)
{
	reporter_ = reporter;
}


UtestShell* MockCheckedActualCall::getTest() const
{
	return reporter_->getTestToFail();
}

void MockCheckedActualCall::failTest(const MockFailure& failure)
{
	setState(CALL_FAILED);
	reporter_->failTest(failure);
}

void MockCheckedActualCall::finnalizeCallWhenFulfilled()
{
	if (unfulfilledExpectations_.hasFulfilledExpectations()) {
		_fulfilledExpectation = unfulfilledExpectations_.removeOneFulfilledExpectation();
		callHasSucceeded();
	}
}

void MockCheckedActualCall::callHasSucceeded()
{
	setState(CALL_SUCCEED);
	unfulfilledExpectations_.resetExpectations();
}

MockActualCall& MockCheckedActualCall::withName(const SimpleString& name)
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

MockActualCall& MockCheckedActualCall::withCallOrder(int)
{
	return *this;
}

void MockCheckedActualCall::checkActualParameter(const MockNamedValue& actualParameter)
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

MockActualCall& MockCheckedActualCall::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualCall& MockCheckedActualCall::withIntParameter(const SimpleString& name, int value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualCall& MockCheckedActualCall::withDoubleParameter(const SimpleString& name, double value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualCall& MockCheckedActualCall::withStringParameter(const SimpleString& name, const char* value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualCall& MockCheckedActualCall::withPointerParameter(const SimpleString& name, void* value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualCall& MockCheckedActualCall::withConstPointerParameter(const SimpleString& name, const void* value)
{
	MockNamedValue actualParameter(name);
	actualParameter.setValue(value);
	checkActualParameter(actualParameter);
	return *this;
}

MockActualCall& MockCheckedActualCall::withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value)
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

bool MockCheckedActualCall::isFulfilled() const
{
	return state_ == CALL_SUCCEED;
}

bool MockCheckedActualCall::hasFailed() const
{
	return state_ == CALL_FAILED;
}

void MockCheckedActualCall::checkExpectations()
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

const char* MockCheckedActualCall::stringFromState(ActualCallState state)
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

void MockCheckedActualCall::checkStateConsistency(ActualCallState oldState, ActualCallState newState)
{
	if (oldState == newState)
		FAIL(StringFromFormat("State change to the same state: %s.", stringFromState(newState)).asCharString());
	if (oldState == CALL_FAILED)
		FAIL("State was already failed. Cannot change state again.");
}

void MockCheckedActualCall::setState(ActualCallState state)
{
	checkStateConsistency(state_, state);
	state_ = state;
}

MockNamedValue MockCheckedActualCall::returnValue()
{
	checkExpectations();
	if (_fulfilledExpectation)
		return _fulfilledExpectation->returnValue();
	return MockNamedValue("no return value");
}

bool MockCheckedActualCall::hasReturnValue()
{
	return ! returnValue().getName().isEmpty();
}

MockActualCall& MockCheckedActualCall::onObject(void* objectPtr)
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

MockActualCallTrace::MockActualCallTrace()
{
}

MockActualCallTrace::~MockActualCallTrace()
{
}

MockActualCall& MockActualCallTrace::withName(const SimpleString& name)
{
	traceBuffer_ += "\nFunction name: ";
	traceBuffer_ += name;
	return *this;
}

MockActualCall& MockActualCallTrace::withCallOrder(int callOrder)
{
	traceBuffer_ += "\nwithCallOrder: ";
	traceBuffer_ += StringFrom(callOrder);
	return *this;
}

void MockActualCallTrace::addParameterName(const SimpleString& name)
{
	traceBuffer_ += " ";
	traceBuffer_ += name;
	traceBuffer_ += ":";
}

MockActualCall& MockActualCallTrace::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualCall& MockActualCallTrace::withIntParameter(const SimpleString& name, int value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualCall& MockActualCallTrace::withDoubleParameter(const SimpleString& name, double value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualCall& MockActualCallTrace::withStringParameter(const SimpleString& name, const char* value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualCall& MockActualCallTrace::withPointerParameter(const SimpleString& name, void* value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualCall& MockActualCallTrace::withConstPointerParameter(const SimpleString& name, const void* value)
{
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualCall& MockActualCallTrace::withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)
{
	traceBuffer_ += " ";
	traceBuffer_ += typeName;
    addParameterName(name);
	traceBuffer_ += StringFrom(value);
	return *this;
}

MockActualCall& MockActualCallTrace::ignoreOtherParameters()
{
	return *this;
}

bool MockActualCallTrace::hasReturnValue()
{
	return false;
}

MockNamedValue MockActualCallTrace::returnValue()
{
	return MockNamedValue("");
}

MockActualCall& MockActualCallTrace::onObject(void* objectPtr)
{
	traceBuffer_ += StringFrom(objectPtr);
	return *this;
}

void MockActualCallTrace::clear()
{
	traceBuffer_ = "";
}

const char* MockActualCallTrace::getTraceOutput()
{
	return traceBuffer_.asCharString();
}

MockActualCallTrace& MockActualCallTrace::instance()
{
	static MockActualCallTrace call;
	return call;
}

MockIgnoredActualCall& MockIgnoredActualCall::instance()
{
    static MockIgnoredActualCall call;
    return call;
}

