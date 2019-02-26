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

#ifndef EXPECT_CALL_H
#define EXPECT_CALL_H

#include <CppUTest/SimpleString.h>
#include "TestDouble.h"

class ExpectedCall;
/// implemented in TestDouble.cpp
ExpectedCall& expectCall( const SimpleString& call );            ///< add expectation to registry

/// A builder for an expectation
class ExpectedCall
{
public:
  const SimpleString  name;

  ExpectedCall( const SimpleString& name );
  ~ExpectedCall();

  static const int EXPECT_ALWAYS = -1;
  ExpectedCall& times( const unsigned int count );

  template<typename T>
  ExpectedCall& with( const SimpleString& name, const T& value )
  {
    TestDouble::Parameter* pParameter = new TestDouble::Parameter( name, value );
    _parameters = new TestDouble::ParameterChain( pParameter, _parameters );
    return *this;
  }

  ExpectedCall& with( const SimpleString& name, const void* staticBuffer, const std::size_t& size );

  template<typename T>
  ExpectedCall& output( const SimpleString& name, const T& value )
  {
    TestDouble::Parameter* pParameter = new TestDouble::Parameter( name, value );
    _outputs = new TestDouble::ParameterChain( pParameter, _outputs );
    return *this;
  }

  const int& getCount() const { return _count; }
  const TestDouble::ParameterChain* getParameters() const { return _parameters; }
  const TestDouble::ParameterChain* getOutputs() const { return _outputs; }

private:
  int   _count;
  TestDouble::ParameterChain*   _parameters;
  TestDouble::ParameterChain*   _outputs;
};

// class Actual;
// /// Behavior handler for an expectation (e.g. Aspect Oriented Programming AoP)
// class IModel
// {
//     /// modifies the actual call instance based on expectation and model state
//     virtual void behave( const Expectation&, Actual& ) = 0;
// };

#endif /* EXPECT_CALL_H */
