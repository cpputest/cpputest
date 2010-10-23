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
#include "CppUTestExt/MockExpectedFunctionsList.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"

MockExpectedFunctionsList::MockExpectedFunctionsList() : head_(NULL)
{
}

MockExpectedFunctionsList::~MockExpectedFunctionsList()
{
	while (head_) {
		MockExpectedFunctionsListNode* next = head_->next_;
		delete head_;
		head_ = next;
	}
}

int MockExpectedFunctionsList::size() const
{
	int count = 0;
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		count++;
	return count;
}

bool MockExpectedFunctionsList::isEmpty() const
{
	return size() == 0;
}


int MockExpectedFunctionsList::amountOfExpectationsFor(const SimpleString& name) const
{
	int count = 0;
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (p->expectedCall_->relatesTo(name)) count++;
	return count;

}

int MockExpectedFunctionsList::amountOfUnfulfilledExpectations() const
{
	int count = 0;
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (! p->expectedCall_->isFulfilled()) count++;
	return count;
}

bool MockExpectedFunctionsList::hasFulfilledExpectations() const
{
	return (size() - amountOfUnfulfilledExpectations()) != 0;
}

bool MockExpectedFunctionsList::hasUnfullfilledExpectations() const
{
	return amountOfUnfulfilledExpectations() != 0;
}

bool MockExpectedFunctionsList::hasExpectationWithName(const SimpleString& name) const
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (p->expectedCall_->relatesTo(name))
			return true;
	return false;
}

void MockExpectedFunctionsList::addExpectedCall(MockExpectedFunctionCall* call)
{
	MockExpectedFunctionsListNode* newCall = new MockExpectedFunctionsListNode(call);

	if (head_ == NULL)
		head_ = newCall;
	else {
		MockExpectedFunctionsListNode* lastCall = head_;
		while (lastCall->next_) lastCall = lastCall->next_;
		lastCall->next_ = newCall;
	}
}

void MockExpectedFunctionsList::addUnfilfilledExpectations(const MockExpectedFunctionsList& list)
{
	for (MockExpectedFunctionsListNode* p = list.head_; p; p = p->next_)
		if (! p->expectedCall_->isFulfilled())
			addExpectedCall(p->expectedCall_);
}

void MockExpectedFunctionsList::addExpectationsRelatedTo(const SimpleString& name, const MockExpectedFunctionsList& list)
{
	for (MockExpectedFunctionsListNode* p = list.head_; p; p = p->next_)
		if (p->expectedCall_->relatesTo(name))
			addExpectedCall(p->expectedCall_);
}

void MockExpectedFunctionsList::addExpectations(const MockExpectedFunctionsList& list)
{
	for (MockExpectedFunctionsListNode* p = list.head_; p; p = p->next_)
		addExpectedCall(p->expectedCall_);
}

void MockExpectedFunctionsList::onlyKeepExpectationsRelatedTo(const SimpleString& name)
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (! p->expectedCall_->relatesTo(name))
			p->expectedCall_ = NULL;

	pruneEmptyNodeFromList();
}

void MockExpectedFunctionsList::onlyKeepUnfulfilledExpectations()
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (p->expectedCall_->isFulfilled())
			p->expectedCall_ = NULL;

	pruneEmptyNodeFromList();
}

void MockExpectedFunctionsList::onlyKeepUnfulfilledExpectationsRelatedTo(const SimpleString& name)
{
	onlyKeepUnfulfilledExpectations();
	onlyKeepExpectationsRelatedTo(name);
}

void MockExpectedFunctionsList::onlyKeepExpectationsWithParameterName(const SimpleString& name)
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (! p->expectedCall_->hasParameterWithName(name))
			p->expectedCall_ = NULL;
	pruneEmptyNodeFromList();
}

void MockExpectedFunctionsList::onlyKeepExpectationsWithParameter(const MockNamedValue& parameter)
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (! p->expectedCall_->hasParameter(parameter))
			p->expectedCall_ = NULL;
	pruneEmptyNodeFromList();
}

void MockExpectedFunctionsList::onlyKeepExpectationsOnObject(void* objectPtr)
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (! p->expectedCall_->relatesToObject(objectPtr))
			p->expectedCall_ = NULL;
	pruneEmptyNodeFromList();
}


