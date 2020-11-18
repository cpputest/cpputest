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

#include "StandardCLibrary.h"

class SimpleStringCollection;
class TestMemoryAllocator;

class SimpleString
{
    friend bool operator==(const SimpleString& left, const SimpleString& right);
    friend bool operator!=(const SimpleString& left, const SimpleString& right);

public:
    SimpleString(const char *value = "");
    SimpleString(const char *value, size_t repeatCount);
    SimpleString(const SimpleString& other);
    ~SimpleString();

    SimpleString& operator=(const SimpleString& other);
    SimpleString operator+(const SimpleString&) const;
    SimpleString& operator+=(const SimpleString&);
    SimpleString& operator+=(const char*);

    static const size_t npos = (size_t) -1;

    char at(size_t pos) const;
    size_t find(char ch) const;
    size_t findFrom(size_t starting_position, char ch) const;
    bool contains(const SimpleString& other) const;
    bool containsNoCase(const SimpleString& other) const;
    bool startsWith(const SimpleString& other) const;
    bool endsWith(const SimpleString& other) const;
    void split(const SimpleString& split,
                    SimpleStringCollection& outCollection) const;
    bool equalsNoCase(const SimpleString& str) const;

    size_t count(const SimpleString& str) const;

    void replace(char to, char with);
    void replace(const char* to, const char* with);

    SimpleString lowerCase() const;
    SimpleString subString(size_t beginPos) const;
    SimpleString subString(size_t beginPos, size_t amount) const;
    SimpleString subStringFromTill(char startChar, char lastExcludedChar) const;
    void copyToBuffer(char* buffer, size_t bufferSize) const;

    SimpleString printable() const;

    const char *asCharString() const;
    size_t size() const;
    bool isEmpty() const;

    static void padStringsToSameLength(SimpleString& str1, SimpleString& str2, char ch);

    static TestMemoryAllocator* getStringAllocator();
    static void setStringAllocator(TestMemoryAllocator* allocator);

    static int AtoI(const char*str);
    static unsigned AtoU(const char*str);
    static int StrCmp(const char* s1, const char* s2);
    static size_t StrLen(const char*);
    static int StrNCmp(const char* s1, const char* s2, size_t n);
    static char* StrNCpy(char* s1, const char* s2, size_t n);
    static const char* StrStr(const char* s1, const char* s2);
    static char ToLower(char ch);
    static int MemCmp(const void* s1, const void *s2, size_t n);
    static char* allocStringBuffer(size_t size, const char* file, size_t line);
    static void deallocStringBuffer(char* str, size_t size, const char* file, size_t line);
private:

    const char* getBuffer() const;

    void deallocateInternalBuffer();
    void setInternalBufferAsEmptyString();
    void setInternalBufferToNewBuffer(size_t bufferSize);
    void setInternalBufferTo(char* buffer, size_t bufferSize);
    void copyBufferToNewInternalBuffer(const char* otherBuffer);
    void copyBufferToNewInternalBuffer(const char* otherBuffer, size_t bufferSize);
    void copyBufferToNewInternalBuffer(const SimpleString& otherBuffer);

    char *buffer_;
    size_t bufferSize_;

    static TestMemoryAllocator* stringAllocator_;

    char* getEmptyString() const;
    static char* copyToNewBuffer(const char* bufferToCopy, size_t bufferSize);
    static bool isDigit(char ch);
    static bool isSpace(char ch);
    static bool isUpper(char ch);
    static bool isControl(char ch);
    static bool isControlWithShortEscapeSequence(char ch);
    
    size_t getPrintableSize() const;
};

class SimpleStringCollection
{
public:
    SimpleStringCollection();
    ~SimpleStringCollection();

    void allocate(size_t size);

    size_t size() const;
    SimpleString& operator[](size_t index);

private:
    SimpleString* collection_;
    SimpleString empty_;
    size_t size_;

    void operator =(SimpleStringCollection&);
    SimpleStringCollection(SimpleStringCollection&);
};

class GlobalSimpleStringAllocatorStash
{
public:
    GlobalSimpleStringAllocatorStash();
    void save();
    void restore();
private:
    TestMemoryAllocator* originalAllocator_;
};

class MemoryAccountant;
class AccountingTestMemoryAllocator;

class GlobalSimpleStringMemoryAccountant
{
public:
    GlobalSimpleStringMemoryAccountant();
    ~GlobalSimpleStringMemoryAccountant();

    void useCacheSizes(size_t cacheSizes[], size_t length);

    void start();
    void stop();
    SimpleString report();

    AccountingTestMemoryAllocator* getAllocator();
private:
    void restoreAllocator();

    AccountingTestMemoryAllocator* allocator_;
    MemoryAccountant* accountant_;
};

SimpleString StringFrom(bool value);
SimpleString StringFrom(const void* value);
SimpleString StringFrom(void (*value)());
SimpleString StringFrom(char value);
SimpleString StringFrom(const char *value);
SimpleString StringFromOrNull(const char * value);
SimpleString StringFrom(int value);
SimpleString StringFrom(unsigned int value);
SimpleString StringFrom(long value);
SimpleString StringFrom(unsigned long value);
SimpleString StringFrom(cpputest_longlong value);
SimpleString StringFrom(cpputest_ulonglong value);
SimpleString HexStringFrom(unsigned int value);
SimpleString HexStringFrom(int value);
SimpleString HexStringFrom(signed char value);
SimpleString HexStringFrom(long value);
SimpleString HexStringFrom(unsigned long value);
SimpleString HexStringFrom(cpputest_longlong value);
SimpleString HexStringFrom(cpputest_ulonglong value);
SimpleString HexStringFrom(const void* value);
SimpleString HexStringFrom(void (*value)());
SimpleString StringFrom(double value, int precision = 6);
SimpleString StringFrom(const SimpleString& other);
SimpleString StringFromFormat(const char* format, ...) _check_format_(printf, 1, 2);
SimpleString VStringFromFormat(const char* format, va_list args);
SimpleString StringFromBinary(const unsigned char* value, size_t size);
SimpleString StringFromBinaryOrNull(const unsigned char* value, size_t size);
SimpleString StringFromBinaryWithSize(const unsigned char* value, size_t size);
SimpleString StringFromBinaryWithSizeOrNull(const unsigned char* value, size_t size);
SimpleString StringFromMaskedBits(unsigned long value, unsigned long mask, size_t byteCount);
SimpleString StringFromOrdinalNumber(unsigned int number);
SimpleString BracketsFormattedHexStringFrom(int value);
SimpleString BracketsFormattedHexStringFrom(unsigned int value);
SimpleString BracketsFormattedHexStringFrom(long value);
SimpleString BracketsFormattedHexStringFrom(unsigned long value);
SimpleString BracketsFormattedHexStringFrom(cpputest_longlong value);
SimpleString BracketsFormattedHexStringFrom(cpputest_ulonglong value);
SimpleString BracketsFormattedHexStringFrom(signed char value);
SimpleString BracketsFormattedHexString(SimpleString hexString);
SimpleString PrintableStringFromOrNull(const char * expected);

/*
 * ARM compiler has only partial support for C++11.
 * Specifically nullptr_t is not officially supported
 */
#if __cplusplus > 199711L && !defined __arm__ && CPPUTEST_USE_STD_CPP_LIB
SimpleString StringFrom(const std::nullptr_t value);
#endif

#if CPPUTEST_USE_STD_CPP_LIB

SimpleString StringFrom(const std::string& other);

#endif

#endif
