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

#ifndef ACTUAL_H
#define ACTUAL_H

#include "CppUTestExt/TestDouble.h"

/**
 * actual( {"<context>"} )             // expectation context, default is global
 * .call( "<method>" )                 // name of method/function
 *  {.with( "parameter", value )}      // parameter name and value upon invocation
 * {.output( "parameter", value& )}    // name of parameter and reference to value to set (value based on expectation or else 0 )
 * {.return<type>()}                   // return value
 */
class Actual;
Actual actual( const SimpleString& context = TEST_DOUBLE_GLOBAL_CONTEXT );

class ActualCall;
class Actual
{
public:
    Actual( const SimpleString& context ) : _context(context) {}

    /// @note an Actual can only have one call invocation
    ActualCall call( const SimpleString& name );

private:
  const SimpleString& _context;
};  // class Actual


class ActualCall
{
public:
  ActualCall( const SimpleString& context, const SimpleString& methodName );

  /// assert the actual
  ~ActualCall() { actual( *this ); }

  template<typename T>
  ActualCall& with( const SimpleString& name, const T& value )
  {
    _actualCall.withParameter( name, value );
    return *this;
  }

  template<typename T>
  ActualCall& with( const SimpleString& name, const T* value, std::size_t size )
  {
    _actualCall.withMemoryBufferParameter( name, reinterpret_cast<const unsigned char*>(value), size );
    return *this;
  }

  template<typename T>
  ActualCall& output( const SimpleString& name, T* const value )
  {
    _actualCall.withOutputParameter( name, value );
    return *this;
  }

  //  return based methods invoke matched expectation (or else do nothing and return 0)
  // TODO template returns
  bool returnBool();
  char returnChar();
  unsigned char returnUnsignedChar();
  int returnInt();
  unsigned int returnUnsignedInt();
  long int returnLongInt();
  unsigned long int returnUnsignedLongInt();
  long long int returnLongLongInt();
  unsigned long long int returnUnsignedLongLongInt();
  float returnFloat();
  double returnDouble();
  void* returnPointer();
  const void* returnConstPointer();
  void(*returnFunctionPointer())();

private:
  SimpleString  _context;
  SimpleString  _methodName;
  MockActualCall& _actualCall;

  template<typename T>
  void _testParameter( const SimpleString& name, const T& value )
  {
    Parameter parameter( name, value );
  }
};  // class ActualCall


class PossibleExpectations
{

};  // class PossibleExpectations

#endif /* ACTUAL_H */
