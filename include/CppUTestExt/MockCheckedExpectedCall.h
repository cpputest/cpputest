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

#ifndef D_MockCheckedExpectedCall_h
#define D_MockCheckedExpectedCall_h

#include "CppUTestExt/MockExpectedCall.h"
#include "CppUTestExt/MockNamedValue.h"

class MockCheckedExpectedCall : public MockExpectedCall
{

public:
    MockCheckedExpectedCall();
    MockCheckedExpectedCall(unsigned int numCalls);
    virtual ~MockCheckedExpectedCall() CPPUTEST_DESTRUCTOR_OVERRIDE;

    virtual MockExpectedCall& withName(const SimpleString& name) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withCallOrder(unsigned int callOrder) CPPUTEST_OVERRIDE { return withCallOrder(callOrder, callOrder); }
    virtual MockExpectedCall& withCallOrder(unsigned int initialCallOrder, unsigned int finalCallOrder) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withBoolParameter(const SimpleString& name, bool value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withIntParameter(const SimpleString& name, int value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withLongIntParameter(const SimpleString& name, long int value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withLongLongIntParameter(const SimpleString& name, cpputest_longlong value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withUnsignedLongLongIntParameter(const SimpleString& name, cpputest_ulonglong value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withDoubleParameter(const SimpleString& name, double value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withDoubleParameter(const SimpleString& name, double value, double tolerance) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withStringParameter(const SimpleString& name, const char* value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withPointerParameter(const SimpleString& name, void* value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withConstPointerParameter(const SimpleString& name, const void* value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withFunctionPointerParameter(const SimpleString& name, void (*value)()) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withOutputParameterReturning(const SimpleString& name, const void* value, size_t size) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withOutputParameterOfTypeReturning(const SimpleString& typeName, const SimpleString& name, const void* value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& withUnmodifiedOutputParameter(const SimpleString& name) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& ignoreOtherParameters() CPPUTEST_OVERRIDE;

    virtual MockExpectedCall& andReturnValue(bool value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(int value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(unsigned int value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(long int value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(unsigned long int value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(cpputest_longlong value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(cpputest_ulonglong value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(double value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(const char* value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(void* value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(const void* value) CPPUTEST_OVERRIDE;
    virtual MockExpectedCall& andReturnValue(void (*value)()) CPPUTEST_OVERRIDE;

    virtual MockNamedValue returnValue();

    virtual MockExpectedCall& onObject(void* objectPtr) CPPUTEST_OVERRIDE;

    virtual MockNamedValue getInputParameter(const SimpleString& name);
    virtual MockNamedValue getOutputParameter(const SimpleString& name);
    virtual SimpleString getInputParameterType(const SimpleString& name);
    virtual SimpleString getInputParameterValueString(const SimpleString& name);

    virtual bool hasInputParameterWithName(const SimpleString& name);
    virtual bool hasInputParameter(const MockNamedValue& parameter);
    virtual bool hasOutputParameterWithName(const SimpleString& name);
    virtual bool hasOutputParameter(const MockNamedValue& parameter);
    virtual bool relatesTo(const SimpleString& functionName);
    virtual bool relatesToObject(const void* objectPtr) const;

    virtual bool isFulfilled();
    virtual bool canMatchActualCalls();
    virtual bool isMatchingActualCallAndFinalized();
    virtual bool isMatchingActualCall();
    virtual bool areParametersMatchingActualCall();
    virtual bool isOutOfOrder() const;

    virtual void callWasMade(unsigned int callOrder);
    virtual void inputParameterWasPassed(const SimpleString& name);
    virtual void outputParameterWasPassed(const SimpleString& name);
    virtual void finalizeActualCallMatch();
    virtual void wasPassedToObject();
    virtual void resetActualCallMatchingState();

    virtual SimpleString callToString();
    virtual SimpleString missingParametersToString();

    enum { NO_EXPECTED_CALL_ORDER = 0 };

    virtual unsigned int getActualCallsFulfilled() const;

protected:
    void setName(const SimpleString& name);
    SimpleString getName() const;

private:
    SimpleString functionName_;

    class MockExpectedFunctionParameter : public MockNamedValue
    {
    public:
        MockExpectedFunctionParameter(const SimpleString& name);
        void setMatchesActualCall(bool b);
        bool isMatchingActualCall() const;

    private:
        bool matchesActualCall_;
    };

    MockExpectedFunctionParameter* item(MockNamedValueListNode* node);

    bool ignoreOtherParameters_;
    bool isActualCallMatchFinalized_;
    unsigned int initialExpectedCallOrder_;
    unsigned int finalExpectedCallOrder_;
    bool outOfOrder_;
    MockNamedValueList* inputParameters_;
    MockNamedValueList* outputParameters_;
    MockNamedValue returnValue_;
    void* objectPtr_;
    bool isSpecificObjectExpected_;
    bool wasPassedToObject_;
    unsigned int actualCalls_;
    unsigned int expectedCalls_;
};

class MockIgnoredExpectedCall: public MockExpectedCall
{
public:

    virtual MockExpectedCall& withName(const SimpleString&) CPPUTEST_OVERRIDE { return *this;}
    virtual MockExpectedCall& withCallOrder(unsigned int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withCallOrder(unsigned int, unsigned int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withBoolParameter(const SimpleString&, bool) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withIntParameter(const SimpleString&, int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withUnsignedIntParameter(const SimpleString&, unsigned int) CPPUTEST_OVERRIDE{ return *this; }
    virtual MockExpectedCall& withLongIntParameter(const SimpleString&, long int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withUnsignedLongIntParameter(const SimpleString&, unsigned long int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withLongLongIntParameter(const SimpleString&, cpputest_longlong) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withUnsignedLongLongIntParameter(const SimpleString&, cpputest_ulonglong) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withDoubleParameter(const SimpleString&, double) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withDoubleParameter(const SimpleString&, double, double) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withStringParameter(const SimpleString&, const char*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withPointerParameter(const SimpleString& , void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withConstPointerParameter(const SimpleString& , const void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withFunctionPointerParameter(const SimpleString& , void(*)()) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withMemoryBufferParameter(const SimpleString&, const unsigned char*, size_t) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withOutputParameterReturning(const SimpleString&, const void*, size_t) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withOutputParameterOfTypeReturning(const SimpleString&, const SimpleString&, const void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& withUnmodifiedOutputParameter(const SimpleString&) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& ignoreOtherParameters() CPPUTEST_OVERRIDE { return *this;}

    virtual MockExpectedCall& andReturnValue(bool) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(unsigned int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(long int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(unsigned long int) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(cpputest_longlong) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(cpputest_ulonglong) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(double) CPPUTEST_OVERRIDE { return *this;}
    virtual MockExpectedCall& andReturnValue(const char*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(const void*) CPPUTEST_OVERRIDE { return *this; }
    virtual MockExpectedCall& andReturnValue(void (*)()) CPPUTEST_OVERRIDE { return *this; }

    virtual MockExpectedCall& onObject(void*) CPPUTEST_OVERRIDE { return *this; }

    static MockExpectedCall& instance();
};

#endif
