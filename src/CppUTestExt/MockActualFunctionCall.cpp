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
	: reporter_(reporter), ignoreOtherCalls_(false), allExpectations_(allExpectations)
{
	allExpectations.addUnfilfilledExpectationsToList(&unfulfilledExpectations_);
}

MockActualFunctionCall::~MockActualFunctionCall()
{
}

void MockActualFunctionCall::ignoreOtherCalls()
{
	ignoreOtherCalls_ = true;
}

MockFunctionCall* MockActualFunctionCall::withName(const SimpleString& name)
{
	name_ = name;
	unfulfilledExpectations_.removeAllExpectationsExceptThisThatRelateTo(name);

	MockExpectedFunctionCall* expectedCall = unfulfilledExpectations_.getExpectedCall();
	if (expectedCall) {
		expectedCall->setFulfilled();
	}
	else if (!ignoreOtherCalls_) {
		int amountOfExpectationsFor =allExpectations_.amountOfExpectationsFor(name);
		if (amountOfExpectationsFor) {
			MockUnexpectedAdditionalCall failure(reporter_->getTestToFail(), amountOfExpectationsFor, name);
			reporter_->failTest(failure);

		}
		else {
			MockUnexpectedCallHappenedFailure failure(reporter_->getTestToFail(), name);
			reporter_->failTest(failure);
		}
	}
	return this;
}

MockFunctionCall* MockActualFunctionCall::withParameter(const SimpleString& /*name*/, int /*value*/)
{
	FAIL("NOT IMPLEMENTED YET");
	return this;
}

MockFunctionCall* MockActualFunctionCall::withParameter(const SimpleString& /*name*/, double /*value*/)
{
	FAIL("NOT IMPLEMENTED YET");
	return this;
}

MockFunctionCall* MockActualFunctionCall::withParameter(const SimpleString& /*name*/, const char* /*value*/)
{
	FAIL("NOT IMPLEMENTED YET");
	return this;
}
MockFunctionCall* MockActualFunctionCall::withParameter(const SimpleString& /*name*/, void* /*value*/)
{
	FAIL("NOT IMPLEMENTED YET");
	return this;
}

SimpleString MockActualFunctionCall::toString() const
{
	return name_;
}

