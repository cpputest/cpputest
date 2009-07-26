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

#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleString.h"
#include "CppUTest/PlatformSpecificFunctions.h"

static char* allocString(int size)
{
   return new char[size];
}

static void deallocString(char* str)
{
   delete [] str;
}
static char* getEmptryString()
{
   char* empty = new char[1];
   empty[0] = '\0';
   return empty;
}
SimpleString::SimpleString (const char *otherBuffer)
{
  if (otherBuffer == 0) {
  	  buffer = getEmptryString();
  }
  else {
     int len = PlatformSpecificStrLen (otherBuffer) + 1;
     buffer = allocString(len);
     PlatformSpecificStrCpy (buffer, otherBuffer);
  }
}

SimpleString::SimpleString (const char *other, int repeatCount)
{
    int len = PlatformSpecificStrLen(other) *  repeatCount + 1;
    buffer = allocString(len);
    char* next = buffer;
    for (int i = 0; i < repeatCount; i++)
    {
       PlatformSpecificStrCpy(next, other);
        next += PlatformSpecificStrLen(other);
    }
    *next = 0;

}
SimpleString::SimpleString (const SimpleString& other)
{
   int len = other.size() + 1;
   buffer = allocString(len);
   PlatformSpecificStrCpy(buffer, other.buffer);
}


SimpleString& SimpleString::operator= (const SimpleString& other)
{
  if (this != &other)
    {
     deallocString(buffer);
     int len = other.size() + 1;
     buffer = allocString(len);
     PlatformSpecificStrCpy(buffer, other.buffer);
    }
  return *this;
}

bool SimpleString::contains(const SimpleString& other) const
 {
    //strstr on some machines does not handle ""
    //the right way.  "" should be found in any string
    if (PlatformSpecificStrLen(other.buffer) == 0)
      return true;
    else if (PlatformSpecificStrLen(buffer) == 0)
      return false;
    else
      return PlatformSpecificStrStr(buffer, other.buffer) != 0;
  }

bool SimpleString::startsWith(const SimpleString& other) const
{
    if (PlatformSpecificStrLen(other.buffer) == 0)
      return true;
    else if (PlatformSpecificStrLen(buffer) == 0)
      return false;
    else
      return PlatformSpecificStrStr(buffer, other.buffer) == buffer;
}

bool SimpleString::endsWith(const SimpleString& other) const
{
	int buffer_length = PlatformSpecificStrLen(buffer);
	int other_buffer_length = PlatformSpecificStrLen(other.buffer);
    if (other_buffer_length == 0) return true;
    if (buffer_length == 0) return false;
    if (buffer_length < other_buffer_length) return false;
	return PlatformSpecificStrCmp(buffer + buffer_length - other_buffer_length, other.buffer) == 0;
}

int SimpleString::count(const SimpleString& substr) const
{
	int num = 0;
	char* str = buffer;
	while( (str = PlatformSpecificStrStr(str, substr.buffer)) ) {
		num++;
		str++;
	}
	return num;
}

void SimpleString::split(const SimpleString& split, SimpleStringCollection& col) const
{
   int num = count(split);
   int extraEndToken = (endsWith(split)) ? 0 : 1;
   col.allocate(num + extraEndToken);

   char* str = buffer;
   char* prev;
   for (int i = 0; i < num; ++i){
      prev = str;
      str = PlatformSpecificStrStr(str, split.buffer) + 1;
      int len = str - prev;
      char* sub = allocString(len+1);
      PlatformSpecificStrNCpy(sub, prev, len);
      sub[len] = '\0';
      col[i] = sub;
      deallocString(sub);
   }
   if (extraEndToken) {
      col[num] = str;
   }
}

void SimpleString::replace(char to, char with)
{
	int s = size();
	for (int i = 0; i < s; i++) {
		if (buffer[i] == to) buffer[i] = with;
	}
}

