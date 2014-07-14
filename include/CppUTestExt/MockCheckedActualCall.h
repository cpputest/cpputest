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

#ifndef D_MockCheckedActualCall_h
#define D_MockCheckedActualCall_h

#include "CppUTestExt/MockActualCall.h"
#include "CppUTestExt/MockExpectedCallsList.h"

class MockCheckedActualCall : public MockActualCall
{
public:
    MockCheckedActualCall(int callOrder, MockFailureReporter* reporter, const MockExpectedCallsList& expectations);
    virtual ~MockCheckedActualCall();

    virtual MockActualCall& withName(const SimpleString& name) _override;
    virtual MockActualCall& withCallOrder(int) _override;
    virtual MockActualCall& withIntParameter(const SimpleString& name, int value) _override;
    virtual MockActualCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) _override;
    virtual MockActualCall& withLongIntParameter(const SimpleString& name, long int value) _override;
    virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value) _override;
    virtual MockActualCall& withDoubleParameter(const SimpleString& name, double value) _override;
    virtual MockActualCall& withStringParameter(const SimpleString& name, const char* value) _override;
    virtual MockActualCall& withPointerParameter(const SimpleString& name, void* value) _override;
    virtual MockActualCall& withConstPointerParameter(const SimpleString& name, const void* value) _override;
    virtual MockActualCall& withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value) _override;
    virtual MockActualCall& withOutputParameter(const SimpleString& name, void* output) _override;

    virtual bool hasReturnValue() _override;
    virtual MockNamedValue returnValue() _override;

    virtual int returnIntValueOrDefault(int default_value) _override;
    virtual int returnIntValue() _override;

    virtual unsigned int returnUnsignedIntValue() _override;
    virtual unsigned int returnUnsignedIntValueOrDefault(unsigned int default_value) _override;

    virtual const char * returnStringValueOrDefault(const char * default_value) _override;
    virtual const char * returnStringValue() _override;

    virtual double returnDoubleValue() _override;
    virtual double returnDoubleValueOrDefault(double default_value) _override;

    virtual void * returnPointerValue() _override;
    virtual void * returnPointerValueOrDefault(void *) _override;

    virtual MockActualCall& onObject(void* objectPtr) _override;

    virtual bool isFulfilled() const;
    virtual bool hasFailed() const;

    virtual void checkExpectations();

    virtual void setMockFailureReporter(MockFailureReporter* reporter);
protected:
    void setName(const SimpleString& name);
    SimpleString getName() const;
    virtual UtestShell* getTest() const;
    virtual void callHasSucceeded();
    virtual void finalizeOutputParameters();
    virtual void finalizeCallWhenFulfilled();
    virtual void failTest(const MockFailure& failure);
    virtual void checkInputParameter(const MockNamedValue& actualParameter);
    virtual void checkOutputParameter(const MockNamedValue& outputParameter);

    enum ActualCallState {
        CALL_IN_PROGESS,
        CALL_FAILED,
        CALL_SUCCEED
    };
    virtual const char* stringFromState(ActualCallState state);
    virtual void setState(ActualCallState state);
    virtual void checkStateConsistency(ActualCallState oldState, ActualCallState newState);

private:
    SimpleString functionName_;
    int callOrder_;
    MockFailureReporter* reporter_;

    ActualCallState state_;
    MockCheckedExpectedCall* fulfilledExpectation_;

    MockExpectedCallsList unfulfilledExpectations_;
    const MockExpectedCallsList& allExpectations_;

    class MockOutputParametersListNode
    {
    public:
        SimpleString* name_;
        void* ptr_;

        MockOutputParametersListNode* next_;
        MockOutputParametersListNode(SimpleString* name, void* ptr)
            : name_(name), ptr_(ptr), next_(NULL) {}
    };

    MockOutputParametersListNode* outputParameterExpectations_;

    virtual void addOutputParameter(const SimpleString& name, void* ptr);
    virtual void cleanUpOutputParameterList();
};

class MockActualCallTrace : public MockActualCall
{
public:
    MockActualCallTrace();
    virtual ~MockActualCallTrace();

