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
#include "CppUTest/PlatformSpecificFunctions.h"

bool MockExpectedFunctionCall::parametersEqual(const SimpleString& type, const MockParameterValue& p1, const MockParameterValue& p2)
{
	if (type == "int")
		return p1.intValue_ == p2.intValue_;
	else if (type == "char*")
		return SimpleString(p1.stringValue_) == SimpleString(p2.stringValue_);
	else if (type == "void*")
		return p1.pointerValue_ == p2.pointerValue_;
	else if (type == "double")
		return (PlatformSpecificFabs(p1.doubleValue_ - p2.doubleValue_) < 0.005);

	MockParameterComparator* comparator = getComparatorForType(type);
	if (comparator)
		return comparator->isEqual(p1.objectPointerValue_, p2.objectPointerValue_);

	return false;
}

SimpleString StringFrom(const SimpleString& type, const MockParameterValue& parameter, MockParameterComparator* comparator)
{
	if (type == "int")
		return StringFrom(parameter.intValue_);
	else if (type == "char*")
		return parameter.stringValue_;
	else if (type == "void*")
		return StringFrom(parameter.pointerValue_);
	else if (type == "double")
		return StringFrom(parameter.doubleValue_);

	if (comparator == NULL)
		return StringFromFormat("No comparator found for type: \"%s\"", type.asCharString());
	return comparator->valueToString(parameter.objectPointerValue_);
}

MockExpectedFunctionCall::MockExpectedFunctionCall()
	: wasCallMade_(true), parameters_(NULL)
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

MockFunctionCall& MockExpectedFunctionCall::withName(const SimpleString& name)
{
	setName(name);
	wasCallMade_ = false;
	return *this;
}


MockFunctionParameter* MockExpectedFunctionCall::addNewParameter(const SimpleString& name, const SimpleString& type)
{
	MockFunctionParameter* newParameter = new MockFunctionParameter(name, type);

	if (parameters_ == NULL)
		parameters_ = newParameter;
	else {
		MockFunctionParameter* lastParameter = parameters_;
		while (lastParameter->nextParameter) lastParameter = lastParameter->nextParameter;
		lastParameter->nextParameter = newParameter;
	}
	return newParameter;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, int value)
{
	MockFunctionParameter* newParameter = addNewParameter(name, "int");
	newParameter->value_.intValue_ = value;
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, double value)
{
	MockFunctionParameter* newParameter = addNewParameter(name, "double");
	newParameter->value_.doubleValue_ = value;
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, const char* value)
{
	MockFunctionParameter* newParameter = addNewParameter(name, "char*");
	newParameter->value_.stringValue_ = value;
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, void* value)
{
	MockFunctionParameter* newParameter = addNewParameter(name, "void*");
	newParameter->value_.pointerValue_ = value;
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameterOfType(const SimpleString& type, const SimpleString& name, void* value)
{
	MockFunctionParameter* newParameter = addNewParameter(name, type);
	newParameter->value_.objectPointerValue_ = value;
	return *this;
}

MockFunctionParameter* MockExpectedFunctionCall::getParameterByName(const SimpleString& name)
{
	for (MockFunctionParameter * p = parameters_; p; p = p->nextParameter)
		if (p->name_ == name)
			return p;
	return NULL;
}

SimpleString MockExpectedFunctionCall::getParameterType(const SimpleString& name)
{
	MockFunctionParameter * p = getParameterByName(name);
	return (p) ? p->type_ : "";
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

bool MockExpectedFunctionCall::areParametersFulfilled()
{
	for (MockFunctionParameter * p = parameters_; p; p = p->nextParameter)
		if (! p->fulfilled_)
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
	return (p) ? StringFrom(p->type_, p->value_, getComparatorForType(p->type_)) : "failed";
}

bool MockExpectedFunctionCall::hasParameter(const MockFunctionParameter& parameter)
{
	MockFunctionParameter * p = getParameterByName(parameter.name_);
	return (p) ? parametersEqual(p->type_, p->value_, parameter.value_) : false;
}

SimpleString MockExpectedFunctionCall::callToString()
{
	SimpleString str;
	str += getName();
	str += " -> ";
	if (parameters_ == NULL)
		str += "no parameters";
	for (MockFunctionParameter * p = parameters_; p; p = p->nextParameter) {
		str += StringFromFormat("%s %s: <%s>", p->type_.asCharString(), p->name_.asCharString(), getParameterValueString(p->name_).asCharString());
		if (p->nextParameter) str += ", ";
	}
	return str;
}

SimpleString MockExpectedFunctionCall::missingParametersToString()
{
	SimpleString str;
	for (MockFunctionParameter * p = parameters_; p; p = p->nextParameter) {
		if (! p->fulfilled_) {
			if (str != "") str += ", ";
			str += StringFromFormat("%s %s", p->type_.asCharString(), p->name_.asCharString());
		}
	}
	return str;
}

bool MockExpectedFunctionCall::relatesTo(const SimpleString& functionName)
{
	return functionName == getName();
}
