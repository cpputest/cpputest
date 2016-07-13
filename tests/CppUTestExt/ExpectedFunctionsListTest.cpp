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
#include "CppUTestExt/MockExpectedCallsList.h"
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockFailure.h"
#include "MockFailureReporterForTest.h"

TEST_GROUP(MockExpectedCallsList)
{
    MockExpectedCallsList * list;
    MockCheckedExpectedCall* call1;
    MockCheckedExpectedCall* call2;
    MockCheckedExpectedCall* call3;
    MockCheckedExpectedCall* call4;
    void setup()
    {
        list = new MockExpectedCallsList;
        call1 = new MockCheckedExpectedCall;
        call2 = new MockCheckedExpectedCall;
        call3 = new MockCheckedExpectedCall;
        call4 = new MockCheckedExpectedCall;
        call1->withName("foo");
        call2->withName("bar");
        call3->withName("boo");
    }
    void teardown()
    {
        delete call1;
        delete call2;
        delete call3;
        delete call4;
        delete list;
        CHECK_NO_MOCK_FAILURE();
    }
};

TEST(MockExpectedCallsList, emptyList)
{
    CHECK(! list->hasUnfulfilledExpectations());
    CHECK(! list->hasFinalizedMatchingExpectations());
    LONGS_EQUAL(0, list->size());
}

TEST(MockExpectedCallsList, addingCalls)
{
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedCallsList, listWithFulfilledExpectationHasNoUnfulfilledOnes)
{
    call1->callWasMade(1);
    call2->callWasMade(2);
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    CHECK(! list->hasUnfulfilledExpectations());
}

TEST(MockExpectedCallsList, listWithFulfilledExpectationButOutOfOrder)
{
    call1->withCallOrder(1);
    call2->withCallOrder(2);
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    call2->callWasMade(1);
    call1->callWasMade(2);
    CHECK(! list->hasUnfulfilledExpectations());
    CHECK(list->hasCallsOutOfOrder());
}

TEST(MockExpectedCallsList, listWithUnFulfilledExpectationHasNoUnfillfilledOnes)
{
    call1->callWasMade(1);
    call3->callWasMade(2);
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->addExpectedCall(call3);
    CHECK(list->hasUnfulfilledExpectations());
}

TEST(MockExpectedCallsList, deleteAllExpectationsAndClearList)
{
    list->addExpectedCall(new MockCheckedExpectedCall);
    list->addExpectedCall(new MockCheckedExpectedCall);
    list->deleteAllExpectationsAndClearList();
}

TEST(MockExpectedCallsList, onlyKeepUnmatchingExpectations)
{
    call1->withName("relate");
    call2->withName("unrelate");
    call3->withName("relate");
    call2->callWasMade(1);
    call3->callWasMade(2);
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->addExpectedCall(call3);
    list->onlyKeepUnmatchingExpectations();
    LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, onlyKeepExpectationsRelatedTo)
{
    call1->withName("relate");
    call2->withName("unrelate");
    call3->withName("unrelate");
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->addExpectedCall(call3);
    list->onlyKeepExpectationsRelatedTo("relate");
    LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, removeAllExpectationsExceptThisThatRelateToTheWoleList)
{
    call1->withName("relate");
    call2->withName("relate");
    call3->withName("relate");
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->addExpectedCall(call3);
    list->onlyKeepExpectationsRelatedTo("unrelate");
    LONGS_EQUAL(0, list->size());
}

TEST(MockExpectedCallsList, removeAllExpectationsExceptThisThatRelateToFirstOne)
{
    call1->withName("relate");
    call2->withName("unrelate");
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->onlyKeepExpectationsRelatedTo("unrelate");
    LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, removeAllExpectationsExceptThisThatRelateToLastOne)
{
    call1->withName("unrelate");
    call2->withName("relate");
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->onlyKeepExpectationsRelatedTo("unrelate");
    LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, onlyKeepExpectationsWithInputParameterName)
{
    call1->withName("func").withParameter("param", 1);
    call2->withName("func").withParameter("diffname", 1);
    call3->withName("func").withParameter("diffname", 1);
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->addExpectedCall(call3);
    list->onlyKeepExpectationsWithInputParameterName("diffname");
    LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedCallsList, onlyKeepExpectationsWithInputParameter)
{
    MockNamedValue parameter("diffname");
    parameter.setValue(1);
    call1->withName("func").withParameter("param", 1);
    call2->withName("func").withParameter("diffname", 1);
    call3->withName("func").withParameter("diffname", 1);
    call4->withName("func").withParameter("diffname", 2);
    call3->callWasMade(1);
    call3->inputParameterWasPassed("diffname");
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->addExpectedCall(call3);
    list->addExpectedCall(call4);
    list->onlyKeepExpectationsWithInputParameter(parameter);
    LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedCallsList, addPotentiallyMatchingExpectationsWithEmptyList)
{
    MockExpectedCallsList newList;
    newList.addPotentiallyMatchingExpectations(*list);
    LONGS_EQUAL(0, newList.size());
}

