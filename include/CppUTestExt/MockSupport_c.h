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

#ifdef __cplusplus
extern "C" {
#endif

#include "CppUTest/StandardCLibrary.h"

typedef enum {
    MOCKVALUETYPE_UNSIGNED_INTEGER,
    MOCKVALUETYPE_INTEGER,
    MOCKVALUETYPE_LONG_INTEGER,
    MOCKVALUETYPE_UNSIGNED_LONG_INTEGER,
    MOCKVALUETYPE_DOUBLE,
    MOCKVALUETYPE_STRING,
    MOCKVALUETYPE_POINTER,
    MOCKVALUETYPE_CONST_POINTER,
    MOCKVALUETYPE_OBJECT
} MockValueType_c;

typedef struct SMockValue_c
{
    MockValueType_c type;
    union {
        int intValue;
        unsigned int unsignedIntValue;
        long int longIntValue;
        unsigned long int unsignedLongIntValue;
        double doubleValue;
        const char* stringValue;
        void* pointerValue;
        const void* constPointerValue;
        const void* objectValue;
    } value;
} MockValue_c;

typedef struct SMockActualCall_c MockActualCall_c;
struct SMockActualCall_c
{
    MockActualCall_c* (*withIntParameters)(const char* name, int value);
    MockActualCall_c* (*withUnsignedIntParameters)(const char* name, unsigned int value);
    MockActualCall_c* (*withLongIntParameters)(const char* name, long int value);
    MockActualCall_c* (*withUnsignedLongIntParameters)(const char* name, unsigned long int value);
    MockActualCall_c* (*withDoubleParameters)(const char* name, double value);
    MockActualCall_c* (*withStringParameters)(const char* name, const char* value);
    MockActualCall_c* (*withPointerParameters)(const char* name, void* value);
    MockActualCall_c* (*withConstPointerParameters)(const char* name, const void* value);
    MockActualCall_c* (*withParameterOfType)(const char* type, const char* name, const void* value);
    MockActualCall_c* (*withOutputParameter)(const char* name, void* value);

    MockValue_c (*returnValue)(void);
};

typedef struct SMockExpectedCall_c MockExpectedCall_c;
struct SMockExpectedCall_c
{
    MockExpectedCall_c* (*withIntParameters)(const char* name, int value);
    MockExpectedCall_c* (*withUnsignedIntParameters)(const char* name, unsigned int value);
    MockExpectedCall_c* (*withLongIntParameters)(const char* name, long int value);
    MockExpectedCall_c* (*withUnsignedLongIntParameters)(const char* name, unsigned long int value);
    MockExpectedCall_c* (*withDoubleParameters)(const char* name, double value);
    MockExpectedCall_c* (*withStringParameters)(const char* name, const char* value);
    MockExpectedCall_c* (*withPointerParameters)(const char* name, void* value);
    MockExpectedCall_c* (*withConstPointerParameters)(const char* name, const void* value);
    MockExpectedCall_c* (*withParameterOfType)(const char* type, const char* name, const void* value);
    MockExpectedCall_c* (*withOutputParameterReturning)(const char* name, const void* value, size_t size);

    MockExpectedCall_c* (*andReturnUnsignedIntValue)(unsigned int value);
    MockExpectedCall_c* (*andReturnIntValue)(int value);
    MockExpectedCall_c* (*andReturnLongIntValue)(long int value);
    MockExpectedCall_c* (*andReturnUnsignedLongIntValue)(unsigned long int value);
    MockExpectedCall_c* (*andReturnDoubleValue)(double value);
    MockExpectedCall_c* (*andReturnStringValue)(const char* value);
    MockExpectedCall_c* (*andReturnPointerValue)(void* value);
    MockExpectedCall_c* (*andReturnConstPointerValue)(const void* value);
};

typedef int (*MockTypeEqualFunction_c)(const void* object1, const void* object2);
typedef char* (*MockTypeValueToStringFunction_c)(const void* object1);

typedef struct SMockSupport_c MockSupport_c;
struct SMockSupport_c
{
    MockExpectedCall_c* (*expectOneCall)(const char* name);
    MockActualCall_c* (*actualCall)(const char* name);
    MockValue_c (*returnValue)(void);

    void (*enable)(void);
    void (*disable)(void);

    void (*setIntData) (const char* name, int value);
    void (*setDoubleData) (const char* name, double value);
    void (*setStringData) (const char* name, const char* value);
    void (*setPointerData) (const char* name, void* value);
    void (*setConstPointerData) (const char* name, const void* value);
    void (*setDataObject) (const char* name, const char* type, void* value);
    MockValue_c (*getData)(const char* name);

    void (*checkExpectations)(void);
    int (*expectedCallsLeft)(void);

    void (*clear)(void);
    void (*crashOnFailure)(unsigned shouldCrash);

    void (*installComparator) (const char* typeName, MockTypeEqualFunction_c isEqual, MockTypeValueToStringFunction_c valueToString);
    void (*removeAllComparators)(void);
};


MockSupport_c* mock_c(void);
MockSupport_c* mock_scope_c(const char* scope);

#ifdef __cplusplus
}
#endif

#endif