    virtual MockActualCall& withName(const SimpleString& name) _override;
    virtual MockActualCall& withCallOrder(int) _override;
    virtual MockActualCall& withIntParameter(const SimpleString& name, int value) _override;
    virtual MockActualCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) _override;
    virtual MockActualCall& withLongIntParameter(const SimpleString& name, long int value) _override;
    virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value) _override;
    virtual MockActualCall& withDoubleParameter(const SimpleString& name, double value) _override;
    virtual MockActualCall& withStringParameter(const SimpleString& name, const char* value) _override;
    virtual MockActualCall& withPointerParameter(const SimpleString& name, void* value) _override;
    virtual MockActualCall& withConstPointerParameter(const SimpleString& name, const void* value) _override;
    virtual MockActualCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value) _override;
    virtual MockActualCall& withOutputParameter(const SimpleString& name, void* output) _override;

    virtual bool hasReturnValue() _override;
    virtual MockNamedValue returnValue() _override;

    virtual int returnIntValueOrDefault(int default_value) _override;
    virtual int returnIntValue() _override;

    virtual unsigned int returnUnsignedIntValue() _override;
    virtual unsigned int returnUnsignedIntValueOrDefault(unsigned int default_value) _override;

    virtual const char * returnStringValueOrDefault(const char * default_value) _override;
    virtual const char * returnStringValue() _override;

    virtual double returnDoubleValue() _override;
    virtual double returnDoubleValueOrDefault(double default_value) _override;

    virtual void * returnPointerValue() _override;
    virtual void * returnPointerValueOrDefault(void *) _override;

    virtual MockActualCall& onObject(void* objectPtr) _override;

    const char* getTraceOutput();
    void clear();
    static MockActualCallTrace& instance();

private:
    SimpleString traceBuffer_;

    void addParameterName(const SimpleString& name);
};

class MockIgnoredActualCall: public MockActualCall
{
public:
    virtual MockActualCall& withName(const SimpleString&) _override { return *this;}
    virtual MockActualCall& withCallOrder(int) _override { return *this; }
    virtual MockActualCall& withIntParameter(const SimpleString&, int) _override { return *this; }
    virtual MockActualCall& withUnsignedIntParameter(const SimpleString&, unsigned int) _override { return *this; }
    virtual MockActualCall& withLongIntParameter(const SimpleString&, long int) _override { return *this; }
    virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString&, unsigned long int) _override { return *this; }
    virtual MockActualCall& withDoubleParameter(const SimpleString&, double) _override { return *this; }
    virtual MockActualCall& withStringParameter(const SimpleString&, const char*) _override { return *this; }
    virtual MockActualCall& withPointerParameter(const SimpleString& , void*) _override { return *this; }
    virtual MockActualCall& withConstPointerParameter(const SimpleString& , const void*) _override { return *this; }
    virtual MockActualCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) _override { return *this; }
    virtual MockActualCall& withOutputParameter(const SimpleString&, void*) _override { return *this; }

    virtual bool hasReturnValue() _override { return false; }
    virtual MockNamedValue returnValue() _override { return MockNamedValue(""); }

    virtual int returnIntValueOrDefault(int) _override { return 0; }
    virtual int returnIntValue() _override { return 0; }

    virtual unsigned int returnUnsignedIntValue() _override { return 0; }
    virtual unsigned int returnUnsignedIntValueOrDefault(unsigned int) _override { return returnUnsignedIntValue(); }

    virtual double returnDoubleValue() _override { return 0.0; }
    virtual double returnDoubleValueOrDefault(double) _override { return returnDoubleValue(); }

    virtual const char * returnStringValueOrDefault(const char *) _override { return returnStringValue(); }
    virtual const char * returnStringValue() _override { return ""; }

    virtual void * returnPointerValue() _override { return NULL; }
    virtual void * returnPointerValueOrDefault(void * ) _override { return returnPointerValue(); }

    virtual MockActualCall& onObject(void* ) _override { return *this; }

    static MockIgnoredActualCall& instance();
};

#endif
