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

  if( type == typeid(void*).name() ) { return _variant.asPointer == pOther->_variant.asPointer; }
  if( type == typeid(const void*).name() ) { return _variant.asConstPointer == pOther->_variant.asConstPointer; }
  if( type == typeid(bool).name() ) { return _variant.asBool == pOther->_variant.asBool; }
  if( type == typeid(char).name() ) { return _variant.asChar == pOther->_variant.asChar; }
  if( type == typeid(unsigned char).name() ) { return _variant.asUnsignedChar == pOther->_variant.asUnsignedChar; }
  if( type == typeid(int).name() ) { return _variant.asInt == pOther->_variant.asInt; }
  if( type == typeid(unsigned int).name() ) { return _variant.asUnsignedInt == pOther->_variant.asUnsignedInt; }
  if( type == typeid(long).name() ) { return _variant.asLong == pOther->_variant.asLong; }
  if( type == typeid(unsigned long).name() ) { return _variant.asUnsignedLong == pOther->_variant.asUnsignedLong; }
  if( type == typeid(long long).name() ) { return _variant.asLongLong == pOther->_variant.asLongLong; }
  if( type == typeid(unsigned long long).name() ) { return _variant.asUnsignedLongLong == pOther->_variant.asUnsignedLongLong; }
  if( type == typeid(float).name() ) { return _variant.asFloat == pOther->_variant.asFloat; }
  if( type == typeid(double).name() ) { return _variant.asDouble == pOther->_variant.asDouble; }
  return _variant.asPointer == pOther->_variant.asPointer;
}

} // namespace TestDouble
