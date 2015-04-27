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
#include "CppUTest/TestFailure.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/SimpleString.h"
#include "CppUTest/PlatformSpecificFunctions.h"

static SimpleString removeAllPrintableCharactersFrom(const SimpleString& str)
{
    size_t bufferSize = str.size()+1;
    char* buffer = (char*) PlatformSpecificMalloc(bufferSize);
    str.copyToBuffer(buffer, bufferSize);

    for (size_t i = 0; i < bufferSize-1; i++)
        if (buffer[i] != '\t' && buffer[i] != '\n')
            buffer[i] = ' ';

    SimpleString result(buffer);
    PlatformSpecificFree(buffer);
    return result;
}

static SimpleString addMarkerToString(const SimpleString& str, int markerPos)
{
    size_t bufferSize = str.size()+1;
    char* buffer = (char*) PlatformSpecificMalloc(bufferSize);
    str.copyToBuffer(buffer, bufferSize);

    buffer[markerPos] = '^';

    SimpleString result(buffer);
    PlatformSpecificFree(buffer);
    return result;

}

TestFailure::TestFailure(UtestShell* test, const char* fileName, int lineNumber, const SimpleString& theMessage) :
    testName_(test->getFormattedName()), fileName_(fileName), lineNumber_(lineNumber), testFileName_(test->getFile()), testLineNumber_(test->getLineNumber()), message_(theMessage)
{
}

TestFailure::TestFailure(UtestShell* test, const SimpleString& theMessage) :
    testName_(test->getFormattedName()), fileName_(test->getFile()), lineNumber_(test->getLineNumber()), testFileName_(test->getFile()), testLineNumber_(test->getLineNumber()), message_(theMessage)
{
}

TestFailure::TestFailure(UtestShell* test, const char* fileName, int lineNum) :
    testName_(test->getFormattedName()), fileName_(fileName), lineNumber_(lineNum), testFileName_(test->getFile()), testLineNumber_(test->getLineNumber()), message_("no message")
{
}

TestFailure::TestFailure(const TestFailure& f) :
    testName_(f.testName_), fileName_(f.fileName_), lineNumber_(f.lineNumber_), testFileName_(f.testFileName_), testLineNumber_(f.testLineNumber_), message_(f.message_)
{
}


TestFailure::~TestFailure()
{
}

SimpleString TestFailure::getFileName() const
{
    return fileName_;
}

SimpleString TestFailure::getTestFileName() const
{
    return testFileName_;
}

SimpleString TestFailure::getTestName() const
{
    return testName_;
}

int TestFailure::getFailureLineNumber() const
{
    return lineNumber_;
}

int TestFailure::getTestLineNumber() const
{
    return testLineNumber_;
}

SimpleString TestFailure::getMessage() const
{
    return message_;
}

bool TestFailure::isOutsideTestFile() const
{
    return testFileName_ != fileName_;
}

bool TestFailure::isInHelperFunction() const
{
    return lineNumber_ < testLineNumber_;
}

SimpleString TestFailure::createButWasString(const SimpleString& expected, const SimpleString& actual)
{
    return StringFromFormat("expected <%s>\n\tbut was  <%s>", expected.asCharString(), actual.asCharString());
}

SimpleString TestFailure::createDifferenceAtPosString(const SimpleString& actual, size_t position, DifferenceFormat format)
{
    SimpleString result;
    const size_t extraCharactersWindow = 20;
    const size_t halfOfExtraCharactersWindow = extraCharactersWindow / 2;
    const size_t actualOffset = (format == DIFFERENCE_STRING) ? position : (position * 3 + 1);

    SimpleString paddingForPreventingOutOfBounds (" ", halfOfExtraCharactersWindow);
    SimpleString actualString = paddingForPreventingOutOfBounds + actual + paddingForPreventingOutOfBounds;
    SimpleString differentString = StringFromFormat("difference starts at position %lu at: <", (unsigned long) position);

    result += "\n";
    result += StringFromFormat("\t%s%s>\n", differentString.asCharString(), actualString.subString(actualOffset, extraCharactersWindow).asCharString());

    SimpleString markString = actualString.subString(actualOffset, halfOfExtraCharactersWindow+1);
    markString = removeAllPrintableCharactersFrom(markString);
    markString = addMarkerToString(markString, halfOfExtraCharactersWindow);

    result += StringFromFormat("\t%s%s", SimpleString(" ", differentString.size()).asCharString(), markString.asCharString());
    return result;
}

EqualsFailure::EqualsFailure(UtestShell* test, const char* fileName, int lineNumber, const char* expected, const char* actual) :
    TestFailure(test, fileName, lineNumber)
{
    message_ = createButWasString(StringFromOrNull(expected), StringFromOrNull(actual));
}

EqualsFailure::EqualsFailure(UtestShell* test, const char* fileName, int lineNumber, const SimpleString& expected, const SimpleString& actual)
    : TestFailure(test, fileName, lineNumber)
{
    message_ = createButWasString(expected, actual);
}

static SimpleString StringFromOrNan(double d)
{
    if (PlatformSpecificIsNan(d))
        return "Nan - Not a number";
    return StringFrom(d);
}

