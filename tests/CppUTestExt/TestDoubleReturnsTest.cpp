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

//======================================================================================================================
TEST_GROUP( Result )
{
  TEST_SETUP()
  {
    // clear any expectations
    checkExpectations();
  }

  TEST_TEARDOWN()
  {
    checkExpectations();
  }
};

TEST( Result, match_bool )
{
  const bool value = true;
  expectCall("foo").returns(value);
  bool actual = false;
  actual = actualCall("foo").returnBool();
  LONGS_EQUAL( value, actual );
}

TEST( Result, match_char )
{
  const char value = 'a';
  expectCall("foo").returns(value);
  char actual = 'b';
  actual = actualCall("foo").returnChar();
  LONGS_EQUAL( value, actual );
}

TEST( Result, match_unsigned_char )
{
  const unsigned char value = 'a';
  expectCall("foo").returns(value);
  unsigned char actual = 'b';
  actual = actualCall("foo").returnUnsignedChar();
  LONGS_EQUAL( value, actual );
}

TEST( Result, match_int )
{
  const int value = 1;
  expectCall("foo").returns(value);
  int actual = 2;
  actual = actualCall("foo").returnInt();
  LONGS_EQUAL( value, actual );
}

TEST( Result, match_unsigned_int )
{
  const unsigned int value = 1;
  expectCall("foo").returns(value);
  unsigned int actual = 2;
  actual = actualCall("foo").returnUnsignedInt();
  LONGS_EQUAL( value, actual );
}

TEST( Result, match_long )
{
  const long value = 1;
  expectCall("foo").returns(value);
  long actual = 2;
  actual = actualCall("foo").returnLong();
  LONGS_EQUAL( value, actual );
}

TEST( Result, match_unsigned_long )
{
  const unsigned long value = 1;
  expectCall("foo").returns(value);
  unsigned long actual = 2;
  actual = actualCall("foo").returnUnsignedLong();
  LONGS_EQUAL( value, actual );
}

TEST( Result, match_long_long )
{
  const long long value = 1;
  expectCall("foo").returns(value);
  long long actual = 2;
  actual = actualCall("foo").returnLongLong();
  LONGLONGS_EQUAL( value, actual );
}

TEST( Result, match_unsigned_long_long )
{
  const unsigned long long value = 1;
  expectCall("foo").returns(value);
  unsigned long long actual = 2;
  actual = actualCall("foo").returnUnsignedLongLong();
  LONGLONGS_EQUAL( value, actual );
}

TEST( Result, match_float )
{
  const float value = 1;
  expectCall("foo").returns(value);
  float actual = 2;
  actual = actualCall("foo").returnFloat();
  LONGLONGS_EQUAL( value, actual );
}

TEST( Result, match_double )
{
  const double value = 1;
  expectCall("foo").returns(value);
  double actual = 2;
  actual = actualCall("foo").returnDouble();
  LONGLONGS_EQUAL( value, actual );
}

TEST( Result, match_pointer )
{
  static char values[] = "HELLO";
  const char* value = values;
  expectCall("foo").returns(value);
  char* actual = 0;
  actual = actualCall("foo").returnPointer<char>();
  POINTERS_EQUAL( value, actual );
}

// FIXME
// typedef void (*fn_t)();
// static fn_t fn { };
// TEST( Result, match_fn )
// {
//   expectCall("foo").returns(fn);
//   fn_t pActual = 0;
//   pActual = actualCall("foo").returnPointer<fn_t>();
//   POINTERS_EQUAL( fn, pActual );
// }


