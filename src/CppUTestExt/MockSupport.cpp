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
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockActualCall.h"
#include "CppUTestExt/MockExpectedCall.h"
#include "CppUTestExt/MockFailure.h"

#define MOCK_SUPPORT_SCOPE_PREFIX "!!!$$$MockingSupportScope$$$!!!"

static MockSupport global_mock;

MockSupport& mock(const SimpleString& mockName, MockFailureReporter* failureReporterForThisCall)
{
    MockSupport& mock_support = (mockName != "") ? *global_mock.getMockSupportScope(mockName) : global_mock;
    mock_support.setActiveReporter(failureReporterForThisCall);
    mock_support.setDefaultComparatorsAndCopiersRepository();
    return mock_support;
}

MockSupport::MockSupport(const SimpleString& mockName)
    : actualCallOrder_(0), expectedCallOrder_(0), strictOrdering_(false), standardReporter_(&defaultReporter_), ignoreOtherCalls_(false),
      enabled_(true), currentActualCall_(NULL), mockName_(mockName), actualCalls_(true), tracing_(false)
{
    setActiveReporter(NULL);
}

MockSupport::~MockSupport()
{
}

void MockSupport::crashOnFailure(bool shouldCrash)
{
    activeReporter_->crashOnFailure(shouldCrash);
}

void MockSupport::setMockFailureStandardReporter(MockFailureReporter* reporter)
{
    standardReporter_ = (reporter != NULL) ? reporter : &defaultReporter_;

    if (currentActualCall_)
        currentActualCall_->setMockFailureReporter(standardReporter_);

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
        if (getMockSupport(p)) getMockSupport(p)->setMockFailureStandardReporter(standardReporter_);
}

void MockSupport::setActiveReporter(MockFailureReporter* reporter)
{
    activeReporter_ = (reporter) ? reporter : standardReporter_;
}

void MockSupport::setDefaultComparatorsAndCopiersRepository()
{
    MockNamedValue::setDefaultComparatorsAndCopiersRepository(&comparatorsAndCopiersRepository_);
}

void MockSupport::installComparator(const SimpleString& typeName, MockNamedValueComparator& comparator)
{
    comparatorsAndCopiersRepository_.installComparator(typeName, comparator);

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
        if (getMockSupport(p)) getMockSupport(p)->installComparator(typeName, comparator);
}

void MockSupport::installCopier(const SimpleString& typeName, MockNamedValueCopier& copier)
{
    comparatorsAndCopiersRepository_.installCopier(typeName, copier);

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
        if (getMockSupport(p)) getMockSupport(p)->installCopier(typeName, copier);
}

void MockSupport::installComparatorsAndCopiers(const MockNamedValueComparatorsAndCopiersRepository& repository)
{
    comparatorsAndCopiersRepository_.installComparatorsAndCopiers(repository);

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
        if (getMockSupport(p)) getMockSupport(p)->installComparatorsAndCopiers(repository);
}

void MockSupport::removeAllComparatorsAndCopiers()
{
    comparatorsAndCopiersRepository_.clear();
    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
        if (getMockSupport(p)) getMockSupport(p)->removeAllComparatorsAndCopiers();
}

void MockSupport::clear()
{
    if (currentActualCall_) {
        delete currentActualCall_;
        currentActualCall_ = NULL;
    }

    tracing_ = false;
    MockActualCallTrace::instance().clear();

    expectations_.deleteAllExpectationsAndClearList();
    actualCalls_.clear();
    ignoreOtherCalls_ = false;
    enabled_ = true;
    actualCallOrder_ = 0;
    expectedCallOrder_ = 0;
    strictOrdering_ = false;

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next()) {
        MockSupport* support = getMockSupport(p);
        if (support) {
            support->clear();
            delete support;
        }
    }
    data_.clear();
}

void MockSupport::strictOrder()
{
    strictOrdering_ = true;
}

