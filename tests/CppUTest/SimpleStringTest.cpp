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
#include "CppUTest/MemoryLeakDetector.h"
#include "CppUTest/TestTestingFixture.h"

class JustUseNewStringAllocator : public TestMemoryAllocator
{
public:
    virtual ~JustUseNewStringAllocator() _destructor_override {}

    char* alloc_memory(size_t size, const char* file, size_t line) _override
    {
        return MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewArrayAllocator(), size, file, line);
    }
    void free_memory(char* str, size_t, const char* file, size_t line) _override
    {
        MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(getCurrentNewArrayAllocator(), str, file, line);
    }
};

class GlobalSimpleStringMemoryAccountantExecFunction
    : public ExecFunction
{
public:
    void (*testFunction_)(GlobalSimpleStringMemoryAccountant*);
    GlobalSimpleStringMemoryAccountant* parameter_;

    virtual void exec() _override
    {
        testFunction_(parameter_);
    }
};

TEST_GROUP(GlobalSimpleStringMemoryAccountant)
{
    GlobalSimpleStringAllocatorStash stash;
    GlobalSimpleStringMemoryAccountantExecFunction testFunction;
    TestTestingFixture fixture;
    GlobalSimpleStringMemoryAccountant accountant;

    void setup() _override
    {
        stash.save();
        testFunction.parameter_ = &accountant;
        fixture.setTestFunction(&testFunction);
    }

    void teardown() _override
    {
        stash.restore();
    }
};

TEST(GlobalSimpleStringMemoryAccountant, start)
{
    accountant.start();
    POINTERS_EQUAL(accountant.getAllocator(), SimpleString::getStringAllocator());
}

TEST(GlobalSimpleStringMemoryAccountant, startTwiceDoesNothing)
{
    accountant.start();
    TestMemoryAllocator* memoryAccountantAllocator = SimpleString::getStringAllocator();
    accountant.start();

    POINTERS_EQUAL(memoryAccountantAllocator, SimpleString::getStringAllocator());

    accountant.stop();
}

TEST(GlobalSimpleStringMemoryAccountant, stop)
{
    TestMemoryAllocator* originalAllocator = SimpleString::getStringAllocator();
    accountant.start();
    accountant.stop();
    POINTERS_EQUAL(originalAllocator, SimpleString::getStringAllocator());
}

static void stopAccountant_(GlobalSimpleStringMemoryAccountant* accountant)
{
    accountant->stop();
}

TEST(GlobalSimpleStringMemoryAccountant, stopWithoutStartWillFail)
{
    testFunction.testFunction_ = stopAccountant_;
    fixture.runAllTests();
    fixture.assertPrintContains("Global SimpleString allocator stopped without starting");
}

static void changeAllocatorBetweenStartAndStop_(GlobalSimpleStringMemoryAccountant* accountant)
{
    TestMemoryAllocator* originalAllocator = SimpleString::getStringAllocator();
    accountant->start();
    SimpleString::setStringAllocator(originalAllocator);
    accountant->stop();
}

TEST(GlobalSimpleStringMemoryAccountant, stopFailsWhenAllocatorWasChangedInBetween)
{
    testFunction.testFunction_ = changeAllocatorBetweenStartAndStop_;
    fixture.runAllTests();
    fixture.assertPrintContains("GlobalStrimpleStringMemoryAccountant: allocator has changed between start and stop!");
}

TEST(GlobalSimpleStringMemoryAccountant, report)
{
    SimpleString str;
    accountant.start();
    str += "More";
    accountant.stop();
    STRCMP_CONTAINS(" 1                0                 1", accountant.report().asCharString());
}

TEST(GlobalSimpleStringMemoryAccountant, reportUseCaches)
{
    size_t caches[] = {32};
    accountant.useCacheSizes(caches, 1);
    SimpleString str;
    accountant.start();
    str += "More";
    accountant.stop();
    STRCMP_CONTAINS("32                   1                1                 1", accountant.report().asCharString());
}


TEST_GROUP(SimpleString)
{
  JustUseNewStringAllocator justNewForSimpleStringTestAllocator;
  GlobalSimpleStringAllocatorStash stash;
  void setup() _override
  {
      stash.save();
      SimpleString::setStringAllocator(&justNewForSimpleStringTestAllocator);
  }
  void teardown() _override
  {
      stash.restore();
  }
};

TEST(SimpleString, defaultAllocatorIsNewArrayAllocator)
{
  SimpleString::setStringAllocator(NULLPTR);
  POINTERS_EQUAL(defaultNewArrayAllocator(), SimpleString::getStringAllocator());
}

class MyOwnStringAllocator : public TestMemoryAllocator
{
public:
    MyOwnStringAllocator() : memoryWasAllocated(false) {}
    virtual ~MyOwnStringAllocator() _destructor_override {}

    bool memoryWasAllocated;
    char* alloc_memory(size_t size, const char* file, size_t line) _override
    {
        memoryWasAllocated = true;
        return TestMemoryAllocator::alloc_memory(size, file, line);
    }
};

TEST(SimpleString, allocatorForSimpleStringCanBeReplaced)
{
    MyOwnStringAllocator myOwnAllocator;
    SimpleString::setStringAllocator(&myOwnAllocator);
    SimpleString simpleString;
    CHECK(myOwnAllocator.memoryWasAllocated);
    SimpleString::setStringAllocator(NULLPTR);
}

TEST(SimpleString, CreateSequence)
{
    SimpleString expected("hellohello");
    SimpleString actual("hello", 2);

    CHECK_EQUAL(expected, actual);
}

