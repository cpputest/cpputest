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

#include "CppUTest/TestResult.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"

TEST_GROUP(TestFailureList)
{    
    UtestShell* tst;
    TestFailure *f;
    TestFailure *f2;
    TestFailure *f3;

    void setup()
    {
        tst = new UtestShell("group", "test", "file", 10);
        f = new TestFailure(tst, "failfile", 20, "message");
        f2 = new TestFailure(tst, "file", 20, "message");
        f3 = new TestFailure(tst, "file", 2, "message");

    }

    void teardown()
    {
        delete tst;
        delete f;
        delete f2;
        delete f3;
    }
};

TEST(TestFailureList, puchBackFailureList)
{
    TestFailureList list;
    list.pushBack(*f);
    list.pushBack(*f2);
    list.pushBack(*f3);
    int count = 0;
    while (!list.empty()) {
		list.erase(list.begin());
        ++count;
	}
    CHECK(count == 3);
}

TEST(TestFailureList, emptyClearFailureList)
{
    TestFailureList list;
    CHECK(list.empty() == true);
    list.pushBack(*f);
    list.pushBack(*f2);
    list.pushBack(*f3);
    int count = 0;
    while (!list.empty()) {
		list.erase(list.begin());
        ++count;
	}
    CHECK(count == 3);
    list.clear();
    CHECK(list.empty() == true);
}
