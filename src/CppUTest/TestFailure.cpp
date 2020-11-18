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

TestFailure::TestFailure(UtestShell* test, const char* fileName, size_t lineNumber, const SimpleString& theMessage) :
    testName_(test->getFormattedName()), testNameOnly_(test->getName()), fileName_(fileName), lineNumber_(lineNumber), testFileName_(test->getFile()), testLineNumber_(test->getLineNumber()), message_(theMessage)
{
}

TestFailure::TestFailure(UtestShell* test, const SimpleString& theMessage) :
    testName_(test->getFormattedName()), testNameOnly_(test->getName()), fileName_(test->getFile()), lineNumber_(test->getLineNumber()), testFileName_(test->getFile()), testLineNumber_(test->getLineNumber()), message_(theMessage)
{
}

TestFailure::TestFailure(UtestShell* test, const char* fileName, size_t lineNum) :
    testName_(test->getFormattedName()), testNameOnly_(test->getName()), fileName_(fileName), lineNumber_(lineNum), testFileName_(test->getFile()), testLineNumber_(test->getLineNumber()), message_("no message")
{
}

TestFailure::TestFailure(const TestFailure& f) :
    testName_(f.testName_), testNameOnly_(f.testNameOnly_), fileName_(f.fileName_), lineNumber_(f.lineNumber_), testFileName_(f.testFileName_), testLineNumber_(f.testLineNumber_), message_(f.message_)
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

SimpleString TestFailure::getTestNameOnly() const
{
    return testNameOnly_;
}

size_t TestFailure::getFailureLineNumber() const
{
    return lineNumber_;
}

size_t TestFailure::getTestLineNumber() const
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

SimpleString TestFailure::createDifferenceAtPosString(const SimpleString& actual, size_t offset, size_t reportedPosition)
{
    SimpleString result;
    const size_t extraCharactersWindow = 20;
    const size_t halfOfExtraCharactersWindow = extraCharactersWindow / 2;

    SimpleString paddingForPreventingOutOfBounds (" ", halfOfExtraCharactersWindow);
    SimpleString actualString = paddingForPreventingOutOfBounds + actual + paddingForPreventingOutOfBounds;
    SimpleString differentString = StringFromFormat("difference starts at position %lu at: <", (unsigned long) reportedPosition);

    result += "\n";
    result += StringFromFormat("\t%s%s>\n", differentString.asCharString(), actualString.subString(offset, extraCharactersWindow).asCharString());


    result += StringFromFormat("\t%s^", SimpleString(" ", (differentString.size() + halfOfExtraCharactersWindow)).asCharString());
    return result;
}

SimpleString TestFailure::createUserText(const SimpleString& text)
{
    SimpleString userMessage = "";
    if (!text.isEmpty())
    {
        //This is a kludge to turn off "Message: " for this case.
        //I don't think "Message: " adds anything, as you get to see the
        //message. I propose we remove "Message: " lead in
        if (!text.startsWith("LONGS_EQUAL"))
            userMessage += "Message: ";
        userMessage += text;
        userMessage += "\n\t";
    }
    return userMessage;
}

EqualsFailure::EqualsFailure(UtestShell* test, const char* fileName, size_t lineNumber, const char* expected, const char* actual, const SimpleString& text) :
    TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    message_ += createButWasString(StringFromOrNull(expected), StringFromOrNull(actual));
}

EqualsFailure::EqualsFailure(UtestShell* test, const char* fileName, size_t lineNumber, const SimpleString& expected, const SimpleString& actual, const SimpleString& text)
    : TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    message_ += createButWasString(expected, actual);
}

DoublesEqualFailure::DoublesEqualFailure(UtestShell* test, const char* fileName, size_t lineNumber, double expected, double actual, double threshold, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    message_ += createButWasString(StringFrom(expected, 7), StringFrom(actual, 7));
    message_ += " threshold used was <";
    message_ += StringFrom(threshold, 7);
    message_ += ">";

    if (PlatformSpecificIsNan(expected) || PlatformSpecificIsNan(actual) || PlatformSpecificIsNan(threshold))
        message_ += "\n\tCannot make comparisons with Nan";
}

