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

#ifndef D_TestOutput_h
#define D_TestOutput_h

///////////////////////////////////////////////////////////////////////////////
//
//  This is a minimal printer interface.
//  We kept streams out to keep footprint small, and so the test
//  harness could be used with less capable compilers so more
//  platforms could use this test harness
//
///////////////////////////////////////////////////////////////////////////////

#include "SimpleString.h"

class UtestShell;
class TestFailure;
class TestResult;

class TestOutput
{
public:
    enum WorkingEnvironment {visualStudio, eclipse, detectEnvironment};
    enum VerbosityLevel {level_quiet, level_verbose, level_veryVerbose};

    explicit TestOutput();
    virtual ~TestOutput();

    virtual void printTestsStarted();
    virtual void printTestsEnded(const TestResult& result);
    virtual void printCurrentTestStarted(const UtestShell& test);
    virtual void printCurrentTestEnded(const TestResult& res);
    virtual void printCurrentGroupStarted(const UtestShell& test);
    virtual void printCurrentGroupEnded(const TestResult& res);

    virtual void verbose(VerbosityLevel level);
    virtual void color();
    virtual void printBuffer(const char*)=0;
    virtual void print(const char*);
    virtual void print(long);
    virtual void print(size_t);
    virtual void printDouble(double);
    virtual void printFailure(const TestFailure& failure);
    virtual void printTestRun(size_t number, size_t total);
    virtual void setProgressIndicator(const char*);

    virtual void printVeryVerbose(const char*);

    virtual void flush()=0;

    static void setWorkingEnvironment(WorkingEnvironment workEnvironment);
    static WorkingEnvironment getWorkingEnvironment();

protected:

    virtual void printEclipseErrorInFileOnLine(SimpleString file, size_t lineNumber);
    virtual void printVisualStudioErrorInFileOnLine(SimpleString file, size_t lineNumber);

    virtual void printProgressIndicator();
    void printFileAndLineForTestAndFailure(const TestFailure& failure);
    void printFileAndLineForFailure(const TestFailure& failure);
    void printFailureInTest(SimpleString testName);
    void printFailureMessage(SimpleString reason);
    void printErrorInFileOnLineFormattedForWorkingEnvironment(SimpleString testFile, size_t lineNumber);

    TestOutput(const TestOutput&);
    TestOutput& operator=(const TestOutput&);

    int dotCount_;
    VerbosityLevel verbose_;
    bool color_;
    const char* progressIndication_;

    static WorkingEnvironment workingEnvironment_;
};

TestOutput& operator<<(TestOutput&, const char*);
TestOutput& operator<<(TestOutput&, long);

///////////////////////////////////////////////////////////////////////////////
//
//  ConsoleTestOutput.h
//
//  Printf Based Solution
//
///////////////////////////////////////////////////////////////////////////////

class ConsoleTestOutput: public TestOutput
{
public:
    explicit ConsoleTestOutput()
    {
    }
    virtual ~ConsoleTestOutput() CPPUTEST_DESTRUCTOR_OVERRIDE
    {
    }

    virtual void printBuffer(const char* s) CPPUTEST_OVERRIDE;
    virtual void flush() CPPUTEST_OVERRIDE;

private:
    ConsoleTestOutput(const ConsoleTestOutput&);
    ConsoleTestOutput& operator=(const ConsoleTestOutput&);
};

///////////////////////////////////////////////////////////////////////////////
//
//  StringBufferTestOutput.h
//
//  TestOutput for test purposes
//
///////////////////////////////////////////////////////////////////////////////


class StringBufferTestOutput: public TestOutput
{
public:
    explicit StringBufferTestOutput()
    {
    }

    virtual ~StringBufferTestOutput() CPPUTEST_DESTRUCTOR_OVERRIDE;

    void printBuffer(const char* s) CPPUTEST_OVERRIDE
    {
        output += s;
    }

    void flush() CPPUTEST_OVERRIDE
    {
        output = "";
    }

    const SimpleString& getOutput()
    {
        return output;
    }

protected:
    SimpleString output;

private:
    StringBufferTestOutput(const StringBufferTestOutput&);
    StringBufferTestOutput& operator=(const StringBufferTestOutput&);

};

class CompositeTestOutput : public TestOutput
{
public:
    virtual void setOutputOne(TestOutput* output);
    virtual void setOutputTwo(TestOutput* output);

    CompositeTestOutput();
    virtual ~CompositeTestOutput() CPPUTEST_DESTRUCTOR_OVERRIDE;

    virtual void printTestsStarted() CPPUTEST_OVERRIDE;
    virtual void printTestsEnded(const TestResult& result) CPPUTEST_OVERRIDE;

    virtual void printCurrentTestStarted(const UtestShell& test) CPPUTEST_OVERRIDE;
    virtual void printCurrentTestEnded(const TestResult& res) CPPUTEST_OVERRIDE;
    virtual void printCurrentGroupStarted(const UtestShell& test) CPPUTEST_OVERRIDE;
    virtual void printCurrentGroupEnded(const TestResult& res) CPPUTEST_OVERRIDE;

    virtual void verbose(VerbosityLevel level) CPPUTEST_OVERRIDE;
    virtual void color() CPPUTEST_OVERRIDE;
    virtual void printBuffer(const char*) CPPUTEST_OVERRIDE;
    virtual void print(const char*) CPPUTEST_OVERRIDE;
    virtual void print(long) CPPUTEST_OVERRIDE;
    virtual void print(size_t) CPPUTEST_OVERRIDE;
    virtual void printDouble(double) CPPUTEST_OVERRIDE;
    virtual void printFailure(const TestFailure& failure) CPPUTEST_OVERRIDE;
    virtual void setProgressIndicator(const char*) CPPUTEST_OVERRIDE;

    virtual void printVeryVerbose(const char*) CPPUTEST_OVERRIDE;

    virtual void flush() CPPUTEST_OVERRIDE;

protected:
    CompositeTestOutput(const TestOutput&);
    CompositeTestOutput& operator=(const TestOutput&);

private:
    TestOutput* outputOne_;
    TestOutput* outputTwo_;
};

#endif