TEST(SimpleString, CreateSequenceOfZero)
{
    SimpleString expected("");
    SimpleString actual("hello", 0);

    CHECK_EQUAL(expected, actual);
}

TEST(SimpleString, Copy)
{
    SimpleString s1("hello");
    SimpleString s2(s1);

    CHECK_EQUAL(s1, s2);
}

TEST(SimpleString, Assignment)
{
    SimpleString s1("hello");
    SimpleString s2("goodbye");

    s2 = s1;

    CHECK_EQUAL(s1, s2);
}

TEST(SimpleString, Equality)
{
    SimpleString s1("hello");
    SimpleString s2("hello");

    CHECK(s1 == s2);
}

TEST(SimpleString, InEquality)
{
    SimpleString s1("hello");
    SimpleString s2("goodbye");

    CHECK(s1 != s2);
}

TEST(SimpleString, CompareNoCaseWithoutCase)
{
    SimpleString s1("hello");
    SimpleString s2("hello");

    CHECK(s1.equalsNoCase(s2));
}

TEST(SimpleString, CompareNoCaseWithCase)
{
    SimpleString s1("hello");
    SimpleString s2("HELLO");

    CHECK(s1.equalsNoCase(s2));
}

TEST(SimpleString, CompareNoCaseWithCaseNotEqual)
{
    SimpleString s1("hello");
    SimpleString s2("WORLD");

    CHECK(!s1.equalsNoCase(s2));
}


TEST(SimpleString, asCharString)
{
    SimpleString s1("hello");

    STRCMP_EQUAL("hello", s1.asCharString());
}

TEST(SimpleString, Size)
{
    SimpleString s1("hello!");

    LONGS_EQUAL(6, s1.size());
}

TEST(SimpleString, lowerCase)
{
    SimpleString s1("AbCdEfG1234");
    SimpleString s2(s1.lowerCase());
    STRCMP_EQUAL("abcdefg1234", s2.asCharString());
    STRCMP_EQUAL("AbCdEfG1234", s1.asCharString());
}

TEST(SimpleString, printable)
{
    SimpleString s1("ABC\01\06\a\n\r\b\t\v\f\x0E\x1F\x7F""abc");
    SimpleString s2(s1.printable());
    STRCMP_EQUAL("ABC\\x01\\x06\\a\\n\\r\\b\\t\\v\\f\\x0E\\x1F\\x7Fabc", s2.asCharString());
    STRCMP_EQUAL("ABC\01\06\a\n\r\b\t\v\f\x0E\x1F\x7F""abc", s1.asCharString());
}

TEST(SimpleString, Addition)
{
    SimpleString s1("hello!");
    SimpleString s2("goodbye!");
    SimpleString s3("hello!goodbye!");
    SimpleString s4;
    s4 = s1 + s2;

    CHECK_EQUAL(s3, s4);
}

TEST(SimpleString, Concatenation)
{
    SimpleString s1("hello!");
    SimpleString s2("goodbye!");
    SimpleString s3("hello!goodbye!");
    SimpleString s4;
    s4 += s1;
    s4 += s2;

    CHECK_EQUAL(s3, s4);

    SimpleString s5("hello!goodbye!hello!goodbye!");
    s4 += s4;

    CHECK_EQUAL(s5, s4);
}

TEST(SimpleString, Contains)
{
    SimpleString s("hello!");
    SimpleString empty("");
    SimpleString beginning("hello");
    SimpleString end("lo!");
    SimpleString mid("l");
    SimpleString notPartOfString("xxxx");

    CHECK(s.contains(empty));
    CHECK(s.contains(beginning));
    CHECK(s.contains(end));
    CHECK(s.contains(mid));
    CHECK(!s.contains(notPartOfString));

    CHECK(empty.contains(empty));
    CHECK(!empty.contains(s));
}

TEST(SimpleString, startsWith)
{
    SimpleString hi("Hi you!");
    SimpleString part("Hi");
    SimpleString diff("Hrrm Hi you! ffdsfd");
    CHECK(hi.startsWith(part));
    CHECK(!part.startsWith(hi));
    CHECK(!diff.startsWith(hi));
}

TEST(SimpleString, split)
{
    SimpleString hi("hello\nworld\nhow\ndo\nyou\ndo\n\n");

    SimpleStringCollection collection;
    hi.split("\n", collection);

    LONGS_EQUAL(7, collection.size());
    STRCMP_EQUAL("hello\n", collection[0].asCharString());
    STRCMP_EQUAL("world\n", collection[1].asCharString());
    STRCMP_EQUAL("how\n", collection[2].asCharString());
    STRCMP_EQUAL("do\n", collection[3].asCharString());
    STRCMP_EQUAL("you\n", collection[4].asCharString());
    STRCMP_EQUAL("do\n", collection[5].asCharString());
    STRCMP_EQUAL("\n", collection[6].asCharString());
}

TEST(SimpleString, splitNoTokenOnTheEnd)
{
    SimpleString string("Bah Yah oops");
    SimpleStringCollection collection;

    string.split(" ", collection);
    LONGS_EQUAL(3, collection.size());
    STRCMP_EQUAL("Bah ", collection[0].asCharString());
    STRCMP_EQUAL("Yah ", collection[1].asCharString());
    STRCMP_EQUAL("oops", collection[2].asCharString());
}

TEST(SimpleString, count)
{
    SimpleString str("ha ha ha ha");
    LONGS_EQUAL(4, str.count("ha"));
}

