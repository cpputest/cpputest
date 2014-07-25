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
#include "CppUTestExt/MockExpectedCallsList.h"
#include "CppUTestExt/MockCheckedExpectedCall.h"

MockExpectedCallsList::MockExpectedCallsList() : head_(NULL)
{
}

MockExpectedCallsList::~MockExpectedCallsList()
{
    while (head_) {
        MockExpectedCallsListNode* next = head_->next_;
        delete head_;
        head_ = next;
    }
}

bool MockExpectedCallsList::hasCallsOutOfOrder() const
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (p->expectedCall_->isOutOfOrder())
            return true;
    return false;
}

int MockExpectedCallsList::size() const
{
    int count = 0;
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        count++;
    return count;
}

bool MockExpectedCallsList::isEmpty() const
{
    return size() == 0;
}


int MockExpectedCallsList::amountOfExpectationsFor(const SimpleString& name) const
{
    int count = 0;
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (p->expectedCall_->relatesTo(name)) count++;
    return count;

}

int MockExpectedCallsList::amountOfUnfulfilledExpectations() const
{
    int count = 0;
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (! p->expectedCall_->isFulfilled()) count++;
    return count;
}

bool MockExpectedCallsList::hasFulfilledExpectations() const
{
    return (size() - amountOfUnfulfilledExpectations()) != 0;
}

bool MockExpectedCallsList::hasFulfilledExpectationsWithoutIgnoredParameters() const
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (p->expectedCall_->isFulfilledWithoutIgnoredParameters())
            return true;
    return false;
}

bool MockExpectedCallsList::hasUnfullfilledExpectations() const
{
    return amountOfUnfulfilledExpectations() != 0;
}

bool MockExpectedCallsList::hasExpectationWithName(const SimpleString& name) const
{
        for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
            if (p->expectedCall_->relatesTo(name))
                return true;
    return false;
}

void MockExpectedCallsList::addExpectedCall(MockCheckedExpectedCall* call)
{
    MockExpectedCallsListNode* newCall = new MockExpectedCallsListNode(call);

    if (head_ == NULL)
        head_ = newCall;
    else {
        MockExpectedCallsListNode* lastCall = head_;
        while (lastCall->next_) lastCall = lastCall->next_;
        lastCall->next_ = newCall;
    }
}

void MockExpectedCallsList::addUnfilfilledExpectations(const MockExpectedCallsList& list)
{
    for (MockExpectedCallsListNode* p = list.head_; p; p = p->next_)
        if (! p->expectedCall_->isFulfilled())
            addExpectedCall(p->expectedCall_);
}

void MockExpectedCallsList::addExpectationsRelatedTo(const SimpleString& name, const MockExpectedCallsList& list)
{
    for (MockExpectedCallsListNode* p = list.head_; p; p = p->next_)
        if (p->expectedCall_->relatesTo(name))
            addExpectedCall(p->expectedCall_);
}

void MockExpectedCallsList::addExpectations(const MockExpectedCallsList& list)
{
    for (MockExpectedCallsListNode* p = list.head_; p; p = p->next_)
        addExpectedCall(p->expectedCall_);
}

void MockExpectedCallsList::onlyKeepExpectationsRelatedTo(const SimpleString& name)
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (! p->expectedCall_->relatesTo(name))
            p->expectedCall_ = NULL;

    pruneEmptyNodeFromList();
}

void MockExpectedCallsList::onlyKeepUnfulfilledExpectations()
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (p->expectedCall_->isFulfilled())
            p->expectedCall_ = NULL;

    pruneEmptyNodeFromList();
}

void MockExpectedCallsList::onlyKeepUnfulfilledExpectationsRelatedTo(const SimpleString& name)
{
    onlyKeepUnfulfilledExpectations();
    onlyKeepExpectationsRelatedTo(name);
}

void MockExpectedCallsList::onlyKeepExpectationsWithInputParameterName(const SimpleString& name)
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (! p->expectedCall_->hasInputParameterWithName(name))
            p->expectedCall_ = NULL;
    pruneEmptyNodeFromList();
}

void MockExpectedCallsList::onlyKeepExpectationsWithOutputParameterName(const SimpleString& name)
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (! p->expectedCall_->hasOutputParameter(name))
            p->expectedCall_ = NULL;
    pruneEmptyNodeFromList();
}

void MockExpectedCallsList::onlyKeepExpectationsWithInputParameter(const MockNamedValue& parameter)
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (! p->expectedCall_->hasInputParameter(parameter))
            p->expectedCall_ = NULL;
    pruneEmptyNodeFromList();
}

void MockExpectedCallsList::onlyKeepExpectationsOnObject(void* objectPtr)
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (! p->expectedCall_->relatesToObject(objectPtr))
            p->expectedCall_ = NULL;
    pruneEmptyNodeFromList();
}


void MockExpectedCallsList::onlyKeepUnfulfilledExpectationsWithInputParameter(const MockNamedValue& parameter)
{
    onlyKeepUnfulfilledExpectations();
    onlyKeepExpectationsWithInputParameter(parameter);
}

