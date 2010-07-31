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

class MockFailure : public TestFailure
{
public:
	MockFailure(Utest* test);
	virtual ~MockFailure(){};
};

class MockFailureReporter
{
protected:
	bool crashOnFailure_;
public:
	MockFailureReporter() : crashOnFailure_(false){};
	virtual ~MockFailureReporter() {};

	virtual void failTest(const MockFailure& failure);
	virtual Utest* getTestToFail();

	virtual void crashOnFailure() {crashOnFailure_ = true; }
};

class MockExpectedFunctionsList;

class MockExpectedCallsDidntHappenFailure : public MockFailure
{
public:
	MockExpectedCallsDidntHappenFailure(Utest* test, const MockExpectedFunctionsList& expectations);
	virtual ~MockExpectedCallsDidntHappenFailure(){};
};

class MockActualFunctionCall;

class MockUnexpectedCallHappenedFailure : public MockFailure
{
public:
	MockUnexpectedCallHappenedFailure(Utest* test, const SimpleString& name);
	virtual ~MockUnexpectedCallHappenedFailure(){};
};

class MockUnexpectedAdditionalCallFailure : public MockFailure
{
public:
	MockUnexpectedAdditionalCallFailure(Utest* test, int amountExpectations, const SimpleString& name);
	virtual ~MockUnexpectedAdditionalCallFailure(){};
};

class MockUnexpectedParameterNameFailure : public MockFailure
{
public:
	MockUnexpectedParameterNameFailure(Utest* test, const SimpleString& functionName, const SimpleString& parameterName);
	virtual ~MockUnexpectedParameterNameFailure(){};
};

class MockUnexpectedParameterValueFailure  : public MockFailure
{
public:
	MockUnexpectedParameterValueFailure(Utest* test, const SimpleString& functionName, const SimpleString& parameterName, const SimpleString& actualValue);
	virtual ~MockUnexpectedParameterValueFailure() {};
};

class MockExpectedParameterDidntHappenFailure : public MockFailure
{
public:
	MockExpectedParameterDidntHappenFailure(Utest* test, const SimpleString& functionName, const SimpleString& parameterName, const SimpleString& value);
	virtual ~MockExpectedParameterDidntHappenFailure(){};
};

#endif
