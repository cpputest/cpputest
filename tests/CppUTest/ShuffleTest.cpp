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

static const int maxNumItems = 3;

TEST_GROUP(ShuffleTest)
{
    int x0;
    int x1;
    int x2;

    void* elements[maxNumItems];

    void setup()
    {
        x0 = 0;
        x1 = 1;
        x2 = 2;

        elements[0] = &x0;
        elements[1] = &x1;
        elements[2] = &x2;
    }

};

static int getZero()
{
    return 0;
}

static int getOne()
{
    return 1;
}

static int getValueExceedingMaxIdx()
{
    return maxNumItems + 1;
}

TEST(ShuffleTest, ShuffleListTestWithNoElementsInList)
{
    UT_PTR_SET(PlatformSpecificRand, getValueExceedingMaxIdx);
    TestRegistry::shuffleList(0, elements);

    CHECK(elements[0] == &x0);
    CHECK(elements[1] == &x1);
    CHECK(elements[2] == &x2);
}

TEST(ShuffleTest, ShuffleListTestWithRandomAlwaysReturningZero)
{
    UT_PTR_SET(PlatformSpecificRand, getZero);
    TestRegistry::shuffleList(3, elements);

    CHECK(elements[0] == &x1);
    CHECK(elements[1] == &x2);
    CHECK(elements[2] == &x0);
}

// swaps with 4 mod 3 (1) then 4 mod 2 (0): 1, [2], [0] --> [1], [0], 2 --> 0, 1, 2
TEST(ShuffleTest, ShuffleListTestWithRandomAlwaysReturningOne)
{
    UT_PTR_SET(PlatformSpecificRand, getOne);
    TestRegistry::shuffleList(3, elements);

    CHECK(elements[0] == &x0);
    CHECK(elements[1] == &x2);
    CHECK(elements[2] == &x1);
}

