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

#include <CppUTestExt/TestDoubleParameter.h>

namespace TestDouble {

bool Parameter::equals( const Parameter* const &pOther ) const
{
  if( type != pOther->type ) return false;

  if( 0 != buffer )
  {
    if( 0 == pOther->buffer ) return false;
    if( bufferSize_bytes != pOther->bufferSize_bytes ) return false;
    return ( 0 == memcmp( buffer, pOther->buffer, bufferSize_bytes ) );
  }

  if( type == typeid(void*).name() ) { return _variant.value.asPointer == pOther->_variant.value.asPointer; }
  if( type == typeid(const void*).name() ) { return _variant.value.asConstPointer == pOther->_variant.value.asConstPointer; }
  if( type == typeid(bool).name() ) { return _variant.value.asBool == pOther->_variant.value.asBool; }
  if( type == typeid(char).name() ) { return _variant.value.asChar == pOther->_variant.value.asChar; }
  if( type == typeid(unsigned char).name() ) { return _variant.value.asUnsignedChar == pOther->_variant.value.asUnsignedChar; }
  if( type == typeid(int).name() ) { return _variant.value.asInt == pOther->_variant.value.asInt; }
  if( type == typeid(unsigned int).name() ) { return _variant.value.asUnsignedInt == pOther->_variant.value.asUnsignedInt; }
  if( type == typeid(long).name() ) { return _variant.value.asLong == pOther->_variant.value.asLong; }
  if( type == typeid(unsigned long).name() ) { return _variant.value.asUnsignedLong == pOther->_variant.value.asUnsignedLong; }
  if( type == typeid(long long).name() ) { return _variant.value.asLongLong == pOther->_variant.value.asLongLong; }
  if( type == typeid(unsigned long long).name() ) { return _variant.value.asUnsignedLongLong == pOther->_variant.value.asUnsignedLongLong; }
  if( type == typeid(float).name() ) { return _variant.value.asFloat == pOther->_variant.value.asFloat; }
  if( type == typeid(double).name() ) { return _variant.value.asDouble == pOther->_variant.value.asDouble; }
  return _variant.value.asPointer == pOther->_variant.value.asPointer;
}

bool Parameter::setValue( const Parameter* const &pOther )
{
  if( 0 != pOther->buffer )
  {
    if( pOther->bufferSize_bytes != bufferSize_bytes ) return false;
    PlatformSpecificMemCpy( outputBuffer, pOther->buffer, bufferSize_bytes );
  }
  else PlatformSpecificMemCpy( outputBuffer, &(pOther->_variant.value), bufferSize_bytes );

  return true;
}

void Parameter::setDefault()
{
  if( 0 != buffer ) PlatformSpecificMemCpy( outputBuffer, buffer, bufferSize_bytes );
  else PlatformSpecificMemCpy( outputBuffer, &_variant.value, bufferSize_bytes );
}

SimpleString Parameter::toString()
{
  SimpleString ret = name + "\t";
  switch( _variant.type )
  {
    case Variant::FN_POINTER:
    {
      return ret + "<function pointer>" + HexStringFrom(_variant.value.asFunctionPointer);
      break;
    }
    case Variant::POINTER:
    {
      return ret + "<pointer>" + HexStringFrom(_variant.value.asPointer);
      break;
    }
    case Variant::CONST_POINTER:
    {
      return ret + "<const pointer>" + HexStringFrom(_variant.value.asConstPointer);
      break;
    }
    case Variant::DOUBLE:
    {
      return ret + "<double>" + StringFrom( _variant.value.asDouble );
      break;
    }
    case Variant::FLOAT:
    {
      return ret + "<float>" + StringFrom( _variant.value.asFloat );
      break;
    }
    case Variant::LONG_LONG:
    {
      return ret + "<long long>" + StringFrom( _variant.value.asLongLong );
      break;
    }
    case Variant::UNSIGNED_LONG_LONG:
    {
      return ret + "<unsigned long long>" + StringFrom( _variant.value.asUnsignedLongLong );
      break;
    }
    case Variant::LONG:
    {
      return ret + "<long>" + StringFrom( _variant.value.asLong );
      break;
    }
    case Variant::UNSIGNED_LONG:
    {
      return ret + "<unsigned long>" + StringFrom( _variant.value.asUnsignedLong );
      break;
    }
    case Variant::INT:
    {
      return ret + "<int>" + StringFrom( _variant.value.asInt );
      break;
    }
    case Variant::UNSIGNED_INT:
    {
      return ret + "<unsigned int>" + StringFrom( _variant.value.asUnsignedInt );
      break;
    }
    case Variant::SHORT:
    {
      return ret + "<short>" + StringFrom( _variant.value.asShort );
      break;
    }
    case Variant::UNSIGNED_SHORT:
    {
      return ret + "<unsigned short>" + StringFrom( _variant.value.asUnsignedShort );
      break;
    }
    case Variant::CHAR:
    {
      return ret + "<char>" + StringFrom( _variant.value.asChar );
      break;
    }
    case Variant::UNSIGNED_CHAR:
    {
      return ret + "<unsigned char>" + StringFrom( _variant.value.asUnsignedChar );
      break;
    }
    case Variant::BOOL:
    {
      return ret + "<bool>" + StringFrom( _variant.value.asBool );
      break;
    }
    default:
    {
      return ret + "<unknown type (" + type + ")>" + HexStringFrom(_variant.value.asConstPointer);
    }
  }
}

} // namespace TestDouble
