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

#include <CppUTestExt/TestDouble.h>

#include "CppUTestExt/ExpectCall.h"
#include "CppUTestExt/ActualCall.h"

#include <CppUTest/TestHarness.h>

static bool _failActuals = false;   ///< reset to false upon checkExpectations()
void failUnexpected() { _failActuals = true; }

static bool _strictOrder = false;   ///< reset to false upon checkExpectations()
void strictOrder() { _strictOrder = true; }

/// global set of expectations
static TestDouble::ExpectationQueue expectations;


void checkExpectations()
{
  // reset framework state
  _failActuals = false;
  _strictOrder = false;

  // fail upon unmatched expectations
  SimpleString report = expectations.check();
  if( false == report.isEmpty() ) FAIL( report.asCharString() );
}

TestDouble::ExpectedCall& expectCall( const SimpleString &call )
{
  TestDouble::ExpectedCall* pExpected = new TestDouble::ExpectedCall( call );
  expectations.enqueue( pExpected );
  return *pExpected;
}

TestDouble::ActualCall actualCall( const SimpleString &call ) { return TestDouble::ActualCall( call ); }



// Expectation framework (i.e. Test Double)
//======================================================================================================================
namespace TestDouble {

bool shouldFailUnexpected() { return _failActuals; }
bool shouldEnforceOrder() { return _strictOrder; }

ExpectationChain::ExpectationChain( const ExpectedCall* const &_pExpectedCall, ExpectationChain* const &pLast )
  : pExpectedCall( _pExpectedCall )
{
  if( 0 != pLast )
  {
    // attach to last entry
    pLast->pNext = this;
  }
}


void ExpectationQueue::enqueue( const ExpectedCall* const &pCall )
{
  if( 0 == _pExpectations )
    // create a new chain
    _pTail = _pExpectations = new ExpectationChain( pCall, 0 );
  else 
    // append to the tail
    _pTail = new ExpectationChain( pCall, _pTail );
}

SimpleString ExpectationQueue::check()
{
  SimpleString ret;

  // find uncalled expectations
  for( const ExpectationChain* pExpectation = expectations.get(); 0 != pExpectation; pExpectation = pExpectation->pNext )
  {
    if( ( ExpectedCall::EXPECT_ALWAYS == pExpectation->pExpectedCall->getCount() )  ?
            ( 0 >= pExpectation->actualCount )    :
            ( pExpectation->actualCount < pExpectation->pExpectedCall->getCount() ) )
    {
      ret += StringFromFormat( "unmet expectation: \n%s(", pExpectation->pExpectedCall->name.asCharString() );

      const TestDouble::ParameterChain* pFirstInput = pExpectation->pExpectedCall->getInputs();
      if( 0 != pFirstInput )
      {
        ret += "\n  INPUTS:\n";
        for( const TestDouble::ParameterChain* pEntry = pFirstInput; 0 != pEntry; pEntry = pEntry->pNext )
        {
          ret += "\t";
          ret += pEntry->pParameter->toString();
          ret += "\n";
        }
      }

      const TestDouble::ParameterChain* pFirstOutput = pExpectation->pExpectedCall->getOutputs();
      if( 0 != pFirstOutput )
      {
        ret += "\n  OUTPUTS:\n";
        for( const TestDouble::ParameterChain* pEntry = pFirstOutput; 0 != pEntry; pEntry = pEntry->pNext )
        {
          ret += "\t";
          ret += pEntry->pParameter->toString();
          ret += "\n";
        }
      }

      ret += ") RETURNS: 0x";
      ret += HexStringFrom( pExpectation->pExpectedCall->getReturn().value.asLongLong );
      ret += "\n";
    }
  }

  if( false == ret.isEmpty() ) printf( "\n%s", ret.asCharString() );

  // drop expectations
  delete _pExpectations;

  // reset expectations chain
  _pExpectations = 0;
  _pTail = 0;

  return ret;
}


static bool _matches( const ExpectationChain &expectation, const ActualCall &actual );
const ExpectedCall* findExpectation( const ActualCall &call )
{
  for( ExpectationChain* pExpectation = expectations.get(); 0 != pExpectation; pExpectation = pExpectation->pNext )
  {
    if( ( ExpectedCall::EXPECT_ALWAYS != pExpectation->pExpectedCall->getCount() )  &&
        ( pExpectation->actualCount >= pExpectation->pExpectedCall->getCount() ) )
    {
      // this expectation has already met its call limit
      continue;
    }
    if( _matches( *pExpectation, call ) )
    {
      pExpectation->actualCount++;
      return pExpectation->pExpectedCall;
    }
    if( TestDouble::shouldEnforceOrder() )
    {
      // must match current expectation before looking further
      break;
    }
  }
  return NULL;
}

static bool _matches( const ExpectationChain &expectation, const ActualCall &actual )
{
  const ExpectedCall& expected = *(expectation.pExpectedCall);
  if( actual.name != expected.name ) return false;

  for( const TestDouble::ParameterChain* pExpectedEntry=expected.getInputs(); 0 != pExpectedEntry; pExpectedEntry = pExpectedEntry->pNext )
  {
    bool used = false;  ///< ensure expected parameter is used
    for( const TestDouble::ParameterChain* pActualEntry=actual.getInputs(); 0 != pActualEntry; pActualEntry = pActualEntry->pNext )
    {
      if( pExpectedEntry->pParameter->name == pActualEntry->pParameter->name )
      {
        used = true;
        if( false == pExpectedEntry->pParameter->equals( pActualEntry->pParameter ) ) return false;
      }
    }
    if( false == used ) return false;
  }

  // match output parameters type
  for( const TestDouble::ParameterChain* pExpectedEntry=expected.getOutputs(); 0 != pExpectedEntry; pExpectedEntry = pExpectedEntry->pNext )
  {
    bool used = false;  ///< ensure expected output is used
    for( const TestDouble::ParameterChain* pActualEntry=actual.getOutputs(); 0 != pActualEntry; pActualEntry = pActualEntry->pNext )
    {
      if( pExpectedEntry->pParameter->name == pActualEntry->pParameter->name )
      {
        used = true;
        if( pExpectedEntry->pParameter->type != pActualEntry->pParameter->type ) return false;
      }
    }
    if( false == used ) return false;
  }

  return true;
}

} // namespace TestDouble


