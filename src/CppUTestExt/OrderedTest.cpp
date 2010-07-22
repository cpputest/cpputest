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
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/OrderedTest.h"

OrderedTest* OrderedTest::_orderedTestsHead = 0;

OrderedTest::OrderedTest() :
	_nextOrderedTest(0)
{
}

OrderedTest::~OrderedTest()
{
}

int OrderedTest::getLevel()
{
	return _level;
}

void OrderedTest::setLevel(int level)
{
	_level = level;
}

void OrderedTest::setOrderedTestHead(OrderedTest* test)
{
	_orderedTestsHead = test;
}

OrderedTest* OrderedTest::getOrderedTestHead()
{
	return _orderedTestsHead;
}

bool OrderedTest::firstOrderedTest()
{
	return (getOrderedTestHead() == 0);
}

OrderedTest* OrderedTest::addOrderedTest(OrderedTest* test)
{
	Utest::addTest(test);
	_nextOrderedTest = test;
	return this;
}

void OrderedTest::addOrderedTestToHead(OrderedTest* test)
{
	TestRegistry *reg = TestRegistry::getCurrentRegistry();

	if (reg->getFirstTest()->isNull() || getOrderedTestHead()
			== reg->getFirstTest()) reg->addTest(test);
	else reg->getTestWithNext(getOrderedTestHead())->addTest(test);

	test->_nextOrderedTest = getOrderedTestHead();
	setOrderedTestHead(test);
}

OrderedTest* OrderedTest::getNextOrderedTest()
{
	return _nextOrderedTest;
}

OrderedTestInstaller::OrderedTestInstaller(OrderedTest* test,
		const char* groupName, const char* testName, const char* fileName,
		int lineNumber, int level)
{
	test->setTestName(testName);
	test->setGroupName(groupName);
	test->setFileName(fileName);
	test->setLineNumber(lineNumber);
	test->setLevel(level);

	if (OrderedTest::firstOrderedTest()) OrderedTest::addOrderedTestToHead(test);
	else addOrderedTestInOrder(test);
}

void OrderedTestInstaller::addOrderedTestInOrder(OrderedTest* test)
{
	if (test->getLevel() < OrderedTest::getOrderedTestHead()->getLevel()) OrderedTest::addOrderedTestToHead(
			test);
	else addOrderedTestInOrderNotAtHeadPosition(test);
}

void OrderedTestInstaller::addOrderedTestInOrderNotAtHeadPosition(
		OrderedTest* test)
{
	OrderedTest* current = OrderedTest::getOrderedTestHead();
	while (current->getNextOrderedTest()) {

		if (current->getNextOrderedTest()->getLevel() > test->getLevel()) {
			test->addOrderedTest(current->getNextOrderedTest());
			current->addOrderedTest(test);
			return;
		}
		current = current->getNextOrderedTest();
	}
	test->addOrderedTest(current->getNextOrderedTest());
	current->addOrderedTest(test);
}

OrderedTestInstaller::~OrderedTestInstaller()
{
}
