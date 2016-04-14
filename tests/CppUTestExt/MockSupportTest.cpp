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
#include "CppUTest/TestTestingFixture.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockExpectedCall.h"
#include "CppUTestExt/MockFailure.h"
#include "MockFailureReporterForTest.h"

TEST_GROUP(MockSupportTest)
{
  MockExpectedCallsListForTest expectations;
  MockFailureReporterInstaller failureReporterInstaller;

  void teardown()
  {
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
  }
};

TEST(MockSupportTest, setDataForUnsignedIntegerValues)
{
    unsigned int expected_data = 7;
    mock().setData("data", expected_data);
    LONGS_EQUAL(expected_data, mock().getData("data").getUnsignedIntValue());
}

TEST(MockSupportTest, setDataForIntegerValues)
{
    int expected_data = 10;
    mock().setData("data", expected_data);
    LONGS_EQUAL(expected_data, mock().getData("data").getIntValue());
}

TEST(MockSupportTest, setDataForBooleanValues)
{
    bool expected_data = true;
    mock().setData("data", expected_data);
    CHECK_EQUAL(expected_data, mock().getData("data").getBoolValue());
}

TEST(MockSupportTest, hasDataBeenSet)
{
    CHECK(!mock().hasData("data"));
    mock().setData("data", 10);
    CHECK(mock().hasData("data"));
}

TEST(MockSupportTest, dataCanBeChanged)
{
    mock().setData("data", 10);
    mock().setData("data", 15);
    LONGS_EQUAL(15, mock().getData("data").getIntValue());
}

TEST(MockSupportTest, uninitializedData)
{
    LONGS_EQUAL(0, mock().getData("nonexisting").getIntValue());
    STRCMP_EQUAL("int", mock().getData("nonexisting").getType().asCharString());
}

TEST(MockSupportTest, setMultipleData)
{
    mock().setData("data", 1);
    mock().setData("data2", 10);
    LONGS_EQUAL(1, mock().getData("data").getIntValue());
    LONGS_EQUAL(10, mock().getData("data2").getIntValue());
}

TEST(MockSupportTest, setDataString)
{
    mock().setData("data", "string");
    STRCMP_EQUAL("string", mock().getData("data").getStringValue());
}

TEST(MockSupportTest, setDataDouble)
{
    mock().setData("data", 1.0);
    DOUBLES_EQUAL(1.0, mock().getData("data").getDoubleValue(), 0.05);
}

TEST(MockSupportTest, setDataPointer)
{
    void * ptr = (void*) 0x001;
    mock().setData("data", ptr);
    POINTERS_EQUAL(ptr, mock().getData("data").getPointerValue());
}

TEST(MockSupportTest, setConstDataPointer)
{
    const void * ptr = (const void*) 0x001;
    mock().setData("data", ptr);
    POINTERS_EQUAL(ptr, mock().getData("data").getConstPointerValue());
}

TEST(MockSupportTest, setDataFunctionPointer)
{
    void (*ptr)() = (void(*)()) 0x001;
    mock().setData("data", ptr);
    FUNCTIONPOINTERS_EQUAL(ptr, mock().getData("data").getFunctionPointerValue());
}

TEST(MockSupportTest, setDataObject)
{
    void * ptr = (void*) 0x001;
    mock().setDataObject("data", "type", ptr);
    POINTERS_EQUAL(ptr, mock().getData("data").getObjectPointer());
    STRCMP_EQUAL("type", mock().getData("data").getType().asCharString());
}

