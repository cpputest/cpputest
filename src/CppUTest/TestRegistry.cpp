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

TestRegistry::TestRegistry()
    : tests(&NullTest::instance())
    , nameFilter_("")
    , groupFilter_("")
    , firstPlugin_ (NullTestPlugin::instance())
{
}

TestRegistry::~TestRegistry()
{
}

void TestRegistry::addTest (Utest *test)
{
  tests = test->addTest(tests);
}

void TestRegistry::runAllTests (TestResult& result)
{
	bool groupStart = true;

	result.testsStarted ();
	for (Utest *test = tests; !test->isLast(); test = test->getNext ()){

		if (groupStart) {
			result.currentGroupStarted(test);
			groupStart = false;
		}

		result.countTest();
		if (testShouldRun(test, result)) {
			result.currentTestStarted(test);
			platformSpecificRunOneTest(test, result);
			result.currentTestEnded(test);
		}
		
		if (endOfGroup (test)) {
			groupStart = true;
			result.currentGroupEnded(test);
		}
	}
  result.testsEnded ();
}

void TestRegistry::runOneTest(Utest* test, TestResult& result)
{
        firstPlugin_->runAllPreTestAction(*test, result) ;
        test->run(result);
        firstPlugin_->runAllPostTestAction(*test, result);
}


bool TestRegistry::endOfGroup(Utest* test)
{
	if (test->isLast() || test->getGroup() != test->getNext()->getGroup())
		return true;
	return false;
}


int  TestRegistry::countTests()
{
	return tests->countTests();
}

TestRegistry* TestRegistry::currentRegistry_ = 0;
	
TestRegistry* TestRegistry::getCurrentRegistry()
{
	static TestRegistry registry;
	return (currentRegistry_ == 0) ? &registry : currentRegistry_;
}

void TestRegistry::setCurrentRegistry(TestRegistry* registry)
{
	currentRegistry_ = registry;
}

void TestRegistry::unDoLastAddTest()
{
  tests = tests->getNext();

}

void TestRegistry::nameFilter(SimpleString f)
{
	nameFilter_ = f;
}

void TestRegistry::groupFilter(SimpleString f)
{
  groupFilter_ = f;
}

SimpleString TestRegistry::getGroupFilter()
{
	return groupFilter_;
}

SimpleString TestRegistry::getNameFilter()
{
	return nameFilter_;
}

bool TestRegistry::testShouldRun(Utest* test, TestResult& result)
{

  if (test->shouldRun(groupFilter_, nameFilter_) )
    return true;
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
