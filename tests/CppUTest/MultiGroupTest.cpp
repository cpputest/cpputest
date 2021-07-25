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
#include "CppUTest/MultiGroup.h"

unsigned int initalCondition = 0;

#define TOTAL_TEST_GROUPS 4
DEFINE_MULTI_GROUP(MyMultiTestGroupName)

TEST_GROUP_SETUP(MyMultiTestGroupName, 1)
{
    initalCondition = 1;
}

TEST_GROUP_SETUP(MyMultiTestGroupName, 2)
{
    initalCondition = 2;
}

TEST_GROUP_SETUP(MyMultiTestGroupName, 3)
{
    initalCondition = 3;
}

TEST_GROUP_SETUP(MyMultiTestGroupName, 4)
{
    initalCondition = 4;
}

TEST_GROUPS_TEARDOWN(MyMultiTestGroupName)
{
    initalCondition = 0;
}

/*
 * This single test will run 4 times, with different
 * inital condition everytime.
 */
TEST_ON_ALL_GROUPS(MyMultiTestGroupName, VerifyInitalConition)
{
    CHECK(initalCondition == TEST_GROUP_NUM);
}
