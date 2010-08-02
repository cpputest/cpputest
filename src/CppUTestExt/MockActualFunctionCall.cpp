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

MockActualFunctionCall::MockActualFunctionCall(MockFailureReporter* reporter, const MockExpectedFunctionsList& allExpectations)
	: reporter_(reporter), ignoreOtherCalls_(false), hasBeenFulfilled_(true), allExpectations_(allExpectations), comparatorRepository_(NULL)
{
	allExpectations.addUnfilfilledExpectationsToList(&unfulfilledExpectations_);
}

MockActualFunctionCall::~MockActualFunctionCall()
{
}

void MockActualFunctionCall::setComparatorRepository(MockParameterComparatorRepository* repository)
{
	comparatorRepository_ = repository;
}

void MockActualFunctionCall::ignoreOtherCalls()
{
	ignoreOtherCalls_ = true;
}


void MockActualFunctionCall::failTestBecauseOfUnexpectedCall(const SimpleString& name)
{
	int amountOfExpectationsFor = allExpectations_.amountOfExpectationsFor(name);
	if (amountOfExpectationsFor) {
		MockUnexpectedAdditionalCallFailure failure(reporter_->getTestToFail(), amountOfExpectationsFor, name);
		reporter_->failTest(failure);
	}
	else {
		MockUnexpectedCallHappenedFailure failure(reporter_->getTestToFail(), name);
		reporter_->failTest(failure);
	}
}

MockFunctionCall* MockActualFunctionCall::withName(const SimpleString& name)
{
	functionName_ = name;
	hasBeenFulfilled_ = false;
	unfulfilledExpectations_.onlyKeepUnfulfilledExpectationsRelatedTo(name);

	if (unfulfilledExpectations_.size() == 0 && !ignoreOtherCalls_) {
		failTestBecauseOfUnexpectedCall(name);
		return this;
	}

	unfulfilledExpectations_.callWasMade();
	hasBeenFulfilled_ = unfulfilledExpectations_.hasFulfilledExpectations();
	if (hasBeenFulfilled_) {
		unfulfilledExpectations_.removeOneFulfilledExpectation();
		unfulfilledExpectations_.resetExpectations();
	}

	return this;
}

void MockActualFunctionCall::checkActualParameter(const MockFunctionParameter& actualParameter)
{
	unfulfilledExpectations_.onlyKeepUnfulfilledExpectationsWithParameterName(actualParameter.name_);

	if (unfulfilledExpectations_.size() == 0 && !ignoreOtherCalls_) {
		MockUnexpectedParameterNameFailure failure(reporter_->getTestToFail(), functionName_, actualParameter.name_);
		reporter_->failTest(failure);
		return;
	}

	unfulfilledExpectations_.onlyKeepUnfulfilledExpectationsWithParameter(actualParameter);
	if (unfulfilledExpectations_.size() == 0 && !ignoreOtherCalls_) {
		MockUnexpectedParameterValueFailure failure(reporter_->getTestToFail(), functionName_, actualParameter.name_,
				StringFrom(actualParameter.type_, actualParameter.value_).asCharString());
		reporter_->failTest(failure);
		return;
	}

	unfulfilledExpectations_.parameterWasPassed(actualParameter.name_);
	hasBeenFulfilled_ = unfulfilledExpectations_.hasFulfilledExpectations();
	if (hasBeenFulfilled_) {
		unfulfilledExpectations_.removeOneFulfilledExpectation();
		unfulfilledExpectations_.resetExpectations();
	}
}

MockFunctionCall* MockActualFunctionCall::withParameter(const SimpleString& name, int value)
{
	MockFunctionParameter actualParameter(name, MOCK_FUNCTION_PARAMETER_INT, NULL);
	actualParameter.value_.intValue_ = value;
	checkActualParameter(actualParameter);
	return this;
}

MockFunctionCall* MockActualFunctionCall::withParameter(const SimpleString& /*name*/, double /*value*/)
{
	FAIL("NOT IMPLEMENTED YET");
	return this;
}

MockFunctionCall* MockActualFunctionCall::withParameter(const SimpleString& name, const char* value)
{
	MockFunctionParameter actualParameter(name, MOCK_FUNCTION_PARAMETER_STRING, NULL);
	actualParameter.value_.stringValue_ = value;
	checkActualParameter(actualParameter);
	return this;
}

MockFunctionCall* MockActualFunctionCall::withParameter(const SimpleString& name, void* value)
{
	MockFunctionParameter actualParameter(name, MOCK_FUNCTION_PARAMETER_POINTER, NULL);
	actualParameter.value_.pointerValue_ = value;
	checkActualParameter(actualParameter);
	return this;
}

MockFunctionCall* MockActualFunctionCall::withParameterOfType(const SimpleString& typeName, const SimpleString& name, void* value)
{
	if (comparatorRepository_ == NULL || comparatorRepository_->getComparatorForType(typeName) == NULL) {
		MockNoWayToCompareCustomTypeFailure failure(reporter_->getTestToFail(), typeName);
		reporter_->failTest(failure);
		return this;
	}
	MockFunctionParameter actualParameter(name, MOCK_FUNCTION_PARAMETER_OBJECT, NULL);
	actualParameter.value_.objectPointerValue_ = value;
	actualParameter.typeName_ = typeName;
	checkActualParameter(actualParameter);
	return this;
}


SimpleString MockActualFunctionCall::toString() const
{
	return functionName_;
}

bool MockActualFunctionCall::isFulfilled() const
{
	return hasBeenFulfilled_;
}

void MockActualFunctionCall::finalizeCall()
{
	MockExpectedFunctionCall* call = unfulfilledExpectations_.getExpectedCall();
	if (!hasBeenFulfilled_ && call) {
		SimpleString unFulfilledParameter = call->getUnfulfilledParameterName();
		SimpleString value = call->getParameterValueString(unFulfilledParameter);
		MockExpectedParameterDidntHappenFailure failure(reporter_->getTestToFail(), functionName_, unFulfilledParameter, value);
		reporter_->failTest(failure);
	}
}