TEST(SimpleString, countTogether)
{
    SimpleString str("hahahaha");
    LONGS_EQUAL(4, str.count("ha"));
}

TEST(SimpleString, countEmptyString)
{
    SimpleString str("hahahaha");
	LONGS_EQUAL(8, str.count(""));
}

TEST(SimpleString, countEmptyStringInEmptyString)
{
    SimpleString str;
    LONGS_EQUAL(0, str.count(""));
}

TEST(SimpleString, endsWith)
{
    SimpleString str("Hello World");
    CHECK(str.endsWith("World"));
    CHECK(!str.endsWith("Worl"));
    CHECK(!str.endsWith("Hello"));
    SimpleString str2("ah");
    CHECK(str2.endsWith("ah"));
    CHECK(!str2.endsWith("baah"));
    SimpleString str3("");
    CHECK(!str3.endsWith("baah"));

    SimpleString str4("ha ha ha ha");
    CHECK(str4.endsWith("ha"));
}

TEST(SimpleString, replaceCharWithChar)
{
    SimpleString str("abcabcabca");
    str.replace('a', 'b');
    STRCMP_EQUAL("bbcbbcbbcb", str.asCharString());
}

TEST(SimpleString, replaceEmptyStringWithEmptyString)
{
    SimpleString str;
    str.replace("", "");
    STRCMP_EQUAL("", str.asCharString());
}

TEST(SimpleString, replaceWholeString)
{
    SimpleString str("boo");
    str.replace("boo", "");
    STRCMP_EQUAL("", str.asCharString());
}

TEST(SimpleString, replaceStringWithString)
{
    SimpleString str("boo baa boo baa boo");
    str.replace("boo", "boohoo");
    STRCMP_EQUAL("boohoo baa boohoo baa boohoo", str.asCharString());
}

TEST(SimpleString, subStringFromEmptyString)
{
    SimpleString str("");
    STRCMP_EQUAL("", str.subString(0, 1).asCharString());
}

TEST(SimpleString, subStringFromSmallString)
{
    SimpleString str("H");
    STRCMP_EQUAL("H", str.subString(0, 1).asCharString());
}

TEST(SimpleString, subStringFromPos0)
{
    SimpleString str("Hello World");
    STRCMP_EQUAL("Hello", str.subString(0, 5).asCharString());
}

TEST(SimpleString, subStringFromPos1)
{
    SimpleString str("Hello World");
    STRCMP_EQUAL("ello ", str.subString(1, 5).asCharString());
}

TEST(SimpleString, subStringFromPos5WithAmountLargerThanString)
{
    SimpleString str("Hello World");
    STRCMP_EQUAL("World", str.subString(6, 10).asCharString());
}

TEST(SimpleString, subStringFromPos6ToEndOfString)
{
    SimpleString str("Hello World");
    STRCMP_EQUAL("World", str.subString(6).asCharString());
}

TEST(SimpleString, subStringBeginPosOutOfBounds)
{
    SimpleString str("Hello World");
    STRCMP_EQUAL("", str.subString(13, 5).asCharString());
}

TEST(SimpleString, subStringFromTillNormal)
{
    SimpleString str("Hello World");
    STRCMP_EQUAL("Hello", str.subStringFromTill('H', ' ').asCharString());
}

TEST(SimpleString, subStringFromTillOutOfBounds)
{
    SimpleString str("Hello World");
    STRCMP_EQUAL("World", str.subStringFromTill('W', '!').asCharString());
}

TEST(SimpleString, subStringFromTillStartDoesntExist)
{
    SimpleString str("Hello World");
    STRCMP_EQUAL("", str.subStringFromTill('!', ' ').asCharString());
}

TEST(SimpleString, subStringFromTillWhenTheEndAppearsBeforeTheStart)
{
    SimpleString str("Hello World");
    STRCMP_EQUAL("World", str.subStringFromTill('W', 'H').asCharString());
}

TEST(SimpleString, findNormal)
{
    SimpleString str("Hello World");
    LONGS_EQUAL(0, str.find('H'));
    LONGS_EQUAL(1, str.find('e'));
    LONGS_EQUAL(SimpleString::npos, str.find('!'));
}

TEST(SimpleString, at)
{
    SimpleString str("Hello World");
    BYTES_EQUAL('H', str.at(0));
}

TEST(SimpleString, copyInBufferNormal)
{
    SimpleString str("Hello World");
    size_t bufferSize = str.size()+1;
    char* buffer = (char*) PlatformSpecificMalloc(bufferSize);
    str.copyToBuffer(buffer, bufferSize);
    STRCMP_EQUAL(str.asCharString(), buffer);
    PlatformSpecificFree(buffer);
}

TEST(SimpleString, copyInBufferWithEmptyBuffer)
{
    SimpleString str("Hello World");
    char* buffer= NULLPTR;
    str.copyToBuffer(buffer, 0);
    POINTERS_EQUAL(NULLPTR, buffer);
}

TEST(SimpleString, copyInBufferWithBiggerBufferThanNeeded)
{
    SimpleString str("Hello");
    size_t bufferSize = 20;
    char* buffer= (char*) PlatformSpecificMalloc(bufferSize);
    str.copyToBuffer(buffer, bufferSize);
    STRCMP_EQUAL(str.asCharString(), buffer);
    PlatformSpecificFree(buffer);
}

TEST(SimpleString, copyInBufferWithSmallerBufferThanNeeded)
{
    SimpleString str("Hello");
    size_t bufferSize = str.size();
    char* buffer= (char*) PlatformSpecificMalloc(bufferSize);
    str.copyToBuffer(buffer, bufferSize);
    STRNCMP_EQUAL(str.asCharString(), buffer, (bufferSize-1));
    LONGS_EQUAL(0, buffer[bufferSize-1]);
    PlatformSpecificFree(buffer);
}

