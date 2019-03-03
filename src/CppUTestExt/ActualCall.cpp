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

#include "CppUTestExt/ActualCall.h"

#include "CppUTestExt/ExpectCall.h"
#include "CppUTestExt/TestDouble.h"
#include "CppUTest/Utest.h"

const ExpectedCall* ActualCall::_setOutputs()
{
  _hasSetOutputs = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( ( TestDouble::shouldFailUnexpected() )  &&  ( 0 == pExpectation ) ) _failureMessage = "unexpected call";
  else for( const TestDouble::ParameterChain* pActualEntry=getOutputs(); 0 != pActualEntry; pActualEntry = pActualEntry->pNext )
  {
    bool used = false;
    // set outputs
    if( 0 != pExpectation ) 
    {
      for( const TestDouble::ParameterChain* pExpectedEntry=pExpectation->getOutputs(); 0 != pExpectedEntry; pExpectedEntry = pExpectedEntry->pNext )
      {
        if( pExpectedEntry->pParameter->name == pActualEntry->pParameter->name )
        {
          if( false == pActualEntry->pParameter->setValue( pExpectedEntry->pParameter ) )
          {
            _failureMessage = StringFromFormat( "mismatch output size for parameter '%s', expected size: %zu  actual size: %zu",
                                           pExpectedEntry->pParameter->name.asCharString(),
                                           pExpectedEntry->pParameter->bufferSize_bytes,
                                           pActualEntry->pParameter->bufferSize_bytes );
          }
          used = true;
          break;  ///< only use the first expectation of a parameter
        }
      }
    }
    if( false == used ) pActualEntry->pParameter->setDefault();
  }

  return pExpectation;
}


double ActualCall::returnDouble( double defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asDouble;
}

float ActualCall::returnFloat( float defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asFloat;
}

long long ActualCall::returnLongLong( long long defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asLongLong;
}

unsigned long long ActualCall::returnUnsignedLongLong( unsigned long long defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asUnsignedLongLong;
}

long ActualCall::returnLong( long defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asLong;
}

unsigned long ActualCall::returnUnsignedLong( unsigned long defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asUnsignedLong;
}

int ActualCall::returnInt( int defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asInt;
}

unsigned int ActualCall::returnUnsignedInt( unsigned int defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asUnsignedInt;
}

short ActualCall::returnShort( short defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asShort;
}

unsigned short ActualCall::returnUnsignedShort( unsigned short defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asUnsignedShort;
}

char ActualCall::returnChar( char defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asChar;
}

unsigned char ActualCall::returnUnsignedChar( unsigned char defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asUnsignedChar;
}

bool ActualCall::returnBool( bool defaultValue )
{
  const ExpectedCall* pExpectation = _setOutputs();

  if( 0 == pExpectation ) return defaultValue;
  else return pExpectation->getReturn().value.asBool;
}


ActualCall::~ActualCall()
{
  if( false == _hasSetOutputs ) _setOutputs();

  if( false == _failureMessage.isEmpty() )
  {
    _failureMessage += "\n";
    _failureMessage += StringFromFormat( "\tactual call %s(\n", name.asCharString() );
    _failureMessage += "\tINPUTS:\n";
    for( const TestDouble::ParameterChain* pEntry = getParameters(); 0 != pEntry; pEntry = pEntry->pNext )
    {
      _failureMessage += "\t\t";
      _failureMessage += pEntry->pParameter->toString();
      _failureMessage += "\n";
    }
    _failureMessage += "\tOUTPUTS:\n";
    for( const TestDouble::ParameterChain* pEntry = getOutputs(); 0 != pEntry; pEntry = pEntry->pNext )
    {
      _failureMessage += "\t\t";
      _failureMessage += pEntry->pParameter->toString();
      _failureMessage += "\n";
    }
    _failureMessage += ")\n";
  }

  delete _parameters;
  delete _outputs;

  if( false == _failureMessage.isEmpty() )
  {
    UtestShell* const pShell = UtestShell::getCurrent();
    TestFailure failure( pShell, pShell->getFile().asCharString(), pShell->getLineNumber(), _failureMessage );
    TestTerminatorWithoutExceptions terminator;
    pShell->failWith( failure, terminator );
  }
}
