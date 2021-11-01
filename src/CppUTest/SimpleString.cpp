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

GlobalSimpleStringAllocatorStash::GlobalSimpleStringAllocatorStash()
    : originalAllocator_(NULLPTR)
{
}

void GlobalSimpleStringAllocatorStash::save()
{
    originalAllocator_ = SimpleString::getStringAllocator();
}

void GlobalSimpleStringAllocatorStash::restore()
{
    SimpleString::setStringAllocator(originalAllocator_);
}


GlobalSimpleStringMemoryAccountant::GlobalSimpleStringMemoryAccountant()
    : allocator_(NULLPTR)
{
    accountant_ = new MemoryAccountant();
}

GlobalSimpleStringMemoryAccountant::~GlobalSimpleStringMemoryAccountant()
{
    restoreAllocator();

    delete accountant_;
    delete allocator_;
}

void GlobalSimpleStringMemoryAccountant::restoreAllocator()
{
    if (SimpleString::getStringAllocator() == allocator_)
        SimpleString::setStringAllocator(allocator_->originalAllocator());
}

void GlobalSimpleStringMemoryAccountant::useCacheSizes(size_t cacheSizes[], size_t length)
{
    accountant_->useCacheSizes(cacheSizes, length);
}

void GlobalSimpleStringMemoryAccountant::start()
{
    if (allocator_ != NULLPTR)
      return;

    allocator_ = new AccountingTestMemoryAllocator(*accountant_, SimpleString::getStringAllocator());

    SimpleString::setStringAllocator(allocator_);
}

void GlobalSimpleStringMemoryAccountant::stop()
{
    if (allocator_ == NULLPTR)
        FAIL("Global SimpleString allocator stopped without starting");

    if (SimpleString::getStringAllocator() != allocator_)
      FAIL("GlobalStrimpleStringMemoryAccountant: allocator has changed between start and stop!");

    restoreAllocator();
}

SimpleString GlobalSimpleStringMemoryAccountant::report()
{
    return accountant_->report();
}

AccountingTestMemoryAllocator* GlobalSimpleStringMemoryAccountant::getAllocator()
{
    return allocator_;
}

TestMemoryAllocator* SimpleString::stringAllocator_ = NULLPTR;

TestMemoryAllocator* SimpleString::getStringAllocator()
{
    if (stringAllocator_ == NULLPTR)
        return defaultNewArrayAllocator();
    return stringAllocator_;
}

void SimpleString::setStringAllocator(TestMemoryAllocator* allocator)
{
    stringAllocator_ = allocator;
}

/* Avoid using the memory leak detector INSIDE SimpleString as its used inside the detector */
char* SimpleString::allocStringBuffer(size_t _size, const char* file, size_t line)
{
    return getStringAllocator()->alloc_memory(_size, file, line);
}

void SimpleString::deallocStringBuffer(char* str, size_t size, const char* file, size_t line)
{
    getStringAllocator()->free_memory(str, size, file, line);
}

char* SimpleString::getEmptyString() const
{
    char* empty = allocStringBuffer(1, __FILE__, __LINE__);
    empty[0] = '\0';
    return empty;
}

// does not support + or - prefixes
unsigned SimpleString::AtoU(const char* str)
{
    while (isSpace(*str)) str++;

    unsigned result = 0;
    for(; isDigit(*str) && *str >= '0'; str++)
    {
        result *= 10;
        result += static_cast<unsigned>(*str - '0');
    }
    return result;
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
   while(*s1 && *s1 == *s2) {
       ++s1;
       ++s2;
   }
   return *(const unsigned char *) s1 - *(const unsigned char *) s2;
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
        --n;
        ++s1;
        ++s2;
    }
    return n ? *(const unsigned char *) s1 - *(const unsigned char *) s2 : 0;
}

char* SimpleString::StrNCpy(char* s1, const char* s2, size_t n)
{
    char* result = s1;

    if((NULLPTR == s1) || (0 == n)) return result;

    *s1 = *s2;
    while ((--n != 0) && *s1){
        *++s1 = *++s2;
    }
    return result;
}

const char* SimpleString::StrStr(const char* s1, const char* s2)
{
    if(!*s2) return s1;
    for (; *s1; s1++)
        if (StrNCmp(s1, s2, StrLen(s2)) == 0)
            return s1;
    return NULLPTR;
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
        if (*p1 != *p2) {
            return *p1 - *p2;
        } else {
            ++p1;
            ++p2;
        }
    return 0;
}

