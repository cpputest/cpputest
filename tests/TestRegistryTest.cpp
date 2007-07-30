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

#include "Platform.h"
#include "TestHarness.h"
#include "MockTestOutput.h"

namespace
  {
  void stub()
  {}
  const int testLineNumber = 1;
}
class MockTest : public Utest
  {
  public:
    MockTest()
        :Utest("Group", "Name", "File", testLineNumber), hasRun_(false)
    {}
    void testBody()
    {
    	hasRun_ = true;
    }
    ;
    bool hasRun_;
	};
	
TEST_GROUP(TestRegistry)
{
	TestRegistry* myRegistry;
	MockTestOutput* output;
	MockTest* test1;
	MockTest* test2;
  TestResult *result;
  TEST_SETUP() 
  {
    output = new MockTestOutput();
    result = new TestResult(*output);
    test1 = new MockTest();
    test2 = new MockTest();
  	myRegistry = new TestRegistry();
  	myRegistry->setCurrentRegistry(myRegistry);
  }
  
  TEST_TEARDOWN()
  {
  	myRegistry->setCurrentRegistry(0);
  	delete myRegistry;
  	delete test1;
  	delete test2;
  	delete result;
    delete output;
 }
};

TEST(TestRegistry, registryMyRegistryAndReset)
{
	CHECK(myRegistry->getCurrentRegistry() == myRegistry);
}

TEST(TestRegistry, emptyRegistryIsEmpty)
{
	CHECK(myRegistry->countTests() == 0);
}

TEST(TestRegistry, addOneTestIsNotEmpty)
{
	myRegistry->addTest(test1);
	CHECK(myRegistry->countTests() == 1);
}

TEST(TestRegistry, addOneTwoTests)
{
	myRegistry->addTest(test1);
	myRegistry->addTest(test2);
	CHECK(myRegistry->countTests() == 2);
}

TEST(TestRegistry, runTwoTests)
{
	myRegistry->addTest(test1);
	myRegistry->addTest(test2);
	CHECK(!test1->hasRun_);
	CHECK(!test2->hasRun_);
	myRegistry->runAllTests(*result);
	CHECK(test1->hasRun_);
	CHECK(test2->hasRun_);
}

TEST(TestRegistry, unDoTest)
{
	myRegistry->addTest(test1);
	CHECK(myRegistry->countTests() == 1);
	myRegistry->unDoLastAddTest();
	CHECK(myRegistry->countTests() == 0);
}


TEST(TestRegistry, unDoButNoTest)
{
	CHECK(myRegistry->countTests() == 0);
	myRegistry->unDoLastAddTest();
	CHECK(myRegistry->countTests() == 0);
}

TEST(TestRegistry, reallyUndoLastTest)
{
	myRegistry->addTest(test1);
	myRegistry->addTest(test2);
	CHECK(myRegistry->countTests() == 2);
	myRegistry->unDoLastAddTest();
	CHECK(myRegistry->countTests() == 1);
	myRegistry->runAllTests(*result);
	CHECK(test1->hasRun_);
	CHECK(!test2->hasRun_);
}
