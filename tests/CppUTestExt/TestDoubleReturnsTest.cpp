/*
 * Copyright (c) 2018, Stephen Holstein
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
#include "CppUTest/TestTestingFixture.h"
#include "CppUTestExt/ExpectCall.h"
#include "CppUTestExt/ActualCall.h"

// TEST_GROUP( TestDoubleReturns )
// {
//   TEST_TEARDOWN()
//   {
//     // FIXME allow non-scoped expectations
//     // checkExpectations();
//   }
// };

// TEST( TestDoubleReturns, returns_bool )
// {
//   const bool expectedValue = true;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const bool actualValue = actual().call("foo").returnBool();
//   CHECK( expectedValue == actualValue );
// }

// TEST( TestDoubleReturns, returns_char )
// {
//   const char expectedValue = -1;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const char actualValue = actual().call("foo").returnChar();
//   CHECK( expectedValue == actualValue );
// }

// TEST( TestDoubleReturns, returns_unsigned_char )
// {
//   const unsigned char expectedValue = 1;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const unsigned char actualValue = actual().call("foo").returnUnsignedChar();
//   CHECK( expectedValue == actualValue );
// }

// TEST( TestDoubleReturns, returns_int )
// {
//   const int expectedValue = -1;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const int actualValue = actual().call("foo").returnInt();
//   CHECK( expectedValue == actualValue );
// }

// TEST( TestDoubleReturns, returns_unsigned_int )
// {
//   const unsigned int expectedValue = 1;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const unsigned int actualValue = actual().call("foo").returnUnsignedInt();
//   CHECK( expectedValue == actualValue );
// }

// TEST( TestDoubleReturns, returns_long )
// {
//   const long int expectedValue = -1;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const long int actualValue = actual().call("foo").returnLongInt();
//   CHECK( expectedValue == actualValue );
// }

// TEST( TestDoubleReturns, returns_unsigned_long )
// {
//   const unsigned long int expectedValue = 1;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const unsigned long int actualValue = actual().call("foo").returnUnsignedLongInt();
//   CHECK( expectedValue == actualValue );
// }

// TEST( TestDoubleReturns, returns_long_long )
// {
//   const long long int expectedValue = -1;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const long long int actualValue = actual().call("foo").returnLongLongInt();
//   CHECK( expectedValue == actualValue );
// }

// TEST( TestDoubleReturns, returns_unsigned_long_long )
// {
//   const unsigned long long int expectedValue = 1;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const unsigned long long int actualValue = actual().call("foo").returnUnsignedLongLongInt();
//   CHECK( expectedValue == actualValue );
// }

// IGNORE_TEST( TestDoubleReturns, returns_float )
// {
//   // Since CppUMock doesn't support float, clang is unhappy
//   // const float expectedValue = 1;
//   // expect().call("foo").times(1).andReturn(expectedValue);
//   // const float actualValue = actual().call("foo").returnFloat();
//   // FLOATS_EQUAL( expectedValue, actualValue, 0 );
// }

// TEST( TestDoubleReturns, returns_double )
// {
//   const double expectedValue = 1;
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const double actualValue = actual().call("foo").returnDouble();
//   DOUBLES_EQUAL( expectedValue, actualValue, 0 );
// }

// TEST( TestDoubleReturns, returns_pointer )
// {
//   void* const expectedValue = reinterpret_cast<void*>(0x100);
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const void* const actualValue = actual().call("foo").returnPointer();
//   POINTERS_EQUAL( expectedValue, actualValue );
// }

// TEST( TestDoubleReturns, returns_const_pointer )
// {
//   const void* const expectedValue = reinterpret_cast<void*>(0x100);
//   expect().call("foo").times(1).andReturn(expectedValue);
//   const void* const actualValue = actual().call("foo").returnConstPointer();
//   POINTERS_EQUAL( expectedValue, actualValue );
// }

// TEST( TestDoubleReturns, returns_function_pointer )
// {
//   expect().call("foo").times(1).andReturn(_fn);
//   void (*actualValue)() = actual().call("foo").returnFunctionPointer();
//   FUNCTIONPOINTERS_EQUAL( _fn, actualValue );
// }

