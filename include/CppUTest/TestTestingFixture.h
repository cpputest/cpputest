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

#ifndef D_TestTestingFixture_H
#define D_TestTestingFixture_H

#include "TestRegistry.h"
#include "TestOutput.h"

class TestTestingFixture
{
public:

    TestTestingFixture();
    virtual ~TestTestingFixture();
    void flushOutputAndResetResult();

    void addTest(UtestShell * test);
    void installPlugin(TestPlugin* plugin);

    void setTestFunction(void(*testFunction)());
    void setTestFunction(ExecFunction* testFunction);
    void setSetup(void(*setupFunction)());
    void setTeardown(void(*teardownFunction)());

    void setOutputVerbose();
    void setRunTestsInSeperateProcess();

    void runTestWithMethod(void(*method)());
    void runAllTests();

    size_t getFailureCount();
    size_t getCheckCount();
    size_t getIgnoreCount();
    size_t getRunCount();
    size_t getTestCount();
    const SimpleString& getOutput();
    TestRegistry* getRegistry();

    bool hasTestFailed();
    void assertPrintContains(const SimpleString& contains);
    void assertPrintContainsNot(const SimpleString& contains);
    void checkTestFailsWithProperTestLocation(const char* text, const char* file, size_t line);

    static void lineExecutedAfterCheck();

private:
    void clearExecFunction();

    static bool lineOfCodeExecutedAfterCheck;

    TestRegistry* registry_;
    ExecFunctionTestShell* genTest_;
    bool ownsExecFunction_;
    StringBufferTestOutput* output_;
    TestResult * result_;
};

class SetBooleanOnDestructorCall
{
    bool& booleanToSet_;
public:
    SetBooleanOnDestructorCall(bool& booleanToSet) : booleanToSet_(booleanToSet)
    {
    }

    virtual ~SetBooleanOnDestructorCall()
    {
        booleanToSet_ = true;
    }
};

#endif
