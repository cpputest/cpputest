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

enum MockFunctionParameterType
{
	MOCK_FUNCTION_PARAMETER_NONE,
	MOCK_FUNCTION_PARAMETER_INT,
	MOCK_FUNCTION_PARAMETER_DOUBLE,
	MOCK_FUNCTION_PARAMETER_STRING,
	MOCK_FUNCTION_PARAMETER_POINTER
};

union MockParameterValue{
	int intValue_;
	double doubleValue_;
	const char* stringValue_;
	void* pointerValue_;
};

struct MockFunctionParameter
{
	MockFunctionParameter(const SimpleString& name, MockFunctionParameterType type, MockFunctionParameter* next)
	: name_(name), type_(type), nextParameter(next){}

	SimpleString name_;
	MockFunctionParameterType type_;
	MockParameterValue value_;
	MockFunctionParameter* nextParameter;
};

class MockExpectedFunctionCall : public MockFunctionCall
{
	SimpleString name_;
	MockFunctionParameter* parameters_;
	bool fulfilled_;

public:
	MockExpectedFunctionCall();
	virtual ~MockExpectedFunctionCall();

	virtual MockFunctionCall* withName(const SimpleString& name);
	virtual MockFunctionCall* withParameter(const SimpleString& name, int value);
	virtual MockFunctionCall* withParameter(const SimpleString& name, double value);
	virtual MockFunctionCall* withParameter(const SimpleString& name, const char* value);
	virtual MockFunctionCall* withParameter(const SimpleString& name, void* value);

	virtual MockFunctionParameterType getParameterType(const SimpleString& name);
	virtual MockParameterValue getParameterValue(const SimpleString& name);

	bool relatesTo(const SimpleString& functionName);
	bool isFulfilled();
	void setFulfilled();

	SimpleString toString();
};

#endif
