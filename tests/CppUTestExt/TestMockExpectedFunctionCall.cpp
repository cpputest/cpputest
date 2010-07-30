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

TEST_GROUP(MockExpectedFunctionCall)
{
};

TEST(MockExpectedFunctionCall, callWithoutParameterSetOrNotFound)
{
	MockExpectedFunctionCall call;
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_NONE, call.getParameterType("nonexisting"));
	LONGS_EQUAL(0, call.getParameterValue("nonexisting").intValue_);
}

TEST(MockExpectedFunctionCall, callWithIntegerParameter)
{
	MockExpectedFunctionCall call;
	call.withParameter("integer", 1);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_INT, call.getParameterType("integer"));
	LONGS_EQUAL(1, call.getParameterValue("integer").intValue_);
}

TEST(MockExpectedFunctionCall, callWithDoubleParameter)
{
	MockExpectedFunctionCall call;
	call.withParameter("double", 1.2);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_DOUBLE, call.getParameterType("double"));
	DOUBLES_EQUAL(1.2, call.getParameterValue("double").doubleValue_, 0.05);
}

TEST(MockExpectedFunctionCall, callWithStringParameter)
{
	MockExpectedFunctionCall call;
	call.withParameter("string", "hello world");
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_STRING, call.getParameterType("string"));
	STRCMP_EQUAL("hello world", call.getParameterValue("string").stringValue_);
}

TEST(MockExpectedFunctionCall, callWithPointerParameter)
{
	MockExpectedFunctionCall call;
	void* ptr = (void*) 0x123;
	call.withParameter("pointer", ptr);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_POINTER, call.getParameterType("pointer"));
	POINTERS_EQUAL(ptr, call.getParameterValue("pointer").pointerValue_);
}

TEST(MockExpectedFunctionCall, callWithTwoIntegerParameter)
{
	MockExpectedFunctionCall call;
	call.withParameter("integer1", 1);
	call.withParameter("integer2", 2);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_INT, call.getParameterType("integer1"));
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_INT, call.getParameterType("integer2"));
	LONGS_EQUAL(1, call.getParameterValue("integer1").intValue_);
	LONGS_EQUAL(2, call.getParameterValue("integer2").intValue_);
}

TEST(MockExpectedFunctionCall, callWithThreeDifferentParameter)
{
	MockExpectedFunctionCall call;
	call.withParameter("integer", 1);
	call.withParameter("string", "hello world");
	call.withParameter("double", 0.12);
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_INT, call.getParameterType("integer"));
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_STRING, call.getParameterType("string"));
	LONGS_EQUAL(MOCK_FUNCTION_PARAMETER_DOUBLE, call.getParameterType("double"));
	LONGS_EQUAL(1, call.getParameterValue("integer").intValue_);
	STRCMP_EQUAL("hello world", call.getParameterValue("string").stringValue_);
	DOUBLES_EQUAL(0.12, call.getParameterValue("double").doubleValue_, 0.05);
}
