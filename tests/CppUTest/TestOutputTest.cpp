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
#include "CppUTest/TestResult.h"
#include "CppUTest/PlatformSpecificFunctions.h"

static long millisTime;

extern "C" {

    static long MockGetPlatformSpecificTimeInMillis()
    {
        return millisTime;
    }

}

TEST_GROUP(TestOutput)
{
    TestOutput* printer;
    StringBufferTestOutput* mock;
    UtestShell* tst;
    TestFailure *f;
    TestFailure *f2;
    TestFailure *f3;
    TestResult* result;

    void setup() _override
    {
        mock = new StringBufferTestOutput();
        printer = mock;
        tst = new UtestShell("group", "test", "file", 10);
        f = new TestFailure(tst, "failfile", 20, "message");
        f2 = new TestFailure(tst, "file", 20, "message");
        f3 = new TestFailure(tst, "file", 2, "message");
        result = new TestResult(*mock);
        result->setTotalExecutionTime(10);
        millisTime = 0;
        UT_PTR_SET(GetPlatformSpecificTimeInMillis, MockGetPlatformSpecificTimeInMillis);
        TestOutput::setWorkingEnvironment(TestOutput::eclipse);

    }
    void teardown() _override
    {
        TestOutput::setWorkingEnvironment(TestOutput::detectEnvironment);
        delete printer;
        delete tst;
        delete f;
        delete f2;
        delete f3;
        delete result;
    }

    void runOneTest()
    {
        result->countTest();
        result->countRun();
    }
};

