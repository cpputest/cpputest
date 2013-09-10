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
SimpleString::SimpleString(const char *otherBuffer)
{
	if (otherBuffer == 0) {
		buffer_ = getEmptyString();
	}
	else {
		size_t len = PlatformSpecificStrLen(otherBuffer) + 1;
		buffer_ = allocStringBuffer(len);
		PlatformSpecificStrCpy(buffer_, otherBuffer);
	}
}

SimpleString::SimpleString(const char *other, size_t repeatCount)
{
	size_t len = PlatformSpecificStrLen(other) * repeatCount + 1;
	buffer_ = allocStringBuffer(len);
	char* next = buffer_;
	for (size_t i = 0; i < repeatCount; i++) {
		PlatformSpecificStrCpy(next, other);
		next += PlatformSpecificStrLen(other);
	}
	*next = 0;

}
SimpleString::SimpleString(const SimpleString& other)
{
	size_t len = other.size() + 1;
	buffer_ = allocStringBuffer(len);
	PlatformSpecificStrCpy(buffer_, other.buffer_);
}

SimpleString& SimpleString::operator=(const SimpleString& other)
{
	if (this != &other) {
		deallocStringBuffer(buffer_);
		size_t len = other.size() + 1;
		buffer_ = allocStringBuffer(len);
		PlatformSpecificStrCpy(buffer_, other.buffer_);
	}
	return *this;
}

bool SimpleString::contains(const SimpleString& other) const
{
	//strstr on some machines does not handle ""
	//the right way.  "" should be found in any string
	if (PlatformSpecificStrLen(other.buffer_) == 0) return true;
	else if (PlatformSpecificStrLen(buffer_) == 0) return false;
	else return PlatformSpecificStrStr(buffer_, other.buffer_) != 0;
}

bool SimpleString::containsNoCase(const SimpleString& other) const
{
	return toLower().contains(other.toLower());
}


bool SimpleString::startsWith(const SimpleString& other) const
{
	if (PlatformSpecificStrLen(other.buffer_) == 0) return true;
	else if (PlatformSpecificStrLen(buffer_) == 0) return false;
	else return PlatformSpecificStrStr(buffer_, other.buffer_) == buffer_;
}

bool SimpleString::endsWith(const SimpleString& other) const
{
	size_t buffer_length = PlatformSpecificStrLen(buffer_);
	size_t other_buffer_length = PlatformSpecificStrLen(other.buffer_);
	if (other_buffer_length == 0) return true;
	if (buffer_length == 0) return false;
	if (buffer_length < other_buffer_length) return false;
	return PlatformSpecificStrCmp(buffer_ + buffer_length - other_buffer_length, other.buffer_) == 0;
}

size_t SimpleString::count(const SimpleString& substr) const
{
	size_t num = 0;
	char* str = buffer_;
	while ((str = PlatformSpecificStrStr(str, substr.buffer_))) {
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
		str = PlatformSpecificStrStr(str, delimiter.buffer_) + 1;
		size_t len = (size_t) (str - prev);
		char* sub = allocStringBuffer(len + 1);
		PlatformSpecificStrNCpy(sub, prev, len);
		sub[len] = '\0';
		col[i] = sub;
		deallocStringBuffer(sub);
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
	size_t tolen = PlatformSpecificStrLen(to);
	size_t withlen = PlatformSpecificStrLen(with);

	size_t newsize = len + (withlen * c) - (tolen * c) + 1;

	if (newsize) {
		char* newbuf = allocStringBuffer(newsize);
		for (size_t i = 0, j = 0; i < len;) {
			if (PlatformSpecificStrNCmp(&buffer_[i], to, tolen) == 0) {
				PlatformSpecificStrNCpy(&newbuf[j], with, withlen);
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

SimpleString SimpleString::toLower() const
{
	SimpleString str(*this);

	size_t str_size = str.size();
	for (size_t i = 0; i < str_size; i++)
		str.buffer_[i] = PlatformSpecificToLower(str.buffer_[i]);

	return str;
}

const char *SimpleString::asCharString() const
{
	return buffer_;
}

size_t SimpleString::size() const
{
	return PlatformSpecificStrLen(buffer_);
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
	return 0 == PlatformSpecificStrCmp(left.asCharString(), right.asCharString());
}

bool SimpleString::equalsNoCase(const SimpleString& str) const
{
	return toLower() == str.toLower();
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
	size_t len = this->size() + PlatformSpecificStrLen(rhs) + 1;
	char* tbuffer = allocStringBuffer(len);
	PlatformSpecificStrCpy(tbuffer, this->buffer_);
	PlatformSpecificStrCat(tbuffer, rhs);
	deallocStringBuffer(buffer_);
	buffer_ = tbuffer;
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


void SimpleString::copyToBuffer(char* bufferToCopy, size_t bufferSize) const
{
	if (bufferToCopy == NULL || bufferSize == 0) return;

	size_t sizeToCopy = (bufferSize-1 < size()) ? bufferSize-1 : size();

	PlatformSpecificStrNCpy(bufferToCopy, buffer_, sizeToCopy);
	bufferToCopy[sizeToCopy] = '\0';

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

#if CPPUTEST_USE_STD_CPP_LIB

#include <string>

SimpleString StringFrom(const std::string& value)
{
	return SimpleString(value.c_str());
}

SimpleString StringFrom(unsigned long i)
{
	return StringFromFormat("%lu (0x%lx)", i, i);
}

SimpleString StringFrom(uint32_t i)
{
	return StringFromFormat("%10u (0x%08x)", i, i);
}

SimpleString StringFrom(uint16_t i)
{
	return StringFromFormat("%5u (0x%04x)", i, i);
}

SimpleString StringFrom(uint8_t i)
{
	return StringFromFormat("%3u (0x%02x)", i, i);
}

#endif

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

SimpleStringCollection::SimpleStringCollection()
{
	collection_ = 0;
	size_ = 0;
}

void SimpleStringCollection::allocate(size_t _size)
{
	if (collection_) delete[] collection_;

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

