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
#include "CppUTest/PlatformSpecificFunctions.h"

TestResult* Utest::testResult_ = 0;
Utest* Utest::currentTest_ = 0;

Utest::Utest()
    : group_("UndefinedTestGroup")
    , name_("UndefinedTest")
    , file_("UndefinedFile")
    , lineNumber_(0)
    , next_(&NullTest::instance())
{
}

Utest::Utest (const char* groupName,
            const char* testName,
            const char* fileName,
            int lineNumber)
    : group_(groupName)
    , name_(testName)
    , file_(fileName)
    , lineNumber_(lineNumber)
    , next_(&NullTest::instance())
{}

Utest::Utest (const char* groupName,
            const char* testName,
            const char* fileName,
            int lineNumber,
            Utest* nextTest)
    : group_(groupName)
    , name_(testName)
    , file_(fileName)
    , lineNumber_(lineNumber)
    , next_(nextTest)
{}

Utest::~Utest ()
{}

void Utest::runOneTestWithPlugins(TestPlugin* plugin, TestResult& result)
{
   executePlatformSpecificRunOneTest(plugin, result);
}

void Utest::runOneTest(TestPlugin* plugin, TestResult& result)
{
   plugin->runAllPreTestAction(*this, result) ;
   run(result);
   plugin->runAllPostTestAction(*this, result);
}


void Utest::run(TestResult& result)
{
  //save test context, so that test class can be tested
  Utest* savedTest = currentTest_;
  TestResult* savedResult = testResult_;

  result.countRun();
  testResult_ = &result;
  currentTest_ = this;

  if (executePlatformSpecificSetup()) {
     executePlatformSpecificTestBody();
  }
  executePlatformSpecificTeardown();

  //restore
  currentTest_ = savedTest;
  testResult_ = savedResult;
}

void Utest::exitCurrentTest()
{
   executePlatformSpecificExitCurrentTest();
}

Utest *Utest::getNext() const
  {
    return next_;
  }


Utest* Utest::addTest(Utest *test)
{
  next_ = test;
  return this;
}

int Utest::countTests()
{
	return next_->countTests() + 1;
}

bool Utest::isNull () const
{
	return false;
}

SimpleString Utest::getMacroName() const
{
  return "TEST";
}


const SimpleString Utest::getName() const
  {
    return SimpleString(name_);
  }

const SimpleString Utest::getGroup() const
{
	return SimpleString(group_);
}

SimpleString Utest::getFormattedName() const
  {
    SimpleString formattedName(getMacroName());
    formattedName += "(";
    formattedName += group_;
    formattedName += ", ";
    formattedName += name_;
    formattedName += ")";

    return formattedName;
  }

const char* Utest::getProgressIndicator() const
{
    return ".";
}

void Utest::setFileName(const char* fileName)
{
	file_ = fileName;
}

void Utest::setLineNumber(int lineNumber)
{
	lineNumber_ = lineNumber;
}

void Utest::setGroupName(const char* groupName)
{
	group_ = groupName;
}

void Utest::setTestName(const char* testName)
{
	name_ = testName;
}

const SimpleString Utest::getFile() const
  {
    return SimpleString(file_);
  }


int Utest::getLineNumber() const
  {
    return lineNumber_;
  }

void Utest::setup()
{
}

void Utest::teardown()
{
}

bool Utest::shouldRun(const SimpleString& groupFilter, const SimpleString& nameFilter) const
  {
    SimpleString group(group_);
    SimpleString name(name_);
    if (group.contains(groupFilter) && name.contains(nameFilter))
      return true;

    return false;
  }

bool Utest::assertTrue(bool condition, const char* conditionString, const char* fileName, int lineNumber)
{
  testResult_->countCheck();
  if (!(condition))
    {
      SimpleString message("CHECK(");
      message += conditionString;
      message += ") failed";
      Failure _f(this, fileName, lineNumber, message);
      testResult_->addFailure (_f);
      return false;
    }
  return true;
}

bool Utest::assertCstrEqual(const char* expected, const char* actual, const char* fileName, int lineNumber)
{
	testResult_->countCheck();
	if (actual == 0 && expected == 0) return true;
	if (actual == 0) {
		EqualsFailure _f(this, fileName, lineNumber, StringFrom(expected), StringFrom("(null)"));
		testResult_->addFailure (_f);
		return false;
	}
	if (expected == 0) {
		EqualsFailure _f(this, fileName, lineNumber, StringFrom("(null)"), StringFrom(actual));
		testResult_->addFailure (_f);
		return false;
	}
	if (PlatformSpecificStrCmp(expected, actual) != 0) {
		EqualsFailure _f(this, fileName, lineNumber, StringFrom(expected), StringFrom(actual));
		testResult_->addFailure (_f);
		return false;
	}
	return true;
}

