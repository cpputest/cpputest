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

IGNORE_TEST( MatchedOutputParameter, match_bool )
{
  const bool value = true;
  expectCall("foo").output("value", value);
  bool actual;
  actualCall("foo").output("value", &actual);
  LONGS_EQUAL( value, actual );
}

// TEST( MatchedOutputParameter, match_char )
// {
//   char value = 'a';
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_unsigned_char )
// {
//   unsigned char value = 'a';
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_int )
// {
//   int value = 1;
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_unsigned_int )
// {
//   unsigned int value = 1;
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_long )
// {
//   long value = 1;
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_unsigned_long )
// {
//   unsigned long value = 1;
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_long_long )
// {
//   long long value = 1;
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_unsigned_long_long )
// {
//   unsigned long long value = 1;
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_float )
// {
//   float value = 1.0;
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_double )
// {
//   double value = 1.0;
//   expectCall("foo").with("value", value);
//   actualCall("foo").with("value", value);
// }

// TEST( MatchedOutputParameter, match_pointer )
// {
//   char buffer[] = "HELLO";
//   expectCall("foo").with("value", buffer);
//   actualCall("foo").with("value", buffer);
// }

// TEST( MatchedOutputParameter, match_const_pointer )
// {
//   const char buffer[] = "HELLO";
//   expectCall("foo").with("value", buffer);
//   actualCall("foo").with("value", buffer);
// }

// static void _fn( void ) {}
// TEST( MatchedOutputParameter, match_fn )
// {
//   expectCall("foo").with("value", _fn);
//   actualCall("foo").with("value", _fn);
// }

// TEST( MatchedOutputParameter, match_static_buffer )
// {
//   char buffer[] = "HELLO";
//   expectCall("foo").with("value", buffer, sizeof(buffer));
//   actualCall("foo").with("value", buffer, sizeof(buffer));
// }


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

static void mismatch_type( void )
{
  expectCall("foo").output( "value", true );
  actualCall("foo").output( "value", "string" );
  checkExpectations();
}
IGNORE_TEST( TestDoubleOutputsFailures, mismatch_type_fails )
{
  fixture.runTestWithMethod( mismatch_type );
}

