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
#include "CppUTest/TestOutput.h"

namespace
{
const int failLineNumber = 2;
const char* failFileName = "fail.cpp";
}

TEST_GROUP(TestFailure)
{
    UtestShell* test;

    void setup()
    {
        test = new UtestShell("groupname", "testname", failFileName, failLineNumber-1);
    }
    void teardown()
    {
        delete test;
    }
};
#define FAILURE_EQUAL(a, b) STRCMP_EQUAL_LOCATION(a, b.getMessage().asCharString(), "", __FILE__, __LINE__)

TEST(TestFailure, CreateFailure)
{
    TestFailure f1(test, failFileName, failLineNumber, "the failure message");
    TestFailure f2(test, "the failure message");
    TestFailure f3(test, failFileName, failLineNumber);
}

TEST(TestFailure, GetTestFileAndLineFromFailure)
{
    TestFailure f1(test, failFileName, failLineNumber, "the failure message");
    STRCMP_EQUAL(failFileName, f1.getTestFileName().asCharString());
    LONGS_EQUAL(1, f1.getTestLineNumber());
}

TEST(TestFailure, EqualsFailureWithText)
{
    EqualsFailure f(test, failFileName, failLineNumber, "expected", "actual", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <expected>\n\tbut was  <actual>", f);
}

TEST(TestFailure, EqualsFailure)
{
    EqualsFailure f(test, failFileName, failLineNumber, "expected", "actual", "");
    FAILURE_EQUAL("expected <expected>\n\tbut was  <actual>", f);
}

TEST(TestFailure, EqualsFailureWithNullAsActual)
{
    EqualsFailure f(test, failFileName, failLineNumber, "expected", NULL, "");
    FAILURE_EQUAL("expected <expected>\n\tbut was  <(null)>", f);
}

TEST(TestFailure, EqualsFailureWithNullAsExpected)
{
    EqualsFailure f(test, failFileName, failLineNumber, NULL, "actual", "");
    FAILURE_EQUAL("expected <(null)>\n\tbut was  <actual>", f);
}

TEST(TestFailure, CheckEqualFailureWithText)
{
    CheckEqualFailure f(test, failFileName, failLineNumber, "expected", "actual", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <expected>\n"
                  "\tbut was  <actual>\n"
                  "\tdifference starts at position 0 at: <          actual    >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, CheckEqualFailure)
{
    CheckEqualFailure f(test, failFileName, failLineNumber, "expected", "actual", "");
    FAILURE_EQUAL("expected <expected>\n"
                  "\tbut was  <actual>\n"
                  "\tdifference starts at position 0 at: <          actual    >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, CheckFailure)
{
    CheckFailure f(test, failFileName, failLineNumber, "CHECK", "chk");
    FAILURE_EQUAL("CHECK(chk) failed", f);
}

TEST(TestFailure, CheckFailureWithText)
{
    CheckFailure f(test, failFileName, failLineNumber, "CHECK", "chk", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\tCHECK(chk) failed", f);
}

TEST(TestFailure, FailFailure)
{
    FailFailure f(test, failFileName, failLineNumber, "chk");
    FAILURE_EQUAL("chk", f);
}

TEST(TestFailure, LongsEqualFailureWithText)
{
    LongsEqualFailure f(test, failFileName, failLineNumber, 1, 2, "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
}

TEST(TestFailure, LongsEqualFailure)
{
    LongsEqualFailure f(test, failFileName, failLineNumber, 1, 2, "");
    FAILURE_EQUAL("expected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
}

TEST(TestFailure, LongLongsEqualFailure)
{
#ifdef CPPUTEST_USE_LONG_LONG
    LongLongsEqualFailure f(test, failFileName, failLineNumber, 1, 2, "");
    FAILURE_EQUAL("expected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
#else
    cpputest_longlong dummy_longlong;
    LongLongsEqualFailure f(test, failFileName, failLineNumber, dummy_longlong, dummy_longlong, "");
    FAILURE_EQUAL("expected <<longlong_unsupported> >\n\tbut was  <<longlong_unsupported> >", f);
#endif
}

TEST(TestFailure, UnsignedLongLongsEqualFailure)
{
#ifdef CPPUTEST_USE_LONG_LONG
    UnsignedLongLongsEqualFailure f(test, failFileName, failLineNumber, 1, 2, "");
    FAILURE_EQUAL("expected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
#else
    cpputest_ulonglong dummy_ulonglong;
    UnsignedLongLongsEqualFailure f(test, failFileName, failLineNumber, dummy_ulonglong, dummy_ulonglong, "");
    FAILURE_EQUAL("expected <<ulonglong_unsupported> >\n\tbut was  <<ulonglong_unsupported> >", f);
#endif
}

TEST(TestFailure, SignedBytesEqualFailure)
{
    SignedBytesEqualFailure f(test, failFileName, failLineNumber, (signed char)-1, (signed char)2, "");
    FAILURE_EQUAL("expected <-1 (0xff)>\n\tbut was  < 2 (0x2)>", f);
}

TEST(TestFailure, StringsEqualFailureWithText)
{
    StringEqualFailure f(test, failFileName, failLineNumber, "abc", "abd", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <abc>\n"
                  "\tbut was  <abd>\n"
                  "\tdifference starts at position 2 at: <        abd         >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailure)
{
    StringEqualFailure f(test, failFileName, failLineNumber, "abc", "abd", "");
    FAILURE_EQUAL("expected <abc>\n"
                "\tbut was  <abd>\n"
                "\tdifference starts at position 2 at: <        abd         >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailureAtTheEnd)
{
    StringEqualFailure f(test, failFileName, failLineNumber, "abc", "ab", "");
    FAILURE_EQUAL("expected <abc>\n"
                "\tbut was  <ab>\n"
                "\tdifference starts at position 2 at: <        ab          >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailureNewVariantAtTheEnd)
{
    StringEqualFailure f(test, failFileName, failLineNumber, "EndOfALongerString", "EndOfALongerStrinG", "");
    FAILURE_EQUAL("expected <EndOfALongerString>\n"
                "\tbut was  <EndOfALongerStrinG>\n"
                "\tdifference starts at position 17 at: <ongerStrinG         >\n"
                "\t                                                ^", f);
}

TEST(TestFailure, StringsEqualFailureWithNewLinesAndTabs)
{
    StringEqualFailure f(test, failFileName, failLineNumber,
            "StringWith\t\nDifferentString",
            "StringWith\t\ndifferentString", "");

    FAILURE_EQUAL("expected <StringWith\t\nDifferentString>\n"
                "\tbut was  <StringWith\t\ndifferentString>\n"
                "\tdifference starts at position 12 at: <ringWith\t\ndifferentS>\n"
                "\t                                              \t\n^", f);
}

TEST(TestFailure, StringsEqualFailureInTheMiddle)
{
    StringEqualFailure f(test, failFileName, failLineNumber, "aa", "ab", "");
    FAILURE_EQUAL("expected <aa>\n"
                "\tbut was  <ab>\n"
                "\tdifference starts at position 1 at: <         ab         >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailureAtTheBeginning)
{
    StringEqualFailure f(test, failFileName, failLineNumber, "aaa", "bbb", "");
    FAILURE_EQUAL("expected <aaa>\n"
                "\tbut was  <bbb>\n"
                "\tdifference starts at position 0 at: <          bbb       >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailureWithNullAsActual)
{
    StringEqualFailure f(test, failFileName, failLineNumber, "abc", NULL, "");
    FAILURE_EQUAL("expected <abc>\n"
                "\tbut was  <(null)>", f);
}

TEST(TestFailure, StringsEqualFailureWithNullAsExpected)
{
    StringEqualFailure f(test, failFileName, failLineNumber, NULL, "abd", "");
    FAILURE_EQUAL("expected <(null)>\n"
                "\tbut was  <abd>", f);
}

TEST(TestFailure, StringsEqualNoCaseFailureWithText)
{
    StringEqualNoCaseFailure f(test, failFileName, failLineNumber, "ABC", "abd", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <ABC>\n"
                  "\tbut was  <abd>\n"
                  "\tdifference starts at position 2 at: <        abd         >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualNoCaseFailure)
{
    StringEqualNoCaseFailure f(test, failFileName, failLineNumber, "ABC", "abd", "");
    FAILURE_EQUAL("expected <ABC>\n"
                "\tbut was  <abd>\n"
                "\tdifference starts at position 2 at: <        abd         >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualNoCaseFailureWithActualAsNull)
{
    StringEqualNoCaseFailure f(test, failFileName, failLineNumber, "ABC", NULL, "");
    FAILURE_EQUAL("expected <ABC>\n"
                "\tbut was  <(null)>", f);
}

TEST(TestFailure, StringsEqualNoCaseFailureWithExpectedAsNull)
{
    StringEqualNoCaseFailure f(test, failFileName, failLineNumber, NULL, "abd", "");
    FAILURE_EQUAL("expected <(null)>\n"
                "\tbut was  <abd>", f);
}

TEST(TestFailure, StringsEqualNoCaseFailure2)
{
    StringEqualNoCaseFailure f(test, failFileName, failLineNumber, "ac", "AB", "");
    FAILURE_EQUAL("expected <ac>\n"
                "\tbut was  <AB>\n"
                "\tdifference starts at position 1 at: <         AB         >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, DoublesEqualNormalWithText)
{
    DoublesEqualFailure f(test, failFileName, failLineNumber, 1.0, 2.0, 3.0, "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <1>\n"
                  "\tbut was  <2> threshold used was <3>", f);
}

TEST(TestFailure, DoublesEqualNormal)
{
    DoublesEqualFailure f(test, failFileName, failLineNumber, 1.0, 2.0, 3.0, "");
    FAILURE_EQUAL("expected <1>\n"
                "\tbut was  <2> threshold used was <3>", f);
}

TEST(TestFailure, BinaryEqualWithText)
{
    const unsigned char expectedData[] = { 0x00 };
    const unsigned char actualData[] = { 0x01 };
    BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <00>\n"
                  "\tbut was  <01>\n"
                  "\tdifference starts at position 0 at: <         01         >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualOneByte)
{
    const unsigned char expectedData[] = { 0x00 };
    const unsigned char actualData[] = { 0x01 };
    BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00>\n"
                "\tbut was  <01>\n"
    			"\tdifference starts at position 0 at: <         01         >\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualTwoBytes)
{
    const unsigned char expectedData[] = {0x00, 0x01};
    const unsigned char actualData[] = {0x00, 0x02};
    BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 01>\n"
                "\tbut was  <00 02>\n"
    			"\tdifference starts at position 1 at: <      00 02         >\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualThreeBytes)
{
    const unsigned char expectedData[] = {0x00, 0x01, 0x00};
    const unsigned char actualData[] = {0x00, 0x02, 0x00};
    BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 01 00>\n"
                "\tbut was  <00 02 00>\n"
    			"\tdifference starts at position 1 at: <      00 02 00      >\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualFullWidth)
{
    const unsigned char expectedData[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
    const unsigned char actualData[] = {0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};
    BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 00 00 01 00 00 00>\n"
                "\tbut was  <00 00 00 02 00 00 00>\n"
    			"\tdifference starts at position 3 at: <00 00 00 02 00 00 00>\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualLast)
{
    const unsigned char expectedData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	const unsigned char actualData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 00 00 00 00 00 00>\n"
                "\tbut was  <00 00 00 00 00 00 01>\n"
    			"\tdifference starts at position 6 at: <00 00 00 01         >\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualActualNull)
{
    const unsigned char expectedData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, NULL, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 00 00 00 00 00 00>\n\tbut was  <(null)>", f);
}

TEST(TestFailure, BinaryEqualExpectedNull)
{
    const unsigned char actualData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    BinaryEqualFailure f(test, failFileName, failLineNumber, NULL, actualData, sizeof(actualData), "");
    FAILURE_EQUAL("expected <(null)>\n\tbut was  <00 00 00 00 00 00 01>", f);
}

TEST(TestFailure, BitsEqualWithText)
{
    BitsEqualFailure f(test, failFileName, failLineNumber, 0x0001, 0x0003, 0x00FF, 2*8/CPPUTEST_CHAR_BIT, "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <xxxxxxxx 00000001>\n\tbut was  <xxxxxxxx 00000011>", f);
}

#if (CPPUTEST_CHAR_BIT == 16)
TEST(TestFailure, BitsEqualChar)
{
    BitsEqualFailure f(test, failFileName, failLineNumber, 0x01, 0x03, 0xFF, sizeof(char), "");
    FAILURE_EQUAL("expected <xxxxxxxx 00000001>\n\tbut was  <xxxxxxxx 00000011>", f);
}
#else
TEST(TestFailure, BitsEqualChar)
{
    BitsEqualFailure f(test, failFileName, failLineNumber, 0x01, 0x03, 0xFF, sizeof(char), "");
    FAILURE_EQUAL("expected <00000001>\n\tbut was  <00000011>", f);
}
#endif

TEST(TestFailure, BitsEqual16Bit)
{
    BitsEqualFailure f(test, failFileName, failLineNumber, 0x0001, 0x0003, 0xFFFF, 2*8/CPPUTEST_CHAR_BIT, "");
    FAILURE_EQUAL("expected <00000000 00000001>\n\tbut was  <00000000 00000011>", f);
}

TEST(TestFailure, BitsEqual32Bit)
{
    BitsEqualFailure f(test, failFileName, failLineNumber, 0x00000001, 0x00000003, 0xFFFFFFFF, 4*8/CPPUTEST_CHAR_BIT, "");
    FAILURE_EQUAL("expected <00000000 00000000 00000000 00000001>\n\tbut was  <00000000 00000000 00000000 00000011>", f);
}

TEST(TestFailure, FeatureUnsupported)
{
    FeatureUnsupportedFailure f(test, failFileName, failLineNumber, "SOME_FEATURE", "");
    FAILURE_EQUAL("The feature \"SOME_FEATURE\" is not supported in this environment or with the feature set selected when building the library.", f);
}
