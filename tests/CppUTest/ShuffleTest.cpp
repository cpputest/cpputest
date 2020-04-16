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
#include "CppUTest/TestRegistry.h"
#include "CppUTest/PlatformSpecificFunctions.h"

static int getZero()
{
    return 0;
}

static int getOne()
{
    return 1;
}

TEST_GROUP(ShuffleTest)
{
    UtestShell* test0;
    UtestShell* test1;
    UtestShell* test2;

    void setup()
    {
        test0 = new IgnoredUtestShell();
        test1 = new IgnoredUtestShell();
        test2 = new IgnoredUtestShell();

        test0->addTest(test1);
        test1->addTest(test2);
    }

    void teardown()
    {
        delete test0;
        delete test1;
        delete test2;
    }
};


TEST(ShuffleTest, empty)
{
    UtestShellPointerArray tests(NULLPTR);
    tests.shuffle(0);
    CHECK(NULL == tests.getFirstTest());
}

TEST(ShuffleTest, testsAreInOrder)
{
    UtestShellPointerArray tests(test0);
    CHECK(tests.get(0) == test0);
    CHECK(tests.get(1) == test1);
    CHECK(tests.get(2) == test2);
}

TEST(ShuffleTest, relinkingTestsWillKeepThemTheSameWhenNothingWasDone)
{
    UtestShellPointerArray tests(test0);
    tests.relinkTestsInOrder();
    CHECK(tests.get(0) == test0);
    CHECK(tests.get(1) == test1);
    CHECK(tests.get(2) == test2);
}


TEST(ShuffleTest, firstTestisNotTheFirstTestWithSeed1234)
{
    UtestShellPointerArray tests(test0);
    tests.shuffle(1234);
    CHECK(tests.getFirstTest() != test0);
}

TEST(ShuffleTest, ShuffleListTestWithRandomAlwaysReturningZero)
{
    UT_PTR_SET(PlatformSpecificRand, getZero);

    UtestShellPointerArray tests(test0);
    tests.shuffle(3);
    CHECK(tests.get(0) == test1);
    CHECK(tests.get(1) == test2);
    CHECK(tests.get(2) == test0);
}

// swaps with 4 mod 3 (1) then 4 mod 2 (0): 1, [2], [0] --> [1], [0], 2 --> 0, 1, 2
TEST(ShuffleTest, ShuffleListTestWithRandomAlwaysReturningOne)
{
    UT_PTR_SET(PlatformSpecificRand, getOne);

    UtestShellPointerArray tests(test0);
    tests.shuffle(3);
    CHECK(tests.get(0) == test0);
    CHECK(tests.get(1) == test2);
    CHECK(tests.get(2) == test1);
}

TEST(ShuffleTest, reverse)
{
    UT_PTR_SET(PlatformSpecificRand, getOne);

    UtestShellPointerArray tests(test0);
    tests.reverse();
    CHECK(tests.get(0) == test2);
    CHECK(tests.get(1) == test1);
    CHECK(tests.get(2) == test0);
}

