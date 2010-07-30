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

int MockExpectedFunctionsList::amountOfExpectationsFor(const SimpleString& name) const
{
	int count = 0;
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (p->expectedCall_->relatesTo(name)) count++;
	return count;

}

void MockExpectedFunctionsList::addExpectedCall(MockExpectedFunctionCall* call)
{
	head_ = new MockExpectedFunctionsListNode(call, head_);
}

void MockExpectedFunctionsList::addUnfilfilledExpectationsToList(MockExpectedFunctionsList* list) const
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_)
		if (! p->expectedCall_->isFulfilled())
			list->addExpectedCall(p->expectedCall_);
}

void MockExpectedFunctionsList::removeAllExpectationsExceptThisThatRelateTo(const SimpleString& name)
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_) {
		if (! p->expectedCall_->relatesTo(name))
			p->expectedCall_ = NULL;
	}
	pruneEmptyNodeFromList();
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
			else {
				previous->next_ = current->next_;
				previous = current;
				current = current->next_;
			}
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

bool MockExpectedFunctionsList::hasUnfullfilledExpectations()
{
	for (MockExpectedFunctionsListNode* p = head_; p; p = p->next_) {
		if (! p->expectedCall_->isFulfilled())
			return true;
	}
	return false;
}

MockExpectedFunctionCall* MockExpectedFunctionsList::getExpectedCall() const
{
	if (head_)
		return head_->expectedCall_;
	return NULL;
}