TEST(MockExpectedCallsList, addPotentiallyMatchingExpectationsMultipleUnmatchedExpectations)
{
    call2->callWasMade(1);
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->addExpectedCall(call3);
    MockExpectedCallsList newList;
    newList.addPotentiallyMatchingExpectations(*list);
    LONGS_EQUAL(2, newList.size());
}

TEST(MockExpectedCallsList, amountOfExpectationsFor)
{
    call1->withName("foo");
    call2->withName("bar");
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    LONGS_EQUAL(1, list->amountOfExpectationsFor("bar"));
}

TEST(MockExpectedCallsList, amountOfExpectationsForHasNone)
{
    call1->withName("foo");
    list->addExpectedCall(call1);
    LONGS_EQUAL(0, list->amountOfExpectationsFor("bar"));
}

TEST(MockExpectedCallsList, callToStringForUnfulfilledFunctions)
{
    call1->withName("foo");
    call2->withName("bar");
    call3->withName("blah");
    call3->callWasMade(1);

    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->addExpectedCall(call3);

    SimpleString expectedString;
    expectedString = StringFromFormat("%s\n%s", call1->callToString().asCharString(), call2->callToString().asCharString());
    STRCMP_EQUAL(expectedString.asCharString(), list->unfulfilledCallsToString().asCharString());
}

TEST(MockExpectedCallsList, callToStringForFulfilledFunctions)
{
    call1->withName("foo");
    call2->withName("bar");

    call2->callWasMade(1);
    call1->callWasMade(2);

    list->addExpectedCall(call1);
    list->addExpectedCall(call2);

    SimpleString expectedString;
    expectedString = StringFromFormat("%s\n%s", call2->callToString().asCharString(), call1->callToString().asCharString());
    STRCMP_EQUAL(expectedString.asCharString(), list->fulfilledCallsToString().asCharString());
}

TEST(MockExpectedCallsList, removeOneFinalizedMatchingExpectationFromEmptyList)
{
    POINTERS_EQUAL(NULL, list->removeFirstFinalizedMatchingExpectation());
}

TEST(MockExpectedCallsList, getOneMatchingExpectationFromEmptyList)
{
    POINTERS_EQUAL(NULL, list->getFirstMatchingExpectation());
}

TEST(MockExpectedCallsList, toStringOnEmptyList)
{
    STRCMP_EQUAL("<none>", list->unfulfilledCallsToString().asCharString());
}

TEST(MockExpectedCallsList, hasFinalizedMatchingExpectations_emptyList)
{
    CHECK(! list->hasFinalizedMatchingExpectations());
}

TEST(MockExpectedCallsList, hasFinalizedMatchingExpectations_listHasNonMatchingCalls)
{
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    list->addExpectedCall(call3);

    CHECK(! list->hasFinalizedMatchingExpectations());
}

TEST(MockExpectedCallsList, hasFinalizedMatchingExpectations_listHasMatchingButNotFinalizedCall)
{
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    call1->ignoreOtherParameters();
    call1->callWasMade(1);

    CHECK(! list->hasFinalizedMatchingExpectations());
}

TEST(MockExpectedCallsList, hasFinalizedMatchingExpectations_listHasFinalizedCallThatIgnoresParameters)
{
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    call1->ignoreOtherParameters();
    call1->callWasMade(1);
    call1->finalizeActualCallMatch();

    CHECK(list->hasFinalizedMatchingExpectations());
}

TEST(MockExpectedCallsList, hasFinalizedMatchingExpectations_listHasFinalizedCallThatDoesntIgnoreParameters)
{
    list->addExpectedCall(call1);
    list->addExpectedCall(call2);
    call1->withParameter("param", 1);
    call1->callWasMade(1);
    call1->inputParameterWasPassed("param");

    CHECK(list->hasFinalizedMatchingExpectations());
}
