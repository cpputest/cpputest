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

#include "CppUTest/TestHarness_c.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/PlatformSpecificFunctions.h"


extern "C" int setup_teardown_was_called_in_test_group_in_C;
extern "C" int test_was_called_in_test_group_in_C;
int setup_teardown_was_called_in_test_group_in_C = 0;
int test_was_called_in_test_group_in_C = 0;

TEST_GROUP_C_WRAPPER(TestGroupInC)
{
    TEST_GROUP_C_SETUP_WRAPPER(TestGroupInC)
    TEST_GROUP_C_TEARDOWN_WRAPPER(TestGroupInC)
};

TEST_C_WRAPPER(TestGroupInC, checkThatTheTestHasRun)
IGNORE_TEST_C_WRAPPER(TestGroupInC, ignoreMacroForCFile)

/*
 * This test is a bit strange. They use the fact that you can do -r2 for repeating the same run.
 * When you do so, the same statics will be shared and therefore we can test whether the setup/teardown is run
 * correctly.
 */

TEST(TestGroupInC, setupHasBeenCalled)
{
    test_was_called_in_test_group_in_C++;
    /* Increased in setup, decreased in teardown. So at this point it must be 1 also on a multiple run */
    LONGS_EQUAL(1, setup_teardown_was_called_in_test_group_in_C);
}

static bool hasDestructorOfTheDestructorCheckedBeenCalled;

class HasTheDestructorBeenCalledChecker
{
public:
    HasTheDestructorBeenCalledChecker(){}
    ~HasTheDestructorBeenCalledChecker() { hasDestructorOfTheDestructorCheckedBeenCalled = true; }
};

TEST_GROUP(TestHarness_c)
{
    TestTestingFixture* fixture;
    TEST_SETUP()
    {
        hasDestructorOfTheDestructorCheckedBeenCalled = false;
        fixture = new TestTestingFixture();
    }
    TEST_TEARDOWN()
    {
        delete fixture;
    }
};

