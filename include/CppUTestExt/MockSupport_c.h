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

#include "CppUTest/CppUTestConfig.h"
#include "CppUTest/StandardCLibrary.h"

typedef enum {
    MOCKVALUETYPE_BOOL,
    MOCKVALUETYPE_UNSIGNED_INTEGER,
    MOCKVALUETYPE_INTEGER,
    MOCKVALUETYPE_LONG_INTEGER,
    MOCKVALUETYPE_UNSIGNED_LONG_INTEGER,
    MOCKVALUETYPE_LONG_LONG_INTEGER,
    MOCKVALUETYPE_UNSIGNED_LONG_LONG_INTEGER,
    MOCKVALUETYPE_DOUBLE,
    MOCKVALUETYPE_STRING,
    MOCKVALUETYPE_POINTER,
    MOCKVALUETYPE_CONST_POINTER,
    MOCKVALUETYPE_FUNCTIONPOINTER,
    MOCKVALUETYPE_MEMORYBUFFER,
    MOCKVALUETYPE_OBJECT
} MockValueType_c;

typedef struct SMockValue_c
{
    MockValueType_c type;
    union {
        int boolValue;
        int intValue;
        unsigned int unsignedIntValue;
        long int longIntValue;
        unsigned long int unsignedLongIntValue;
#ifdef CPPUTEST_USE_LONG_LONG
        cpputest_longlong longLongIntValue;
        cpputest_ulonglong unsignedLongLongIntValue;
#else
        char longLongPlaceholder[CPPUTEST_SIZE_OF_FAKE_LONG_LONG_TYPE];
#endif
        double doubleValue;
        const char* stringValue;
        void* pointerValue;
        const void* constPointerValue;
        void (*functionPointerValue)(void);
        const unsigned char* memoryBufferValue;
        void* objectValue;
        const void* constObjectValue;
    } value;
} MockValue_c;

typedef struct SMockActualCall_c MockActualCall_c;
struct SMockActualCall_c
{
    MockActualCall_c* (*withBoolParameters)(const char* name, int value);
    MockActualCall_c* (*withIntParameters)(const char* name, int value);
    MockActualCall_c* (*withUnsignedIntParameters)(const char* name, unsigned int value);
    MockActualCall_c* (*withLongIntParameters)(const char* name, long int value);
    MockActualCall_c* (*withUnsignedLongIntParameters)(const char* name, unsigned long int value);
    MockActualCall_c* (*withLongLongIntParameters)(const char* name, cpputest_longlong value);
    MockActualCall_c* (*withUnsignedLongLongIntParameters)(const char* name, cpputest_ulonglong value);
    MockActualCall_c* (*withDoubleParameters)(const char* name, double value);
    MockActualCall_c* (*withStringParameters)(const char* name, const char* value);
    MockActualCall_c* (*withPointerParameters)(const char* name, void* value);
    MockActualCall_c* (*withConstPointerParameters)(const char* name, const void* value);
    MockActualCall_c* (*withFunctionPointerParameters)(const char* name, void (*value)(void));
    MockActualCall_c* (*withMemoryBufferParameter)(const char* name, const unsigned char* value, size_t size);
    MockActualCall_c* (*withParameterOfType)(const char* type, const char* name, const void* value);
    MockActualCall_c* (*withOutputParameter)(const char* name, void* value);
    MockActualCall_c* (*withOutputParameterOfType)(const char* type, const char* name, void* value);
    int (*hasReturnValue)(void);
    MockValue_c (*returnValue)(void);
    int (*boolReturnValue)(void);
    int (*returnBoolValueOrDefault)(int defaultValue);
    int (*intReturnValue)(void);
    int (*returnIntValueOrDefault)(int defaultValue);
    unsigned int (*unsignedIntReturnValue)(void);
    unsigned int (*returnUnsignedIntValueOrDefault)(unsigned int defaultValue);
    long int (*longIntReturnValue)(void);
    long int (*returnLongIntValueOrDefault)(long int defaultValue);
    unsigned long int (*unsignedLongIntReturnValue)(void);
    unsigned long int (*returnUnsignedLongIntValueOrDefault)(unsigned long int defaultValue);
    cpputest_longlong (*longLongIntReturnValue)(void);
    cpputest_longlong (*returnLongLongIntValueOrDefault)(cpputest_longlong defaultValue);
    cpputest_ulonglong (*unsignedLongLongIntReturnValue)(void);
    cpputest_ulonglong (*returnUnsignedLongLongIntValueOrDefault)(cpputest_ulonglong defaultValue);
    const char* (*stringReturnValue)(void);
    const char* (*returnStringValueOrDefault)(const char * defaultValue);
    double (*doubleReturnValue)(void);
    double (*returnDoubleValueOrDefault)(double defaultValue);
    void* (*pointerReturnValue)(void);
    void* (*returnPointerValueOrDefault)(void * defaultValue);
    const void* (*constPointerReturnValue)(void);
    const void* (*returnConstPointerValueOrDefault)(const void * defaultValue);
    void (*(*functionPointerReturnValue)(void))(void);
    void (*(*returnFunctionPointerValueOrDefault)(void(*defaultValue)(void)))(void);
};

