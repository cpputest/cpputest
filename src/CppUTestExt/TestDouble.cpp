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

static bool _failActuals = false;
void failUnexpected( bool mode ) { _failActuals = mode; }

void checkExpectations()
{
  TestDouble::expectations.check();
}

ExpectedCall& expectCall( const SimpleString& call )
{
  ExpectedCall* pExpected = new ExpectedCall( call );
  TestDouble::expectations.enqueue( pExpected );
  return *pExpected;
}

ActualCall actualCall( const SimpleString& call ) { return ActualCall( call ); }



namespace TestDouble {

ExpectationChain::ExpectationChain( const ExpectedCall* const _pExpectedCall, ExpectationChain* const pLast )
  : pExpectedCall( _pExpectedCall )
   ,pNext(0)
{
  if( 0 != pLast )
  {
    // append to chain
    pLast->pNext = this;
  }
}
ExpectationChain::~ExpectationChain()
{
  delete pExpectedCall;
  delete pNext;
}


ExpectationQueue::~ExpectationQueue()
{
  delete _pExpectations;
}

void ExpectationQueue::enqueue( const ExpectedCall* pCall )
{
  if( 0 == TestDouble::expectations._pExpectations )
  {
    TestDouble::expectations._pExpectations = new ExpectationChain( pCall, 0 );
    TestDouble::expectations._pTail = _pExpectations;
  }
  else 
  {
    TestDouble::expectations._pTail = new ExpectationChain( pCall, TestDouble::expectations._pTail );
  }
}

void ExpectationQueue::check()
{
  // TODO find uncalled expectations

  // drop expectations state
  delete TestDouble::expectations._pExpectations;
  TestDouble::expectations._pExpectations = 0;
}


static bool _matches( const ExpectationChain& expectation, const ActualCall& actual );
const ExpectedCall* findExpectation( const ActualCall& call )
{
  for( ExpectationChain* pExpectation = TestDouble::expectations.get(); 0 != pExpectation; pExpectation = pExpectation->pNext )
  {
    if( _matches( *pExpectation, call ) ) return pExpectation->pExpectedCall;
  }
  return NULL;
}

ParameterChain::ParameterChain( const TestDouble::Parameter* const _pParameter, ParameterChain* const _pNext )
  : pParameter(_pParameter)
   ,pNext(_pNext)   ///< prepend chain
{}
ParameterChain::~ParameterChain()
{
  delete pParameter;
  delete pNext;
}


/// upon match, the expectation::calledCount will be incremented
static bool _matches( const ExpectationChain& expectation, const ActualCall& actual )
{
  const ExpectedCall expected = *(expectation.pExpectedCall);
  if( actual.methodName != expected.methodName ) return false;

  for( const TestDouble::ParameterChain* pExpectedEntry=expected.getParameters(); 0 != pExpectedEntry; pExpectedEntry = pExpectedEntry->pNext )
  {
    for( const TestDouble::ParameterChain* pActualEntry=actual.getParameters(); 0 != pActualEntry; pActualEntry = pActualEntry->pNext )
    {
      if( pExpectedEntry->pParameter->name == pActualEntry->pParameter->name )
      {
        if( pExpectedEntry->pParameter->type != pActualEntry->pParameter->type )
        {
          UT_PRINT( StringFromFormat( "Type Mismatch: Expected call to '%s' with parameter '%s' of type '%s', but actual paramter was of type '%s'.",
            expected.methodName.asCharString(), pExpectedEntry->pParameter->name.asCharString(),
            pExpectedEntry->pParameter->type.asCharString(), pActualEntry->pParameter->type.asCharString() ).asCharString()
          );
          return false;
        }

        if( false == pExpectedEntry->pParameter->equals( pActualEntry->pParameter ) ) return false;
      }
    }
  }

  return true;
}
// bool Expectations::check()
// {
//   bool passed = true;

//   // clean up expectations
//   while( _expectedCalls != 0 )
//   {
//     const ExpectedCallEntry* pExpectedCallEntry = _expectedCalls;
//     const ExpectedCall& expectedCall = *(pExpectedCallEntry->pExpectedCall);
//     if( ( ( expectedCall.getCount() == ExpectedCall::EXPECT_ALWAYS ) && ( pExpectedCallEntry->calledCount == 0 ) )   ||
//         ( ( expectedCall.getCount() != ExpectedCall::EXPECT_ALWAYS ) && ( pExpectedCallEntry->calledCount <= expectedCall.getCount() ) ) )
//     {
//       // FIXME why is the test result protected?!
//       // UtestShell::getCurrent()->getTestResult()->print( expectedCall.asCharString() );
//       // Fake access to test result
//       // UtestShell::getCurrent()->print( expectedCall.toString().asCharString(), "", 0 );
//       passed = false;
//     }

//     ExpectedCallEntry* pNextExpectedEntry = pExpectedCallEntry->pNext;
//     delete pExpectedCallEntry->pExpectedCall;
//     delete pExpectedCallEntry;
//     _expectedCalls = pNextExpectedEntry;
//   }

//   return passed;
// }

} // namespace TestDouble


