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
#include "CppUTest/MockTestOutput.h"
#include "GenericTest.h"
#include <string.h>

TEST_GROUP(CommandLineTestRunner)
{
	CommandLineTestRunner* runner;
	MockTestOutput *output;
	void setup()
	{
        UT_FPSET(exit_current_test, exit_current_test_fake);
		output = new MockTestOutput(); 
		runner = new CommandLineTestRunner(output);
	}
	void teardown()
	{
		delete runner;
		delete output;
	}
};

TEST(CommandLineTestRunner, verboseSetMultipleParameters)
{
	int argc = 2;
	char* argv[] = {"tests.exe", "-v"};
	CHECK(runner->parseArguments(argc, argv));
	CHECK(runner->isVerbose());	
}

TEST(CommandLineTestRunner, repeatSet)
{
	int argc = 2;
	char* argv[] = {"tests.exe", "-r3"};
	CHECK(runner->parseArguments(argc, argv));
	LONGS_EQUAL(3, runner->getRepeatCount());	
}

TEST(CommandLineTestRunner, repeatSetDifferentParameter)
{
	int argc = 3;
	char* argv[] = {"tests.exe", "-r", "4"};
	CHECK(runner->parseArguments(argc, argv));
	LONGS_EQUAL(4, runner->getRepeatCount());	
}

TEST(CommandLineTestRunner, repeatSetDefaultsToTwo)
{
	int argc = 2;
	char* argv[] = {"tests.exe", "-r"};
	CHECK(runner->parseArguments(argc, argv));
	LONGS_EQUAL(2, runner->getRepeatCount());	
}

TEST(CommandLineTestRunner, setGroupFilter)
{
	int argc = 3;
	char* argv[] = {"tests.exe", "-g", "group"};
	CHECK(runner->parseArguments(argc, argv));
	STRCMP_EQUAL("group", runner->getGroupFilter().asCharString());	
}

TEST(CommandLineTestRunner, setGroupFilterSameParameter)
{
	int argc = 2;
	char* argv[] = {"tests.exe", "-ggroup"};
	CHECK(runner->parseArguments(argc, argv));
	STRCMP_EQUAL("group", runner->getGroupFilter().asCharString());	
}

TEST(CommandLineTestRunner, setNameFilter)
{
	int argc = 3;
	char* argv[] = {"tests.exe", "-n", "name"};
	CHECK(runner->parseArguments(argc, argv));
	STRCMP_EQUAL("name", runner->getNameFilter().asCharString());	
}

TEST(CommandLineTestRunner, setNameFilterSameParameter)
{
	int argc = 2;
	char* argv[] = {"tests.exe", "-nname"};
	CHECK(runner->parseArguments(argc, argv));
	STRCMP_EQUAL("name", runner->getNameFilter().asCharString());	
}

TEST(CommandLineTestRunner, setNormalOutput)
{
	int argc = 2;
	char* argv[] = {"tests.exe", "-onormal"};
	CHECK(runner->parseArguments(argc, argv));
	LONGS_EQUAL(CommandLineTestRunner::OUTPUT_NORMAL, runner->getOutputType());	
}

TEST(CommandLineTestRunner, setNormalOutputDifferentParameter)
{
	int argc = 3;
	char* argv[] = {"tests.exe", "-o", "normal"};
	CHECK(runner->parseArguments(argc, argv));
	LONGS_EQUAL(CommandLineTestRunner::OUTPUT_NORMAL, runner->getOutputType());	
}

TEST(CommandLineTestRunner, setJUnitOutputDifferentParameter)
{
	int argc = 3;
	char* argv[] = {"tests.exe", "-o", "junit"};
	CHECK(runner->parseArguments(argc, argv));
	LONGS_EQUAL(CommandLineTestRunner::OUTPUT_JUNIT, runner->getOutputType());	
}

TEST(CommandLineTestRunner, setOutputToGarbage)
{
	int argc = 3;
	char* argv[] = {"tests.exe", "-o", "garbage"};
	CHECK(!runner->parseArguments(argc, argv));
}

TEST(CommandLineTestRunner, weirdParamatersPrintsUsageAndReturnsFalse)
{
	int argc = 2;
	char* argv[] = {"tests.exe", "-SomethingWeird"};
	CHECK(!runner->parseArguments(argc, argv));
	GenericTestFixture::assertPrintContains(output, "usage [-v] [-r#] [-g groupName] [-n testName] [-o{normal, junit}]\n");
}


