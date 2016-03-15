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
#include "CppUTestExt/MockCheckedActualCall.h"
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockFailure.h"
#include "CppUTest/PlatformSpecificFunctions.h"

MockActualCall::MockActualCall()
{
}

MockActualCall::~MockActualCall()
{
}

void MockCheckedActualCall::setName(const SimpleString& name)
{
    functionName_ = name;
}

SimpleString MockCheckedActualCall::getName() const
{
    return functionName_;
}

MockCheckedActualCall::MockCheckedActualCall(int callOrder, MockFailureReporter* reporter, const MockExpectedCallsList& allExpectations)
    : callOrder_(callOrder), reporter_(reporter), state_(CALL_SUCCEED), fulfilledExpectation_(NULL), allExpectations_(allExpectations), outputParameterExpectations_(NULL)
{
    unfulfilledExpectations_.addUnfulfilledExpectations(allExpectations);
}

MockCheckedActualCall::~MockCheckedActualCall()
{
    cleanUpOutputParameterList();
}

void MockCheckedActualCall::setMockFailureReporter(MockFailureReporter* reporter)
{
    reporter_ = reporter;
}

UtestShell* MockCheckedActualCall::getTest() const
{
    return reporter_->getTestToFail();
}

void MockCheckedActualCall::failTest(const MockFailure& failure)
{
    if (!hasFailed()) {
        setState(CALL_FAILED);
        reporter_->failTest(failure);
    }
}

void MockCheckedActualCall::finalizeOutputParameters(MockCheckedExpectedCall* expectedCall)
{
    for (MockOutputParametersListNode* p = outputParameterExpectations_; p; p = p->next_)
    {
        MockNamedValue outputParameter = expectedCall->getOutputParameter(p->name_);
        MockNamedValueCopier* copier = outputParameter.getCopier();
        if (copier)
        {
            copier->copy(p->ptr_, outputParameter.getObjectPointer());
        }
        else if ((outputParameter.getType() == "const void*") && (p->type_ == "void*"))
        {
            const void* data = outputParameter.getConstPointerValue();
            size_t size = outputParameter.getSize();
            PlatformSpecificMemCpy(p->ptr_, data, size);
        }
        else if (outputParameter.getName() != "")
        {
            SimpleString type = expectedCall->getOutputParameter(p->name_).getType();
            MockNoWayToCopyCustomTypeFailure failure(getTest(), type);
            failTest(failure);
        }
    }
}

void MockCheckedActualCall::finalizeCallWhenFulfilled()
{
    if (unfulfilledExpectations_.hasFulfilledExpectationsWithoutIgnoredParameters()) {
        finalizeOutputParameters(unfulfilledExpectations_.getOneFulfilledExpectationWithIgnoredParameters());
    }

    if (unfulfilledExpectations_.hasFulfilledExpectations()) {
        fulfilledExpectation_ = unfulfilledExpectations_.removeOneFulfilledExpectation();
        callHasSucceeded();
    }
}

void MockCheckedActualCall::callHasSucceeded()
{
    setState(CALL_SUCCEED);
}

void MockCheckedActualCall::callIsInProgress()
{
    setState(CALL_IN_PROGRESS);
    if (fulfilledExpectation_)
    {
        fulfilledExpectation_->resetExpectation();
        fulfilledExpectation_ = NULL;
    }
    unfulfilledExpectations_.onlyKeepUnfulfilledExpectations();
}

MockActualCall& MockCheckedActualCall::withName(const SimpleString& name)
{
    setName(name);
    callIsInProgress();

    unfulfilledExpectations_.onlyKeepExpectationsRelatedTo(name);
    if (unfulfilledExpectations_.isEmpty()) {
        MockUnexpectedCallHappenedFailure failure(getTest(), name, allExpectations_);
        failTest(failure);
        return *this;
    }

    unfulfilledExpectations_.callWasMade(callOrder_);

    finalizeCallWhenFulfilled();

    return *this;
}

MockActualCall& MockCheckedActualCall::withCallOrder(int)
{
    return *this;
}