SimpleString MockSupport::appendScopeToName(const SimpleString& functionName)
{
    if (mockName_.isEmpty()) return functionName;
    return mockName_ + "::" + functionName;
}

MockExpectedCall& MockSupport::expectOneCall(const SimpleString& functionName)
{
    return expectRangeOfCalls(1, 1, functionName);
}

MockExpectedCall& MockSupport::expectNCalls(unsigned int amount, const SimpleString& functionName)
{
    return expectRangeOfCalls(amount, amount, functionName);
}

void MockSupport::expectNoCall(const SimpleString& functionName)
{
    expectRangeOfCalls(0, 0, functionName);
//    if (!enabled_) return;
//
//    countCheck();
//
//    MockCheckedExpectedCall* call = new MockCheckedExpectedCall;
//    call->withName(appendScopeToName(functionName));
//    unExpectations_.addExpectedCall(call);
}

MockExpectedCall& MockSupport::expectAtLeastOneCall(const SimpleString& functionName)
{
    return expectRangeOfCalls(1, (unsigned int)-1, functionName);
}

MockExpectedCall& MockSupport::expectAtLeastNCalls(unsigned int amount, const SimpleString& functionName)
{
    return expectRangeOfCalls(amount, (unsigned int)-1, functionName);
}

MockExpectedCall& MockSupport::expectAtMostOneCall(const SimpleString& functionName)
{
    return expectRangeOfCalls(0, 1, functionName);
}

MockExpectedCall& MockSupport::expectAtMostNCalls(unsigned int amount, const SimpleString& functionName)
{
    return expectRangeOfCalls(0, amount, functionName);
}

MockExpectedCall& MockSupport::expectAnyCalls(const SimpleString& functionName)
{
    return expectRangeOfCalls(0, (unsigned int)-1, functionName);
}

MockExpectedCall& MockSupport::expectRangeOfCalls(unsigned int minCalls, unsigned int maxCalls, const SimpleString& functionName)
{
    if (!enabled_) return MockIgnoredExpectedCall::instance();

    countCheck();

    const SimpleString scopeFunctionName = appendScopeToName(functionName);
    MockCheckedExpectedCall* call = new MockCheckedExpectedCall(minCalls, maxCalls);
    call->withName(scopeFunctionName);
    if (strictOrdering_) {
        if (minCalls == maxCalls) {
            call->withCallOrder(expectedCallOrder_ + 1, expectedCallOrder_ + minCalls);
            expectedCallOrder_ += minCalls;
        } else {
            MockStrictOrderingIncompatibleWithOptionalCallsFailure failure(activeReporter_->getTestToFail(), scopeFunctionName, minCalls, maxCalls);
            failTest(failure);
        }
    }
    expectations_.addExpectedCall(call);
    return *call;
}

MockCheckedActualCall* MockSupport::createActualCall()
{
    currentActualCall_ = new MockCheckedActualCall(++actualCallOrder_, activeReporter_, *this);
    return currentActualCall_;
}

bool MockSupport::callIsIgnored(const SimpleString& functionName)
{
    return ignoreOtherCalls_ && !expectations_.hasExpectationWithName(functionName);
}

MockActualCall& MockSupport::actualCall(const SimpleString& functionName)
{
    const SimpleString scopeFunctionName = appendScopeToName(functionName);

    if (currentActualCall_) {
        currentActualCall_->checkExpectations();
        if (currentActualCall_->isFulfilled()) {
            actualCalls_.pushBack(currentActualCall_);
        } else {
            delete currentActualCall_;
        }
        currentActualCall_ = NULL;
    }

    if (!enabled_) return MockIgnoredActualCall::instance();
    if (tracing_) return MockActualCallTrace::instance().withCallOrder(++actualCallOrder_).withName(scopeFunctionName);

    if (callIsIgnored(scopeFunctionName)) {
        return MockIgnoredActualCall::instance();
    }

    MockCheckedActualCall* call = createActualCall();
    call->withName(scopeFunctionName);
    return *call;
}

