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
#include "CppUTestExt/Expect.h"
#include "CppUTestExt/ActualCall.h"

TEST_GROUP( TestDoubleParameters )
{
  TEST_SETUP()
  {
    failUnexpected( false );
  }

  TEST_TEARDOWN()
  {
    checkExpectations();
  }
};

TEST( TestDoubleParameters, expect_boolean_parameter_with_value )
{
  const bool value = true;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_char_parameter_with_value )
{
  const char value = -1;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_unsigned_char_parameter_with_value )
{
  const unsigned char value = 1;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_int_parameter_with_value )
{
  const int value = 1;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_unsigned_int_parameter_with_value )
{
  const unsigned int value = 1;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_long_parameter_with_value )
{
  const long value = 1;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_unsigned_long_parameter_with_value )
{
  const unsigned long value = 1;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_long_long_parameter_with_value )
{
  const long long value = 1;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_unsigned_long_long_parameter_with_value )
{
  const unsigned long long value = 1;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_float_parameter_with_value )
{
  const float value = 1.1f;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_double_parameter_with_value )
{
  const double value = 1.1;
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_pointer_parameter_with_value )
{
  void* const value = reinterpret_cast<void*>(0x100);
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

TEST( TestDoubleParameters, expect_const_pointer_parameter_with_value )
{
  const void* const value = reinterpret_cast<void*>(0x100);
  expectCall("foo").with("value", value);
  actualCall("foo").with("value", value);
}

static void _fn( void ) {}
TEST( TestDoubleParameters, expect_function_pointer_parameter_with_value )
{
  expectCall("foo").with("value", _fn);
  actualCall("foo").with("value", _fn);
}

// FIXME add support for objects
// TEST( TestDoubleParameters, expect_buffer_parameter_with_value )
// {
//   char buffer[] = "HELLO WORLD";
//   expectCall("foo").times(1).with("value", buffer, sizeof(buffer));
//   actualCall("foo").with("value", buffer, sizeof(buffer));
// }

TEST( TestDoubleParameters, unexpected_calls_pass )
{
  const bool value = true;
  actualCall("foo").with("value", value);
}


TEST_GROUP( TestDoubleParametersFailure )
{
  TestTestingFixture fixture;

  TEST_TEARDOWN()
  {
    checkExpectations();
  }
};

static void _mismatch_type( void )
{
  expectCall("foo").with( "value", true );
  actualCall("foo").with( "value", "string" );
}
TEST( TestDoubleParametersFailure, mismatch_type )
{
  fixture.runTestWithMethod( _mismatch_type );
  CHECK( fixture.hasTestFailed() );
}

static void _unexpected( void )
{
  actualCall("foo").with( "value", true );
}
TEST( TestDoubleParametersFailure, when_failUnexpected )
{
  failUnexpected( true );
  fixture.runTestWithMethod( _unexpected );
  CHECK( fixture.hasTestFailed() );
}

static void _expected( void )
{
  expectCall("foo").with( "value", true );
}
TEST( TestDoubleParametersFailure, when_unactualized_expectations )
{
  fixture.runTestWithMethod( _expected );
  checkExpectations();
  CHECK( fixture.hasTestFailed() );
}

