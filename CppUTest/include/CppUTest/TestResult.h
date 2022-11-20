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

///////////////////////////////////////////////////////////////////////////////
//
// A TestResult is a collection of the history of some test runs.  Right now
// it just collects failures.  Really it just prints the failures.
//

#ifndef D_TestResult_h
#define D_TestResult_h

class TestFailure;
class TestOutput;
class UtestShell;

class TestResult
{
public:
    TestResult(TestOutput&);
    DEFAULT_COPY_CONSTRUCTOR(TestResult)
    virtual ~TestResult();

    virtual void testsStarted();
    virtual void testsEnded();
    virtual void currentGroupStarted(UtestShell* test);
    virtual void currentGroupEnded(UtestShell* test);
    virtual void currentTestStarted(UtestShell* test);
    virtual void currentTestEnded(UtestShell* test);

    virtual void countTest();
    virtual void countRun();
    virtual void countCheck();
    virtual void countFilteredOut();
    virtual void countIgnored();
    virtual void addFailure(const TestFailure& failure);
    virtual void print(const char* text);
    virtual void printVeryVerbose(const char* text);

    size_t getTestCount() const
    {
        return testCount_;
    }
    size_t getRunCount() const
    {
        return runCount_;
    }
    size_t getCheckCount() const
    {
        return checkCount_;
    }
    size_t getFilteredOutCount() const
    {
        return filteredOutCount_;
    }
    size_t getIgnoredCount() const
    {
        return ignoredCount_;
    }
    size_t getFailureCount() const
    {
        return failureCount_;
    }

    bool isFailure() const
    {
        return (getFailureCount() != 0) || (getRunCount() + getIgnoredCount() == 0);
    }

    size_t getTotalExecutionTime() const;
    void setTotalExecutionTime(size_t exTime);

    size_t getCurrentTestTotalExecutionTime() const;
    size_t getCurrentGroupTotalExecutionTime() const;
private:

    TestOutput& output_;
    size_t testCount_;
    size_t runCount_;
    size_t checkCount_;
    size_t failureCount_;
    size_t filteredOutCount_;
    size_t ignoredCount_;
    size_t totalExecutionTime_;
    size_t timeStarted_;
    size_t currentTestTimeStarted_;
    size_t currentTestTotalExecutionTime_;
    size_t currentGroupTimeStarted_;
    size_t currentGroupTotalExecutionTime_;
};

#endif
