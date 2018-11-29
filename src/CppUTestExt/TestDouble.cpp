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

#include "CppUTestExt/Expect.h"
#include "CppUTestExt/ActualCall.h"

#include <CppUTest/TestHarness.h>

static bool _failActuals = false;
void failUnexpected( bool mode ) { _failActuals = mode; }


class Expectations
{
public:
  ExpectedCall& add( const SimpleString& call );
  void check( const ActualCall& call );
  void check();
};
static Expectations expectations;
void checkExpectations() { expectations.check(); }

ExpectedCall& expectCall( const SimpleString& call ) { return expectations.add( call ); }
ActualCall actualCall( const SimpleString& call ) { return ActualCall( call ); }
void verifyActual( const ActualCall& call ) { expectations.check( call ); }


// Implementation of Expectation framework
struct ExpectedCallEntry
{
  const ExpectedCall* const pExpectedCall;
  unsigned int              calledCount;
  ExpectedCallEntry* const  pNext;
};
static ExpectedCallEntry* _expectedCalls = 0;


ExpectedCall& Expectations::add( const SimpleString& call )
{
  ExpectedCall* pExpected = new ExpectedCall( call );
  // prepend expected chain
  _expectedCalls = new ExpectedCallEntry{ pExpected, 0, _expectedCalls };
  return *pExpected;
}

bool matches( const ActualCall& actual, const ExpectedCall& expected );
void Expectations::check( const ActualCall& call )
{
  // TODO check sequence expectations first

  bool verified = false;
  for( ExpectedCallEntry* pExpectedEntry=_expectedCalls; pExpectedEntry != 0; pExpectedEntry=pExpectedEntry->pNext )
  {
    const ExpectedCall& expectedCall = *(pExpectedEntry->pExpectedCall);
    // skip fulfilled expectations
    if( ( expectedCall.count != ExpectedCall::EXPECT_ALWAYS ) && ( pExpectedEntry->calledCount >= expectedCall.count ) ) continue;

    if( matches( call, expectedCall ) )
    {
      pExpectedEntry->calledCount++;
      verified = true;
      break;
    }
  }

  if( ( false == verified ) && ( true == _failActuals ) )
  {
    // TODO failActual()
    UT_PRINT( "Actual call had no matching expectation." );
  }

  // TODO set output parameters
}

void Expectations::check()
{
  // clean up expectations
  while( _expectedCalls != 0 )
  {
    ExpectedCallEntry* pNext = _expectedCalls->pNext;

    // TODO fail upon unfulfilled expectation
    // if( ( expectedCall.count != ExpectedCall::EXPECT_ALWAYS ) && ( pExpectedEntry->calledCount >= expectedCall.count ) ) continue;

    delete _expectedCalls->pExpectedCall;
    delete _expectedCalls;
    _expectedCalls = pNext;
  }
}

bool matches( const ActualCall& actual, const ExpectedCall& expected )
{
  if( actual.methodName != expected.methodName ) return false;

  for( const ParameterEntry* pExpectedEntry=expected.getParameters(); pExpectedEntry != 0; pExpectedEntry=pExpectedEntry->pNext )
  {
    for( const ParameterEntry* pActualEntry=actual.getParameters(); pActualEntry != 0; pActualEntry=pActualEntry->pNext )
    {
      if( pExpectedEntry->pParameter->name.equalsNoCase( pActualEntry->pParameter->name ) )
      {
        if( pExpectedEntry->pParameter->type != pActualEntry->pParameter->type )
        {
          FAIL(
            StringFromFormat("Type Mismatch: Expected call to '%s' with parameter '%s' of type '%s', but actual paramter was of type '%s'.",
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
