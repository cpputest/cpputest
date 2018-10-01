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

#include "CppUTestExt/Actual.h"

Actual actual( const SimpleString& context )
{
  return Actual( context );
}

ActualCall Actual::call( const SimpleString& name )
{
  return ActualCall( _context, name );
}

ActualCall::ActualCall( const SimpleString& context, const SimpleString& name )
  : _context(context), _methodName(name)
{ }

bool ActualCall::returnBool()
{
  // return _actualCall.returnBoolValue();
  return true;
}

char ActualCall::returnChar()
{
  // return (char)_actualCall.returnIntValue();
  return 0;
}

unsigned char ActualCall::returnUnsignedChar()
{
  // return (unsigned char)_actualCall.returnUnsignedIntValue();
  return 0;
}

int ActualCall::returnInt()
{
  // return _actualCall.returnIntValue();
  return 0;
}

unsigned int ActualCall::returnUnsignedInt()
{
  // return _actualCall.returnUnsignedIntValue();
  return 0;
}

long ActualCall::returnLongInt()
{
  // return _actualCall.returnLongIntValue();
  return 0;
}

unsigned long ActualCall::returnUnsignedLongInt()
{
  // return _actualCall.returnUnsignedLongIntValue();
  return 0;
}

long long ActualCall::returnLongLongInt()
{
  // return _actualCall.returnLongLongIntValue();
  return 0;
}

unsigned long long ActualCall::returnUnsignedLongLongInt()
{
  // return _actualCall.returnUnsignedLongLongIntValue();
  return 0;
}

// FIXME CppUMock doesn't support float
// float ActualCall::returnFloat()
// {
//   return static_cast<float>(_actualCall.returnDoubleValue());
// }

double ActualCall::returnDouble()
{
  // return _actualCall.returnDoubleValue();
  return 0;
}

void* ActualCall::returnPointer()
{
  // return _actualCall.returnPointerValue();
  return 0;
}

const void* ActualCall::returnConstPointer()
{
  // return _actualCall.returnConstPointerValue();
  return 0;
}

void (*ActualCall::returnFunctionPointer())()
{
  // return _actualCall.returnFunctionPointerValue();
  return 0;
}
