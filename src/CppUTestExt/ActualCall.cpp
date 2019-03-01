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
#include "CppUTest/Utest.h"

ActualCall::~ActualCall()
{
  if( false == _returned )
  {
    /// find an expectation
    const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
    if( ( TestDouble::shouldFailUnexpected() )  &&  ( 0 == pExpectation ) )
    {
      _failActual();
    }
  }

  delete _parameters;
  delete _outputs;
}

void ActualCall::returns()
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( ( TestDouble::shouldFailUnexpected() )  &&  ( 0 == pExpectation ) )
  {
      _failActual();
  }
  else _setOutputs( pExpectation );
}
  
void ActualCall::_setOutputs( const ExpectedCall* &pExpectation )
{
  for( const TestDouble::ParameterChain* pActualEntry=getOutputs(); 0 != pActualEntry; pActualEntry = pActualEntry->pNext )
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
              // TODO format a usable message
              SimpleString failureMessage = "unmet actual";
              UtestShell* const pShell = UtestShell::getCurrent();
              TestFailure failure( pShell, pShell->getFile().asCharString(), pShell->getLineNumber(), failureMessage );
              pShell->failWith( failure );
          }
          used = true;
          break;  ///< only use the first expectation of a parameter
        }
      }
    }
    if( false == used ) pActualEntry->pParameter->setDefault();
  }
}

bool ActualCall::returnBool( bool defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asBool;
}

char ActualCall::returnChar( char defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asChar;
}

unsigned char ActualCall::returnUnsignedChar( unsigned char defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asUnsignedChar;
}

int ActualCall::returnInt( int defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asInt;
}

unsigned int ActualCall::returnUnsignedInt( unsigned int defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;


  _setOutputs( pExpectation );
  return pExpectation->getReturn().asUnsignedInt;
}

long ActualCall::returnLong( long defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asLong;
}

unsigned long ActualCall::returnUnsignedLong( unsigned long defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asUnsignedLong;
}

long long ActualCall::returnLongLong( long long defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asLongLong;
}

unsigned long long ActualCall::returnUnsignedLongLong( unsigned long long defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asUnsignedLongLong;
}

float ActualCall::returnFloat( float defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asFloat;
}

double ActualCall::returnDouble( double defaultValue )
{
  _returned = true;

  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );
  if( 0 == pExpectation ) return defaultValue;

  _setOutputs( pExpectation );
  return pExpectation->getReturn().asDouble;
}




void ActualCall::_failActual()
{
  // TODO format a usable message
  SimpleString failureMessage = "unmet actual";
  UtestShell* const pShell = UtestShell::getCurrent();
  TestFailure failure( pShell, pShell->getFile().asCharString(), pShell->getLineNumber(), failureMessage );
  TestTerminatorWithoutExceptions terminator;
  pShell->failWith( failure, terminator );
}

// ActualCall& ActualCall::with( const SimpleString& _name, const void* const buffer, const std::size_t& size )
// {
//   TestDouble::Parameter* pParameter = new TestDouble::Parameter( _name, buffer, size );
//   _parameters = new TestDouble::ParameterChain( pParameter, _parameters );
//   return *this;
// }

    // // print actual (input and output parameters)
    // SimpleString msg = StringFromFormat("no expectation matching actual call: \n\t%s(\n", methodName.asCharString() );
    // for( const TestDouble::ParameterChain* pEntry = getParameters(); 0 != pEntry; pEntry = pEntry->pNext )
    // {
    //   const TestDouble::Parameter* const pParameter = pEntry->pParameter;
    //   msg += StringFromFormat( "\t\t %s = %s \n", pParameter->name.asCharString(), pParameter->type.asCharString() );
    // }
    // msg += "\t)\n";
    // UtestShell& shell = *(UtestShell::getCurrent());
    // // FIXME  does failure never return?
    // // shell.fail( msg.asCharString(), shell.getFile().asCharString(), shell.getLineNumber() );

    // // UT_PRINT( StringFromFormat( "Type Mismatch: Expected call to '%s' with parameter '%s' of type '%s', but actual paramter was of type '%s'.",
    // //   expected.methodName.asCharString(), pExpectedEntry->pParameter->name.asCharString(),
    // //   pExpectedEntry->pParameter->type.asCharString(), pActualEntry->pParameter->type.asCharString() ).asCharString()
    // // );