TEST(TestOutput, PrintConstCharStar)
{
    printer->print("hello");
    printer->print("hello\n");
    STRCMP_EQUAL("hellohello\n", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintLong)
{
    long number = 1234;
    printer->print(number);
    STRCMP_EQUAL("1234", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintSize)
{
    size_t ten = 10;
    printer->print(ten);
    STRCMP_EQUAL("10", mock->getOutput().asCharString());
}


TEST(TestOutput, PrintDouble)
{
    printer->printDouble(12.34);
    STRCMP_EQUAL("12.34", mock->getOutput().asCharString());
}

TEST(TestOutput, StreamOperators)
{
    *printer << "n=" << 1234;
    STRCMP_EQUAL("n=1234", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintTestEnded)
{
    printer->printCurrentTestEnded(*result);
    STRCMP_EQUAL(".", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintTestALot)
{
    for (int i = 0; i < 60; ++i) {
        printer->printCurrentTestEnded(*result);
    }
    STRCMP_EQUAL("..................................................\n..........", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintTestALotAndSimulateRepeatRun)
{
    for (int i = 0; i < 60; ++i) {
        runOneTest();
        printer->printCurrentTestEnded(*result);
    }

    printer->printTestsEnded(*result);

    for (int i = 0; i < 60; ++i) {
        runOneTest();
        printer->printCurrentTestEnded(*result);
    }
    STRCMP_EQUAL("..................................................\n.........." \
        "\nOK (60 tests, 60 ran, 0 checks, 0 ignored, 0 filtered out, 10 ms)\n\n" \
        "..................................................\n..........", mock->getOutput().asCharString());
}

TEST(TestOutput, SetProgressIndicator)
{
    printer->setProgressIndicator(".");
    printer->printCurrentTestEnded(*result);
    printer->setProgressIndicator("!");
    printer->printCurrentTestEnded(*result);
    printer->setProgressIndicator(".");
    printer->printCurrentTestEnded(*result);

    STRCMP_EQUAL(".!.", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintTestVerboseStarted)
{
    mock->verbose(TestOutput::level_verbose);
    printer->printCurrentTestStarted(*tst);
    STRCMP_EQUAL("TEST(group, test)", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintTestVerboseEnded)
{
    mock->verbose(TestOutput::level_verbose);
    result->currentTestStarted(tst);
    millisTime = 5;
    result->currentTestEnded(tst);
    STRCMP_EQUAL("TEST(group, test) - 5 ms\n", mock->getOutput().asCharString());
}

TEST(TestOutput, printColorWithSuccess)
{
    mock->color();
    runOneTest();
    printer->printTestsEnded(*result);
    STRCMP_EQUAL("\n\033[32;1mOK (1 tests, 1 ran, 0 checks, 0 ignored, 0 filtered out, 10 ms)\033[m\n\n",
        mock->getOutput().asCharString());
}

TEST(TestOutput, printColorWithFailures)
{
    mock->color();
    runOneTest();
    result->addFailure(*f);
    printer->flush();
    printer->printTestsEnded(*result);
    STRCMP_EQUAL("\n\033[31;1mErrors (1 failures, 1 tests, 1 ran, 0 checks, 0 ignored, 0 filtered out, 10 ms)"
                 "\033[m\n\n", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintTestRun)
{
    printer->printTestRun(2, 3);
    STRCMP_EQUAL("Test run 2 of 3\n", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintTestRunOnlyOne)
{
    printer->printTestRun(1, 1);
    STRCMP_EQUAL("", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintWithFailureInSameFile)
{
    printer->printFailure(*f2);
    STRCMP_EQUAL("\nfile:20: error: Failure in TEST(group, test)\n\tmessage\n\n", mock->getOutput().asCharString());
}

TEST(TestOutput, PrintFailureWithFailInDifferentFile)
{
    printer->printFailure(*f);
    const char* expected =
            "\nfile:10: error: Failure in TEST(group, test)"
            "\nfailfile:20: error:\n\tmessage\n\n";
    STRCMP_EQUAL(expected, mock->getOutput().asCharString());
}

TEST(TestOutput, PrintFailureWithFailInHelper)
{
    printer->printFailure(*f3);
    const char* expected =
            "\nfile:10: error: Failure in TEST(group, test)"
            "\nfile:2: error:\n\tmessage\n\n";
    STRCMP_EQUAL(expected, mock->getOutput().asCharString());
}

TEST(TestOutput, PrintInVisualStudioFormat)
{
    TestOutput::setWorkingEnvironment(TestOutput::visualStudio);
    printer->printFailure(*f3);
    const char* expected =
            "\nfile(10): error: Failure in TEST(group, test)"
            "\nfile(2): error:\n\tmessage\n\n";
    STRCMP_EQUAL(expected, mock->getOutput().asCharString());
}

TEST(TestOutput, PrintTestStarts)
{
    printer->printTestsStarted();
    STRCMP_EQUAL("", mock->getOutput().asCharString());
}

TEST(TestOutput, printTestsEnded)
{
    result->countTest();
    result->countCheck();
    result->countIgnored();
    result->countIgnored();
    result->countRun();
    result->countRun();
    result->countRun();
    printer->printTestsEnded(*result);
    STRCMP_EQUAL("\nOK (1 tests, 3 ran, 1 checks, 2 ignored, 0 filtered out, 10 ms)\n\n", mock->getOutput().asCharString());
}

TEST(TestOutput, printTestsEndedWithFailures)
{
    result->addFailure(*f);
    printer->flush();
    printer->printTestsEnded(*result);
    STRCMP_EQUAL("\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 10 ms)\n\n", mock->getOutput().asCharString());
}

TEST(TestOutput, printTestsEndedWithNoTestsRunOrIgnored)
{
    result->countTest();
    printer->flush();
    printer->printTestsEnded(*result);
    STRCMP_EQUAL("\nErrors (ran nothing, 1 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 10 ms)\n"
                 "Note: test run failed because no tests were run or ignored. Assuming something went wrong. "
                 "This often happens because of linking errors or typos in test filter.\n\n",
        mock->getOutput().asCharString());
}

class CompositeTestOutputTestStringBufferTestOutput : public StringBufferTestOutput
{
  public:
    virtual void printTestsStarted() _override
    {
      output += "Test Start\n";
    }

    virtual void printTestsEnded(const TestResult& result) _override
    {
      output += StringFromFormat("Test End %d\n", (int) result.getTestCount());
    }

    void printCurrentGroupStarted(const UtestShell& test) _override
    {
      output += StringFromFormat("Group %s Start\n", test.getGroup().asCharString());
    }

    void printCurrentGroupEnded(const TestResult& res) _override
    {
      output += StringFromFormat("Group End %d\n", (int) res.getTestCount());
    }

    virtual void printCurrentTestStarted(const UtestShell&) _override
    {
      output += "s";
    }

    void flush() _override
    {
      output += "flush";
    }

    virtual bool isVerbose()
    {
      return verbose_ == level_verbose || verbose_ == level_veryVerbose;
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
  CompositeTestOutputTestStringBufferTestOutput* output1;
  CompositeTestOutputTestStringBufferTestOutput* output2;
  CompositeTestOutput compositeOutput;
  TestResult* result;
  UtestShell* test;

  void setup() _override
  {
    output1 = new CompositeTestOutputTestStringBufferTestOutput;
    output2 = new CompositeTestOutputTestStringBufferTestOutput;
    compositeOutput.setOutputOne(output1);
    compositeOutput.setOutputTwo(output2);
    result = new TestResult(compositeOutput);
    test = new UtestShell("Group", "Name", "file", 10);
  }

  void teardown() _override
  {
    delete test;
    delete result;
  }
};

TEST(CompositeTestOutput, TestStartedAndEnded)
{
  compositeOutput.printTestsStarted();
  compositeOutput.printTestsEnded(*result);
  STRCMP_EQUAL("Test Start\nTest End 0\n", output1->getOutput().asCharString());
  STRCMP_EQUAL("Test Start\nTest End 0\n", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, CurrentTestStartedAndEnded)
{
  compositeOutput.printCurrentTestStarted(*test);
  compositeOutput.printCurrentTestEnded(*result);
  STRCMP_EQUAL("s.", output1->getOutput().asCharString());
  STRCMP_EQUAL("s.", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, CurrentGroupStartedAndEnded)
{
  compositeOutput.printCurrentGroupStarted(*test);
  compositeOutput.printCurrentGroupEnded(*result);
  STRCMP_EQUAL("Group Group Start\nGroup End 0\n", output1->getOutput().asCharString());
  STRCMP_EQUAL("Group Group Start\nGroup End 0\n", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, PrintBuffer)
{
  compositeOutput.printBuffer("Boo");
  STRCMP_EQUAL("Boo", output1->getOutput().asCharString());
  STRCMP_EQUAL("Boo", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, printChar)
{
  compositeOutput.print("Boo");
  STRCMP_EQUAL("Boo", output1->getOutput().asCharString());
  STRCMP_EQUAL("Boo", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, printLong)
{
  long ten = 10;
  compositeOutput.print(ten);
  STRCMP_EQUAL("10", output1->getOutput().asCharString());
  STRCMP_EQUAL("10", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, PrintSize)
{
    size_t ten = 10;
    compositeOutput.print(ten);
    STRCMP_EQUAL("10", output1->getOutput().asCharString());
    STRCMP_EQUAL("10", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, printDouble)
{
  compositeOutput.printDouble(1.01);
  STRCMP_EQUAL("1.01", output1->getOutput().asCharString());
  STRCMP_EQUAL("1.01", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, verbose)
{
  compositeOutput.verbose(TestOutput::level_verbose);
  CHECK(output1->isVerbose());
  CHECK(output2->isVerbose());
}

TEST(CompositeTestOutput, color)
{
  compositeOutput.color();
  CHECK(output1->isColor());
  CHECK(output2->isColor());
}

TEST(CompositeTestOutput, PrintTestFailure)
{
  TestOutput::WorkingEnvironment previousEnvironment = TestOutput::getWorkingEnvironment();
  TestOutput::setWorkingEnvironment(TestOutput::eclipse);
  TestFailure failure(test, "file", 10, "failed");
  compositeOutput.printFailure(failure);
  STRCMP_EQUAL("\nfile:10: error: Failure in TEST(Group, Name)\n\tfailed\n\n", output1->getOutput().asCharString());
  STRCMP_EQUAL("\nfile:10: error: Failure in TEST(Group, Name)\n\tfailed\n\n", output2->getOutput().asCharString());
  TestOutput::setWorkingEnvironment(previousEnvironment);
}

TEST(CompositeTestOutput, PrintTestRun)
{
  compositeOutput.printTestRun(1, 2);
  STRCMP_EQUAL("Test run 1 of 2\n", output1->getOutput().asCharString());
  STRCMP_EQUAL("Test run 1 of 2\n", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, setProgressIndicator)
{
  compositeOutput.setProgressIndicator("?");
  STRCMP_EQUAL("?", output1->getProgressIndicator());
  STRCMP_EQUAL("?", output2->getProgressIndicator());
}

TEST(CompositeTestOutput, flush)
{
  compositeOutput.flush();
  STRCMP_EQUAL("flush", output1->getOutput().asCharString());
  STRCMP_EQUAL("flush", output2->getOutput().asCharString());
}

TEST(CompositeTestOutput, deletePreviousInstanceWhenSettingNew)
{
  compositeOutput.setOutputOne(new CompositeTestOutput);
  compositeOutput.setOutputTwo(new CompositeTestOutput);

  // CHECK NO MEMORY LEAKS
}