TEST(SimpleString, ContainsNull)
{
    SimpleString s(NULLPTR);
    STRCMP_EQUAL("", s.asCharString());
}

TEST(SimpleString, NULLReportsNullString)
{
    STRCMP_EQUAL("(null)", StringFromOrNull((char*) NULLPTR).asCharString());
}

TEST(SimpleString, NULLReportsNullStringPrintable)
{
    STRCMP_EQUAL("(null)", PrintableStringFromOrNull((char*) NULLPTR).asCharString());
}

TEST(SimpleString, Booleans)
{
    SimpleString s1(StringFrom(true));
    SimpleString s2(StringFrom(false));
    CHECK(s1 == "true");
    CHECK(s2 == "false");
}

TEST(SimpleString, Pointers)
{
    SimpleString s(StringFrom((void *)0x1234));
    STRCMP_EQUAL("0x1234", s.asCharString());
}

TEST(SimpleString, FunctionPointers)
{
    SimpleString s(StringFrom((void (*)())0x1234));
    STRCMP_EQUAL("0x1234", s.asCharString());
}

TEST(SimpleString, Characters)
{
    SimpleString s(StringFrom('a'));
    STRCMP_EQUAL("a", s.asCharString());
}

TEST(SimpleString, NegativeSignedBytes)
{
    STRCMP_EQUAL("-15", StringFrom((signed char)-15).asCharString());
}

TEST(SimpleString, PositiveSignedBytes)
{
    STRCMP_EQUAL("4", StringFrom(4).asCharString());
}

TEST(SimpleString, LongInts)
{
    SimpleString s(StringFrom((long)1));
    CHECK(s == "1");
}

TEST(SimpleString, UnsignedLongInts)
{
    SimpleString s(StringFrom((unsigned long)1));
    SimpleString s2(StringFrom((unsigned long)1));
    CHECK(s == s2);
}

#ifdef CPPUTEST_USE_LONG_LONG

TEST(SimpleString, LongLongInts)
{
    SimpleString s(StringFrom((long long)1));
    CHECK(s == "1");
}

TEST(SimpleString, UnsignedLongLongInts)
{
    SimpleString s(StringFrom((unsigned long long)1));
    SimpleString s2(StringFrom((unsigned long long)1));
    CHECK(s == s2);
}

#endif /* CPPUTEST_USE_LONG_LONG */

TEST(SimpleString, Doubles)
{
    SimpleString s(StringFrom(1.2));
    STRCMP_EQUAL("1.2", s.asCharString());
}

extern "C" {
    static int alwaysTrue(double) { return true; }
}

TEST(SimpleString, NaN)
{
    UT_PTR_SET(PlatformSpecificIsNan, alwaysTrue);
    SimpleString s(StringFrom(0.0));
    STRCMP_EQUAL("Nan - Not a number", s.asCharString());
}

TEST(SimpleString, Inf)
{
    UT_PTR_SET(PlatformSpecificIsInf, alwaysTrue);
    SimpleString s(StringFrom(0.0));
    STRCMP_EQUAL("Inf - Infinity", s.asCharString());
}

TEST(SimpleString, SmallDoubles)
{
    SimpleString s(StringFrom(1.2e-10));
    STRCMP_CONTAINS("1.2e", s.asCharString());
}

TEST(SimpleString, Sizes)
{
    size_t size = 10;
    STRCMP_EQUAL("10", StringFrom((int) size).asCharString());
}

#if __cplusplus > 199711L && !defined __arm__ && CPPUTEST_USE_STD_CPP_LIB

TEST(SimpleString, nullptr_type)
{
    SimpleString s(StringFrom(nullptr));
    STRCMP_EQUAL("(null)", s.asCharString());
}

#endif

TEST(SimpleString, HexStrings)
{
    STRCMP_EQUAL("f3", HexStringFrom((signed char)-13).asCharString());

    SimpleString h1 = HexStringFrom(0xffffL);
    STRCMP_EQUAL("ffff", h1.asCharString());

#ifdef CPPUTEST_USE_LONG_LONG
    SimpleString h15 = HexStringFrom(0xffffLL);
    STRCMP_EQUAL("ffff", h15.asCharString());
#endif

    SimpleString h2 = HexStringFrom((void *)0xfffeL);
    STRCMP_EQUAL("fffe", h2.asCharString());

    SimpleString h3 = HexStringFrom((void (*)())0xfffdL);
    STRCMP_EQUAL("fffd", h3.asCharString());
}

TEST(SimpleString, StringFromFormat)
{
    SimpleString h1 = StringFromFormat("%s %s! %d", "Hello", "World", 2009);
    STRCMP_EQUAL("Hello World! 2009", h1.asCharString());
}

TEST(SimpleString, StringFromFormatpointer)
{
    //this is not a great test. but %p is odd on mingw and even more odd on Solaris.
    SimpleString h1 = StringFromFormat("%p", (void*) 1);
    if (h1.size() == 3)
        STRCMP_EQUAL("0x1", h1.asCharString());
    else if (h1.size() == 8)
        STRCMP_EQUAL("00000001", h1.asCharString());
    else if (h1.size() == 9)
        STRCMP_EQUAL("0000:0001", h1.asCharString());
    else if (h1.size() == 16)
        STRCMP_EQUAL("0000000000000001", h1.asCharString());
    else if (h1.size() == 1)
        STRCMP_EQUAL("1", h1.asCharString());
    else
        FAIL("Off %p behavior");
}

