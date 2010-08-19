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

#ifndef D_MockSupport_c_h
#define D_MockSupport_c_h

struct MockFunctionCall_c
{
	MockFunctionCall_c* (*withIntParamaters)(const char* name, int value);
	MockFunctionCall_c* (*withDoubleParameters)(const char* name, double value);
	MockFunctionCall_c* (*withStringParameters)(const char* name, const char* value);
	MockFunctionCall_c* (*withPointerParameters)(const char* name, void* value);
	MockFunctionCall_c* (*withParameterOfType)(const char* type, const char* name, void* value);
};

typedef int (*MockTypeEqualFunction_c)(void* object1, void* object2);
typedef char* (*MockTypeValueToStringFunction_c)(void* object1);

struct MockSupport_c
{
	MockFunctionCall_c* (*expectOneCall)(const char* name);
	MockFunctionCall_c* (*actualCall)(const char* name);

	void (*installComparator) (const char* typeName, MockTypeEqualFunction_c isEqual, MockTypeValueToStringFunction_c valueToString);
	void (*removeAllComparators)();
};


MockSupport_c* mock_c();

#endif
