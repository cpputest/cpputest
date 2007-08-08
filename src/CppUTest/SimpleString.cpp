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


SimpleString::SimpleString (const char *otherBuffer)
{
  if (otherBuffer == 0) {
  	  buffer = new char [1];
  	  buffer [0] = '\0';
  }
  else {
  	buffer = new char [strlen (otherBuffer) + 1];
  	strcpy (buffer, otherBuffer);
  }
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

bool SimpleString::startsWith(const SimpleString& other) const
{
    if (strlen(other.buffer) == 0)
      return true;
    else if (strlen(buffer) == 0)
      return false;
    else
      return strstr(buffer, other.buffer) == buffer;
}

bool SimpleString::endsWith(const SimpleString& other) const
{
	int buffer_length = strlen(buffer);
	int other_buffer_length = strlen(other.buffer);
    if (other_buffer_length == 0) return true;
    if (buffer_length == 0) return false;
    if (buffer_length < other_buffer_length) return false;
	return strcmp(buffer + buffer_length - other_buffer_length, other.buffer) == 0;
}

int SimpleString::count(const SimpleString& substr) const
{
	int num = 0;
	char* str = buffer;
	while( (str = strstr(str, substr.buffer)) ) {
		num++;
		str++;
	}
	return num;
}

int SimpleString::split(const SimpleString& split, SimpleString*& output) const
{
	int num = count(split);
	int extraEndToken = (endsWith(split)) ? 0 : 1;
	output = new SimpleString[num + extraEndToken];

	char* str = buffer;
	char* prev;
	for (int i = 0; i < num; ++i){
		prev = str;
		str = strstr(str, split.buffer) + 1;
		int len = str - prev;
		char* sub = new char[len+1];
		strncpy(sub, prev, len);
		sub[len] = '\0';
		output[i] = sub;
		delete [] sub;
	}
	if (extraEndToken) {
		output[num] = str;
	}
	return num + extraEndToken;
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
	int tolen = strlen(to);
	int withlen = strlen(with);
	
	int newsize = len + (withlen * c) - (tolen * c) + 1;
	
	if (newsize) {
		char* newbuf = new char[newsize];
		for (int i = 0, j = 0; i < len;) {
			if (strncmp(&buffer[i], to, tolen) == 0) {
				strncpy(&newbuf[j], with, withlen);
				j += withlen;
				i += tolen;
			}
			else {
				newbuf[j] = buffer[i];
				j++;
				i++;
			}
		}
		delete [] buffer;
		buffer = newbuf;
		buffer[newsize-1] = '\0';
	}
	else {
  	  buffer = new char [1];
  	  buffer [0] = '\0';
	}		
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

SimpleString StringFrom (char value)
{
  char buffer [10];
  sprintf (buffer, "%c", value);

  return SimpleString(buffer);
}

SimpleString StringFrom (const SimpleString& value)
{
  return SimpleString(value);
}
