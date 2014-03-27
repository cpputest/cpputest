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

#ifndef D_MockCheckedExpectedCall_h
#define D_MockCheckedExpectedCall_h

#include "CppUTestExt/MockFunctionCall.h"
#include "CppUTestExt/MockNamedValue.h"

extern SimpleString StringFrom(const MockNamedValue& parameter);

class MockExpectedCall
{
public:
	MockExpectedCall();
	virtual ~MockExpectedCall();

	virtual MockExpectedCall& withName(const SimpleString& name)=0;
	virtual MockExpectedCall& withCallOrder(int)=0;
	MockExpectedCall& withParameter(const SimpleString& name, int value) { return withIntParameter(name, value); }
	MockExpectedCall& withParameter(const SimpleString& name, unsigned int value) { return withUnsignedIntParameter(name, value); }
	MockExpectedCall& withParameter(const SimpleString& name, double value) { return withDoubleParameter(name, value); }
	MockExpectedCall& withParameter(const SimpleString& name, const char* value) { return withStringParameter(name, value); }
	MockExpectedCall& withParameter(const SimpleString& name, void* value) { return withPointerParameter(name, value); }
	MockExpectedCall& withParameter(const SimpleString& name, const void* value) { return withConstPointerParameter(name, value); }
	virtual MockExpectedCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)=0;
	virtual MockExpectedCall& ignoreOtherParameters() { return *this;}

	virtual MockExpectedCall& withIntParameter(const SimpleString& name, int value)=0;
	virtual MockExpectedCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value)=0;
	virtual MockExpectedCall& withDoubleParameter(const SimpleString& name, double value)=0;
	virtual MockExpectedCall& withStringParameter(const SimpleString& name, const char* value)=0;
	virtual MockExpectedCall& withPointerParameter(const SimpleString& name, void* value)=0;
	virtual MockExpectedCall& withConstPointerParameter(const SimpleString& name, const void* value)=0;
	virtual MockExpectedCall& andReturnValue(int value)=0;
	virtual MockExpectedCall& andReturnValue(unsigned int value)=0;
	virtual MockExpectedCall& andReturnValue(double value)=0;
	virtual MockExpectedCall& andReturnValue(const char* value)=0;
	virtual MockExpectedCall& andReturnValue(void* value)=0;
	virtual MockExpectedCall& andReturnValue(const void* value)=0;
	virtual bool hasReturnValue()=0;
	virtual MockNamedValue returnValue()=0;

	virtual MockExpectedCall& onObject(void* objectPtr)=0;
};

class MockCheckedExpectedCall : public MockExpectedCall
{

public:
	MockCheckedExpectedCall();
	virtual ~MockCheckedExpectedCall();

	virtual MockExpectedCall& withName(const SimpleString& name) _override;
	virtual MockExpectedCall& withCallOrder(int) _override;
	virtual MockExpectedCall& withIntParameter(const SimpleString& name, int value) _override;
	virtual MockExpectedCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value) _override;
	virtual MockExpectedCall& withDoubleParameter(const SimpleString& name, double value) _override;
	virtual MockExpectedCall& withStringParameter(const SimpleString& name, const char* value) _override;
	virtual MockExpectedCall& withPointerParameter(const SimpleString& name, void* value) _override;
	virtual MockExpectedCall& withConstPointerParameter(const SimpleString& name, const void* value) _override;
	virtual MockExpectedCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value) _override;
	virtual MockExpectedCall& ignoreOtherParameters() _override;

	virtual MockExpectedCall& andReturnValue(int value) _override;
	virtual MockExpectedCall& andReturnValue(unsigned int value) _override;
	virtual MockExpectedCall& andReturnValue(double value) _override;
	virtual MockExpectedCall& andReturnValue(const char* value) _override;
	virtual MockExpectedCall& andReturnValue(void* value) _override;
	virtual MockExpectedCall& andReturnValue(const void* value) _override;

	virtual bool hasReturnValue() _override;
	virtual MockNamedValue returnValue() _override;

	virtual MockExpectedCall& onObject(void* objectPtr) _override;

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

protected:
	void setName(const SimpleString& name);
	SimpleString getName() const;
	
private:
	SimpleString functionName_;

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

struct MockExpectedCallCompositeNode;
class MockExpectedCallComposite : public MockExpectedCall
{
public:
	MockExpectedCallComposite();
	virtual ~MockExpectedCallComposite();

	virtual MockExpectedCall& withName(const SimpleString&) _override;
	virtual MockExpectedCall& withCallOrder(int) _override;
	virtual MockExpectedCall& withIntParameter(const SimpleString&, int) _override;
	virtual MockExpectedCall& withUnsignedIntParameter(const SimpleString&, unsigned int) _override;
	virtual MockExpectedCall& withDoubleParameter(const SimpleString&, double) _override;
	virtual MockExpectedCall& withStringParameter(const SimpleString&, const char*) _override;
	virtual MockExpectedCall& withConstPointerParameter(const SimpleString& , const void*) _override;
	virtual MockExpectedCall& withPointerParameter(const SimpleString& , void*) _override;
	virtual MockExpectedCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) _override;
	virtual MockExpectedCall& ignoreOtherParameters() _override;

	virtual MockExpectedCall& andReturnValue(int) _override;
	virtual MockExpectedCall& andReturnValue(unsigned int) _override;
	virtual MockExpectedCall& andReturnValue(double) _override;
	virtual MockExpectedCall& andReturnValue(const char*) _override;
	virtual MockExpectedCall& andReturnValue(void*) _override;
	virtual MockExpectedCall& andReturnValue(const void*) _override;

	virtual bool hasReturnValue() _override;
	virtual MockNamedValue returnValue() _override;

	virtual MockExpectedCall& onObject(void* ) _override;

	virtual void add(MockExpectedCall& call);
	virtual void clear();
private:
	MockExpectedCallCompositeNode* head_;
};

class MockIgnoredExpectedCall : public MockExpectedCall
{
public:

	virtual MockExpectedCall& withName(const SimpleString&) _override { return *this;}
	virtual MockExpectedCall& withCallOrder(int) _override { return *this; }
	virtual MockExpectedCall& withIntParameter(const SimpleString&, int) _override { return *this; }
	virtual MockExpectedCall& withUnsignedIntParameter(const SimpleString&, unsigned int) _override{ return *this; }
	virtual MockExpectedCall& withDoubleParameter(const SimpleString&, double) _override { return *this; }
	virtual MockExpectedCall& withStringParameter(const SimpleString&, const char*) _override { return *this; }
	virtual MockExpectedCall& withPointerParameter(const SimpleString& , void*) _override { return *this; }
	virtual MockExpectedCall& withConstPointerParameter(const SimpleString& , const void*) _override { return *this; }
	virtual MockExpectedCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) _override { return *this; }

	virtual MockExpectedCall& andReturnValue(int) _override { return *this; }
	virtual MockExpectedCall& andReturnValue(unsigned int) _override { return *this; }
	virtual MockExpectedCall& andReturnValue(double) _override { return *this;}
	virtual MockExpectedCall& andReturnValue(const char*) _override { return *this; }
	virtual MockExpectedCall& andReturnValue(void*) _override { return *this; }
	virtual MockExpectedCall& andReturnValue(const void*) _override { return *this; }

	virtual bool hasReturnValue() _override { return false; }
	virtual MockNamedValue returnValue() _override { return MockNamedValue(""); }

	virtual MockExpectedCall& onObject(void* ) _override { return *this; }

	static MockExpectedCall& instance();
};

#endif
