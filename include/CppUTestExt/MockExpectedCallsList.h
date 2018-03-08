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

#ifndef D_MockExpectedCallsList_h
#define D_MockExpectedCallsList_h

class MockCheckedExpectedCall;
class MockNamedValue;

class MockExpectedCallsList
{

public:
    MockExpectedCallsList();
    virtual ~MockExpectedCallsList();
    virtual void deleteAllExpectationsAndClearList();

    virtual unsigned int size() const;
    virtual unsigned int amountOfActualCallsFulfilledFor(const SimpleString& name) const;
    virtual unsigned int amountOfUnfulfilledExpectations() const;
    virtual bool hasUnfulfilledExpectations() const;
    virtual bool hasFinalizedMatchingExpectations() const;
    virtual bool hasUnmatchingExpectationsBecauseOfMissingParameters() const;
    virtual bool hasExpectationWithName(const SimpleString& name) const;
    virtual bool hasCallsOutOfOrder() const;
    virtual bool isEmpty() const;

    virtual void addExpectedCall(MockCheckedExpectedCall* call);
    virtual void addExpectations(const MockExpectedCallsList& list);
    virtual void addExpectationsRelatedTo(const SimpleString& name, const MockExpectedCallsList& list);

    virtual void onlyKeepOutOfOrderExpectations();
    virtual void addPotentiallyMatchingExpectations(const MockExpectedCallsList& list);

    virtual void onlyKeepExpectationsRelatedTo(const SimpleString& name);
    virtual void onlyKeepExpectationsWithInputParameter(const MockNamedValue& parameter);
    virtual void onlyKeepExpectationsWithInputParameterName(const SimpleString& name);
    virtual void onlyKeepExpectationsWithOutputParameter(const MockNamedValue& parameter);
    virtual void onlyKeepExpectationsWithOutputParameterName(const SimpleString& name);
    virtual void onlyKeepExpectationsOnObject(const void* objectPtr);
    virtual void onlyKeepUnmatchingExpectations();

    virtual MockCheckedExpectedCall* removeFirstFinalizedMatchingExpectation();
    virtual MockCheckedExpectedCall* removeFirstMatchingExpectation();
    virtual MockCheckedExpectedCall* getFirstMatchingExpectation();

    virtual void resetActualCallMatchingState();
    virtual void wasPassedToObject();
    virtual void parameterWasPassed(const SimpleString& parameterName);
    virtual void outputParameterWasPassed(const SimpleString& parameterName);

    virtual SimpleString unfulfilledCallsToString(const SimpleString& linePrefix = "") const;
    virtual SimpleString fulfilledCallsToString(const SimpleString& linePrefix = "") const;
    virtual SimpleString missingParametersToString() const;

protected:
    virtual void pruneEmptyNodeFromList();

    class MockExpectedCallsListNode
    {
    public:
        MockCheckedExpectedCall* expectedCall_;

        MockExpectedCallsListNode* next_;
        MockExpectedCallsListNode(MockCheckedExpectedCall* expectedCall)
            : expectedCall_(expectedCall), next_(NULLPTR) {}
    };

private:
    MockExpectedCallsListNode* head_;

    MockExpectedCallsList(const MockExpectedCallsList&);
};

#endif
