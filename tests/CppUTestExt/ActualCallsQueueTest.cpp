/*
 * Copyright (c) 2016, Jesus Gonzalez
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
#include "CppUTestExt/MockActualCallsQueue.h"
#include "CppUTestExt/MockCheckedActualCall.h"
#include "CppUTestExt/MockFailure.h"
#include "MockFailureReporterForTest.h"

TEST_GROUP(MockActualCallsQueue)
{
    MockSupport testMock;

    void teardown()
    {
        CHECK_NO_MOCK_FAILURE();
        testMock.clear();
    }
};

TEST(MockActualCallsQueue, emptyQueue)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(true);
    CHECK_TRUE(actualCalls->isEmpty());
    LONGS_EQUAL(0, actualCalls->size());
    LONGS_EQUAL((unsigned int)-1, actualCalls->getMaxSize());
    delete actualCalls;
}

TEST(MockActualCallsQueue, pushBack_Owner)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(true);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);

    actualCalls->pushBack(call1);
    CHECK_FALSE(actualCalls->isEmpty());
    LONGS_EQUAL(1, actualCalls->size());

    actualCalls->pushBack(call2);
    LONGS_EQUAL(2, actualCalls->size());

    delete actualCalls;
}

TEST(MockActualCallsQueue, pushBack_NotOwner)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(false);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);

    actualCalls->pushBack(call1);
    CHECK_FALSE(actualCalls->isEmpty());
    LONGS_EQUAL(1, actualCalls->size());

    actualCalls->pushBack(call2);
    LONGS_EQUAL(2, actualCalls->size());

    delete actualCalls;
    delete call1;
    delete call2;
}

TEST(MockActualCallsQueue, pushBack_MaxSizeReached_Owner)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(true, 2);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    MockCheckedActualCall* call3 = new MockCheckedActualCall(3, NULL, testMock);

    actualCalls->pushBack(call1);
    CHECK_FALSE(actualCalls->isEmpty());
    LONGS_EQUAL(1, actualCalls->size());

    actualCalls->pushBack(call2);
    LONGS_EQUAL(2, actualCalls->size());

    actualCalls->pushBack(call3);
    LONGS_EQUAL(2, actualCalls->size());

    delete actualCalls;
}

TEST(MockActualCallsQueue, pushBack_MaxSizeReached_NotOwner)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(false, 2);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    MockCheckedActualCall* call3 = new MockCheckedActualCall(3, NULL, testMock);

    actualCalls->pushBack(call1);
    CHECK_FALSE(actualCalls->isEmpty());
    LONGS_EQUAL(1, actualCalls->size());

    actualCalls->pushBack(call2);
    LONGS_EQUAL(2, actualCalls->size());

    delete actualCalls;
    delete call1;
    delete call2;
    delete call3;
}

TEST(MockActualCallsQueue, clear_Owner)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(true);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    actualCalls->pushBack(call1);
    actualCalls->pushBack(call2);

    actualCalls->clear();
    CHECK_TRUE(actualCalls->isEmpty());
    LONGS_EQUAL(0, actualCalls->size());

    delete actualCalls;
}

TEST(MockActualCallsQueue, clear_NotOwner)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(false);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    actualCalls->pushBack(call1);
    actualCalls->pushBack(call2);

    actualCalls->clear();
    CHECK_TRUE(actualCalls->isEmpty());
    LONGS_EQUAL(0, actualCalls->size());

    delete actualCalls;
    delete call1;
    delete call2;
}

TEST(MockActualCallsQueue, popFront_Owner)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(true);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    actualCalls->pushBack(call1);
    actualCalls->pushBack(call2);

    MockCheckedActualCall* returnedCall = actualCalls->popFront();
    LONGS_EQUAL(1, actualCalls->size());
    POINTERS_EQUAL(call1, returnedCall);

    delete actualCalls;
    delete call1;
}

TEST(MockActualCallsQueue, popFront_NotOwner)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(false);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    actualCalls->pushBack(call1);
    actualCalls->pushBack(call2);

    MockCheckedActualCall* returnedCall = actualCalls->popFront();
    LONGS_EQUAL(1, actualCalls->size());
    POINTERS_EQUAL(call1, returnedCall);

    returnedCall = actualCalls->popFront();
    LONGS_EQUAL(0, actualCalls->size());
    CHECK_TRUE(actualCalls->isEmpty());
    POINTERS_EQUAL(call2, returnedCall);

    delete actualCalls;
    delete call1;
    delete call2;
}

TEST(MockActualCallsQueue, popFront_OnEmptyList)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(false);

    MockCheckedActualCall* returnedCall = actualCalls->popFront();
    LONGS_EQUAL(0, actualCalls->size());
    CHECK_TRUE(actualCalls->isEmpty());
    POINTERS_EQUAL(NULL, returnedCall);

    delete actualCalls;
}

TEST(MockActualCallsQueue, pushBackAll_Empty)
{
    MockActualCallsQueue* actualCalls1 = new MockActualCallsQueue(false);
    MockActualCallsQueue* actualCalls2 = new MockActualCallsQueue(false);

    actualCalls2->pushBackAll(actualCalls1);
    CHECK_TRUE(actualCalls2->isEmpty());
    LONGS_EQUAL(0, actualCalls2->size());

    delete actualCalls1;
    delete actualCalls2;
}

TEST(MockActualCallsQueue, pushBackAll_NotEmpty_FromNotOwner)
{
    MockActualCallsQueue* actualCalls1 = new MockActualCallsQueue(false);
    MockActualCallsQueue* actualCalls2 = new MockActualCallsQueue(false);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    actualCalls1->pushBack(call1);
    actualCalls1->pushBack(call2);

    actualCalls2->pushBackAll(*actualCalls1);
    CHECK_FALSE(actualCalls2->isEmpty());
    LONGS_EQUAL(2, actualCalls2->size());

    delete actualCalls1;
    delete actualCalls2;
    delete call1;
    delete call2;
}

TEST(MockActualCallsQueue, pushBackAll_NotEmpty_FromOwner)
{
    MockActualCallsQueue* actualCalls1 = new MockActualCallsQueue(true);
    MockActualCallsQueue* actualCalls2 = new MockActualCallsQueue(false);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    actualCalls1->pushBack(call1);
    actualCalls1->pushBack(call2);

    actualCalls2->pushBackAll(*actualCalls1);
    CHECK_FALSE(actualCalls2->isEmpty());
    LONGS_EQUAL(2, actualCalls2->size());

    delete actualCalls2;
    delete actualCalls1;
}

TEST(MockActualCallsQueue, setMaxSize_GreaterThanCurrent)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(false, 2);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    actualCalls->pushBack(call1);
    actualCalls->pushBack(call2);

    LONGS_EQUAL(2, actualCalls->size());
    actualCalls->setMaxSize(5);
    LONGS_EQUAL(2, actualCalls->size());

    delete actualCalls;
    delete call1;
    delete call2;
}

TEST(MockActualCallsQueue, setMaxSize_LowerThanCurrent)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(false);
    MockCheckedActualCall* call1 = new MockCheckedActualCall(1, NULL, testMock);
    MockCheckedActualCall* call2 = new MockCheckedActualCall(2, NULL, testMock);
    MockCheckedActualCall* call3 = new MockCheckedActualCall(3, NULL, testMock);
    actualCalls->pushBack(call1);
    actualCalls->pushBack(call2);
    actualCalls->pushBack(call3);

    LONGS_EQUAL(3, actualCalls->size());
    actualCalls->setMaxSize(2);
    LONGS_EQUAL(2, actualCalls->size());

    MockCheckedActualCall* returnedCall = actualCalls->popFront();
    LONGS_EQUAL(1, actualCalls->size());
    POINTERS_EQUAL(call2, returnedCall);

    delete actualCalls;
    delete call1;
    delete call2;
    delete call3;
}

TEST(MockActualCallsQueue, callToString_Empty)
{
    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(true);

    STRCMP_EQUAL("###<none>", actualCalls->toString("###").asCharString());

    delete actualCalls;
}

TEST(MockActualCallsQueue, callToString_NotEmpty)
{
    MockCheckedExpectedCall expectedCall1(1, 1);
    expectedCall1.withName("foo");
    MockCheckedExpectedCall expectedCall2(1, 1);
    expectedCall2.withName("bar");

    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(true);
    MockCheckedActualCall* actualCall1 = new MockCheckedActualCallForTest(1, "", &expectedCall1);
    MockCheckedActualCall* actualCall2 = new MockCheckedActualCallForTest(2, "", &expectedCall2);
    actualCalls->pushBack(actualCall1);
    actualCalls->pushBack(actualCall2);

    SimpleString expectedString = StringFromFormat("$\t%s\n$\t%s", actualCall1->toString().asCharString(), actualCall2->toString().asCharString());
    STRCMP_EQUAL(expectedString.asCharString(), actualCalls->toString("$\t").asCharString());

    delete actualCalls;
}

TEST(MockActualCallsQueue, toStringFilterByFunction_NoMatches)
{
    MockCheckedExpectedCall expectedCall1(1, 1);
    expectedCall1.withName("foo");
    MockCheckedExpectedCall expectedCall2(1, 1);
    expectedCall2.withName("bar");

    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(true);
    MockCheckedActualCall* actualCall1 = new MockCheckedActualCallForTest(1, "", &expectedCall1);
    MockCheckedActualCall* actualCall2 = new MockCheckedActualCallForTest(2, "", &expectedCall2);
    actualCalls->pushBack(actualCall1);
    actualCalls->pushBack(actualCall2);

    STRCMP_EQUAL("[@]<none>", actualCalls->toStringFilterByFunction("foobar", "[@]").asCharString());

    delete actualCalls;
}

TEST(MockActualCallsQueue, toStringFilterByFunction_WithMatches)
{
    MockCheckedExpectedCall expectedCall1(1, 1);
    expectedCall1.withName("foo");
    MockCheckedExpectedCall expectedCall2(1, 1);
    expectedCall2.withName("bar");

    MockActualCallsQueue* actualCalls = new MockActualCallsQueue(true);
    MockCheckedActualCall* actualCall1 = new MockCheckedActualCallForTest(1, "", &expectedCall1);
    MockCheckedActualCall* actualCall2 = new MockCheckedActualCallForTest(2, "", &expectedCall2);
    actualCalls->pushBack(actualCall1);
    actualCalls->pushBack(actualCall2);

    SimpleString expectedString = StringFromFormat("\t~ %s", actualCall1->toString().asCharString());
    STRCMP_EQUAL(expectedString.asCharString(), actualCalls->toStringFilterByFunction("foo", "\t~ ").asCharString());

    delete actualCalls;
}
