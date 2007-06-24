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

EXPORT_TEST_GROUP(Utest);

namespace
  {
  void stub()
  {}
  const int testLineNumber = 1;
}

void callAllMacrosThatPass()
{
  CHECK(0 == 0);
  LONGS_EQUAL(1,1);
  CHECK_EQUAL("THIS", "THIS");
  STRCMP_EQUAL("THIS", "THIS");
  DOUBLES_EQUAL(1.0, 1.0, .01);
}

void callMacrosThatFail()
{
 	FAIL("This fails");
}

class PassingTestImplementation : public Utest
  {
  public:
    PassingTestImplementation()
        :Utest("Group", "Name", "File", testLineNumber, stub, stub)
    {}
    void testBody()
    {}
    ;
  };

class FailingTestImplementation : public Utest
  {
  public:
    FailingTestImplementation()
        :Utest("Group", "Name", "File", testLineNumber, stub, stub)
    {}
    void testBody()
    {
      FAIL("This test fails");
    };
  };

class ChecksInSetupTest : public Utest
  {
  public:
    ChecksInSetupTest(void (*setUp)())
        :Utest("Group", "Name", "File", testLineNumber, setUp, stub)
    {}

    virtual void setUp()
    {
  		CHECK(this == Utest::getCurrent());
  		Utest::setUp();
    }
    virtual void tearDown()
    {
    	CHECK(this == Utest::getCurrent());
    	Utest::tearDown();
    }

    void testBody()
    {
    };
  };

namespace
  {
  Utest* passingTest;
  Utest* failingTest;
  MockTestOutput* output;

  void SetUp()
  {
    passingTest = new PassingTestImplementation();
    failingTest = new FailingTestImplementation();
    output = new MockTestOutput();
        
  }
  void TearDown()
  {
    delete passingTest;
    delete failingTest;
    delete output;
  };


  void assertPrintContains(MockTestOutput* output, SimpleString& contains)
  {
    if (output->getOutput().contains(contains))
      return;
    SimpleString message("\tActual <");
    message += output->getOutput().asCharString();
    message += ">\n";
    message += "\tdid not contain <";
    message += contains.asCharString();
    message += ">\n";

    FAIL(message.asCharString());

  }
};


TEST(Utest, FailurePrintsSomething)
{
  TestResult result(*output);
  failingTest->run(result);
  SimpleString contains("This test fails");
  assertPrintContains(output, contains);
}

TEST(Utest, SuccessPrintsNothing)
{
  TestResult result(*output);
  passingTest->run(result);
  SimpleString expected = "";
  CHECK_EQUAL(expected, output->getOutput());
}

TEST(Utest, OutOfMacroHelper)
{
  callAllMacrosThatPass();
}

TEST(Utest, AssertsActLikeStatements)
{
  if (output != 0)
    CHECK(true)
  else
    CHECK(false)

  if (output != 0)
    CHECK_EQUAL(true, true)
  else
    CHECK_EQUAL(false, false)

  if (output != 0)
    STRCMP_EQUAL("", "")
    else
      STRCMP_EQUAL("", " ")

  if (output != 0)
    LONGS_EQUAL(1, 1)
    else
      LONGS_EQUAL(1, 0)

  if (output != 0)
    DOUBLES_EQUAL(1, 1, 0.01)
    else
      DOUBLES_EQUAL(1, 0, 0.01)

  if (false)
    FAIL("")
    else
      ;

  if (true)
    ;
  else
    FAIL("")

}


IGNORE_TEST(Utest, IgnoreTestSupportsAllMacros)
{
  CHECK(true);
  CHECK_EQUAL(true, true);
  STRCMP_EQUAL("", "");
  LONGS_EQUAL(1, 1);
  DOUBLES_EQUAL(1, 1, 0.01);
  FAIL("");
}

TEST(Utest, MacrosUsedInSetup)
{
	TestRegistry myRegistry;
	MockTestOutput output;
  TestResult result(output);
 	ChecksInSetupTest callsMacrosInSetupTest(callMacrosThatFail);

  myRegistry.setCurrentRegistry(&myRegistry);
	myRegistry.addTest(&callsMacrosInSetupTest);
	myRegistry.runAllTests(result, &output);
 	myRegistry.setCurrentRegistry(0);
 	LONGS_EQUAL(1, result.getFailureCount());
 	
}
