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
#include "CppUTest/TestOutput.h"
#include "CppUTest/CompositeTestOutput.h"
#include "CppUTest/TestResult.h"

class CompositeTestOutputTestStringBufferTestOutput : public StringBufferTestOutput
{
  public:
    virtual void printTestsStarted()
    {
      output += "Test Start\n";
    }

    virtual void printTestsEnded(const TestResult& result)
    {
      output += StringFromFormat("Test End %d\n", result.getTestCount());
    }

    void printCurrentGroupStarted(const UtestShell& test)
    {
      output += StringFromFormat("Group %s Start\n", test.getGroup().asCharString());
    }

    void printCurrentGroupEnded(const TestResult& res)
    {
      output += StringFromFormat("Group End %d\n", res.getTestCount());
    }

    virtual void printCurrentTestStarted(const UtestShell&)
    {
      output += "s";
    }

    void flush()
    {
      output += "flush";
    }

    virtual bool isVerbose()
    {
      return verbose_;
    }

    virtual bool isColor()
    {
      return color_;
    }

    virtual const char* getProgressIndicator()
    {
      return progressIndication_;
    }
};

TEST_GROUP(CompositeTestOutput)
{
  CompositeTestOutputTestStringBufferTestOutput output1;
  CompositeTestOutputTestStringBufferTestOutput output2;
  CompositeTestOutput compositeOutput;
  TestResult* result;
  UtestShell* test;

  void setup()
  {
    compositeOutput.setOutputOne(&output1);
    compositeOutput.setOutputTwo(&output2);
    result = new TestResult(compositeOutput);
    test = new UtestShell("Group", "Name", "file", 10);
  }

  void teardown()
  {
    delete test;
    delete result;
  }
};

TEST(CompositeTestOutput, TestStartedAndEnded)
{
  compositeOutput.printTestsStarted();
  compositeOutput.printTestsEnded(*result);
  STRCMP_EQUAL("Test Start\nTest End 0\n", output1.getOutput().asCharString());
  STRCMP_EQUAL("Test Start\nTest End 0\n", output2.getOutput().asCharString());
}

TEST(CompositeTestOutput, CurrentTestStartedAndEnded)
{
  compositeOutput.printCurrentTestStarted(*test);
  compositeOutput.printCurrentTestEnded(*result);
  STRCMP_EQUAL("s.", output1.getOutput().asCharString());
  STRCMP_EQUAL("s.", output2.getOutput().asCharString());
}

TEST(CompositeTestOutput, CurrentGroupStartedAndEnded)
{
  compositeOutput.printCurrentGroupStarted(*test);
  compositeOutput.printCurrentGroupEnded(*result);
  STRCMP_EQUAL("Group Group Start\nGroup End 0\n", output1.getOutput().asCharString());
  STRCMP_EQUAL("Group Group Start\nGroup End 0\n", output2.getOutput().asCharString());
}

TEST(CompositeTestOutput, PrintBuffer)
{
  compositeOutput.printBuffer("Boo");
  STRCMP_EQUAL("Boo", output1.getOutput().asCharString());
  STRCMP_EQUAL("Boo", output2.getOutput().asCharString());
}

TEST(CompositeTestOutput, printChar)
{
  compositeOutput.print("Boo");
  STRCMP_EQUAL("Boo", output1.getOutput().asCharString());
  STRCMP_EQUAL("Boo", output2.getOutput().asCharString());
}

TEST(CompositeTestOutput, printLong)
{
  compositeOutput.print(10);
  STRCMP_EQUAL("10", output1.getOutput().asCharString());
  STRCMP_EQUAL("10", output2.getOutput().asCharString());
}

TEST(CompositeTestOutput, printDouble)
{
  compositeOutput.printDouble(1.01);
  STRCMP_EQUAL("1.01", output1.getOutput().asCharString());
  STRCMP_EQUAL("1.01", output2.getOutput().asCharString());
}

TEST(CompositeTestOutput, verbose)
{
  compositeOutput.verbose();
  CHECK(output1.isVerbose());
  CHECK(output2.isVerbose());
}

TEST(CompositeTestOutput, color)
{
  compositeOutput.color();
  CHECK(output1.isColor());
  CHECK(output2.isColor());
}

TEST(CompositeTestOutput, PrintTestFailure)
{
  TestFailure failure(test, "file", 10, "failed");
  compositeOutput.print(failure);
  STRCMP_EQUAL("\nfile:10: error: Failure in TEST(Group, Name)\n\tfailed\n\n", output1.getOutput().asCharString());
  STRCMP_EQUAL("\nfile:10: error: Failure in TEST(Group, Name)\n\tfailed\n\n", output2.getOutput().asCharString());
}

TEST(CompositeTestOutput, PrintTestRun)
{
  compositeOutput.printTestRun(1, 2);
  STRCMP_EQUAL("Test run 1 of 2\n", output1.getOutput().asCharString());
  STRCMP_EQUAL("Test run 1 of 2\n", output2.getOutput().asCharString());
}

TEST(CompositeTestOutput, setProgressIndicator)
{
  compositeOutput.setProgressIndicator("?");
  STRCMP_EQUAL("?", output1.getProgressIndicator());
  STRCMP_EQUAL("?", output2.getProgressIndicator());
}

TEST(CompositeTestOutput, flush)
{
  compositeOutput.flush();
  STRCMP_EQUAL("flush", output1.getOutput().asCharString());
  STRCMP_EQUAL("flush", output2.getOutput().asCharString());
}

