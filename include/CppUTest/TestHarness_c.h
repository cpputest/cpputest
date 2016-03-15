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

/******************************************************************************
 *
 * Provides an interface for when working with pure C
 *
 *******************************************************************************/

#ifndef D_TestHarness_c_h
#define D_TestHarness_c_h

#include "CppUTestConfig.h"

#define CHECK_EQUAL_C_INT(expected,actual) \
  CHECK_EQUAL_C_INT_LOCATION(expected,actual,__FILE__,__LINE__)

#define CHECK_EQUAL_C_REAL(expected,actual,threshold) \
  CHECK_EQUAL_C_REAL_LOCATION(expected,actual,threshold,__FILE__,__LINE__)

#define CHECK_EQUAL_C_CHAR(expected,actual) \
  CHECK_EQUAL_C_CHAR_LOCATION(expected,actual,__FILE__,__LINE__)

#define CHECK_EQUAL_C_STRING(expected,actual) \
  CHECK_EQUAL_C_STRING_LOCATION(expected,actual,__FILE__,__LINE__)

#define CHECK_EQUAL_C_POINTER(expected,actual) \
  CHECK_EQUAL_C_POINTER_LOCATION(expected,actual,__FILE__,__LINE__)

#define CHECK_EQUAL_C_BITS(expected, actual, mask)\
  CHECK_EQUAL_C_BITS_LOCATION(expected, actual, mask, sizeof(actual), __FILE__, __LINE__)

#define FAIL_TEXT_C(text) \
  FAIL_TEXT_C_LOCATION(text,__FILE__,__LINE__)

#define FAIL_C() \
  FAIL_C_LOCATION(__FILE__,__LINE__)

#define CHECK_C(condition) \
  CHECK_C_LOCATION(condition, #condition, __FILE__,__LINE__)


/******************************************************************************
 *
 * TEST macros for in C.
 *
 *******************************************************************************/

/* For use in C file */
#define TEST_GROUP_C_SETUP(group_name) \
    extern void group_##group_name##_setup_wrapper_c(void); \
    void group_##group_name##_setup_wrapper_c()

#define TEST_GROUP_C_TEARDOWN(group_name) \
    extern void group_##group_name##_teardown_wrapper_c(void); \
    void group_##group_name##_teardown_wrapper_c()

#define TEST_C(group_name, test_name) \
    extern void test_##group_name##_##test_name##_wrapper_c(void);\
    void test_##group_name##_##test_name##_wrapper_c()


/* For use in C++ file */

#define TEST_GROUP_C_WRAPPER(group_name) \
    extern "C" void group_##group_name##_setup_wrapper_c(void); \
    extern "C" void group_##group_name##_teardown_wrapper_c(void); \
    TEST_GROUP(group_name)

#define TEST_GROUP_C_SETUP_WRAPPER(group_name) \
    void setup() { \
       group_##group_name##_setup_wrapper_c(); \
    }

#define TEST_GROUP_C_TEARDOWN_WRAPPER(group_name) \
    void teardown() { \
       group_##group_name##_teardown_wrapper_c(); \
    }

#define TEST_C_WRAPPER(group_name, test_name) \
    extern "C" void test_##group_name##_##test_name##_wrapper_c(); \
    TEST(group_name, test_name) { \
        test_##group_name##_##test_name##_wrapper_c(); \
    }

#ifdef __cplusplus
extern "C"
{
#endif


/* CHECKS that can be used from C code */
extern void CHECK_EQUAL_C_INT_LOCATION(int expected, int actual,
        const char* fileName, int lineNumber);
extern void CHECK_EQUAL_C_REAL_LOCATION(double expected, double actual,
        double threshold, const char* fileName, int lineNumber);
extern void CHECK_EQUAL_C_CHAR_LOCATION(char expected, char actual,
        const char* fileName, int lineNumber);
extern void CHECK_EQUAL_C_STRING_LOCATION(const char* expected,
        const char* actual, const char* fileName, int lineNumber);
extern void CHECK_EQUAL_C_POINTER_LOCATION(const void* expected,
        const void* actual, const char* fileName, int lineNumber);
extern void CHECK_EQUAL_C_BITS_LOCATION(unsigned int expected, unsigned int actual,
        unsigned int mask, size_t size, const char* fileName, int lineNumber);
extern void FAIL_TEXT_C_LOCATION(const char* text, const char* fileName,
        int lineNumber);
extern void FAIL_C_LOCATION(const char* fileName, int lineNumber);
extern void CHECK_C_LOCATION(int condition, const char* conditionString,
        const char* fileName, int lineNumber);

extern void* cpputest_malloc(size_t size);
extern void* cpputest_calloc(size_t num, size_t size);
extern void* cpputest_realloc(void* ptr, size_t size);
extern void  cpputest_free(void* buffer);

extern void* cpputest_malloc_location(size_t size, const char* file, int line);
extern void* cpputest_calloc_location(size_t num, size_t size,
        const char* file, int line);
extern void* cpputest_realloc_location(void* memory, size_t size,
        const char* file, int line);
extern void cpputest_free_location(void* buffer, const char* file, int line);

void cpputest_malloc_set_out_of_memory(void);
void cpputest_malloc_set_not_out_of_memory(void);
void cpputest_malloc_set_out_of_memory_countdown(int);
void cpputest_malloc_count_reset(void);
int cpputest_malloc_get_count(void);

#ifdef __cplusplus
}
#endif


/*
 * Small additional macro for unused arguments. This is common when stubbing, but in C you cannot remove the
 * name of the parameter (as in C++).
 */

#ifndef PUNUSED
#if defined(__GNUC__)
# define PUNUSED(x) PUNUSED_ ##x __attribute__((unused))
#else
# define PUNUSED(x) x
#endif
#endif

#endif
