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
    : ignoreOtherParameters_(false), isActualCallMatchFinalized_(false),
      initialExpectedCallOrder_(NO_EXPECTED_CALL_ORDER), finalExpectedCallOrder_(NO_EXPECTED_CALL_ORDER),
      outOfOrder_(false), returnValue_(""), objectPtr_(NULLPTR), isSpecificObjectExpected_(false), wasPassedToObject_(true),
      actualCalls_(0), expectedCalls_(1)
{
    inputParameters_ = new MockNamedValueList();
    outputParameters_ = new MockNamedValueList();
}

MockCheckedExpectedCall::MockCheckedExpectedCall(unsigned int numCalls)
    : ignoreOtherParameters_(false), isActualCallMatchFinalized_(false),
      initialExpectedCallOrder_(NO_EXPECTED_CALL_ORDER), finalExpectedCallOrder_(NO_EXPECTED_CALL_ORDER),
      outOfOrder_(false), returnValue_(""), objectPtr_(NULLPTR), isSpecificObjectExpected_(false), wasPassedToObject_(true),
      actualCalls_(0), expectedCalls_(numCalls)
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

#ifdef CPPUTEST_USE_LONG_LONG

MockExpectedCall& MockCheckedExpectedCall::withLongLongIntParameter(const SimpleString& name, cpputest_longlong value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withUnsignedLongLongIntParameter(const SimpleString& name, cpputest_ulonglong value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

#else

MockExpectedCall& MockCheckedExpectedCall::withLongLongIntParameter(const SimpleString&, cpputest_longlong)
{
    FAIL("Long Long type is not supported");
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withUnsignedLongLongIntParameter(const SimpleString&, cpputest_ulonglong)
{
    FAIL("Unsigned Long Long type is not supported");
    return *this;
}

#endif

MockExpectedCall& MockCheckedExpectedCall::withDoubleParameter(const SimpleString& name, double value)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withDoubleParameter(const SimpleString& name, double value, double tolerance)
{
    MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
    inputParameters_->add(newParameter);
    newParameter->setValue(value, tolerance);
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
    newParameter->setConstObjectPointer(type, value);
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
    newParameter->setConstObjectPointer(type, value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::withUnmodifiedOutputParameter(const SimpleString& name)
{
    return withOutputParameterReturning(name, NULLPTR, 0);
}

SimpleString MockCheckedExpectedCall::getInputParameterType(const SimpleString& name)
{
    MockNamedValue * p = inputParameters_->getValueByName(name);
    return (p) ? p->getType() : "";
}

bool MockCheckedExpectedCall::hasInputParameterWithName(const SimpleString& name)
{
    MockNamedValue * p = inputParameters_->getValueByName(name);
    return p != NULLPTR;
}

bool MockCheckedExpectedCall::hasOutputParameterWithName(const SimpleString& name)
{
    MockNamedValue * p = outputParameters_->getValueByName(name);
    return p != NULLPTR;
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
    return (actualCalls_ == expectedCalls_);
}

bool MockCheckedExpectedCall::canMatchActualCalls()
{
    return (actualCalls_ < expectedCalls_);
}

bool MockCheckedExpectedCall::isMatchingActualCallAndFinalized()
{
    return isMatchingActualCall() && (!ignoreOtherParameters_ || isActualCallMatchFinalized_);
}

bool MockCheckedExpectedCall::isMatchingActualCall()
{
    return areParametersMatchingActualCall() && wasPassedToObject_;
}

void MockCheckedExpectedCall::callWasMade(unsigned int callOrder)
{
    actualCalls_++;

    if ( (initialExpectedCallOrder_ != NO_EXPECTED_CALL_ORDER) &&
         ((callOrder < initialExpectedCallOrder_) || (callOrder > finalExpectedCallOrder_)) ) {
        outOfOrder_ = true;
    }

    resetActualCallMatchingState();
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
    wasPassedToObject_ = !isSpecificObjectExpected_;
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
    if (isSpecificObjectExpected_)
        str = StringFromFormat("(object address: %p)::", objectPtr_);

    str += getName();
    str += " -> ";
    if (initialExpectedCallOrder_ != NO_EXPECTED_CALL_ORDER) {
        if (initialExpectedCallOrder_ == finalExpectedCallOrder_) {
            str += StringFromFormat("expected call order: <%u> -> ", initialExpectedCallOrder_);
        } else {
            str += StringFromFormat("expected calls order: <%u..%u> -> ", initialExpectedCallOrder_, finalExpectedCallOrder_);
        }
    }

    if (inputParameters_->begin() == NULLPTR && outputParameters_->begin() == NULLPTR) {
        str += (ignoreOtherParameters_) ? "all parameters ignored" : "no parameters";
    } else {
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
    }

    str += StringFromFormat(" (expected %d call%s, called %d time%s)",
                            expectedCalls_, (expectedCalls_ == 1) ? "" : "s", actualCalls_, (actualCalls_ == 1) ? "" : "s" );

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
    return (!isSpecificObjectExpected_) || (objectPtr_ == objectPtr);
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

#ifdef CPPUTEST_USE_LONG_LONG

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(cpputest_longlong value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(cpputest_ulonglong value)
{
    returnValue_.setName("returnValue");
    returnValue_.setValue(value);
    return *this;
}

#else

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(cpputest_longlong)
{
    FAIL("Long Long type is not supported");
    return *this;
}

MockExpectedCall& MockCheckedExpectedCall::andReturnValue(cpputest_ulonglong)
{
    FAIL("Unsigned Long Long type is not supported");
    return *this;
}

#endif

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
    isSpecificObjectExpected_ = true;
    wasPassedToObject_ = false;
    objectPtr_ = objectPtr;
    return *this;
}

MockNamedValue MockCheckedExpectedCall::returnValue()
{
    return returnValue_;
}

MockExpectedCall& MockCheckedExpectedCall::withCallOrder(unsigned int initialCallOrder, unsigned int finalCallOrder)
{
    initialExpectedCallOrder_ = initialCallOrder;
    finalExpectedCallOrder_ = finalCallOrder;
    return *this;
}

bool MockCheckedExpectedCall::isOutOfOrder() const
{
    return outOfOrder_;
}

unsigned int MockCheckedExpectedCall::getActualCallsFulfilled() const
{
    return actualCalls_;
}

MockExpectedCall& MockIgnoredExpectedCall::instance()
{
    static MockIgnoredExpectedCall call;
    return call;
}
