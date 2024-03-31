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
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
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
    MockCheckedActualCall(unsigned int callOrder, MockFailureReporter* reporter, const MockExpectedCallsList& expectations);
    virtual ~MockCheckedActualCall() CPPUTEST_DESTRUCTOR_OVERRIDE;

    virtual MockActualCall& withName(const SimpleString& name) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withCallOrder(unsigned int) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withBoolParameter(const SimpleString& name, bool value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withIntParameter(const SimpleString& name, int value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withLongIntParameter(const SimpleString& name, long int value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withLongLongIntParameter(const SimpleString& name, cpputest_longlong value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withUnsignedLongLongIntParameter(const SimpleString& name, cpputest_ulonglong value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withDoubleParameter(const SimpleString& name, double value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withStringParameter(const SimpleString& name, const char* value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withPointerParameter(const SimpleString& name, void* value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withConstPointerParameter(const SimpleString& name, const void* value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withFunctionPointerParameter(const SimpleString& name, void (*value)()) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withOutputParameter(const SimpleString& name, void* output) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withOutputParameterOfType(const SimpleString& type, const SimpleString& name, void* output) CPPUTEST_OVERRIDE;

    virtual bool hasReturnValue() CPPUTEST_OVERRIDE;
    virtual MockNamedValue returnValue() CPPUTEST_OVERRIDE;

    virtual bool returnBoolValueOrDefault(bool default_value) CPPUTEST_OVERRIDE;
    virtual bool returnBoolValue() CPPUTEST_OVERRIDE;

    virtual int returnIntValueOrDefault(int default_value) CPPUTEST_OVERRIDE;
    virtual int returnIntValue() CPPUTEST_OVERRIDE;

    virtual unsigned long int returnUnsignedLongIntValue() CPPUTEST_OVERRIDE;
    virtual unsigned long int returnUnsignedLongIntValueOrDefault(unsigned long int) CPPUTEST_OVERRIDE;

    virtual long int returnLongIntValue() CPPUTEST_OVERRIDE;
    virtual long int returnLongIntValueOrDefault(long int default_value) CPPUTEST_OVERRIDE;

    virtual cpputest_ulonglong returnUnsignedLongLongIntValue() CPPUTEST_OVERRIDE;
    virtual cpputest_ulonglong returnUnsignedLongLongIntValueOrDefault(cpputest_ulonglong default_value) CPPUTEST_OVERRIDE;

    virtual cpputest_longlong returnLongLongIntValue() CPPUTEST_OVERRIDE;
    virtual cpputest_longlong returnLongLongIntValueOrDefault(cpputest_longlong default_value) CPPUTEST_OVERRIDE;

    virtual unsigned int returnUnsignedIntValue() CPPUTEST_OVERRIDE;
    virtual unsigned int returnUnsignedIntValueOrDefault(unsigned int default_value) CPPUTEST_OVERRIDE;

    virtual const char * returnStringValueOrDefault(const char * default_value) CPPUTEST_OVERRIDE;
    virtual const char * returnStringValue() CPPUTEST_OVERRIDE;

    virtual double returnDoubleValue() CPPUTEST_OVERRIDE;
    virtual double returnDoubleValueOrDefault(double default_value) CPPUTEST_OVERRIDE;

    virtual const void * returnConstPointerValue() CPPUTEST_OVERRIDE;
    virtual const void * returnConstPointerValueOrDefault(const void * default_value) CPPUTEST_OVERRIDE;

    virtual void * returnPointerValue() CPPUTEST_OVERRIDE;
    virtual void * returnPointerValueOrDefault(void *) CPPUTEST_OVERRIDE;

    typedef void (*FunctionPointerReturnValue)();
    virtual FunctionPointerReturnValue returnFunctionPointerValue() CPPUTEST_OVERRIDE;
    virtual FunctionPointerReturnValue returnFunctionPointerValueOrDefault(void (*)()) CPPUTEST_OVERRIDE;

    virtual MockActualCall& onObject(const void* objectPtr) CPPUTEST_OVERRIDE;

    virtual bool isFulfilled() const;
    virtual bool hasFailed() const;

    virtual void checkExpectations();

    virtual void setMockFailureReporter(MockFailureReporter* reporter);
protected:
    void setName(const SimpleString& name);
    SimpleString getName() const;
    virtual UtestShell* getTest() const;
    virtual void callHasSucceeded();
    virtual void copyOutputParameters(MockCheckedExpectedCall* call);
    virtual void completeCallWhenMatchIsFound();
    virtual void failTest(const MockFailure& failure);
    virtual void checkInputParameter(const MockNamedValue& actualParameter);
    virtual void checkOutputParameter(const MockNamedValue& outputParameter);
    virtual void discardCurrentlyMatchingExpectations();

    enum ActualCallState {
        CALL_IN_PROGRESS,
        CALL_FAILED,
        CALL_SUCCEED
    };
    virtual void setState(ActualCallState state);

private:
    SimpleString functionName_;
    unsigned int callOrder_;
    MockFailureReporter* reporter_;

    ActualCallState state_;
    bool expectationsChecked_;
    MockCheckedExpectedCall* matchingExpectation_;

    MockExpectedCallsList potentiallyMatchingExpectations_;
    const MockExpectedCallsList& allExpectations_;

    class MockOutputParametersListNode
    {
    public:
        SimpleString name_;
        SimpleString type_;
        void* ptr_;

        MockOutputParametersListNode* next_;
        MockOutputParametersListNode(const SimpleString& name, const SimpleString& type, void* ptr)
            : name_(name), type_(type), ptr_(ptr), next_(NULLPTR) {}
    };

    MockOutputParametersListNode* outputParameterExpectations_;

    virtual void addOutputParameter(const SimpleString& name, const SimpleString& type, void* ptr);
    void cleanUpOutputParameterList();
};

class MockActualCallTrace : public MockActualCall
{
public:
    MockActualCallTrace();
    virtual ~MockActualCallTrace() CPPUTEST_DESTRUCTOR_OVERRIDE;

    virtual MockActualCall& withName(const SimpleString& name) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withCallOrder(unsigned int) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withBoolParameter(const SimpleString& name, bool value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withIntParameter(const SimpleString& name, int value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withLongIntParameter(const SimpleString& name, long int value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withLongLongIntParameter(const SimpleString& name, cpputest_longlong value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withUnsignedLongLongIntParameter(const SimpleString& name, cpputest_ulonglong value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withDoubleParameter(const SimpleString& name, double value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withStringParameter(const SimpleString& name, const char* value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withPointerParameter(const SimpleString& name, void* value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withConstPointerParameter(const SimpleString& name, const void* value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withFunctionPointerParameter(const SimpleString& name, void (*value)()) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withOutputParameter(const SimpleString& name, void* output) CPPUTEST_OVERRIDE;
    virtual MockActualCall& withOutputParameterOfType(const SimpleString& typeName, const SimpleString& name, void* output) CPPUTEST_OVERRIDE;

    virtual bool hasReturnValue() CPPUTEST_OVERRIDE;
    virtual MockNamedValue returnValue() CPPUTEST_OVERRIDE;

    virtual bool returnBoolValueOrDefault(bool default_value) CPPUTEST_OVERRIDE;
    virtual bool returnBoolValue() CPPUTEST_OVERRIDE;

    virtual int returnIntValueOrDefault(int default_value) CPPUTEST_OVERRIDE;
    virtual int returnIntValue() CPPUTEST_OVERRIDE;

    virtual unsigned long int returnUnsignedLongIntValue() CPPUTEST_OVERRIDE;
    virtual unsigned long int returnUnsignedLongIntValueOrDefault(unsigned long int) CPPUTEST_OVERRIDE;

    virtual long int returnLongIntValue() CPPUTEST_OVERRIDE;
    virtual long int returnLongIntValueOrDefault(long int default_value) CPPUTEST_OVERRIDE;

    virtual cpputest_ulonglong returnUnsignedLongLongIntValue() CPPUTEST_OVERRIDE;
    virtual cpputest_ulonglong returnUnsignedLongLongIntValueOrDefault(cpputest_ulonglong default_value) CPPUTEST_OVERRIDE;

    virtual cpputest_longlong returnLongLongIntValue() CPPUTEST_OVERRIDE;
    virtual cpputest_longlong returnLongLongIntValueOrDefault(cpputest_longlong default_value) CPPUTEST_OVERRIDE;

    virtual unsigned int returnUnsignedIntValue() CPPUTEST_OVERRIDE;
    virtual unsigned int returnUnsignedIntValueOrDefault(unsigned int default_value) CPPUTEST_OVERRIDE;

    virtual const char * returnStringValueOrDefault(const char * default_value) CPPUTEST_OVERRIDE;
    virtual const char * returnStringValue() CPPUTEST_OVERRIDE;

    virtual double returnDoubleValue() CPPUTEST_OVERRIDE;
    virtual double returnDoubleValueOrDefault(double default_value) CPPUTEST_OVERRIDE;

    virtual void * returnPointerValue() CPPUTEST_OVERRIDE;
    virtual void * returnPointerValueOrDefault(void *) CPPUTEST_OVERRIDE;

    virtual const void * returnConstPointerValue() CPPUTEST_OVERRIDE;
    virtual const void * returnConstPointerValueOrDefault(const void * default_value) CPPUTEST_OVERRIDE;

    virtual MockCheckedActualCall::FunctionPointerReturnValue returnFunctionPointerValue() CPPUTEST_OVERRIDE;
    virtual MockCheckedActualCall::FunctionPointerReturnValue returnFunctionPointerValueOrDefault(void (*)()) CPPUTEST_OVERRIDE;

    virtual MockActualCall& onObject(const void* objectPtr) CPPUTEST_OVERRIDE;

    const char* getTraceOutput();
    void clear();
    static MockActualCallTrace& instance();
    static void clearInstance();

private:
    SimpleString traceBuffer_;

    static MockActualCallTrace* instance_;

    void addParameterName(const SimpleString& name);
};

class MockIgnoredActualCall: public MockActualCall
{
public:
    virtual MockActualCall& withName(const SimpleString&) CPPUTEST_OVERRIDE { return *this;}
    virtual MockActualCall& withCallOrder(unsigned int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withBoolParameter(const SimpleString&, bool) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withIntParameter(const SimpleString&, int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withUnsignedIntParameter(const SimpleString&, unsigned int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withLongIntParameter(const SimpleString&, long int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString&, unsigned long int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withLongLongIntParameter(const SimpleString&, cpputest_longlong) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withUnsignedLongLongIntParameter(const SimpleString&, cpputest_ulonglong) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withDoubleParameter(const SimpleString&, double) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withStringParameter(const SimpleString&, const char*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withPointerParameter(const SimpleString& , void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withConstPointerParameter(const SimpleString& , const void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withFunctionPointerParameter(const SimpleString& , void (*)()) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withMemoryBufferParameter(const SimpleString&, const unsigned char*, size_t) CPPUTEST_OVERRIDE  { return *this; }
    virtual MockActualCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withOutputParameter(const SimpleString&, void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockActualCall& withOutputParameterOfType(const SimpleString&, const SimpleString&, void*) CPPUTEST_OVERRIDE { return *this; }

    virtual bool hasReturnValue() CPPUTEST_OVERRIDE { return false; }
    virtual MockNamedValue returnValue() CPPUTEST_OVERRIDE { return MockNamedValue(""); }

    virtual bool returnBoolValueOrDefault(bool value) CPPUTEST_OVERRIDE { return value; }
    virtual bool returnBoolValue() CPPUTEST_OVERRIDE { return false; }

    virtual int returnIntValue() CPPUTEST_OVERRIDE { return 0; }
    virtual int returnIntValueOrDefault(int value) CPPUTEST_OVERRIDE { return value; }

    virtual unsigned long int returnUnsignedLongIntValue() CPPUTEST_OVERRIDE { return 0; }
    virtual unsigned long int returnUnsignedLongIntValueOrDefault(unsigned long int value) CPPUTEST_OVERRIDE { return value; }

    virtual long int returnLongIntValue() CPPUTEST_OVERRIDE { return 0; }
    virtual long int returnLongIntValueOrDefault(long int value) CPPUTEST_OVERRIDE { return value; }

    virtual cpputest_ulonglong returnUnsignedLongLongIntValue() CPPUTEST_OVERRIDE
    {
#if CPPUTEST_USE_LONG_LONG
        return 0;
#else
        cpputest_ulonglong ret = {};
        return ret;
#endif
    }

    virtual cpputest_ulonglong returnUnsignedLongLongIntValueOrDefault(cpputest_ulonglong value) CPPUTEST_OVERRIDE { return value; }

    virtual cpputest_longlong returnLongLongIntValue() CPPUTEST_OVERRIDE
    {
#if CPPUTEST_USE_LONG_LONG
        return 0;
#else
        cpputest_longlong ret = {};
        return ret;
#endif
    }

    virtual cpputest_longlong returnLongLongIntValueOrDefault(cpputest_longlong value) CPPUTEST_OVERRIDE { return value; }

    virtual unsigned int returnUnsignedIntValue() CPPUTEST_OVERRIDE { return 0; }
    virtual unsigned int returnUnsignedIntValueOrDefault(unsigned int value) CPPUTEST_OVERRIDE { return value; }

    virtual double returnDoubleValue() CPPUTEST_OVERRIDE { return 0.0; }
    virtual double returnDoubleValueOrDefault(double value) CPPUTEST_OVERRIDE { return value; }

    virtual const char * returnStringValue() CPPUTEST_OVERRIDE { return ""; }
    virtual const char * returnStringValueOrDefault(const char * value) CPPUTEST_OVERRIDE { return value; }

    virtual void * returnPointerValue() CPPUTEST_OVERRIDE { return NULLPTR; }
    virtual void * returnPointerValueOrDefault(void * value) CPPUTEST_OVERRIDE { return value; }

    virtual const void * returnConstPointerValue() CPPUTEST_OVERRIDE { return NULLPTR; }
    virtual const void * returnConstPointerValueOrDefault(const void * value) CPPUTEST_OVERRIDE { return value; }

    virtual MockCheckedActualCall::FunctionPointerReturnValue returnFunctionPointerValue() CPPUTEST_OVERRIDE { return NULLPTR; }
    virtual MockCheckedActualCall::FunctionPointerReturnValue returnFunctionPointerValueOrDefault(void (*value)()) CPPUTEST_OVERRIDE { return value; }

    virtual MockActualCall& onObject(const void* ) CPPUTEST_OVERRIDE { return *this; }

    static MockIgnoredActualCall& instance();
};

#endif