void SimpleString::replace(const char* to, const char* with)
{
	int c = count(to);
	int len = size();
	int tolen = PlatformSpecificStrLen(to);
	int withlen = PlatformSpecificStrLen(with);

	int newsize = len + (withlen * c) - (tolen * c) + 1;

	if (newsize) {
		char* newbuf = allocString(newsize);
		for (int i = 0, j = 0; i < len;) {
			if (PlatformSpecificStrNCmp(&buffer[i], to, tolen) == 0) {
			   PlatformSpecificStrNCpy(&newbuf[j], with, withlen);
				j += withlen;
				i += tolen;
			}
			else {
				newbuf[j] = buffer[i];
				j++;
				i++;
			}
		}
		deallocString(buffer);
		buffer = newbuf;
		buffer[newsize-1] = '\0';
	}
	else {
  	  buffer = getEmptryString();
  	  buffer [0] = '\0';
	}
}


const char *SimpleString::asCharString () const
  {
    return buffer;
  }

int SimpleString::size() const
  {
    return PlatformSpecificStrLen (buffer);
  }

SimpleString::~SimpleString ()
{
   deallocString(buffer);
}


bool operator== (const SimpleString& left, const SimpleString& right)
{
  return 0 == PlatformSpecificStrCmp (left.asCharString (), right.asCharString ());
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
  int len = this->size() + PlatformSpecificStrLen(rhs) + 1;
  char* tbuffer = allocString(len);
  PlatformSpecificStrCpy(tbuffer, this->buffer);
  PlatformSpecificStrCat(tbuffer, rhs);
  deallocString(buffer);
  buffer = tbuffer;
  return *this;
}

SimpleString StringFrom (bool value)
{
  return SimpleString(StringFromFormat("%s", value ? "true" : "false"));
}

SimpleString StringFrom (const char *value)
{
  return SimpleString(value);
}

SimpleString StringFrom (int value)
{
    return StringFromFormat("%d", value);
}

SimpleString StringFrom (long value)
{
  return StringFromFormat("%ld", value);
}

SimpleString StringFrom (void* value)
{
    return SimpleString("0x") + HexStringFrom((long)value);
}

SimpleString HexStringFrom (long value)
{
  return StringFromFormat("%lx", value);
}

SimpleString StringFrom (double value, int precision)
{
   SimpleString format = StringFromFormat("%%.%df", precision);
   return StringFromFormat(format.asCharString(), value);
}

SimpleString StringFrom (char value)
{
  return StringFromFormat("%c", value);
}

SimpleString StringFrom (const SimpleString& value)
{
  return SimpleString(value);
}


SimpleString StringFromFormat(const char* format, ...)
{
   SimpleString resultString;
   va_list arguments;
   va_start(arguments, format);

   resultString = VStringFromFormat(format, arguments);
   va_end(arguments);
   return resultString;
}

SimpleString VStringFromFormat(const char* format, va_list args)
{
   enum { sizeOfdefaultBuffer = 100 };
   char defaultBuffer[sizeOfdefaultBuffer];
   SimpleString resultString;

   int size = PlatformSpecificVSNprintf(defaultBuffer, sizeOfdefaultBuffer, format, args);
   if (size < sizeOfdefaultBuffer) {
      resultString = SimpleString(defaultBuffer);
   }
   else {
      char* newBuffer = new char[size+1];
      PlatformSpecificVSNprintf(newBuffer, size+1, format, args);
      resultString = SimpleString(newBuffer);
      delete [] newBuffer;
   }
   return resultString;
}

SimpleStringCollection::SimpleStringCollection()
{
   collection = 0;
   _size = 0;
}

void SimpleStringCollection::allocate(int size)
{
   if (collection) delete [] collection;

   _size = size;
   collection = new SimpleString[_size];
}


SimpleStringCollection::~SimpleStringCollection()
{
	 delete[] (collection);
}

int SimpleStringCollection::size () const
{
   return _size;
}

SimpleString& SimpleStringCollection::operator[](int index)
{
   if (index >= _size) {
      empty = "";
      return empty;
   }

   return collection[index];
}

