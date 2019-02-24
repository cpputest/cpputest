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


ActualCall::ActualCall( const SimpleString& name )
  : methodName( name )
   ,_parameters(0)
  //  ,_pOutputParameter(0)
{ }

ActualCall::~ActualCall()
{
  /// find an expectation
  const ExpectedCall* pExpectation = TestDouble::findExpectation( *this );

  if( TestDouble::shouldFailUnexpected() && ( 0 == pExpectation ) )
  {
    // print actual (input and output parameters)
    SimpleString msg = StringFromFormat("no expectation matching actual call: \n\t %s \n", methodName.asCharString() );
    for( const TestDouble::ParameterChain* pEntry = getParameters(); 0 != pEntry; pEntry = pEntry->pNext )
    {
      const TestDouble::Parameter* const pParameter = pEntry->pParameter;
      msg += StringFromFormat( "\t\t %s = %s \n", pParameter->name.asCharString(), pParameter->type.asCharString() );
    }
    msg += ")\n";
    UtestShell& shell = *(UtestShell::getCurrent());
    // failure never returns so clean up memory
    _deconstructor();
    shell.fail( msg.asCharString(), shell.getFile().asCharString(), shell.getLineNumber() );

    // UT_PRINT( StringFromFormat( "Type Mismatch: Expected call to '%s' with parameter '%s' of type '%s', but actual paramter was of type '%s'.",
    //   expected.methodName.asCharString(), pExpectedEntry->pParameter->name.asCharString(),
    //   pExpectedEntry->pParameter->type.asCharString(), pActualEntry->pParameter->type.asCharString() ).asCharString()
    // );

  }
  else _deconstructor();
}

void ActualCall::_deconstructor()
{
  delete _parameters;
  // delete _pOutputParameter;
}

ActualCall& ActualCall::with( const SimpleString& name, const void* const buffer, const std::size_t& size )
{
  TestDouble::Parameter* pParameter = new TestDouble::Parameter( name, buffer, size );
  _parameters = new TestDouble::ParameterChain( pParameter, _parameters );
  return *this;
}
