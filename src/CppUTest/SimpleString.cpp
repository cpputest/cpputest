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
#include "CppUTest/TestMemoryAllocator.h"

TestMemoryAllocator* SimpleString::stringAllocator_ = NULL;

TestMemoryAllocator* SimpleString::getStringAllocator()
{
    if (stringAllocator_ == NULL)
        return defaultNewArrayAllocator();
    return stringAllocator_;
}

void SimpleString::setStringAllocator(TestMemoryAllocator* allocator)
{
    stringAllocator_ = allocator;
}

/* Avoid using the memory leak detector INSIDE SimpleString as its used inside the detector */
char* SimpleString::allocStringBuffer(size_t _size)
{
    return getStringAllocator()->alloc_memory(_size, __FILE__, __LINE__);
}

void SimpleString::deallocStringBuffer(char* str)
{
    getStringAllocator()->free_memory(str, __FILE__, __LINE__);
}

char* SimpleString::getEmptyString() const
{
    char* empty = allocStringBuffer(1);
    empty[0] = '\0';
    return empty;
}

int SimpleString::AtoI(const char* str)
{
    while (isSpace(*str)) str++;

    char first_char = *str;
    if (first_char == '-' || first_char == '+') str++;

    int  result = 0;
    for(; isDigit(*str); str++)
    {
        result *= 10;
        result += *str - '0';
    }
    return (first_char == '-') ? -result : result;
}

int SimpleString::StrCmp(const char* s1, const char* s2)
{
   while(*s1 && *s1 == *s2)
       s1++, s2++;
   return *(unsigned char *) s1 - *(unsigned char *) s2;
}

size_t SimpleString::StrLen(const char* str)
{
    size_t n = (size_t)-1;
    do n++; while (*str++);
    return n;
}

int SimpleString::StrNCmp(const char* s1, const char* s2, size_t n)
{
    while (n && *s1 && *s1 == *s2) {
        n--, s1++, s2++;
    }
    return n ? *(unsigned char *) s1 - *(unsigned char *) s2 : 0;
}

char* SimpleString::StrNCpy(char* s1, const char* s2, size_t n)
{
    char* result = s1;

    if((NULL == s1) || (0 == n)) return result;

    while ((*s1++ = *s2++) && --n != 0)
        ;
    return result;
}

char* SimpleString::StrStr(const char* s1, const char* s2)
{
    if(!*s2) return (char*) s1;
    for (; *s1; s1++)
        if (StrNCmp(s1, s2, StrLen(s2)) == 0)
            return (char*) s1;
    return NULL;
}

char SimpleString::ToLower(char ch)
{
    return isUpper(ch) ? (char)((int)ch + ('a' - 'A')) : ch;
}

int SimpleString::MemCmp(const void* s1, const void *s2, size_t n)
{
    const unsigned char* p1 = (const unsigned char*) s1;
    const unsigned char* p2 = (const unsigned char*) s2;

    while (n--)
        if (*p1 != *p2)
            return *p1 - *p2;
        else
            p1++, p2++;
    return 0;
}

SimpleString::SimpleString(const char *otherBuffer)
{
    if (otherBuffer == 0) {
        buffer_ = getEmptyString();
    }
    else {
        buffer_ = copyToNewBuffer(otherBuffer);
    }
}

SimpleString::SimpleString(const char *other, size_t repeatCount)
{
    size_t otherStringLength = StrLen(other);
    size_t len = otherStringLength * repeatCount + 1;
    buffer_ = allocStringBuffer(len);
    char* next = buffer_;
    for (size_t i = 0; i < repeatCount; i++) {
        StrNCpy(next, other, otherStringLength + 1);
        next += otherStringLength;
    }
    *next = 0;
}

SimpleString::SimpleString(const SimpleString& other)
{
    buffer_ = copyToNewBuffer(other.buffer_);
}

SimpleString& SimpleString::operator=(const SimpleString& other)
{
    if (this != &other) {
        deallocStringBuffer(buffer_);
        buffer_ = copyToNewBuffer(other.buffer_);
    }
    return *this;
}

bool SimpleString::contains(const SimpleString& other) const
{
    return StrStr(buffer_, other.buffer_) != 0;
}

bool SimpleString::containsNoCase(const SimpleString& other) const
{
    return lowerCase().contains(other.lowerCase());
}

bool SimpleString::startsWith(const SimpleString& other) const
{
    if (StrLen(other.buffer_) == 0) return true;
    else if (size() == 0) return false;
    else return StrStr(buffer_, other.buffer_) == buffer_;
}

bool SimpleString::endsWith(const SimpleString& other) const
{
    size_t buffer_length = size();
    size_t other_buffer_length = StrLen(other.buffer_);
    if (other_buffer_length == 0) return true;
    if (buffer_length == 0) return false;
    if (buffer_length < other_buffer_length) return false;
    return StrCmp(buffer_ + buffer_length - other_buffer_length, other.buffer_) == 0;
}

size_t SimpleString::count(const SimpleString& substr) const
{
    size_t num = 0;
    char* str = buffer_;
    while (*str && (str = StrStr(str, substr.buffer_))) {
        num++;
        str++;
    }
    return num;
}

