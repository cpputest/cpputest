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
    MockCheckedActualCall(unsigned int callOrder, MockFailureReporter* reporter, const MockExpectedCallsList& expectations);
    virtual ~MockCheckedActualCall() _destructor_override;

    virtual MockActualCall& withName(const SimpleString& name) override;
    virtual MockActualCall& withCallOrder(unsigned int) override;
    virtual MockActualCall& withBoolParameter(const SimpleString& name, bool value) override;
    virtual MockActualCall& withIntParameter(const SimpleString& name, int value) override;
    virtual MockActualCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) override;
    virtual MockActualCall& withLongIntParameter(const SimpleString& name, long int value) override;
    virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value) override;
    virtual MockActualCall& withLongLongIntParameter(const SimpleString& name, cpputest_longlong value) override;
    virtual MockActualCall& withUnsignedLongLongIntParameter(const SimpleString& name, cpputest_ulonglong value) override;
    virtual MockActualCall& withDoubleParameter(const SimpleString& name, double value) override;
    virtual MockActualCall& withStringParameter(const SimpleString& name, const char* value) override;
    virtual MockActualCall& withPointerParameter(const SimpleString& name, void* value) override;
    virtual MockActualCall& withConstPointerParameter(const SimpleString& name, const void* value) override;
    virtual MockActualCall& withFunctionPointerParameter(const SimpleString& name, void (*value)()) override;
    virtual MockActualCall& withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size) override;
    virtual MockActualCall& withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value) override;
    virtual MockActualCall& withOutputParameter(const SimpleString& name, void* output) override;
    virtual MockActualCall& withOutputParameterOfType(const SimpleString& type, const SimpleString& name, void* output) override;

    virtual bool hasReturnValue() override;
    virtual MockNamedValue returnValue() override;

    virtual bool returnBoolValueOrDefault(bool default_value) override;
    virtual bool returnBoolValue() override;

    virtual int returnIntValueOrDefault(int default_value) override;
    virtual int returnIntValue() override;

    virtual unsigned long int returnUnsignedLongIntValue() override;
    virtual unsigned long int returnUnsignedLongIntValueOrDefault(unsigned long int) override;

    virtual long int returnLongIntValue() override;
    virtual long int returnLongIntValueOrDefault(long int default_value) override;

    virtual cpputest_ulonglong returnUnsignedLongLongIntValue() override;
    virtual cpputest_ulonglong returnUnsignedLongLongIntValueOrDefault(cpputest_ulonglong default_value) override;

    virtual cpputest_longlong returnLongLongIntValue() override;
    virtual cpputest_longlong returnLongLongIntValueOrDefault(cpputest_longlong default_value) override;

    virtual unsigned int returnUnsignedIntValue() override;
    virtual unsigned int returnUnsignedIntValueOrDefault(unsigned int default_value) override;

    virtual const char * returnStringValueOrDefault(const char * default_value) override;
    virtual const char * returnStringValue() override;

    virtual double returnDoubleValue() override;
    virtual double returnDoubleValueOrDefault(double default_value) override;

    virtual const void * returnConstPointerValue() override;
    virtual const void * returnConstPointerValueOrDefault(const void * default_value) override;

    virtual void * returnPointerValue() override;
    virtual void * returnPointerValueOrDefault(void *) override;

    typedef void (*FunctionPointerReturnValue)();
    virtual FunctionPointerReturnValue returnFunctionPointerValue() override;
    virtual FunctionPointerReturnValue returnFunctionPointerValueOrDefault(void (*)()) override;

    virtual MockActualCall& onObject(const void* objectPtr) override;

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
            : name_(name), type_(type), ptr_(ptr), next_(nullptr) {}
    };

    MockOutputParametersListNode* outputParameterExpectations_;

    virtual void addOutputParameter(const SimpleString& name, const SimpleString& type, void* ptr);
    virtual void cleanUpOutputParameterList();
};

