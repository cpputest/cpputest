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

#ifndef D_MockFunctionCall_h
#define D_MockFunctionCall_h

/*
 * MockFunctionCall is the main interface for recording and actualizing mock calls.
 *
 */

#include "CppUTestExt/MockNamedValue.h"

class MockFunctionCall
{
public:
	MockFunctionCall() {}
	virtual ~MockFunctionCall() {}

	virtual MockFunctionCall& withName(const SimpleString& name)=0;
	virtual MockFunctionCall& withCallOrder(int)=0;
	MockFunctionCall& withParameter(const SimpleString& name, int value) { return withIntParameter(name, value); }
	MockFunctionCall& withParameter(const SimpleString& name, unsigned int value) { return withUnsignedIntParameter(name, value); }
	MockFunctionCall& withParameter(const SimpleString& name, double value) { return withDoubleParameter(name, value); }
	MockFunctionCall& withParameter(const SimpleString& name, const char* value) { return withStringParameter(name, value); }
	MockFunctionCall& withParameter(const SimpleString& name, void* value) { return withPointerParameter(name, value); }
	MockFunctionCall& withParameter(const SimpleString& name, const void* value) { return withConstPointerParameter(name, value); }
	virtual MockFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)=0;
	virtual MockFunctionCall& ignoreOtherParameters() { return *this;}

	virtual MockFunctionCall& withIntParameter(const SimpleString& name, int value)=0;
	virtual MockFunctionCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value)=0;
	virtual MockFunctionCall& withDoubleParameter(const SimpleString& name, double value)=0;
	virtual MockFunctionCall& withStringParameter(const SimpleString& name, const char* value)=0;
	virtual MockFunctionCall& withPointerParameter(const SimpleString& name, void* value)=0;
	virtual MockFunctionCall& withConstPointerParameter(const SimpleString& name, const void* value)=0;
	virtual MockFunctionCall& andReturnValue(int value)=0;
	virtual MockFunctionCall& andReturnValue(unsigned int value)=0;
	virtual MockFunctionCall& andReturnValue(double value)=0;
	virtual MockFunctionCall& andReturnValue(const char* value)=0;
	virtual MockFunctionCall& andReturnValue(void* value)=0;
	virtual MockFunctionCall& andReturnValue(const void* value)=0;
	virtual bool hasReturnValue()=0;
	virtual MockNamedValue returnValue()=0;

	virtual MockFunctionCall& onObject(void* objectPtr)=0;

protected:
	void setName(const SimpleString& name);
	SimpleString getName() const;
private:
	SimpleString functionName_;
};

struct MockFunctionCallCompositeNode;
class MockFunctionCallComposite : public MockFunctionCall
{
public:
	MockFunctionCallComposite();
	virtual ~MockFunctionCallComposite();

	virtual MockFunctionCall& withName(const SimpleString&) _override;
	virtual MockFunctionCall& withCallOrder(int) _override;
	virtual MockFunctionCall& withIntParameter(const SimpleString&, int) _override;
	virtual MockFunctionCall& withUnsignedIntParameter(const SimpleString&, unsigned int) _override;
	virtual MockFunctionCall& withDoubleParameter(const SimpleString&, double) _override;
	virtual MockFunctionCall& withStringParameter(const SimpleString&, const char*) _override;
	virtual MockFunctionCall& withConstPointerParameter(const SimpleString& , const void*) _override;
	virtual MockFunctionCall& withPointerParameter(const SimpleString& , void*) _override;
	virtual MockFunctionCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) _override;
	virtual MockFunctionCall& ignoreOtherParameters() _override;

	virtual MockFunctionCall& andReturnValue(int) _override;
	virtual MockFunctionCall& andReturnValue(unsigned int) _override;
	virtual MockFunctionCall& andReturnValue(double) _override;
	virtual MockFunctionCall& andReturnValue(const char*) _override;
	virtual MockFunctionCall& andReturnValue(void*) _override;
	virtual MockFunctionCall& andReturnValue(const void*) _override;

	virtual bool hasReturnValue() _override;
	virtual MockNamedValue returnValue() _override;

	virtual MockFunctionCall& onObject(void* ) _override;

	virtual void add(MockFunctionCall& call);
	virtual void clear();
private:
	MockFunctionCallCompositeNode* head_;
};

class MockIgnoredCall : public MockFunctionCall
{
public:

	virtual MockFunctionCall& withName(const SimpleString&) _override { return *this;}
	virtual MockFunctionCall& withCallOrder(int) _override { return *this; }
	virtual MockFunctionCall& withIntParameter(const SimpleString&, int) _override { return *this; }
	virtual MockFunctionCall& withUnsignedIntParameter(const SimpleString&, unsigned int) _override{ return *this; }
	virtual MockFunctionCall& withDoubleParameter(const SimpleString&, double) _override { return *this; }
	virtual MockFunctionCall& withStringParameter(const SimpleString&, const char*) _override { return *this; }
	virtual MockFunctionCall& withPointerParameter(const SimpleString& , void*) _override { return *this; }
	virtual MockFunctionCall& withConstPointerParameter(const SimpleString& , const void*) _override { return *this; }
	virtual MockFunctionCall& withParameterOfType(const SimpleString&, const SimpleString&, const void*) _override { return *this; }

	virtual MockFunctionCall& andReturnValue(int) _override { return *this; }
	virtual MockFunctionCall& andReturnValue(unsigned int) _override { return *this; }
	virtual MockFunctionCall& andReturnValue(double) _override { return *this;}
	virtual MockFunctionCall& andReturnValue(const char*) _override { return *this; }
	virtual MockFunctionCall& andReturnValue(void*) _override { return *this; }
	virtual MockFunctionCall& andReturnValue(const void*) _override { return *this; }

	virtual bool hasReturnValue() _override { return false; }
	virtual MockNamedValue returnValue() _override { return MockNamedValue(""); }

	virtual MockFunctionCall& onObject(void* ) _override { return *this; }

	static MockFunctionCall& instance();
};

#endif
