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
#include "CppUTestExt/MockCheckedExpectedCall.h"

MockExpectedCall::MockExpectedCall()
{
}

MockExpectedCall::~MockExpectedCall()
{
}

SimpleString StringFrom(const MockNamedValue& parameter)
{
    return parameter.toString();
}

void MockCheckedExpectedCall::setName(const SimpleString& name)
{
    functionName_ = name;
}

SimpleString MockCheckedExpectedCall::getName() const
{
    return functionName_;
}

MockCheckedExpectedCall::MockCheckedExpectedCall()
    : ignoreOtherParameters_(false), isActualCallMatchFinalized_(false), actualCallOrder_(0), expectedCallOrder_(NO_EXPECTED_CALL_ORDER), outOfOrder_(true), returnValue_(""), objectPtr_(NULL), wasPassedToObject_(true)
{
    inputParameters_ = new MockNamedValueList();
    outputParameters_ = new MockNamedValueList();
}

MockCheckedExpectedCall::~MockCheckedExpectedCall()
{
    inputParameters_->clear();
    delete inputParameters_;
    outputParameters_->clear();
    delete outputParameters_;
}

MockExpectedCall& MockCheckedExpectedCall::withName(const SimpleString& name)
{
    setName(name);
    actualCallOrder_ = NOT_CALLED_YET;
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withBoolParameter(const SimpleString& name, bool value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withIntParameter(const SimpleString& name, int value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withLongIntParameter(const SimpleString& name, long int value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withDoubleParameter(const SimpleString& name, double value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withStringParameter(const SimpleString& name, const char* value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withPointerParameter(const SimpleString& name, void* value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withConstPointerParameter(const SimpleString& name, const void* value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withFunctionPointerParameter(const SimpleString& name, void (*value)())
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setMemoryBuffer(value, size);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setObjectPointer(type, value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withOutputParameterReturning(const SimpleString& name, const void* value, size_t size)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    outputParameters_->add(newParameter);
    newParameter->setValue(value);
    newParameter->setSize(size);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withOutputParameterOfTypeReturning(const SimpleString& type, const SimpleString& name, const void* value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    outputParameters_->add(newParameter);
    newParameter->setObjectPointer(type, value);
    return *this;
}

SimpleString MockCheckedExpectedCall::getInputParameterType(const SimpleString& name)
{
    MockNamedValue * p = inputParameters_->getValueByName(name);
    return (p) ? p->getType() : "";
}

bool MockCheckedExpectedCall::hasInputParameterWithName(const SimpleString& name)
{
    MockNamedValue * p = inputParameters_->getValueByName(name);
    return p != NULL;
}

bool MockCheckedExpectedCall::hasOutputParameterWithName(const SimpleString& name)
{
    MockNamedValue * p = outputParameters_->getValueByName(name);
    return p != NULL;
}

MockNamedValue MockCheckedExpectedCall::getInputParameter(const SimpleString& name)
{
    MockNamedValue * p = inputParameters_->getValueByName(name);
    return (p) ? *p : MockNamedValue("");
}

MockNamedValue MockCheckedExpectedCall::getOutputParameter(const SimpleString& name)
{
    MockNamedValue * p = outputParameters_->getValueByName(name);
    return (p) ? *p : MockNamedValue("");
}

bool MockCheckedExpectedCall::areParametersMatchingActualCall()
{
    MockNamedValueListNode* p;
    for (p = inputParameters_->begin(); p; p = p->next())
        if (! item(p)->isMatchingActualCall())
            return false;
    for (p = outputParameters_->begin(); p; p = p->next())
        if (! item(p)->isMatchingActualCall())
            return false;
    return true;
}

MockExpectedCall& MockCheckedExpectedCall::ignoreOtherParameters()
{
    ignoreOtherParameters_ = true;
    return *this;
}

bool MockCheckedExpectedCall::isFulfilled()
{
    return isMatchingActualCallAndFinalized();
}

bool MockCheckedExpectedCall::canMatchActualCalls()
{
    return !isFulfilled();
}

bool MockCheckedExpectedCall::isMatchingActualCallAndFinalized()
{
    return isMatchingActualCall() && (!ignoreOtherParameters_ || isActualCallMatchFinalized_);
}

bool MockCheckedExpectedCall::isMatchingActualCall()
{
    return (actualCallOrder_ != NOT_CALLED_YET) && areParametersMatchingActualCall() && wasPassedToObject_;
}

void MockCheckedExpectedCall::callWasMade(unsigned int callOrder)
{
    actualCallOrder_ = callOrder;
    if (expectedCallOrder_ == NO_EXPECTED_CALL_ORDER)
        outOfOrder_ = false;
    else if (actualCallOrder_ == expectedCallOrder_)
        outOfOrder_ = false;
    else
        outOfOrder_ = true;
}

void MockCheckedExpectedCall::finalizeActualCallMatch()
{
    isActualCallMatchFinalized_ = true;
}


void MockCheckedExpectedCall::wasPassedToObject()
{
    wasPassedToObject_ = true;
}

void MockCheckedExpectedCall::resetActualCallMatchingState()
{
    actualCallOrder_ = NOT_CALLED_YET;
    wasPassedToObject_ = (objectPtr_ == NULL);
    isActualCallMatchFinalized_ = false;

    MockNamedValueListNode* p;

    for (p = inputParameters_->begin(); p; p = p->next())
        item(p)->setMatchesActualCall(false);
    for (p = outputParameters_->begin(); p; p = p->next())
        item(p)->setMatchesActualCall(false);
}

void MockCheckedExpectedCall::inputParameterWasPassed(const SimpleString& name)
{
    for (MockNamedValueListNode* p = inputParameters_->begin(); p; p = p->next()) {
        if (p->getName() == name)
            item(p)->setMatchesActualCall(true);
    }
}

void MockCheckedExpectedCall::outputParameterWasPassed(const SimpleString& name)
{
    for (MockNamedValueListNode* p = outputParameters_->begin(); p; p = p->next()) {
        if (p->getName() == name)
            item(p)->setMatchesActualCall(true);
    }
}

SimpleString MockCheckedExpectedCall::getInputParameterValueString(const SimpleString& name)
{
    MockNamedValue * p = inputParameters_->getValueByName(name);
    return (p) ? StringFrom(*p) : "failed";
}

bool MockCheckedExpectedCall::hasInputParameter(const MockNamedValue& parameter)
{
    MockNamedValue * p = inputParameters_->getValueByName(parameter.getName());
    return (p) ? p->equals(parameter) : ignoreOtherParameters_;
}

bool MockCheckedExpectedCall::hasOutputParameter(const MockNamedValue& parameter)
{
    MockNamedValue * p = outputParameters_->getValueByName(parameter.getName());
    return (p) ? p->compatibleForCopying(parameter) : ignoreOtherParameters_;
}

SimpleString MockCheckedExpectedCall::callToString()
{
    SimpleString str;
    if (objectPtr_)
        str = StringFromFormat("(object address: %p)::", objectPtr_);

    str += getName();
    str += " -> ";
    if (expectedCallOrder_ != NO_EXPECTED_CALL_ORDER) {
        str += StringFromFormat("expected call order: <%d> -> ", expectedCallOrder_);
    }

    if (inputParameters_->begin() == NULL && outputParameters_->begin() == NULL) {
        str += (ignoreOtherParameters_) ? "all parameters ignored" : "no parameters";
        return str;
    }

	MockNamedValueListNode* p;

    for (p = inputParameters_->begin(); p; p = p->next()) {
        str += StringFromFormat("%s %s: <%s>", p->getType().asCharString(), p->getName().asCharString(), getInputParameterValueString(p->getName()).asCharString());
        if (p->next()) str += ", ";
    }

    if (inputParameters_->begin() && outputParameters_->begin())
    {
        str += ", ";
    }

    for (p = outputParameters_->begin(); p; p = p->next()) {
        str += StringFromFormat("%s %s: <output>", p->getType().asCharString(), p->getName().asCharString());
        if (p->next()) str += ", ";
    }

    if (ignoreOtherParameters_)
        str += ", other parameters are ignored";
    return str;
}

SimpleString MockCheckedExpectedCall::missingParametersToString()
{
    SimpleString str;
	MockNamedValueListNode* p;

    for (p = inputParameters_->begin(); p; p = p->next()) {
        if (! item(p)->isMatchingActualCall()) {
            if (str != "") str += ", ";
            str += StringFromFormat("%s %s", p->getType().asCharString(), p->getName().asCharString());
        }
    }
    for (p = outputParameters_->begin(); p; p = p->next()) {
        if (! item(p)->isMatchingActualCall()) {
            if (str != "") str += ", ";
            str += StringFromFormat("%s %s", p->getType().asCharString(), p->getName().asCharString());
        }
    }
    return str;
}

bool MockCheckedExpectedCall::relatesTo(const SimpleString& functionName)
{
    return functionName == getName();
}

bool MockCheckedExpectedCall::relatesToObject(const void* objectPtr) const
{
    return objectPtr_ == objectPtr;
}

MockCheckedExpectedCall::MockExpectedFunctionParameter* MockCheckedExpectedCall::item(MockNamedValueListNode* node)
{
    return (MockExpectedFunctionParameter*) node->item();
}

MockCheckedExpectedCall::MockExpectedFunctionParameter::MockExpectedFunctionParameter(const SimpleString& name)
            : MockNamedValue(name), matchesActualCall_(false)
{
}

void MockCheckedExpectedCall::MockExpectedFunctionParameter::setMatchesActualCall(bool b)
{
    matchesActualCall_ = b;
}

bool MockCheckedExpectedCall::MockExpectedFunctionParameter::isMatchingActualCall() const
{
    return matchesActualCall_;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(bool value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(unsigned int value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(int value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(long int value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(unsigned long int value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(const char* value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(double value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(void* value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(const void* value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(void (*value)())
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::onObject(void* objectPtr)
{
    wasPassedToObject_ = false;
    objectPtr_ = objectPtr;
    return *this;
}

MockNamedValue MockCheckedExpectedCall::returnValue()
{
    return returnValue_;
}

unsigned int MockCheckedExpectedCall::getCallOrder() const
{
    return actualCallOrder_;
}

MockExpectedCall& MockCheckedExpectedCall::withCallOrder(unsigned int callOrder)
{
    expectedCallOrder_ = callOrder;
    return *this;
}

bool MockCheckedExpectedCall::isOutOfOrder() const
{
    return outOfOrder_;
}

struct MockExpectedCallCompositeNode
{
    MockExpectedCallCompositeNode(MockExpectedCall& functionCall, MockExpectedCallCompositeNode* next) : next_(next), call_(functionCall){}

    MockExpectedCallCompositeNode* next_;
    MockExpectedCall& call_;
};

MockExpectedCallComposite::MockExpectedCallComposite() : head_(NULL)
{
}

MockExpectedCallComposite::~MockExpectedCallComposite()
{

}

void MockExpectedCallComposite::add(MockExpectedCall& call)
{
    head_ = new MockExpectedCallCompositeNode(call, head_);
}

void MockExpectedCallComposite::clear()
{
    while (head_) {
        MockExpectedCallCompositeNode* next = head_->next_;
        delete head_;
        head_ = next;
    }
}

MockExpectedCall& MockExpectedCallComposite::withName(const SimpleString& name)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withName(name);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withCallOrder(unsigned int)
{
    FAIL("withCallOrder not supported for CompositeCalls");
    return *this; // LCOV_EXCL_LINE
}

MockExpectedCall& MockExpectedCallComposite::withBoolParameter(const SimpleString& name, bool value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withIntParameter(const SimpleString& name, int value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withLongIntParameter(const SimpleString& name, long int value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withDoubleParameter(const SimpleString& name, double value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;

}

MockExpectedCall& MockExpectedCallComposite::withStringParameter(const SimpleString& name, const char* value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withPointerParameter(const SimpleString& name, void* value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withConstPointerParameter(const SimpleString& name, const void* value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withFunctionPointerParameter(const SimpleString& name, void (*value)())
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameter(name, value, size);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withParameterOfType(typeName, name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withOutputParameterReturning(const SimpleString& name, const void* value, size_t size)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withOutputParameterReturning(name, value, size);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::withOutputParameterOfTypeReturning(const SimpleString& typeName, const SimpleString& name, const void* value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.withOutputParameterOfTypeReturning(typeName, name, value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::ignoreOtherParameters()
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.ignoreOtherParameters();
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(bool value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(unsigned int value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(int value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(long int value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(unsigned long int value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(double value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(const char* value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(void* value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(const void* value)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::andReturnValue(void (*value)())
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.andReturnValue(value);
    return *this;
}

MockExpectedCall& MockExpectedCallComposite::onObject(void* object)
{
    for (MockExpectedCallCompositeNode* node = head_; node != NULL; node = node->next_)
        node->call_.onObject(object);
    return *this;
}

MockExpectedCall& MockIgnoredExpectedCall::instance()
{
    static MockIgnoredExpectedCall call;
    return call;
}
