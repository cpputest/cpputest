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
#ifndef TEST_DOUBLE_H
#define TEST_DOUBLE_H

#include "CppUTestExt/MockSupport.h"

#include <typeinfo>
#include "CppUTest/UtestMacros.h"

const static SimpleString TEST_DOUBLE_GLOBAL_CONTEXT = " ";

void checkExpectations( const SimpleString& context = TEST_DOUBLE_GLOBAL_CONTEXT );

class Expectation;
void addExpectation( const Expectation& expectation );

class Actual;
void checkActual( Actual& actual );

class Parameter
{
public:
  const SimpleString& name;
  const SimpleString  type;

  template<typename T>
  Parameter( const SimpleString& _name, const T& _value )
  : name(_name), type( typeid(_value).name() ), _variant(_value)
  { }

  template< typename T >
  T value() const;

  bool boolValue() const
  {
    const char* const returnType = typeid(bool).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asBool;
  }
  char charValue() const
  {
    const char* const returnType = typeid(char).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asChar;
  }
  unsigned char unsignedCharValue() const
  {
    const char* const returnType = typeid(unsigned char).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asUnsignedChar;
  }
  int intValue() const
  {
    const char* const returnType = typeid(int).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asInt;
  }
  unsigned int unsignedIntValue() const
  {
    const char* const returnType = typeid(unsigned int).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asUnsignedInt;
  }
  long int longIntValue() const
  {
    const char* const returnType = typeid(long int).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asLongInt;
  }
  unsigned long int unsignedLongIntValue() const
  {
    const char* const returnType = typeid(unsigned long int).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asUnsignedLongInt;
  }
  long long int longlongIntValue() const
  {
    const char* const returnType = typeid(long long int).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asLongLongInt;
  }
  unsigned long long int unsignedLongLongIntValue() const
  {
    const char* const returnType = typeid(unsigned long long int).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asUnsignedLongLongInt;
  }
  float floatValue() const
  {
    const char* const returnType = typeid(float).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asFloat;
  }
  double doubleValue() const
  {
    const char* const returnType = typeid(double).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asDouble;
  }
  void* pointerValue() const
  {
    const char* const returnType = typeid(void*).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asPointer;
  }
  const void* constPointerValue() const
  {
    const char* const returnType = typeid(const void*).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asConstPointer;
  }
  const void (*functionPointerValue())() const
  {
    const char* const returnType = typeid(const void(*)()).name();
    if( type != returnType )  FAIL( StringFromFormat( "illegal use of a %s as a %s.", type.asCharString(), returnType ).asCharString() );
    return _variant.asFunctionPointer;
  }



private:
  const union Variant {
    bool                    asBool;
    char                    asChar;
    unsigned char           asUnsignedChar;
    int                     asInt;
    unsigned int            asUnsignedInt;
    long int                asLongInt;
    unsigned long int       asUnsignedLongInt;
    long long int           asLongLongInt;
    unsigned long long int  asUnsignedLongLongInt;
    float                   asFloat;
    double                  asDouble;
    void*                   asPointer;
    const void*             asConstPointer;
    const void(*asFunctionPointer)();

    Variant( const bool& value ) : asBool(value) {}
    Variant( const char& value ) : asChar(value) {}
    Variant( const unsigned char& value ) : asUnsignedChar(value) {}
    Variant( const int& value ) : asInt(value) {}
    Variant( const unsigned int& value ) : asUnsignedInt(value) {}
    Variant( const long int& value ) : asLongInt(value) {}
    Variant( const unsigned long int& value ) : asUnsignedLongInt(value) {}
    Variant( const long long int& value ) : asLongLongInt(value) {}
    Variant( const unsigned long long int& value ) : asUnsignedLongLongInt(value) {}
    Variant( const float& value ) : asFloat(value) {}
    Variant( const double& value ) : asDouble(value) {}
    Variant( void*& value ) : asPointer(value) {}
    Variant( const void*& value ) : asConstPointer(value) {}
    Variant( const void(*value)() ) : asFunctionPointer(value) {}
  } _variant;

};  // class Parameter

#endif /* TEST_DOUBLE_H */
