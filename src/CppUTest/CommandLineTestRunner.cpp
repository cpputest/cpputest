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


#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/MemoryLeakWarningPlugin.h"
#include "CppUTest/TestOutput.h"
#include <stdlib.h>

CommandLineTestRunner::CommandLineTestRunner(int ac, const char** av, TestOutput* output)
: argc(ac), argv(av), output_(output)
{
}

CommandLineTestRunner::~CommandLineTestRunner()
{
	delete arguments;
}

int CommandLineTestRunner::RunAllTests(int ac, char** av)
{
	return RunAllTests(ac, const_cast<const char**>(av));
}

int CommandLineTestRunner::RunAllTests(int ac, const char** av)
{
	ConsoleTestOutput output;
	CommandLineTestRunner runner(ac, av, &output);
	return runner.runAllTestsMain();
}

int CommandLineTestRunner::runAllTestsMain()
{
	int testResult = 0;

	SetPointerPlugin pPlugin(DEF_PLUGIN_SET_POINTER);
	TestRegistry::getCurrentRegistry()->installPlugin(&pPlugin);

	MemoryLeakWarningPlugin memLeakWarn(DEF_PLUGIN_MEM_LEAK);
	TestRegistry::getCurrentRegistry()->installPlugin(&memLeakWarn);
	memLeakWarn.Enable();

	if (!parseArguments(TestRegistry::getCurrentRegistry()->getFirstPlugin()))
		return 1;

	testResult = runAllTests();

	TestRegistry::getCurrentRegistry()->cleanup();
	if (testResult == 0)
		*output_ << memLeakWarn.FinalReport(3); // TODO: where are the 3 leaks from
	return testResult;
}

void CommandLineTestRunner::initializeTestRun()
{
  	TestRegistry::getCurrentRegistry()->groupFilter(arguments->getGroupFilter());
  	TestRegistry::getCurrentRegistry()->nameFilter(arguments->getNameFilter());
  	if (arguments->isVerbose())
  		output_->verbose();
}

int CommandLineTestRunner::runAllTests()
{
	initializeTestRun();
  	int loopCount = 0;
  	int failureCount = 0;
  	int repeat_ = arguments->getRepeatCount();

  	while (loopCount++ < repeat_) {
		output_->printTestRun(loopCount, repeat_);
		TestResult tr(*output_);
		TestRegistry::getCurrentRegistry()->runAllTests(tr);
		failureCount += tr.getFailureCount();
	}

	return failureCount;
}

bool CommandLineTestRunner::parseArguments(TestPlugin* plugin)
{
	arguments = new CommandLineArguments(argc, argv, plugin);
	if (arguments->parse())
		return true;
	else
	{
        output_->print(arguments->usage());
        return false;
	}
}

bool CommandLineTestRunner::isVerbose()
{
	return arguments->isVerbose();
}

int CommandLineTestRunner::getRepeatCount()
{
	return arguments->getRepeatCount();
}

SimpleString CommandLineTestRunner::getGroupFilter()
{
	return arguments->getGroupFilter();
}

SimpleString CommandLineTestRunner::getNameFilter()
{
	return arguments->getNameFilter();
}

CommandLineArguments::OutputType CommandLineTestRunner::getOutputType()
{
	return arguments->getOutputType();
}
