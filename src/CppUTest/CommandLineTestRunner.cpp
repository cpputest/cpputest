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
#include "CppUTest/RealTestOutput.h"
#include "CppUTest/JUnitTestOutput.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

CommandLineTestRunner::CommandLineTestRunner(TestOutput* output) :
	verbose_(false), output_(output), repeat_(1), groupFilter_(0), nameFilter_(0), outputType_(OUTPUT_NORMAL)
{
}	

CommandLineTestRunner::~CommandLineTestRunner()
{
	if (output_) delete output_;
}

int CommandLineTestRunner::RunAllTests(int ac, char** av)
{
	MemoryLeakWarningPlugin memLeakWarn;
	TestRegistry::getCurrentRegistry()->installPlugin(&memLeakWarn);
	memLeakWarn.Enable();
		
	CommandLineTestRunner runner(new RealTestOutput());
	if (!runner.parseArguments(ac, av)) {
		return 1;
	}

	int testResult = runner.RunAllTests(); 
  	*runner.output_ << memLeakWarn.FinalReport(1);
	return testResult; 
}

void CommandLineTestRunner::initializeTestRun()
{
  	if (groupFilter_) TestRegistry::getCurrentRegistry()->groupFilter(groupFilter_);
  	if (nameFilter_) TestRegistry::getCurrentRegistry()->nameFilter(nameFilter_);	
  	if (verbose_) output_->verbose();
  	if (outputType_ == OUTPUT_JUNIT) {
  		delete output_;
  		output_ = new JUnitTestOutput();
  	}
}

int CommandLineTestRunner::RunAllTests()
{
	initializeTestRun();
  	int loopCount = 0;
  	int failureCount = 0;

  	while (loopCount++ < repeat_) {
		output_->printTestRun(loopCount, repeat_);
		TestResult tr(*output_);
		TestRegistry::getCurrentRegistry()->runAllTests(tr);
		failureCount += tr.getFailureCount();
	}
	
	return failureCount;
}

bool CommandLineTestRunner::parseArguments(int ac, char** av)
{
	bool correctParameters = true;
	for (int i = 1; i < ac; i++) {
		if (strcmp(av[i], "-v") == 0)
			verbose_ = true;
		else if (av[i] == strstr(av[i], "-r"))
			SetRepeatCount(ac, av, i);
      	else if (av[i] == strstr(av[i], "-g"))
        	SetGroupFilter(ac, av, i);
      	else if (av[i] == strstr(av[i], "-n"))
        	SetNameFilter(ac, av, i);
      	else if (av[i] == strstr(av[i], "-o"))
        	correctParameters = SetOutputType(ac, av, i);
		else
			correctParameters = false;

		if (correctParameters == false) {
          output_->print("usage [-v] [-r#] [-g groupName] [-n testName] [-o{normal, junit}]\n");
          return false;
        }
	}
	return true;
}

bool CommandLineTestRunner::isVerbose()
{
	return verbose_;
}

int CommandLineTestRunner::getRepeatCount()
{
	return repeat_;
}

char* CommandLineTestRunner::getGroupFilter()
{
	return groupFilter_;
}

char* CommandLineTestRunner::getNameFilter()
{
	return nameFilter_;
}

CommandLineTestRunner::OutputType CommandLineTestRunner::getOutputType()
{
	return outputType_;
}

void CommandLineTestRunner::SetRepeatCount(int ac, char** av, int& i)
{
  repeat_ = 0;

  if (strlen(av[i]) > 2)
    repeat_ = atoi(av[i] + 2);
  else if (i + 1 < ac)
    {
      repeat_ = atoi(av[i+1]);
      if (repeat_ != 0)
        i++;
    }

  if (0 == repeat_)
    repeat_ = 2;

}

static char* getParameterField(int ac, char** av, int& i)
{
	if (strlen(av[i]) > 2)
		return av[i] + 2;
	else if (i + 1 < ac)
		return av[++i];
	return NULL;
}


void CommandLineTestRunner::SetGroupFilter(int ac, char** av, int& i)
{
  groupFilter_ = getParameterField(ac, av, i);
}

void CommandLineTestRunner::SetNameFilter(int ac, char** av, int& i)
{
   nameFilter_ = getParameterField(ac, av, i);
}

bool CommandLineTestRunner::SetOutputType(int ac, char** av, int& i)
{
	char* outputType = getParameterField(ac, av, i);
	if (outputType == 0) return false;
	
	if (strcmp("normal", outputType) == 0) {
		outputType_ = OUTPUT_NORMAL;
		return true;
	}
	if (strcmp("junit", outputType) == 0) {
		outputType_ = OUTPUT_JUNIT;
		return true;
	}
	return false;
}