TEST(SimpleString, StringFromFormatLarge)
{
    const char* s = "ThisIsAPrettyLargeStringAndIfWeAddThisManyTimesToABufferItWillbeFull";
    SimpleString h1 = StringFromFormat("%s%s%s%s%s%s%s%s%s%s", s, s, s, s, s, s, s, s, s, s);
    LONGS_EQUAL(10, h1.count(s));
}

TEST(SimpleString, StringFromConstSimpleString)
{
	STRCMP_EQUAL("bla", StringFrom(SimpleString("bla")).asCharString());
}

static int WrappedUpVSNPrintf(char* buf, size_t n, const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);

    int result = PlatformSpecificVSNprintf(buf, n, format, arguments);
    va_end(arguments);
    return result;
}

TEST(SimpleString, PlatformSpecificSprintf_fits)
{
    char buf[10];

    int count = WrappedUpVSNPrintf(buf, sizeof(buf), "%s", "12345");
    STRCMP_EQUAL("12345", buf);
    LONGS_EQUAL(5, count);
}

TEST(SimpleString, PlatformSpecificSprintf_doesNotFit)
{
    char buf[10];

    int count = WrappedUpVSNPrintf(buf, sizeof(buf), "%s", "12345678901");
    STRCMP_EQUAL("123456789", buf);
    LONGS_EQUAL(11, count);
}

TEST(SimpleString, PadStringsToSameLengthString1Larger)
{
    SimpleString str1("1");
    SimpleString str2("222");

    SimpleString::padStringsToSameLength(str1, str2, '4');
    STRCMP_EQUAL("441", str1.asCharString());
    STRCMP_EQUAL("222", str2.asCharString());
}

TEST(SimpleString, PadStringsToSameLengthString2Larger)
{
    SimpleString str1("    ");
    SimpleString str2("");

    SimpleString::padStringsToSameLength(str1, str2, ' ');
    STRCMP_EQUAL("    ", str1.asCharString());
    STRCMP_EQUAL("    ", str2.asCharString());
}

TEST(SimpleString, PadStringsToSameLengthWithSameLengthStrings)
{
    SimpleString str1("123");
    SimpleString str2("123");

    SimpleString::padStringsToSameLength(str1, str2, ' ');
    STRCMP_EQUAL("123", str1.asCharString());
    STRCMP_EQUAL("123", str2.asCharString());
}

TEST(SimpleString, NullParameters2)
{
    SimpleString* arr = new SimpleString[100];
    delete[] arr;
}

TEST(SimpleString, CollectionMultipleAllocateNoLeaksMemory)
{
    SimpleStringCollection col;
    col.allocate(5);
    col.allocate(5);
    // CHECK no memory leak
}

TEST(SimpleString, CollectionReadOutOfBoundsReturnsEmptyString)
{
    SimpleStringCollection col;
    col.allocate(3);
    STRCMP_EQUAL("", col[3].asCharString());
}

TEST(SimpleString, CollectionWritingToEmptyString)
{
    SimpleStringCollection col;
    col.allocate(3);
    col[3] = SimpleString("HAH");
    STRCMP_EQUAL("", col[3].asCharString());
}

#ifdef CPPUTEST_64BIT

TEST(SimpleString, 64BitAddressPrintsCorrectly)
{
    char* p = (char*) 0x0012345678901234;
    SimpleString expected("0x12345678901234");
    SimpleString actual = StringFrom((void*)p);
    STRCMP_EQUAL(expected.asCharString(), actual.asCharString());
}

#ifndef CPPUTEST_64BIT_32BIT_LONGS

TEST(SimpleString, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
	long value = -1;

	STRCMP_EQUAL("(0xffffffffffffffff)", BracketsFormattedHexStringFrom(value).asCharString());
}

#else

TEST(SimpleString, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
	long value = -1;

	STRCMP_EQUAL("(0xffffffff)", BracketsFormattedHexStringFrom(value).asCharString());
}

#endif
#else
/*
 * This test case cannot pass on 32 bit systems.
 */
IGNORE_TEST(SimpleString, 64BitAddressPrintsCorrectly)
{
}

TEST(SimpleString, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
	long value = -1;

	STRCMP_EQUAL("(0xffffffff)", BracketsFormattedHexStringFrom(value).asCharString());
}
#endif

TEST(SimpleString, BuildStringFromUnsignedLongInteger)
{
    unsigned long int i = 0xffffffff;

    SimpleString result = StringFrom(i);
    const char* expected_string = "4294967295";
    CHECK_EQUAL(expected_string, result);
}

TEST(SimpleString, BuildStringFromUnsignedInteger)
{
    unsigned int i = 0xff;

    SimpleString result = StringFrom(i);
    const char* expected_string = "255";
    CHECK_EQUAL(expected_string, result);
}

#if CPPUTEST_USE_STD_CPP_LIB

TEST(SimpleString, fromStdString)
{
    std::string s("hello");
    SimpleString s1(StringFrom(s));

    STRCMP_EQUAL("hello", s1.asCharString());
}

TEST(SimpleString, CHECK_EQUAL_unsigned_long)
{
    unsigned long i = 0xffffffffUL;
    CHECK_EQUAL(i, i);
}

TEST(SimpleString, unsigned_long)
{
    unsigned long i = 0xffffffffUL;

    SimpleString result = StringFrom(i);
    const char* expected_string = "4294967295";
    CHECK_EQUAL(expected_string, result);
}

