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
#include "CppUTestExt/MockExpectedFunctionsList.h"

class MockFailureReporter;
class MockFailure;
class MockNamedValue;

class MockCheckedActualCall : public MockActualCall
{
public:
	MockCheckedActualCall(int callOrder, MockFailureReporter* reporter, const MockExpectedFunctionsList& expectations);
	virtual ~MockCheckedActualCall();

	virtual MockActualCall& withName(const SimpleString& name) _override;
	virtual MockActualCall& withCallOrder(int) _override;
	virtual MockActualCall& withIntParameter(const SimpleString& name, int value) _override;
	virtual MockActualCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) _override;
	virtual MockActualCall& withDoubleParameter(const SimpleString& name, double value) _override;
	virtual MockActualCall& withStringParameter(const SimpleString& name, const char* value) _override;
	virtual MockActualCall& withPointerParameter(const SimpleString& name, void* value) _override;
	virtual MockActualCall& withConstPointerParameter(const SimpleString& name, const void* value) _override;
	virtual MockActualCall& withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value) _override;

	virtual bool hasReturnValue() _override;
	virtual MockNamedValue returnValue() _override;

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
	MockCheckedExpectedCall* _fulfilledExpectation;

	MockExpectedFunctionsList unfulfilledExpectations_;
	const MockExpectedFunctionsList& allExpectations_;
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
	virtual MockActualCall& withDoubleParameter(const SimpleString& name, double value) _override;
	virtual MockActualCall& withStringParameter(const SimpleString& name, const char* value) _override;
	virtual MockActualCall& withPointerParameter(const SimpleString& name, void* value) _override;
	virtual MockActualCall& withConstPointerParameter(const SimpleString& name, const void* value) _override;
	virtual MockActualCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value) _override;
	virtual MockActualCall& ignoreOtherParameters() _override;

	virtual bool hasReturnValue() _override;
	virtual MockNamedValue returnValue() _override;

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
	virtual MockActualCall& withUnsignedIntParameter(const SimpleString&, unsigned int) _override{ return *this; }
	virtual MockActualCall& withDoubleParameter(const SimpleString&, double) _override { return *this; }
	virtual MockActualCall& withStringParameter(const SimpleString&, const char*) _override { return *this; }
	virtual MockActualCall& withPointerParameter(const SimpleString& , void*) _override { return *this; }
	virtual MockActualCall& withConstPointerParameter(const SimpleString& , const void*) _override { return *this; }
	virtual MockActualCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) _override { return *this; }

	virtual bool hasReturnValue() _override { return false; }
	virtual MockNamedValue returnValue() _override { return MockNamedValue(""); }

	virtual MockActualCall& onObject(void* ) _override { return *this; }

    static MockIgnoredActualCall& instance();
};


#endif
