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

#ifndef D_MockExpectedFunctionCall_h
#define D_MockExpectedFunctionCall_h

#include "CppUTestExt/MockFunctionCall.h"
#include "CppUTestExt/MockNamedValue.h"

extern SimpleString StringFrom(const MockNamedValue& parameter);

/*
 * MockExpectedFunctionCall is the interface for handling mock expectations.
 *
 */
class MockExpectedFunctionCall
{
public:
	MockExpectedFunctionCall();
	virtual ~MockExpectedFunctionCall();

	virtual MockExpectedFunctionCall& withName(const SimpleString& name)=0;
	virtual MockExpectedFunctionCall& withCallOrder(int)=0;
	MockExpectedFunctionCall& withParameter(const SimpleString& name, int value) { return withIntParameter(name, value); }
	MockExpectedFunctionCall& withParameter(const SimpleString& name, unsigned int value) { return withUnsignedIntParameter(name, value); }
	MockExpectedFunctionCall& withParameter(const SimpleString& name, double value) { return withDoubleParameter(name, value); }
	MockExpectedFunctionCall& withParameter(const SimpleString& name, const char* value) { return withStringParameter(name, value); }
	MockExpectedFunctionCall& withParameter(const SimpleString& name, void* value) { return withPointerParameter(name, value); }
	MockExpectedFunctionCall& withParameter(const SimpleString& name, const void* value) { return withConstPointerParameter(name, value); }
	virtual MockExpectedFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)=0;
	virtual MockExpectedFunctionCall& ignoreOtherParameters() { return *this;}

	virtual MockExpectedFunctionCall& withIntParameter(const SimpleString& name, int value)=0;
	virtual MockExpectedFunctionCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value)=0;
	virtual MockExpectedFunctionCall& withDoubleParameter(const SimpleString& name, double value)=0;
	virtual MockExpectedFunctionCall& withStringParameter(const SimpleString& name, const char* value)=0;
	virtual MockExpectedFunctionCall& withPointerParameter(const SimpleString& name, void* value)=0;
	virtual MockExpectedFunctionCall& withConstPointerParameter(const SimpleString& name, const void* value)=0;
	virtual MockExpectedFunctionCall& andReturnValue(int value)=0;
	virtual MockExpectedFunctionCall& andReturnValue(unsigned int value)=0;
	virtual MockExpectedFunctionCall& andReturnValue(double value)=0;
	virtual MockExpectedFunctionCall& andReturnValue(const char* value)=0;
	virtual MockExpectedFunctionCall& andReturnValue(void* value)=0;
	virtual MockExpectedFunctionCall& andReturnValue(const void* value)=0;
	
	virtual bool hasReturnValue()=0;
	
	virtual MockExpectedFunctionCall& onObject(void* objectPtr)=0;
};

class CheckedMockExpectedFunctionCall : public MockExpectedFunctionCall, public MockFunctionCallBase
{

public:
	CheckedMockExpectedFunctionCall();
	virtual ~CheckedMockExpectedFunctionCall();

	virtual MockExpectedFunctionCall& withName(const SimpleString& name) _override;
	virtual MockExpectedFunctionCall& withCallOrder(int) _override;
	virtual MockExpectedFunctionCall& withIntParameter(const SimpleString& name, int value) _override;
	virtual MockExpectedFunctionCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) _override;
	virtual MockExpectedFunctionCall& withDoubleParameter(const SimpleString& name, double value) _override;
	virtual MockExpectedFunctionCall& withStringParameter(const SimpleString& name, const char* value) _override;
	virtual MockExpectedFunctionCall& withPointerParameter(const SimpleString& name, void* value) _override;
	virtual MockExpectedFunctionCall& withConstPointerParameter(const SimpleString& name, const void* value) _override;
	virtual MockExpectedFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value) _override;
	virtual MockExpectedFunctionCall& ignoreOtherParameters() _override;

	virtual MockExpectedFunctionCall& andReturnValue(int value) _override;
	virtual MockExpectedFunctionCall& andReturnValue(unsigned int value) _override;
	virtual MockExpectedFunctionCall& andReturnValue(double value) _override;
	virtual MockExpectedFunctionCall& andReturnValue(const char* value) _override;
	virtual MockExpectedFunctionCall& andReturnValue(void* value) _override;
	virtual MockExpectedFunctionCall& andReturnValue(const void* value) _override;

	virtual bool hasReturnValue() _override;
	virtual MockNamedValue returnValue();

	virtual MockExpectedFunctionCall& onObject(void* objectPtr) _override;

	virtual MockNamedValue getParameter(const SimpleString& name);
	virtual SimpleString getParameterType(const SimpleString& name);
	virtual SimpleString getParameterValueString(const SimpleString& name);

	virtual bool hasParameterWithName(const SimpleString& name);
	virtual bool hasParameter(const MockNamedValue& parameter);
	virtual bool relatesTo(const SimpleString& functionName);
	virtual bool relatesToObject(void*objectPtr) const;

	virtual bool isFulfilled();
	virtual bool isFulfilledWithoutIgnoredParameters();
	virtual bool areParametersFulfilled();
	virtual bool areIgnoredParametersFulfilled();
	virtual bool isOutOfOrder() const;

	virtual void callWasMade(int callOrder);
	virtual void parameterWasPassed(const SimpleString& name);
	virtual void parametersWereIgnored();
	virtual void wasPassedToObject();
	virtual void resetExpectation();

	virtual SimpleString callToString();
	virtual SimpleString missingParametersToString();

	enum { NOT_CALLED_YET = -1, NO_EXPECTED_CALL_ORDER = -1};
	virtual int getCallOrder() const;