void SimpleString::deallocateInternalBuffer()
{
    if (buffer_) {
        deallocStringBuffer(buffer_, bufferSize_, __FILE__, __LINE__);
        buffer_ = NULLPTR;
        bufferSize_ = 0;
    }
}

void SimpleString::setInternalBufferAsEmptyString()
{
    deallocateInternalBuffer();

    bufferSize_ = 1;
    buffer_ = getEmptyString();
}

void SimpleString::copyBufferToNewInternalBuffer(const char* otherBuffer, size_t bufferSize)
{
    deallocateInternalBuffer();

    bufferSize_ = bufferSize;
    buffer_ = copyToNewBuffer(otherBuffer, bufferSize_);
}

void SimpleString::setInternalBufferToNewBuffer(size_t bufferSize)
{
    deallocateInternalBuffer();

    bufferSize_ = bufferSize;
    buffer_ = allocStringBuffer(bufferSize_, __FILE__, __LINE__);
    buffer_[0] = '\0';
}

void SimpleString::setInternalBufferTo(char* buffer, size_t bufferSize)
{
    deallocateInternalBuffer();

    bufferSize_ = bufferSize;
    buffer_ = buffer;
}

void SimpleString::copyBufferToNewInternalBuffer(const SimpleString& otherBuffer)
{
    copyBufferToNewInternalBuffer(otherBuffer.buffer_, otherBuffer.size() + 1);
}

void SimpleString::copyBufferToNewInternalBuffer(const char* otherBuffer)
{
    copyBufferToNewInternalBuffer(otherBuffer, StrLen(otherBuffer) + 1);
}

const char* SimpleString::getBuffer() const
{
    return buffer_;
}

SimpleString::SimpleString(const char *otherBuffer)
    : buffer_(NULLPTR), bufferSize_(0)
{
    if (otherBuffer == NULLPTR)
        setInternalBufferAsEmptyString();
    else
        copyBufferToNewInternalBuffer(otherBuffer);
}

SimpleString::SimpleString(const char *other, size_t repeatCount)
    : buffer_(NULLPTR), bufferSize_(0)
{
    size_t otherStringLength = StrLen(other);
    setInternalBufferToNewBuffer(otherStringLength * repeatCount + 1);

    char* next = buffer_;
    for (size_t i = 0; i < repeatCount; i++) {
        StrNCpy(next, other, otherStringLength + 1);
        next += otherStringLength;
    }
    *next = 0;
}

SimpleString::SimpleString(const SimpleString& other)
    : buffer_(NULLPTR), bufferSize_(0)
{
    copyBufferToNewInternalBuffer(other.getBuffer());
}

SimpleString& SimpleString::operator=(const SimpleString& other)
{
    if (this != &other)
        copyBufferToNewInternalBuffer(other);
    return *this;
}

bool SimpleString::contains(const SimpleString& other) const
{
    return StrStr(getBuffer(), other.getBuffer()) != NULLPTR;
}

bool SimpleString::containsNoCase(const SimpleString& other) const
{
    return lowerCase().contains(other.lowerCase());
}

bool SimpleString::startsWith(const SimpleString& other) const
{
    if (other.size() == 0) return true;
    else if (size() == 0) return false;
    else return StrStr(getBuffer(), other.getBuffer()) == getBuffer();
}

bool SimpleString::endsWith(const SimpleString& other) const
{
    size_t length = size();
    size_t other_length = other.size();

    if (other_length == 0) return true;
    if (length == 0) return false;
    if (length < other_length) return false;

    return StrCmp(getBuffer() + length - other_length, other.getBuffer()) == 0;
}

size_t SimpleString::count(const SimpleString& substr) const
{
    size_t num = 0;
    const char* str = getBuffer();
    while (*str && (str = StrStr(str, substr.getBuffer()))) {
        str++;
        num++;
    }
    return num;
}

void SimpleString::split(const SimpleString& delimiter, SimpleStringCollection& col) const
{
    size_t num = count(delimiter);
    size_t extraEndToken = (endsWith(delimiter)) ? 0 : 1U;
    col.allocate(num + extraEndToken);

    const char* str = getBuffer();
    const char* prev;
    for (size_t i = 0; i < num; ++i) {
        prev = str;
        str = StrStr(str, delimiter.getBuffer()) + 1;
        col[i] = SimpleString(prev).subString(0, size_t (str - prev));
    }
    if (extraEndToken) {
        col[num] = str;
    }
}

