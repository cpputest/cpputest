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

// FIXME uses CppUMock for now
#include <CppUTestExt/MockSupport.h>

#include "CppUTestExt/Expect.h"
#include "CppUTestExt/ActualCall.h"

static bool _failActuals = false;
void failUponUnexpected( bool mode )
{
  _failActuals = mode;
}

class Expectations
{
public:
  ExpectedCall add( const SimpleString& call );
  ExpectedCall add( const SimpleString& sequence, const SimpleString& call );
  void check( const ActualCall& call );
  void check();
};
static Expectations expectations;

void checkExpectations()
{
  expectations.check();
}

ExpectedCall expectCall( const SimpleString& call ) { return expectations.add( call ); }

ExpectedCall expectNext( const SimpleString& sequenceName, const SimpleString& call )
{
  // FIXME
}

ActualCall actualCall( const SimpleString& call )
{
  return ActualCall( call );
}

void verifyActual( const ActualCall& call )
{
  expectations.check( call );
}



// Implementation of Expectation framework

struct ExpectedCallEntry
{
  const ExpectedCall* const pExpectedCall;
  int                       calledCount;
  ExpectedCallEntry* const  pNext;
};


static ExpectedCallEntry* expectedCalls = 0;
void _add( const ExpectedCall* const pExpectation )
{
  expectedCalls = new ExpectedCallEntry{ pExpectation, 0, expectedCalls };
}

ExpectedCall Expectations::add( const SimpleString& call )
{
  //FIXME
  ExpectedCall* pExpected = new ExpectedCall( call );
  _add( pExpected );
  return *pExpected;
}
ExpectedCall Expectations::add( const SimpleString& sequence, const SimpleString& call )
{
  // FIXMe
  ExpectedCall* pExpected = new ExpectedCall( call );
  return *pExpected;
}

void Expectations::check( const ActualCall& call )
{
  // FIXME
}
void Expectations::check()
{
  // FIXME

  // clean up expectations
  while( expectedCalls != 0 )
  {
    ExpectedCallEntry* pNext = expectedCalls->pNext;
    delete expectedCalls->pExpectedCall;
    delete expectedCalls;
    expectedCalls = pNext;
  }
}

// class Expectations
// {
//   ExpectedCall _addExpectation( const SimpleString& call )
//   {
//     ExpectedCallEntry* pExpectedCallEntry = new ExpectedCallEntry();
//     ExpectedCall* pExpectedCall = new ExpectedCall( call );

//     pExpectedCallEntry->self = pExpectedCall;
//     pExpectedCallEntry->callCount = 0;
//     pExpectedCallEntry->pNext = ExpectedCallsHead;
//     ExpectedCallsHead = pExpectedCallEntry;

//     return *pExpectedCall;
//   }
// }