TEST(MockSupportTest, tracing)
{
    mock().tracing(true);

    mock().actualCall("boo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock("scope").actualCall("foo").withParameter("double", 1.0).withParameter("int", 1).withParameter("string", "string");
    mock().checkExpectations();

    STRCMP_CONTAINS("boo", mock().getTraceOutput());
    STRCMP_CONTAINS("foo", mock().getTraceOutput());
}

TEST(MockSupportTest, tracingWorksHierarchically)
{
    mock("scope").tracing(true);
    mock().tracing(true);

    mock().actualCall("boo");
    mock("scope").actualCall("foo");
    mock().checkExpectations();

    STRCMP_CONTAINS("boo", mock().getTraceOutput());
    STRCMP_CONTAINS("foo", mock().getTraceOutput());
}

TEST_GROUP(MockSupportTestWithFixture)
{
    TestTestingFixture fixture;
};

static void CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failedTestMethod_()
{
    MockExpectedCallsList list;
    MockUnexpectedCallHappenedFailure expectedFailure(UtestShell::getCurrent(), "unexpected", list);
    mock().actualCall("boo");
    CHECK_EXPECTED_MOCK_FAILURE_LOCATION(expectedFailure, "file", 1);
}

TEST(MockSupportTestWithFixture, CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failed)
{
    mock().setMockFailureStandardReporter(MockFailureReporterForTest::getReporter());
    fixture.setTestFunction(CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failedTestMethod_);
    fixture.runAllTests();
    fixture.assertPrintContains("MockFailures are different.");
    fixture.assertPrintContains("Expected MockFailure:");
    fixture.assertPrintContains("Mock Failure: Unexpected call to function: unexpected");
    fixture.assertPrintContains("Actual MockFailure:");
    fixture.assertPrintContains("Mock Failure: Unexpected call to function: boo");
}

static void CHECK_NO_MOCK_FAILURE_LOCATION_failedTestMethod_()
{
    mock().actualCall("boo");
    CHECK_NO_MOCK_FAILURE_LOCATION("file", 1);
}

TEST(MockSupportTestWithFixture, CHECK_NO_MOCK_FAILURE_LOCATION_failed)
{
    mock().setMockFailureStandardReporter(MockFailureReporterForTest::getReporter());
    fixture.setTestFunction(CHECK_NO_MOCK_FAILURE_LOCATION_failedTestMethod_);
    fixture.runAllTests();
    fixture.assertPrintContains("Unexpected mock failure:");
    fixture.assertPrintContains("Mock Failure: Unexpected call to function: boo");
}

static bool cpputestHasCrashed;

static void crashMethod()
{
    cpputestHasCrashed = true;
}

static void unexpectedCallTestFunction_(void)
{
    mock().actualCall("unexpected");
} // LCOV_EXCL_LINE

#include "CppUTestExt/OrderedTest.h"

TEST(MockSupportTestWithFixture, shouldCrashOnFailure)
{
    mock().crashOnFailure(true);
    UtestShell::setCrashMethod(crashMethod);
    fixture.setTestFunction(unexpectedCallTestFunction_);

    fixture.runAllTests();

    CHECK(cpputestHasCrashed);

    mock().crashOnFailure(false);
    UtestShell::resetCrashMethod();
}

TEST(MockSupportTestWithFixture, ShouldNotCrashOnFailureAfterCrashMethodWasReset)
{
    cpputestHasCrashed = false;
    UtestShell::setCrashMethod(crashMethod);
    fixture.setTestFunction(unexpectedCallTestFunction_);
    UtestShell::resetCrashMethod();

    fixture.runAllTests();

    fixture.assertPrintContains("Unexpected call to function: unexpected");
    CHECK_FALSE(cpputestHasCrashed);
}

TEST(MockSupportTestWithFixture, failedMockShouldFailAgainWhenRepeated)
{
    fixture.setTestFunction(unexpectedCallTestFunction_);
    int repeatCount = 2;
    while(repeatCount--)
    {
        fixture.runAllTests();
        fixture.assertPrintContains("Unexpected call to function: unexpected");
        fixture.assertPrintContains("Errors (1 failures, 1 tests, 1 ran, 0 checks, 0 ignored, 0 filtered out");
        fixture.output_->flush();
        delete fixture.result_;
        fixture.result_ = new TestResult(*fixture.output_);
    }
}

