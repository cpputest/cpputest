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

MockSupport::MockSupport() : reporter_(&defaultReporter_), ignoreOtherCalls_(false), lastActualFunctionCall_(NULL)
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

void MockSupport::clearExpectations()
{
	delete lastActualFunctionCall_;
	lastActualFunctionCall_ = NULL;
	expectations_.deleteAllExpectationsAndClearList();
}

MockFunctionCall* MockSupport::expectOneCall(const SimpleString& functionName)
{
	MockExpectedFunctionCall* call = new MockExpectedFunctionCall;
	call->withName(functionName);
	expectations_.addExpectedCall(call);
	return call;
}

MockActualFunctionCall* MockSupport::createActualFunctionCall()
{
	if (lastActualFunctionCall_) delete lastActualFunctionCall_;
	lastActualFunctionCall_ = new MockActualFunctionCall(reporter_, expectations_);
	return lastActualFunctionCall_;
}

MockFunctionCall* MockSupport::actualCall(const SimpleString& functionName)
{
	if (lastActualFunctionCall_) lastActualFunctionCall_->finalizeCall();

	MockActualFunctionCall* call = createActualFunctionCall();
	if (ignoreOtherCalls_) call->ignoreOtherCalls();
	call->withName(functionName);
	return call;
}

void MockSupport::ignoreOtherCalls()
{
	ignoreOtherCalls_ = true;
}

bool MockSupport::expectedCallsLeft()
{
	return expectations_.hasUnfullfilledExpectations();
}

void MockSupport::checkExpectations()
{
	if (lastActualFunctionCall_ && !lastActualFunctionCall_->isFulfilled()) {
		lastActualFunctionCall_->finalizeCall();
	}
	else if (expectedCallsLeft()) {
		MockExpectedFunctionsList expectedCalls;
		expectations_.addUnfilfilledExpectationsToList(&expectedCalls);
		MockExpectedCallsDidntHappenFailure failure(reporter_->getTestToFail(), expectedCalls);
		clearExpectations();
		reporter_->failTest(failure);
	}
	clearExpectations();
}
