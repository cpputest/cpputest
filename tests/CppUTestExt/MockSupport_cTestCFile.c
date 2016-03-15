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

#include "CppUTestExt/MockSupport_c.h"
#include "MockSupport_cTestCFile.h"

static int typeNameIsEqual(const void* object1, const void* object2)
{
    return object1 == object2;
}

static const char* typeNameValueToString(const void* object)
{
    return (const char*) object;
}

void all_mock_support_c_calls(void)
{
    mock_c()->strictOrder();
    mock_c()->expectOneCall("boo");
    mock_c()->expectNoCall("bla");
    mock_c()->expectNCalls(1, "foo");
    mock_c()->actualCall("boo");
    mock_c()->actualCall("foo");
    mock_c()->checkExpectations();

    mock_c()->expectOneCall("boo")->withIntParameters("integer", 1)->
            withUnsignedIntParameters("unsigned", 1)->
            withLongIntParameters("long int", (long int) -1)->
            withUnsignedLongIntParameters("unsigned long int", (unsigned long int) 1)->
            withDoubleParameters("double", 1.0)->
            withStringParameters("string", "string")->
            withPointerParameters("pointer", (void*) 1)->
            withConstPointerParameters("constpointer", (const void*) 1)->
            withFunctionPointerParameters("functionpointer", (void(*)()) 1)->
            withMemoryBufferParameter("name", (void*) 1, 0UL)->
            ignoreOtherParameters();

    mock_c()->actualCall("boo")->withIntParameters("integer", 1)->
            withUnsignedIntParameters("unsigned", 1)->
            withLongIntParameters("long int", (long int) -1)->
            withUnsignedLongIntParameters("unsigned long int", (unsigned long int) 1)->
            withDoubleParameters("double", 1.0)->
            withStringParameters("string", "string")->
            withPointerParameters("pointer", (void*) 1)->
            withConstPointerParameters("constpointer", (const void*) 1)->
            withFunctionPointerParameters("functionpointer", (void(*)()) 1)->
            withMemoryBufferParameter("name", (void*) 1, 0UL)->
            hasReturnValue();

    mock_c()->disable();
    mock_c()->expectOneCall("boo")->withParameterOfType("type", "name", (void*) 1)->
            withOutputParameterReturning("name", (void*)1, 0UL)->
            withOutputParameterOfTypeReturning("type", "name", (void*)1);
    mock_c()->actualCall("boo")->withParameterOfType("type", "name", (void*) 1)->
            withOutputParameter("name", (void*)1)->
            withOutputParameterOfType("type", "name", (void*)1);
    mock_c()->enable();

    mock_c()->clear();

    mock_c()->installComparator("typeName", typeNameIsEqual, typeNameValueToString);
    mock_c()->expectOneCall("boo")->withParameterOfType("typeName", "name", (void*) 1);
    mock_c()->actualCall("boo")->withParameterOfType("typeName", "name", (void*) 1);
    mock_c()->clear();
    mock_c()->removeAllComparatorsAndCopiers();

    mock_c()->expectOneCall("boo")->andReturnIntValue(10);
    mock_c()->actualCall("boo")->returnValue();
    mock_c()->returnValue();
    mock_c()->expectOneCall("boo2")->andReturnDoubleValue(1.0);
    mock_c()->actualCall("boo2")->returnValue();
    mock_c()->returnValue();

    mock_c()->expectOneCall("boo3")->andReturnStringValue("hello world");
    mock_c()->actualCall("boo3")->returnValue();
    mock_c()->returnValue();

    mock_c()->expectOneCall("boo4")->andReturnPointerValue((void*) 10);
    mock_c()->actualCall("boo4")->returnValue();
    mock_c()->returnValue();

    mock_c()->expectOneCall("boo5")->andReturnFunctionPointerValue((void(*)()) 10);
    mock_c()->actualCall("boo5")->returnValue();
    mock_c()->returnValue();

    mock_c()->setIntData("int", 5);
    mock_c()->expectOneCall("bla")->withIntParameters("int", 5);
    mock_c()->actualCall("bla")->withIntParameters("int", mock_c()->getData("int").value.intValue);

    mock_c()->setStringData("string", "lol");
    mock_c()->expectOneCall("bla")->withStringParameters("str", "lol");
    mock_c()->actualCall("bla")->withStringParameters("str", mock_c()->getData("string").value.stringValue);

    mock_c()->setDoubleData("double", 0.001f);
    mock_c()->expectOneCall("bla")->withDoubleParameters("double", 0.001f);
    mock_c()->actualCall("bla")->withDoubleParameters("double", mock_c()->getData("double").value.doubleValue);

    mock_c()->setPointerData("ptr", (void*)1);
    mock_c()->expectOneCall("bla")->withPointerParameters("ptr", (void*)1);
    mock_c()->actualCall("bla")->withPointerParameters("ptr", mock_c()->getData("ptr").value.pointerValue);

    mock_c()->setConstPointerData("cptr", (const void*)1);
    mock_c()->expectOneCall("bla")->withConstPointerParameters("cptr", (const void*)1);
    mock_c()->actualCall("bla")->withConstPointerParameters("cptr", mock_c()->getData("ptr").value.constPointerValue);

    mock_c()->setFunctionPointerData("ptr", (void(*)())1);
    mock_c()->expectOneCall("bla")->withFunctionPointerParameters("ptr", (void(*)())1);
    mock_c()->actualCall("bla")->withFunctionPointerParameters("ptr", mock_c()->getData("ptr").value.functionPointerValue);

    mock_c()->hasReturnValue();
    mock_c()->intReturnValue();
    mock_c()->returnIntValueOrDefault(-1);
    mock_c()->unsignedIntReturnValue();
    mock_c()->returnUnsignedIntValueOrDefault(1);
    mock_c()->longIntReturnValue();
    mock_c()->returnLongIntValueOrDefault(-1L);
    mock_c()->unsignedLongIntReturnValue();
    mock_c()->returnUnsignedLongIntValueOrDefault(1L);
    mock_c()->stringReturnValue();
    mock_c()->returnStringValueOrDefault("");
    mock_c()->doubleReturnValue();
    mock_c()->returnDoubleValueOrDefault(0.01F);
    mock_c()->pointerReturnValue();
    mock_c()->returnPointerValueOrDefault(0);
    mock_c()->constPointerReturnValue();
    mock_c()->returnConstPointerValueOrDefault(0);
    mock_c()->functionPointerReturnValue();
    mock_c()->returnFunctionPointerValueOrDefault(0);

    mock_c()->disable();
    mock_c()->actualCall("disabled");
    mock_c()->enable();
    mock_c()->checkExpectations();

    mock_c()->setIntData("bla1", -2);
    mock_c()->setUnsignedIntData("bla2", 2);
    mock_c()->setDoubleData("bla3", 0.035);
    mock_c()->setStringData("bla4", "abc");
    mock_c()->setPointerData("bla", (void*) 2);
    mock_c()->setConstPointerData("bla", (const void*) 2);
    mock_c()->setFunctionPointerData("bla", (void (*)()) 2);
    mock_c()->setDataObject("bla", "type", (void*) 2);
    mock_c()->getData("bla");

    mock_scope_c("scope")->expectOneCall("boo");
    mock_scope_c("other")->expectedCallsLeft();
    mock_scope_c("scope")->expectedCallsLeft();
    mock_scope_c("scope")->actualCall("boo");
}