typedef struct SMockExpectedCall_c MockExpectedCall_c;
struct SMockExpectedCall_c
{
    MockExpectedCall_c* (*withBoolParameters)(const char* name, int value);
    MockExpectedCall_c* (*withIntParameters)(const char* name, int value);
    MockExpectedCall_c* (*withUnsignedIntParameters)(const char* name, unsigned int value);
    MockExpectedCall_c* (*withLongIntParameters)(const char* name, long int value);
    MockExpectedCall_c* (*withUnsignedLongIntParameters)(const char* name, unsigned long int value);
    MockExpectedCall_c* (*withLongLongIntParameters)(const char* name, cpputest_longlong value);
    MockExpectedCall_c* (*withUnsignedLongLongIntParameters)(const char* name, cpputest_ulonglong value);
    MockExpectedCall_c* (*withDoubleParameters)(const char* name, double value);
    MockExpectedCall_c* (*withDoubleParametersAndTolerance)(const char* name, double value, double tolerance);
    MockExpectedCall_c* (*withStringParameters)(const char* name, const char* value);
    MockExpectedCall_c* (*withPointerParameters)(const char* name, void* value);
    MockExpectedCall_c* (*withConstPointerParameters)(const char* name, const void* value);
    MockExpectedCall_c* (*withFunctionPointerParameters)(const char* name, void (*value)(void));
    MockExpectedCall_c* (*withMemoryBufferParameter)(const char* name, const unsigned char* value, size_t size);
    MockExpectedCall_c* (*withParameterOfType)(const char* type, const char* name, const void* value);
    MockExpectedCall_c* (*withOutputParameterReturning)(const char* name, const void* value, size_t size);
    MockExpectedCall_c* (*withOutputParameterOfTypeReturning)(const char* type, const char* name, const void* value);
    MockExpectedCall_c* (*withUnmodifiedOutputParameter)(const char* name);
    MockExpectedCall_c* (*ignoreOtherParameters)(void);

    MockExpectedCall_c* (*andReturnBoolValue)(int value);
    MockExpectedCall_c* (*andReturnUnsignedIntValue)(unsigned int value);
    MockExpectedCall_c* (*andReturnIntValue)(int value);
    MockExpectedCall_c* (*andReturnLongIntValue)(long int value);
    MockExpectedCall_c* (*andReturnUnsignedLongIntValue)(unsigned long int value);
    MockExpectedCall_c* (*andReturnLongLongIntValue)(cpputest_longlong value);
    MockExpectedCall_c* (*andReturnUnsignedLongLongIntValue)(cpputest_ulonglong value);
    MockExpectedCall_c* (*andReturnDoubleValue)(double value);
    MockExpectedCall_c* (*andReturnStringValue)(const char* value);
    MockExpectedCall_c* (*andReturnPointerValue)(void* value);
    MockExpectedCall_c* (*andReturnConstPointerValue)(const void* value);
    MockExpectedCall_c* (*andReturnFunctionPointerValue)(void (*value)(void));
};

