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

///////////////////////////////////////////////////////////////////////////////
//
// SIMPLESTRING.H
//
// One of the design goals of CppUnitLite is to compilation with very old C++
// compilers.  For that reason, the simple string class that provides
// only the operations needed in CppUnitLite.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_SimpleString_h
#define D_SimpleString_h

class SimpleStringCollection;

class SimpleString
  {
    friend bool	operator== (const SimpleString& left, const SimpleString& right);
    friend bool	operator!= (const SimpleString& left, const SimpleString& right);

  public:
    SimpleString (const char *value = "");
    SimpleString (const char *value, int repeatCount);
    SimpleString (const SimpleString& other);
    ~SimpleString ();

    SimpleString& operator= (const SimpleString& other);
    SimpleString operator+(const SimpleString&);
    SimpleString& operator+=(const SimpleString&);
    SimpleString& operator+=(const char*);

    bool contains(const SimpleString& other) const;
    bool startsWith(const SimpleString& other) const;
    bool endsWith(const SimpleString& other) const;
    void split(const SimpleString& split, SimpleStringCollection& outCollection) const;

    int count(const SimpleString& str) const;

    void replace(char to, char with);
    void replace(const char* to, const char* with);

    const char *asCharString () const;
    int	size() const;

  private:
    char *buffer;
  };

class SimpleStringCollection
{
public:
   SimpleStringCollection();
   ~SimpleStringCollection();

   void allocate(int size);

   int size () const;
   SimpleString& operator[](int index);

private:
   SimpleString* collection;
   SimpleString empty;
   int _size;

   void operator =(SimpleStringCollection&);
   SimpleStringCollection(SimpleStringCollection&);
};

SimpleString StringFrom (bool value);
SimpleString StringFrom (void* value);
SimpleString StringFrom (char value);
SimpleString StringFrom (const char *value);
SimpleString StringFrom (long value);
SimpleString StringFrom (int value);
SimpleString HexStringFrom(long value);
SimpleString StringFrom (double value, int precision = 6);
SimpleString StringFrom (const SimpleString& other);
SimpleString StringFromFormat(const char* format, ...);
SimpleString VStringFromFormat(const char* format, va_list args);

#endif
