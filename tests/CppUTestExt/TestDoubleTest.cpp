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
TEST_GROUP( TestDoubleState )
{
  TEST_SETUP()
  {
    // clear any expectations and reset TestDouble state
    checkExpectations();
  }
};

TEST( TestDoubleState, new_expectation_has_zero_actualCount )
{
  TestDouble::ExpectedCall* pExpectation = new TestDouble::ExpectedCall("foo");
  TestDouble::ExpectationChain chain( pExpectation, 0 );
  CHECK( 0 == chain.actualCount );
}

TEST( TestDoubleState, expectation_queue_is_FIFO )
{
  TestDouble::ExpectedCall* pExpectation1 = new TestDouble::ExpectedCall("foo");
  TestDouble::ExpectedCall* pExpectation2 = new TestDouble::ExpectedCall("foo");

  TestDouble::ExpectationQueue queue;
  queue.enqueue( pExpectation1 );
  queue.enqueue( pExpectation2 );
  CHECK( pExpectation1 == queue.get()->pExpectedCall );
  CHECK( pExpectation2 == queue.get()->pNext->pExpectedCall );

  // clean up allocated memory
  queue.check();
}

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
TEST_GROUP( IgnoreUnmatchedActual )
{
  TEST_SETUP()
  {
    // clear any expectations (default don't fail unmatched actuals)
    checkExpectations();
  }

  TEST_TEARDOWN()
  {
    // clear any expectations resources
    checkExpectations();
  }
};

TEST( IgnoreUnmatchedActual, ignore_all )
{
  actualCall("foo");
}

//======================================================================================================================
TEST_GROUP( UseModel )
{
  class OutputModel : public TestDouble::IModel
  {
  public:
    const int value = 2;    ///< don't use 1 as default actual produces true == 1

    virtual void model( TestDouble::AActualCall &actualCall )
    {
      actualCall.setOutput( "value", value );
    }
  } outputModel;

  class ReturnModel : public TestDouble::IModel
  {
  public:
    const int value = 2;    ///< don't use 1 as default actual produces true == 1

    virtual void model( TestDouble::AActualCall &actualCall )
    {
      actualCall.setReturn( value );
    }
  } returnModel;

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

TEST( UseModel, set_outputs )
{
  expectCall("doc").use( outputModel );
  int actual = 0;
  actualCall("doc").output( "value", &actual ).returns();
  CHECK( outputModel.value == actual );
}

TEST( UseModel, set_return )
{
  expectCall("doc").use( returnModel );
  int actual = actualCall("doc").returnInt();
  CHECK( returnModel.value == actual );
}


//======================================================================================================================
TEST_GROUP( TestDoubleFailures )
{
  TestTestingFixture fixture;

  TEST_SETUP()
  {
    // clear any expectations
    checkExpectations();
  }

  TEST_TEARDOWN()
  {
    // clear any expectations resources
    checkExpectations();
    CHECK( fixture.hasTestFailed() );
  }
};

static void unmet_expectation()
{
  expectCall("foo");
  checkExpectations();
}
TEST( TestDoubleFailures, unmet_expectation_fails )
{
  fixture.runTestWithMethod( unmet_expectation );
}

static void sequence()
{
  expectCall("foo");
  expectCall("bar");
  actualCall("bar");
  actualCall("foo");
  checkExpectations();
}
TEST( TestDoubleFailures, unmet_order_fails )
{
  strictOrder();
  fixture.runTestWithMethod( sequence );
}

static void expect_times_greater()
{
  expectCall("foo").times(2);
  actualCall("foo");
  checkExpectations();
}
TEST( TestDoubleFailures, expect_times_fails )
{
  fixture.runTestWithMethod( expect_times_greater );
}

static void actual_times_greater()
{
  expectCall("foo").times(1);
  actualCall("foo");
  actualCall("foo");
}

TEST( TestDoubleFailures, actual_times_fails )
{
  failUnexpected();
  fixture.runTestWithMethod( actual_times_greater );
}

// TODO
// TEST( TestDoubleFailures, model_fails )
// {
//   expectCall("doc").useModel( failModel );
//   actualCall("doc");
// }
