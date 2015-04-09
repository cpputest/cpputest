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
#include "CppUTest/ConsoleTestOutput.h"
#include "CppUTest/PlatformSpecificFunctions.h"


ConsoleTestOutput::ConsoleTestOutput(bool verbose, bool color) :
    dotCount_(0), verbose_(verbose), color_(color), progressIndication_(".")
{
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
