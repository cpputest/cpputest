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


#include "Utest.h"
#include "TestRegistry.h"
#include "TestResult.h"
#include "Failure.h"
#include "EqualsFailure.h"
#include "NullTest.h"

#include <string.h>
#include <stdio.h>

namespace
  {
  void doNothing()
  {}
}

TestResult* Utest::testResult_ = 0;
Utest* Utest::currentTest_ = 0;

Utest::Utest (const char* groupName,
            const char* testName,
            const char* fileName,
            int lineNumber,
            void (*setUp)(),
            void (*tearDown)())
    : group_(groupName)
    , name_(testName)
    , file_(fileName)
    , lineNumber_(lineNumber)
    , next_(&NullTest::instance())
    , setUp_(setUp)
    , tearDown_(tearDown)
{}

Utest::Utest (const char* groupName,
            const char* testName,
            const char* fileName,
            int lineNumber,
            void (*setUp)(),
            void (*tearDown)(),
            Utest* nextTest)
    : group_(groupName)
    , name_(testName)
    , file_(fileName)
    , lineNumber_(lineNumber)
    , next_(nextTest)
    , setUp_(setUp)
    , tearDown_(tearDown)
{}

Utest::~Utest ()
{}

IgnoredTest::IgnoredTest(
  const char* groupName,
  const char* testName,
  const char* fileName,
  int lineNumber)
    : Utest(groupName, testName, fileName, lineNumber, doNothing, doNothing)
{}

IgnoredTest::~IgnoredTest ()
{}

void Utest::run(TestResult& result)
{
  //save test context, so that test class can be tested
  Utest* savedTest = currentTest_;
  TestResult* savedResult = testResult_;

  result.countRun();
  testResult_ = &result;
  currentTest_ = this;
  setUp();
  testBody();
  tearDown();

  //restore
  currentTest_ = savedTest;
  testResult_ = savedResult;
}

void IgnoredTest::run(TestResult& result)
{
  result.countIgnored();
}

void IgnoredTest::testBody()
{}


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

bool Utest::isLast () const
{
	return false;
}

const SimpleString Utest::getName() const
  {
    return SimpleString(name_);
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

const SimpleString Utest::getFile() const
  {
    return SimpleString(file_);
  }


int Utest::getLineNumber() const
  {
    return lineNumber_;
  }

void Utest::setUp()
{
  (*setUp_)();
}

void Utest::tearDown()
{
  (*tearDown_)();
}

bool Utest::shouldRun(const SimpleString& groupFilter, const SimpleString& nameFilter) const
  {
    SimpleString group(group_);
    SimpleString name(name_);
    if (group.contains(groupFilter) && name.contains(nameFilter))
      return true;

    return false;
  }

bool Utest::assertTrue(bool condition, const char* conditionString, int lineNumber)
{
  testResult_->countCheck();
  if (!(condition))
    {
      SimpleString message("CHECK(");
      message += conditionString;
      message += ") failed";
      Failure _f(this, lineNumber, message);
      testResult_->addFailure (_f);
      return false;
    }
  return true;
}

bool Utest::assertCstrEqual(const char* expected, const char* actual, int lineNumber)
{
  testResult_->countCheck();
  if (strcmp(expected, actual) != 0)
    {
      EqualsFailure _f(this, lineNumber, StringFrom(expected), StringFrom(actual));
      testResult_->addFailure (_f);
      return false;
    }
  return true;
}

bool Utest::assertLongsEqual(long expected, long actual, int lineNumber)
{
  testResult_->countCheck();
  if (expected != actual)
    {
      EqualsFailure _f(this, lineNumber, StringFrom(expected), StringFrom(actual));
      testResult_->addFailure (_f);
      return false;
    }
  return true;
}

bool Utest::assertDoublesEqual(double expected, double actual, double threshold, int lineNumber)
{
  testResult_->countCheck();
  if (fabs(expected-actual) > threshold)
    {
      EqualsFailure _f(this, lineNumber, StringFrom(expected), StringFrom(actual));
      testResult_->addFailure (_f);
      return false;
    }
  return true;
}

void Utest::fail(const char *text, int lineNumber)
{
  Failure _f(this, lineNumber, text);
  testResult_->addFailure (_f);
}


