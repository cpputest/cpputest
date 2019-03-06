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

#ifndef ACTUAL_CALL_H
#define ACTUAL_CALL_H

#include <CppUTest/SimpleString.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/TestDoubleParameter.h>
#include <CppUTestExt/ExpectCall.h>
#include <typeinfo>

namespace TestDouble { class ActualCall; }
/// create an actual call object using a [Builder pattern](https://en.wikipedia.org/wiki/Builder_pattern)
/// @note implemented in TestDouble.cpp
TestDouble::ActualCall actualCall( const SimpleString &name );

namespace TestDouble 
{
class ActualCall : public AActualCall
{
public:
  const SimpleString      name;

  ActualCall( const SimpleString &_name ) : name(_name) { }

  /// match this against expectations if not already returned
  ~ActualCall();

  template<typename T>
  ActualCall& with( const SimpleString &_name, const T &value )
  {
    TestDouble::Parameter* pParameter = new TestDouble::Parameter( _name, value );
    _inputs = new TestDouble::ParameterChain( pParameter, _inputs );
    return *this;
  }

  ActualCall& withBuffer( const SimpleString &_name, void* const &staticBuffer, const std::size_t &size )
  {
    TestDouble::Parameter* pParameter = new TestDouble::Parameter( _name, staticBuffer, size );
    _inputs = new TestDouble::ParameterChain( pParameter, _inputs );
    return *this;
  }

  template<typename T>
  ActualCall& output( const SimpleString &_name, T* const &pValue, const T defaultValue=T(true) )
  {
    TestDouble::Parameter* pParameter = new TestDouble::Parameter( _name, pValue, defaultValue, true );
    _outputs = new TestDouble::ParameterChain( pParameter, _outputs );
    return *this;
  }

  ActualCall& outputBuffer( const SimpleString &_name, void* const &staticBuffer, const std::size_t &size_bytes, const void* const pDefault=0 )
  {
    TestDouble::Parameter* pParameter = new TestDouble::Parameter( _name, staticBuffer, size_bytes, pDefault );
    _outputs = new TestDouble::ParameterChain( pParameter, _outputs );
    return *this;
  }


  /// used for testing so that actual sets outputs immediately (prior to destructor)
  void returns() { _setOutputs(); }

  template<typename T>
  T* returnPointer( T* const defaultValue=0 )
  {
    const ExpectedCall* pExpectation = _setOutputs();

    if( 0 == pExpectation ) return defaultValue;
    if( pExpectation->hasModel() ) return static_cast<T*>(_return.value.asPointer);
    return static_cast<T*>(pExpectation->getReturn().value.asPointer);
  }
  template<typename T>
  const T* returnConstPointer( const T* const defaultValue=0 )
  {
    const ExpectedCall* pExpectation = _setOutputs();

    if( 0 == pExpectation ) return defaultValue;
    if( pExpectation->hasModel() ) return static_cast<const T*>(_return.value.asConstPointer);
    return static_cast<const T*>(pExpectation->getReturn().value.asConstPointer);
  }
  double returnDouble( double defaultValue=true );
  float returnFloat( float defaultValue=true );
  long long returnLongLong( long long defaultValue=true );
  unsigned long long returnUnsignedLongLong( unsigned long long defaultValue=true );
  long returnLong( long defaultValue=true );
  unsigned long returnUnsignedLong( unsigned long defaultValue=true );
  int returnInt( int defaultValue=true );
  unsigned int returnUnsignedInt( unsigned int defaultValue=true );
  short returnShort( short defaultValue=true );
  unsigned short returnUnsignedShort( unsigned short defaultValue=true );
  char returnChar( char defaultValue=true );
  unsigned char returnUnsignedChar( unsigned char defaultValue=true );
  bool returnBool( bool defaultValue=true );

  virtual const TestDouble::ParameterChain* getInputs() const { return _inputs; }
  virtual TestDouble::ParameterChain* getOutputs() const { return _outputs; }

private:
  TestDouble::ParameterChain*     _inputs = 0;
  TestDouble::ParameterChain*     _outputs = 0;

  bool  _hasSetOutputs = false;
  /// sets outputs and handles expectation model (which may change return value)
  /// returns   found expectation or 0
  const ExpectedCall* _setOutputs();

  SimpleString _failureMessage;

};  // class ActualCall
} // namespace TestDouble

#endif /* ACTUAL_CALL_H */