void SimpleString::replace(char to, char with)
{
    size_t s = size();
    for (size_t i = 0; i < s; i++) {
        if (getBuffer()[i] == to) buffer_[i] = with;
    }
}

void SimpleString::replace(const char* to, const char* with)
{
    size_t c = count(to);
    if (c == 0) {
        return;
    }
    size_t len = size();
    size_t tolen = StrLen(to);
    size_t withlen = StrLen(with);

    size_t newsize = len + (withlen * c) - (tolen * c) + 1;

    if (newsize > 1) {
        char* newbuf = allocStringBuffer(newsize, __FILE__, __LINE__);
        for (size_t i = 0, j = 0; i < len;) {
            if (StrNCmp(&getBuffer()[i], to, tolen) == 0) {
                StrNCpy(&newbuf[j], with, withlen + 1);
                j += withlen;
                i += tolen;
            }
            else {
                newbuf[j] = getBuffer()[i];
                j++;
                i++;
            }
        }
        newbuf[newsize - 1] = '\0';
        setInternalBufferTo(newbuf, newsize);
    }
    else
        setInternalBufferAsEmptyString();
}

SimpleString SimpleString::printable() const
{
    static const char* shortEscapeCodes[] =
    {
        "\\a",
        "\\b",
        "\\t",
        "\\n",
        "\\v",
        "\\f",
        "\\r"
    };

    SimpleString result;
    result.setInternalBufferToNewBuffer(getPrintableSize() + 1);

    size_t str_size = size();
    size_t j = 0;
    for (size_t i = 0; i < str_size; i++)
    {
        char c = buffer_[i];
        if (isControlWithShortEscapeSequence(c))
        {
            StrNCpy(&result.buffer_[j], shortEscapeCodes[(unsigned char)(c - '\a')], 2);
            j += 2;
        }
        else if (isControl(c))
        {
            SimpleString hexEscapeCode = StringFromFormat("\\x%02X ", c);
            StrNCpy(&result.buffer_[j], hexEscapeCode.asCharString(), 4);
            j += 4;
        }
        else
        {
            result.buffer_[j] = c;
            j++;
        }
    }
    result.buffer_[j] = 0;

    return result;
}

size_t SimpleString::getPrintableSize() const
{
    size_t str_size = size();
    size_t printable_str_size = str_size;

    for (size_t i = 0; i < str_size; i++)
    {
        char c = buffer_[i];
        if (isControlWithShortEscapeSequence(c))
        {
            printable_str_size += 1;
        }
        else if (isControl(c))
        {
            printable_str_size += 3;
        }
    }

    return printable_str_size;
}

SimpleString SimpleString::lowerCase() const
{
    SimpleString str(*this);

    size_t str_size = str.size();
    for (size_t i = 0; i < str_size; i++)
        str.buffer_[i] = ToLower(str.getBuffer()[i]);

    return str;
}

const char *SimpleString::asCharString() const
{
    return getBuffer();
}

size_t SimpleString::size() const
{
    return StrLen(getBuffer());
}

bool SimpleString::isEmpty() const
{
    return size() == 0;
}