private:

	class MockExpectedFunctionParameter : public MockNamedValue
	{
	public:
		MockExpectedFunctionParameter(const SimpleString& name);
		void setFulfilled(bool b);
		bool isFulfilled() const;

	private:
		bool fulfilled_;
	};

	MockExpectedFunctionParameter* item(MockNamedValueListNode* node);

	bool ignoreOtherParameters_;
	bool parametersWereIgnored_;
	int callOrder_;
	int expectedCallOrder_;
	bool outOfOrder_;
	MockNamedValueList* parameters_;
	MockNamedValue returnValue_;
	void* objectPtr_;
	bool wasPassedToObject_;
};

struct MockFunctionCallCompositeNode;
class MockFunctionCallComposite : public CheckedMockExpectedFunctionCall
{
public:
	MockFunctionCallComposite();
	virtual ~MockFunctionCallComposite();

	virtual MockExpectedFunctionCall& withName(const SimpleString&) _override;
	virtual MockExpectedFunctionCall& withCallOrder(int) _override;
	virtual MockExpectedFunctionCall& withIntParameter(const SimpleString&, int) _override;
	virtual MockExpectedFunctionCall& withUnsignedIntParameter(const SimpleString&, unsigned int) _override;
	virtual MockExpectedFunctionCall& withDoubleParameter(const SimpleString&, double) _override;
	virtual MockExpectedFunctionCall& withStringParameter(const SimpleString&, const char*) _override;
	virtual MockExpectedFunctionCall& withConstPointerParameter(const SimpleString& , const void*) _override;
	virtual MockExpectedFunctionCall& withPointerParameter(const SimpleString& , void*) _override;
	virtual MockExpectedFunctionCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) _override;
	virtual MockExpectedFunctionCall& ignoreOtherParameters() _override;

	virtual MockExpectedFunctionCall& andReturnValue(int) _override;
	virtual MockExpectedFunctionCall& andReturnValue(unsigned int) _override;
	virtual MockExpectedFunctionCall& andReturnValue(double) _override;
	virtual MockExpectedFunctionCall& andReturnValue(const char*) _override;
	virtual MockExpectedFunctionCall& andReturnValue(void*) _override;
	virtual MockExpectedFunctionCall& andReturnValue(const void*) _override;

	virtual bool hasReturnValue() _override;

	virtual MockExpectedFunctionCall& onObject(void* ) _override;

	virtual void add(MockExpectedFunctionCall& call);
	virtual void clear();
private:
	MockFunctionCallCompositeNode* head_;
};

class MockIgnoredExpectedCall: public MockExpectedFunctionCall
{
public:

	virtual MockExpectedFunctionCall& withName(const SimpleString&) _override { return *this;}
	virtual MockExpectedFunctionCall& withCallOrder(int) _override { return *this; }
	virtual MockExpectedFunctionCall& withIntParameter(const SimpleString&, int) _override { return *this; }
	virtual MockExpectedFunctionCall& withUnsignedIntParameter(const SimpleString&, unsigned int) _override{ return *this; }
	virtual MockExpectedFunctionCall& withDoubleParameter(const SimpleString&, double) _override { return *this; }
	virtual MockExpectedFunctionCall& withStringParameter(const SimpleString&, const char*) _override { return *this; }
	virtual MockExpectedFunctionCall& withPointerParameter(const SimpleString& , void*) _override { return *this; }
	virtual MockExpectedFunctionCall& withConstPointerParameter(const SimpleString& , const void*) _override { return *this; }
	virtual MockExpectedFunctionCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) _override { return *this; }

	virtual MockExpectedFunctionCall& andReturnValue(int) _override { return *this; }
	virtual MockExpectedFunctionCall& andReturnValue(unsigned int) _override { return *this; }
	virtual MockExpectedFunctionCall& andReturnValue(double) _override { return *this;}
	virtual MockExpectedFunctionCall& andReturnValue(const char*) _override { return *this; }
	virtual MockExpectedFunctionCall& andReturnValue(void*) _override { return *this; }
	virtual MockExpectedFunctionCall& andReturnValue(const void*) _override { return *this; }

	virtual bool hasReturnValue() _override { return false; }
	virtual MockNamedValue returnValue() _override { return MockNamedValue(""); }

	virtual MockExpectedFunctionCall& onObject(void* ) _override { return *this; }

    static MockIgnoredExpectedCall& instance();
};

#endif
