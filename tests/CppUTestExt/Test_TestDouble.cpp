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
//#include "CppUTestExt/MockExpectedCall.h"
//#include "CppUTestExt/MockFailure.h"
// #include "MockFailureReporterForTest.h"
#include "CppUTestExt/Expect.h"
#include "CppUTestExt/Actual.h"
#include "CppUTestExt/TestDouble.h"

TEST_GROUP( TestDouble )
{

};

IGNORE_TEST( TestDouble, upon_FAIL_report_unexpected_calls )
{
  // TODO CppUMock FAILS upon first unexpected call
}

IGNORE_TEST( TestDouble, FAIL_if_expected_calls_arent_actualized )
{
}


TEST( TestDouble, expect_boolean_parameter_with_value )
{
  const bool value = true;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_char_parameter_with_value )
{
  const char value = -1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_unsigned_char_parameter_with_value )
{
  const unsigned char value = 1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_int_parameter_with_value )
{
  const int value = 1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_unsigned_int_parameter_with_value )
{
  const unsigned int value = 1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_long_parameter_with_value )
{
  const long value = 1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_unsigned_long_parameter_with_value )
{
  const unsigned long value = 1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_long_long_parameter_with_value )
{
  const long long value = 1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_unsigned_long_long_parameter_with_value )
{
  const unsigned long long value = 1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_float_parameter_with_value )
{
  const float value = 1.1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_double_parameter_with_value )
{
  const double value = 1.1;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_string_with_value )
{
  char value[] = "HELLO WORLD";
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_pointer_parameter_with_value )
{
  void* const value = 0;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

TEST( TestDouble, expect_const_pointer_parameter_with_value )
{
  const void* const value = 0;
  expect().call("foo").times(1).with("value", value);
  actual().call("foo").with("value", value);
  checkExpectations();
}

static void _fn( void ) {};
TEST( TestDouble, expect_function_pointer_parameter_with_value )
{
  expect().call("foo").times(1).with("value", _fn);
  actual().call("foo").with("value", _fn);
  checkExpectations();
}

TEST( TestDouble, expect_buffer_parameter_with_value )
{
  char buffer[] = "HELLO WORLD";
  expect().call("foo").times(1).with("value", buffer, sizeof(buffer));
  actual().call("foo").with("value", buffer, sizeof(buffer));
  checkExpectations();
}

