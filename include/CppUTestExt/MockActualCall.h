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

#ifndef D_MockActualCall_h
#define D_MockActualCall_h

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockNamedValue.h"
#include "CppUTestExt/MockExpectedCallsList.h"

class MockFailureReporter;
class MockFailure;

class MockActualCall
{
public:
	MockActualCall();
	virtual ~MockActualCall();

	virtual MockActualCall& withName(const SimpleString& name)=0;
	virtual MockActualCall& withCallOrder(int callOrder)=0;
	MockActualCall& withParameter(const SimpleString& name, int value) { return withIntParameter(name, value); }
	MockActualCall& withParameter(const SimpleString& name, unsigned int value) { return withUnsignedIntParameter(name, value); }
	MockActualCall& withParameter(const SimpleString& name, long int value) { return withLongIntParameter(name, value); }
	MockActualCall& withParameter(const SimpleString& name, unsigned long int value) { return withUnsignedLongIntParameter(name, value); }
	MockActualCall& withParameter(const SimpleString& name, double value) { return withDoubleParameter(name, value); }
	MockActualCall& withParameter(const SimpleString& name, const char* value) { return withStringParameter(name, value); }
	MockActualCall& withParameter(const SimpleString& name, void* value) { return withPointerParameter(name, value); }
	MockActualCall& withParameter(const SimpleString& name, const void* value) { return withConstPointerParameter(name, value); }
	virtual MockActualCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)=0;
	virtual MockActualCall& withOutputParameter(const SimpleString& name, void* output, size_t size)=0;

	virtual MockActualCall& withIntParameter(const SimpleString& name, int value)=0;
	virtual MockActualCall& withUnsignedIntParameter(const SimpleString& name, unsigned int value)=0;
	virtual MockActualCall& withLongIntParameter(const SimpleString& name, long int value)=0;
	virtual MockActualCall& withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value)=0;
	virtual MockActualCall& withDoubleParameter(const SimpleString& name, double value)=0;
	virtual MockActualCall& withStringParameter(const SimpleString& name, const char* value)=0;
	virtual MockActualCall& withPointerParameter(const SimpleString& name, void* value)=0;
	virtual MockActualCall& withConstPointerParameter(const SimpleString& name, const void* value)=0;

	virtual bool hasReturnValue()=0;
	virtual MockNamedValue returnValue()=0;

	virtual MockActualCall& onObject(void* objectPtr)=0;
};

#endif
