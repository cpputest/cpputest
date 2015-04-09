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
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/ConsoleTestOutput.h"
#include "CppUTest/JUnitTestOutput.h"
#include "CppUTest/TestRegistry.h"

CommandLineTestRunner::CommandLineTestRunner( TestOutput* output, TestRegistry* registry, CommandLineArguments* arguments ):
    output_(output), arguments_(arguments), registry_(registry)
{

}

CommandLineTestRunner::~CommandLineTestRunner()
{
}

int CommandLineTestRunner::RunAllTests(int ac, char** av)
{
    return RunAllTests(ac, const_cast<const char**> (av));
}

int CommandLineTestRunner::RunAllTests(int ac, const char** av)
{
    int result = 0;
    TestRegistry* registry = TestRegistry::getCurrentRegistry();
    CommandLineArguments arguments( ac, av );
    arguments.parse( registry->getFirstPlugin() );
    TestOutput* output;
    if( arguments.isJUnitOutput() ) {
        JUnitTestOutput* jUnitOutput = new JUnitTestOutput;
        jUnitOutput->setPackageName( arguments.getPackageName() );
        output = jUnitOutput;
    }
    else {
        output = new ConsoleTestOutput( arguments.isVerbose(), arguments.isColor() );
    }

    MemoryLeakWarningPlugin memLeakWarn(DEF_PLUGIN_MEM_LEAK);
    memLeakWarn.destroyGlobalDetectorAndTurnOffMemoryLeakDetectionInDestructor(true);
    registry->installPlugin( &memLeakWarn );

    SetPointerPlugin pPlugin( DEF_PLUGIN_SET_POINTER );
    registry->installPlugin( &pPlugin );

    if(arguments.isSuccesfullyParsed())
    {
        CommandLineTestRunner runner( output, registry, &arguments );
        result = runner.runAllTestsMain();
    }
    else {
        output->print( arguments.usage() );
    }

    registry->removePluginByName( DEF_PLUGIN_SET_POINTER );

    if (result == 0) {
        *output << memLeakWarn.FinalReport(0);
    }
    registry->removePluginByName( DEF_PLUGIN_MEM_LEAK );

    delete output;

    return result;
}

int CommandLineTestRunner::runAllTestsMain()
{
    int testResult = 0;

    if( arguments_->isSuccesfullyParsed() ) {
        testResult = runAllTests();
    }

    return testResult;
}

void CommandLineTestRunner::initializeTestRun()
{
    registry_->setGroupFilters(arguments_->getGroupFilters());
    registry_->setNameFilters(arguments_->getNameFilters());
    if (arguments_->runTestsInSeperateProcess()) registry_->setRunTestsInSeperateProcess();
}

int CommandLineTestRunner::runAllTests()
{
    initializeTestRun();
    int loopCount = 0;
    int failureCount = 0;
    int repeat_ = arguments_->getRepeatCount();

    while (loopCount++ < repeat_) {
        output_->printTestRun(loopCount, repeat_);
        TestResult tr(*output_);
        registry_->runAllTests(tr);
        failureCount += tr.getFailureCount();
    }

    return failureCount;
}
