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
#include "CppUTestExt/MockExpectedFunctionsList.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"

TEST_GROUP(MockExpectedFunctionsList)
{
	MockExpectedFunctionsList * list;
	MockExpectedFunctionCall* call1;
	MockExpectedFunctionCall* call2;
	MockExpectedFunctionCall* call3;
	void setup()
	{
		list = new MockExpectedFunctionsList;
		call1 = new MockExpectedFunctionCall;
		call2 = new MockExpectedFunctionCall;
		call3 = new MockExpectedFunctionCall;
	}
	void teardown()
	{
		delete call1;
		delete call2;
		delete call3;
		delete list;
	}
};

TEST(MockExpectedFunctionsList, emptyList)
{
	CHECK(! list->hasUnfullfilledExpectations());
	CHECK(list->getExpectedCall() == NULL);
}

TEST(MockExpectedFunctionsList, addingCalls)
{
	list->addExpectedCall(call1);
	list->addExpectedCall(call2);
	LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedFunctionsList, listWithFulfilledExpectationHasNoUnfillfilledOnes)
{
	call1->setFulfilled();
	call2->setFulfilled();
	list->addExpectedCall(call1);
	list->addExpectedCall(call2);
	CHECK(! list->hasUnfullfilledExpectations());
}

TEST(MockExpectedFunctionsList, listWithUnFulfilledExpectationHasNoUnfillfilledOnes)
{
	call1->setFulfilled();
	call3->setFulfilled();
	list->addExpectedCall(call1);
	list->addExpectedCall(call2);
	list->addExpectedCall(call3);
	CHECK(list->hasUnfullfilledExpectations());
}

TEST(MockExpectedFunctionsList, deleteAllExpectationsAndClearList)
{
	list->addExpectedCall(new MockExpectedFunctionCall);
	list->addExpectedCall(new MockExpectedFunctionCall);
	list->deleteAllExpectationsAndClearList();
}

TEST(MockExpectedFunctionsList, removeAllExpectationsExceptThisThatRelateTo)
{
	call1->withName("relate");
	call2->withName("unrelate");
	call3->withName("relate");
	list->addExpectedCall(call1);
	list->addExpectedCall(call2);
	list->addExpectedCall(call3);
	list->removeAllExpectationsExceptThisThatRelateTo("unrelate");
	LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedFunctionsList, removeAllExpectationsExceptThisThatRelateToTheWoleList)
{
	call1->withName("relate");
	call2->withName("relate");
	call3->withName("relate");
	list->addExpectedCall(call1);
	list->addExpectedCall(call2);
	list->addExpectedCall(call3);
	list->removeAllExpectationsExceptThisThatRelateTo("unrelate");
	LONGS_EQUAL(0, list->size());
}

TEST(MockExpectedFunctionsList, removeAllExpectationsExceptThisThatRelateToFirstOne)
{
	call1->withName("relate");
	call2->withName("unrelate");
	list->addExpectedCall(call1);
	list->addExpectedCall(call2);
	list->removeAllExpectationsExceptThisThatRelateTo("unrelate");
	LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedFunctionsList, removeAllExpectationsExceptThisThatRelateToLastOne)
{
	call1->withName("unrelate");
	call2->withName("relate");
	list->addExpectedCall(call1);
	list->addExpectedCall(call2);
	list->removeAllExpectationsExceptThisThatRelateTo("unrelate");
	LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedFunctionsList, addUnfilfilledExpectationsWithEmptyList)
{
	MockExpectedFunctionsList newList;
	list->addUnfilfilledExpectationsToList(&newList);
	LONGS_EQUAL(0, newList.size());
}

TEST(MockExpectedFunctionsList, addUnfilfilledExpectationsMultipleUnfulfilledExpectations)
{
	call2->setFulfilled();
	list->addExpectedCall(call1);
	list->addExpectedCall(call2);
	list->addExpectedCall(call3);
	MockExpectedFunctionsList newList;
	list->addUnfilfilledExpectationsToList(&newList);
	LONGS_EQUAL(2, newList.size());
}

TEST(MockExpectedFunctionsList, amountOfExpectationsFor)
{
	call1->withName("foo");
	call2->withName("bar");
	list->addExpectedCall(call1);
	list->addExpectedCall(call2);
	LONGS_EQUAL(1, list->amountOfExpectationsFor("bar"));
}

TEST(MockExpectedFunctionsList, amountOfExpectationsForHasNone)
{
	call1->withName("foo");
	list->addExpectedCall(call1);
	LONGS_EQUAL(0, list->amountOfExpectationsFor("bar"));
}
