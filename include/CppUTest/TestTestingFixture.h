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

    TestTestingFixture()
    {
        output_ = new StringBufferTestOutput();
        result_ = new TestResult(*output_);
        genTest_ = new ExecFunctionTestShell();
        registry_ = new TestRegistry();

        registry_->setCurrentRegistry(registry_);
        registry_->addTest(genTest_);

        lineOfCodeExecutedAfterCheck = false;
    }

    virtual ~TestTestingFixture()
    {
        registry_->setCurrentRegistry(0);
        delete registry_;
        delete result_;
        delete output_;
        delete genTest_;
    }

    void addTest(UtestShell * test)
    {
        registry_->addTest(test);
    }

    void setTestFunction(void(*testFunction)())
    {
        genTest_->testFunction_ = testFunction;
    }

    void setSetup(void(*setupFunction)())
    {
        genTest_->setup_ = setupFunction;
    }

    void setTeardown(void(*teardownFunction)())
    {
        genTest_->teardown_ = teardownFunction;
    }

    void runTestWithMethod(void(*method)())
    {
        setTestFunction(method);
        runAllTests();
    }

    void runAllTests()
    {
        registry_->runAllTests(*result_);
    }

    int getFailureCount()
    {
        return result_->getFailureCount();
    }

    int getCheckCount()
    {
        return result_->getCheckCount();
    }

    int getIgnoreCount()
    {
        return result_->getIgnoredCount();
    }

    bool hasTestFailed()
    {
        return genTest_->hasFailed();
    }

    void assertPrintContains(const SimpleString& contains)
    {
        assertPrintContains(getOutput(), contains);
    }

    const SimpleString& getOutput()
    {
        return output_->getOutput();
    }

    static void assertPrintContains(const SimpleString& output, const SimpleString& contains)
    {
        STRCMP_CONTAINS(contains.asCharString(), output.asCharString());
    }

    int getRunCount()
    {
    	return result_->getRunCount();
    }

    void checkTestFailsWithProperTestLocation(const char* text, const char* file, int line);

    static void lineExecutedAfterCheck();
    static bool lineOfCodeExecutedAfterCheck;

    TestRegistry* registry_;
    ExecFunctionTestShell* genTest_;
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
