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


#include "CommandLineTestRunner.h"
#include "MemoryLeakWarningPlugin.h"
#include "TestOutput.h"
#include "RealTestOutput.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int CommandLineTestRunner::repeat = 0;
TestOutput* CommandLineTestRunner::output;

int CommandLineTestRunner::RunAllTests(int ac, char** av)
{
	MemoryLeakWarningPlugin memLeakWarn;
	TestRegistry::getCurrentRegistry()->installPlugin(&memLeakWarn);
  memLeakWarn.Enable();
  RealTestOutput realTestOutput;
  output = &realTestOutput;

  SetOptions(ac, av);
  bool repeating = (repeat > 1);
  int loopCount = 1;
  int failureCount = 0;

  do
    {
      if (repeating)
        {
          output->print("Test run ");
          output->print(loopCount);
          output->print("\n");
        }

      TestResult tr(*output);
      TestRegistry::getCurrentRegistry()->runAllTests(tr, output);
      failureCount = tr.getFailureCount();
    }
  while (++loopCount <= repeat);

  realTestOutput << memLeakWarn.FinalReport();
  
  return failureCount;
}

void CommandLineTestRunner::SetOptions(int ac, char** av)
{
  repeat = 1;
  for (int i = 1; i < ac; i++)
    {
      if (0 == strcmp("-v", av[i]))
        TestRegistry::getCurrentRegistry()->verbose();
      else if (av[i] == strstr(av[i], "-r"))
        SetRepeatCount(ac, av, i);
      else if (av[i] == strstr(av[i], "-g"))
        SetGroupFilter(ac, av, i);
      else if (av[i] == strstr(av[i], "-n"))
        SetNameFilter(ac, av, i);
      else
        {
          output->print("usage [-v] [-r#] [-g groupName] [-n testName]\n");
        }
    }
}


void CommandLineTestRunner::SetRepeatCount(int ac, char** av, int& i)
{
  repeat = 0;

  if (strlen(av[i]) > 2)
    repeat = atoi(av[i] + 2);
  else if (i + 1 < ac)
    {
      repeat = atoi(av[i+1]);
      if (repeat != 0)
        i++;
    }

  if (0 == repeat)
    repeat = 2;

}

void CommandLineTestRunner::SetGroupFilter(int ac, char** av, int& i)
{
  const char* groupFilter;

  if (strlen(av[i]) > 2)
    groupFilter = av[i] + 2;
  else if (i + 1 < ac)
    {
      groupFilter = av[++i];
    }
  TestRegistry::getCurrentRegistry()->groupFilter(groupFilter);

}

void CommandLineTestRunner::SetNameFilter(int ac, char** av, int& i)
{
  const char* nameFilter;

  if (strlen(av[i]) > 2)
    nameFilter = av[i] + 2;
  else if (i + 1 < ac)
    {
      nameFilter = av[++i];
    }
  TestRegistry::getCurrentRegistry()->nameFilter(nameFilter);

}

