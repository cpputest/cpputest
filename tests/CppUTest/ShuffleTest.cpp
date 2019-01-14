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

#include "CppUTest/Shuffle.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"

TEST_GROUP(ShuffleTest)
{
};

static const int maxNumItems = 3;

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

TEST(ShuffleTest, ShuffleListTest)
{
    int x0 = 0;
    int x1 = 1;
    int x2 = 2;
    void* tests[maxNumItems] = {&x0, &x1, &x2};

    // check no-op
    shuffle_list(getValueExceedingMaxIdx, 0, tests);
    CHECK(tests[0] == &x0);
    CHECK(tests[1] == &x1);
    CHECK(tests[2] == &x2);

    // swap element with itself: 0, [1], 2 --> 0, 1, 2
    shuffle_list(getOne, 1, tests);
    CHECK(tests[0] == &x0);
    CHECK(tests[1] == &x1);
    CHECK(tests[2] == &x2);

    // always swaps with element at index 0: [0], 1, [2] --> [2], [1], 0 --> 1, 2, 0
    shuffle_list(getZero, maxNumItems, tests);
    CHECK(tests[0] == &x1);
    CHECK(tests[1] == &x2);
    CHECK(tests[2] == &x0);

    // swaps with 4 mod 3 (1) then 4 mod 2 (0): 1, [2], [0] --> [1], [0], 2 --> 0, 1, 2
    shuffle_list(getValueExceedingMaxIdx, maxNumItems, tests);
    CHECK(tests[0] == &x0);
    CHECK(tests[1] == &x1);
    CHECK(tests[2] == &x2);
}
