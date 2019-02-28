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

#include <typeinfo>
#include <CppUTest/SimpleString.h>

#include <CppUTest/PlatformSpecificFunctions.h>

// CppUTest replaces string.h API but not memcmp
extern "C" int memcmp ( const void * ptr1, const void * ptr2, size_t num );

namespace TestDouble {

class Parameter
{
public:
  const SimpleString  name;
  const SimpleString  type;
  // TODO should buffer require a static reference or should this clone?
  const void* buffer = 0;
  void* const outputBuffer = 0;
  const std::size_t bufferSize_bytes = 0;

  /// an input parameter
  template<typename T>
  Parameter( const SimpleString &_name, const T &value )
  : name(_name), type( typeid(value).name() ), _variant(value)
  { }

  /// an input buffer
  Parameter( const SimpleString &_name, void* const &_buffer, const std::size_t &_bufferSize_bytes )
  : name(_name), type( typeid(_buffer).name() ), buffer(_buffer), bufferSize_bytes(_bufferSize_bytes)
  { }

  /// an output parameter (FIXME the unused bool parameter is a hack, should be a class OutputParameter with base Parameter)
  template<typename T>
  Parameter( const SimpleString &_name, T* const &_buffer, T defaultValue, const bool )
  : name(_name), type( typeid(T).name() ), outputBuffer(_buffer), bufferSize_bytes(sizeof(T)), _variant(defaultValue)
  { }

  Parameter( const SimpleString &_name, void* const &_buffer, const std::size_t &_bufferSize_bytes, const void* const defaultValue )
  : name(_name), type( typeid(_buffer).name() ), outputBuffer(_buffer), bufferSize_bytes(_bufferSize_bytes), _variant(defaultValue)
  { }


  bool equals( const Parameter* const &pOther ) const;

  /// used by ActualCall to set output based on expectation parameter
  void setValue( const Parameter* const &pOther )
  {
    PlatformSpecificMemCpy( outputBuffer, &(pOther->_variant), bufferSize_bytes );
  }

  /// used by ActualCall to set output to provided default (or true-ish)
  void setDefault()
  {
    PlatformSpecificMemCpy( outputBuffer, &_variant, bufferSize_bytes );
  }
  
  

private:
  const union Variant {
    bool                    asBool;
    char                    asChar;
    unsigned char           asUnsignedChar;
    int                     asInt;
    unsigned int            asUnsignedInt;
    long int                asLong;
    unsigned long int       asUnsignedLong;
    long long int           asLongLong;
    unsigned long long int  asUnsignedLongLong;
    float                   asFloat;
    double                  asDouble;
    void*                   asPointer;
    const void*             asConstPointer;
    void(*asFunctionPointer)();

    // all types will degrade to bool so no constructor necessary for bool
    // provide pointer support first (so references won't degrade to primitives)
    Variant( void(*value)() ) : asFunctionPointer(value) {}
    Variant( void* value ) : asPointer(value) {}
    Variant( const void* value ) : asConstPointer(value) {}
    // provide primitive support
    Variant( const char& value ) : asChar(value) {}
    Variant( const unsigned char& value ) : asUnsignedChar(value) {}
    Variant( const int& value ) : asInt(value) {}
    Variant( const unsigned int& value ) : asUnsignedInt(value) {}
    Variant( const long& value ) : asLong(value) {}
    Variant( const unsigned long& value ) : asUnsignedLong(value) {}
    Variant( const long long& value ) : asLongLong(value) {}
    Variant( const unsigned long long& value ) : asUnsignedLongLong(value) {}
    Variant( const float& value ) : asFloat(value) {}
    Variant( const double& value ) : asDouble(value) {}

  } _variant = 0;

};  // class Parameter

} // namespace TestDouble

#endif // TEST_DOUBLE_PARAMETER_H