#endif

TEST(SimpleString, StrCmp)
{
    char empty[] = "";
    char blabla[] = "blabla";
    char bla[] = "bla";
    CHECK(SimpleString::StrCmp(empty, empty) == 0);
    CHECK(SimpleString::StrCmp(bla, blabla) == -(int)'b');
    CHECK(SimpleString::StrCmp(blabla, bla) == 'b');
    CHECK(SimpleString::StrCmp(bla, empty) == 'b');
    CHECK(SimpleString::StrCmp(empty, bla) == -(int)'b');
    CHECK(SimpleString::StrCmp(bla, bla) == 0);
}

TEST(SimpleString, StrNCpy_no_zero_termination)
{
    char str[] = "XXXXXXXXXX";
    STRCMP_EQUAL("womanXXXXX", SimpleString::StrNCpy(str, "woman", 5));
}

TEST(SimpleString, StrNCpy_zero_termination)
{
    char str[] = "XXXXXXXXXX";
    STRCMP_EQUAL("woman", SimpleString::StrNCpy(str, "woman", 6));
}

TEST(SimpleString, StrNCpy_null_proof)
{
    POINTERS_EQUAL(NULLPTR, SimpleString::StrNCpy(NULLPTR, "woman", 6));
}

TEST(SimpleString, StrNCpy_stops_at_end_of_string)
{
    char str[] = "XXXXXXXXXX";
    STRCMP_EQUAL("woman", SimpleString::StrNCpy(str, "woman", 8));
}

TEST(SimpleString, StrNCpy_nothing_to_do)
{
    char str[] = "XXXXXXXXXX";
    STRCMP_EQUAL("XXXXXXXXXX", SimpleString::StrNCpy(str, "woman", 0));
}

TEST(SimpleString, StrNCpy_write_into_the_middle)
{
    char str[] = "womanXXXXX";
    SimpleString::StrNCpy(str+3, "e", 1);
    STRCMP_EQUAL("womenXXXXX", str);
}

TEST(SimpleString, StrNCmp_equal)
{
    int result = SimpleString::StrNCmp("teststring", "tests", 5);
    LONGS_EQUAL(0, result);
}

TEST(SimpleString, StrNCmp_should_always_return_0_when_n_is_0)
{
    int result = SimpleString::StrNCmp("a", "b", 0);
    LONGS_EQUAL(0, result);
}

TEST(SimpleString, StrNCmp_s1_smaller)
{
    int result = SimpleString::StrNCmp("testing", "tests", 7);
    LONGS_EQUAL('i' - 's', result);
}

TEST(SimpleString, StrNCmp_s1_larger)
{
    int result = SimpleString::StrNCmp("teststring", "tester", 7);
    LONGS_EQUAL('s' - 'e', result);
}

TEST(SimpleString, StrNCmp_n_too_large)
{
    int result = SimpleString::StrNCmp("teststring", "teststring", 20);
    LONGS_EQUAL(0, result);
}

TEST(SimpleString, StrNCmp_s1_empty)
{
    int result = SimpleString::StrNCmp("", "foo", 2);
    LONGS_EQUAL(0 - 'f', result);
}

TEST(SimpleString, StrNCmp_s2_empty)
{
    int result = SimpleString::StrNCmp("foo", "", 2);
    LONGS_EQUAL('f', result);
}

TEST(SimpleString, StrNCmp_s1_and_s2_empty)
{
    int result = SimpleString::StrNCmp("", "", 2);
    LONGS_EQUAL(0, result);
}

TEST(SimpleString, StrStr)
{
    char foo[] = "foo";
    char empty[] = "";
    char foobarfoo[] = "foobarfoo";
    char barf[] = "barf";
    CHECK(SimpleString::StrStr(foo, empty) == foo);
    CHECK(SimpleString::StrStr(empty, foo) == NULLPTR);
    CHECK(SimpleString::StrStr(foobarfoo, barf) == foobarfoo+3);
    CHECK(SimpleString::StrStr(barf, foobarfoo) == NULLPTR);
    CHECK(SimpleString::StrStr(foo, foo) == foo);
}

TEST(SimpleString, AtoI)
{
    char max_short_str[] = "32767";
    char min_short_str[] = "-32768";

    CHECK(12345  == SimpleString::AtoI("012345"));
    CHECK(6789   == SimpleString::AtoI("6789"));
    CHECK(12345  == SimpleString::AtoI("12345/"));
    CHECK(12345  == SimpleString::AtoI("12345:"));
    CHECK(-12345 == SimpleString::AtoI("-12345"));
    CHECK(123    == SimpleString::AtoI("\t \r\n123"));
    CHECK(123    == SimpleString::AtoI("123-foo"));
    CHECK(0      == SimpleString::AtoI("-foo"));
    CHECK(-32768 == SimpleString::AtoI(min_short_str));
    CHECK(32767  == SimpleString::AtoI(max_short_str));
}

TEST(SimpleString, AtoU)
{
    char max_short_str[] = "65535";
    CHECK(12345  == SimpleString::AtoU("012345"));
    CHECK(6789   == SimpleString::AtoU("6789"));
    CHECK(12345  == SimpleString::AtoU("12345/"));
    CHECK(12345  == SimpleString::AtoU("12345:"));
    CHECK(123    == SimpleString::AtoU("\t \r\n123"));
    CHECK(123    == SimpleString::AtoU("123-foo"));
    CHECK(65535  == SimpleString::AtoU(max_short_str));
    CHECK(0      == SimpleString::AtoU("foo"));
    CHECK(0      == SimpleString::AtoU("-foo"));
    CHECK(0      == SimpleString::AtoU("+1"));
    CHECK(0      == SimpleString::AtoU("-1"));
    CHECK(0      == SimpleString::AtoU("0"));
}

