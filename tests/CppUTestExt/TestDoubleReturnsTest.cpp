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

#include "CppUTestExt/TestDouble.h"

// using following interfaces for testing implementation
#include "CppUTestExt/ActualCall.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestTestingFixture.h"


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
    // clear any expectations resources
    checkExpectations();
  }
};

TEST( Result, match_bool )
{
  const bool value = true;
  expectCall("foo").returns(value);
  bool actual = false;    ///< don't use true as default actual produces true == 1
  actual = actualCall("foo").returnBool();
  CHECK( value == actual );
}

TEST( Result, match_char )
{
  const char value = 'a';
  expectCall("foo").returns(value);
  char actual = 'b';
  actual = actualCall("foo").returnChar();
  CHECK( value == actual );
}

TEST( Result, match_unsigned_char )
{
  const unsigned char value = 'a';
  expectCall("foo").returns(value);
  unsigned char actual = 'b';     ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnUnsignedChar();
  CHECK( value == actual );
}

TEST( Result, match_short )
{
  const short value = 1;
  expectCall("foo").returns(value);
  short actual = 2;   ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnShort();
  CHECK( value == actual );
}

TEST( Result, match_unsigned_short )
{
  const unsigned short value = 1;
  expectCall("foo").returns(value);
  unsigned short actual = 2;    ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnUnsignedShort();
  CHECK( value == actual );
}

TEST( Result, match_int )
{
  const int value = 1;
  expectCall("foo").returns(value);
  int actual = 2;     ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnInt();
  CHECK( value == actual );
}

TEST( Result, match_unsigned_int )
{
  const unsigned int value = 1;
  expectCall("foo").returns(value);
  unsigned int actual = 2;    ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnUnsignedInt();
  CHECK( value == actual );
}

TEST( Result, match_long )
{
  const long value = 1;
  expectCall("foo").returns(value);
  long actual = 2;    ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnLong();
  CHECK( value == actual );
}

TEST( Result, match_unsigned_long )
{
  const unsigned long value = 1;
  expectCall("foo").returns(value);
  unsigned long actual = 2;     ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnUnsignedLong();
  CHECK( value == actual );
}

TEST( Result, match_long_long )
{
  const long long value = 1;
  expectCall("foo").returns(value);
  long long actual = 2;     ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnLongLong();
  CHECK( value == actual );
}

TEST( Result, match_unsigned_long_long )
{
  const unsigned long long value = 1;
  expectCall("foo").returns(value);
  unsigned long long actual = 2;    ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnUnsignedLongLong();
  CHECK( value == actual );
}

TEST( Result, match_float )
{
  const float value = 1;
  expectCall("foo").returns(value);
  float actual = 2;     ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnFloat();
  CHECK( value == actual );
}

TEST( Result, match_double )
{
  const double value = 1;
  expectCall("foo").returns(value);
  double actual = 2;    ///< don't use 1 as default actual produces true == 1
  actual = actualCall("foo").returnDouble();
  CHECK( value == actual );
}

TEST( Result, match_pointer )
{
  static char values[] = "HELLO";
  const char* pValue = values;
  expectCall("foo").returns(pValue);
  char* pActual = 0;    ///< don't use 1 as default actual produces true == 1
  pActual = actualCall("foo").returnPointer<char>();
  CHECK( pValue == pActual );
}

typedef void (*fn_t)();
static fn_t fn { };
TEST( Result, match_fn )
{
  expectCall("foo").returns(fn);
  fn_t pActual = 0;     ///< don't use 1 as default actual produces true == 1
  pActual = (fn_t)actualCall("foo").returnPointer<void>();
  POINTERS_EQUAL( fn, pActual );
}

//======================================================================================================================
TEST_GROUP( ResultDefault )
{
  TEST_SETUP()
  {
    // clear any expectations
    checkExpectations();
  }

  TEST_TEARDOWN()
  {
    // clear any expectations resources
    checkExpectations();
  }
};

