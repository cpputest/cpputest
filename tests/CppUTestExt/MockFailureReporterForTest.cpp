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

#include "MockFailureReporterForTest.h"

void MockFailureReporterForTest::failTest(const MockFailure& failure)
{
    mockFailureString = failure.getMessage();
}

MockFailureReporterForTest* MockFailureReporterForTest::instance_ = NULLPTR;

MockFailureReporterForTest* MockFailureReporterForTest::getReporter()
{
    if (instance_ == NULLPTR)
        instance_ = new MockFailureReporterForTest;

    return instance_;
}

void MockFailureReporterForTest::clearReporter()
{
    delete instance_;
    instance_ = NULLPTR;
}

MockFailureReporterInstaller::MockFailureReporterInstaller()
{
  mock().setMockFailureStandardReporter(MockFailureReporterForTest::getReporter());
}

MockFailureReporterInstaller::~MockFailureReporterInstaller()
{
  mock().setMockFailureStandardReporter(NULLPTR);
  MockFailureReporterForTest::clearReporter();
}

UtestShell* mockFailureTest()
{
    return MockFailureReporterForTest::getReporter()->getTestToFail();
}

SimpleString mockFailureString()
{
    return MockFailureReporterForTest::getReporter()->mockFailureString;
}

void CLEAR_MOCK_FAILURE()
{
    MockFailureReporterForTest::getReporter()->mockFailureString = "";
}

void CHECK_EXPECTED_MOCK_FAILURE_LOCATION(const MockFailure& expectedFailure, const char* file, size_t line)
{
    SimpleString expectedFailureString = expectedFailure.getMessage();
    SimpleString actualFailureString = mockFailureString();
    CLEAR_MOCK_FAILURE();
    if (expectedFailureString != actualFailureString)
    {
        SimpleString error = "MockFailures are different.\n";
        error += "Expected MockFailure:\n\t";
        error += expectedFailureString;
        error += "\nActual MockFailure:\n\t";
        error += actualFailureString;
        FAIL_LOCATION(error.asCharString(), file, line);
    }
}

void CHECK_NO_MOCK_FAILURE_LOCATION(const char* file, size_t line)
{
    if (mockFailureString() != "") {
        SimpleString error = "Unexpected mock failure:\n";
        error += mockFailureString();
        CLEAR_MOCK_FAILURE();
        FAIL_LOCATION(error.asCharString(), file, line);

    }
    CLEAR_MOCK_FAILURE();
}

MockExpectedCallsListForTest::~MockExpectedCallsListForTest()
{
  deleteAllExpectationsAndClearList();
}

MockCheckedExpectedCall* MockExpectedCallsListForTest::addFunction(const SimpleString& name)
{
  MockCheckedExpectedCall* newCall = new MockCheckedExpectedCall;
  newCall->withName(name);
  addExpectedCall(newCall);
  return newCall;
}

MockCheckedExpectedCall* MockExpectedCallsListForTest::addFunction(unsigned int numCalls, const SimpleString& name)
{
  MockCheckedExpectedCall* newCall = new MockCheckedExpectedCall(numCalls);
  newCall->withName(name);
  addExpectedCall(newCall);
  return newCall;
}

MockCheckedExpectedCall* MockExpectedCallsListForTest::addFunctionOrdered(const SimpleString& name, unsigned int order)
{
  MockCheckedExpectedCall* newCall = addFunction(name);
  newCall->withCallOrder(order);
  return newCall;
}