void MockExpectedCallsList::onlyKeepUnfulfilledExpectationsWithOutputParameter(const MockNamedValue& parameter)
{
    onlyKeepUnfulfilledExpectations();
    onlyKeepExpectationsWithOutputParameterName(parameter.getName());
}

void MockExpectedCallsList::onlyKeepUnfulfilledExpectationsOnObject(void* objectPtr)
{
    onlyKeepUnfulfilledExpectations();
    onlyKeepExpectationsOnObject(objectPtr);
}

MockCheckedExpectedCall* MockExpectedCallsList::removeOneFulfilledExpectation()
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
        if (p->expectedCall_->isFulfilled()) {
            MockCheckedExpectedCall* fulfilledCall = p->expectedCall_;
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList();
            return fulfilledCall;
        }
    }
    return NULL;
}

MockCheckedExpectedCall* MockExpectedCallsList::getOneFulfilledExpectationWithIgnoredParameters()
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
        if (p->expectedCall_->isFulfilledWithoutIgnoredParameters()) {
            return p->expectedCall_;
        }
    }
    return NULL;
}

MockCheckedExpectedCall* MockExpectedCallsList::removeOneFulfilledExpectationWithIgnoredParameters()
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
        if (p->expectedCall_->isFulfilledWithoutIgnoredParameters()) {
            MockCheckedExpectedCall* fulfilledCall = p->expectedCall_;
            p->expectedCall_->parametersWereIgnored();
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList();
            return fulfilledCall;
        }
    }
    return NULL;
}

void MockExpectedCallsList::pruneEmptyNodeFromList()
{
    MockExpectedCallsListNode* current = head_;
    MockExpectedCallsListNode* previous = NULL;
    MockExpectedCallsListNode* toBeDeleted = NULL;

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

void MockExpectedCallsList::deleteAllExpectationsAndClearList()
{
    while (head_) {
        MockExpectedCallsListNode* next = head_->next_;
        delete head_->expectedCall_;
        delete head_;
        head_ = next;
    }
}

void MockExpectedCallsList::resetExpectations()
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        p->expectedCall_->resetExpectation();
}

void MockExpectedCallsList::callWasMade(int callOrder)
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        p->expectedCall_->callWasMade(callOrder);
}

void MockExpectedCallsList::wasPassedToObject()
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        p->expectedCall_->wasPassedToObject();
}


void MockExpectedCallsList::parameterWasPassed(const SimpleString& parameterName)
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        p->expectedCall_->inputParameterWasPassed(parameterName);
}

void MockExpectedCallsList::outputParameterWasPassed(const SimpleString& parameterName)
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        p->expectedCall_->outputParameterWasPassed(parameterName);
}

MockExpectedCallsList::MockExpectedCallsListNode* MockExpectedCallsList::findNodeWithCallOrderOf(int callOrder) const
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (p->expectedCall_->getCallOrder() == callOrder && p->expectedCall_->isFulfilled())
            return p;
    return NULL;
}

static SimpleString stringOrNoneTextWhenEmpty(const SimpleString& inputString, const SimpleString& linePrefix)
{
    SimpleString str = inputString;
    if (str == "") {
        str += linePrefix;
        str += "<none>";
    }
    return str;
}

static SimpleString appendStringOnANewLine(const SimpleString& inputString, const SimpleString& linePrefix, const SimpleString& stringToAppend)
{
    SimpleString str = inputString;
    if (str != "") str += "\n";
    str += linePrefix;
    str += stringToAppend;
    return str;
}

SimpleString MockExpectedCallsList::unfulfilledCallsToString(const SimpleString& linePrefix) const
{
    SimpleString str;
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (!p->expectedCall_->isFulfilled())
            str = appendStringOnANewLine(str, linePrefix, p->expectedCall_->callToString());
    return stringOrNoneTextWhenEmpty(str, linePrefix);
}

SimpleString MockExpectedCallsList::fulfilledCallsToString(const SimpleString& linePrefix) const
{
    SimpleString str;

    MockExpectedCallsListNode* nextNodeInOrder;
    for (int callOrder = 1; (nextNodeInOrder = findNodeWithCallOrderOf(callOrder)); callOrder++)
        if (nextNodeInOrder)
            str = appendStringOnANewLine(str, linePrefix, nextNodeInOrder->expectedCall_->callToString());

    return stringOrNoneTextWhenEmpty(str, linePrefix);
}

SimpleString MockExpectedCallsList::missingParametersToString() const
{
    SimpleString str;
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (! p->expectedCall_->isFulfilled())
            str = appendStringOnANewLine(str, "", p->expectedCall_->missingParametersToString());

    return stringOrNoneTextWhenEmpty(str, "");
}

bool MockExpectedCallsList::hasUnfulfilledExpectationsBecauseOfMissingParameters() const
{
    for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
        if (! p->expectedCall_->areParametersFulfilled())
            return true;
    return false;
}

