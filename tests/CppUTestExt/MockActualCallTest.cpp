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
#include "CppUTestExt/MockCheckedActualCall.h"
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockExpectedCallsList.h"
#include "CppUTestExt/MockFailure.h"
#include "MockFailureTest.h"

TEST_GROUP(MockCheckedActualCall)
{
    MockExpectedCallsList* emptyList;
    MockExpectedCallsList* list;
    MockFailureReporter* reporter;

    void setup()
    {
        emptyList = new MockExpectedCallsList;
        list = new MockExpectedCallsList;
        reporter = MockFailureReporterForTest::getReporter();
    }

    void teardown()
    {
        CHECK_NO_MOCK_FAILURE();
        delete emptyList;
        delete list;
    }
};

TEST(MockCheckedActualCall, unExpectedCall)
{
    MockCheckedActualCall actualCall(1, reporter, *emptyList);
    actualCall.withName("unexpected");

    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "unexpected", *list);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, unExpectedParameterName)
{
    MockCheckedExpectedCall call1;
    call1.withName("func");
    list->addExpectedCall(&call1);

    MockCheckedActualCall actualCall(1, reporter, *list);
    actualCall.withName("func").withParameter("integer", 1);

    MockNamedValue parameter("integer");
    parameter.setValue(1);

    MockUnexpectedInputParameterFailure expectedFailure(mockFailureTest(), "func", parameter, *list);
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, multipleSameFunctionsExpectingAndHappenGradually)
{
    MockCheckedExpectedCall* call1 = new MockCheckedExpectedCall();
    MockCheckedExpectedCall* call2 = new MockCheckedExpectedCall();
    call1->withName("func");
    call2->withName("func");
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);

    MockCheckedActualCall actualCall1(1, reporter, *list);
    MockCheckedActualCall actualCall2(2, reporter, *list);

    LONGS_EQUAL(2, list->amountOfUnfulfilledExpectations());
    actualCall1.withName("func");
    LONGS_EQUAL(1, list->amountOfUnfulfilledExpectations());
    actualCall2.withName("func");
    LONGS_EQUAL(0, list->amountOfUnfulfilledExpectations());

    list->deleteAllExpectationsAndClearList();
}
