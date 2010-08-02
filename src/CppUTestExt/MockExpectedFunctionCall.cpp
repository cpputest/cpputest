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

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"

struct MockParameterComparatorRepositoryNode
{
	MockParameterComparatorRepositoryNode(const SimpleString& name, MockParameterComparator& comparator, MockParameterComparatorRepositoryNode* next)
		: name_(name), comparator_(comparator), next_(next) {};
	SimpleString name_;
	MockParameterComparator& comparator_;
	MockParameterComparatorRepositoryNode* next_;
};

MockParameterComparatorRepository::MockParameterComparatorRepository() : head_(NULL)
{

}

MockParameterComparatorRepository::~MockParameterComparatorRepository()
{
	clear();
}

void MockParameterComparatorRepository::clear()
{
	while (head_) {
		MockParameterComparatorRepositoryNode* next = head_->next_;
		delete head_;
		head_ = next;
	}
}

void MockParameterComparatorRepository::installComparator(const SimpleString& name, MockParameterComparator& comparator)
{
	head_ = new MockParameterComparatorRepositoryNode(name, comparator, head_);
}

MockParameterComparator* MockParameterComparatorRepository::getComparatorForType(const SimpleString& name)
{
	for (MockParameterComparatorRepositoryNode* p = head_; p; p = p->next_)
			if (p->name_ == name) return &p->comparator_;
	return NULL;;
}

/* Type field works for now. Objects won't make it much better. Only these two switches, if more... change! */

bool MockExpectedFunctionCall::parametersEqual(MockFunctionParameterType type, const SimpleString& typeName, const MockParameterValue& p1, const MockParameterValue& p2)
{
	switch (type)
	{
		case MOCK_FUNCTION_PARAMETER_INT:
			return p1.intValue_ == p2.intValue_;
		case MOCK_FUNCTION_PARAMETER_STRING:
			return SimpleString(p1.stringValue_) == SimpleString(p2.stringValue_);
		case MOCK_FUNCTION_PARAMETER_POINTER:
			return p1.pointerValue_ == p2.pointerValue_;
		case MOCK_FUNCTION_PARAMETER_OBJECT:
			if (comparatorRepository_) {
				MockParameterComparator* comparator = comparatorRepository_->getComparatorForType(typeName);
				if (comparator) return comparator->isEqual(p1.objectPointerValue_, p2.objectPointerValue_);
			}
			return false;
		case MOCK_FUNCTION_PARAMETER_NONE:
		case MOCK_FUNCTION_PARAMETER_DOUBLE:
		default:
			FAIL("Not implemented");
			;
	}
	return false;
}

SimpleString StringFrom(MockFunctionParameterType type, const MockParameterValue& parameter, MockParameterComparator* comparator)
{
	switch (type)
	{
		case MOCK_FUNCTION_PARAMETER_INT:
			return StringFrom(parameter.intValue_);
		case MOCK_FUNCTION_PARAMETER_STRING:
			return parameter.stringValue_;
		case MOCK_FUNCTION_PARAMETER_POINTER:
			return StringFrom(parameter.pointerValue_);
		case MOCK_FUNCTION_PARAMETER_OBJECT:
				CHECK(comparator);
				return comparator->valueToString(parameter.objectPointerValue_);
		case MOCK_FUNCTION_PARAMETER_NONE:
		case MOCK_FUNCTION_PARAMETER_DOUBLE:
		default:
			FAIL("Not implemented");
			;
	}
	return "";
}


MockExpectedFunctionCall::MockExpectedFunctionCall()
	: parameters_(NULL), wasCallMade_(true), comparatorRepository_(NULL)
{
}

MockExpectedFunctionCall::~MockExpectedFunctionCall()
{
	while (parameters_) {
		MockFunctionParameter* next = parameters_->nextParameter;
		delete parameters_;
		parameters_ = next;
	}
}

void MockExpectedFunctionCall::setComparatorRepository(MockParameterComparatorRepository* repository)
{
	comparatorRepository_ = repository;
}

MockFunctionCall* MockExpectedFunctionCall::withName(const SimpleString& name)
{
	wasCallMade_ = false;
	name_ = name;
	return this;
}

MockFunctionCall* MockExpectedFunctionCall::withParameter(const SimpleString& name, int value)
{
	parameters_ = new MockFunctionParameter(name, MOCK_FUNCTION_PARAMETER_INT, parameters_);
	parameters_->value_.intValue_ = value;
	return this;
}

MockFunctionCall* MockExpectedFunctionCall::withParameter(const SimpleString& name, double value)
{
	parameters_ = new MockFunctionParameter(name, MOCK_FUNCTION_PARAMETER_DOUBLE, parameters_);
	parameters_->value_.doubleValue_ = value;
	return this;
}

