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

#ifndef GTESTCONVERTOR_H_
#define GTESTCONVERTOR_H_

#include "CppUTest/Utest.h"

#ifdef GTEST__H_
#error "Please include this file before you include any other GTest files"
#endif

/*
 * Usage:
 *
 * This file must only be included in the main. The whole implementation is inline so that this can
 * be compiled on usage and not on CppUTest compile-time. This avoids a hard dependency with CppUTest
 * and with GTest
 *
 * Add the following lines to your main:
 *
 *      GTestConvertor convertor;
 *      convertor.addAllGTestToTestRegistry();
 *
 *
 */

class GTestResultReporter;
class GTestFlagsThatAllocateMemory;

namespace testing {
    class TestInfo;
    class TestCase;
    class Test;
}

class GTestShell : public UtestShell
{
    ::testing::TestInfo* testinfo_;
    GTestShell* next_;
    GTestFlagsThatAllocateMemory* flags_;
public:
    GTestShell(::testing::TestInfo* testinfo, GTestShell* next, GTestFlagsThatAllocateMemory* flags);

    virtual Utest* createTest() _override;

    GTestShell* nextGTest()
    {
        return next_;
    }
};

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

#include "CppUTestExt/GTest.h"
#include "CppUTestExt/GMock.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest-death-test.h"
/*
 * We really need some of its internals as they don't have a public interface.
 *
 */
#define GTEST_IMPLEMENTATION_ 1
#include "src/gtest-internal-inl.h"

#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestFailure.h"
#include "CppUTest/TestResult.h"


#ifdef GTEST_VERSION_GTEST_1_7
#define GTEST_STRING std::string
#define GTEST_NO_STRING_VALUE ""
#else
#define GTEST_STRING ::testing::internal::String
#define GTEST_NO_STRING_VALUE NULL
#endif

/* Store some of the flags as we'll need to reset them each test to avoid leaking memory */

class GTestFlagsThatAllocateMemory
{
public:
    void storeValuesOfGTestFLags()
    {
        GTestFlagcolor = ::testing::GTEST_FLAG(color);
        GTestFlagfilter = ::testing::GTEST_FLAG(filter);
        GTestFlagoutput = ::testing::GTEST_FLAG(output);
        GTestFlagdeath_test_style = ::testing::GTEST_FLAG(death_test_style);
        GTestFlaginternal_run_death_test = ::testing::internal::GTEST_FLAG(internal_run_death_test);
        #ifndef GTEST_VERSION_GTEST_1_5
        GTestFlagstream_result_to = ::testing::GTEST_FLAG(stream_result_to);
        #endif
    }

    void resetValuesOfGTestFlags()
    {
        ::testing::GTEST_FLAG(color) = GTestFlagcolor;
        ::testing::GTEST_FLAG(filter) = GTestFlagfilter;
        ::testing::GTEST_FLAG(output) = GTestFlagoutput;
        ::testing::GTEST_FLAG(death_test_style) = GTestFlagdeath_test_style;
        ::testing::internal::GTEST_FLAG(internal_run_death_test) = GTestFlaginternal_run_death_test;
        #ifndef GTEST_VERSION_GTEST_1_5
        ::testing::GTEST_FLAG(stream_result_to) = GTestFlagstream_result_to;
        #endif
    }

    void setGTestFLagValuesToNULLToAvoidMemoryLeaks()
    {
    #ifndef GTEST_VERSION_GTEST_1_7
        ::testing::GTEST_FLAG(color) = GTEST_NO_STRING_VALUE;
        ::testing::GTEST_FLAG(filter) = GTEST_NO_STRING_VALUE;
        ::testing::GTEST_FLAG(output) = GTEST_NO_STRING_VALUE;
        ::testing::GTEST_FLAG(death_test_style) = GTEST_NO_STRING_VALUE;
        ::testing::internal::GTEST_FLAG(internal_run_death_test) = GTEST_NO_STRING_VALUE;
        #ifndef GTEST_VERSION_GTEST_1_5
        ::testing::GTEST_FLAG(stream_result_to) = GTEST_NO_STRING_VALUE;
        #endif
    #endif
    }

private:
    GTEST_STRING GTestFlagcolor;
    GTEST_STRING GTestFlagfilter;
    GTEST_STRING GTestFlagoutput;
    GTEST_STRING GTestFlagdeath_test_style;
    GTEST_STRING GTestFlaginternal_run_death_test;
    #ifndef GTEST_VERSION_GTEST_1_5
    GTEST_STRING GTestFlagstream_result_to;
    #endif
};

