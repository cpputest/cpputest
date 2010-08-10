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

bool MockExpectedFunctionCall::parametersEqual(const MockNamedValue& p1, const MockNamedValue& p2)
{
	return p1.equals(p2);
}

SimpleString StringFrom(const MockNamedValue& parameter)
{
	return parameter.toString();
}

MockExpectedFunctionCall::MockExpectedFunctionCall()
	: wasCallMade_(true), parameters_(NULL)
{
}

MockExpectedFunctionCall::~MockExpectedFunctionCall()
{
	while (parameters_) {
		MockFunctionParameterNode* next = parameters_->next_;
		delete parameters_->data_;
		delete parameters_;
		parameters_ = next;
	}
}

MockFunctionCall& MockExpectedFunctionCall::withName(const SimpleString& name)
{
	setName(name);
	wasCallMade_ = false;
	return *this;
}


MockNamedValue* MockExpectedFunctionCall::addNewParameter(const SimpleString& name)
{
	MockExpectedFunctionParameter* newParameter = new MockExpectedFunctionParameter(name);
	MockFunctionParameterNode* newParameterNode = new MockFunctionParameterNode(newParameter);

	if (parameters_ == NULL)
		parameters_ = newParameterNode;
	else {
		MockFunctionParameterNode* lastParameter = parameters_;
		while (lastParameter->next_) lastParameter = lastParameter->next_;
		lastParameter->next_ = newParameterNode;
	}
	return newParameter;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, int value)
{
	MockNamedValue* newParameter = addNewParameter(name);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, double value)
{
	MockNamedValue* newParameter = addNewParameter(name);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, const char* value)
{
	MockNamedValue* newParameter = addNewParameter(name);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, void* value)
{
	MockNamedValue* newParameter = addNewParameter(name);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameterOfType(const SimpleString& type, const SimpleString& name, void* value)
{
	MockNamedValue* newParameter = addNewParameter(name);
	newParameter->setObjectPointer(type, value);
	newParameter->setComparator(getComparatorForType(type));
	return *this;
}

MockNamedValue* MockExpectedFunctionCall::getParameterByName(const SimpleString& name)
{
	for (MockFunctionParameterNode * p = parameters_; p; p = p->next_)
		if (p->data_->getName() == name)
			return p->data_;
	return NULL;
}

SimpleString MockExpectedFunctionCall::getParameterType(const SimpleString& name)
{
	MockNamedValue * p = getParameterByName(name);
	return (p) ? p->getType() : "";
}

bool MockExpectedFunctionCall::hasParameterWithName(const SimpleString& name)
{
	MockNamedValue * p = getParameterByName(name);
	return p != NULL;
}

MockNamedValue MockExpectedFunctionCall::getParameter(const SimpleString& name)
{
	MockNamedValue* parameter = getParameterByName(name);
	return (parameter) ? *parameter : MockNamedValue("");
}

bool MockExpectedFunctionCall::areParametersFulfilled()
{
	for (MockFunctionParameterNode * p = parameters_; p; p = p->next_)
		if (! p->data_->fulfilled_)
			return false;
	return true;
}

bool MockExpectedFunctionCall::isFulfilled()
{
	return wasCallMade_ && areParametersFulfilled();
}

void MockExpectedFunctionCall::callWasMade()
{
	wasCallMade_ = true;
}

void MockExpectedFunctionCall::resetExpectation()
{
	wasCallMade_ = false;
	for (MockFunctionParameterNode * p = parameters_; p; p = p->next_)
		p->data_->fulfilled_ = false;
}

void MockExpectedFunctionCall::parameterWasPassed(const SimpleString& name)
{
	for (MockFunctionParameterNode * p = parameters_; p; p = p->next_) {
		if (p->data_->getName() == name)
			p->data_->fulfilled_ = true;
	}
}

SimpleString MockExpectedFunctionCall::getParameterValueString(const SimpleString& name)
{
	MockNamedValue * p = getParameterByName(name);
	return (p) ? StringFrom(*p) : "failed";
}

bool MockExpectedFunctionCall::hasParameter(const MockNamedValue& parameter)
{
	MockNamedValue * p = getParameterByName(parameter.getName());
	return (p) ? parametersEqual(*p, parameter) : false;
}

SimpleString MockExpectedFunctionCall::callToString()
{
	SimpleString str;
	str += getName();
	str += " -> ";
	if (parameters_ == NULL)
		str += "no parameters";
	for (MockFunctionParameterNode * p = parameters_; p; p = p->next_) {
		str += StringFromFormat("%s %s: <%s>", p->data_->getType().asCharString(), p->data_->getName().asCharString(), getParameterValueString(p->data_->getName()).asCharString());
		if (p->next_) str += ", ";
	}
	return str;
}

SimpleString MockExpectedFunctionCall::missingParametersToString()
{
	SimpleString str;
	for (MockFunctionParameterNode * p = parameters_; p; p = p->next_) {
		if (! p->data_->fulfilled_) {
			if (str != "") str += ", ";
			str += StringFromFormat("%s %s", p->data_->getType().asCharString(), p->data_->getName().asCharString());
		}
	}
	return str;
}

bool MockExpectedFunctionCall::relatesTo(const SimpleString& functionName)
{
	return functionName == getName();
}