CheckEqualFailure::CheckEqualFailure(UtestShell* test, const char* fileName, size_t lineNumber, const SimpleString& expected, const SimpleString& actual, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    SimpleString printableExpected = PrintableStringFromOrNull(expected.asCharString());
    SimpleString printableActual = PrintableStringFromOrNull(actual.asCharString());

    message_ += createButWasString(printableExpected, printableActual);

    size_t failStart;
    for (failStart = 0; actual.at(failStart) == expected.at(failStart); failStart++)
        ;
    size_t failStartPrintable;
    for (failStartPrintable = 0; printableActual.at(failStartPrintable) == printableExpected.at(failStartPrintable); failStartPrintable++)
        ;
    message_ += createDifferenceAtPosString(printableActual, failStartPrintable, failStart);
}

ComparisonFailure::ComparisonFailure(UtestShell *test, const char *fileName, size_t lineNumber, const SimpleString& checkString, const SimpleString &comparisonString, const SimpleString &text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);
    message_ += checkString;
    message_ += "(";
    message_ += comparisonString;
    message_ += ") failed";
}

ContainsFailure::ContainsFailure(UtestShell* test, const char* fileName, size_t lineNumber, const SimpleString& expected, const SimpleString& actual, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    message_ += StringFromFormat("actual <%s>\n\tdid not contain  <%s>", actual.asCharString(), expected.asCharString());
}

CheckFailure::CheckFailure(UtestShell* test, const char* fileName, size_t lineNumber, const SimpleString& checkString, const SimpleString& conditionString, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    message_ += checkString;
    message_ += "(";
    message_ += conditionString;
    message_ += ") failed";
}

FailFailure::FailFailure(UtestShell* test, const char* fileName, size_t lineNumber, const SimpleString& message) : TestFailure(test, fileName, lineNumber)
{
    message_ = message;
}

LongsEqualFailure::LongsEqualFailure(UtestShell* test, const char* fileName, size_t lineNumber, long expected, long actual, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    SimpleString aDecimal = StringFrom(actual);
    SimpleString eDecimal = StringFrom(expected);

    SimpleString::padStringsToSameLength(aDecimal, eDecimal, ' ');

    SimpleString actualReported = aDecimal + " " + BracketsFormattedHexStringFrom(actual);
    SimpleString expectedReported = eDecimal + " " + BracketsFormattedHexStringFrom(expected);
    message_ += createButWasString(expectedReported, actualReported);
}

UnsignedLongsEqualFailure::UnsignedLongsEqualFailure(UtestShell* test, const char* fileName, size_t lineNumber, unsigned long expected, unsigned long actual, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    SimpleString aDecimal = StringFrom(actual);
    SimpleString eDecimal = StringFrom(expected);

    SimpleString::padStringsToSameLength(aDecimal, eDecimal, ' ');

    SimpleString actualReported = aDecimal + " " + BracketsFormattedHexStringFrom(actual);
    SimpleString expectedReported = eDecimal + " " + BracketsFormattedHexStringFrom(expected);

    message_ += createButWasString(expectedReported, actualReported);
}

LongLongsEqualFailure::LongLongsEqualFailure(UtestShell* test, const char* fileName, size_t lineNumber, cpputest_longlong expected, cpputest_longlong actual, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    SimpleString aDecimal = StringFrom(actual);
    SimpleString eDecimal = StringFrom(expected);

    SimpleString::padStringsToSameLength(aDecimal, eDecimal, ' ');

    SimpleString actualReported = aDecimal + " " + BracketsFormattedHexStringFrom(actual);
    SimpleString expectedReported = eDecimal + " " + BracketsFormattedHexStringFrom(expected);
    message_ += createButWasString(expectedReported, actualReported);
}

