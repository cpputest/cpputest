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
#include "CppUTest/MemoryLeakWarningPlugin.h"
#include "CppUTest/MemoryLeakDetector.h"
#include <stdlib.h>

class MemoryLeakWarningReporter : public MemoryLeakFailure
{
public:
   virtual ~MemoryLeakWarningReporter() {};
   virtual void fail(char* fail_string) {
      FAIL(fail_string);
   }
};

static MemoryLeakWarningReporter reporter;
static MemoryLeakDetector globalDetector(&reporter);

MemoryLeakWarningPlugin* MemoryLeakWarningPlugin::firstPlugin = 0;

MemoryLeakWarningPlugin* MemoryLeakWarningPlugin::getFirstPlugin()
{
   return firstPlugin;
}

MemoryLeakDetector* MemoryLeakWarningPlugin::getMemoryLeakDetector()
{
   return memLeakDetector;
}

void MemoryLeakWarningPlugin::ignoreAllLeaksInTest()
{
   ignoreAllWarnings = true;
}

void MemoryLeakWarningPlugin::expectLeaksInTest(int n)
{
   expectedLeaks = n;
}

MemoryLeakWarningPlugin::MemoryLeakWarningPlugin(const SimpleString& name, MemoryLeakDetector* localDetector)
	: TestPlugin(name), ignoreAllWarnings(false), expectedLeaks(0)
{
   if (firstPlugin == 0) firstPlugin = this;

   if (localDetector) memLeakDetector = localDetector;
   else memLeakDetector = &globalDetector;
}

MemoryLeakWarningPlugin::~MemoryLeakWarningPlugin()
{
}

void MemoryLeakWarningPlugin::preTestAction(Utest& test, TestResult& result)
{
   memLeakDetector->startChecking();
   failureCount = result.getFailureCount();
}

void MemoryLeakWarningPlugin::postTestAction(Utest& test, TestResult& result)
{
   memLeakDetector->stopChecking();
   int leaks = memLeakDetector->totalMemoryLeaks(mem_leak_period_checking);

   if (!ignoreAllWarnings && expectedLeaks != leaks && failureCount == result.getFailureCount()) {
	    Failure f(&test, memLeakDetector->report(mem_leak_period_checking));
	    result.addFailure(f);
   }
   memLeakDetector->markCheckingPeriodLeaksAsNonCheckingPeriod();
   ignoreAllWarnings = false;
   expectedLeaks = 0;
}

void MemoryLeakWarningPlugin::Enable()
{
   memLeakDetector->enable();
}

const char* MemoryLeakWarningPlugin::FinalReport(int toBeDeletedLeaks)
{
   int leaks = memLeakDetector->totalMemoryLeaks(mem_leak_period_enabled);
   if (leaks != toBeDeletedLeaks)
      return memLeakDetector->report(mem_leak_period_enabled);
   return "";
}

#undef new

void* operator new(size_t size)
{
   return globalDetector.allocOperatorNew(size);
}

void operator delete(void* mem)
{
	if (!globalDetector.isGone()) globalDetector.freeOperatorDelete((char*)mem);
}

void* operator new[](size_t size)
{
   return globalDetector.allocOperatorNewArray(size);
}

void operator delete[](void* mem)
{
   if (!globalDetector.isGone()) globalDetector.freeOperatorDeleteArray((char*)mem);
}

void* operator new(size_t size, const char* file, int line)
{
   return globalDetector.allocOperatorNew(size, (char*) file, line);
}

void* operator new [](size_t size, const char* file, int line)
{
   return globalDetector.allocOperatorNewArray(size, (char*) file, line);
}
