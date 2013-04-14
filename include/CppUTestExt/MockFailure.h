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


#ifndef D_MockFailure_h
#define D_MockFailure_h

#include "CppUTest/TestFailure.h"

class MockExpectedFunctionsList;
class MockActualFunctionCall;
class MockNamedValue;
class MockFailure;

class MockFailureReporter
{
protected:
	bool crashOnFailure_;
public:
	MockFailureReporter() : crashOnFailure_(false){}
	virtual ~MockFailureReporter() {}

	virtual void failTest(const MockFailure& failure);
	virtual UtestShell* getTestToFail();
	virtual int getAmountOfTestFailures();

	virtual void crashOnFailure() {crashOnFailure_ = true; }
};

class MockFailure : public TestFailure
{
public:
	MockFailure(UtestShell* test);
	virtual ~MockFailure(){}
protected:
	void addExpectationsAndCallHistory(const MockExpectedFunctionsList& expectations);
	void addExpectationsAndCallHistoryRelatedTo(const SimpleString& function, const MockExpectedFunctionsList& expectations);
};

class MockExpectedCallsDidntHappenFailure : public MockFailure
{
public:
	MockExpectedCallsDidntHappenFailure(UtestShell* test, const MockExpectedFunctionsList& expectations);
	virtual ~MockExpectedCallsDidntHappenFailure(){}
};

class MockUnexpectedCallHappenedFailure : public MockFailure
{
public:
	MockUnexpectedCallHappenedFailure(UtestShell* test, const SimpleString& name, const MockExpectedFunctionsList& expectations);
	virtual ~MockUnexpectedCallHappenedFailure(){}
};

class MockCallOrderFailure : public MockFailure
{
public:
	MockCallOrderFailure(UtestShell* test, const MockExpectedFunctionsList& expectations);
	virtual ~MockCallOrderFailure(){}
};

class MockUnexpectedParameterFailure : public MockFailure
{
public:
	MockUnexpectedParameterFailure(UtestShell* test, const SimpleString& functionName, const MockNamedValue& parameter, const MockExpectedFunctionsList& expectations);
	virtual ~MockUnexpectedParameterFailure(){}
};

class MockExpectedParameterDidntHappenFailure : public MockFailure
{
public:
	MockExpectedParameterDidntHappenFailure(UtestShell* test, const SimpleString& functionName, const MockExpectedFunctionsList& expectations);
	virtual ~MockExpectedParameterDidntHappenFailure(){}
};

class MockNoWayToCompareCustomTypeFailure : public MockFailure
{
public:
	MockNoWayToCompareCustomTypeFailure(UtestShell* test, const SimpleString& typeName);
	virtual ~MockNoWayToCompareCustomTypeFailure(){}
};

class MockUnexpectedObjectFailure : public MockFailure
{
public:
	MockUnexpectedObjectFailure(UtestShell* test, const SimpleString& functionName, void* expected, const MockExpectedFunctionsList& expectations);
	virtual ~MockUnexpectedObjectFailure(){}
};

class MockExpectedObjectDidntHappenFailure : public MockFailure
{
public:
	MockExpectedObjectDidntHappenFailure(UtestShell* test, const SimpleString& functionName, const MockExpectedFunctionsList& expectations);
	virtual ~MockExpectedObjectDidntHappenFailure(){}
};

#endif
