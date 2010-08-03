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

union MockParameterValue{
	int intValue_;
	double doubleValue_;
	const char* stringValue_;
	void* pointerValue_;
	void* objectPointerValue_;
};

struct MockFunctionParameter
{
	MockFunctionParameter(const SimpleString& name, const SimpleString& type)
		: name_(name), type_(type), fulfilled_(false), nextParameter(NULL){}

	SimpleString name_;
	SimpleString type_;
	MockParameterValue value_;
	bool fulfilled_;
	MockFunctionParameter* nextParameter;
};


struct MockParameterComparatorRepositoryNode;
class MockParameterComparatorRepository
{
	MockParameterComparatorRepositoryNode* head_;
public:
	MockParameterComparatorRepository();
	virtual ~MockParameterComparatorRepository();

	virtual void installComparator(const SimpleString& name, MockParameterComparator& comparator);
	virtual MockParameterComparator* getComparatorForType(const SimpleString& name);

	void clear();
};

extern SimpleString StringFrom(const SimpleString& type, const MockParameterValue& parameter, MockParameterComparator* comparator = NULL);

class MockExpectedFunctionCall : public MockFunctionCall
{

public:
	MockExpectedFunctionCall();
	virtual ~MockExpectedFunctionCall();

	virtual void setComparatorRepository(MockParameterComparatorRepository* repository);

	virtual MockFunctionCall& withName(const SimpleString& name);
	virtual MockFunctionCall& withParameter(const SimpleString& name, int value);
	virtual MockFunctionCall& withParameter(const SimpleString& name, double value);
	virtual MockFunctionCall& withParameter(const SimpleString& name, const char* value);
	virtual MockFunctionCall& withParameter(const SimpleString& name, void* value);
	virtual MockFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, void* value);

	virtual SimpleString getParameterType(const SimpleString& name);
	virtual MockParameterValue getParameterValue(const SimpleString& name);
	virtual SimpleString getParameterValueString(const SimpleString& name);
	virtual SimpleString getUnfulfilledParameterName() const;

	virtual bool hasParameterWithName(const SimpleString& name);
	virtual bool hasParameter(const MockFunctionParameter& parameter);
	virtual bool relatesTo(const SimpleString& functionName);

	virtual bool isFulfilled();
	virtual bool parametersFulfilled();

	virtual void callWasMade();
	virtual void parameterWasPassed(const SimpleString& name);
	virtual void resetExpectation();

	virtual SimpleString toString();

private:
	SimpleString name_;
	bool wasCallMade_;

	MockFunctionParameter* parameters_;
	MockParameterComparatorRepository* comparatorRepository_;

	MockFunctionParameter* addNewParameter(const SimpleString& name, const SimpleString& type);
	bool parametersEqual(const SimpleString& type, const MockParameterValue& p1, const MockParameterValue& p2);
	MockFunctionParameter* getParameterByName(const SimpleString& name);

};

#endif