void MockExpectedFunctionsList::onlyKeepUnfulfilledExpectationsWithParameter(const MockNamedValue& parameter)
{
	onlyKeepUnfulfilledExpectations();
	onlyKeepExpectationsWithParameter(parameter);
}

void MockExpectedFunctionsList::onlyKeepUnfulfilledExpectationsOnObject(void* objectPtr)
{
	onlyKeepUnfulfilledExpectations();
	onlyKeepExpectationsOnObject(objectPtr);
}

MockExpectedFunctionCall* MockExpectedFunctionsList::removeOneFulfilledExpectation()
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_) {
		if (p->expectedCall_->isFulfilled()) {
			MockExpectedFunctionCall* fulfilledCall = p->expectedCall_;
			p->expectedCall_ = NULL;
			pruneEmptyNodeFromList();
			return fulfilledCall;
		}
	}
	return NULL;
}

MockExpectedFunctionCall* MockExpectedFunctionsList::removeOneFulfilledExpectationWithIgnoredParameters()
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_) {
		if (p->expectedCall_->isFulfilledWithoutIgnoredParameters()) {
			MockExpectedFunctionCall* fulfilledCall = p->expectedCall_;
			p->expectedCall_->parametersWereIgnored();
			p->expectedCall_ = NULL;
			pruneEmptyNodeFromList();
			return fulfilledCall;
		}
	}
	return NULL;
}


void MockExpectedFunctionsList::pruneEmptyNodeFromList()
{
	MockExpectedFunctionsListNode* current = head_;
	MockExpectedFunctionsListNode* previous = NULL;
	MockExpectedFunctionsListNode* toBeDeleted = NULL;

	while (current) {
		if (current->expectedCall_ == NULL) {
			toBeDeleted = current;
			if (previous == NULL)
				head_ = current = current->next_;
			else
				current = previous->next_ = current->next_;
			delete toBeDeleted;
		}
		else {
			previous = current;
			current = current->next_;
		}
	}
}

void MockExpectedFunctionsList::deleteAllExpectationsAndClearList()
{
	while (head_) {
		MockExpectedFunctionsListNode* next = head_->next_;
		delete head_->expectedCall_;
		delete head_;
		head_ = next;
	}
}

void MockExpectedFunctionsList::resetExpectations()
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		p->expectedCall_->resetExpectation();
}

void MockExpectedFunctionsList::callWasMade()
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		p->expectedCall_->callWasMade();
}

void MockExpectedFunctionsList::wasPassedToObject()
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		p->expectedCall_->wasPassedToObject();
}


void MockExpectedFunctionsList::parameterWasPassed(const SimpleString& parameterName)
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		p->expectedCall_->parameterWasPassed(parameterName);
}

SimpleString MockExpectedFunctionsList::functionsToString(const SimpleString& linePrefix, bool wasFulfilled) const
{
	SimpleString str;
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_) {
		if (p->expectedCall_->isFulfilled() == wasFulfilled) {
			if (str != "") str += "\n";
			str += linePrefix;
			str += p->expectedCall_->callToString();
		}
	}
	if (str == "") {
		str += linePrefix;
		str += "<none>";
	}
	return str;

}

SimpleString MockExpectedFunctionsList::unfulfilledFunctionsToString(const SimpleString& linePrefix) const
{
	return functionsToString(linePrefix, false);
}

SimpleString MockExpectedFunctionsList::fulfilledFunctionsToString(const SimpleString& linePrefix) const
{
	return functionsToString(linePrefix, true);
}

SimpleString MockExpectedFunctionsList::missingParametersToString() const
{
	SimpleString str;
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_) {
		if (! p->expectedCall_->isFulfilled()) {
			if (str != "") str += "\n";
			str += p->expectedCall_->missingParametersToString();
		}
	}

	if (str == "")
		str = "<none>";
	return str;
}

bool MockExpectedFunctionsList::hasUnfulfilledExpectationsBecauseOfMissingParameters() const
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (! p->expectedCall_->areParametersFulfilled())
			return true;
	return false;
}