void MockCheckedActualCall::checkInputParameter(const MockNamedValue& actualParameter)
{
    if(hasFailed())
    {
        return;
    }

    callIsInProgress();

    unfulfilledExpectations_.onlyKeepExpectationsWithInputParameter(actualParameter);

    if (unfulfilledExpectations_.isEmpty()) {
        MockUnexpectedInputParameterFailure failure(getTest(), getName(), actualParameter, allExpectations_);
        failTest(failure);
        return;
    }

    unfulfilledExpectations_.parameterWasPassed(actualParameter.getName());
    finalizeCallWhenFulfilled();
}

void MockCheckedActualCall::checkOutputParameter(const MockNamedValue& outputParameter)
{
    if(hasFailed())
    {
        return;
    }

    callIsInProgress();

    unfulfilledExpectations_.onlyKeepExpectationsWithOutputParameter(outputParameter);

    if (unfulfilledExpectations_.isEmpty()) {
        MockUnexpectedOutputParameterFailure failure(getTest(), getName(), outputParameter, allExpectations_);
        failTest(failure);
        return;
    }

    unfulfilledExpectations_.outputParameterWasPassed(outputParameter.getName());
    finalizeCallWhenFulfilled();
}

MockActualCall& MockCheckedActualCall::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withIntParameter(const SimpleString& name, int value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withLongIntParameter(const SimpleString& name, long int value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withDoubleParameter(const SimpleString& name, double value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withStringParameter(const SimpleString& name, const char* value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withPointerParameter(const SimpleString& name, void* value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withConstPointerParameter(const SimpleString& name, const void* value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withFunctionPointerParameter(const SimpleString& name, void (*value)())
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size)
{
    MockNamedValue actualParameter(name);
    actualParameter.setMemoryBuffer(value, size);
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setObjectPointer(type, value);

    if (actualParameter.getComparator() == NULL) {
        MockNoWayToCompareCustomTypeFailure failure(getTest(), type);
        failTest(failure);
        return *this;
    }
    checkInputParameter(actualParameter);
    return *this;
}

MockActualCall& MockCheckedActualCall::withOutputParameter(const SimpleString& name, void* output)
{
    addOutputParameter(name, "void*", output);

    MockNamedValue outputParameter(name);
    outputParameter.setValue(output);
    checkOutputParameter(outputParameter);

    return *this;
}

MockActualCall& MockCheckedActualCall::withOutputParameterOfType(const SimpleString& type, const SimpleString& name, void* output)
{
    addOutputParameter(name, type, output);

    MockNamedValue outputParameter(name);
    outputParameter.setObjectPointer(type, output);
    checkOutputParameter(outputParameter);

    return *this;
}

bool MockCheckedActualCall::isFulfilled() const
{
    return state_ == CALL_SUCCEED;
}

bool MockCheckedActualCall::hasFailed() const
{
    return state_ == CALL_FAILED;
}

void MockCheckedActualCall::checkExpectations()
{
    if (state_ != CALL_IN_PROGRESS)
    {
        unfulfilledExpectations_.resetExpectations();
        return;
    }

    if (! unfulfilledExpectations_.hasUnfulfilledExpectations())
        FAIL("Actual call is in progress. Checking expectations. But no unfulfilled expectations. Cannot happen.") // LCOV_EXCL_LINE

    fulfilledExpectation_ = unfulfilledExpectations_.removeOneFulfilledExpectationWithIgnoredParameters();
    if (fulfilledExpectation_) {
        callHasSucceeded();
        unfulfilledExpectations_.resetExpectations();
        return;
    }

    if (unfulfilledExpectations_.hasUnfulfilledExpectationsBecauseOfMissingParameters()) {
        MockExpectedParameterDidntHappenFailure failure(getTest(), getName(), allExpectations_);
        failTest(failure);
    }
    else {
        MockExpectedObjectDidntHappenFailure failure(getTest(), getName(), allExpectations_);
        failTest(failure);
    }
}

void MockCheckedActualCall::setState(ActualCallState state)
{
    state_ = state;
}

MockNamedValue MockCheckedActualCall::returnValue()
{
    checkExpectations();
    if (fulfilledExpectation_)
        return fulfilledExpectation_->returnValue();
    return MockNamedValue("no return value");
}

int MockCheckedActualCall::returnIntValueOrDefault(int default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnIntValue();
}

int MockCheckedActualCall::returnIntValue()
{
    return returnValue().getIntValue();
}

unsigned long int MockCheckedActualCall::returnUnsignedLongIntValue()
{
    return returnValue().getUnsignedLongIntValue();
}

unsigned long int MockCheckedActualCall::returnUnsignedLongIntValueOrDefault(unsigned long int default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnUnsignedLongIntValue();
}

long int MockCheckedActualCall::returnLongIntValue()
{
    return returnValue().getLongIntValue();
}

long int MockCheckedActualCall::returnLongIntValueOrDefault(long int default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnLongIntValue();
}

double MockCheckedActualCall::returnDoubleValue()
{
    return returnValue().getDoubleValue();
}

double MockCheckedActualCall::returnDoubleValueOrDefault(double default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnDoubleValue();
}

unsigned int MockCheckedActualCall::returnUnsignedIntValue()
{
    return returnValue().getUnsignedIntValue();
}

unsigned int MockCheckedActualCall::returnUnsignedIntValueOrDefault(unsigned int default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnUnsignedIntValue();
}

void * MockCheckedActualCall::returnPointerValueOrDefault(void * default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnPointerValue();
}

void * MockCheckedActualCall::returnPointerValue()
{
    return returnValue().getPointerValue();
}

const void * MockCheckedActualCall::returnConstPointerValue()
{
    return returnValue().getConstPointerValue();
}

const void * MockCheckedActualCall::returnConstPointerValueOrDefault(const void * default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnConstPointerValue();
}

void (*MockCheckedActualCall::returnFunctionPointerValue())()
{
    return returnValue().getFunctionPointerValue();
}

void (*MockCheckedActualCall::returnFunctionPointerValueOrDefault(void (*default_value)()))()
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnFunctionPointerValue();
}

const char * MockCheckedActualCall::returnStringValueOrDefault(const char * default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnStringValue();
}

const char * MockCheckedActualCall::returnStringValue()
{
    return returnValue().getStringValue();
}

bool MockCheckedActualCall::hasReturnValue()
{
    return ! returnValue().getName().isEmpty();
}

MockActualCall& MockCheckedActualCall::onObject(const void* objectPtr)
{
    callIsInProgress();

    unfulfilledExpectations_.onlyKeepExpectationsOnObject(objectPtr);

    if (unfulfilledExpectations_.isEmpty()) {
        MockUnexpectedObjectFailure failure(getTest(), getName(), objectPtr, allExpectations_);
        failTest(failure);
        return *this;
    }

    unfulfilledExpectations_.wasPassedToObject();

    finalizeCallWhenFulfilled();
    return *this;
}

void MockCheckedActualCall::addOutputParameter(const SimpleString& name, const SimpleString& type, void* ptr)
{
    MockOutputParametersListNode* newNode = new MockOutputParametersListNode(name, type, ptr);

    if (outputParameterExpectations_ == NULL)
        outputParameterExpectations_ = newNode;
    else {
        MockOutputParametersListNode* lastNode = outputParameterExpectations_;
        while (lastNode->next_) lastNode = lastNode->next_;
        lastNode->next_ = newNode;
    }
}

void MockCheckedActualCall::cleanUpOutputParameterList()
{
    MockOutputParametersListNode* current = outputParameterExpectations_;
    MockOutputParametersListNode* toBeDeleted = NULL;

    while (current) {
        toBeDeleted = current;
        outputParameterExpectations_ = current = current->next_;
        delete toBeDeleted;
    }
}


MockActualCallTrace::MockActualCallTrace()
{
}

MockActualCallTrace::~MockActualCallTrace()
{
}

MockActualCall& MockActualCallTrace::withName(const SimpleString& name)
{
    traceBuffer_ += "\nFunction name:";
    traceBuffer_ += name;
    return *this;
}

MockActualCall& MockActualCallTrace::withCallOrder(int callOrder)
{
    traceBuffer_ += " withCallOrder:";
    traceBuffer_ += StringFrom(callOrder);
    return *this;
}

void MockActualCallTrace::addParameterName(const SimpleString& name)
{
    traceBuffer_ += " ";
    traceBuffer_ += name;
    traceBuffer_ += ":";
}

MockActualCall& MockActualCallTrace::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withIntParameter(const SimpleString& name, int value)
{
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value)
{
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withLongIntParameter(const SimpleString& name, long int value)
{
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withDoubleParameter(const SimpleString& name, double value)
{
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withStringParameter(const SimpleString& name, const char* value)
{
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withPointerParameter(const SimpleString& name, void* value)
{
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withConstPointerParameter(const SimpleString& name, const void* value)
{
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withFunctionPointerParameter(const SimpleString& name, void (*value)())
{
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size)
{
    addParameterName(name);
    traceBuffer_ += StringFromBinaryWithSizeOrNull(value, size);
    return *this;
}

MockActualCall& MockActualCallTrace::withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)
{
    traceBuffer_ += " ";
    traceBuffer_ += typeName;
    addParameterName(name);
    traceBuffer_ += StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withOutputParameter(const SimpleString& name, void* output)
{
    addParameterName(name);
    traceBuffer_ += StringFrom(output);
    return *this;
}

MockActualCall& MockActualCallTrace::withOutputParameterOfType(const SimpleString& typeName, const SimpleString& name, void* output)
{
    traceBuffer_ += " ";
    traceBuffer_ += typeName;
    addParameterName(name);
    traceBuffer_ += StringFrom(output);
    return *this;
}

bool MockActualCallTrace::hasReturnValue()
{
    return false;
}

MockNamedValue MockActualCallTrace::returnValue()
{
    return MockNamedValue("");
}

long int MockActualCallTrace::returnLongIntValue()
{
    return 0;
}

unsigned long int MockActualCallTrace::returnUnsignedLongIntValue()
{
    return 0;
}

unsigned long int MockActualCallTrace::returnUnsignedLongIntValueOrDefault(unsigned long)
{
    return 0;
}

long int MockActualCallTrace::returnLongIntValueOrDefault(long int)
{
    return returnLongIntValue();
}

int MockActualCallTrace::returnIntValue()
{
    return 0;
}

double MockActualCallTrace::returnDoubleValue()
{
    return 0.0;
}

double MockActualCallTrace::returnDoubleValueOrDefault(double)
{
    return returnDoubleValue();
}

unsigned int MockActualCallTrace::returnUnsignedIntValue()
{
    return 0;
}

void * MockActualCallTrace::returnPointerValue()
{
    return NULL;
}

const void * MockActualCallTrace::returnConstPointerValue()
{
    return NULL;
}

void (*MockActualCallTrace::returnFunctionPointerValue())()
{
    return NULL;
}

const void * MockActualCallTrace::returnConstPointerValueOrDefault(const void *)
{
    return returnConstPointerValue();
}

void * MockActualCallTrace::returnPointerValueOrDefault(void *)
{
    return returnPointerValue();
}

void (*MockActualCallTrace::returnFunctionPointerValueOrDefault(void (*)()))()
{
    return returnFunctionPointerValue();
}

const char * MockActualCallTrace::returnStringValue()
{
    return "";
}

const char * MockActualCallTrace::returnStringValueOrDefault(const char *)
{
    return returnStringValue();
}

int MockActualCallTrace::returnIntValueOrDefault(int)
{
    return 0;
}

unsigned int MockActualCallTrace::returnUnsignedIntValueOrDefault(unsigned int)
{
    return returnUnsignedIntValue();
}

MockActualCall& MockActualCallTrace::onObject(const void* objectPtr)
{
    traceBuffer_ += " onObject:";
    traceBuffer_ += StringFrom(objectPtr);
    return *this;
}

void MockActualCallTrace::clear()
{
    traceBuffer_ = "";
}

const char* MockActualCallTrace::getTraceOutput()
{
    return traceBuffer_.asCharString();
}

MockActualCallTrace& MockActualCallTrace::instance()
{
    static MockActualCallTrace call;
    return call;
}

MockIgnoredActualCall& MockIgnoredActualCall::instance()
{
    static MockIgnoredActualCall call;
    return call;
}