TEST( ResultDefault, match_bool )
{
  const bool value = true;
  bool actual = false;
  actual = actualCall("foo").returnBool(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_char )
{
  const char value = 'a';
  char actual = 'b';
  actual = actualCall("foo").returnChar(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_unsigned_char )
{
  const unsigned char value = 'a';
  unsigned char actual = 'b';
  actual = actualCall("foo").returnUnsignedChar(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_int )
{
  const int value = 1;
  int actual = 2;
  actual = actualCall("foo").returnInt(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_unsigned_int )
{
  const unsigned int value = 1;
  unsigned int actual = 2;
  actual = actualCall("foo").returnUnsignedInt(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_long )
{
  const long value = 1;
  long actual = 2;
  actual = actualCall("foo").returnLong(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_unsigned_long )
{
  const unsigned long value = 1;
  unsigned long actual = 2;
  actual = actualCall("foo").returnUnsignedLong(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_long_long )
{
  const long long value = 1;
  long long actual = 2;
  actual = actualCall("foo").returnLongLong(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_unsigned_long_long )
{
  const unsigned long long value = 1;
  unsigned long long actual = 2;
  actual = actualCall("foo").returnUnsignedLongLong(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_float )
{
  const float value = 1;
  float actual = 2;
  actual = actualCall("foo").returnFloat(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_double )
{
  const double value = 1;
  double actual = 2;
  actual = actualCall("foo").returnDouble(value);
  CHECK( value == actual );
}

TEST( ResultDefault, match_pointer )
{
  static char values[] = "HELLO";
  const char* pValue = values;
  char* pActual = 0;
  pActual = actualCall("foo").returnPointer<char>(pValue);
  CHECK( pValue == pActual );
}

TEST( ResultDefault, match_fn )
{
  fn_t pActual = 0;
  pActual = (fn_t)actualCall("foo").returnPointer<void>((void*)fn);
  POINTERS_EQUAL( fn, pActual );
}

//======================================================================================================================
TEST_GROUP( ReturnSetsOutputs )
{
  TEST_SETUP()
  {
    // clear any expectations
    checkExpectations();
  }

  TEST_TEARDOWN()
  {
    // clear any expectations resources
    checkExpectations();
  }
};

TEST( ReturnSetsOutputs, match_void )
{
  const bool value = true;
  bool actual = false;
  actualCall("foo").output("value", &actual, value).returns();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_bool )
{
  const bool value = true;
  bool actual = false;
  actualCall("foo").output("value", &actual, value).returnBool();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_char )
{
  const char value = 'a';
  char actual = 'b';
  actualCall("foo").output("value", &actual, value).returnChar();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_unsigned_char )
{
  const unsigned char value = 'a';
  unsigned char actual = 'b';
  actualCall("foo").output("value", &actual, value).returnUnsignedChar();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_int )
{
  const int value = 1;
  int actual = 2;
  actualCall("foo").output("value", &actual, value).returnInt();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_unsigned_int )
{
  const unsigned int value = 1;
  unsigned int actual = 2;
  actualCall("foo").output("value", &actual, value).returnUnsignedInt();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_long )
{
  const long value = 1;
  long actual = 2;
  actualCall("foo").output("value", &actual, value).returnLong();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_unsigned_long )
{
  const unsigned long value = 1;
  unsigned long actual = 2;
  actualCall("foo").output("value", &actual, value).returnUnsignedLong();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_long_long )
{
  const long long value = 1;
  long long actual = 2;
  actualCall("foo").output("value", &actual, value).returnLongLong();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_unsigned_long_long )
{
  const unsigned long long value = 1;
  unsigned long long actual = 2;
  actualCall("foo").output("value", &actual, value).returnUnsignedLongLong();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_float )
{
  const float value = 1;
  float actual = 2;
  actualCall("foo").output("value", &actual, value).returnFloat();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_double )
{
  const double value = 1;
  double actual = 2;
  actualCall("foo").output("value", &actual, value).returnDouble();
  CHECK( value == actual );
}

TEST( ReturnSetsOutputs, match_pointer )
{
  static char values[] = "HELLO";
  char* pValue = values;
  char* pActual = 0;
  actualCall("foo").output("value", &pActual, pValue).returnPointer<char>();
  pActual = actualCall("foo").returnPointer<char>(pValue);
  CHECK( pValue == pActual );
}

TEST( ReturnSetsOutputs, match_fn )
{
  fn_t pActual = 0;
  actualCall("foo").output("value", &pActual, fn).returnPointer<void>();
  POINTERS_EQUAL( fn, pActual );
}






