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

#include "CppUTestExt/GTestConvertor.h"


Utest* GTestShell::createTest()
{
	return new GTestUTest(testinfo_, flags_);
};

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
		GTestShell* next = first_->nextGTest();
		delete first_;
		first_ = next;
	}
}

void GTestConvertor::addNewTestCaseForTestInfo(::testing::TestInfo* testinfo)
{
	first_ = new GTestShell(testinfo, first_, &flags_);
	TestRegistry::getCurrentRegistry()->addTest(first_);
}

void GTestConvertor::addAllTestsFromTestCaseToTestRegistry(::testing::TestCase* testcase)
{
	int currentTestCount = 0;
	::testing::TestInfo* currentTest = (::testing::TestInfo*) testcase->GetTestInfo(currentTestCount);
	while (currentTest) {
		addNewTestCaseForTestInfo(currentTest);
		currentTestCount++;
		currentTest = (::testing::TestInfo*) testcase->GetTestInfo(currentTestCount);
	}
}

void GTestConvertor::createDummyInSequenceToAndFailureReporterAvoidMemoryLeakInGMock()
{
#ifdef CPPUTEST_USE_REAL_GMOCK
	::testing::InSequence seq;
	::testing::internal::GetFailureReporter();
#endif
}

void GTestConvertor::addAllGTestToTestRegistry()
{
	createDummyInSequenceToAndFailureReporterAvoidMemoryLeakInGMock();
	flags_.storeValuesOfGTestFLags();

#ifdef CPPUTEST_USE_REAL_GMOCK
	int argc = 2;
	const char * argv[] = {"NameOfTheProgram", "--gmock_catch_leaked_mocks=0"};
	::testing::InitGoogleMock(&argc, (char**) argv);
#endif

	::testing::UnitTest* unitTests = ::testing::UnitTest::GetInstance();

	int currentUnitTestCount = 0;
	::testing::TestCase* currentTestCase = (::testing::TestCase*) unitTests->GetTestCase(currentUnitTestCount);
	while (currentTestCase) {
		addAllTestsFromTestCaseToTestRegistry(currentTestCase);
		currentUnitTestCount++;
		currentTestCase = (::testing::TestCase*) unitTests->GetTestCase(currentUnitTestCount);
	}
}
#else

extern int totallyDummySymbolToNotCauseAnyLinkerWarningsAboutEmptyCompilationUnits;
int totallyDummySymbolToNotCauseAnyLinkerWarningsAboutEmptyCompilationUnits = 0;

#endif
