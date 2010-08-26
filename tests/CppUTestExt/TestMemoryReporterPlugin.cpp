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
#include "CppUTestExt/MockNamedValue.h"

static MemoryLeakAllocator* previousNewAllocator;

class TemporaryDefaultNewAllocator
{
	MemoryLeakAllocator* newAllocator;
public:
	TemporaryDefaultNewAllocator(MemoryLeakAllocator* oldAllocator)
	{
		newAllocator = MemoryLeakAllocator::getCurrentNewAllocator();
		MemoryLeakAllocator::setCurrentNewAllocator(oldAllocator);
	}
	~TemporaryDefaultNewAllocator()
	{
		MemoryLeakAllocator::setCurrentNewAllocator(newAllocator);
	}
};

class MockMemoryReportFormatter : public MemoryReportFormatter
{
public:
	virtual void report_testgroup_start(TestResult* result, Utest& test)
	{
		TemporaryDefaultNewAllocator tempAlloc(previousNewAllocator);
		mock("formatter").actualCall("report_testgroup_start").withParameter("result", result).withParameter("test", &test);
	}

	virtual void report_testgroup_end(TestResult* result, Utest& test)
	{
		TemporaryDefaultNewAllocator tempAlloc(previousNewAllocator);
		mock("formatter").actualCall("report_testgroup_end").withParameter("result", result).withParameter("test", &test);
	}

	virtual void report_test_start(TestResult* result, Utest& test)
	{
		TemporaryDefaultNewAllocator tempAlloc(previousNewAllocator);
		mock("formatter").actualCall("report_test_start").withParameter("result", result).withParameter("test", &test);
	}

	virtual void report_test_end(TestResult* result, Utest& test)
	{
		TemporaryDefaultNewAllocator tempAlloc(previousNewAllocator);
		mock("formatter").actualCall("report_test_end").withParameter("result", result).withParameter("test", &test);
	}

	virtual void report_alloc_memory(TestResult* result, MemoryLeakAllocator* allocator, size_t, char* , const char* , int )
	{
		TemporaryDefaultNewAllocator tempAlloc(previousNewAllocator);
		mock("formatter").actualCall("report_alloc_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", allocator);
	}

	virtual void report_free_memory(TestResult* result, MemoryLeakAllocator* allocator, char* , const char* , int )
	{
		TemporaryDefaultNewAllocator tempAlloc(previousNewAllocator);
		mock("formatter").actualCall("report_free_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", allocator);
	}
};

static MockMemoryReportFormatter formatterForPluginTest;

class MemoryReporterPluginUnderTest : public MemoryReporterPlugin
{
public:
    MemoryReportFormatter* createMemoryFormatter(const SimpleString& type)
    {
    	mock("reporter").actualCall("createMemoryFormatter").onObject(this).withParameter("type", type.asCharString());
    	return new MockMemoryReportFormatter;
    }
};

class MemoryLeakAllocatorComparator : public MockNamedValueComparator
{
public:
	bool isEqual(void* object1, void* object2)
	{
		return ((MemoryLeakAllocator*)object1)->name() == ((MemoryLeakAllocator*)object2)->name();
	}
	SimpleString valueToString(void* object)
	{
		return ((MemoryLeakAllocator*)object)->name();
	}

};

TEST_GROUP(MemoryReporterPlugin)
{
	MemoryReporterPluginUnderTest* reporter;
	StringBufferTestOutput output;
	MemoryLeakAllocatorComparator memLeakAllocatorComparator;

	TestResult* result;
	Utest* test;

	void setup()
	{
		previousNewAllocator = MemoryLeakAllocator::getCurrentNewAllocator();
		result = new TestResult(output);
		test = new Utest("groupname", "testname", "filename", 1);
		reporter = new MemoryReporterPluginUnderTest;

		mock("formatter").installComparator("MemoryLeakAllocator", memLeakAllocatorComparator);

		mock("reporter").disable();
		const char *cmd_line[] = {"-pmemoryreport=normal"};
		reporter->parseArguments(1, cmd_line, 0);
		mock("reporter").enable();

		//		mock.crashOnFailure();
	}
	void teardown()
	{
		delete reporter;
		delete test;
		delete result;
	}
};

TEST(MemoryReporterPlugin, offReportsNothing)
{
	MemoryReporterPluginUnderTest freshReporter;
	freshReporter.preTestAction(*test, *result);
	char* memory = new char;
	delete memory;
	freshReporter.postTestAction(*test, *result);
}

TEST(MemoryReporterPlugin, meaninglessArgumentsAreIgnored)
{
	const char *cmd_line[] = {"-nothing", "-pnotmemoryreport=normal", "alsomeaningless", "-pmemoryreportnonsensebutnotus"};
	CHECK(reporter->parseArguments(3, cmd_line, 1) == false);
}

TEST(MemoryReporterPlugin, commandLineParameterTurnsOnNormalLogging)
{
	mock("reporter").expectOneCall("createMemoryFormatter").onObject(reporter).withParameter("type", "normal");

	const char *cmd_line[] = {"-nothing", "-pmemoryreport=normal", "alsomeaningless" };
	CHECK(reporter->parseArguments(3, cmd_line, 1));
}

TEST(MemoryReporterPlugin, preTestActionReportsTest)
{
	mock("formatter").expectOneCall("report_testgroup_start").withParameter("result", result).withParameter("test", test);
	mock("formatter").expectOneCall("report_test_start").withParameter("result", result).withParameter("test", test);
	reporter->preTestAction(*test, *result);
}

TEST(MemoryReporterPlugin, postTestActionReportsTest)
{
	mock("formatter").expectOneCall("report_test_end").withParameter("result", result).withParameter("test", test);;
	mock("formatter").expectOneCall("report_testgroup_end").withParameter("result", result).withParameter("test", test);;

	reporter->postTestAction(*test, *result);
}