DoublesEqualFailure::DoublesEqualFailure(UtestShell* test, const char* fileName, int lineNumber, double expected, double actual, double threshold)  : TestFailure(test, fileName, lineNumber)
{
    message_ = createButWasString(StringFromOrNan(expected), StringFromOrNan(actual));
    message_ += " threshold used was <";
    message_ += StringFromOrNan(threshold);
    message_ += ">";

    if (PlatformSpecificIsNan(expected) || PlatformSpecificIsNan(actual) || PlatformSpecificIsNan(threshold))
        message_ += "\n\tCannot make comparisons with Nan";
}

CheckEqualFailure::CheckEqualFailure(UtestShell* test, const char* fileName, int lineNumber, const SimpleString& expected, const SimpleString& actual) : TestFailure(test, fileName, lineNumber)
{
    size_t failStart;
    for (failStart = 0; actual.asCharString()[failStart] == expected.asCharString()[failStart]; failStart++)
        ;
    message_ = createButWasString(expected, actual);
    message_ += createDifferenceAtPosString(actual, failStart);

}

ContainsFailure::ContainsFailure(UtestShell* test, const char* fileName, int lineNumber, const SimpleString& expected, const SimpleString& actual) :
    TestFailure(test, fileName, lineNumber)
{
    message_ = StringFromFormat("actual <%s>\n\tdid not contain  <%s>", actual.asCharString(), expected.asCharString());
}

CheckFailure::CheckFailure(UtestShell* test, const char* fileName, int lineNumber, const SimpleString& checkString, const SimpleString& conditionString, const SimpleString& text) : TestFailure(test, fileName, lineNumber)
{
    message_ = "";
    if (!text.isEmpty()) {
        message_ += "Message: ";
        message_ += text;
        message_ += "\n\t";
    }
    message_ += checkString;
    message_ += "(";
    message_ += conditionString;
    message_ += ") failed";
}

FailFailure::FailFailure(UtestShell* test, const char* fileName, int lineNumber, const SimpleString& message) : TestFailure(test, fileName, lineNumber)
{
    message_ = message;
}

LongsEqualFailure::LongsEqualFailure(UtestShell* test, const char* fileName, int lineNumber, long expected, long actual) : TestFailure(test, fileName, lineNumber)
{
    SimpleString aDecimal = StringFrom(actual);
    SimpleString aHex = HexStringFrom(actual);
    SimpleString eDecimal = StringFrom(expected);
    SimpleString eHex = HexStringFrom(expected);

    SimpleString::padStringsToSameLength(aDecimal, eDecimal, ' ');
    SimpleString::padStringsToSameLength(aHex, eHex, '0');

    SimpleString actualReported = aDecimal + " 0x" + aHex;
    SimpleString expectedReported = eDecimal + " 0x" + eHex;
    message_ = createButWasString(expectedReported, actualReported);
}

UnsignedLongsEqualFailure::UnsignedLongsEqualFailure(UtestShell* test, const char* fileName, int lineNumber, unsigned long expected, unsigned long actual) : TestFailure(test, fileName, lineNumber)
{
    SimpleString aDecimal = StringFrom(actual);
    SimpleString aHex = HexStringFrom(actual);
    SimpleString eDecimal = StringFrom(expected);
    SimpleString eHex = HexStringFrom(expected);

    SimpleString::padStringsToSameLength(aDecimal, eDecimal, ' ');
    SimpleString::padStringsToSameLength(aHex, eHex, '0');

    SimpleString actualReported = aDecimal + " 0x" + aHex;
    SimpleString expectedReported = eDecimal + " 0x" + eHex;
    message_ = createButWasString(expectedReported, actualReported);
}

StringEqualFailure::StringEqualFailure(UtestShell* test, const char* fileName, int lineNumber, const char* expected, const char* actual) : TestFailure(test, fileName, lineNumber)
{
    message_ = createButWasString(StringFromOrNull(expected), StringFromOrNull(actual));
    if((expected) && (actual))
    {
        size_t failStart;
        for (failStart = 0; actual[failStart] == expected[failStart]; failStart++)
            ;
        message_ += createDifferenceAtPosString(actual, failStart);
    }
}

StringEqualNoCaseFailure::StringEqualNoCaseFailure(UtestShell* test, const char* fileName, int lineNumber, const char* expected, const char* actual) : TestFailure(test, fileName, lineNumber)
{
    message_ = createButWasString(StringFromOrNull(expected), StringFromOrNull(actual));
    if((expected) && (actual))
    {
        size_t failStart;
        for (failStart = 0; SimpleString::ToLower(actual[failStart]) == SimpleString::ToLower(expected[failStart]); failStart++)
            ;
        message_ += createDifferenceAtPosString(actual, failStart);
    }
}

BinaryEqualFailure::BinaryEqualFailure(UtestShell* test, const char* fileName, int lineNumber, const unsigned char* expected, const unsigned char* actual, size_t size) :
		TestFailure(test, fileName, lineNumber)
{
	message_ = createButWasString(StringFromBinaryOrNull(expected, size), StringFromBinaryOrNull(actual, size));
	if ((expected) && (actual))
	{
		size_t failStart;
		for (failStart = 0; actual[failStart] == expected[failStart]; failStart++)
			;
		message_ += createDifferenceAtPosString(StringFromBinary(actual, size), failStart, DIFFERENCE_BINARY);
	}
}
