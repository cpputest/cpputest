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

static void _fn( void ) {}
TEST( MatchedActual, match_all )
{
  // allow bool to upcast to type
  expectCall("foo").with("value", (bool)true);
  actualCall("foo").with("value", (bool)true);

  expectCall("foo").with("value", (char)true);
  actualCall("foo").with("value", (char)true);

  expectCall("foo").with("value", (unsigned char)true);
  actualCall("foo").with("value", (unsigned char)true);

  expectCall("foo").with("value", (int)true);
  actualCall("foo").with("value", (int)true);

  expectCall("foo").with("value", (unsigned int)true);
  actualCall("foo").with("value", (unsigned int)true);

  expectCall("foo").with("value", (long)true);
  actualCall("foo").with("value", (long)true);

  expectCall("foo").with("value", (unsigned long)true);
  actualCall("foo").with("value", (unsigned long)true);

  expectCall("foo").with("value", (long long)true);
  actualCall("foo").with("value", (long long)true);

  expectCall("foo").with("value", (unsigned long long)true);
  actualCall("foo").with("value", (unsigned long long)true);

  expectCall("foo").with("value", (float)true);
  actualCall("foo").with("value", (float)true);

  expectCall("foo").with("value", (double)true);
  actualCall("foo").with("value", (double)true);

  char buffer[] = "HELLO";
  expectCall("foo").with("value", buffer);
  actualCall("foo").with("value", buffer);

  expectCall("foo").with("value", (const char*)buffer);
  actualCall("foo").with("value", (const char*)buffer);

  expectCall("foo").with("value", _fn);
  actualCall("foo").with("value", _fn);

  expectCall("foo").with("value", buffer, sizeof(buffer));
  actualCall("foo").with("value", buffer, sizeof(buffer));
}


//======================================================================================================================
TEST_GROUP( IgnoreUnmatchedActual )
{
  TEST_SETUP()
  {
    // clear any expectations
    checkExpectations();

    // default don't fail unmatched actuals
  }

  TEST_TEARDOWN()
  {
    checkExpectations();
  }
};

TEST( IgnoreUnmatchedActual, ignore_all )
{
  // allow bool to upcast to type
  actualCall("foo").with("value", true);
  actualCall("foo").with("value", (char)true);
  actualCall("foo").with("value", (unsigned char)true);
  actualCall("foo").with("value", (int)true);
  actualCall("foo").with("value", (unsigned int)true);
  actualCall("foo").with("value", (long)true);
  actualCall("foo").with("value", (unsigned long)true);
  actualCall("foo").with("value", (long)true);
  actualCall("foo").with("value", (unsigned long)true);
  actualCall("foo").with("value", (long long)true);
  actualCall("foo").with("value", (unsigned long long)true);
  actualCall("foo").with("value", (float)true);
  actualCall("foo").with("value", (double)true);
  char buffer[] = "HELLO";
  actualCall("foo").with("value", buffer);
  actualCall("foo").with("value", (const char*)buffer);
  actualCall("foo").with("value", _fn);
  actualCall("foo").with("value", buffer, sizeof(buffer));
}


//======================================================================================================================
TEST_GROUP( TestDoubleState )
{
  TEST_SETUP()
  {
    // clear any expectations
    checkExpectations();
  }
};

TEST( TestDoubleState, upon_checkExpectations_restore_default_state )
{
  // assert default state
  CHECK_FALSE( TestDouble::shouldFailUnexpected() );
  CHECK_FALSE( TestDouble::shouldEnforceOrder() );

  // modify state
  failUnexpected();
  CHECK_TRUE( TestDouble::shouldFailUnexpected() );
  strictOrder();
  CHECK_TRUE( TestDouble::shouldEnforceOrder() );

  checkExpectations();

  // assert default state
  CHECK_FALSE( TestDouble::shouldFailUnexpected() );
  CHECK_FALSE( TestDouble::shouldEnforceOrder() );
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

static void unexpectedParameter()
{
  expectCall("foo");
  checkExpectations();
}
IGNORE_TEST( TestDoubleParametersFailures, when_unmatched_expecations_fail )
{
  fixture.runTestWithMethod( unexpectedParameter );
}


// static void _mismatch_type( void )
// {
//   expectCall("foo").with( "value", true );
//   actualCall("foo").with( "value", "string" );
// }
// TEST( TestDoubleParametersFailure, mismatch_type ) // {
//   fixture.runTestWithMethod( _mismatch_type );
//   CHECK( fixture.hasTestFailed() );
// }

// static void _unexpected( void )
// {
//   actualCall("foo").with( "value", true );
// }
// TEST( TestDoubleParametersFailure, when_failUnexpected )
// {
//   failUnexpected( true );
//   fixture.runTestWithMethod( _unexpected );
//   CHECK( fixture.hasTestFailed() );
// }

// static void _expected( void )
// {
//   expectCall("foo").with( "value", true );
//   checkExpectations();
// }
// TEST( TestDoubleParametersFailure, when_unactualized_expectations )
// {
//   fixture.runTestWithMethod( _expected );
//   CHECK( fixture.hasTestFailed() );
// }

