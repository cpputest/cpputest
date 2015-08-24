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

MockExpectedCallsList::MockExpectedCallsList() : head_(NULL), tail_(NULL)
{
}

MockExpectedCallsList::~MockExpectedCallsList()
{
    while (head_) {
        MockExpectedCallsListNode* next = head_->next_;
        delete head_;
        head_ = next;
    }
    tail_ = NULL;
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

bool MockExpectedCallsList::hasUnfulfilledExpectations() const
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

    if (head_ == NULL) {
        head_ = newCall;
    } else {
        tail_->next_ = newCall;
    }
    tail_ = newCall;
}

void MockExpectedCallsList::addUnfulfilledExpectations(const MockExpectedCallsList& list)
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

// Tracking the parent is needed because we need to know it in order to remove an element
// during the iteration.
// parent must not advance if p has been deleted (we achieve this by setting p=parent at the end
// of pruneEmptyNodeFromList(), and letting the "parent = p" execute normally in the for loop).
//
// You need to close the curly bracket after the processing step.
#define ITERATE_WITH_PARENT_AND_NEXT \
    for (MockExpectedCallsListNode* p = head_, *parent = NULL, *next; p; parent = p, p = next) {\
        next = p->next_;


void MockExpectedCallsList::pruneEmptyNodeFromList(MockExpectedCallsListNode*& parent,
                                                   MockExpectedCallsListNode*& p) {
    // assert(p);
    // assert(p->expectedCall_ == NULL);
    if (!parent) {
        // assert(head_ == p);
        head_ = p->next_;
    } else {
        // assert(parent->next_ == p);
        parent->next_ = p->next_;
    }
    if (tail_ == p) {   // we're removing the tail
        tail_ = parent;
        // assert(parent || (head_ == NULL));
    }
    delete p;
    p = parent;     // so that parent doesn't advance in the next iteration
}


void MockExpectedCallsList::onlyKeepExpectationsRelatedTo(const SimpleString& name)
{
    ITERATE_WITH_PARENT_AND_NEXT
        if (! p->expectedCall_->relatesTo(name)) {
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList(parent, p);
        }
    }
}

void MockExpectedCallsList::onlyKeepUnfulfilledExpectations()
{
    ITERATE_WITH_PARENT_AND_NEXT
        if (p->expectedCall_->isFulfilled())
        {
            p->expectedCall_->resetExpectation();
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList(parent, p);
        }
    }
}

void MockExpectedCallsList::onlyKeepExpectationsWithInputParameterName(const SimpleString& name)
{
    ITERATE_WITH_PARENT_AND_NEXT
        if (! p->expectedCall_->hasInputParameterWithName(name)) {
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList(parent, p);
        }
    }
}

void MockExpectedCallsList::onlyKeepExpectationsWithOutputParameterName(const SimpleString& name)
{
    ITERATE_WITH_PARENT_AND_NEXT
        if (! p->expectedCall_->hasOutputParameterWithName(name)) {
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList(parent, p);
        }
    }
}

void MockExpectedCallsList::onlyKeepExpectationsWithInputParameter(const MockNamedValue& parameter)
{
    ITERATE_WITH_PARENT_AND_NEXT
        if (! p->expectedCall_->hasInputParameter(parameter)) {
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList(parent, p);
        }
    }
}

void MockExpectedCallsList::onlyKeepExpectationsWithOutputParameter(const MockNamedValue& parameter)
{
    ITERATE_WITH_PARENT_AND_NEXT
        if (! p->expectedCall_->hasOutputParameter(parameter)) {
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList(parent, p);
        }
    }
}

void MockExpectedCallsList::onlyKeepExpectationsOnObject(void* objectPtr)
{
    ITERATE_WITH_PARENT_AND_NEXT
        if (! p->expectedCall_->relatesToObject(objectPtr)) {
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList(parent, p);
        }
    }
}

MockCheckedExpectedCall* MockExpectedCallsList::removeOneFulfilledExpectation()
{
    ITERATE_WITH_PARENT_AND_NEXT
        if (p->expectedCall_->isFulfilled()) {
            MockCheckedExpectedCall* fulfilledCall = p->expectedCall_;
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList(parent, p);
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
    ITERATE_WITH_PARENT_AND_NEXT
        if (p->expectedCall_->isFulfilledWithoutIgnoredParameters()) {
            MockCheckedExpectedCall* fulfilledCall = p->expectedCall_;
            p->expectedCall_->parametersWereIgnored();
            p->expectedCall_ = NULL;
            pruneEmptyNodeFromList(parent, p);
            return fulfilledCall;
        }
    }
    return NULL;
}

void MockExpectedCallsList::deleteAllExpectationsAndClearList()
{
    while (head_) {
        MockExpectedCallsListNode* next = head_->next_;
        delete head_->expectedCall_;
        delete head_;
        head_ = next;
    }
    tail_ = NULL;
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