class GTestConvertor
{
public:
    GTestConvertor(bool shouldSimulateFailureAtCreationToAllocateThreadLocalData = true);
    virtual ~GTestConvertor();

    virtual void addAllGTestToTestRegistry();
protected:
    virtual void simulateGTestFailureToPreAllocateAllTheThreadLocalData();

    virtual void addNewTestCaseForTestInfo(::testing::TestInfo* testinfo);
    virtual void addAllTestsFromTestCaseToTestRegistry(::testing::TestCase* testcase);

    virtual void createDummyInSequenceToAndFailureReporterAvoidMemoryLeakInGMock();
private:
    GTestResultReporter* reporter_;
    GTestShell* first_;
    GTestFlagsThatAllocateMemory flags_;
};

class GTestDummyResultReporter : public ::testing::ScopedFakeTestPartResultReporter
{
public:
    GTestDummyResultReporter () : ::testing::ScopedFakeTestPartResultReporter(INTERCEPT_ALL_THREADS, NULL) {}
    virtual void ReportTestPartResult(const ::testing::TestPartResult& /*result*/) {}
};

class GMockTestTerminator : public TestTerminator
{
public:
    GMockTestTerminator(const ::testing::TestPartResult& result) : result_(result)
    {
    }

    virtual void exitCurrentTest() const
    {
        /*
         * When using GMock, it throws an exception from the destructor leaving
         * the system in an unstable state.
         * Therefore, when the test fails because of failed gmock expectation
         * then don't throw the exception, but let it return. Usually this should
         * already be at the end of the test, so it doesn't matter much
         */


        /*
         * TODO: We probably want this check here, however the tests fail when putting it there. Also, we'll need to
         * check how to get all the gTest tests to run within CppUTest. At the moment, the 'death tests' seem to fail
         * still.
         *
         * if (result_.type() == ::testing::TestPartResult::kFatalFailure) {
         */
            if (!SimpleString(result_.message()).contains("Actual: never called") &&
                    !SimpleString(result_.message()).contains("Actual function call count doesn't match"))
                throw CppUTestFailedException();

    }
    virtual ~GMockTestTerminator()
    {
    }
private:
    const ::testing::TestPartResult& result_;
};


class GTestResultReporter : public ::testing::ScopedFakeTestPartResultReporter
{
public:
    GTestResultReporter () : ::testing::ScopedFakeTestPartResultReporter(INTERCEPT_ALL_THREADS, NULL) {}

    virtual void ReportTestPartResult(const ::testing::TestPartResult& result)
    {
        FailFailure failure(UtestShell::getCurrent(), result.file_name(), result.line_number(), result.message());
        UtestShell::getCurrent()->failWith(failure, GMockTestTerminator(result));
    }
};

inline GTestShell::GTestShell(::testing::TestInfo* testinfo, GTestShell* next, GTestFlagsThatAllocateMemory* flags) : testinfo_(testinfo), next_(next), flags_(flags)
{
    setGroupName(testinfo->test_case_name());
    setTestName(testinfo->name());
}

class GTestUTest: public Utest {
public:
    GTestUTest(::testing::TestInfo* testinfo, GTestFlagsThatAllocateMemory* flags) : testinfo_(testinfo), test_(NULL), flags_(flags)
    {

    }

    void testBody()
    {
        try {
            test_->TestBody();
        }
        catch (CppUTestFailedException& ex)
        {
        }
    }