void SimpleString::split(const SimpleString& delimiter, SimpleStringCollection& col) const
{
    size_t num = count(delimiter);
    size_t extraEndToken = (endsWith(delimiter)) ? 0 : 1U;
    col.allocate(num + extraEndToken);

    char* str = buffer_;
    char* prev;
    for (size_t i = 0; i < num; ++i) {
        prev = str;
        str = StrStr(str, delimiter.buffer_) + 1;
        size_t len = (size_t) (str - prev) + 1;
        col[i].buffer_ = copyToNewBuffer(prev, len);
    }
    if (extraEndToken) {
        col[num] = str;
    }
}

void SimpleString::replace(char to, char with)
{
    size_t s = size();
    for (size_t i = 0; i < s; i++) {
        if (buffer_[i] == to) buffer_[i] = with;
    }
}

void SimpleString::replace(const char* to, const char* with)
{
    size_t c = count(to);
    size_t len = size();
    size_t tolen = StrLen(to);
    size_t withlen = StrLen(with);

    size_t newsize = len + (withlen * c) - (tolen * c) + 1;

    if (newsize > 1) {
        char* newbuf = allocStringBuffer(newsize);
        for (size_t i = 0, j = 0; i < len;) {
            if (StrNCmp(&buffer_[i], to, tolen) == 0) {
                StrNCpy(&newbuf[j], with, withlen + 1);
                j += withlen;
                i += tolen;
            }
            else {
                newbuf[j] = buffer_[i];
                j++;
                i++;
            }
        }
        deallocStringBuffer(buffer_);
        buffer_ = newbuf;
        buffer_[newsize - 1] = '\0';
    }
    else {
        buffer_ = getEmptyString();
        buffer_[0] = '\0';
    }
}

SimpleString SimpleString::lowerCase() const
{
    SimpleString str(*this);

    size_t str_size = str.size();
    for (size_t i = 0; i < str_size; i++)
        str.buffer_[i] = ToLower(str.buffer_[i]);

    return str;
}

const char *SimpleString::asCharString() const
{
    return buffer_;
}

size_t SimpleString::size() const
{
    return StrLen(buffer_);
}

bool SimpleString::isEmpty() const
{
    return size() == 0;
}


SimpleString::~SimpleString()
{
    deallocStringBuffer(buffer_);
}

bool operator==(const SimpleString& left, const SimpleString& right)
{
    return 0 == SimpleString::StrCmp(left.asCharString(), right.asCharString());
}

bool SimpleString::equalsNoCase(const SimpleString& str) const
{
    return lowerCase() == str.lowerCase();
}


bool operator!=(const SimpleString& left, const SimpleString& right)
{
    return !(left == right);
}

SimpleString SimpleString::operator+(const SimpleString& rhs)
{
    SimpleString t(buffer_);
    t += rhs.buffer_;
    return t;
}

SimpleString& SimpleString::operator+=(const SimpleString& rhs)
{
    return operator+=(rhs.buffer_);
}

SimpleString& SimpleString::operator+=(const char* rhs)
{
    size_t originalSize = this->size();
    size_t additionalStringSize = StrLen(rhs) + 1;
    size_t sizeOfNewString = originalSize + additionalStringSize;
    char* tbuffer = copyToNewBuffer(this->buffer_, sizeOfNewString);
    StrNCpy(tbuffer + originalSize, rhs, additionalStringSize);
    deallocStringBuffer(this->buffer_);
    this->buffer_ = tbuffer;
    return *this;
}

void SimpleString::padStringsToSameLength(SimpleString& str1, SimpleString& str2, char padCharacter)
{
    if (str1.size() > str2.size()) {
        padStringsToSameLength(str2, str1, padCharacter);
        return;
    }

    char pad[2];
    pad[0] = padCharacter;
    pad[1] = 0;
    str1 = SimpleString(pad, str2.size() - str1.size()) + str1;
}

SimpleString SimpleString::subString(size_t beginPos, size_t amount) const
{
    if (beginPos > size()-1) return "";

    SimpleString newString = buffer_ + beginPos;

    if (newString.size() > amount)
        newString.buffer_[amount] = '\0';

    return newString;
}

char SimpleString::at(int pos) const
{
    return buffer_[pos];
}

int SimpleString::find(char ch) const
{
    return findFrom(0, ch);
}

int SimpleString::findFrom(size_t starting_position, char ch) const
{
    size_t length = size();
    for (size_t i = starting_position; i < length; i++)
        if (buffer_[i] == ch) return (int) i;
    return -1;
}

SimpleString SimpleString::subStringFromTill(char startChar, char lastExcludedChar) const
{
    int beginPos = find(startChar);
    if (beginPos < 0) return "";

    int endPos = findFrom((size_t)beginPos, lastExcludedChar);
    if (endPos == -1) return subString((size_t)beginPos, size());

    return subString((size_t)beginPos, (size_t) (endPos - beginPos));
}

