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

#ifndef D_MockActualFunctionCall_h
#define D_MockActualFunctionCall_h

#include "CppUTestExt/MockFunctionCall.h"
#include "CppUTestExt/MockExpectedFunctionsList.h"

class MockFailureReporter;
class MockFailure;
class MockNamedValue;

class MockActualFunctionCall
{
public:
	MockActualFunctionCall() {}
	virtual ~MockActualFunctionCall() {}

	virtual MockActualFunctionCall& withName(const SimpleString& name)=0;
	virtual MockActualFunctionCall& withCallOrder(int)=0;
	MockActualFunctionCall& withParameter(const SimpleString& name, int value) { return withIntParameter(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, unsigned int value) { return withUnsignedIntParameter(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, double value) { return withDoubleParameter(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, const char* value) { return withStringParameter(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, void* value) { return withPointerParameter(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, const void* value) { return withConstPointerParameter(name, value); }
	virtual MockActualFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)=0;
	virtual MockActualFunctionCall& ignoreOtherParameters() { return *this;}

	virtual MockActualFunctionCall& withIntParameter(const SimpleString& name, int value)=0;
	virtual MockActualFunctionCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value)=0;
	virtual MockActualFunctionCall& withDoubleParameter(const SimpleString& name, double value)=0;
	virtual MockActualFunctionCall& withStringParameter(const SimpleString& name, const char* value)=0;
	virtual MockActualFunctionCall& withPointerParameter(const SimpleString& name, void* value)=0;
	virtual MockActualFunctionCall& withConstPointerParameter(const SimpleString& name, const void* value)=0;
	virtual MockActualFunctionCall& andReturnValue(int value)=0;
	virtual MockActualFunctionCall& andReturnValue(unsigned int value)=0;
	virtual MockActualFunctionCall& andReturnValue(double value)=0;
	virtual MockActualFunctionCall& andReturnValue(const char* value)=0;
	virtual MockActualFunctionCall& andReturnValue(void* value)=0;
	virtual MockActualFunctionCall& andReturnValue(const void* value)=0;
	virtual bool hasReturnValue()=0;
	virtual MockNamedValue returnValue()=0;

	virtual MockActualFunctionCall& onObject(void* objectPtr)=0;
};

class CheckedMockActualFunctionCall : public MockActualFunctionCall
{
public:
	CheckedMockActualFunctionCall(int callOrder, MockFailureReporter* reporter, const MockExpectedFunctionsList& expectations);
	virtual ~CheckedMockActualFunctionCall();

	virtual MockActualFunctionCall& withName(const SimpleString& name) _override;
	virtual MockActualFunctionCall& withCallOrder(int) _override;
	virtual MockActualFunctionCall& withIntParameter(const SimpleString& name, int value) _override;
	virtual MockActualFunctionCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) _override;
	virtual MockActualFunctionCall& withDoubleParameter(const SimpleString& name, double value) _override;
	virtual MockActualFunctionCall& withStringParameter(const SimpleString& name, const char* value) _override;
	virtual MockActualFunctionCall& withPointerParameter(const SimpleString& name, void* value) _override;
	virtual MockActualFunctionCall& withConstPointerParameter(const SimpleString& name, const void* value) _override;
	virtual MockActualFunctionCall& withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value) _override;

	virtual MockActualFunctionCall& andReturnValue(int value) _override;
	virtual MockActualFunctionCall& andReturnValue(unsigned int value) _override;
	virtual MockActualFunctionCall& andReturnValue(double value) _override;
	virtual MockActualFunctionCall& andReturnValue(const char* value) _override;
	virtual MockActualFunctionCall& andReturnValue(void* value) _override;
	virtual MockActualFunctionCall& andReturnValue(const void* value) _override;

	virtual bool hasReturnValue() _override;
	virtual MockNamedValue returnValue() _override;

	virtual MockActualFunctionCall& onObject(void* objectPtr) _override;

	virtual bool isFulfilled() const;
	virtual bool hasFailed() const;

	virtual void checkExpectations();

	virtual void setMockFailureReporter(MockFailureReporter* reporter);
	
protected:
	void setName(const SimpleString& name);
	SimpleString getName() const;
	virtual UtestShell* getTest() const;
	virtual void callHasSucceeded();
	virtual void finnalizeCallWhenFulfilled();
	virtual void failTest(const MockFailure& failure);
	virtual void checkActualParameter(const MockNamedValue& actualParameter);

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
	MockExpectedFunctionCall* _fulfilledExpectation;

	MockExpectedFunctionsList unfulfilledExpectations_;
	const MockExpectedFunctionsList& allExpectations_;
};

class MockFunctionCallTrace : public MockActualFunctionCall
{
public:
	MockFunctionCallTrace();
	virtual ~MockFunctionCallTrace();

