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
#include "CppUTest/TestOutput.h"
#include "CppUTest/PlatformSpecificFunctions.h"

namespace
{
const int testLineNumber = 1;
}

class MockTest: public UtestShell
{
public:
    MockTest(const char* group = "Group") :
        UtestShell(group, "Name", "File", testLineNumber), hasRun_(false)
    {
    }
    virtual void runOneTest(TestPlugin*, TestResult&) _override
    {
        hasRun_ = true;
    }

    bool hasRun_;
};

class MockTestResult: public TestResult
{
public:

    int countTestsStarted;
    int countTestsEnded;
    int countCurrentTestStarted;
    int countCurrentTestEnded;
    int countCurrentGroupStarted;
    int countCurrentGroupEnded;

    MockTestResult(TestOutput& p) :
        TestResult(p)
    {
        resetCount();
    }

    virtual ~MockTestResult() _destructor_override
    {
    }

    void resetCount()
    {
        countTestsStarted = 0;
        countTestsEnded = 0;
        countCurrentTestStarted = 0;
        countCurrentTestEnded = 0;
        countCurrentGroupStarted = 0;
        countCurrentGroupEnded = 0;
    }

    virtual void testsStarted() _override
    {
        countTestsStarted++;
    }
    virtual void testsEnded() _override
    {
        countTestsEnded++;
    }
    virtual void currentTestStarted(UtestShell* /*test*/) _override
    {
        countCurrentTestStarted++;
    }
    virtual void currentTestEnded(UtestShell* /*test*/) _override
    {
        countCurrentTestEnded++;
    }
    virtual void currentGroupStarted(UtestShell* /*test*/) _override
    {
        countCurrentGroupStarted++;
    }
    virtual void currentGroupEnded(UtestShell* /*test*/) _override
    {
        countCurrentGroupEnded++;
    }

};

