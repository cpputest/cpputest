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
#include "CppUTestExt/TestDouble.h"
#include "CppUTestExt/ExpectCall.h"
#include "CppUTestExt/ActualCall.h"

//======================================================================================================================
TEST_GROUP( MatchedOutputParameter )
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

TEST( MatchedOutputParameter, match_bool )
{
  const bool value = true;
  expectCall("foo").output("value", value);
  bool actual = !value;
  actualCall("foo").output("value", &actual).returns();
  LONGS_EQUAL( value, actual );
}

TEST( MatchedOutputParameter, match_char )
{
  const char value = 'a';
  expectCall("foo").output("value", value);
  char actual = 'b';
  actualCall("foo").output("value", &actual).returns();
  LONGS_EQUAL( value, actual );
}

TEST( MatchedOutputParameter, match_unsigned_char )
{
  const unsigned char value = 'a';
  expectCall("foo").output("value", value);
  unsigned char actual = 'b';
  actualCall("foo").output("value", &actual).returns();
  LONGS_EQUAL( value, actual );
}

TEST( MatchedOutputParameter, match_int )
{
  const int value = 1;
  expectCall("foo").output("value", value);
  int actual = 2;
  actualCall("foo").output("value", &actual).returns();
  LONGS_EQUAL( value, actual );
}

TEST( MatchedOutputParameter, match_unsigned_int )
{
  const unsigned int value = 1;
  expectCall("foo").output("value", value);
  unsigned int actual = 2;
  actualCall("foo").output("value", &actual).returns();
  LONGS_EQUAL( value, actual );
}

TEST( MatchedOutputParameter, match_long )
{
  const long value = 1;
  expectCall("foo").output("value", value);
  long actual = 2;
  actualCall("foo").output("value", &actual).returns();
  LONGS_EQUAL( value, actual );
}

TEST( MatchedOutputParameter, match_unsigned_long )
{
  const unsigned long value = 1;
  expectCall("foo").output("value", value);
  unsigned long actual = 2;
  actualCall("foo").output("value", &actual).returns();
  LONGS_EQUAL( value, actual );
}

TEST( MatchedOutputParameter, match_long_long )
{
  const long long value = 1;
  expectCall("foo").output("value", value);
  long long actual = 2;
  actualCall("foo").output("value", &actual).returns();
  LONGS_EQUAL( value, actual );
}

TEST( MatchedOutputParameter, match_unsigned_long_long )
{
  const unsigned long long value = 1;
  expectCall("foo").output("value", value);
  unsigned long long actual = 2;
  actualCall("foo").output("value", &actual).returns();
  LONGS_EQUAL( value, actual );
}

TEST( MatchedOutputParameter, match_float )
{
  const float value = 1.1f;
  expectCall("foo").output("value", value);
  float actual = 2.2f;
  actualCall("foo").output("value", &actual).returns();
  CHECK( value == actual );
}

TEST( MatchedOutputParameter, match_double )
{
  const double value = 1.1;
  expectCall("foo").output("value", value);
  double actual = 2.2;
  actualCall("foo").output("value", &actual).returns();
  CHECK( value == actual );
}

TEST( MatchedOutputParameter, match_pointer )
{
  static char values[] = "HELLO";
  char* pValue = values;
  expectCall("foo").output("value", pValue);
  char* pActual = 0;
  actualCall("foo").output("value", &pActual).returns();
  POINTERS_EQUAL( pValue, pActual );
}

TEST( MatchedOutputParameter, match_const_pointer )
{
  const char values[] = "HELLO";
  const char* pValue = values;
  expectCall("foo").output("value", pValue);
  const char* pActual = 0;
  actualCall("foo").output("value", &pActual).returns();
  POINTERS_EQUAL( pValue, pActual );
}

typedef void (*fn_t)();
static fn_t fn { };
TEST( MatchedOutputParameter, match_fn )
{
  expectCall("foo").output("value", fn);
  fn_t pActual = 0;
  actualCall("foo").output("value", &pActual).returns();
  POINTERS_EQUAL( fn, pActual );
}

// FIXME not implemented
IGNORE_TEST( MatchedOutputParameter, match_static_buffer )
{
  const char values[] = "HELLO";
  expectCall("foo").output("value", values, sizeof(values));
  char actuals[] = "UHTOH";
  actualCall("foo").output("value", actuals, sizeof(actuals)).returns();
  MEMCMP_EQUAL( values, actuals, sizeof(values) );
}


//======================================================================================================================
TEST_GROUP( TestDoubleOutputsFailures )
{
  TestTestingFixture fixture;

  TEST_SETUP()
  {
    // clear any expectations
    checkExpectations();
  }

  TEST_TEARDOWN()
  {
    CHECK( fixture.hasTestFailed() );
  }
};

// static void mismatch_type( void )
// {
//   expectCall("foo").output( "value", true );
//   char actual;
//   actualCall("foo").output( "value", &actual );
//   checkExpectations();
// }
// TEST( TestDoubleOutputsFailures, mismatch_type_fails )
// {
//   fixture.runTestWithMethod( mismatch_type );
// }