void MockSupport::ignoreOtherCalls()
{
    ignoreOtherCalls_ = true;

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
        if (getMockSupport(p)) getMockSupport(p)->ignoreOtherCalls();
}

void MockSupport::disable()
{
    enabled_ = false;

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
        if (getMockSupport(p)) getMockSupport(p)->disable();
}

void MockSupport::enable()
{
    enabled_ = true;

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
        if (getMockSupport(p)) getMockSupport(p)->enable();
}

void MockSupport::tracing(bool enabled)
{
    tracing_ = enabled;

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
        if (getMockSupport(p)) getMockSupport(p)->tracing(enabled);
}

const char* MockSupport::getTraceOutput()
{
    return MockActualCallTrace::instance().getTraceOutput();
}

bool MockSupport::expectedCallsLeft()
{
    if (expectations_.hasUnfulfilledExpectations()) {
        return true;
    }

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next()) {
        if (getMockSupport(p) && getMockSupport(p)->expectedCallsLeft()) {
            return true;
        }
    }

    return false;
}

bool MockSupport::isLastActualCallFulfilled()
{
    return !currentActualCall_ || currentActualCall_->isFulfilled();
}

bool MockSupport::shallFailWithExpectedCallsNotFulfilled()
{
    if (isLastActualCallFulfilled() && expectations_.hasUnfulfilledExpectations()) {
        return true;
    }

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next()) {
        if (getMockSupport(p) && getMockSupport(p)->shallFailWithExpectedCallsNotFulfilled()) {
                return true;
        }
    }

    return false;
}

void MockSupport::addExpectedCallsForAllScopes(MockExpectedCallsList& expectedCalls)
{
    expectedCalls.addExpectations(expectations_);

    for (MockNamedValueListNode *p = data_.begin();p;p = p->next()) {
        if (getMockSupport(p)) {
            expectedCalls.addExpectations(getMockSupport(p)->expectations_);
        }
    }
}

void MockSupport::addActualCallsForAllScopes(MockActualCallsQueue& actualCalls)
{
    actualCalls.pushBackAll(actualCalls_);

    for (MockNamedValueListNode *p = data_.begin();p;p = p->next()) {
        if (getMockSupport(p)) {
            actualCalls.pushBackAll(getMockSupport(p)->actualCalls_);
        }
    }
}

void MockSupport::failTestWithExpectedCallsNotFulfilled()
{
    MockExpectedCallsList expectedCallsForAllScopes;
    addExpectedCallsForAllScopes(expectedCallsForAllScopes);

    MockActualCallsQueue actualCallsForAllScopes(false);
    addActualCallsForAllScopes(actualCallsForAllScopes);

    MockExpectedCallsNotFulfilledFailure failure(activeReporter_->getTestToFail(), expectedCallsForAllScopes, actualCallsForAllScopes);
    failTest(failure);
}

void MockSupport::failTestWithOutOfOrderCalls()
{
    MockExpectedCallsList expectedCallsForAllScopes;
    addExpectedCallsForAllScopes(expectedCallsForAllScopes);

    MockActualCallsQueue actualCallsForAllScopes(false);
    addActualCallsForAllScopes(actualCallsForAllScopes);

    MockCallOrderFailure failure(activeReporter_->getTestToFail(), expectedCallsForAllScopes, actualCallsForAllScopes);
    failTest(failure);
}

void MockSupport::failTest(MockFailure& failure)
{
    clear();
    activeReporter_->failTest(failure);
}

void MockSupport::countCheck()
{
    UtestShell::getCurrent()->countCheck();
}

void MockSupport::checkExpectationsOfLastActualCall()
{
    if (currentActualCall_) {
        currentActualCall_->checkExpectations();
        if (currentActualCall_->isFulfilled()) {
            actualCalls_.pushBack(currentActualCall_);
            currentActualCall_ = NULL;
        }
    }
}

