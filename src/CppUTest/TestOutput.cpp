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
#include "CppUTest/PlatformSpecificFunctions.h"

TestOutput::WorkingEnvironment TestOutput::workingEnvironment_ = TestOutput::detectEnvironment;

void TestOutput::setWorkingEnvironment(TestOutput::WorkingEnvironment workEnvironment)
{
    workingEnvironment_ = workEnvironment;
}

TestOutput::WorkingEnvironment TestOutput::getWorkingEnvironment()
{
    if (workingEnvironment_ == TestOutput::detectEnvironment)
        return PlatformSpecificGetWorkingEnvironment();
    return workingEnvironment_;
}


ConsoleTestOutput::ConsoleTestOutput() :
    dotCount_(0), verbose_(false), color_(false), progressIndication_(".")
{
}

ConsoleTestOutput::~ConsoleTestOutput()
{
}

void ConsoleTestOutput::verbose()
{
    verbose_ = true;
}

void ConsoleTestOutput::color()
{
    color_ = true;
}

void ConsoleTestOutput::print(const char* str)
{
    printBuffer(str);
}

void ConsoleTestOutput::print(long n)
{
    print(StringFrom(n).asCharString());
}

void ConsoleTestOutput::printDouble(double d)
{
    print(StringFrom(d).asCharString());
}

ConsoleTestOutput& operator<<(ConsoleTestOutput& p, const char* s)
{
    p.print(s);
    return p;
}

ConsoleTestOutput& operator<<(ConsoleTestOutput& p, long int i)
{
    p.print(i);
    return p;
}

void ConsoleTestOutput::printCurrentTestStarted(const UtestShell& test)
{
    if (verbose_) print(test.getFormattedName().asCharString());

    if (test.willRun()) {
       setProgressIndicator(".");
    }
    else {
       setProgressIndicator("!");
    }
}

void ConsoleTestOutput::printCurrentTestEnded(const TestResult& res)
{
    if (verbose_) {
        print(" - ");
        print(res.getCurrentTestTotalExecutionTime());
        print(" ms\n");
    }
    else {
        printProgressIndicator();
    }
}

void ConsoleTestOutput::printProgressIndicator()
{
    print(progressIndication_);
    if (++dotCount_ % 50 == 0) print("\n");
}

void ConsoleTestOutput::setProgressIndicator(const char* indicator)
{
    progressIndication_ = indicator;
}

void ConsoleTestOutput::printTestsStarted()
{
}

void ConsoleTestOutput::printCurrentGroupStarted(const UtestShell& /*test*/)
{
}

void ConsoleTestOutput::printCurrentGroupEnded(const TestResult& /*res*/)
{
}

void ConsoleTestOutput::printTestsEnded(const TestResult& result)
{
    print("\n");
    if (result.getFailureCount() > 0) {
        if (color_) {
            print("\033[31;1m");
        }
        print("Errors (");
        print(result.getFailureCount());
        print(" failures, ");
    }
    else {
        if (color_) {
            print("\033[32;1m");
        }
        print("OK (");
    }
    print(result.getTestCount());
    print(" tests, ");
    print(result.getRunCount());
    print(" ran, ");
    print(result.getCheckCount());
    print(" checks, ");
    print(result.getIgnoredCount());
    print(" ignored, ");
    print(result.getFilteredOutCount());
    print(" filtered out, ");
    print(result.getTotalExecutionTime());
    print(" ms)");
    if (color_) {
        print("\033[m");
    }
    print("\n\n");
}

void ConsoleTestOutput::printTestRun(int number, int total)
{
    if (total > 1) {
        print("Test run ");
        print(number);
        print(" of ");
        print(total);
        print("\n");
    }
}

void ConsoleTestOutput::print(const TestFailure& failure)
{
    if (failure.isOutsideTestFile() || failure.isInHelperFunction())
        printFileAndLineForTestAndFailure(failure);
    else
        printFileAndLineForFailure(failure);

    printFailureMessage(failure.getMessage());
}

void ConsoleTestOutput::printFileAndLineForTestAndFailure(const TestFailure& failure)
{
    printErrorInFileOnLineFormattedForWorkingEnvironment(failure.getTestFileName(), failure.getTestLineNumber());
    printFailureInTest(failure.getTestName());
    printErrorInFileOnLineFormattedForWorkingEnvironment(failure.getFileName(), failure.getFailureLineNumber());
}

void ConsoleTestOutput::printFileAndLineForFailure(const TestFailure& failure)
{
    printErrorInFileOnLineFormattedForWorkingEnvironment(failure.getFileName(), failure.getFailureLineNumber());
    printFailureInTest(failure.getTestName());
}

void ConsoleTestOutput::printFailureInTest(SimpleString testName)
{
    print(" Failure in ");
    print(testName.asCharString());
}

