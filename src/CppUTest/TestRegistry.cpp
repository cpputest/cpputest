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
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
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
#include "CppUTest/PlatformSpecificFunctions.h"

TestRegistry::TestRegistry() :
    tests_(NULLPTR), nameFilters_(NULLPTR), groupFilters_(NULLPTR), firstPlugin_(NullTestPlugin::instance()), runInSeperateProcess_(false), currentRepetition_(0), runIgnored_(false)
{
}

TestRegistry::~TestRegistry()
{
}

void TestRegistry::addTest(UtestShell *test)
{
    tests_ = test->addTest(tests_);
}

void TestRegistry::runAllTests(TestResult& result)
{
    bool groupStart = true;

    result.testsStarted();
    for (UtestShell *test = tests_; test != NULLPTR; test = test->getNext()) {
        if (runInSeperateProcess_) test->setRunInSeperateProcess();
        if (runIgnored_) test->setRunIgnored();

        if (groupStart) {
            result.currentGroupStarted(test);
            groupStart = false;
        }

        result.countTest();
        if (testShouldRun(test, result)) {
            result.currentTestStarted(test);
            test->runOneTest(firstPlugin_, result);
            result.currentTestEnded(test);
        }

        if (endOfGroup(test)) {
            groupStart = true;
            result.currentGroupEnded(test);
        }
    }
    result.testsEnded();
    currentRepetition_++;
}

void TestRegistry::listTestGroupNames(TestResult& result)
{
    SimpleString groupList;

    for (UtestShell *test = tests_; test != NULLPTR; test = test->getNext()) {
        SimpleString gname;
        gname += "#";
        gname += test->getGroup();
        gname += "#";

        if (!groupList.contains(gname)) {
            groupList += gname;
            groupList += " ";
        }
    }

    groupList.replace("#", "");

    if (groupList.endsWith(" "))
        groupList = groupList.subString(0, groupList.size() - 1);
    result.print(groupList.asCharString());
}

void TestRegistry::listTestGroupAndCaseNames(TestResult& result)
{
    SimpleString groupAndNameList;

    for (UtestShell *test = tests_; test != NULLPTR; test = test->getNext()) {
        if (testShouldRun(test, result)) {
            SimpleString groupAndName;
            groupAndName += "#";
            groupAndName += test->getGroup();
            groupAndName += ".";
            groupAndName += test->getName();
            groupAndName += "#";

            if (!groupAndNameList.contains(groupAndName)) {
                groupAndNameList += groupAndName;
                groupAndNameList += " ";
            }
        }
    }

    groupAndNameList.replace("#", "");

    if (groupAndNameList.endsWith(" "))
        groupAndNameList = groupAndNameList.subString(0, groupAndNameList.size() - 1);
    result.print(groupAndNameList.asCharString());
}

void TestRegistry::listTestLocations(TestResult& result)
{
    SimpleString testLocations;

    for (UtestShell *test = tests_; test != NULLPTR; test = test->getNext()) {
            SimpleString testLocation;
            testLocation += test->getGroup();
            testLocation += ".";
            testLocation += test->getName();
            testLocation += ".";
            testLocation += test->getFile();
            testLocation += ".";
            testLocation += StringFromFormat("%d\n",(int) test->getLineNumber());

            testLocations += testLocation;
    }

    result.print(testLocations.asCharString());
}

bool TestRegistry::endOfGroup(UtestShell* test)
{
    return (!test || !test->getNext() || test->getGroup() != test->getNext()->getGroup());
}

size_t TestRegistry::countTests()
{
    return tests_ ? tests_->countTests() : 0;
}

TestRegistry* TestRegistry::currentRegistry_ = NULLPTR;

TestRegistry* TestRegistry::getCurrentRegistry()
{
    static TestRegistry registry;
    return (currentRegistry_ == NULLPTR) ? &registry : currentRegistry_;
}

void TestRegistry::setCurrentRegistry(TestRegistry* registry)
{
    currentRegistry_ = registry;
}

void TestRegistry::unDoLastAddTest()
{
    tests_ = tests_ ? tests_->getNext() : NULLPTR;

}

void TestRegistry::setNameFilters(const TestFilter* filters)
{
    nameFilters_ = filters;
}

void TestRegistry::setGroupFilters(const TestFilter* filters)
{
    groupFilters_ = filters;
}

void TestRegistry::setRunIgnored()
{
    runIgnored_ = true;
}

void TestRegistry::setRunTestsInSeperateProcess()
{
    runInSeperateProcess_ = true;
}

int TestRegistry::getCurrentRepetition()
{
    return currentRepetition_;
}

bool TestRegistry::testShouldRun(UtestShell* test, TestResult& result)
{
    if (test->shouldRun(groupFilters_, nameFilters_)) return true;
    else {
        result.countFilteredOut();
        return false;
    }
}

void TestRegistry::resetPlugins()
{
    firstPlugin_ = NullTestPlugin::instance();
}

void TestRegistry::installPlugin(TestPlugin* plugin)
{
    firstPlugin_ = plugin->addPlugin(firstPlugin_);
}

TestPlugin* TestRegistry::getFirstPlugin()
{
    return firstPlugin_;
}

TestPlugin* TestRegistry::getPluginByName(const SimpleString& name)
{
    return firstPlugin_->getPluginByName(name);
}

void TestRegistry::removePluginByName(const SimpleString& name)
{
    if (firstPlugin_->removePluginByName(name) == firstPlugin_) firstPlugin_ = firstPlugin_->getNext();
    if (firstPlugin_->getName() == name) firstPlugin_ = firstPlugin_->getNext();
    firstPlugin_->removePluginByName(name);
}

int TestRegistry::countPlugins()
{
    int count = 0;
    for (TestPlugin* plugin = firstPlugin_; plugin != NullTestPlugin::instance(); plugin = plugin->getNext())
        count++;
    return count;
}


UtestShell* TestRegistry::getFirstTest()
{
    return tests_;
}

void TestRegistry::shuffleTests(size_t seed)
{
    UtestShellPointerArray array(getFirstTest());
    array.shuffle(seed);
    tests_ = array.getFirstTest();
}

void TestRegistry::reverseTests()
{
    UtestShellPointerArray array(getFirstTest());
    array.reverse();
    tests_ = array.getFirstTest();
}

UtestShell* TestRegistry::getTestWithNext(UtestShell* test)
{
    UtestShell* current = tests_;
    while (current && current->getNext() != test)
        current = current->getNext();
    return current;
}

UtestShell* TestRegistry::findTestWithName(const SimpleString& name)
{
    UtestShell* current = tests_;
    while (current) {
        if (current->getName() == name)
            return current;
        current = current->getNext();
    }
    return NULLPTR;
}

UtestShell* TestRegistry::findTestWithGroup(const SimpleString& group)
{
    UtestShell* current = tests_;
    while (current) {
        if (current->getGroup() == group)
            return current;
        current = current->getNext();
    }
    return NULLPTR;
}