TEST_GROUP(TestRegistry)
{
    TestRegistry* myRegistry;
    StringBufferTestOutput* output;
    MockTest* test1;
    MockTest* test2;
    MockTest* test3;
    MockTest* test4;
    TestResult *result;
    MockTestResult *mockResult;
    void setup() _override
    {
        output = new StringBufferTestOutput();
        mockResult = new MockTestResult(*output);
        result = mockResult;
        test1 = new MockTest();
        test2 = new MockTest();
        test3 = new MockTest("group2");
        test4 = new MockTest();
        myRegistry = new TestRegistry();
        myRegistry->setCurrentRegistry(myRegistry);
    }

    void teardown() _override
    {
        myRegistry->setCurrentRegistry(NULLPTR);
        delete myRegistry;
        delete test1;
        delete test2;
        delete test3;
        delete test4;
        delete result;
        delete output;
    }

    void addAndRunAllTests()
    {
        myRegistry->addTest(test1);
        myRegistry->addTest(test2);
        myRegistry->addTest(test3);
        myRegistry->runAllTests(*result);
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

TEST(TestRegistry, runTwoTestsCheckResultFunctionsCalled)
{
    myRegistry->addTest(test1);
    myRegistry->addTest(test2);
    myRegistry->runAllTests(*result);
    LONGS_EQUAL(1, mockResult->countTestsStarted);
    LONGS_EQUAL(1, mockResult->countTestsEnded);
    LONGS_EQUAL(1, mockResult->countCurrentGroupStarted);
    LONGS_EQUAL(1, mockResult->countCurrentGroupEnded);
    LONGS_EQUAL(2, mockResult->countCurrentTestStarted);
    LONGS_EQUAL(2, mockResult->countCurrentTestEnded);
}

TEST(TestRegistry, runThreeTestsandTwoGroupsCheckResultFunctionsCalled)
{
    addAndRunAllTests();
    LONGS_EQUAL(2, mockResult->countCurrentGroupStarted);
    LONGS_EQUAL(2, mockResult->countCurrentGroupEnded);
    LONGS_EQUAL(3, mockResult->countCurrentTestStarted);
    LONGS_EQUAL(3, mockResult->countCurrentTestEnded);
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

TEST(TestRegistry, findTestWithNameDoesntExist)
{
    CHECK(myRegistry->findTestWithName("ThisTestDoesntExists") == NULLPTR);
}

TEST(TestRegistry, findTestWithName)
{
    test1->setTestName("NameOfATestThatDoesExist");
    test2->setTestName("SomeOtherTest");
    myRegistry->addTest(test1);
    myRegistry->addTest(test2);
    CHECK(myRegistry->findTestWithName("NameOfATestThatDoesExist") != NULLPTR);
}

TEST(TestRegistry, findTestWithGroupDoesntExist)
{
    CHECK(myRegistry->findTestWithGroup("ThisTestGroupDoesntExists") == NULLPTR);
}

TEST(TestRegistry, findTestWithGroup)
{
    test1->setGroupName("GroupOfATestThatDoesExist");
    test2->setGroupName("SomeOtherGroup");
    myRegistry->addTest(test1);
    myRegistry->addTest(test2);
    CHECK(myRegistry->findTestWithGroup("GroupOfATestThatDoesExist") != NULLPTR);
}

TEST(TestRegistry, nameFilterWorks)
{
    test1->setTestName("testname");
    test2->setTestName("noname");
    TestFilter nameFilter("testname");
    myRegistry->setNameFilters(&nameFilter);
    addAndRunAllTests();
    CHECK(test1->hasRun_);
    CHECK(!test2->hasRun_);
}

TEST(TestRegistry, groupFilterWorks)
{
    test1->setGroupName("groupname");
    test2->setGroupName("noname");
    TestFilter groupFilter("groupname");
    myRegistry->setGroupFilters(&groupFilter);
    addAndRunAllTests();
    CHECK(test1->hasRun_);
    CHECK(!test2->hasRun_);
}

TEST(TestRegistry, runTestInSeperateProcess)
{
    myRegistry->setRunTestsInSeperateProcess();
    myRegistry->addTest(test1);
    myRegistry->runAllTests(*result);
    CHECK(test1->isRunInSeperateProcess());
}

TEST(TestRegistry, CurrentRepetitionIsCorrectNone)
{
    CHECK(0 == myRegistry->getCurrentRepetition());
    myRegistry->runAllTests(*result);
    LONGS_EQUAL(1, myRegistry->getCurrentRepetition());
}

TEST(TestRegistry, CurrentRepetitionIsCorrectTwo)
{
    CHECK(0 == myRegistry->getCurrentRepetition());
    myRegistry->runAllTests(*result);
    myRegistry->runAllTests(*result);
    LONGS_EQUAL(2, myRegistry->getCurrentRepetition());
}

class MyTestPluginDummy: public TestPlugin
{
public:
    MyTestPluginDummy(const SimpleString& name) : TestPlugin(name) {}
    virtual ~MyTestPluginDummy() _destructor_override {}
    virtual void runAllPreTestAction(UtestShell&, TestResult&) _override {}
    virtual void runAllPostTestAction(UtestShell&, TestResult&) _override {}
};

TEST(TestRegistry, ResetPluginsWorks)
{
    MyTestPluginDummy plugin1("Plugin-1");
    MyTestPluginDummy plugin2("Plugin-2");
    myRegistry->installPlugin(&plugin1);
    myRegistry->installPlugin(&plugin2);
    LONGS_EQUAL(2, myRegistry->countPlugins());
    myRegistry->resetPlugins();
    LONGS_EQUAL(0, myRegistry->countPlugins());
}

TEST(TestRegistry, listTestGroupNames_shouldListBackwardsGroup1AfterGroup11AndGroup2OnlyOnce)
{
    test1->setGroupName("GROUP_1");
    myRegistry->addTest(test1);
    test2->setGroupName("GROUP_2");
    myRegistry->addTest(test2);
    test3->setGroupName("GROUP_11");
    myRegistry->addTest(test3);
    test4->setGroupName("GROUP_2");
    myRegistry->addTest(test4);

    myRegistry->listTestGroupNames(*result);
    SimpleString s = output->getOutput();
    STRCMP_EQUAL("GROUP_2 GROUP_11 GROUP_1", s.asCharString());
}

TEST(TestRegistry, listTestGroupAndCaseNames_shouldListBackwardsGroupATestaAfterGroupAtestaa)
{
    test1->setGroupName("GROUP_A");
    test1->setTestName("test_a");
    myRegistry->addTest(test1);
    test2->setGroupName("GROUP_B");
    test2->setTestName("test_b");
    myRegistry->addTest(test2);
    test3->setGroupName("GROUP_A");
    test3->setTestName("test_aa");
    myRegistry->addTest(test3);

    myRegistry->listTestGroupAndCaseNames(*result);
    SimpleString s = output->getOutput();
    STRCMP_EQUAL("GROUP_A.test_aa GROUP_B.test_b GROUP_A.test_a", s.asCharString());
}

TEST(TestRegistry, listTestLocations_shouldListBackwardsGroupATestaAfterGroupAtestaa)
{
    test1->setGroupName("GROUP_A");
    test1->setTestName("test_a");
    test1->setFileName("cpptest_simple/my_tests/testa.cpp");
    test1->setLineNumber(100);
    myRegistry->addTest(test1);
    test2->setGroupName("GROUP_B");
    test2->setTestName("test_b");
    test2->setFileName("cpptest_simple/my tests/testb.cpp");
    test2->setLineNumber(200);
    myRegistry->addTest(test2);
    test3->setGroupName("GROUP_A");
    test3->setTestName("test_aa");
    test3->setFileName("cpptest_simple/my_tests/testaa.cpp");
    test3->setLineNumber(300);
    myRegistry->addTest(test3);

    myRegistry->listTestLocations(*result);
    SimpleString s = output->getOutput();
    STRCMP_EQUAL("GROUP_A.test_aa.cpptest_simple/my_tests/testaa.cpp.300\nGROUP_B.test_b.cpptest_simple/my tests/testb.cpp.200\nGROUP_A.test_a.cpptest_simple/my_tests/testa.cpp.100\n", s.asCharString());
}

TEST(TestRegistry, shuffleEmptyListIsNoOp)
{
    CHECK_TRUE(myRegistry->getFirstTest() == NULLPTR);
    myRegistry->shuffleTests(0);
    CHECK_TRUE(myRegistry->getFirstTest() == NULLPTR);
}

TEST(TestRegistry, shuffleSingleTestIsNoOp)
{
    myRegistry->addTest(test1);
    myRegistry->shuffleTests(0);
    CHECK_TRUE(myRegistry->getFirstTest() == test1);
}

static int getZero()
{
    return 0;
}

IGNORE_TEST(TestRegistry, shuffleTestList)
{
    UT_PTR_SET(PlatformSpecificRand, getZero);
    myRegistry->addTest(test3);
    myRegistry->addTest(test2);
    myRegistry->addTest(test1);

    UtestShell* first_before  = myRegistry->getFirstTest();
    UtestShell* second_before = first_before->getNext();
    UtestShell* third_before  = second_before->getNext();

    CHECK_TRUE(first_before  == test1);
    CHECK_TRUE(second_before == test2);
    CHECK_TRUE(third_before  == test3);
    CHECK_TRUE(third_before->getNext()  == NULLPTR);

    // shuffle always with element at index 0: [1] 2 [3] --> [3] [2] 1 --> 2 3 1
    myRegistry->shuffleTests(0);

    UtestShell* first_after  = myRegistry->getFirstTest();
    UtestShell* second_after = first_after->getNext();
    UtestShell* third_after  = second_after->getNext();

    CHECK_TRUE(first_after  == test2);
    CHECK_TRUE(second_after == test3);
    CHECK_TRUE(third_after  == test1);
    CHECK_TRUE(third_after->getNext() == NULLPTR);
}

TEST(TestRegistry, reverseTests)
{
    myRegistry->addTest(test1);
    myRegistry->addTest(test2);

    myRegistry->reverseTests();

    CHECK(test1 == myRegistry->getFirstTest());
}

TEST(TestRegistry, reverseZeroTests)
{
    myRegistry->reverseTests();

    CHECK(NULLPTR == myRegistry->getFirstTest());
}