MockFunctionCall* MockExpectedFunctionCall::withParameter(const SimpleString& name, const char* value)
{
	parameters_ = new MockFunctionParameter(name, MOCK_FUNCTION_PARAMETER_STRING, parameters_);
	parameters_->value_.stringValue_ = value;
	return this;
}

MockFunctionCall* MockExpectedFunctionCall::withParameter(const SimpleString& name, void* value)
{
	parameters_ = new MockFunctionParameter(name, MOCK_FUNCTION_PARAMETER_POINTER, parameters_);
	parameters_->value_.pointerValue_ = value;
	return this;
}

MockFunctionCall* MockExpectedFunctionCall::withParameterOfType(const SimpleString& typeName, const SimpleString& name, void* value)
{
	parameters_ = new MockFunctionParameter(name, MOCK_FUNCTION_PARAMETER_OBJECT, parameters_);
	parameters_->value_.objectPointerValue_ = value;
	parameters_->typeName_ = typeName;
	return this;
}

MockFunctionParameter* MockExpectedFunctionCall::getParameterByName(const SimpleString& name)
{
	for (MockFunctionParameter * p = parameters_; p; p = p->nextParameter)
		if (p->name_ == name)
			return p;
	return NULL;
}

MockFunctionParameterType MockExpectedFunctionCall::getParameterType(const SimpleString& name)
{
	MockFunctionParameter * p = getParameterByName(name);
	return (p) ? p->type_ : MOCK_FUNCTION_PARAMETER_NONE;
}

SimpleString MockExpectedFunctionCall::getParameterTypeName(const SimpleString& name)
{
	MockFunctionParameter * p = getParameterByName(name);
	return (p) ? p->typeName_ : "";
}

bool MockExpectedFunctionCall::hasParameterWithName(const SimpleString& name)
{
	MockFunctionParameter * p = getParameterByName(name);
	return p != NULL;
}

MockParameterValue MockExpectedFunctionCall::getParameterValue(const SimpleString& name)
{
	MockFunctionParameter * p = getParameterByName(name);
	if (p) return p->value_;

	MockParameterValue defaultValue;
	defaultValue.intValue_ = 0;
	return defaultValue;
}

SimpleString MockExpectedFunctionCall::getUnfulfilledParameterName() const
{
	for (MockFunctionParameter * p = parameters_; p; p = p->nextParameter)
		if (! p->fulfilled_)
			return p->name_;
	return "";
}

bool MockExpectedFunctionCall::parametersFulfilled()
{
	for (MockFunctionParameter * p = parameters_; p; p = p->nextParameter)
		if (! p->fulfilled_)
			return false;
	return true;
}

bool MockExpectedFunctionCall::isFulfilled()
{
	return wasCallMade_ && parametersFulfilled();
}

void MockExpectedFunctionCall::callWasMade()
{
	wasCallMade_ = true;
}

void MockExpectedFunctionCall::resetExpectation()
{
	wasCallMade_ = false;
	for (MockFunctionParameter * p = parameters_; p; p = p->nextParameter)
		p->fulfilled_ = false;
}

void MockExpectedFunctionCall::parameterWasPassed(const SimpleString& name)
{
	for (MockFunctionParameter * p = parameters_; p; p = p->nextParameter) {
		if (p->name_ == name)
			p->fulfilled_ = true;
	}
}

SimpleString MockExpectedFunctionCall::getParameterValueString(const SimpleString& name)
{
	MockFunctionParameter * p = getParameterByName(name);
	if (p->type_ == MOCK_FUNCTION_PARAMETER_OBJECT)
	{
		if (comparatorRepository_ == NULL)
			return "No comparator repository was set.";
		MockParameterComparator* comparator = comparatorRepository_->getComparatorForType(p->typeName_);
		if (comparator == NULL)
			return StringFromFormat("No comparator found for type: \"%s\"", p->typeName_.asCharString());
		return comparator->valueToString(p->value_.objectPointerValue_);
	}
	return (p) ? StringFrom(p->type_, p->value_) : "failed";
}

bool MockExpectedFunctionCall::hasParameter(const MockFunctionParameter& parameter)
{
	MockFunctionParameter * p = getParameterByName(parameter.name_);
	return (p) ? parametersEqual(p->type_, p->typeName_, p->value_, parameter.value_) : false;
}

SimpleString MockExpectedFunctionCall::toString()
{
	return name_;
}

bool MockExpectedFunctionCall::relatesTo(const SimpleString& functionName)
{
	return functionName == name_;
}
