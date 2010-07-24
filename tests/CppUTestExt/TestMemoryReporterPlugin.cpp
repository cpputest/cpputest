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
#include "CppUTest/TestOutput.h"
#include "CppUTestExt/MemoryReporterPlugin.h"

TEST_GROUP(MemoryReporterPlugin)
{
	MemoryReporterPlugin* reporter;
	StringBufferTestOutput output;

	TestResult* result;
	Utest* test;

	void setup()
	{
		result = new TestResult(output);
		test = new Utest("groupname", "testname", "filename", 1);
		reporter = new MemoryReporterPlugin;
		reporter->setReporting(MEMORY_REPORT_TYPE_NORMAL);
	}
	void teardown()
	{
		delete reporter;
		delete result;
		delete test;
	}
};

TEST(MemoryReporterPlugin, offReportsNothing)
{
	reporter->setReporting(MEMORY_REPORT_TYPE_OFF);
	reporter->preTestAction(*test, *result);
	char* memory = new char;
	delete memory;
	reporter->postTestAction(*test, *result);
	STRCMP_EQUAL("", output.getOutput().asCharString());
}

TEST(MemoryReporterPlugin, meaninglessArgumentsAreIgnored)
{
	reporter->setReporting(MEMORY_REPORT_TYPE_OFF);

	const char *cmd_line[] = {"-nothing", "-pnotmemoryreport=normal", "alsomeaningless", "-pmemoryreportnonsensebutnotus"};
	CHECK(reporter->parseArguments(3, cmd_line, 1) == false);
	LONGS_EQUAL(MEMORY_REPORT_TYPE_OFF, reporter->getReporting());
}

TEST(MemoryReporterPlugin, wrongArgumentsAreAcceptedButIgnored)
{
	reporter->setReporting(MEMORY_REPORT_TYPE_OFF);

	const char *cmd_line[] = {"-nothing", "-pmemoryreport=makesnosense", "alsomeaningless", "-pmemoryreportnonsensebutnotus"};
	CHECK(reporter->parseArguments(3, cmd_line, 1));
	LONGS_EQUAL(MEMORY_REPORT_TYPE_OFF, reporter->getReporting());
}

TEST(MemoryReporterPlugin, commandLineParameterTurnsOnNormalLogging)
{
	reporter->setReporting(MEMORY_REPORT_TYPE_OFF);

	const char *cmd_line[] = {"-nothing", "-pmemoryreport=normal", "alsomeaningless" };
	CHECK(reporter->parseArguments(3, cmd_line, 1));
	LONGS_EQUAL(MEMORY_REPORT_TYPE_NORMAL, reporter->getReporting());
}

TEST(MemoryReporterPlugin, commandLineParameterTurnsOnCodeLogging)
{
	const char *cmd_line[] = {"-nothing", "-pmemoryreport=code", "alsomeaningless" };
	CHECK(reporter->parseArguments(3, cmd_line, 1));
	LONGS_EQUAL(MEMORY_REPORT_TYPE_CODE, reporter->getReporting());
}

TEST(MemoryReporterPlugin, preTestActionReportsTest)
{
	reporter->preTestAction(*test, *result);
	STRCMP_EQUAL("TEST(groupname, testname)\n", output.getOutput().asCharString());
}

TEST(MemoryReporterPlugin, postTestActionReportsTest)
{
	reporter->postTestAction(*test, *result);
	STRCMP_EQUAL("ENDTEST(groupname, testname)\n", output.getOutput().asCharString());
}

TEST(MemoryReporterPlugin, newAllocationsAreReportedTest)
{
	reporter->preTestAction(*test, *result);
	char* memory = new char;
	STRCMP_CONTAINS("Allocation using new", output.getOutput().asCharString());
	delete memory;
}

TEST(MemoryReporterPlugin, whenUsingOnlyMallocAllocatorNoOtherOfTheAllocatorsAreUsed)
{
	reporter->preTestAction(*test, *result);
	void* memory = malloc(100);
	CHECK(!output.getOutput().contains("new"));
	free(memory);
}

TEST(MemoryReporterPlugin, newAllocationsAreReportedTestUseCodeMode)
{
	reporter->setReporting(MEMORY_REPORT_TYPE_CODE);
	reporter->preTestAction(*test, *result);
	char* memory = new char;
	STRCMP_CONTAINS("new char", output.getOutput().asCharString());
	delete memory;
}

TEST(MemoryReporterPlugin, newArrayAllocationsAreReportedTest)
{
	reporter->preTestAction(*test, *result);
	char* memory = new char [100];
	STRCMP_CONTAINS("Allocation using new []", output.getOutput().asCharString());
	delete [] memory;
}

TEST(MemoryReporterPlugin, mallocAllocationsAreReportedTest)
{
	reporter->preTestAction(*test, *result);
	void* memory = malloc(100);
	STRCMP_CONTAINS("Allocation using malloc", output.getOutput().asCharString());
	free(memory);
}

TEST(MemoryReporterPlugin, preActionReplacesAllocator)
{
	MemoryLeakAllocator* allocator = MemoryLeakAllocator::getCurrentMallocAllocator();
	reporter->preTestAction(*test, *result);
	CHECK(allocator != MemoryLeakAllocator::getCurrentMallocAllocator());
}

TEST(MemoryReporterPlugin, postActioReturnsAllocator)
{
	MemoryLeakAllocator* allocator = MemoryLeakAllocator::getCurrentMallocAllocator();
	reporter->preTestAction(*test, *result);
	reporter->postTestAction(*test, *result);
	CHECK(allocator == MemoryLeakAllocator::getCurrentMallocAllocator());
}

