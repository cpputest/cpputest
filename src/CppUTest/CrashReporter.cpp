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
#include <signal.h>
#include <execinfo.h> // TODO: extern
#include <stdio.h>

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
	initSignalMap();
	initSignalHandlers();
	programOutput = NULL;
	currentTest = NULL;
}

CrashReporter::~CrashReporter()
{
	destroySignalHandlers();
	singleCrashReporter = NULL;// TODO: is this necessary?
}

void CrashReporter::destroySignalHandlers()
{
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	updateSignalHandlers(&sa);
}

void CrashReporter::initSignalMap()
{
	signalMap[SIGABRT] = "SIGABRT";
	signalMap[SIGFPE] = "SIGFPE";
	signalMap[SIGILL] = "SIGILL";
	signalMap[SIGINT] = "SIGINT";
	signalMap[SIGSEGV] = "SIGSEGV";
	signalMap[SIGTERM] = "SIGTERM";
}

void CrashReporter::updateSignalHandlers(struct sigaction* sa)
{
	for (std::map<int,SimpleString>::iterator i = signalMap.begin(); i != signalMap.end(); i++)
	{
		sigaction(i->first, sa, NULL);
	}
}

void CrashReporter::initSignalHandlers()
{
	struct sigaction sa;
	sa.sa_handler = signalHandler;
	updateSignalHandlers(&sa);
}

void CrashReporter::signalHandler(int signalID)
{
	getInstance()->crashSignalName = getInstance()->signalName(signalID);
	getInstance()->printCrashMessage();
	signal(signalID, SIG_DFL);
	raise(signalID);
}

SimpleString CrashReporter::signalName(int signal) {
	return signalMap[signal];
}

void CrashReporter::updateCurrentTest(const UtestShell* Test) {
     currentTest = Test;
}

void CrashReporter::setOutput(TestOutput* output)
{
	programOutput = output;
}

bool CrashReporter::validTest(const UtestShell* Test){
	if (Test == NULL) // TODO: add in check for Test getting corrupted during crash
	{
		return false;
	}
	return true;
}

bool CrashReporter::validProgramOutput(TestOutput* output){
	if (output == NULL) // TODO: add in check for output getting corrupted from crash
	{
		return false;
	}
	return true;
}

void CrashReporter::printCrashMessage()
{
	programOutput = NULL;
	if (validProgramOutput(programOutput))
	{
		printCrashMessageProgramOutput();
	}
	else
	{
		printCrashMessageConsoleOutput();
	}
}

void CrashReporter::printCrashMessageProgramOutput()
{
	if (validTest(currentTest))
	{
		programOutput->printCrashMessage(currentTest, crashSignalName);
	}
	else
	{
		programOutput->printCrashMessage(crashSignalName);

	}
}

void CrashReporter::printCrashMessageConsoleOutput()
{
	if (validTest(currentTest))
	{
		printf("\n\n %s:%d: error: ", currentTest->getFile().asCharString(), currentTest->getLineNumber());
		printf(" Crashed with signal ");
		printf("%s", crashSignalName.asCharString());
		printf(" running test ");
		printf("%s", (currentTest->getFormattedName()).asCharString());
		printf("\n\n");
	}
	else
	{
		printf("Crash during Unknown Test with crash signal ");
		printf("%s", crashSignalName.asCharString());
		printf("\n\n");
	}
	fflush(stdout);
}


