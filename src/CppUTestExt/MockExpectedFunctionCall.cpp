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

SimpleString StringFrom(const MockNamedValue& parameter)
{
	return parameter.toString();
}

MockExpectedFunctionCall::MockExpectedFunctionCall()
	: ignoreOtherParameters_(false), parametersWereIgnored_(false), wasCallMade_(true), returnValue_(""), objectPtr_(NULL), wasPassedToObject_(true)
{
	parameters_ = new MockNamedValueList();
}

MockExpectedFunctionCall::~MockExpectedFunctionCall()
{
	parameters_->clear();
	delete parameters_;
}

MockFunctionCall& MockExpectedFunctionCall::withName(const SimpleString& name)
{
	setName(name);
	wasCallMade_ = false;
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, int value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, double value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, const char* value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameter(const SimpleString& name, void* value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::withParameterOfType(const SimpleString& type, const SimpleString& name, void* value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setObjectPointer(type, value);
	newParameter->setComparator(getComparatorForType(type));
	return *this;
}

SimpleString MockExpectedFunctionCall::getParameterType(const SimpleString& name)
{
	MockNamedValue * p = parameters_->getValueByName(name);
	return (p) ? p->getType() : "";
}

bool MockExpectedFunctionCall::hasParameterWithName(const SimpleString& name)
{
	MockNamedValue * p = parameters_->getValueByName(name);
	return p != NULL;
}

MockNamedValue MockExpectedFunctionCall::getParameter(const SimpleString& name)
{
	MockNamedValue * p = parameters_->getValueByName(name);
	return (p) ? *p : MockNamedValue("");
}

bool MockExpectedFunctionCall::areParametersFulfilled()
{
	for (MockNamedValueListNode* p = parameters_->begin(); p; p = p->next())
		if (! item(p)->isFulfilled())
			return false;
	return true;
}

bool MockExpectedFunctionCall::areIgnoredParametersFulfilled()
{
	if (ignoreOtherParameters_)
		return parametersWereIgnored_;
	return true;
}

MockFunctionCall& MockExpectedFunctionCall::ignoreOtherParameters()
{
	ignoreOtherParameters_ = true;
	return *this;
}

bool MockExpectedFunctionCall::isFulfilled()
{
	return isFulfilledWithoutIgnoredParameters() && areIgnoredParametersFulfilled();
}

bool MockExpectedFunctionCall::isFulfilledWithoutIgnoredParameters()
{
	return wasCallMade_ && areParametersFulfilled() && wasPassedToObject_;
}


void MockExpectedFunctionCall::callWasMade()
{
	wasCallMade_ = true;
}

void MockExpectedFunctionCall::parametersWereIgnored()
{
	parametersWereIgnored_ = true;
}


void MockExpectedFunctionCall::wasPassedToObject()
{
	wasPassedToObject_ = true;
}

void MockExpectedFunctionCall::resetExpectation()
{
	wasCallMade_ = false;
	wasPassedToObject_ = (objectPtr_ == NULL);
	for (MockNamedValueListNode* p = parameters_->begin(); p; p = p->next())
		item(p)->setFulfilled(false);
}

void MockExpectedFunctionCall::parameterWasPassed(const SimpleString& name)
{
	for (MockNamedValueListNode* p = parameters_->begin(); p; p = p->next()) {
		if (p->getName() == name)
			item(p)->setFulfilled(true);
	}
}

SimpleString MockExpectedFunctionCall::getParameterValueString(const SimpleString& name)
{
	MockNamedValue * p = parameters_->getValueByName(name);
	return (p) ? StringFrom(*p) : "failed";
}

bool MockExpectedFunctionCall::hasParameter(const MockNamedValue& parameter)
{
	MockNamedValue * p = parameters_->getValueByName(parameter.getName());
	return (p) ? p->equals(parameter) : ignoreOtherParameters_;
}

SimpleString MockExpectedFunctionCall::callToString()
{
	SimpleString str;
	if (objectPtr_)
		str = StringFromFormat("(object address: %p)::", objectPtr_);

	str += getName();
	str += " -> ";
	if (parameters_->begin() == NULL) {
		str += (ignoreOtherParameters_) ? "all parameters ignored" : "no parameters";
		return str;
	}

	for (MockNamedValueListNode* p = parameters_->begin(); p; p = p->next()) {
		str += StringFromFormat("%s %s: <%s>", p->getType().asCharString(), p->getName().asCharString(), getParameterValueString(p->getName()).asCharString());
		if (p->next()) str += ", ";
	}
	if (ignoreOtherParameters_)
		str += ", other parameters are ignored";
	return str;
}

SimpleString MockExpectedFunctionCall::missingParametersToString()
{
	SimpleString str;
	for (MockNamedValueListNode* p = parameters_->begin(); p; p = p->next()) {
		if (! item(p)->isFulfilled()) {
			if (str != "") str += ", ";
			str += StringFromFormat("%s %s", p->getType().asCharString(), p->getName().asCharString());
		}
	}
	return str;
}

bool MockExpectedFunctionCall::relatesTo(const SimpleString& functionName)
{
	return functionName == getName();
}

bool MockExpectedFunctionCall::relatesToObject(void*objectPtr) const
{
	return objectPtr_ == objectPtr;
}

MockExpectedFunctionCall::MockExpectedFunctionParameter* MockExpectedFunctionCall::item(MockNamedValueListNode* node)
{
	return (MockExpectedFunctionParameter*) node->item();
}

MockExpectedFunctionCall::MockExpectedFunctionParameter::MockExpectedFunctionParameter(const SimpleString& name)
			: MockNamedValue(name), fulfilled_(false)
{
}

void MockExpectedFunctionCall::MockExpectedFunctionParameter::setFulfilled(bool b)
{
	fulfilled_ = b;
}

bool MockExpectedFunctionCall::MockExpectedFunctionParameter::isFulfilled() const
{
	return fulfilled_;
}

MockFunctionCall& MockExpectedFunctionCall::andReturnValue(int value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::andReturnValue(const char* value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::andReturnValue(double value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::andReturnValue(void* value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& MockExpectedFunctionCall::onObject(void* objectPtr)
{
	wasPassedToObject_ = false;
	objectPtr_ = objectPtr;
	return *this;
}

bool MockExpectedFunctionCall::hasReturnValue()
{
	return ! returnValue_.getName().isEmpty();
}

MockNamedValue MockExpectedFunctionCall::returnValue()
{
	return returnValue_;
}



