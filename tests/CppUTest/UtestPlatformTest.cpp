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
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/StandardCLibrary.h"
#include "CppUTest/TestMemoryAllocator.h"

#if CPPUTEST_USE_STD_C_LIB

// This will cause a crash in VS2010 due to PlatformSpecificFree being uninitialized
static const SimpleString str1("abc");
static const SimpleString str2("def");
static const SimpleString str3(str1 + str2);

TEST_GROUP(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess)
{
    TestTestingFixture fixture;
};

// There is a possibility that a compiler provides fork but not waitpid.
#if !defined(CPPUTEST_HAVE_FORK) || !defined(CPPUTEST_HAVE_WAITPID)

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, DummyFailsWithMessage)
{
    fixture.setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("-p doesn't work on this platform, as it is lacking fork.\b");
}

#else

static void failFunction_()
{
    FAIL("This test fails");
}

static void exitNonZeroFunction_() _no_return_;
static void exitNonZeroFunction_()
{
    /* destructor of static objects will be called. If StringCache was there then the allocator will report invalid deallocations of static SimpleString */
    SimpleString::setStringAllocator(SimpleString::getStringAllocator()->actualAllocator());
    exit(1);
}

#include <errno.h>

static int waitpid_while_debugging_stub_number_called = 0;
static int waitpid_while_debugging_stub_forced_failures = 0;

extern "C" {

    static int (*original_waitpid)(int, int*, int) = NULLPTR;

    static int fork_failed_stub(void) { return -1; }

    static int waitpid_while_debugging_stub(int pid, int* status, int options)
    {
        static int saved_status;

        if (waitpid_while_debugging_stub_number_called++ < waitpid_while_debugging_stub_forced_failures) {
            saved_status = *status;
            errno=EINTR;
            return -1;
        }
        else {
            *status = saved_status;
            return original_waitpid(pid, status, options);
        }
    }

    static int waitpid_failed_stub(int, int*, int) { return -1; }
}

#include <unistd.h>
#include <signal.h>

static void stoppedTestFunction_()
{
    kill(getpid(), SIGSTOP);
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, TestInSeparateProcessWorks)
{
    fixture.setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("OK (1 tests, 1 ran, 0 checks, 0 ignored, 0 filtered out");
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, FailureInSeparateProcessWorks)
{
    fixture.setRunTestsInSeperateProcess();
    fixture.setTestFunction(failFunction_);
    fixture.runAllTests();
    fixture.assertPrintContains("Failed in separate process");
    fixture.assertPrintContains("Errors (1 failures, 1 tests, 1 ran, 0 checks, 0 ignored, 0 filtered out");
}

#if (! CPPUTEST_SANITIZE_ADDRESS)

static int accessViolationTestFunction_()
{
    return *(volatile int*) NULLPTR;
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, AccessViolationInSeparateProcessWorks)
{
    fixture.setRunTestsInSeperateProcess();
    fixture.setTestFunction((void(*)())accessViolationTestFunction_);
    fixture.runAllTests();
    fixture.assertPrintContains("Failed in separate process - killed by signal 11");
    fixture.assertPrintContains("Errors (1 failures, 1 tests, 1 ran");
}

#endif

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, StoppedInSeparateProcessWorks)
{
    fixture.setRunTestsInSeperateProcess();
    fixture.setTestFunction(stoppedTestFunction_);
    fixture.runAllTests();
    fixture.assertPrintContains("Stopped in separate process - continuing");
    fixture.assertPrintContains("Errors (1 failures, 1 tests, 1 ran");
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, CallToForkFailedInSeparateProcessWorks)
{
    UT_PTR_SET(PlatformSpecificFork, fork_failed_stub);
    fixture.setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("Call to fork() failed");
    fixture.assertPrintContains("Errors (1 failures, 1 tests, 1 ran");
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, CallToWaitPidWhileDebuggingInSeparateProcessWorks)
{
    UT_PTR_SET(original_waitpid, PlatformSpecificWaitPid);
    UT_PTR_SET(PlatformSpecificWaitPid, waitpid_while_debugging_stub);
    waitpid_while_debugging_stub_number_called = 0;
    waitpid_while_debugging_stub_forced_failures = 10;
    fixture.setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("OK (1 tests, 1 ran, 0 checks, 0 ignored, 0 filtered out");
    // extra check to confirm that waitpid() was polled until it passed (and passed call adds one)
    CHECK(waitpid_while_debugging_stub_number_called > waitpid_while_debugging_stub_forced_failures);
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, CallToWaitPidStopsAndReportsAnErrorAfter20TimesRetry)
{
    UT_PTR_SET(original_waitpid, PlatformSpecificWaitPid);
    UT_PTR_SET(PlatformSpecificWaitPid, waitpid_while_debugging_stub);
    waitpid_while_debugging_stub_number_called = 0;
    waitpid_while_debugging_stub_forced_failures = 40;
    fixture.setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("Call to waitpid() failed with EINTR. Tried 30 times and giving up! Sometimes happens in debugger");
    // extra check to confirm that waitpid() was polled until it passed (and passed call adds one)
    CHECK(waitpid_while_debugging_stub_number_called > 30);
}


TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, CallToWaitPidFailedInSeparateProcessWorks)
{
    UT_PTR_SET(PlatformSpecificWaitPid, waitpid_failed_stub);
    fixture.setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("Call to waitpid() failed");
    fixture.assertPrintContains("Errors (1 failures, 1 tests, 1 ran");
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, MultipleTestsInSeparateProcessAreCountedProperly)
{
    fixture.setRunTestsInSeperateProcess();
    fixture.runTestWithMethod(NULLPTR);
    fixture.runTestWithMethod(stoppedTestFunction_);
    fixture.runTestWithMethod(NULLPTR);
    fixture.runTestWithMethod(exitNonZeroFunction_);
    fixture.runTestWithMethod(NULLPTR);
    fixture.assertPrintContains("Failed in separate process");
    fixture.assertPrintContains("Stopped in separate process");
    fixture.assertPrintContains("Errors (2 failures, 5 tests, 5 ran, 0 checks, 0 ignored, 0 filtered out");
}

#endif
#endif
