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
class MockNamedValueComparator;
class MockNamedValueComparatorRepository;

#include "CppUTestExt/MockNamedValue.h"

class MockFunctionCall
{
public:
	MockFunctionCall();
	virtual ~MockFunctionCall();

	virtual MockFunctionCall& withName(const SimpleString& name)=0;
	virtual MockFunctionCall& withParameter(const SimpleString& name, int value)=0;
	virtual MockFunctionCall& withParameter(const SimpleString& name, double value)=0;
	virtual MockFunctionCall& withParameter(const SimpleString& name, const char* value)=0;
	virtual MockFunctionCall& withParameter(const SimpleString& name, void* value)=0;
	virtual MockFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, void* value)=0;

	virtual MockFunctionCall& andReturnValue(int value)=0;
	virtual MockFunctionCall& andReturnValue(double value)=0;
	virtual MockFunctionCall& andReturnValue(const char* value)=0;
	virtual MockFunctionCall& andReturnValue(void* value)=0;
	virtual MockNamedValue returnValue()=0;

	virtual void setComparatorRepository(MockNamedValueComparatorRepository* repository);

protected:
	void setName(const SimpleString& name);
	SimpleString getName() const;
	MockNamedValueComparator* getComparatorForType(const SimpleString& type) const;
private:
	SimpleString functionName_;
	MockNamedValueComparatorRepository* comparatorRepository_;
};

class MockIgnoredCall : public MockFunctionCall
{
public:
	virtual MockFunctionCall& withName(const SimpleString&) { return *this;}
	virtual MockFunctionCall& withParameter(const SimpleString&, int) { return *this; }
	virtual MockFunctionCall& withParameter(const SimpleString&, double) { return *this; }
	virtual MockFunctionCall& withParameter(const SimpleString&, const char*) { return *this; }
	virtual MockFunctionCall& withParameter(const SimpleString& , void*) { return *this; }
	virtual MockFunctionCall& withParameterOfType(const SimpleString&, const SimpleString&, void*) { return *this; }

	virtual MockFunctionCall& andReturnValue(int) { return *this; }
	virtual MockFunctionCall& andReturnValue(double) { return *this;}
	virtual MockFunctionCall& andReturnValue(const char*) { return *this; }
	virtual MockFunctionCall& andReturnValue(void*) { return *this; }
	virtual MockNamedValue returnValue() { return MockNamedValue(""); }

	static MockFunctionCall& instance() { static MockIgnoredCall call; return call; };
};

#endif
