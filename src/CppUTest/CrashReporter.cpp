/*  ----------------------------------------------------------------------------
    Copyright (c) 2015, MindTribe Product Engineering, Inc.
    All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of MindTribe Product Engineering, Inc nor the names of
       its contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    Author(s):  Erin Watson <erin@mindtribe.com>

    Target(s):  ISO/IEC 14882:2003 (C++03)
    ------------------------------------------------------------------------- */

#include "CppUTest/CrashReporter.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include <stdio.h>

CrashReporter* CrashReporter::singleCrashReporter = NULL;

CrashReporter* CrashReporter::getInstance()
{
    if (singleCrashReporter == NULL) {
        singleCrashReporter = new CrashReporter();
    }
    return singleCrashReporter;
}

CrashReporter::CrashReporter()
{
    testRunnerOutput = NULL;
    currentTest = NULL;
    PlatformSpecificInstallCrashTrap(crashHandler, NULL);
}

CrashReporter::~CrashReporter()
{
    PlatformSpecificRemoveCrashTrap();
    delete(singleCrashReporter);
    singleCrashReporter = NULL;
}

void CrashReporter::crashHandler(void *context, const char *crash_message)
{
    (void)context;
    getInstance()->printCrashMessage(crash_message);
}

void CrashReporter::setCurrentTest(const UtestShell* test)
{
    currentTest = test;
}

void CrashReporter::setOutput(TestOutput* output)
{
    testRunnerOutput = output;
}

bool CrashReporter::validTest(const UtestShell* Test){
    if (Test == NULL) {
        return false;
    } else {
        return true;
    }
}

bool CrashReporter::validOutput(TestOutput* output){
    if (output == NULL) {
        return false;
    } else {
        return true;
    }
}

void CrashReporter::printCrashMessage(char const *crashMessage)
{
    if (validOutput(testRunnerOutput)) {
        printCrashMessageTestRunnerOutput(crashMessage);
    } else {
        printCrashMessageConsoleOutput(crashMessage);
    }
}

void CrashReporter::printCrashMessageTestRunnerOutput(char const *crashMessage)
{
    if (validTest(currentTest)) {
        testRunnerOutput->printCrashMessage(currentTest, crashMessage);
    } else {
        testRunnerOutput->printCrashMessage(crashMessage);
    }
}

void CrashReporter::printCrashMessageConsoleOutput(char const *crashMessage)
{
    if (validTest(currentTest)) {
        printf("\n %s:%u: error: Crashed with '%s' running test %s\n\n",
            currentTest->getFile().asCharString(),
            currentTest->getLineNumber(),
            crashMessage,
            currentTest->getFormattedName().asCharString());
    } else {
        printf("\nCrashed with '%s' running unknown test.\n\n", crashMessage);
    }
}
