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
  bool check( const ActualCall& pActualCall );
  bool check();
};
static Expectations expectations;
void checkExpectations()
{
  if( false == expectations.check() )
  {
    FAIL( "Unmet expectations" );
  }
}

ExpectedCall& expectCall( const SimpleString& call ) { return expectations.add( call ); }
ActualCall actualCall( const SimpleString& call ) { return ActualCall( call ); }
bool verifyActual( const ActualCall& call ) { expectations.check( call ); }


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

#include <stdio.h>
bool _matches( const ActualCall& pCall, const ExpectedCall& expected );
bool Expectations::check( const ActualCall& call )
{
  // TODO check sequence expectations first

  if( ( 0 == _expectedCalls ) && _failActuals ) return false;

  for( ExpectedCallEntry* pExpectedEntry =_expectedCalls; pExpectedEntry != 0; pExpectedEntry=pExpectedEntry->pNext )
  {
    const ExpectedCall& expectedCall = *(pExpectedEntry->pExpectedCall);
    // skip fulfilled expectations
    if( ( expectedCall.count != ExpectedCall::EXPECT_ALWAYS ) && ( pExpectedEntry->calledCount >= expectedCall.count ) ) continue;

    if( _matches( call, expectedCall ) ) pExpectedEntry->calledCount++;
    else
    {
      check();
      return false;
    }
  }
}

bool Expectations::check()
{
  bool passed = true;

  // clean up expectations
  while( _expectedCalls != 0 )
  {
    const ExpectedCallEntry* pExpectedCallEntry = _expectedCalls;
    const ExpectedCall& expectedCall = *(pExpectedCallEntry->pExpectedCall);
    if( ( ( expectedCall.count == ExpectedCall::EXPECT_ALWAYS ) && ( pExpectedCallEntry->calledCount == 0 ) )   ||
        ( ( expectedCall.count != ExpectedCall::EXPECT_ALWAYS ) && ( pExpectedCallEntry->calledCount <= expectedCall.count ) ) )
    {
      // FIXME why is the test result protected?!
      // UtestShell::getCurrent()->getTestResult()->print( expectedCall.asCharString() );
      // Fake access to test result
      UtestShell::getCurrent()->print( expectedCall.toString().asCharString(), "", 0 );
      passed = false;
    }

    ExpectedCallEntry* pNextExpectedEntry = pExpectedCallEntry->pNext;
    delete pExpectedCallEntry->pExpectedCall;
    delete pExpectedCallEntry;
    _expectedCalls = pNextExpectedEntry;
  }

  return passed;
}

bool _matches( const ActualCall& actual, const ExpectedCall& expected )
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