bool Utest::assertCstrContains(const char* expected, const char* actual, const char* fileName, int lineNumber)
{
   testResult_->countCheck();
   if (actual == 0 && expected == 0) return true;
   if (actual == 0) {
      ContainsFailure _f(this, fileName, lineNumber, StringFrom(expected), StringFrom("(null)"));
      testResult_->addFailure (_f);
      return false;
   }
   if (expected == 0) {
      ContainsFailure _f(this, fileName, lineNumber, StringFrom("(null)"), StringFrom(actual));
      testResult_->addFailure (_f);
      return false;
   }
   if (!SimpleString(actual).contains(expected)) {
      ContainsFailure _f(this, fileName, lineNumber, StringFrom(expected), StringFrom(actual));
      testResult_->addFailure (_f);
      return false;
   }
   return true;
}

void PadStringsToSameLength(SimpleString& aDecimal, SimpleString& eDecimal, char padCharacter)
{
    char pad[2];
    pad[0] = padCharacter;
    pad[1] = 0;
    if (aDecimal.size() > eDecimal.size())
        eDecimal = SimpleString(pad, aDecimal.size() - eDecimal.size()) + eDecimal;
    else
        aDecimal = SimpleString(pad, eDecimal.size() - aDecimal.size()) + aDecimal;
}

bool Utest::assertLongsEqual(long expected, long actual, const char* fileName, int lineNumber)
{
  testResult_->countCheck();
  if (expected != actual)
    {
      SimpleString aDecimal = StringFrom(actual);
      SimpleString aHex = HexStringFrom(actual);
      SimpleString eDecimal = StringFrom(expected);
      SimpleString eHex = HexStringFrom(expected);

      PadStringsToSameLength(aDecimal, eDecimal, ' ');
      PadStringsToSameLength(aHex, eHex, '0');

      SimpleString actualReported = aDecimal + " 0x" + aHex;
      SimpleString expectedReported = eDecimal + " 0x" + eHex;

      EqualsFailure _f(this, fileName, lineNumber, expectedReported, actualReported);
      testResult_->addFailure (_f);
      return false;
    }
  return true;
}

bool Utest::assertPointersEqual(void* expected, void* actual, const char* fileName, int lineNumber)
{
  testResult_->countCheck();
  if (expected != actual)
    {
      EqualsFailure _f(this, fileName, lineNumber, StringFrom(expected), StringFrom(actual));
      testResult_->addFailure (_f);
      return false;
    }
  return true;
}

bool Utest::assertDoublesEqual(double expected, double actual, double threshold, const char* fileName, int lineNumber)
{
  testResult_->countCheck();
  if (PlatformSpecificFabs(expected-actual) > threshold)
    {
      EqualsFailure _f(this, fileName, lineNumber, StringFrom(expected), StringFrom(actual));
      testResult_->addFailure (_f);
      return false;
    }
  return true;
}

void Utest::fail(const char *text, const char* fileName, int lineNumber)
{
  Failure _f(this, fileName, lineNumber, text);
  testResult_->addFailure (_f);
}

void Utest::print(const char *text, const char* fileName, int lineNumber)
{
   SimpleString stringToPrint = "\n";
   stringToPrint += fileName;
   stringToPrint += ":";
   stringToPrint += StringFrom(lineNumber);
   stringToPrint += " ";
   stringToPrint += text;
   testResult_->print(stringToPrint.asCharString());
}

void Utest::print(const SimpleString& text, const char* fileName, int lineNumber)
{
   print(text.asCharString(), fileName, lineNumber);
}

TestResult* Utest::getTestResult()
{
  return testResult_;
}

Utest* Utest::getCurrent()
{
  return currentTest_;
}

////////////// NullTest ////////////


NullTest::NullTest()
    :Utest("NullGroup", "NullName", "NullFile", -1, 0)
{}

NullTest::~NullTest()
{}

NullTest& NullTest::instance()
{
	static NullTest _instance;
	return _instance;
}

int NullTest::countTests()
{
	return 0;
}

Utest* NullTest::getNext() const
{
	return &instance();
}

bool NullTest::isNull () const
{
	return true;
}

////////////// TestInstaller ////////////

TestInstaller::TestInstaller(Utest* t, const char* groupName, const char* testName, const char* fileName, int lineNumber)
{
	t->setGroupName(groupName);
	t->setTestName(testName);
	t->setFileName(fileName);
	t->setLineNumber(lineNumber);
	TestRegistry::getCurrentRegistry()->addTest(t);
}

TestInstaller::~TestInstaller()
{}

void TestInstaller::unDo()
{
  TestRegistry::getCurrentRegistry()->unDoLastAddTest();
}
