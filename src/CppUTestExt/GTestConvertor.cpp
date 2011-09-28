/*
 * Copyright (c) 2011, Michael Feathers, James Grenning and Bas Vodde
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

#ifdef CPPUTEST_USE_REAL_GTEST

#undef new

/* Enormous hack!
 *
 * This sucks enormously. We need to do two things in GTest that seem to not be possible without
 * this hack. Hopefully there is *another way*.
 *
 * We need to access the factory in the TestInfo in order to be able to create tests. The factory
 * is private and there seems to be no way to access it...
 *
 * We need to be able to call the Test SetUp and TearDown methods, but they are protected for
 * some reason. We can't subclass either as the tests are created with the TEST macro.
 *
 * If anyone knows how to get the above things done *without* these ugly #defines, let me know!
 *
 */

#define private public
#define protected public

#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest-death-test.h"

#ifdef ADD_FAILURE_AT
#define GTEST_VERSION_GTEST_1_6
#else
#define GTEST_VERSION_GTEST_1_5
#endif

#ifdef GTEST_VERSION_GTEST_1_5
/*
 * Old GTest has the factory_ even more hidden making it even harder to create the darn test.
 * Why do they have to hide creating tests so much!
 */
#define GTEST_IMPLEMENTATION_ 1
#include "src/gtest-internal-inl.h"
#endif


#include "CppUTestExt/GTestConvertor.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestFailure.h"
#include "CppUTest/TestResult.h"

/* Left a global to avoid a header dependency to gtest.h */

class GTestDummyResultReporter : public ::testing::ScopedFakeTestPartResultReporter
{
public:
	GTestDummyResultReporter () : ::testing::ScopedFakeTestPartResultReporter(INTERCEPT_ALL_THREADS, NULL) {}
	virtual void ReportTestPartResult(const ::testing::TestPartResult& /*result*/) {}
};

class GTestResultReporter : public ::testing::ScopedFakeTestPartResultReporter
{
public:
	GTestResultReporter () : ::testing::ScopedFakeTestPartResultReporter(INTERCEPT_ALL_THREADS, NULL) {}

	virtual void ReportTestPartResult(const ::testing::TestPartResult& result)
	{
		FailFailure failure(Utest::getCurrent(), result.file_name(), result.line_number(), result.message());
		Utest::getCurrent()->getTestResult()->addFailure(failure);
		throw GTestException();
	}
};

GTest::GTest(const ::testing::TestInfo* testinfo, GTest* next) : testinfo_(testinfo), next_(next)
{
	setGroupName(testinfo->test_case_name());
	setTestName(testinfo->name());
}

void GTest::setup()
{
#ifdef GTEST_VERSION_GTEST_1_5
	test_ = testinfo_->impl()->factory_->CreateTest();
#else
	test_ = testinfo_->factory_->CreateTest();
#endif
	try {
		test_->SetUp();
	}
	catch (GTestException& ex)
	{
	}
}

void GTest::teardown()
{
	try {
		test_->TearDown();
	}
	catch (GTestException& ex)
	{
	}
	delete test_;

	/* These are reset in String which allocates a string and makes the memory leak detector
	 * unhappy. So therefore reset them to NULL.
	 */

	::testing::GTEST_FLAG(color) = NULL;
	::testing::GTEST_FLAG(filter) = NULL;
	::testing::GTEST_FLAG(output) = NULL;
	::testing::GTEST_FLAG(death_test_style) = NULL;
	::testing::internal::GTEST_FLAG(internal_run_death_test) = NULL;
#ifndef GTEST_VERSION_GTEST_1_5
	::testing::GTEST_FLAG(stream_result_to) = NULL;
#endif
}

void GTest::testBody()
{
	try {
		test_->TestBody();
	}
	catch (GTestException& ex)
	{
	}
}

GTest* GTest::nextGTest()
{
	return next_;
}

void GTestConvertor::simulateGTestFailureToPreAllocateAllTheThreadLocalData()
{
	GTestDummyResultReporter *dummyReporter = new GTestDummyResultReporter();
	ASSERT_TRUE(false);
	delete dummyReporter;
}

GTestConvertor::GTestConvertor(bool shouldSimulateFailureAtCreationToAllocateThreadLocalData) : first_(NULL)
{
	if (shouldSimulateFailureAtCreationToAllocateThreadLocalData)
		simulateGTestFailureToPreAllocateAllTheThreadLocalData();
	reporter_ = new GTestResultReporter();
}

GTestConvertor::~GTestConvertor()
{
	delete reporter_;

	while (first_) {
		GTest* next = first_->nextGTest();
		delete first_;
		first_ = next;
	}
}


void GTestConvertor::addNewTestCaseForTestInfo(const ::testing::TestInfo* testinfo)
{
	first_ = new GTest(testinfo, first_);
	TestRegistry::getCurrentRegistry()->addTest(first_);
}

void GTestConvertor::addAllTestsFromTestCaseToTestRegistry(const ::testing::TestCase* testcase)
{
	int currentTestCount = 0;
	const ::testing::TestInfo* currentTest = testcase->GetTestInfo(currentTestCount);
	while (currentTest) {
		addNewTestCaseForTestInfo(currentTest);
		currentTestCount++;
		currentTest = testcase->GetTestInfo(currentTestCount);
	}
}

void GTestConvertor::addAllGTestToTestRegistry()
{
	::testing::UnitTest* unitTests = ::testing::UnitTest::GetInstance();

	int currentUnitTestCount = 0;
	const ::testing::TestCase* currentTestCase = unitTests->GetTestCase(currentUnitTestCount);
	while (currentTestCase) {
		addAllTestsFromTestCaseToTestRegistry(currentTestCase);
		currentUnitTestCount++;
		currentTestCase = unitTests->GetTestCase(currentUnitTestCount);
	}
}
#else

int totallyDummySymbolToNotCauseAnyLinkerWarningsAboutEmptyCompilationUnits = 0;

#endif
