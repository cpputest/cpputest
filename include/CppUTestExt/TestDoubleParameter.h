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

namespace TestDouble {

class Parameter
{
public:
  const SimpleString  name;
  const SimpleString  type;

  template<typename T>
  Parameter( const SimpleString& _name, const T& _value )
  : name(_name), type( typeid(_value).name() ), _variant(_value)
  { }

  bool equals( const Parameter* pOther ) const
  {
    if( type == typeid(bool).name() ) { return _variant.asBool == pOther->_variant.asBool; }
    if( type == typeid(char).name() ) { return _variant.asChar == pOther->_variant.asChar; }
    if( type == typeid(unsigned char).name() ) { return _variant.asUnsignedChar == pOther->_variant.asUnsignedChar; }
    if( type == typeid(int).name() ) { return _variant.asInt == pOther->_variant.asInt; }
    if( type == typeid(unsigned int).name() ) { return _variant.asUnsignedInt == pOther->_variant.asUnsignedInt; }
    if( type == typeid(long).name() ) { return _variant.asLongInt == pOther->_variant.asLongInt; }
    if( type == typeid(unsigned long).name() ) { return _variant.asUnsignedLongInt == pOther->_variant.asUnsignedLongInt; }
    if( type == typeid(long long).name() ) { return _variant.asLongLongInt == pOther->_variant.asLongLongInt; }
    if( type == typeid(unsigned long long).name() ) { return _variant.asUnsignedLongLongInt == pOther->_variant.asUnsignedLongLongInt; }
    if( type == typeid(float).name() ) { return _variant.asFloat == pOther->_variant.asFloat; }
    if( type == typeid(double).name() ) { return _variant.asDouble == pOther->_variant.asDouble; }
    // if( type == typeid(void*).name() ) { return _variant.asPointer == pOther->_variant.asPointer; }
    // if( type == typeid(const void*).name() ) { return _variant.asConstPointer == pOther->_variant.asConstPointer; }
    // return _variant.asFunctionPointer == pOther->_variant.asFunctionPointer;
    return _variant.asPointer == pOther->_variant.asPointer;
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
    Variant( const long& value ) : asLongInt(value) {}
    Variant( const unsigned long& value ) : asUnsignedLongInt(value) {}
    Variant( const long long& value ) : asLongLongInt(value) {}
    Variant( const unsigned long long& value ) : asUnsignedLongLongInt(value) {}
    Variant( const float& value ) : asFloat(value) {}
    Variant( const double& value ) : asDouble(value) {}
    Variant( void*& value ) : asPointer(value) {}
    Variant( const void*& value ) : asConstPointer(value) {}
    Variant( const void(*value)() ) : asFunctionPointer(value) {}
  } _variant;

};  // class Parameter

} // namespace TestDouble

#endif // TEST_DOUBLE_PARAMETER_H