void MockSupport::checkExpectationsOfLastActualCallsInAllScopes()
{
    checkExpectationsOfLastActualCall();

    for (MockNamedValueListNode *p = data_.begin();p;p = p->next()) {
        if (getMockSupport(p)) {
            getMockSupport(p)->checkExpectationsOfLastActualCallsInAllScopes();
        }
    }
}

bool MockSupport::hasCallsOutOfOrderInAnyScope()
{
    if (expectations_.hasCallsOutOfOrder()) {
        return true;
    }

    for (MockNamedValueListNode* p = data_.begin(); p; p = p->next()) {
        if (getMockSupport(p) && getMockSupport(p)->hasCallsOutOfOrderInAnyScope()) {
            return true;
        }
    }

    return false;
}

void MockSupport::checkExpectations()
{
    checkExpectationsOfLastActualCallsInAllScopes();

    if (shallFailWithExpectedCallsNotFulfilled()) {
        failTestWithExpectedCallsNotFulfilled();
    }

    if (hasCallsOutOfOrderInAnyScope()) {
        failTestWithOutOfOrderCalls();
    }
}

bool MockSupport::hasData(const SimpleString& name)
{
    return data_.getValueByName(name) != NULL;
}

MockNamedValue* MockSupport::retrieveDataFromStore(const SimpleString& name)
{
    MockNamedValue* newData = data_.getValueByName(name);
    if (newData == NULL) {
        newData = new MockNamedValue(name);
        data_.add(newData);
    }
    return newData;
}

void MockSupport::setData(const SimpleString& name, bool value)
{
    MockNamedValue* newData = retrieveDataFromStore(name);
    newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, unsigned int value)
{
    MockNamedValue* newData = retrieveDataFromStore(name);
    newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, int value)
{
    MockNamedValue* newData = retrieveDataFromStore(name);
    newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, const char* value)
{
    MockNamedValue* newData = retrieveDataFromStore(name);
    newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, double value)
{
    MockNamedValue* newData = retrieveDataFromStore(name);
    newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, void* value)
{
    MockNamedValue* newData = retrieveDataFromStore(name);
    newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, const void* value)
{
    MockNamedValue* newData = retrieveDataFromStore(name);
    newData->setValue(value);
}

void MockSupport::setData(const SimpleString& name, void (*value)())
{
    MockNamedValue* newData = retrieveDataFromStore(name);
    newData->setValue(value);
}

void MockSupport::setDataObject(const SimpleString& name, const SimpleString& type, void* value)
{
    MockNamedValue* newData = retrieveDataFromStore(name);
    newData->setObjectPointer(type, value);
}

MockNamedValue MockSupport::getData(const SimpleString& name)
{
    MockNamedValue* value = data_.getValueByName(name);
    if (value == NULL)
        return MockNamedValue("");
    return *value;
}

MockSupport* MockSupport::clone(const SimpleString& mockName)
{
    MockSupport* newMock = new MockSupport(mockName);
    newMock->setMockFailureStandardReporter(standardReporter_);
    if (ignoreOtherCalls_) newMock->ignoreOtherCalls();

    if (!enabled_) newMock->disable();

    if (strictOrdering_) newMock->strictOrder();

    newMock->tracing(tracing_);
    newMock->installComparatorsAndCopiers(comparatorsAndCopiersRepository_);
    return newMock;
}

MockSupport* MockSupport::getMockSupportScope(const SimpleString& name)
{
    SimpleString mockingSupportName = MOCK_SUPPORT_SCOPE_PREFIX;
    mockingSupportName += name;

    if (hasData(mockingSupportName)) {
        STRCMP_EQUAL("MockSupport", getData(mockingSupportName).getType().asCharString());
        return (MockSupport*) getData(mockingSupportName).getObjectPointer();
    }

    MockSupport *newMock = clone(name);

    setDataObject(mockingSupportName, "MockSupport", newMock);
    return newMock;
}

