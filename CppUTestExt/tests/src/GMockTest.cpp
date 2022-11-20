/*
 * Copyright (c) 2011, Michael Feathers, James Grenning and Bas Vodde
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

#ifdef CPPUTEST_INCLUDE_GTEST_TESTS

#include "CppUTestExt/GMock.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestTestingFixture.h"


TEST_GROUP(GMock)
{
    TestTestingFixture *fixture;
    void setup() _override
    {
        fixture = new TestTestingFixture;
    }

    void teardown() _override
    {
        delete fixture;

    }
};

class myMock
{
public:
    MOCK_METHOD0(methodName, int());
};

static void failedMockCall()
{
    myMock mock;
    EXPECT_CALL(mock, methodName()).WillOnce(Return(1));
}

TEST(GMock, GMockFailuresWorkAsExpected)
{
    fixture->setTestFunction(failedMockCall);
    fixture->runAllTests();
    LONGS_EQUAL(1, fixture->getFailureCount());
}

static void failedMockCallAfterOneSuccess()
{
    myMock mock;
    EXPECT_CALL(mock, methodName()).Times(2).WillRepeatedly(Return(1));

    mock.methodName();
}

TEST(GMock, GMockFailuresWorkAsExpectedWithTwoExpectedCallButJustOneActual)
{
    fixture->setTestFunction(failedMockCallAfterOneSuccess);
    fixture->runAllTests();
    LONGS_EQUAL(1, fixture->getFailureCount());
}

TEST(GMock, GMockNiceMocksWorkFine)
{
    NiceMock<myMock> mock;
    mock.methodName();
}

#endif
