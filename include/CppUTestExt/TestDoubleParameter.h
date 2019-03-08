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

#ifndef TEST_DOUBLE_PARAMETER_H
#define TEST_DOUBLE_PARAMETER_H

#include <CppUTest/SimpleString.h>
#include <typeinfo>

namespace TestDouble {

class Parameter
{
public:
  const SimpleString  name;
  const SimpleString  type;
  const void* buffer = 0;           ///< reference to a test's static buffer (i.e. the buffer must be referencable throughout the test)
  void* const outputBuffer = 0;     ///< reference to a actual's static buffer (i.e. the buffer must be referencable by the test double)
  const std::size_t bufferSize_bytes = 0;

  /// an input parameter
  template<typename T>
  Parameter( const SimpleString &_name, const T &value )
  : name(_name), type(typeid(value).name()), _variant(value) {}

  /// an input buffer
  Parameter( const SimpleString &_name, const void* const &_buffer, const std::size_t &_bufferSize_bytes )
  : name(_name), type(typeid(void*).name()), buffer(_buffer), bufferSize_bytes(_bufferSize_bytes) {}

  /// an output parameter (the unused bool parameter is used to workaround overriding input buffer constructor)
  template<typename T>
  Parameter( const SimpleString &_name, T* const &_buffer, T defaultValue, const bool )
  : name(_name), type(typeid(T).name()), outputBuffer(_buffer), bufferSize_bytes(sizeof(T)), _variant(defaultValue) {}

  /// an output buffer parameter
  Parameter( const SimpleString &_name, void* const &_buffer, const std::size_t &_bufferSize_bytes, const void* const defaultValue )
  : name(_name), type(typeid(_buffer).name()), buffer(defaultValue), outputBuffer(_buffer), bufferSize_bytes(_bufferSize_bytes) {}


  bool equals( const Parameter* const &pOther ) const;

  /// used by ActualCall to set output based on expectation parameter
  bool setValue( const Parameter* const &pOther );

  /// used by ActualCall to set output to provided default (or true-ish by default)
  void setDefault();
  
  SimpleString toString();

  /// a generic data object used as an Input, Output, and ReturnValue
  struct Variant
  {
    const enum Type
    {
        FN_POINTER,
        POINTER, CONST_POINTER,
        DOUBLE,
        FLOAT,
        LONG_LONG, UNSIGNED_LONG_LONG,
        LONG, UNSIGNED_LONG,
        INT, UNSIGNED_INT,
        SHORT, UNSIGNED_SHORT,
        CHAR, UNSIGNED_CHAR,
        BOOL,
        RETURN_VALUE
    } type;

    /// copies value (does not change original type)
    Variant& operator=( const TestDouble::Parameter::Variant &other );

    union Value
    {
      void(*asFunctionPointer)();
      void*                   asPointer;
      const void*             asConstPointer;
      double                  asDouble;
      float                   asFloat;
      long long int           asLongLong;
      unsigned long long int  asUnsignedLongLong;
      long int                asLong;
      unsigned long int       asUnsignedLong;
      int                     asInt;
      unsigned int            asUnsignedInt;
      short                   asShort;
      unsigned short          asUnsignedShort;
      char                    asChar;
      unsigned char           asUnsignedChar;
      bool                    asBool;

      Value( void(*_value)() ) : asFunctionPointer(_value) {}
      Value( void* const &_value ) : asPointer(_value) {}
      Value( const void* const &_value ) : asConstPointer(_value) {}
      Value( const double &_value ) : asDouble(_value) {}
      Value( const float &_value ) : asFloat(_value) {}
      Value( const long long &_value ) : asLongLong(_value) {}
      Value( const unsigned long long &_value ) : asUnsignedLongLong(_value) {}
      Value( const long &_value ) : asLong(_value) {}
      Value( const unsigned long &_value ) : asUnsignedLong(_value) {}
      Value( const int &_value ) : asInt(_value) {}
      Value( const unsigned int &_value ) : asUnsignedInt(_value) {}
      Value( const short &_value ) : asShort(_value) {}
      Value( const unsigned short &_value ) : asUnsignedShort(_value) {}
      Value( const char &_value ) : asChar(_value) {}
      Value( const unsigned char &_value ) : asUnsignedChar(_value) {}
      Value( const bool &_value ) : asBool(_value) {}
      Value() {}    ///< a default constructor for a future return value

    } value;

    // provide pointer support first (so references won't degrade to primitives)
    Variant( void(*_value)() ) : type(FN_POINTER), value(_value) {}
    Variant( const void* &_value ) : type(POINTER), value(_value) {}
    Variant( const void* const &_value ) : type(CONST_POINTER), value(_value) {}
    Variant( const double &_value ) : type(DOUBLE), value(_value) {}
    Variant( const float &_value ) : type(FLOAT), value(_value) {}
    Variant( const long long &_value ) : type(LONG_LONG), value(_value) {}
    Variant( const unsigned long long &_value ) : type(UNSIGNED_LONG_LONG), value(_value) {}
    Variant( const long &_value ) : type(LONG), value(_value) {}
    Variant( const unsigned long &_value ) : type(UNSIGNED_LONG), value(_value) {}
    Variant( const int &_value ) : type(INT), value(_value) {}
    Variant( const unsigned int &_value ) : type(UNSIGNED_INT), value(_value) {}
    Variant( const short &_value ) : type(SHORT), value(_value) {}
    Variant( const unsigned short &_value ) : type(UNSIGNED_SHORT), value(_value) {}
    Variant( const char &_value ) : type(CHAR), value(_value) {}
    Variant( const unsigned char &_value ) : type(UNSIGNED_CHAR), value(_value) {}
    Variant( const bool &_value ) : type(BOOL), value(_value) {}

    /// constructor for return values (C/C++ return values can be implicitly converted,
    ///    so explicit type cannot be enforced by Variant)
    Variant() : type(RETURN_VALUE) {}

  } _variant = 0;

};  // class Parameter


// generic list of parameters (used for both input and output)
struct ParameterChain
{
  TestDouble::Parameter* const pParameter;
  ParameterChain* const pNext;

  /// prepends new paramater to the chain
  ParameterChain( TestDouble::Parameter* const &_pParameter, ParameterChain* const &_pNext )
  : pParameter(_pParameter) ,pNext(_pNext) {}
  ~ParameterChain() { delete pParameter; delete pNext; }
};

} // namespace TestDouble

#endif // TEST_DOUBLE_PARAMETER_H
