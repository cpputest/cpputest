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

CheckedMockExpectedFunctionCall::CheckedMockExpectedFunctionCall()
	: ignoreOtherParameters_(false), parametersWereIgnored_(false), callOrder_(0), expectedCallOrder_(NO_EXPECTED_CALL_ORDER), outOfOrder_(true), returnValue_(""), objectPtr_(NULL), wasPassedToObject_(true)
{
	parameters_ = new MockNamedValueList();
}

CheckedMockExpectedFunctionCall::~CheckedMockExpectedFunctionCall()
{
	parameters_->clear();
	delete parameters_;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::withName(const SimpleString& name)
{
	setName(name);
	callOrder_ = NOT_CALLED_YET;
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::withIntParameter(const SimpleString& name, int value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::withDoubleParameter(const SimpleString& name, double value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::withStringParameter(const SimpleString& name, const char* value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::withPointerParameter(const SimpleString& name, void* value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::withConstPointerParameter(const SimpleString& name, const void* value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value)
{
	MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
	parameters_->add(newParameter);
	newParameter->setObjectPointer(type, value);
	newParameter->setComparator(getComparatorForType(type));
	return *this;
}

SimpleString CheckedMockExpectedFunctionCall::getParameterType(const SimpleString& name)
{
	MockNamedValue * p = parameters_->getValueByName(name);
	return (p) ? p->getType() : "";
}

bool CheckedMockExpectedFunctionCall::hasParameterWithName(const SimpleString& name)
{
	MockNamedValue * p = parameters_->getValueByName(name);
	return p != NULL;
}

MockNamedValue CheckedMockExpectedFunctionCall::getParameter(const SimpleString& name)
{
	MockNamedValue * p = parameters_->getValueByName(name);
	return (p) ? *p : MockNamedValue("");
}

bool CheckedMockExpectedFunctionCall::areParametersFulfilled()
{
	for (MockNamedValueListNode* p = parameters_->begin(); p; p = p->next())
		if (! item(p)->isFulfilled())
			return false;
	return true;
}

bool CheckedMockExpectedFunctionCall::areIgnoredParametersFulfilled()
{
	if (ignoreOtherParameters_)
		return parametersWereIgnored_;
	return true;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::ignoreOtherParameters()
{
	ignoreOtherParameters_ = true;
	return *this;
}

bool CheckedMockExpectedFunctionCall::isFulfilled()
{
	return isFulfilledWithoutIgnoredParameters() && areIgnoredParametersFulfilled();
}

bool CheckedMockExpectedFunctionCall::isFulfilledWithoutIgnoredParameters()
{
	return callOrder_ != NOT_CALLED_YET && areParametersFulfilled() && wasPassedToObject_;
}


void CheckedMockExpectedFunctionCall::callWasMade(int callOrder)
{
	callOrder_ = callOrder;
	if (expectedCallOrder_ == NO_EXPECTED_CALL_ORDER)
		outOfOrder_ = false;
	else if (callOrder_ == expectedCallOrder_)
		outOfOrder_ = false;
	else
		outOfOrder_ = true;
}

void CheckedMockExpectedFunctionCall::parametersWereIgnored()
{
	parametersWereIgnored_ = true;
}


void CheckedMockExpectedFunctionCall::wasPassedToObject()
{
	wasPassedToObject_ = true;
}

void CheckedMockExpectedFunctionCall::resetExpectation()
{
	callOrder_ = NOT_CALLED_YET;
	wasPassedToObject_ = (objectPtr_ == NULL);
	for (MockNamedValueListNode* p = parameters_->begin(); p; p = p->next())
		item(p)->setFulfilled(false);
}

void CheckedMockExpectedFunctionCall::parameterWasPassed(const SimpleString& name)
{
	for (MockNamedValueListNode* p = parameters_->begin(); p; p = p->next()) {
		if (p->getName() == name)
			item(p)->setFulfilled(true);
	}
}

SimpleString CheckedMockExpectedFunctionCall::getParameterValueString(const SimpleString& name)
{
	MockNamedValue * p = parameters_->getValueByName(name);
	return (p) ? StringFrom(*p) : "failed";
}

bool CheckedMockExpectedFunctionCall::hasParameter(const MockNamedValue& parameter)
{
	MockNamedValue * p = parameters_->getValueByName(parameter.getName());
	return (p) ? p->equals(parameter) : ignoreOtherParameters_;
}

SimpleString CheckedMockExpectedFunctionCall::callToString()
{
	SimpleString str;
	if (objectPtr_)
		str = StringFromFormat("(object address: %p)::", objectPtr_);

	str += getName();
	str += " -> ";
	if (expectedCallOrder_ != NO_EXPECTED_CALL_ORDER) {
		str += StringFromFormat("expected call order: <%d> -> ", expectedCallOrder_);
	}

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

SimpleString CheckedMockExpectedFunctionCall::missingParametersToString()
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

bool CheckedMockExpectedFunctionCall::relatesTo(const SimpleString& functionName)
{
	return functionName == getName();
}

bool CheckedMockExpectedFunctionCall::relatesToObject(void*objectPtr) const
{
	return objectPtr_ == objectPtr;
}

CheckedMockExpectedFunctionCall::MockExpectedFunctionParameter* CheckedMockExpectedFunctionCall::item(MockNamedValueListNode* node)
{
	return (MockExpectedFunctionParameter*) node->item();
}

CheckedMockExpectedFunctionCall::MockExpectedFunctionParameter::MockExpectedFunctionParameter(const SimpleString& name)
			: MockNamedValue(name), fulfilled_(false)
{
}

void CheckedMockExpectedFunctionCall::MockExpectedFunctionParameter::setFulfilled(bool b)
{
	fulfilled_ = b;
}

bool CheckedMockExpectedFunctionCall::MockExpectedFunctionParameter::isFulfilled() const
{
	return fulfilled_;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::andReturnValue(unsigned int value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::andReturnValue(int value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::andReturnValue(const char* value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::andReturnValue(double value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::andReturnValue(void* value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::andReturnValue(const void* value)
{
	returnValue_.setName("returnValue");
	returnValue_.setValue(value);
	return *this;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::onObject(void* objectPtr)
{
	wasPassedToObject_ = false;
	objectPtr_ = objectPtr;
	return *this;
}

bool CheckedMockExpectedFunctionCall::hasReturnValue()
{
	return ! returnValue_.getName().isEmpty();
}

MockNamedValue CheckedMockExpectedFunctionCall::returnValue()
{
	return returnValue_;
}

int CheckedMockExpectedFunctionCall::getCallOrder() const
{
	return callOrder_;
}

MockFunctionCall& CheckedMockExpectedFunctionCall::withCallOrder(int callOrder)
{
	expectedCallOrder_ = callOrder;
	return *this;
}

bool CheckedMockExpectedFunctionCall::isOutOfOrder() const
{
	return outOfOrder_;
}

struct MockFunctionCallCompositeNode
{
	MockFunctionCallCompositeNode(MockFunctionCall& functionCall, MockFunctionCallCompositeNode* next) : next_(next), call_(functionCall){}

	MockFunctionCallCompositeNode* next_;
	MockFunctionCall& call_;
};

MockFunctionCallComposite::MockFunctionCallComposite() : head_(NULL)
{
}

MockFunctionCallComposite::~MockFunctionCallComposite()
{

}

void MockFunctionCallComposite::add(MockFunctionCall& call)
{
	head_ = new MockFunctionCallCompositeNode(call, head_);
}

void MockFunctionCallComposite::clear()
{
	while (head_) {
		MockFunctionCallCompositeNode* next = head_->next_;
		delete head_;
		head_ = next;
	}
}

MockFunctionCall& MockFunctionCallComposite::withName(const SimpleString& name)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.withName(name);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::withCallOrder(int)
{
	FAIL("withCallOrder not supported for CompositeCalls");
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.withParameter(name, value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::withIntParameter(const SimpleString& name, int value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.withParameter(name, value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::withDoubleParameter(const SimpleString& name, double value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.withParameter(name, value);
	return *this;

}

MockFunctionCall& MockFunctionCallComposite::withStringParameter(const SimpleString& name, const char* value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.withParameter(name, value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::withPointerParameter(const SimpleString& name, void* value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.withParameter(name, value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::withConstPointerParameter(const SimpleString& name, const void* value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.withParameter(name, value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.withParameterOfType(typeName, name, value);
	return *this;

}

MockFunctionCall& MockFunctionCallComposite::ignoreOtherParameters()
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.ignoreOtherParameters();
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::andReturnValue(unsigned int value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.andReturnValue(value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::andReturnValue(int value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.andReturnValue(value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::andReturnValue(double value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.andReturnValue(value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::andReturnValue(const char* value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.andReturnValue(value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::andReturnValue(void* value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.andReturnValue(value);
	return *this;
}

MockFunctionCall& MockFunctionCallComposite::andReturnValue(const void* value)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.andReturnValue(value);
	return *this;
}

bool MockFunctionCallComposite::hasReturnValue()
{
	return head_->call_.hasReturnValue();
}

MockNamedValue MockFunctionCallComposite::returnValue()
{
	return head_->call_.returnValue();
}

MockFunctionCall& MockFunctionCallComposite::onObject(void* object)
{
	for (MockFunctionCallCompositeNode* node = head_; node != NULL; node = node->next_)
		node->call_.onObject(object);
	return *this;
}
