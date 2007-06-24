/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
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


#include "SimpleString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


SimpleString::SimpleString ()
    : buffer(new char [1])
{
  buffer [0] = '\0';
}


SimpleString::SimpleString (const char *otherBuffer)
    : buffer (new char [strlen (otherBuffer) + 1])
{
  strcpy (buffer, otherBuffer);
}

SimpleString::SimpleString (const SimpleString& other)
{
  buffer = new char [other.size() + 1];
  strcpy(buffer, other.buffer);
}


SimpleString& SimpleString::operator= (const SimpleString& other)
{
  if (this != &other)
    {
      delete [] buffer;
      buffer = new char [other.size() + 1];
      strcpy(buffer, other.buffer);
    }
  return *this;
}

bool SimpleString::contains(const SimpleString& other) const
  {
    //strstr on some machines does not handle ""
    //the right way.  "" should be found in any string
    if (strlen(other.buffer) == 0)
      return true;
    else if (strlen(buffer) == 0)
      return false;
    else
      return strstr(buffer, other.buffer) != NULL;
  }

const char *SimpleString::asCharString () const
  {
    return buffer;
  }

int SimpleString::size() const
  {
    return strlen (buffer);
  }

SimpleString::~SimpleString ()
{
  delete [] buffer;
}


bool operator== (const SimpleString& left, const SimpleString& right)
{
  return 0 == strcmp (left.asCharString (), right.asCharString ());
}

bool operator!= (const SimpleString& left, const SimpleString& right)
{
  return !(left == right);
}

SimpleString SimpleString::operator+(const SimpleString& rhs)
{
  SimpleString t(buffer);
  t += rhs.buffer;
  return t;
}

SimpleString& SimpleString::operator+=(const SimpleString& rhs)
{
  return operator+=(rhs.buffer);
}

SimpleString& SimpleString::operator+=(const char* rhs)
{
  char* tbuffer = new char [this->size() + strlen(rhs) + 1];
  strcpy(tbuffer, this->buffer);
  strcat(tbuffer, rhs);
  delete [] buffer;
  buffer = tbuffer;
  return *this;
}

SimpleString StringFrom (bool value)
{
  char buffer [sizeof ("false") + 1];
  sprintf (buffer, "%s", value ? "true" : "false");
  return SimpleString(buffer);
}

SimpleString StringFrom (const char *value)
{
  return SimpleString(value);
}

SimpleString StringFrom (long value)
{
  char buffer [20];
  sprintf (buffer, "%ld", value);

  return SimpleString(buffer);
}

SimpleString StringFrom (double value)
{
  char buffer [40];
  sprintf (buffer, "%lf", value);

  return SimpleString(buffer);
}

SimpleString StringFrom (const SimpleString& value)
{
  return SimpleString(value);
}
