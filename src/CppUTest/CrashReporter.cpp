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
#include <execinfo.h> // TODO: extern?
//#include <stdio.h>
//#include <stdlib.h>


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
	consoleOutput = NULL;
	currentTest = NULL;
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
	signal(SIGABRT, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGFPE, SIG_DFL);

}

void CrashReporter::initSignalHandlers()
{
	signal(SIGABRT, signalHandler);
	signal(SIGSEGV, signalHandler);
	signal(SIGINT, signalHandler);
	signal(SIGFPE, signalHandler);

	// TODO: change to sigaction
	// TODO: add aditional signal handlers
	// TODO: catch exceptions?
	// TODO: make modular for differnte OSs
}

void CrashReporter::signalHandler(int signalID)
{
	getInstance()->printCrashMessage(signalID);
	getInstance()->printStackTrace();
	signal(signalID, SIG_DFL);
	raise(signalID);
}

//TODO: http://www.gnu.org/software/libc/manual/html_node/Backtraces.html
void CrashReporter::printStackTrace()
{
	void *array[100];
	int size;
	char **strings;
	int i;

	size = backtrace(array, 100);
	strings = backtrace_symbols(array, size);

	printf ("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++)
	{
		printf ("%s\n", strings[i]);
	}
	fflush(stdout);
}

void CrashReporter::printfCrash(int signalID) // TODO: refactor/clean this up...printf is used in case output has been corrupted
{
	if (validTest(currentTest))
	{
		printf("\n\n %s:%d: error: ", currentTest->getFile().asCharString(), currentTest->getLineNumber());
		printf("TEST(%s, %s):\n", currentTest->getGroup().asCharString(), currentTest->getName().asCharString());
	}
	printf("%s", signalCrashMessage(signalID).asCharString());
	fflush(stdout);
}

SimpleString CrashReporter::signalCrashMessage(int signalID){
	signalID++; // TODO: add in signals & make sure string isn't being copied
	return SimpleString(" Crashed with Signal");
}

bool CrashReporter::validTest(const UtestShell* Test){
	if (Test == NULL) // TODO: add in check for Test getting corrupted during crash
	{
		return false;
	}
	return true;
}

bool CrashReporter::validConsoleOutput(TestOutput* output){
	if (output == NULL) // TODO: add in check for output getting corrupted from crash
	{
		return false;
	}
	return true;
}

void CrashReporter::printCrashMessage(int signal){
	if (validConsoleOutput(consoleOutput)) // restructure for less cases
	{
		if (validTest(currentTest))
		{
			consoleOutput->printCrashMessage(currentTest, signalCrashMessage(signal));
		}
		else
		{
			consoleOutput->printCrashMessage(signalCrashMessage(signal));

		}
	}
	else
	{
		printfCrash(signal);
	}
}
void CrashReporter::updateCurrentTest(const UtestShell* Test) {
     currentTest = Test;
}

void CrashReporter::setOutput(TestOutput* output)
{
	consoleOutput = output;
}