static void failBoolMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_BOOL(1, 0);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkBool)
{
    CHECK_EQUAL_C_BOOL(1, 1);
    CHECK_EQUAL_C_BOOL(1, 2);
    fixture->setTestFunction(failBoolMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <true>\n	but was  <false>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failBoolTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_BOOL_TEXT(1, 0, "BoolTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkBoolText)
{
    CHECK_EQUAL_C_BOOL_TEXT(1, 1, "Text");
    CHECK_EQUAL_C_BOOL_TEXT(1, 2, "Text");
    fixture->setTestFunction(failBoolTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <true>\n	but was  <false>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: BoolTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failIntMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_INT(1, 2);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkInt)
{
    CHECK_EQUAL_C_INT(2, 2);
    fixture->setTestFunction(failIntMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failIntTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_INT_TEXT(1, 2, "IntTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkIntText)
{
    CHECK_EQUAL_C_INT_TEXT(2, 2, "Text");
    fixture->setTestFunction(failIntTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: IntTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failUnsignedIntMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_UINT(1, 2);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkUnsignedInt)
{
    CHECK_EQUAL_C_UINT(2, 2);
    fixture->setTestFunction(failUnsignedIntMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failUnsignedIntTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_UINT_TEXT(1, 2, "UnsignedIntTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkUnsignedIntText)
{
    CHECK_EQUAL_C_UINT_TEXT(2, 2, "Text");
    fixture->setTestFunction(failUnsignedIntTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: UnsignedIntTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failLongIntMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_LONG(1, 2);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkLongInt)
{
    CHECK_EQUAL_C_LONG(2, 2);
    fixture->setTestFunction(failLongIntMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failLongIntTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_LONG_TEXT(1, 2, "LongIntTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkLongIntText)
{
    CHECK_EQUAL_C_LONG_TEXT(2, 2, "Text");
    fixture->setTestFunction(failLongIntTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: LongIntTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failUnsignedLongIntMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_ULONG(1, 2);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkUnsignedLongInt)
{
    CHECK_EQUAL_C_ULONG(2, 2);
    fixture->setTestFunction(failUnsignedLongIntMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failUnsignedLongIntTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_ULONG_TEXT(1, 2, "UnsignedLongIntTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkUnsignedLongIntText)
{
    CHECK_EQUAL_C_ULONG_TEXT(2, 2, "Text");
    fixture->setTestFunction(failUnsignedLongIntTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: UnsignedLongIntTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

#ifdef CPPUTEST_USE_LONG_LONG

static void failLongLongIntMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_LONGLONG(1, 2);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkLongLongInt)
{
    CHECK_EQUAL_C_LONGLONG(2, 2);
    fixture->setTestFunction(failLongLongIntMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failLongLongIntTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_LONGLONG_TEXT(1, 2, "LongLongTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkLongLongIntText)
{
    CHECK_EQUAL_C_LONGLONG_TEXT(2, 2, "Text");
    fixture->setTestFunction(failLongLongIntTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: LongLongTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failUnsignedLongLongIntMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_ULONGLONG(1, 2);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkUnsignedLongLongInt)
{
    CHECK_EQUAL_C_ULONGLONG(2, 2);
    fixture->setTestFunction(failUnsignedLongLongIntMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failUnsignedLongLongIntTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_ULONGLONG_TEXT(1, 2, "UnsignedLongLongTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkUnsignedLongLongIntText)
{
    CHECK_EQUAL_C_ULONGLONG_TEXT(2, 2, "Text");
    fixture->setTestFunction(failUnsignedLongLongIntTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1 (0x1)>\n	but was  <2 (0x2)>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: UnsignedLongLongTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

#else

static void failLongLongIntMethod_()
{
    cpputest_longlong dummy_longlong;
    CHECK_EQUAL_C_LONGLONG(dummy_longlong, dummy_longlong);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkLongLongInt)
{
    fixture->setTestFunction(failLongLongIntMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("is not supported");
    fixture->assertPrintContains("arness_c");
}

static void failLongLongIntTextMethod_()
{
    cpputest_longlong dummy_longlong;
    CHECK_EQUAL_C_LONGLONG_TEXT(dummy_longlong, dummy_longlong, "Text");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkLongLongIntText)
{
    fixture->setTestFunction(failLongLongIntTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("is not supported");
    fixture->assertPrintContains("arness_c");
}

static void failUnsignedLongLongIntMethod_()
{
    cpputest_ulonglong dummy_ulonglong;
    CHECK_EQUAL_C_ULONGLONG(dummy_ulonglong, dummy_ulonglong);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkUnsignedLongLongInt)
{
    fixture->setTestFunction(failUnsignedLongLongIntMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("is not supported");
    fixture->assertPrintContains("arness_c");
}

static void failUnsignedLongLongIntTextMethod_()
{
    cpputest_ulonglong dummy_ulonglong;
    CHECK_EQUAL_C_ULONGLONG_TEXT(dummy_ulonglong, dummy_ulonglong, "Text");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkUnsignedLongLongIntText)
{
    fixture->setTestFunction(failUnsignedLongLongIntTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("is not supported");
    fixture->assertPrintContains("arness_c");
}

#endif

static void failRealMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_REAL(1.0, 2.0, 0.5);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkReal)
{
    CHECK_EQUAL_C_REAL(1.0, 1.1, 0.5);
    fixture->setTestFunction(failRealMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1>\n	but was  <2>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failRealTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_REAL_TEXT(1.0, 2.0, 0.5, "RealTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkRealText)
{
    CHECK_EQUAL_C_REAL_TEXT(1.0, 1.1, 0.5, "Text");
    fixture->setTestFunction(failRealTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <1>\n	but was  <2>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: RealTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failCharMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_CHAR('a', 'c');
}

TEST(TestHarness_c, checkChar)
{
    CHECK_EQUAL_C_CHAR('a', 'a');
    fixture->setTestFunction(failCharMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <a>\n	but was  <c>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failCharTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_CHAR_TEXT('a', 'c', "CharTestText");
}

TEST(TestHarness_c, checkCharText)
{
    CHECK_EQUAL_C_CHAR_TEXT('a', 'a', "Text");
    fixture->setTestFunction(failCharTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <a>\n	but was  <c>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: CharTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failUnsignedByteMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_UBYTE(254, 253);
}

TEST(TestHarness_c, checkUnsignedByte)
{
    CHECK_EQUAL_C_UBYTE(254, 254);
    fixture->setTestFunction(failUnsignedByteMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <254>\n	but was  <253>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failUnsignedByteTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_UBYTE_TEXT(254, 253, "UnsignedByteTestText");
}

TEST(TestHarness_c, checkUnsignedByteText)
{
    CHECK_EQUAL_C_UBYTE_TEXT(254, 254, "Text");
    fixture->setTestFunction(failUnsignedByteTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <254>\n	but was  <253>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: UnsignedByteTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failSignedByteMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_SBYTE(-3, -5);
}

TEST(TestHarness_c, checkSignedByte)
{
    CHECK_EQUAL_C_SBYTE(-3, -3);
    fixture->setTestFunction(failSignedByteMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <-3>\n	but was  <-5>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failSignedByteTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_SBYTE_TEXT(-3, -5, "SignedByteTestText");
}

TEST(TestHarness_c, checkSignedByteText)
{
    CHECK_EQUAL_C_SBYTE_TEXT(-3, -3, "Text");
    fixture->setTestFunction(failSignedByteTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <-3>\n	but was  <-5>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: SignedByteTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failStringMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_STRING("Hello", "Hello World");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkString)
{
    CHECK_EQUAL_C_STRING("Hello", "Hello");
    fixture->setTestFunction(failStringMethod_);
    fixture->runAllTests();

    StringEqualFailure failure(UtestShell::getCurrent(), "file", 1, "Hello", "Hello World", "");
    fixture->assertPrintContains(failure.getMessage());
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failStringTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_STRING_TEXT("Hello", "Hello World", "StringTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkStringText)
{
    CHECK_EQUAL_C_STRING_TEXT("Hello", "Hello", "Text");
    fixture->setTestFunction(failStringTextMethod_);
    fixture->runAllTests();

    StringEqualFailure failure(UtestShell::getCurrent(), "file", 1, "Hello", "Hello World", "");
    fixture->assertPrintContains(failure.getMessage());
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: StringTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failPointerMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_POINTER(NULLPTR, (void *)0x1);
}

TEST(TestHarness_c, checkPointer)
{
    CHECK_EQUAL_C_POINTER(NULLPTR, NULLPTR);
    fixture->setTestFunction(failPointerMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <0x0>\n	but was  <0x1>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failPointerTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_POINTER_TEXT(NULLPTR, (void *)0x1, "PointerTestText");
}

TEST(TestHarness_c, checkPointerText)
{
    CHECK_EQUAL_C_POINTER_TEXT(NULLPTR, NULLPTR, "Text");
    fixture->setTestFunction(failPointerTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <0x0>\n	but was  <0x1>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: PointerTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failBitsMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_BITS(0x0001, (unsigned short)0x0003, 0xFFFF);
}

TEST(TestHarness_c, checkBits)
{
    CHECK_EQUAL_C_BITS(0xABCD, (unsigned short)0xABCD, 0xFFFF);
    fixture->setTestFunction(failBitsMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <00000000 00000001>\n\tbut was  <00000000 00000011>");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failBitsTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_EQUAL_C_BITS_TEXT(0x0001, (unsigned short)0x0003, 0xFFFF, "BitsTestText");
}

TEST(TestHarness_c, checkBitsText)
{
    CHECK_EQUAL_C_BITS_TEXT(0xABCD, (unsigned short)0xABCD, 0xFFFF, "Text");
    fixture->setTestFunction(failBitsTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("expected <00000000 00000001>\n\tbut was  <00000000 00000011>");
    fixture->assertPrintContains("arness_c");
    fixture->assertPrintContains("Message: BitsTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    FAIL_TEXT_C("Booo");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkFailText)
{
    fixture->setTestFunction(failTextMethod_);
    fixture->runAllTests();
    fixture->assertPrintContains("Booo");
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void failMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    FAIL_C();
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkFail)
{
    fixture->setTestFunction(failMethod_);
    fixture->runAllTests();
    LONGS_EQUAL(1, fixture->getFailureCount());
    fixture->assertPrintContains("arness_c");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void CheckMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_C(false);
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkCheck)
{
    CHECK_C(true);
    fixture->setTestFunction(CheckMethod_);
    fixture->runAllTests();
    LONGS_EQUAL(1, fixture->getFailureCount());
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void CheckTextMethod_()
{
    HasTheDestructorBeenCalledChecker checker;
    CHECK_C_TEXT(false, "CheckTestText");
} // LCOV_EXCL_LINE

TEST(TestHarness_c, checkCheckText)
{
    CHECK_C_TEXT(true, "Text");
    fixture->setTestFunction(CheckTextMethod_);
    fixture->runAllTests();
    LONGS_EQUAL(1, fixture->getFailureCount());
    fixture->assertPrintContains("Message: CheckTestText");
    CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

#if CPPUTEST_USE_MEM_LEAK_DETECTION

TEST(TestHarness_c, cpputest_malloc_out_of_memory)
{
    cpputest_malloc_set_out_of_memory();
    CHECK(NULLPTR == cpputest_malloc(100));

    cpputest_malloc_set_not_out_of_memory();
    void * mem = cpputest_malloc(100);
    CHECK(NULLPTR != mem);
    cpputest_free(mem);
}

TEST(TestHarness_c, cpputest_malloc_out_of_memory_after_n_mallocs)
{
    cpputest_malloc_set_out_of_memory_countdown(3);
    void * m1 = cpputest_malloc(10);
    void * m2 = cpputest_malloc(11);
    void * m3 = cpputest_malloc(12);
    CHECK(m1 != NULLPTR);
    CHECK(m2 != NULLPTR);
    CHECK(m3 == NULLPTR);
    cpputest_malloc_set_not_out_of_memory();
    cpputest_free(m1);
    cpputest_free(m2);
}

TEST(TestHarness_c, cpputest_malloc_out_of_memory_after_0_mallocs)
{
    cpputest_malloc_set_out_of_memory_countdown(0);
    void * m1 = cpputest_malloc(10);
    CHECK(m1 == NULLPTR);
    cpputest_malloc_set_not_out_of_memory();
}

TEST(TestHarness_c, count_mallocs)
{
    cpputest_malloc_count_reset();
    void * m1 = cpputest_malloc(10);
    void * m2 = cpputest_malloc(11);
    void * m3 = cpputest_malloc(12);
    cpputest_free(m1);
    cpputest_free(m2);
    cpputest_free(m3);
    LONGS_EQUAL(3, cpputest_malloc_get_count());
}

#ifdef CPPUTEST_USE_STRDUP_MACROS

TEST(TestHarness_c, cpputest_strdup)
{
    char * mem = cpputest_strdup("0123456789");
    CHECK(NULLPTR != mem);
    STRCMP_EQUAL("0123456789", mem);
    cpputest_free(mem);
}

TEST(TestHarness_c, cpputest_strndup)
{
    char * mem = cpputest_strndup("0123456789", 3);
    CHECK(NULLPTR != mem);
    STRCMP_EQUAL("012", mem);
    cpputest_free(mem);
}

#endif

TEST(TestHarness_c, cpputest_calloc)
{
    void * mem = cpputest_calloc(10, 10);
    CHECK(NULLPTR != mem);
    cpputest_free(mem);
}

TEST(TestHarness_c, cpputest_realloc_larger)
{
    const char* number_string = "123456789";

    char* mem1 = (char*) cpputest_malloc(10);

    SimpleString::StrNCpy(mem1, number_string, 10);

    CHECK(mem1 != NULLPTR);

    char* mem2 = (char*) cpputest_realloc(mem1, 1000);

    CHECK(mem2 != NULLPTR);
    STRCMP_EQUAL(number_string, mem2);

    cpputest_free(mem2);
}

#include "CppUTest/MemoryLeakDetector.h"

TEST(TestHarness_c, macros)
{
#if CPPUTEST_USE_MALLOC_MACROS
    MemoryLeakDetector* memLeakDetector = MemoryLeakWarningPlugin::getGlobalDetector();
    size_t memLeaks = memLeakDetector->totalMemoryLeaks(mem_leak_period_checking);
#endif
    void* mem1 = malloc(10);
    void* mem2 = calloc(10, 20);
    void* mem3 = realloc(mem2, 100);
#if CPPUTEST_USE_MALLOC_MACROS
    LONGS_EQUAL(memLeaks + 2, memLeakDetector->totalMemoryLeaks(mem_leak_period_checking));
#endif
    free(mem1);
    free(mem3);
#if CPPUTEST_USE_MALLOC_MACROS
    LONGS_EQUAL(memLeaks, memLeakDetector->totalMemoryLeaks(mem_leak_period_checking));
#endif

}

TEST(TestHarness_c, callocInitializedToZero)
{
    char* mem = (char*) calloc(20, sizeof(char));
    for (int i = 0; i < 20; i++)
        CHECK(mem[i] == 0);
    free(mem);
}

TEST(TestHarness_c, callocShouldReturnNULLWhenOutOfMemory)
{
    cpputest_malloc_set_out_of_memory_countdown(0);
    void * m = cpputest_calloc(1, 1);
    CHECK(m == NULLPTR);
    cpputest_malloc_set_not_out_of_memory();
}
#endif