char* SimpleString::copyToNewBuffer(const char* bufferToCopy, size_t bufferSize)
{
    if(bufferSize == 0) bufferSize = StrLen(bufferToCopy) + 1;

    char* newBuffer = allocStringBuffer(bufferSize);
    StrNCpy(newBuffer, bufferToCopy, bufferSize);
    newBuffer[bufferSize-1] = '\0';
    return newBuffer;
}

void SimpleString::copyToBuffer(char* bufferToCopy, size_t bufferSize) const
{
    if (bufferToCopy == NULL || bufferSize == 0) return;

    size_t sizeToCopy = (bufferSize-1 < size()) ? bufferSize : size();

    StrNCpy(bufferToCopy, buffer_, sizeToCopy);
    bufferToCopy[sizeToCopy] = '\0';
}

bool SimpleString::isDigit(char ch)
{
    return '0' <= ch && '9' >= ch;
}

bool SimpleString::isSpace(char ch)
{
    return (ch == ' ') || (0x08 < ch && 0x0E > ch);
}

bool SimpleString::isUpper(char ch)
{
    return 'A' <= ch && 'Z' >= ch;
}

SimpleString StringFrom(bool value)
{
    return SimpleString(StringFromFormat("%s", value ? "true" : "false"));
}

SimpleString StringFrom(const char *value)
{
    return SimpleString(value);
}

SimpleString StringFromOrNull(const char * expected)
{
    return (expected) ? StringFrom(expected) : "(null)";
}

SimpleString StringFrom(int value)
{
    return StringFromFormat("%d", value);
}

SimpleString StringFrom(long value)
{
    return StringFromFormat("%ld", value);
}

SimpleString StringFrom(const void* value)
{
    return SimpleString("0x") + HexStringFrom(value);
}

SimpleString HexStringFrom(long value)
{
    return StringFromFormat("%lx", value);
}

SimpleString HexStringFrom(unsigned long value)
{
    return StringFromFormat("%lx", value);
}

static long convertPointerToLongValue(const void* value)
{
    /*
     * This way of converting also can convert a 64bit pointer in a 32bit integer by truncating.
     * This isn't the right way to convert pointers values and need to change by implementing a
     * proper portable way to convert pointers to strings.
     */
    long* long_value = (long*) &value;
    return *long_value;
}

SimpleString HexStringFrom(const void* value)
{
    return StringFromFormat("%lx", convertPointerToLongValue(value));
}

SimpleString StringFrom(double value, int precision)
{
    return StringFromFormat("%.*g", precision, value);
}

SimpleString StringFrom(char value)
{
    return StringFromFormat("%c", value);
}

SimpleString StringFrom(const SimpleString& value)
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

SimpleString StringFrom(unsigned int i)
{
    return StringFromFormat("%10u (0x%08x)", i, i);
}

#if CPPUTEST_USE_STD_CPP_LIB

#include <string>

SimpleString StringFrom(const std::string& value)
{
    return SimpleString(value.c_str());
}

#endif

SimpleString StringFrom(unsigned long i)
{
    return StringFromFormat("%lu (0x%lx)", i, i);
}

//Kludge to get a va_copy in VC++ V6
#ifndef va_copy
#define va_copy(copy, original) copy = original;
#endif

SimpleString VStringFromFormat(const char* format, va_list args)
{
    va_list argsCopy;
    va_copy(argsCopy, args);
    enum
    {
        sizeOfdefaultBuffer = 100
    };
    char defaultBuffer[sizeOfdefaultBuffer];
    SimpleString resultString;

    size_t size = (size_t)PlatformSpecificVSNprintf(defaultBuffer, sizeOfdefaultBuffer, format, args);
    if (size < sizeOfdefaultBuffer) {
        resultString = SimpleString(defaultBuffer);
    }
    else {
        size_t newBufferSize = size + 1;
        char* newBuffer = SimpleString::allocStringBuffer(newBufferSize);
        PlatformSpecificVSNprintf(newBuffer, newBufferSize, format, argsCopy);
        resultString = SimpleString(newBuffer);

        SimpleString::deallocStringBuffer(newBuffer);
    }
    va_end(argsCopy);
    return resultString;
}

SimpleString StringFromBinary(const unsigned char* value, size_t size)
{
    SimpleString result;

    for (size_t i = 0; i < size; i++) {
        result += StringFromFormat("%02X ", value[i]);
    }
    result = result.subString(0, result.size() - 1);

    return result;
}

SimpleString StringFromBinaryOrNull(const unsigned char* value, size_t size)
{
    return (value) ? StringFromBinary(value, size) : "(null)";
}

SimpleStringCollection::SimpleStringCollection()
{
    collection_ = 0;
    size_ = 0;
}

void SimpleStringCollection::allocate(size_t _size)
{
    delete[] collection_;

    size_ = _size;
    collection_ = new SimpleString[size_];
}

SimpleStringCollection::~SimpleStringCollection()
{
    delete[] (collection_);
}

size_t SimpleStringCollection::size() const
{
    return size_;
}

SimpleString& SimpleStringCollection::operator[](size_t index)
{
    if (index >= size_) {
        empty_ = "";
        return empty_;
    }

    return collection_[index];
}
