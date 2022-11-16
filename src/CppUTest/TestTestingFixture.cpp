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

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestTestingFixture.h"

bool TestTestingFixture::lineOfCodeExecutedAfterCheck = false;

TestTestingFixture::TestTestingFixture()
{
    output_ = new StringBufferTestOutput();
    result_ = new TestResult(*output_);
    genTest_ = new ExecFunctionTestShell();
    registry_ = new TestRegistry();
    ownsExecFunction_ = false;

    registry_->setCurrentRegistry(registry_);
    registry_->addTest(genTest_);

    lineOfCodeExecutedAfterCheck = false;
}

void TestTestingFixture::flushOutputAndResetResult()
{
     output_->flush();
     delete result_;
     result_ = new TestResult(*output_);
}

TestTestingFixture::~TestTestingFixture()
{
    registry_->setCurrentRegistry(NULLPTR);
    clearExecFunction();
    delete registry_;
    delete result_;
    delete output_;
    delete genTest_;
}

void TestTestingFixture::clearExecFunction()
{
    if (genTest_->testFunction_ && ownsExecFunction_)
        delete genTest_->testFunction_;
}

void TestTestingFixture::addTest(UtestShell * test)
{
    registry_->addTest(test);
}

void TestTestingFixture::setTestFunction(void(*testFunction)())
{
    clearExecFunction();

    genTest_->testFunction_ = new ExecFunctionWithoutParameters(testFunction);
    ownsExecFunction_ = true;
}

void TestTestingFixture::setTestFunction(ExecFunction* testFunction)
{
    clearExecFunction();

    genTest_->testFunction_ = testFunction;

    ownsExecFunction_ = false;
}

void TestTestingFixture::setSetup(void(*setupFunction)())
{
    genTest_->setup_ = setupFunction;
}

void TestTestingFixture::setTeardown(void(*teardownFunction)())
{
    genTest_->teardown_ = teardownFunction;
}

void TestTestingFixture::installPlugin(TestPlugin* plugin)
{
    registry_->installPlugin(plugin);
}

void TestTestingFixture::setRunTestsInSeperateProcess()
{
    registry_->setRunTestsInSeperateProcess();
}

void TestTestingFixture::setOutputVerbose()
{
    output_->verbose(TestOutput::level_verbose);
}

void TestTestingFixture::runTestWithMethod(void(*method)())
{
    setTestFunction(method);
    runAllTests();
}

void TestTestingFixture::runAllTests()
{
    registry_->runAllTests(*result_);
}

size_t TestTestingFixture::getFailureCount()
{
    return result_->getFailureCount();
}

size_t TestTestingFixture::getCheckCount()
{
    return result_->getCheckCount();
}

size_t TestTestingFixture::getTestCount()
{
    return result_->getTestCount();
}

size_t TestTestingFixture::getIgnoreCount()
{
    return result_->getIgnoredCount();
}

TestRegistry* TestTestingFixture::getRegistry()
{
    return registry_;
}

bool TestTestingFixture::hasTestFailed()
{
    return genTest_->hasFailed();
}

void TestTestingFixture::assertPrintContains(const SimpleString& contains)
{
    STRCMP_CONTAINS(contains.asCharString(), getOutput().asCharString());
}

void TestTestingFixture::assertPrintContainsNot(const SimpleString& contains)
{
    CHECK(! getOutput().contains(contains));
}


const SimpleString& TestTestingFixture::getOutput()
{
    return output_->getOutput();
}

size_t TestTestingFixture::getRunCount()
{
  	return result_->getRunCount();
}

void TestTestingFixture::lineExecutedAfterCheck()
{
    lineOfCodeExecutedAfterCheck = true;
}

void TestTestingFixture::checkTestFailsWithProperTestLocation(const char* text, const char* file, size_t line)
{
    if (getFailureCount() != 1)
      FAIL_LOCATION(StringFromFormat("Expected one test failure, but got %d amount of test failures", (int) getFailureCount()).asCharString(), file, line);

    STRCMP_CONTAINS_LOCATION(text, output_->getOutput().asCharString(), "", file, line);

    if (lineOfCodeExecutedAfterCheck)
      FAIL_LOCATION("The test should jump/throw on failure and not execute the next line. However, the next line was executed.", file, line);
}