SimpleString::~SimpleString()
{
    deallocateInternalBuffer();
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

SimpleString SimpleString::operator+(const SimpleString& rhs) const
{
    SimpleString t(getBuffer());
    t += rhs.getBuffer();
    return t;
}

SimpleString& SimpleString::operator+=(const SimpleString& rhs)
{
    return operator+=(rhs.getBuffer());
}

SimpleString& SimpleString::operator+=(const char* rhs)
{
    size_t originalSize = this->size();
    size_t additionalStringSize = StrLen(rhs) + 1;
    size_t sizeOfNewString = originalSize + additionalStringSize;
    char* tbuffer = copyToNewBuffer(this->getBuffer(), sizeOfNewString);
    StrNCpy(tbuffer + originalSize, rhs, additionalStringSize);

    setInternalBufferTo(tbuffer, sizeOfNewString);
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

    SimpleString newString = getBuffer() + beginPos;

    if (newString.size() > amount)
        newString.buffer_[amount] = '\0';

    return newString;
}

SimpleString SimpleString::subString(size_t beginPos) const
{
    return subString(beginPos, npos);
}

char SimpleString::at(size_t pos) const
{
    return getBuffer()[pos];
}

size_t SimpleString::find(char ch) const
{
    return findFrom(0, ch);
}

size_t SimpleString::findFrom(size_t starting_position, char ch) const
{
    size_t length = size();
    for (size_t i = starting_position; i < length; i++)
        if (at(i) == ch) return i;
    return npos;
}

SimpleString SimpleString::subStringFromTill(char startChar, char lastExcludedChar) const
{
    size_t beginPos = find(startChar);
    if (beginPos == npos) return "";

    size_t endPos = findFrom(beginPos, lastExcludedChar);
    if (endPos == npos) return subString(beginPos);

    return subString(beginPos, endPos - beginPos);
}

char* SimpleString::copyToNewBuffer(const char* bufferToCopy, size_t bufferSize)
{
    char* newBuffer = allocStringBuffer(bufferSize, __FILE__, __LINE__);
    StrNCpy(newBuffer, bufferToCopy, bufferSize);
    newBuffer[bufferSize-1] = '\0';
    return newBuffer;
}


void SimpleString::copyToBuffer(char* bufferToCopy, size_t bufferSize) const
{
    if (bufferToCopy == NULLPTR || bufferSize == 0) return;

    size_t sizeToCopy = (bufferSize-1 < size()) ? (bufferSize-1) : size();

    StrNCpy(bufferToCopy, getBuffer(), sizeToCopy);
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

bool SimpleString::isControl(char ch)
{
    return ch < ' ' || ch == char(0x7F);
}

bool SimpleString::isControlWithShortEscapeSequence(char ch)
{
    return '\a' <= ch && '\r' >= ch;
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

SimpleString PrintableStringFromOrNull(const char * expected)
{
    return (expected) ? StringFrom(expected).printable() : "(null)";
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

SimpleString StringFrom(void (*value)())
{
    return SimpleString("0x") + HexStringFrom(value);
}

SimpleString HexStringFrom(long value)
{
    return StringFromFormat("%lx", value);
}

SimpleString HexStringFrom(int value)
{
    return StringFromFormat("%x", value);
}

SimpleString HexStringFrom(signed char value)
{
    SimpleString result = StringFromFormat("%x", value);
    if(value < 0) {
        size_t size = result.size();
        result = result.subString(size-(CPPUTEST_CHAR_BIT/4));
    }
    return result;
}

SimpleString HexStringFrom(unsigned long value)
{
    return StringFromFormat("%lx", value);
}

SimpleString HexStringFrom(unsigned int value)
{
    return StringFromFormat("%x", value);
}

SimpleString BracketsFormattedHexStringFrom(int value)
{
    return BracketsFormattedHexString(HexStringFrom(value));
}

SimpleString BracketsFormattedHexStringFrom(unsigned int value)
{
    return BracketsFormattedHexString(HexStringFrom(value));
}

SimpleString BracketsFormattedHexStringFrom(long value)
{
    return BracketsFormattedHexString(HexStringFrom(value));
}


SimpleString BracketsFormattedHexStringFrom(unsigned long value)
{
    return BracketsFormattedHexString(HexStringFrom(value));
}

SimpleString BracketsFormattedHexStringFrom(signed char value)
{
    return BracketsFormattedHexString(HexStringFrom(value));
}

SimpleString BracketsFormattedHexString(SimpleString hexString)
{
    return SimpleString("(0x") + hexString + ")" ;
}

/*
 * ARM compiler has only partial support for C++11.
 * Specifically nullptr_t is not officially supported
 */
#if __cplusplus > 199711L && !defined __arm__ && CPPUTEST_USE_STD_CPP_LIB
SimpleString StringFrom(const std::nullptr_t value)
{
    (void) value;
    return "(null)";
}
#endif

#ifdef CPPUTEST_USE_LONG_LONG

SimpleString StringFrom(cpputest_longlong value)
{
    return StringFromFormat("%lld", value);
}

SimpleString StringFrom(cpputest_ulonglong value)
{
    return StringFromFormat("%llu", value);
}

SimpleString HexStringFrom(cpputest_longlong value)
{
    return StringFromFormat("%llx", value);
}

SimpleString HexStringFrom(cpputest_ulonglong value)
{
    return StringFromFormat("%llx", value);
}

SimpleString HexStringFrom(const void* value)
{
    return HexStringFrom((cpputest_ulonglong) value);
}

SimpleString HexStringFrom(void (*value)())
{
    return HexStringFrom((cpputest_ulonglong) value);
}

SimpleString BracketsFormattedHexStringFrom(cpputest_longlong value)
{
    return BracketsFormattedHexString(HexStringFrom(value));
}


SimpleString BracketsFormattedHexStringFrom(cpputest_ulonglong value)
{
    return BracketsFormattedHexString(HexStringFrom(value));
}

#else   /* CPPUTEST_USE_LONG_LONG */

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

static long convertFunctionPointerToLongValue(void (*value)())
{
    /*
     * This way of converting also can convert a 64bit pointer in a 32bit integer by truncating.
     * This isn't the right way to convert pointers values and need to change by implementing a
     * proper portable way to convert pointers to strings.
     */
    long* long_value = (long*) &value;
    return *long_value;
}

SimpleString StringFrom(cpputest_longlong)
{
    return "<longlong_unsupported>";
}

SimpleString StringFrom(cpputest_ulonglong)
{
    return "<ulonglong_unsupported>";
}

SimpleString HexStringFrom(cpputest_longlong)
{
    return "<longlong_unsupported>";
}

SimpleString HexStringFrom(cpputest_ulonglong)
{
    return "<ulonglong_unsupported>";
}

SimpleString HexStringFrom(const void* value)
{
    return StringFromFormat("%lx", convertPointerToLongValue(value));
}

SimpleString HexStringFrom(void (*value)())
{
    return StringFromFormat("%lx", convertFunctionPointerToLongValue(value));
}

SimpleString BracketsFormattedHexStringFrom(cpputest_longlong)
{
    return "";
}


SimpleString BracketsFormattedHexStringFrom(cpputest_ulonglong)
{
    return "";
}

#endif  /* CPPUTEST_USE_LONG_LONG */

SimpleString StringFrom(double value, int precision)
{
    if (PlatformSpecificIsNan(value))
        return "Nan - Not a number";
    else if (PlatformSpecificIsInf(value))
        return "Inf - Infinity";
    else
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
    return StringFromFormat("%u", i);
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
    return StringFromFormat("%lu", i);
}

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
        char* newBuffer = SimpleString::allocStringBuffer(newBufferSize, __FILE__, __LINE__);
        PlatformSpecificVSNprintf(newBuffer, newBufferSize, format, argsCopy);
        resultString = SimpleString(newBuffer);

        SimpleString::deallocStringBuffer(newBuffer, newBufferSize, __FILE__, __LINE__);
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

SimpleString StringFromBinaryWithSize(const unsigned char* value, size_t size)
{
    SimpleString result = StringFromFormat("Size = %u | HexContents = ", (unsigned) size);
    size_t displayedSize = ((size > 128) ? 128 : size);
    result += StringFromBinaryOrNull(value, displayedSize);
    if (size > displayedSize)
    {
        result += " ...";
    }
    return result;
}

SimpleString StringFromBinaryWithSizeOrNull(const unsigned char* value, size_t size)
{
    return (value) ? StringFromBinaryWithSize(value, size) : "(null)";
}

SimpleString StringFromMaskedBits(unsigned long value, unsigned long mask, size_t byteCount)
{
    SimpleString result;
    size_t bitCount = (byteCount > sizeof(unsigned long)) ? (sizeof(unsigned long) * CPPUTEST_CHAR_BIT) : (byteCount * CPPUTEST_CHAR_BIT);
    const unsigned long msbMask = (((unsigned long) 1) << (bitCount - 1));

    for (size_t i = 0; i < bitCount; i++) {
        if (mask & msbMask) {
            result += (value & msbMask) ? "1" : "0";
        }
        else {
            result += "x";
        }

        if (((i % 8) == 7) && (i != (bitCount - 1))) {
            result += " ";
        }

        value <<= 1;
        mask <<= 1;
    }

    return result;
}

SimpleString StringFromOrdinalNumber(unsigned int number)
{
    unsigned int onesDigit = number % 10;

    const char* suffix;
    if (number >= 11 && number <= 13) {
        suffix = "th";
    } else if (3 == onesDigit) {
        suffix = "rd";
    } else if (2 == onesDigit) {
        suffix = "nd";
    } else if (1 == onesDigit) {
        suffix = "st";
    } else {
        suffix = "th";
    }

    return StringFromFormat("%u%s", number, suffix);
}

SimpleStringCollection::SimpleStringCollection()
{
    collection_ = NULLPTR;
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
