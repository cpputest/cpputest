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
#include "CppUTest/PlatformSpecificFunctions.h"

TestFailure::TestFailure(Utest* test, const char* fileName, int lineNumber, const SimpleString& theMessage) :
	testName_(test->getFormattedName()), fileName_(fileName), lineNumber_(lineNumber), message_(theMessage)
{
}

TestFailure::TestFailure(Utest* test, const SimpleString& theMessage) :
	testName_(test->getFormattedName()), fileName_(test->getFile()), lineNumber_(test->getLineNumber()), message_(theMessage)
{
}

TestFailure::TestFailure(Utest* test, const char* fileName, int lineNum) :
	testName_(test->getFormattedName()), fileName_(fileName), lineNumber_(lineNum), message_("no message")
{
}

TestFailure::TestFailure(const TestFailure& f) :
	testName_(f.testName_), fileName_(f.fileName_), lineNumber_(f.lineNumber_), message_(f.message_)
{
}

TestFailure::~TestFailure()
{
}

SimpleString TestFailure::getFileName() const
{
	return fileName_;
}

SimpleString TestFailure::getTestName() const
{
	return testName_;
}

int TestFailure::getLineNumber() const
{
	return lineNumber_;
}

SimpleString TestFailure::getMessage() const
{
	return message_;
}

EqualsFailure::EqualsFailure(Utest* test, const char* fileName, int lineNumber, const char* expected, const char* actual) :
	TestFailure(test, fileName, lineNumber)
{
	const char* format = "expected <%s>\n\tbut was  <%s>";
	message_ = StringFromFormat(format, StringFromOrNull(expected).asCharString(), StringFromOrNull(actual).asCharString());
}

EqualsFailure::EqualsFailure(Utest* test, const char* fileName, int lineNumber, const SimpleString& expected, const SimpleString& actual)
	: TestFailure(test, fileName, lineNumber)
{
	const char* format = "expected <%s>\n\tbut was  <%s>";
	message_ = StringFromFormat(format, expected.asCharString(), actual.asCharString());
}

ContainsFailure::ContainsFailure(Utest* test, const char* fileName, int lineNumber, const SimpleString& expected, const SimpleString& actual) :
	TestFailure(test, fileName, lineNumber)
{

	const char* format = "actual <%s>\n\tdid not contain  <%s>";
	message_ = StringFromFormat(format, actual.asCharString(), expected.asCharString());
}

CheckFailure::CheckFailure(Utest* test, const char* fileName, int lineNumber, const SimpleString& conditionString) : TestFailure(test, fileName, lineNumber)
{
	message_ = "CHECK(";
	message_ += conditionString;
	message_ += ") failed";
}

FailFailure::FailFailure(Utest* test, const char* fileName, int lineNumber, const SimpleString& message) : TestFailure(test, fileName, lineNumber)
{
	message_ = message;
}

LongsEqualFailure::LongsEqualFailure(Utest* test, const char* fileName, int lineNumber, long expected, long actual) : TestFailure(test, fileName, lineNumber)
{
	SimpleString aDecimal = StringFrom(actual);
	SimpleString aHex = HexStringFrom(actual);
	SimpleString eDecimal = StringFrom(expected);
	SimpleString eHex = HexStringFrom(expected);

	SimpleString::padStringsToSameLength(aDecimal, eDecimal, ' ');
	SimpleString::padStringsToSameLength(aHex, eHex, '0');

	SimpleString actualReported = aDecimal + " 0x" + aHex;
	SimpleString expectedReported = eDecimal + " 0x" + eHex;
	message_ = StringFromFormat("expected <%s>\n\tbut was  <%s>", expectedReported.asCharString(), actualReported.asCharString());
}

SimpleString subStringForFailure(const SimpleString& str, int pos)
{
	SimpleString finalString = StringFromFormat("     %s     ", str.asCharString());
	return finalString.subString(pos, 10);
}

StringEqualFailure::StringEqualFailure(Utest* test, const char* fileName, int lineNumber, const char* expected, const char* actual, bool jamesvariant) : TestFailure(test, fileName, lineNumber)
{
	int failStart;
	for (failStart = 0; actual[failStart] == expected[failStart]; failStart++)
		;
	if (jamesvariant) {

		const char * error = "<!>";
		char * message;

		//cpputest_malloc is needed instead of new[] for vc6 compatibility
		message = (char*)cpputest_malloc(PlatformSpecificStrLen(actual) + PlatformSpecificStrLen(error) + 10);

		int j;
		for (j = 0; j < failStart; j++)
			message[j] = actual[j];

		for (int k = 0; k < (int)PlatformSpecificStrLen(error); j++, k++)
			message[j] = error[k];

		for (int i = failStart; actual[i]; i++, j++)
			message[j] = actual[i];

		message[j] = '\0';

		message_ = StringFromFormat("expected <%s>\n\tbut was  <%s>", expected, message);
		cpputest_free(message);
	}
	else {
		message_ = StringFromFormat("expected <%s>\n\tbut was  <%s>\n", expected, actual);

		SimpleString differentString;
		differentString = StringFromFormat("difference starts at position %d at: <", failStart);
		size_t len = differentString.size();
		SimpleString markString;
		markString += "\t";
		markString += SimpleString(" ", len+5);
		markString += "^";

		message_ += "\t";
		message_ += differentString;
		message_ += subStringForFailure(actual, failStart);
		message_ += ">\n";
		message_ += markString;
	}
}

StringEqualNoCaseFailure::StringEqualNoCaseFailure(Utest* test, const char* fileName, int lineNumber, const char* expected, const char* actual) : TestFailure(test, fileName, lineNumber)
{
	int failStart;
    for (failStart = 0; PlatformSpecificToLower(actual[failStart]) == PlatformSpecificToLower(expected[failStart]); failStart++)
    	;

    const char * error = "<!>";
	char * message;

	//cpputest_malloc is needed instead of new[] for vc6 compatibility
	message = (char*)cpputest_malloc(PlatformSpecificStrLen(actual) + PlatformSpecificStrLen(error) + 10);

	int j;
	for (j = 0; j < failStart; j++)
		message[j] = actual[j];

	for (int k = 0; k < (int)PlatformSpecificStrLen(error); j++, k++)
		message[j] = error[k];

	for (int i = failStart; actual[i]; i++, j++)
		message[j] = actual[i];

	message[j] = '\0';

	message_ = StringFromFormat("expected <%s>\n\tbut was  <%s>", expected, message);
	cpputest_free(message);
}