    void setup()
    {
        flags_->resetValuesOfGTestFlags();

        #ifdef GTEST_VERSION_GTEST_1_5
        test_ = testinfo_->impl()->factory_->CreateTest();
    #else
        test_ = testinfo_->factory_->CreateTest();
    #endif

        ::testing::UnitTest::GetInstance()->impl()->set_current_test_info(testinfo_);
        try {
            test_->SetUp();
        }
        catch (CppUTestFailedException& ex)
        {
        }
    }

    void teardown()
    {
        try {
            test_->TearDown();
        }
        catch (CppUTestFailedException& ex)
        {
        }
        ::testing::UnitTest::GetInstance()->impl()->set_current_test_info(NULL);
        delete test_;

        flags_->setGTestFLagValuesToNULLToAvoidMemoryLeaks();
        ::testing::internal::DeathTest::set_last_death_test_message(GTEST_NO_STRING_VALUE);
    }

private:
    ::testing::Test* test_;
    ::testing::TestInfo* testinfo_;
    GTestFlagsThatAllocateMemory* flags_;
};

inline Utest* GTestShell::createTest()
{
    return new GTestUTest(testinfo_, flags_);
};

inline void GTestConvertor::simulateGTestFailureToPreAllocateAllTheThreadLocalData()
{
    GTestDummyResultReporter *dummyReporter = new GTestDummyResultReporter();
    ASSERT_TRUE(false);
    delete dummyReporter;
}

inline GTestConvertor::GTestConvertor(bool shouldSimulateFailureAtCreationToAllocateThreadLocalData) : first_(NULL)
{
    if (shouldSimulateFailureAtCreationToAllocateThreadLocalData)
        simulateGTestFailureToPreAllocateAllTheThreadLocalData();
    reporter_ = new GTestResultReporter();
}

inline GTestConvertor::~GTestConvertor()
{
    delete reporter_;

    while (first_) {
        GTestShell* next = first_->nextGTest();
        delete first_;
        first_ = next;
    }
}

inline void GTestConvertor::addNewTestCaseForTestInfo(::testing::TestInfo* testinfo)
{
    first_ = new GTestShell(testinfo, first_, &flags_);
    TestRegistry::getCurrentRegistry()->addTest(first_);
}

inline void GTestConvertor::addAllTestsFromTestCaseToTestRegistry(::testing::TestCase* testcase)
{
    int currentTestCount = 0;
    ::testing::TestInfo* currentTest = (::testing::TestInfo*) testcase->GetTestInfo(currentTestCount);
    while (currentTest) {
        addNewTestCaseForTestInfo(currentTest);
        currentTestCount++;
        currentTest = (::testing::TestInfo*) testcase->GetTestInfo(currentTestCount);
    }
}

inline void GTestConvertor::createDummyInSequenceToAndFailureReporterAvoidMemoryLeakInGMock()
{
    ::testing::InSequence seq;
    ::testing::internal::GetFailureReporter();
}

inline void GTestConvertor::addAllGTestToTestRegistry()
{
    createDummyInSequenceToAndFailureReporterAvoidMemoryLeakInGMock();
    flags_.storeValuesOfGTestFLags();

    int argc = 2;
    const char * argv[] = {"NameOfTheProgram", "--gmock_catch_leaked_mocks=0"};
    ::testing::InitGoogleMock(&argc, (char**) argv);

    ::testing::UnitTest* unitTests = ::testing::UnitTest::GetInstance();

    int currentUnitTestCount = 0;
    ::testing::TestCase* currentTestCase = (::testing::TestCase*) unitTests->GetTestCase(currentUnitTestCount);
    while (currentTestCase) {
        addAllTestsFromTestCaseToTestRegistry(currentTestCase);
        currentUnitTestCount++;
        currentTestCase = (::testing::TestCase*) unitTests->GetTestCase(currentUnitTestCount);
    }
}


#endif
