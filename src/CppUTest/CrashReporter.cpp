/*  ----------------------------------------------------------------------------
    Copyright (c) 2014, MindTribe Product Engineering, Inc.
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

	singleton pattern: http://www.codeproject.com/Articles/1921/Singleton-Pattern-its-implementation-with-C
	TODO: clean this up
    Author(s):  Erin Watson <erin@mindtribe.com>

    Target(s):  ISO/IEC 14882:2003 (C++03)
    ------------------------------------------------------------------------- */

#include "CppUTest/TestHarness.h"
#include "CppUTest/CrashReporter.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestOutput.h"

#include <csignal>
#include <cstdio>

CrashReporter* CrashReporter::singleCrashReporter = NULL;

CrashReporter* CrashReporter::getInstance()
{
	if (singleCrashReporter == NULL)
	{
		static CrashReporter singleCrashReporter;
		return &singleCrashReporter;
	}
	return singleCrashReporter;
}

CrashReporter::CrashReporter()
{
	initSignalHandlers();
	// TODO: init currenttest
}

CrashReporter::~CrashReporter()
{
	destroySignalHandlers();
	singleCrashReporter = NULL;
	// TODO: singleCrashReporter = NULL, necessary?
}

void CrashReporter::destroySignalHandlers()
{
	std::signal(SIGABRT, SIG_DFL);
	std::signal(SIGSEGV, SIG_DFL);
	std::signal(SIGINT, SIG_DFL);

}

void CrashReporter::initSignalHandlers()
{
	std::signal(SIGABRT, signalHandler);
	std::signal(SIGSEGV, signalHandler);
	std::signal(SIGINT, signalHandler);
	std::signal(SIGFPE, signalHandler);

	// TODO: change to sigaction
	// TODO: add aditional signal handlers
	// TODO: catch exceptions?
	// TODO: make modular for differnte OSs
}

void CrashReporter::signalHandler(int signal)
{
	getInstance()->printEclipseCrash(signal);
	std::signal(signal, SIG_DFL);
	std::raise(signal);
}

void CrashReporter::printEclipseCrash(int signal) // TODO: incorporate with TestOutput, make cross platform
{
	printf("\n\n %s:%d: error:", currentTest.file.asCharString(), currentTest.lineNumber);
	printf("Crash in TEST(%s, %s): Signal %d \n\n", currentTest.group.asCharString(), currentTest.name.asCharString(), signal);
	fflush(stdout);

}


void CrashReporter::updateCurrentTest(const UtestShell* Test) {
    currentTest.name = Test->getName();
    currentTest.group = Test->getGroup();
    currentTest.file = Test->getFile();
    currentTest.lineNumber = Test->getLineNumber();
}