TEST(SimpleString, Binary)
{
    const unsigned char value[] = { 0x00, 0x01, 0x2A, 0xFF };
    const char expectedString[] = "00 01 2A FF";

    STRCMP_EQUAL(expectedString, StringFromBinary(value, sizeof(value)).asCharString());
    STRCMP_EQUAL(expectedString, StringFromBinaryOrNull(value, sizeof(value)).asCharString());
    STRCMP_EQUAL("", StringFromBinary(value, 0).asCharString());
    STRCMP_EQUAL("(null)", StringFromBinaryOrNull(NULLPTR, 0).asCharString());
}

TEST(SimpleString, BinaryWithSize)
{
    const unsigned char value[] = { 0x12, 0xFE, 0xA1 };
    const char expectedString[] = "Size = 3 | HexContents = 12 FE A1";

    STRCMP_EQUAL(expectedString, StringFromBinaryWithSize(value, sizeof(value)).asCharString());
    STRCMP_EQUAL(expectedString, StringFromBinaryWithSizeOrNull(value, sizeof(value)).asCharString());
    STRCMP_EQUAL("Size = 0 | HexContents = ", StringFromBinaryWithSize(value, 0).asCharString());
    STRCMP_EQUAL("(null)", StringFromBinaryWithSizeOrNull(NULLPTR, 0).asCharString());
}

TEST(SimpleString, BinaryWithSizeLargerThan128)
{
    unsigned char value[129];
    value[127] = 0x00;
    value[128] = 0xff;

    STRCMP_CONTAINS("00 ...", StringFromBinaryWithSize(value, sizeof(value)).asCharString());
}

TEST(SimpleString, MemCmp)
{
    unsigned char smaller[] = { 0x00, 0x01, 0x2A, 0xFF };
    unsigned char greater[] = { 0x00, 0x01, 0xFF, 0xFF };

    LONGS_EQUAL(0, SimpleString::MemCmp(smaller, smaller, sizeof(smaller)));
    CHECK(SimpleString::MemCmp(smaller, greater, sizeof(smaller)) < 0);
    CHECK(SimpleString::MemCmp(greater, smaller, sizeof(smaller)) > 0);
    LONGS_EQUAL(0, SimpleString::MemCmp(NULLPTR, NULLPTR, 0));
}

TEST(SimpleString, MemCmpFirstLastNotMatching)
{
    unsigned char base[] = { 0x00, 0x01, 0x2A, 0xFF };
    unsigned char firstNotMatching[] = { 0x01, 0x01, 0x2A, 0xFF };
    unsigned char lastNotMatching[] = { 0x00, 0x01, 0x2A, 0x00 };
    CHECK(0 != SimpleString::MemCmp(base, firstNotMatching, sizeof(base)));
    CHECK(0 != SimpleString::MemCmp(base, lastNotMatching, sizeof(base)));
}

#if (CPPUTEST_CHAR_BIT == 16)
TEST(SimpleString, MaskedBitsChar)
{
    STRCMP_EQUAL("xxxxxxxx xxxxxxxx", StringFromMaskedBits(0x00, 0x00, 1).asCharString());
    STRCMP_EQUAL("xxxxxxxx 00000000", StringFromMaskedBits(0x00, 0xFF, 1).asCharString());
    STRCMP_EQUAL("xxxxxxxx 11111111", StringFromMaskedBits(0xFF, 0xFF, 1).asCharString());
    STRCMP_EQUAL("xxxxxxxx 1xxxxxxx", StringFromMaskedBits(0x80, 0x80, 1).asCharString());
    STRCMP_EQUAL("xxxxxxxx xxxxxxx1", StringFromMaskedBits(0x01, 0x01, 1).asCharString());
    STRCMP_EQUAL("xxxxxxxx 11xx11xx", StringFromMaskedBits(0xFF, 0xCC, 1).asCharString());
}
#else
TEST(SimpleString, MaskedBitsChar)
{
    STRCMP_EQUAL("xxxxxxxx", StringFromMaskedBits(0x00, 0x00, 1).asCharString());
    STRCMP_EQUAL("00000000", StringFromMaskedBits(0x00, 0xFF, 1).asCharString());
    STRCMP_EQUAL("11111111", StringFromMaskedBits(0xFF, 0xFF, 1).asCharString());
    STRCMP_EQUAL("1xxxxxxx", StringFromMaskedBits(0x80, 0x80, 1).asCharString());
    STRCMP_EQUAL("xxxxxxx1", StringFromMaskedBits(0x01, 0x01, 1).asCharString());
    STRCMP_EQUAL("11xx11xx", StringFromMaskedBits(0xFF, 0xCC, 1).asCharString());
}
#endif