TEST(MemoryReporterPlugin, newAllocationsAreReportedTest)
{
	mock("formatter").expectOneCall("report_alloc_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", StandardNewAllocator::defaultAllocator());
	mock("formatter").expectOneCall("report_free_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", StandardNewAllocator::defaultAllocator());
	mock("formatter").ignoreOtherCalls();

	reporter->preTestAction(*test, *result);
	char *memory = MemoryLeakAllocator::getCurrentNewAllocator()->allocMemoryLeakNode(100);
	MemoryLeakAllocator::getCurrentNewAllocator()->free_memory(memory, "unknown", 1);
}

TEST(MemoryReporterPlugin, whenUsingOnlyMallocAllocatorNoOtherOfTheAllocatorsAreUsed)
{
	mock("formatter").expectOneCall("report_test_start").withParameter("result", result).withParameter("test", test);
	mock("formatter").expectOneCall("report_alloc_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", StandardMallocAllocator::defaultAllocator());
	mock("formatter").expectOneCall("report_free_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", StandardMallocAllocator::defaultAllocator());
	mock("formatter").ignoreOtherCalls();

	reporter->preTestAction(*test, *result);
	char *memory = MemoryLeakAllocator::getCurrentMallocAllocator()->allocMemoryLeakNode(100);
	MemoryLeakAllocator::getCurrentMallocAllocator()->free_memory(memory, "unknown", 1);
}

TEST(MemoryReporterPlugin, newArrayAllocationsAreReportedTest)
{
	mock("formatter").expectOneCall("report_alloc_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", StandardNewArrayAllocator::defaultAllocator());
	mock("formatter").expectOneCall("report_free_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", StandardNewArrayAllocator::defaultAllocator());
	mock("formatter").ignoreOtherCalls();

	reporter->preTestAction(*test, *result);
	char *memory = MemoryLeakAllocator::getCurrentNewArrayAllocator()->allocMemoryLeakNode(100);
	MemoryLeakAllocator::getCurrentNewArrayAllocator()->free_memory(memory, "unknown", 1);
}

TEST(MemoryReporterPlugin, mallocAllocationsAreReportedTest)
{
	mock("formatter").expectOneCall("report_alloc_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", StandardMallocAllocator::defaultAllocator());
	mock("formatter").expectOneCall("report_free_memory").withParameter("result", result).withParameterOfType("MemoryLeakAllocator", "allocator", StandardMallocAllocator::defaultAllocator());
	mock("formatter").ignoreOtherCalls();

	reporter->preTestAction(*test, *result);
	char *memory = MemoryLeakAllocator::getCurrentMallocAllocator()->allocMemoryLeakNode(100);
	MemoryLeakAllocator::getCurrentMallocAllocator()->free_memory(memory, "unknown", 1);
}

TEST(MemoryReporterPlugin, startOfANewTestWillReportTheTestGroupStart)
{
	mock("formatter").expectOneCall("report_testgroup_start").withParameter("result", result).withParameter("test", test);
	mock("formatter").expectOneCall("report_test_start").withParameter("result", result).withParameter("test", test);
	mock("formatter").expectOneCall("report_test_end").withParameter("result", result).withParameter("test", test);
	mock("formatter").expectOneCall("report_test_start").withParameter("result", result).withParameter("test", test);
	mock("formatter").expectOneCall("report_test_end").withParameter("result", result).withParameter("test", test);
	mock("formatter").ignoreOtherCalls();

	reporter->preTestAction(*test, *result);
	reporter->postTestAction(*test, *result);
	reporter->preTestAction(*test, *result);
	reporter->postTestAction(*test, *result);
}

class UtestForMemoryReportingPlugingTest : public Utest
{
public:
	UtestForMemoryReportingPlugingTest(const char* groupname, Utest* test) : Utest(groupname, "testname", "filename", 1, test)
	{

	}
};

TEST(MemoryReporterPlugin, endOfaTestGroupWillReportSo)
{
	UtestForMemoryReportingPlugingTest fourthTest("differentGroupName", NULL);
	UtestForMemoryReportingPlugingTest thirdTest("differentGroupName", &fourthTest);
	UtestForMemoryReportingPlugingTest secondTest("groupname", &thirdTest);
	UtestForMemoryReportingPlugingTest firstTest("groupname", &secondTest);

	mock("formatter").expectOneCall("report_testgroup_end").withParameter("result", result).withParameter("test", &secondTest);
	mock("formatter").ignoreOtherCalls();

	reporter->preTestAction(firstTest, *result);
	reporter->postTestAction(firstTest, *result);
	reporter->preTestAction(secondTest, *result);
	reporter->postTestAction(secondTest, *result);
	reporter->preTestAction(thirdTest, *result);
	reporter->postTestAction(thirdTest, *result);
}

TEST(MemoryReporterPlugin, preActionReplacesAllocators)
{
	mock("formatter").ignoreOtherCalls();

	MemoryLeakAllocator* allocator = MemoryLeakAllocator::getCurrentMallocAllocator();
	reporter->preTestAction(*test, *result);
	CHECK(allocator != MemoryLeakAllocator::getCurrentMallocAllocator());
}

TEST(MemoryReporterPlugin, postActionRestoresAllocators)
{
	mock("formatter").ignoreOtherCalls();

	MemoryLeakAllocator* allocator = MemoryLeakAllocator::getCurrentMallocAllocator();
	reporter->preTestAction(*test, *result);
	reporter->postTestAction(*test, *result);
	CHECK(allocator == MemoryLeakAllocator::getCurrentMallocAllocator());
}