typedef int (*MockTypeEqualFunction_c)(const void* object1, const void* object2);
typedef const char* (*MockTypeValueToStringFunction_c)(const void* object1);
typedef void (*MockTypeCopyFunction_c)(void* dst, const void* src);

typedef struct SMockSupport_c MockSupport_c;
struct SMockSupport_c
{
    void (*strictOrder)(void);
    MockExpectedCall_c* (*expectOneCall)(const char* name);
    void (*expectNoCall)(const char* name);
    MockExpectedCall_c* (*expectNCalls)(unsigned int number, const char* name);
    MockActualCall_c* (*actualCall)(const char* name);
    int (*hasReturnValue)(void);
    MockValue_c (*returnValue)(void);
    int (*boolReturnValue)(void);
    int (*returnBoolValueOrDefault)(int defaultValue);
    int (*intReturnValue)(void);
    int (*returnIntValueOrDefault)(int defaultValue);
    unsigned int (*unsignedIntReturnValue)(void);
    unsigned int (*returnUnsignedIntValueOrDefault)(unsigned int defaultValue);
    long int (*longIntReturnValue)(void);
    long int (*returnLongIntValueOrDefault)(long int defaultValue);
    unsigned long int (*unsignedLongIntReturnValue)(void);
    unsigned long int (*returnUnsignedLongIntValueOrDefault)(unsigned long int defaultValue);
    cpputest_longlong (*longLongIntReturnValue)(void);
    cpputest_longlong (*returnLongLongIntValueOrDefault)(cpputest_longlong defaultValue);
    cpputest_ulonglong (*unsignedLongLongIntReturnValue)(void);
    cpputest_ulonglong (*returnUnsignedLongLongIntValueOrDefault)(cpputest_ulonglong defaultValue);
    const char* (*stringReturnValue)(void);
    const char* (*returnStringValueOrDefault)(const char * defaultValue);
    double (*doubleReturnValue)(void);
    double (*returnDoubleValueOrDefault)(double defaultValue);
    void* (*pointerReturnValue)(void);
    void* (*returnPointerValueOrDefault)(void * defaultValue);
    const void* (*constPointerReturnValue)(void);
    const void* (*returnConstPointerValueOrDefault)(const void * defaultValue);
    void (*(*functionPointerReturnValue)(void))(void);
    void (*(*returnFunctionPointerValueOrDefault) (void(*defaultValue)(void)))(void);

    void (*setBoolData) (const char* name, int value);
    void (*setIntData) (const char* name, int value);
    void (*setUnsignedIntData) (const char* name, unsigned int value);
    void (*setStringData) (const char* name, const char* value);
    void (*setDoubleData) (const char* name, double value);
    void (*setPointerData) (const char* name, void* value);
    void (*setConstPointerData) (const char* name, const void* value);
    void (*setFunctionPointerData) (const char* name, void (*value)(void));
    void (*setDataObject) (const char* name, const char* type, void* value);
    void (*setDataConstObject) (const char* name, const char* type, const void* value);
    MockValue_c (*getData)(const char* name);

    void (*disable)(void);
    void (*enable)(void);
    void (*ignoreOtherCalls)(void);

    void (*checkExpectations)(void);
    int (*expectedCallsLeft)(void);

    void (*clear)(void);
    void (*crashOnFailure)(unsigned shouldCrash);

    void (*installComparator) (const char* typeName, MockTypeEqualFunction_c isEqual, MockTypeValueToStringFunction_c valueToString);
    void (*installCopier) (const char* typeName, MockTypeCopyFunction_c copier);
    void (*removeAllComparatorsAndCopiers)(void);
};

MockSupport_c* mock_c(void);
MockSupport_c* mock_scope_c(const char* scope);

#ifdef __cplusplus
}
#endif

#endif
