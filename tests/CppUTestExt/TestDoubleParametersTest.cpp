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
/// Demonstrate parity with CppuMock (i.e. actuals must meet expectations)
TEST_GROUP( MatchedActual )
{
  TEST_SETUP()
  {
    // clear any expectations
    checkExpectations();

    failUnexpected();
  }

  TEST_TEARDOWN()
  {
    checkExpectations();
  }
};

TEST( MatchedActual, match_bool )
{
  // allow bool to upcast to type
  expectCall("foo").with("value", (bool)true);
  actualCall("foo").with("value", (bool)true);
}

TEST( MatchedActual, match_char )
{
  expectCall("foo").with("value", (char)true);
  actualCall("foo").with("value", (char)true);
}

TEST( MatchedActual, match_unsigned_char )
{
  expectCall("foo").with("value", (unsigned char)true);
  actualCall("foo").with("value", (unsigned char)true);
}

TEST( MatchedActual, match_int )
{
  expectCall("foo").with("value", (int)true);
  actualCall("foo").with("value", (int)true);
}

TEST( MatchedActual, match_unsigned_int )
{
  expectCall("foo").with("value", (unsigned int)true);
  actualCall("foo").with("value", (unsigned int)true);
}

TEST( MatchedActual, match_long )
{
  expectCall("foo").with("value", (long)true);
  actualCall("foo").with("value", (long)true);
}

TEST( MatchedActual, match_unsigned_long )
{
  expectCall("foo").with("value", (unsigned long)true);
  actualCall("foo").with("value", (unsigned long)true);
}

TEST( MatchedActual, match_long_long )
{
  expectCall("foo").with("value", (long long)true);
  actualCall("foo").with("value", (long long)true);
}

TEST( MatchedActual, match_unsigned_long_long )
{
  expectCall("foo").with("value", (unsigned long long)true);
  actualCall("foo").with("value", (unsigned long long)true);
}

TEST( MatchedActual, match_float )
{
  expectCall("foo").with("value", (float)true);
  actualCall("foo").with("value", (float)true);
}

TEST( MatchedActual, match_double )
{
  expectCall("foo").with("value", (double)true);
  actualCall("foo").with("value", (double)true);
}

TEST( MatchedActual, match_pointer )
{
  char buffer[] = "HELLO";
  expectCall("foo").with("value", buffer);
  actualCall("foo").with("value", buffer);
}

TEST( MatchedActual, match_const_pointer )
{
  const char buffer[] = "HELLO";
  expectCall("foo").with("value", buffer);
  actualCall("foo").with("value", buffer);
}

static void _fn( void ) {}
TEST( MatchedActual, match_fn )
{
  expectCall("foo").with("value", _fn);
  actualCall("foo").with("value", _fn);
}

TEST( MatchedActual, match_static_buffer )
{
  char buffer[] = "HELLO";
  expectCall("foo").with("value", buffer, sizeof(buffer));
  actualCall("foo").with("value", buffer, sizeof(buffer));
}

TEST( MatchedActual, match_const_static_buffer )
{
  const char buffer[] = "HELLO";
  expectCall("foo").with("value", buffer, sizeof(buffer));
  actualCall("foo").with("value", buffer, sizeof(buffer));
}

//======================================================================================================================
TEST_GROUP( TestDoubleParametersFailures )
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
  expectCall("foo").with( "value", true );
  actualCall("foo").with( "value", "string" );
  checkExpectations();
}
TEST( TestDoubleParametersFailures, mismatch_type_fails )
{
  fixture.runTestWithMethod( mismatch_type );
}

static void mismatch_value( void )
{
  expectCall("foo").with( "value", true );
  actualCall("foo").with( "value", false );
  checkExpectations();
}
TEST( TestDoubleParametersFailures, mismatch_value_fails )
{
  fixture.runTestWithMethod( mismatch_value );
}