MockSupport* MockSupport::getMockSupport(MockNamedValueListNode* node)
{
    if (node->getType() == "MockSupport" && node->getName().contains(MOCK_SUPPORT_SCOPE_PREFIX))
        return  (MockSupport*) node->item()->getObjectPointer();
    return NULL;
}

MockNamedValue MockSupport::returnValue()
{
    if (currentActualCall_) return currentActualCall_->returnValue();
    return MockNamedValue("");
}

bool MockSupport::boolReturnValue()
{
    return returnValue().getBoolValue();
}

unsigned int MockSupport::unsignedIntReturnValue()
{
    return returnValue().getUnsignedIntValue();
}

int MockSupport::intReturnValue()
{
    return returnValue().getIntValue();
}

const char * MockSupport::returnStringValueOrDefault(const char * defaultValue)
{
    if (hasReturnValue()) {
        return stringReturnValue();
    }
    return defaultValue;
}

double MockSupport::returnDoubleValueOrDefault(double defaultValue)
{
    if (hasReturnValue()) {
        return doubleReturnValue();
    }
    return defaultValue;
}

long int MockSupport::returnLongIntValueOrDefault(long int defaultValue)
{
    if (hasReturnValue()) {
        return longIntReturnValue();
    }
    return defaultValue;
}

bool MockSupport::returnBoolValueOrDefault(bool defaultValue)
{
    if (hasReturnValue()) {
        return boolReturnValue();
    }
    return defaultValue;
}

int MockSupport::returnIntValueOrDefault(int defaultValue)
{
    if (hasReturnValue()) {
        return intReturnValue();
    }
    return defaultValue;
}

unsigned int MockSupport::returnUnsignedIntValueOrDefault(unsigned int defaultValue)
{
    if (hasReturnValue()) {
        return unsignedIntReturnValue();
    }
    return defaultValue;
}

unsigned long int MockSupport::returnUnsignedLongIntValueOrDefault(unsigned long int defaultValue)
{
    if (hasReturnValue()) {
        return unsignedLongIntReturnValue();
    }
    return defaultValue;
}

long int MockSupport::longIntReturnValue()
{
    return returnValue().getLongIntValue();
}

unsigned long int MockSupport::unsignedLongIntReturnValue()
{
    return returnValue().getUnsignedLongIntValue();
}

const char* MockSupport::stringReturnValue()
{
    return returnValue().getStringValue();
}

double MockSupport::doubleReturnValue()
{
    return returnValue().getDoubleValue();
}

void * MockSupport::returnPointerValueOrDefault(void * defaultValue)
{
    if (hasReturnValue()) {
        return pointerReturnValue();
    }
    return defaultValue;
}

const void* MockSupport::returnConstPointerValueOrDefault(const void * defaultValue)
{
    if (hasReturnValue()) {
        return constPointerReturnValue();
    }
    return defaultValue;
}

void (*MockSupport::returnFunctionPointerValueOrDefault(void (*defaultValue)()))()
{
    if (hasReturnValue()) {
        return functionPointerReturnValue();
    }
    return defaultValue;
}

void* MockSupport::pointerReturnValue()
{
    return returnValue().getPointerValue();
}

const void* MockSupport::constPointerReturnValue()
{
    return returnValue().getConstPointerValue();
}

void (*MockSupport::functionPointerReturnValue())()
{
    return returnValue().getFunctionPointerValue();
}

bool MockSupport::hasReturnValue()
{
    if (currentActualCall_) return currentActualCall_->hasReturnValue();
    return false;
}

const MockExpectedCallsList& MockSupport::getExpectedCalls() const
{
    return expectations_;
}

const MockActualCallsQueue& MockSupport::getActualCalls() const
{
    return actualCalls_;
}

const SimpleString&  MockSupport::getName() const
{
    return mockName_;
}