class MockActualCallTrace : public MockActualCall
{
public:
    MockActualCallTrace();
    virtual ~MockActualCallTrace() _destructor_override;

    virtual MockActualCall& withName(const SimpleString& name) override;
    virtual MockActualCall& withCallOrder(unsigned int) override;
    virtual MockActualCall& withBoolParameter(const SimpleString& name, bool value) override;
    virtual MockActualCall& withIntParameter(const SimpleString& name, int value) override;
    virtual MockActualCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) override;
    virtual MockActualCall& withLongIntParameter(const SimpleString& name, long int value) override;
    virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value) override;
    virtual MockActualCall& withLongLongIntParameter(const SimpleString& name, cpputest_longlong value) override;
    virtual MockActualCall& withUnsignedLongLongIntParameter(const SimpleString& name, cpputest_ulonglong value) override;
    virtual MockActualCall& withDoubleParameter(const SimpleString& name, double value) override;
    virtual MockActualCall& withStringParameter(const SimpleString& name, const char* value) override;
    virtual MockActualCall& withPointerParameter(const SimpleString& name, void* value) override;
    virtual MockActualCall& withConstPointerParameter(const SimpleString& name, const void* value) override;
    virtual MockActualCall& withFunctionPointerParameter(const SimpleString& name, void (*value)()) override;
    virtual MockActualCall& withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size) override;
    virtual MockActualCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value) override;
    virtual MockActualCall& withOutputParameter(const SimpleString& name, void* output) override;
    virtual MockActualCall& withOutputParameterOfType(const SimpleString& typeName, const SimpleString& name, void* output) override;

    virtual bool hasReturnValue() override;
    virtual MockNamedValue returnValue() override;

    virtual bool returnBoolValueOrDefault(bool default_value) override;
    virtual bool returnBoolValue() override;

    virtual int returnIntValueOrDefault(int default_value) override;
    virtual int returnIntValue() override;

    virtual unsigned long int returnUnsignedLongIntValue() override;
    virtual unsigned long int returnUnsignedLongIntValueOrDefault(unsigned long int) override;

    virtual long int returnLongIntValue() override;
    virtual long int returnLongIntValueOrDefault(long int default_value) override;

    virtual cpputest_ulonglong returnUnsignedLongLongIntValue() override;
    virtual cpputest_ulonglong returnUnsignedLongLongIntValueOrDefault(cpputest_ulonglong default_value) override;

    virtual cpputest_longlong returnLongLongIntValue() override;
    virtual cpputest_longlong returnLongLongIntValueOrDefault(cpputest_longlong default_value) override;

    virtual unsigned int returnUnsignedIntValue() override;
    virtual unsigned int returnUnsignedIntValueOrDefault(unsigned int default_value) override;

    virtual const char * returnStringValueOrDefault(const char * default_value) override;
    virtual const char * returnStringValue() override;

    virtual double returnDoubleValue() override;
    virtual double returnDoubleValueOrDefault(double default_value) override;

    virtual void * returnPointerValue() override;
    virtual void * returnPointerValueOrDefault(void *) override;

    virtual const void * returnConstPointerValue() override;
    virtual const void * returnConstPointerValueOrDefault(const void * default_value) override;

    virtual MockCheckedActualCall::FunctionPointerReturnValue returnFunctionPointerValue() override;
    virtual MockCheckedActualCall::FunctionPointerReturnValue returnFunctionPointerValueOrDefault(void (*)()) override;

    virtual MockActualCall& onObject(const void* objectPtr) override;

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
    virtual MockActualCall& withName(const SimpleString&) override { return *this;}
    virtual MockActualCall& withCallOrder(unsigned int) override { return *this; }
    virtual MockActualCall& withBoolParameter(const SimpleString&, bool) override { return *this; }
    virtual MockActualCall& withIntParameter(const SimpleString&, int) override { return *this; }
    virtual MockActualCall& withUnsignedIntParameter(const SimpleString&, unsigned int) override { return *this; }
    virtual MockActualCall& withLongIntParameter(const SimpleString&, long int) override { return *this; }
    virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString&, unsigned long int) override { return *this; }
    virtual MockActualCall& withLongLongIntParameter(const SimpleString&, cpputest_longlong) override { return *this; }
    virtual MockActualCall& withUnsignedLongLongIntParameter(const SimpleString&, cpputest_ulonglong) override { return *this; }
    virtual MockActualCall& withDoubleParameter(const SimpleString&, double) override { return *this; }
    virtual MockActualCall& withStringParameter(const SimpleString&, const char*) override { return *this; }
    virtual MockActualCall& withPointerParameter(const SimpleString& , void*) override { return *this; }
    virtual MockActualCall& withConstPointerParameter(const SimpleString& , const void*) override { return *this; }
    virtual MockActualCall& withFunctionPointerParameter(const SimpleString& , void (*)()) override { return *this; }
    virtual MockActualCall& withMemoryBufferParameter(const SimpleString&, const unsigned char*, size_t) override  { return *this; }
    virtual MockActualCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) override { return *this; }
    virtual MockActualCall& withOutputParameter(const SimpleString&, void*) override { return *this; }
    virtual MockActualCall& withOutputParameterOfType(const SimpleString&, const SimpleString&, void*) override { return *this; }

    virtual bool hasReturnValue() override { return false; }
    virtual MockNamedValue returnValue() override { return MockNamedValue(""); }

    virtual bool returnBoolValueOrDefault(bool value) override { return value; }
    virtual bool returnBoolValue() override { return false; }

    virtual int returnIntValue() override { return 0; }
    virtual int returnIntValueOrDefault(int value) override { return value; }

    virtual unsigned long int returnUnsignedLongIntValue() override { return 0; }
    virtual unsigned long int returnUnsignedLongIntValueOrDefault(unsigned long int value) override { return value; }

    virtual long int returnLongIntValue() override { return 0; }
    virtual long int returnLongIntValueOrDefault(long int value) override { return value; }

    virtual cpputest_ulonglong returnUnsignedLongLongIntValue() override
    {
#if CPPUTEST_USE_LONG_LONG
        return 0;
#else
        cpputest_ulonglong ret = {};
        return ret;
#endif
    }

    virtual cpputest_ulonglong returnUnsignedLongLongIntValueOrDefault(cpputest_ulonglong value) override { return value; }

    virtual cpputest_longlong returnLongLongIntValue() override
    {
#if CPPUTEST_USE_LONG_LONG
        return 0;
#else
        cpputest_longlong ret = {};
        return ret;
#endif
    }

    virtual cpputest_longlong returnLongLongIntValueOrDefault(cpputest_longlong value) override { return value; }

    virtual unsigned int returnUnsignedIntValue() override { return 0; }
    virtual unsigned int returnUnsignedIntValueOrDefault(unsigned int value) override { return value; }

    virtual double returnDoubleValue() override { return 0.0; }
    virtual double returnDoubleValueOrDefault(double value) override { return value; }

    virtual const char * returnStringValue() override { return ""; }
    virtual const char * returnStringValueOrDefault(const char * value) override { return value; }

    virtual void * returnPointerValue() override { return nullptr; }
    virtual void * returnPointerValueOrDefault(void * value) override { return value; }

    virtual const void * returnConstPointerValue() override { return nullptr; }
    virtual const void * returnConstPointerValueOrDefault(const void * value) override { return value; }

    virtual MockCheckedActualCall::FunctionPointerReturnValue returnFunctionPointerValue() override { return nullptr; }
    virtual MockCheckedActualCall::FunctionPointerReturnValue returnFunctionPointerValueOrDefault(void (*value)()) override { return value; }

    virtual MockActualCall& onObject(const void* ) override { return *this; }

    static MockIgnoredActualCall& instance();
};

#endif
