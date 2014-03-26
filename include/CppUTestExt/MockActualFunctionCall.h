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
	MockActualFunctionCall();
	virtual ~MockActualFunctionCall();

	MockActualFunctionCall& withName(const SimpleString& name) { return withName_(name); }
	MockActualFunctionCall& withCallOrder(int n) { return withCallOrder_(n); }
	MockActualFunctionCall& withParameter(const SimpleString& name, int value) { return withIntParameter_(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, unsigned int value) { return withUnsignedIntParameter_(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, double value) { return withDoubleParameter_(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, const char* value) { return withStringParameter_(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, void* value) { return withPointerParameter_(name, value); }
	MockActualFunctionCall& withParameter(const SimpleString& name, const void* value) { return withConstPointerParameter_(name, value); }
	MockActualFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)
	    { return withParameterOfType_(typeName, name, value); }
	MockActualFunctionCall& ignoreOtherParameters() { return ignoreOtherParameters_(); }

    bool hasReturnValue() { return hasReturnValue_(); }
	MockNamedValue returnValue() { return returnValue_(); }

	MockActualFunctionCall& onObject(void* objectPtr) {return onObject_(objectPtr); }

protected:
	void setName(const SimpleString& name);
	SimpleString getName() const;

private:
	virtual MockActualFunctionCall& withName_(const SimpleString& name)=0;
	virtual MockActualFunctionCall& withCallOrder_(int)=0;
	virtual MockActualFunctionCall& withIntParameter_(const SimpleString& name, int value)=0;
	virtual MockActualFunctionCall& withUnsignedIntParameter_(const SimpleString& name, unsigned int value)=0;
	virtual MockActualFunctionCall& withDoubleParameter_(const SimpleString& name, double value)=0;
	virtual MockActualFunctionCall& withStringParameter_(const SimpleString& name, const char* value)=0;
	virtual MockActualFunctionCall& withPointerParameter_(const SimpleString& name, void* value)=0;
	virtual MockActualFunctionCall& withConstPointerParameter_(const SimpleString& name, const void* value)=0;
	virtual MockActualFunctionCall& withParameterOfType_(const SimpleString& typeName, const SimpleString& name, const void* value)=0;
	virtual MockActualFunctionCall& ignoreOtherParameters_() { return *this; }
	virtual bool hasReturnValue_()=0;
	virtual MockNamedValue returnValue_()=0;
	virtual MockActualFunctionCall& onObject_(void* objectPtr)=0;

	SimpleString functionName_;
};

class CheckedMockActualFunctionCall : public MockActualFunctionCall
{
public:
	CheckedMockActualFunctionCall(int callOrder, MockFailureReporter* reporter, const MockExpectedFunctionsList& expectations);
	virtual ~CheckedMockActualFunctionCall();


	bool isFulfilled() const;
	bool hasFailed() const;

	void checkExpectations();

	void setMockFailureReporter(MockFailureReporter* reporter);
	
protected:
	UtestShell* getTest() const;
	void callHasSucceeded();
	void finnalizeCallWhenFulfilled();
	void failTest(const MockFailure& failure);
	void checkActualParameter(const MockNamedValue& actualParameter);

	enum ActualCallState {
		CALL_IN_PROGESS,
		CALL_FAILED,
		CALL_SUCCEED
	};
	const char* stringFromState(ActualCallState state);
	void setState(ActualCallState state);
	void checkStateConsistency(ActualCallState oldState, ActualCallState newState);

private:
	virtual MockActualFunctionCall& withName_(const SimpleString& name) _override;
	virtual MockActualFunctionCall& withCallOrder_(int) _override;
	virtual MockActualFunctionCall& withIntParameter_(const SimpleString& name, int value) _override;
	virtual MockActualFunctionCall& withUnsignedIntParameter_(const SimpleString& name, unsigned int value) _override;
	virtual MockActualFunctionCall& withDoubleParameter_(const SimpleString& name, double value) _override;
	virtual MockActualFunctionCall& withStringParameter_(const SimpleString& name, const char* value) _override;
	virtual MockActualFunctionCall& withPointerParameter_(const SimpleString& name, void* value) _override;
	virtual MockActualFunctionCall& withConstPointerParameter_(const SimpleString& name, const void* value) _override;
	virtual MockActualFunctionCall& withParameterOfType_(const SimpleString& type, const SimpleString& name, const void* value) _override;

    virtual bool hasReturnValue_() _override;
	virtual MockNamedValue returnValue_() _override;

	virtual MockActualFunctionCall& onObject_(void* objectPtr) _override;
	
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


	const char* getTraceOutput();
	void clear();
	static MockFunctionCallTrace& instance();

private:
	virtual MockActualFunctionCall& withName_(const SimpleString& name) _override;
	virtual MockActualFunctionCall& withCallOrder_(int) _override;
	virtual MockActualFunctionCall& withIntParameter_(const SimpleString& name, int value) _override;
	virtual MockActualFunctionCall& withUnsignedIntParameter_(const SimpleString& name, unsigned int value) _override;
	virtual MockActualFunctionCall& withDoubleParameter_(const SimpleString& name, double value) _override;
	virtual MockActualFunctionCall& withStringParameter_(const SimpleString& name, const char* value) _override;
	virtual MockActualFunctionCall& withPointerParameter_(const SimpleString& name, void* value) _override;
	virtual MockActualFunctionCall& withConstPointerParameter_(const SimpleString& name, const void* value) _override;
	virtual MockActualFunctionCall& withParameterOfType_(const SimpleString& typeName, const SimpleString& name, const void* value) _override;

	virtual bool hasReturnValue_() _override;
	virtual MockNamedValue returnValue_() _override;

	virtual MockActualFunctionCall& onObject_(void* objectPtr) _override;

	SimpleString traceBuffer_;

	void addParameterName(const SimpleString& name);
};

class MockIgnoredActualCall: public MockActualFunctionCall
{
public:
    static MockIgnoredActualCall& instance();
    
private:
	virtual MockActualFunctionCall& withName_(const SimpleString&) _override { return *this;}
	virtual MockActualFunctionCall& withCallOrder_(int) _override { return *this; }
	virtual MockActualFunctionCall& withIntParameter_(const SimpleString&, int) _override { return *this; }
	virtual MockActualFunctionCall& withUnsignedIntParameter_(const SimpleString&, unsigned int) _override{ return *this; }
	virtual MockActualFunctionCall& withDoubleParameter_(const SimpleString&, double) _override { return *this; }
	virtual MockActualFunctionCall& withStringParameter_(const SimpleString&, const char*) _override { return *this; }
	virtual MockActualFunctionCall& withPointerParameter_(const SimpleString& , void*) _override { return *this; }
	virtual MockActualFunctionCall& withConstPointerParameter_(const SimpleString& , const void*) _override { return *this; }
	virtual MockActualFunctionCall& withParameterOfType_(const SimpleString&, const SimpleString&, const void*) _override { return *this; }

	virtual bool hasReturnValue_() _override { return false; }
	virtual MockNamedValue returnValue_() _override { return MockNamedValue(""); }

	virtual MockActualFunctionCall& onObject_(void* ) _override { return *this; }
};

#endif