	virtual MockActualFunctionCall& withName(const SimpleString& name) _override;
	virtual MockActualFunctionCall& withCallOrder(int) _override;
	virtual MockActualFunctionCall& withIntParameter(const SimpleString& name, int value) _override;
	virtual MockActualFunctionCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) _override;
	virtual MockActualFunctionCall& withDoubleParameter(const SimpleString& name, double value) _override;
	virtual MockActualFunctionCall& withStringParameter(const SimpleString& name, const char* value) _override;
	virtual MockActualFunctionCall& withPointerParameter(const SimpleString& name, void* value) _override;
	virtual MockActualFunctionCall& withConstPointerParameter(const SimpleString& name, const void* value) _override;
	virtual MockActualFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value) _override;
	virtual MockActualFunctionCall& ignoreOtherParameters() _override;

	virtual MockActualFunctionCall& andReturnValue(int value) _override;
	virtual MockActualFunctionCall& andReturnValue(unsigned int value) _override;
	virtual MockActualFunctionCall& andReturnValue(double value) _override;
	virtual MockActualFunctionCall& andReturnValue(const char* value) _override;
	virtual MockActualFunctionCall& andReturnValue(void* value) _override;
	virtual MockActualFunctionCall& andReturnValue(const void* value) _override;

	virtual bool hasReturnValue() _override;
	virtual MockNamedValue returnValue() _override;

	virtual MockActualFunctionCall& onObject(void* objectPtr) _override;

	const char* getTraceOutput();
	void clear();
	static MockFunctionCallTrace& instance();

private:
	SimpleString traceBuffer_;

	void addParameterName(const SimpleString& name);
};


class MockIgnoredActualCall: public MockActualFunctionCall
{
public:
	virtual MockActualFunctionCall& withName(const SimpleString&) _override { return *this;}
	virtual MockActualFunctionCall& withCallOrder(int) _override { return *this; }
	virtual MockActualFunctionCall& withIntParameter(const SimpleString&, int) _override { return *this; }
	virtual MockActualFunctionCall& withUnsignedIntParameter(const SimpleString&, unsigned int) _override{ return *this; }
	virtual MockActualFunctionCall& withDoubleParameter(const SimpleString&, double) _override { return *this; }
	virtual MockActualFunctionCall& withStringParameter(const SimpleString&, const char*) _override { return *this; }
	virtual MockActualFunctionCall& withPointerParameter(const SimpleString& , void*) _override { return *this; }
	virtual MockActualFunctionCall& withConstPointerParameter(const SimpleString& , const void*) _override { return *this; }
	virtual MockActualFunctionCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) _override { return *this; }

	virtual MockActualFunctionCall& andReturnValue(int) _override { return *this; }
	virtual MockActualFunctionCall& andReturnValue(unsigned int) _override { return *this; }
	virtual MockActualFunctionCall& andReturnValue(double) _override { return *this;}
	virtual MockActualFunctionCall& andReturnValue(const char*) _override { return *this; }
	virtual MockActualFunctionCall& andReturnValue(void*) _override { return *this; }
	virtual MockActualFunctionCall& andReturnValue(const void*) _override { return *this; }

	virtual bool hasReturnValue() _override { return false; }
	virtual MockNamedValue returnValue() _override { return MockNamedValue(""); }

	virtual MockActualFunctionCall& onObject(void* ) _override { return *this; }

    static MockIgnoredActualCall& instance();
};


#endif
