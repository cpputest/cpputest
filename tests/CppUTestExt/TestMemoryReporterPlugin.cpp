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
#include "CppUTestExt/MemoryReportFormatter.h"
#include "CppUTestExt/MockSupport.h"

static MockSupport mock;

class TemporaryDefaultNewAllocator
{
	MemoryLeakAllocator* newAllocator;
public:
	TemporaryDefaultNewAllocator()
	{
		newAllocator = MemoryLeakAllocator::getCurrentNewAllocator();
		MemoryLeakAllocator::setCurrentNewAllocator(StandardNewAllocator::defaultAllocator());
	}
	~TemporaryDefaultNewAllocator()
	{
		MemoryLeakAllocator::setCurrentNewAllocator(newAllocator);
	}
};

class MockMemoryReportFormatter : public MemoryReportFormatter
{
public:
	virtual void report_test_start(TestResult* result, Utest& test)
	{
		TemporaryDefaultNewAllocator tempAlloc;
		mock.actualCall("report_test_start")->withParameter("result", result)->withParameter("test", &test);
	}

	virtual void report_test_end(TestResult* result, Utest& test)
	{
		TemporaryDefaultNewAllocator tempAlloc;
		mock.actualCall("report_test_end")->withParameter("result", result)->withParameter("test", &test);;
	}

	virtual void report_alloc_memory(TestResult* , MemoryLeakAllocator* , size_t , char* , const char* , int )
	{
		TemporaryDefaultNewAllocator tempAlloc;
		mock.actualCall("report_alloc_memory");
	}

	virtual void report_free_memory(TestResult* , MemoryLeakAllocator* , char* , const char* , int )
	{
		TemporaryDefaultNewAllocator tempAlloc;
		mock.actualCall("report_free_memory");
	}
};

static MockMemoryReportFormatter formatterForPluginTest;

class MemoryReporterPluginUnderTest : public MemoryReporterPlugin
{
public:
	MockMemoryReportFormatter mockFormatter;
	SimpleString type_;
    MemoryReportFormatter* createMemoryFormatter(const SimpleString& type)
    {
    	type_ = type;
    	return new MockMemoryReportFormatter;
    }
};

TEST_GROUP(MemoryReporterPlugin)
{
	MemoryReporterPluginUnderTest* reporter;
	StringBufferTestOutput output;

	TestResult* result;
	Utest* test;

	void setReportingToNormal()
	{
		const char *cmd_line[] = {"-pmemoryreport=normal"};
		reporter->parseArguments(1, cmd_line, 0);
	}

	void setReportingToCode()
	{
		const char *cmd_line[] = {"-pmemoryreport=code"};
		reporter->parseArguments(1, cmd_line, 0);
	}

	void setup()
	{
		result = new TestResult(output);
		test = new Utest("groupname", "testname", "filename", 1);
		reporter = new MemoryReporterPluginUnderTest;
//		mock.crashOnFailure();
	}
	void teardown()
	{
		delete reporter;
		delete test;
		delete result;
		mock.checkExpectations();
	}
};

TEST(MemoryReporterPlugin, offReportsNothing)
{
	reporter->preTestAction(*test, *result);
	char* memory = new char;
	delete memory;
	reporter->postTestAction(*test, *result);
	STRCMP_EQUAL("", reporter->type_.asCharString());
}

TEST(MemoryReporterPlugin, meaninglessArgumentsAreIgnored)
{
	const char *cmd_line[] = {"-nothing", "-pnotmemoryreport=normal", "alsomeaningless", "-pmemoryreportnonsensebutnotus"};
	CHECK(reporter->parseArguments(3, cmd_line, 1) == false);
	STRCMP_EQUAL("", reporter->type_.asCharString());
}

TEST(MemoryReporterPlugin, wrongArgumentsAreAcceptedButIgnored)
{
	const char *cmd_line[] = {"-nothing", "-pmemoryreport=makesnosense", "alsomeaningless", "-pmemoryreportnonsensebutnotus"};
	CHECK(reporter->parseArguments(3, cmd_line, 1));
	STRCMP_EQUAL("makesnosense", reporter->type_.asCharString());
}

TEST(MemoryReporterPlugin, commandLineParameterTurnsOnNormalLogging)
{
	const char *cmd_line[] = {"-nothing", "-pmemoryreport=normal", "alsomeaningless" };
	CHECK(reporter->parseArguments(3, cmd_line, 1));
	STRCMP_EQUAL("normal", reporter->type_.asCharString());
}

TEST(MemoryReporterPlugin, commandLineParameterTurnsOnCodeLogging)
{
	const char *cmd_line[] = {"-nothing", "-pmemoryreport=code", "alsomeaningless" };
	CHECK(reporter->parseArguments(3, cmd_line, 1));
	STRCMP_EQUAL("code", reporter->type_.asCharString());
}

TEST(MemoryReporterPlugin, preTestActionReportsTest)
{
	setReportingToNormal();

	mock.expectOneCall("report_test_start")->withParameter("result", result)->withParameter("test", test);
	reporter->preTestAction(*test, *result);
}

TEST(MemoryReporterPlugin, postTestActionReportsTest)
{
	mock.expectOneCall("report_test_end")->withParameter("result", result)->withParameter("test", test);;

	setReportingToNormal();
	reporter->postTestAction(*test, *result);
}

TEST(MemoryReporterPlugin, newAllocationsAreReportedTest)
{
	mock.expectOneCall("report_alloc_memory");
	mock.ignoreOtherCalls();

	setReportingToNormal();
	reporter->preTestAction(*test, *result);
	char* memory = new char;
	delete memory;
}

TEST(MemoryReporterPlugin, whenUsingOnlyMallocAllocatorNoOtherOfTheAllocatorsAreUsed)
{
	mock.expectOneCall("report_test_start")->withParameter("result", result)->withParameter("test", test);;
	mock.expectOneCall("report_alloc_memory");
	mock.expectOneCall("report_free_memory");

	setReportingToNormal();
	reporter->preTestAction(*test, *result);
	void* memory = malloc(100);
	CHECK(!output.getOutput().contains("new"));
	free(memory);
}

TEST(MemoryReporterPlugin, newArrayAllocationsAreReportedTest)
{
	mock.expectOneCall("report_alloc_memory");
	mock.ignoreOtherCalls();

	setReportingToNormal();
	reporter->preTestAction(*test, *result);
	char* memory = new char [100];
	delete [] memory;
}

TEST(MemoryReporterPlugin, mallocAllocationsAreReportedTest)
{
	mock.expectOneCall("report_alloc_memory");
	mock.ignoreOtherCalls();

	setReportingToNormal();
	reporter->preTestAction(*test, *result);
	void* memory = malloc(100);
	free(memory);
}

TEST(MemoryReporterPlugin, preActionReplacesAllocators)
{
	mock.ignoreOtherCalls();

	setReportingToNormal();
	MemoryLeakAllocator* allocator = MemoryLeakAllocator::getCurrentMallocAllocator();
	reporter->preTestAction(*test, *result);
	CHECK(allocator != MemoryLeakAllocator::getCurrentMallocAllocator());
}

TEST(MemoryReporterPlugin, postActionRestoresAllocators)
{
	mock.ignoreOtherCalls();

	setReportingToNormal();
	MemoryLeakAllocator* allocator = MemoryLeakAllocator::getCurrentMallocAllocator();
	reporter->preTestAction(*test, *result);
	reporter->postTestAction(*test, *result);
	CHECK(allocator == MemoryLeakAllocator::getCurrentMallocAllocator());
}