void ConsoleTestOutput::printFailureMessage(SimpleString reason)
{
    print("\n");
    print("\t");
    print(reason.asCharString());
    print("\n\n");
}

void ConsoleTestOutput::printErrorInFileOnLineFormattedForWorkingEnvironment(SimpleString file, int lineNumber)
{
    if (ConsoleTestOutput::getWorkingEnvironment() == ConsoleTestOutput::vistualStudio)
        printVistualStudioErrorInFileOnLine(file, lineNumber);
    else
        printEclipseErrorInFileOnLine(file, lineNumber);
}

void ConsoleTestOutput::printEclipseErrorInFileOnLine(SimpleString file, int lineNumber)
{
    print("\n");
    print(file.asCharString());
    print(":");
    print(lineNumber);
    print(":");
    print(" error:");
}

void ConsoleTestOutput::printVistualStudioErrorInFileOnLine(SimpleString file, int lineNumber)
{
    print("\n");
    print(file.asCharString());
    print("(");
    print(lineNumber);
    print("):");
    print(" error:");
}

void ConsoleTestOutput::printBuffer(const char* s)
{
    while (*s) {
        PlatformSpecificPutchar(*s);
        s++;
    }
    flush();
}

void ConsoleTestOutput::flush()
{
    PlatformSpecificFlush();
}

StringBufferTestOutput::~StringBufferTestOutput()
{
}

CompositeTestOutput::CompositeTestOutput()
  : outputOne_(NULL), outputTwo_(NULL)
{
}

CompositeTestOutput::~CompositeTestOutput()
{
  delete outputOne_;
  delete outputTwo_;
}

void CompositeTestOutput::setOutputOne(TestOutput* output)
{
  delete outputOne_;
  outputOne_ = output;
}

void CompositeTestOutput::setOutputTwo(TestOutput* output)
{
  delete outputTwo_;
  outputTwo_ = output;
}

void CompositeTestOutput::printTestsStarted()
{
  if (outputOne_) outputOne_->printTestsStarted();
  if (outputTwo_) outputTwo_->printTestsStarted();
}

void CompositeTestOutput::printTestsEnded(const TestResult& result)
{
  if (outputOne_) outputOne_->printTestsEnded(result);
  if (outputTwo_) outputTwo_->printTestsEnded(result);
}

void CompositeTestOutput::printCurrentTestStarted(const UtestShell& test)
{
  if (outputOne_) outputOne_->printCurrentTestStarted(test);
  if (outputTwo_) outputTwo_->printCurrentTestStarted(test);
}

void CompositeTestOutput::printCurrentTestEnded(const TestResult& res)
{
  if (outputOne_) outputOne_->printCurrentTestEnded(res);
  if (outputTwo_) outputTwo_->printCurrentTestEnded(res);
}

void CompositeTestOutput::printCurrentGroupStarted(const UtestShell& test)
{
  if (outputOne_) outputOne_->printCurrentGroupStarted(test);
  if (outputTwo_) outputTwo_->printCurrentGroupStarted(test);
}

void CompositeTestOutput::printCurrentGroupEnded(const TestResult& res)
{
  if (outputOne_) outputOne_->printCurrentGroupEnded(res);
  if (outputTwo_) outputTwo_->printCurrentGroupEnded(res);
}

void CompositeTestOutput::verbose()
{
  if (outputOne_) outputOne_->verbose();
  if (outputTwo_) outputTwo_->verbose();
}

void CompositeTestOutput::color()
{
  if (outputOne_) outputOne_->color();
  if (outputTwo_) outputTwo_->color();
}

void CompositeTestOutput::printBuffer(const char* buffer)
{
  if (outputOne_) outputOne_->printBuffer(buffer);
  if (outputTwo_) outputTwo_->printBuffer(buffer);
}

void CompositeTestOutput::print(const char* buffer)
{
  if (outputOne_) outputOne_->print(buffer);
  if (outputTwo_) outputTwo_->print(buffer);
}

void CompositeTestOutput::print(long number)
{
  if (outputOne_) outputOne_->print(number);
  if (outputTwo_) outputTwo_->print(number);
}

void CompositeTestOutput::printDouble(double number)
{
  if (outputOne_) outputOne_->printDouble(number);
  if (outputTwo_) outputTwo_->printDouble(number);
}

void CompositeTestOutput::print(const TestFailure& failure)
{
  if (outputOne_) outputOne_->print(failure);
  if (outputTwo_) outputTwo_->print(failure);
}

void CompositeTestOutput::setProgressIndicator(const char* indicator)
{
  if (outputOne_) outputOne_->setProgressIndicator(indicator);
  if (outputTwo_) outputTwo_->setProgressIndicator(indicator);
}

void CompositeTestOutput::flush()
{
  if (outputOne_) outputOne_->flush();
  if (outputTwo_) outputTwo_->flush();
}

