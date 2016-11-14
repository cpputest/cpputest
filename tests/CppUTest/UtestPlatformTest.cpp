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

// This will cause a crash in VS2010 due to PlatformSpecificFree being uninitialized
static const SimpleString str1("abc");
static const SimpleString str2("def");
static const SimpleString str3(str1 + str2);

TEST_GROUP(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess)
{
    TestTestingFixture fixture;
};

#ifndef CPPUTEST_HAVE_FORK

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, DummyFailsWithMessage)
{
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("-p doesn't work on this platform, as it is lacking fork.\b");
}

#else

static void _failFunction()
{
    FAIL("This test fails");
}

#include <errno.h>

extern "C" {

    static int (*original_waitpid)(int, int*, int) = NULL;

    static int fork_failed_stub(void) { return -1; }

    static int waitpid_while_debugging_stub(int pid, int* status, int options)
    {
        static int number_called = 0;
        static int saved_status;

        if (number_called++ < 10) {
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

static int _accessViolationTestFunction()
{
    return *(volatile int*) 0;
}

#include <unistd.h>
#include <signal.h>

static void _stoppedTestFunction()
{
    kill(getpid(), SIGSTOP);
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, FailureInSeparateProcessWorks)
{
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.setTestFunction(_failFunction);
    fixture.runAllTests();
    fixture.assertPrintContains("Failed in separate process");
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, AccessViolationInSeparateProcessWorks)
{
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.setTestFunction((void(*)())_accessViolationTestFunction);
    fixture.runAllTests();
    fixture.assertPrintContains("Failed in separate process - killed by signal 11");
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, StoppedInSeparateProcessWorks)
{
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.setTestFunction(_stoppedTestFunction);
    fixture.runAllTests();
    fixture.assertPrintContains("Stopped in separate process - continuing");
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, CallToForkFailedInSeparateProcessWorks)
{
    UT_PTR_SET(PlatformSpecificFork, fork_failed_stub);
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("Call to fork() failed");
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, CallToWaitPidWhileDebuggingInSeparateProcessWorks)
{
    UT_PTR_SET(original_waitpid, PlatformSpecificWaitPid);
    UT_PTR_SET(PlatformSpecificWaitPid, waitpid_while_debugging_stub);
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("OK (1 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out");
}

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, CallToWaitPidFailedInSeparateProcessWorks)
{
    UT_PTR_SET(PlatformSpecificWaitPid, waitpid_failed_stub);
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("Call to waitpid() failed");
}

#endif
