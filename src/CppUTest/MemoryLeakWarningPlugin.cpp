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
#include "CppUTest/MemoryLeakAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"

class MemoryLeakWarningReporter: public MemoryLeakFailure
{
public:
	virtual ~MemoryLeakWarningReporter()
	{
	}
	;
	virtual void fail(char* fail_string)
	{
		FAIL(fail_string);
	}
};

static MemoryLeakWarningReporter* globalReporter = 0;
static MemoryLeakDetector* globalDetector = 0;

void destroyDetector()
{
	PlatformSpecificFree(globalDetector);
	globalReporter->~MemoryLeakWarningReporter();
	PlatformSpecificFree(globalReporter);
	globalReporter = 0;
	globalDetector = 0;
}

MemoryLeakDetector* MemoryLeakWarningPlugin::getGlobalDetector()
{
	if (globalDetector == 0) {

		/*  Want to void using operator new here, however.. still need to init the vtable.
		 *  Now just memcpy a local stack variable in the malloced memory. Ought to work everywhere :))
		 */
		MemoryLeakWarningReporter reporter;
		globalReporter = (MemoryLeakWarningReporter*) PlatformSpecificMalloc(sizeof(MemoryLeakWarningReporter));
		PlatformSpecificMemCpy(globalReporter, &reporter, sizeof(MemoryLeakWarningReporter));

		globalDetector = (MemoryLeakDetector*) PlatformSpecificMalloc(sizeof(MemoryLeakDetector));
		if (globalDetector == 0)
		FAIL("operator new(size, bool) not enough memory");
		globalDetector->init(globalReporter);
	}
	return globalDetector;
}

MemoryLeakWarningPlugin* MemoryLeakWarningPlugin::firstPlugin_ = 0;

MemoryLeakWarningPlugin* MemoryLeakWarningPlugin::getFirstPlugin()
{
	return firstPlugin_;
}

MemoryLeakDetector* MemoryLeakWarningPlugin::getMemoryLeakDetector()
{
	return memLeakDetector_;
}

void MemoryLeakWarningPlugin::ignoreAllLeaksInTest()
{
	ignoreAllWarnings_ = true;
}

void MemoryLeakWarningPlugin::expectLeaksInTest(int n)
{
	expectedLeaks_ = n;
}

MemoryLeakWarningPlugin::MemoryLeakWarningPlugin(const SimpleString& name, MemoryLeakDetector* localDetector) :
	TestPlugin(name), ignoreAllWarnings_(false), expectedLeaks_(0)
{
	if (firstPlugin_ == 0) firstPlugin_ = this;

	if (localDetector) memLeakDetector_ = localDetector;
	else memLeakDetector_ = getGlobalDetector();

	memLeakDetector_->enable();
}

MemoryLeakWarningPlugin::~MemoryLeakWarningPlugin()
{
	if (this == firstPlugin_) firstPlugin_ = 0;
}

void MemoryLeakWarningPlugin::preTestAction(Utest& /*test*/, TestResult& result)
{
	memLeakDetector_->startChecking();
	failureCount_ = result.getFailureCount();
}

void MemoryLeakWarningPlugin::postTestAction(Utest& test, TestResult& result)
{
	memLeakDetector_->stopChecking();
	int leaks = memLeakDetector_->totalMemoryLeaks(mem_leak_period_checking);

	if (!ignoreAllWarnings_ && expectedLeaks_ != leaks && failureCount_ == result.getFailureCount()) {
		TestFailure f(&test, memLeakDetector_->report(mem_leak_period_checking));
		result.addFailure(f);
	}
	memLeakDetector_->markCheckingPeriodLeaksAsNonCheckingPeriod();
	ignoreAllWarnings_ = false;
	expectedLeaks_ = 0;
}

const char* MemoryLeakWarningPlugin::FinalReport(int toBeDeletedLeaks)
{
	int leaks = memLeakDetector_->totalMemoryLeaks(mem_leak_period_enabled);
	if (leaks != toBeDeletedLeaks) return memLeakDetector_->report(mem_leak_period_enabled);
	return "";
}

#if CPPUTEST_USE_MEM_LEAK_DETECTION
#undef new

#if CPPUTEST_USE_STD_CPP_LIB
#define UT_THROW_BAD_ALLOC_WHEN_NULL(memory) if (memory == NULL) throw std::bad_alloc();
#define UT_THROW(except) throw (except)
#define UT_THROW_EMPTY() throw ()
#else
#define UT_THROW_BAD_ALLOC_WHEN_NULL(memory)
#define UT_THROW(except)
#define UT_THROW_EMPTY()
#endif

void* operator new(size_t size) UT_THROW(std::bad_alloc)
{
	void* memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(MemoryLeakAllocator::getCurrentNewAllocator(), size);
	UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
	return memory;
}

void* operator new(size_t size, const char* file, int line) UT_THROW(std::bad_alloc)
{
	void *memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(MemoryLeakAllocator::getCurrentNewAllocator(), size, (char*) file, line);
	UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
	return memory;
}

void operator delete(void* mem) UT_THROW_EMPTY()
{
	MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(MemoryLeakAllocator::getCurrentNewAllocator(), (char*) mem);
}

void* operator new[](size_t size) UT_THROW(std::bad_alloc)
{
	void* memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(MemoryLeakAllocator::getCurrentNewArrayAllocator(), size);
	UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
	return memory;
}

void* operator new [](size_t size, const char* file, int line) UT_THROW(std::bad_alloc)
{
	void* memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(MemoryLeakAllocator::getCurrentNewArrayAllocator(), size, (char*) file, line);
	UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
	return memory;
}

void operator delete[](void* mem) UT_THROW_EMPTY()
{
	MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(MemoryLeakAllocator::getCurrentNewArrayAllocator(), (char*) mem);
}

#endif
