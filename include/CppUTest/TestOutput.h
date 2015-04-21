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

#ifndef D_TestOutput_h
#define D_TestOutput_h

///////////////////////////////////////////////////////////////////////////////
//
//  This is a minimal printer inteface.
//  We kept streams out too keep footprint small, and so the test
//  harness could be used with less capable compilers so more
//  platforms could use this test harness
//
///////////////////////////////////////////////////////////////////////////////

class UtestShell;
class TestFailure;
class TestResult;

class TestOutput
{
public:
    explicit TestOutput() {}
    virtual ~TestOutput() {}

    virtual void printTestsStarted() = 0;
    virtual void printTestsEnded(const TestResult& result) = 0;
    virtual void printCurrentTestStarted(const UtestShell& test) = 0;
    virtual void printCurrentTestEnded(const TestResult& res) = 0;
    virtual void printCurrentGroupStarted(const UtestShell& test) = 0;
    virtual void printCurrentGroupEnded(const TestResult& res) = 0;

    virtual void verbose() = 0;
    virtual void color() = 0;
    virtual bool isVerbose() = 0;
    virtual void print(const char*) = 0;
    virtual void print(const TestFailure& failure) = 0;
    virtual void printTestRun(int number, int total) = 0;

    enum WorkingEnvironment {vistualStudio, eclipse, detectEnvironment};

    static void setWorkingEnvironment(WorkingEnvironment workEnvironment);
    static WorkingEnvironment getWorkingEnvironment();

protected:

    TestOutput(const TestOutput&);
    TestOutput& operator=(const TestOutput&);

    static WorkingEnvironment workingEnvironment_;
};

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
    explicit ConsoleTestOutput();
    virtual ~ConsoleTestOutput();

    virtual void printTestsStarted() _override;
    virtual void printTestsEnded(const TestResult& result) _override;
    virtual void printCurrentTestStarted(const UtestShell& test) _override;
    virtual void printCurrentTestEnded(const TestResult& res) _override;
    virtual void printCurrentGroupStarted(const UtestShell& test) _override;
    virtual void printCurrentGroupEnded(const TestResult& res) _override;

    virtual void verbose() _override;
    virtual bool isVerbose() _override { return verbose_; }
    virtual void color() _override;
    virtual void printBuffer(const char*);
    virtual void print(const char*) _override;
    virtual void print(long);
    virtual void printDouble(double);
    virtual void print(const TestFailure& failure) _override;
    virtual void printTestRun(int number, int total) _override;
    virtual void setProgressIndicator(const char*);

    virtual void flush();

protected:

    virtual void printEclipseErrorInFileOnLine(SimpleString file, int lineNumber);
    virtual void printVistualStudioErrorInFileOnLine(SimpleString file, int lineNumber);

    virtual void printProgressIndicator();
    void printFileAndLineForTestAndFailure(const TestFailure& failure);
    void printFileAndLineForFailure(const TestFailure& failure);
    void printFailureInTest(SimpleString testName);
    void printFailureMessage(SimpleString reason);
    void printErrorInFileOnLineFormattedForWorkingEnvironment(SimpleString testFile, int lineNumber);

    int dotCount_;
    bool verbose_;
    bool color_;
    const char* progressIndication_;

private:
    ConsoleTestOutput(const ConsoleTestOutput&);
    ConsoleTestOutput& operator=(const ConsoleTestOutput&);
};

ConsoleTestOutput& operator<<(ConsoleTestOutput&, const char*);
ConsoleTestOutput& operator<<(ConsoleTestOutput&, long);

///////////////////////////////////////////////////////////////////////////////
//
//  StringBufferTestOutput.h
//
//  TestOutput for test purposes
//
///////////////////////////////////////////////////////////////////////////////


class StringBufferTestOutput: public ConsoleTestOutput
{
public:
    explicit StringBufferTestOutput() {}
    virtual ~StringBufferTestOutput();

    void printBuffer(const char* s) _override
    {
        output += s;
    }

    void flush() _override
    {
        output = "";
    }

    const SimpleString& getOutput()
    {
        return output;
    }

private:
    SimpleString output;

    StringBufferTestOutput(const StringBufferTestOutput&);
    StringBufferTestOutput& operator=(const StringBufferTestOutput&);

};

#endif
