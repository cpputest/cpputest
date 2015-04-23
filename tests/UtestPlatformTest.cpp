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
#include "CppUTest/UtestPlatformWrapper.h"

TEST_GROUP(UtestPlatformTest) {};

TEST(UtestPlatformTest, PlatformSpecificGetPlatformSpecificTimeStringWorksProperly)
{
    struct tm* time = localtime_stub(NULL);
    time->tm_year = 115;
    time->tm_mon = 3;
    time->tm_mday = 29;
    time->tm_hour = 3;
    time->tm_min = 22;
    time->tm_sec = 55;

    STRCMP_EQUAL("2015-04-29T03:22:55", GetPlatformSpecificTimeString());
}

TEST_GROUP(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess)
{
    TestTestingFixture fixture;
};

#if defined(__MINGW32__)

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, MinGwWorks)
{
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains(
       "-p doesn't work on MinGW as it is lacking fork.");
}

#elif defined(_MSC_VER)

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, VisualCppWorks)
{
   fixture.registry_->setRunTestsInSeperateProcess();
   fixture.runAllTests();
   fixture.assertPrintContains(
      "-p doesn't work on Visual C++ as it is lacking fork.");
}

#else

static void _failFunction()
{
    FAIL("This test fails");
}

extern "C" {
    static int fork_failed_stub(void) { return -1; }
    static int waitpid_failed_stub(int, int*, int) { return -1; }
}

static int _accessViolationTestFunction()
{
    return *(volatile int*) 0;
}

static int _divisionByZeroTestFunction()
{
    volatile int a = 1;
    return 1 / (a - a);
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

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, DivisionByZeroInSeparateProcessWorks)
{
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.setTestFunction((void(*)())_divisionByZeroTestFunction);
    fixture.runAllTests();
    fixture.assertPrintContains("Failed in separate process - killed by signal 8");
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

TEST(UTestPlatformsTest_PlatformSpecificRunTestInASeperateProcess, CallToWaitPidFailedInSeparateProcessWorks)
{
    UT_PTR_SET(PlatformSpecificWaitPid, waitpid_failed_stub);
    fixture.registry_->setRunTestsInSeperateProcess();
    fixture.runAllTests();
    fixture.assertPrintContains("Call to waitpid() failed");
}

#endif
