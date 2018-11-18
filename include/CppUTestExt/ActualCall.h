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

#include "CppUTestExt/TestDouble.h"

#include "CppUTestExt/TestDoubleParameter.h"

class ActualCall;
ActualCall actualCall( const SimpleString& call );

class ActualCall
{
public:
  ActualCall( const SimpleString& call );

  ~ActualCall()
  {
    /// assert the actual
    verifyActual( *this );

    if( 0 != _pOutputParameter) delete _pOutputParameter;
    while( 0 != _pParameterHead )
    {
      ParameterEntry* nextHead = _pParameterHead->pNext;
      delete _pParameterHead->pParameter;
      delete _pParameterHead;
      _pParameterHead = nextHead;
    }
  }

  template<typename T>
  ActualCall& with( const SimpleString& name, const T& value )
  {
    _addParameter( name, value );
    return *this;
  }

  template<typename T>
  ActualCall& with( const SimpleString& name, const T* value, std::size_t size )
  {
    _addParameter( name, value, size );
    return *this;
  }

  template<typename T>
  ActualCall& output( const SimpleString& name, T* const value )
  {
    // TODO should overriding output be allowed?
    if( 0 != _pOutputParameter ) delete _pOutputParameter;
    _pOutputParameter = new Parameter( name, value );
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
  const SimpleString  _methodName;
  Parameter*    _pOutputParameter = 0;

  struct ParameterEntry
  {
    const Parameter* const pParameter;
    ParameterEntry* const pNext;
  };
  ParameterEntry*_pParameterHead = 0;

  template<typename T>
  void _addParameter( const SimpleString& name, const T& value )
  {
    Parameter* pParameter = new Parameter( name, value );
    _pParameterHead = new ParameterEntry{ pParameter, _pParameterHead };
  }

  template<typename T>
  void _addParameter( const SimpleString& name, const T& value, std::size_t size )
  {
    // FIXME support buffer return
  }

};  // class ActualCall

#endif /* ACTUAL_CALL_H */