UnsignedLongLongsEqualFailure::UnsignedLongLongsEqualFailure(UtestShell* test, const char* fileName, size_t lineNumber, cpputest_ulonglong expected, cpputest_ulonglong actual, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    SimpleString aDecimal = StringFrom(actual);
    SimpleString eDecimal = StringFrom(expected);

    SimpleString::padStringsToSameLength(aDecimal, eDecimal, ' ');

    SimpleString actualReported = aDecimal + " " + BracketsFormattedHexStringFrom(actual);
    SimpleString expectedReported = eDecimal + " " + BracketsFormattedHexStringFrom(expected);
    message_ += createButWasString(expectedReported, actualReported);
}

SignedBytesEqualFailure::SignedBytesEqualFailure (UtestShell* test, const char* fileName, size_t lineNumber, signed char expected, signed char actual, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    SimpleString aDecimal = StringFrom((int)actual);
    SimpleString eDecimal = StringFrom((int)expected);

    SimpleString::padStringsToSameLength(aDecimal, eDecimal, ' ');

    SimpleString actualReported = aDecimal + " " + BracketsFormattedHexStringFrom(actual);
    SimpleString expectedReported = eDecimal + " " + BracketsFormattedHexStringFrom(expected);
    message_ += createButWasString(expectedReported, actualReported);
}

StringEqualFailure::StringEqualFailure(UtestShell* test, const char* fileName, size_t lineNumber, const char* expected, const char* actual, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    SimpleString printableExpected = PrintableStringFromOrNull(expected);
    SimpleString printableActual = PrintableStringFromOrNull(actual);

    message_ += createButWasString(printableExpected, printableActual);
    if((expected) && (actual))
    {
        size_t failStart;
        for (failStart = 0; actual[failStart] == expected[failStart]; failStart++)
            ;
        size_t failStartPrintable;
        for (failStartPrintable = 0; printableActual.at(failStartPrintable) == printableExpected.at(failStartPrintable); failStartPrintable++)
            ;
        message_ += createDifferenceAtPosString(printableActual, failStartPrintable, failStart);
    }
}

StringEqualNoCaseFailure::StringEqualNoCaseFailure(UtestShell* test, const char* fileName, size_t lineNumber, const char* expected, const char* actual, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    SimpleString printableExpected = PrintableStringFromOrNull(expected);
    SimpleString printableActual = PrintableStringFromOrNull(actual);

    message_ += createButWasString(printableExpected, printableActual);
    if((expected) && (actual))
    {
        size_t failStart;
        for (failStart = 0; SimpleString::ToLower(actual[failStart]) == SimpleString::ToLower(expected[failStart]); failStart++)
            ;
        size_t failStartPrintable;
        for (failStartPrintable = 0;
             SimpleString::ToLower(printableActual.at(failStartPrintable)) == SimpleString::ToLower(printableExpected.at(failStartPrintable));
             failStartPrintable++)
            ;
        message_ += createDifferenceAtPosString(printableActual, failStartPrintable, failStart);
    }
}

BinaryEqualFailure::BinaryEqualFailure(UtestShell* test, const char* fileName, size_t lineNumber, const unsigned char* expected,
                                       const unsigned char* actual, size_t size, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    SimpleString actualHex = StringFromBinaryOrNull(actual, size);

	message_ += createButWasString(StringFromBinaryOrNull(expected, size), actualHex);
	if ((expected) && (actual))
	{
		size_t failStart;
		for (failStart = 0; actual[failStart] == expected[failStart]; failStart++)
			;
		message_ += createDifferenceAtPosString(actualHex, (failStart * 3 + 1), failStart);
	}
}

BitsEqualFailure::BitsEqualFailure(UtestShell* test, const char* fileName, size_t lineNumber, unsigned long expected, unsigned long actual,
                                   unsigned long mask, size_t byteCount, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    message_ += createButWasString(StringFromMaskedBits(expected, mask, byteCount), StringFromMaskedBits(actual, mask, byteCount));
}

FeatureUnsupportedFailure::FeatureUnsupportedFailure(UtestShell* test, const char* fileName, size_t lineNumber,
                                                     const SimpleString& featureName, const SimpleString& text)
: TestFailure(test, fileName, lineNumber)
{
    message_ = createUserText(text);

    message_ += StringFromFormat("The feature \"%s\" is not supported in this environment or with the feature set selected when building the library.", featureName.asCharString());
}