TEST(SimpleString, MaskedBits16Bit)
{
    STRCMP_EQUAL("xxxxxxxx xxxxxxxx", StringFromMaskedBits(0x0000, 0x0000, 2*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("00000000 00000000", StringFromMaskedBits(0x0000, 0xFFFF, 2*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("11111111 11111111", StringFromMaskedBits(0xFFFF, 0xFFFF, 2*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("1xxxxxxx xxxxxxxx", StringFromMaskedBits(0x8000, 0x8000, 2*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("xxxxxxxx xxxxxxx1", StringFromMaskedBits(0x0001, 0x0001, 2*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("11xx11xx 11xx11xx", StringFromMaskedBits(0xFFFF, 0xCCCC, 2*8/CPPUTEST_CHAR_BIT).asCharString());
}

TEST(SimpleString, MaskedBits32Bit)
{
    STRCMP_EQUAL("xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx", StringFromMaskedBits(0x00000000, 0x00000000, 4*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("00000000 00000000 00000000 00000000", StringFromMaskedBits(0x00000000, 0xFFFFFFFF, 4*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("11111111 11111111 11111111 11111111", StringFromMaskedBits(0xFFFFFFFF, 0xFFFFFFFF, 4*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("1xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx", StringFromMaskedBits(0x80000000, 0x80000000, 4*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxx1", StringFromMaskedBits(0x00000001, 0x00000001, 4*8/CPPUTEST_CHAR_BIT).asCharString());
    STRCMP_EQUAL("11xx11xx 11xx11xx 11xx11xx 11xx11xx", StringFromMaskedBits(0xFFFFFFFF, 0xCCCCCCCC, 4*8/CPPUTEST_CHAR_BIT).asCharString());
}

TEST(SimpleString, StringFromOrdinalNumberOnes)
{
    STRCMP_EQUAL("2nd", StringFromOrdinalNumber(2).asCharString());
    STRCMP_EQUAL("3rd", StringFromOrdinalNumber(3).asCharString());
    STRCMP_EQUAL("4th", StringFromOrdinalNumber(4).asCharString());
    STRCMP_EQUAL("5th", StringFromOrdinalNumber(5).asCharString());
    STRCMP_EQUAL("6th", StringFromOrdinalNumber(6).asCharString());
    STRCMP_EQUAL("7th", StringFromOrdinalNumber(7).asCharString());
}

TEST(SimpleString, StringFromOrdinalNumberTens)
{
    STRCMP_EQUAL("10th", StringFromOrdinalNumber(10).asCharString());
    STRCMP_EQUAL("11th", StringFromOrdinalNumber(11).asCharString());
    STRCMP_EQUAL("12th", StringFromOrdinalNumber(12).asCharString());
    STRCMP_EQUAL("13th", StringFromOrdinalNumber(13).asCharString());
    STRCMP_EQUAL("14th", StringFromOrdinalNumber(14).asCharString());
    STRCMP_EQUAL("18th", StringFromOrdinalNumber(18).asCharString());
}

TEST(SimpleString, StringFromOrdinalNumberOthers)
{
    STRCMP_EQUAL("21st", StringFromOrdinalNumber(21).asCharString());
    STRCMP_EQUAL("22nd", StringFromOrdinalNumber(22).asCharString());
    STRCMP_EQUAL("23rd", StringFromOrdinalNumber(23).asCharString());
    STRCMP_EQUAL("24th", StringFromOrdinalNumber(24).asCharString());
    STRCMP_EQUAL("32nd", StringFromOrdinalNumber(32).asCharString());
    STRCMP_EQUAL("100th", StringFromOrdinalNumber(100).asCharString());
    STRCMP_EQUAL("101st", StringFromOrdinalNumber(101).asCharString());
}

TEST(SimpleString, BracketsFormattedHexStringFromForSignedChar)
{
	signed char value = 'c';

	STRCMP_EQUAL("(0x63)", BracketsFormattedHexStringFrom(value).asCharString());
}


TEST(SimpleString, BracketsFormattedHexStringFromForUnsignedInt)
{
	unsigned int value = 1;

	STRCMP_EQUAL("(0x1)", BracketsFormattedHexStringFrom(value).asCharString());
}

TEST(SimpleString, BracketsFormattedHexStringFromForUnsignedLong)
{
	unsigned long value = 1;

	STRCMP_EQUAL("(0x1)", BracketsFormattedHexStringFrom(value).asCharString());
}

#ifdef CPPUTEST_16BIT_INTS
TEST(SimpleString, BracketsFormattedHexStringFromForInt)
{
	int value = -1;

	STRCMP_EQUAL("(0xffff)", BracketsFormattedHexStringFrom(value).asCharString());
}
#else
TEST(SimpleString, BracketsFormattedHexStringFromForInt)
{
	int value = -1;
	STRCMP_EQUAL("(0xffffffff)", BracketsFormattedHexStringFrom(value).asCharString());
}
#endif

TEST(SimpleString, BracketsFormattedHexStringFromForLong)
{
	long value = 1;

	STRCMP_EQUAL("(0x1)", BracketsFormattedHexStringFrom(value).asCharString());
}
#ifdef CPPUTEST_USE_LONG_LONG

TEST(SimpleString, BracketsFormattedHexStringFromForLongLong)
{
	cpputest_longlong value = 1;

	STRCMP_EQUAL("(0x1)", BracketsFormattedHexStringFrom(value).asCharString());
}
TEST(SimpleString, BracketsFormattedHexStringFromForULongLong)
{
	cpputest_ulonglong value = 1;

	STRCMP_EQUAL("(0x1)", BracketsFormattedHexStringFrom(value).asCharString());
}
#else
TEST(SimpleString, BracketsFormattedHexStringFromForLongLong)
{
	cpputest_longlong value = 1;

	STRCMP_EQUAL("", BracketsFormattedHexStringFrom(value).asCharString());
}
TEST(SimpleString, BracketsFormattedHexStringFromForULongLong)
{
	cpputest_ulonglong value = 1;

	STRCMP_EQUAL("", BracketsFormattedHexStringFrom(value).asCharString());
}

#endif
