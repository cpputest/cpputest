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
#include "CppUTest/TestMemoryAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/SimpleMutex.h"

/********** Enabling and disabling for C also *********/

#if CPPUTEST_USE_MEM_LEAK_DETECTION

class MemLeakScopedMutex
{
public:
    MemLeakScopedMutex() : lock(MemoryLeakWarningPlugin::getGlobalDetector()->getMutex()) { }
private:
    ScopedMutexLock lock;
};

static void* threadsafe_mem_leak_malloc(size_t size, const char* file, int line)
{
    MemLeakScopedMutex lock;
    return MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentMallocAllocator(), size, file, line, true);
}

static void threadsafe_mem_leak_free(void* buffer, const char* file, int line)
{
    MemLeakScopedMutex lock;
    MemoryLeakWarningPlugin::getGlobalDetector()->invalidateMemory((char*) buffer);
    MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(getCurrentMallocAllocator(), (char*) buffer, file, line, true);
}

static void* threadsafe_mem_leak_realloc(void* memory, size_t size, const char* file, int line)
{
    MemLeakScopedMutex lock;
    return MemoryLeakWarningPlugin::getGlobalDetector()->reallocMemory(getCurrentMallocAllocator(), (char*) memory, size, file, line, true);
}


static void* mem_leak_malloc(size_t size, const char* file, int line)
{
    return MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentMallocAllocator(), size, file, line, true);
}

static void mem_leak_free(void* buffer, const char* file, int line)
{
    MemoryLeakWarningPlugin::getGlobalDetector()->invalidateMemory((char*) buffer);
    MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(getCurrentMallocAllocator(), (char*) buffer, file, line, true);
}

static void* mem_leak_realloc(void* memory, size_t size, const char* file, int line)
{
    return MemoryLeakWarningPlugin::getGlobalDetector()->reallocMemory(getCurrentMallocAllocator(), (char*) memory, size, file, line, true);
}

#endif

static void* normal_malloc(size_t size, const char*, int)
{
    return PlatformSpecificMalloc(size);
}

static void* normal_realloc(void* memory, size_t size, const char*, int)
{
    return PlatformSpecificRealloc(memory, size);
}

static void normal_free(void* buffer, const char*, int)
{
    PlatformSpecificFree(buffer);
}

#if CPPUTEST_USE_MEM_LEAK_DETECTION
static void *(*malloc_fptr)(size_t size, const char* file, int line) = mem_leak_malloc;
static void (*free_fptr)(void* mem, const char* file, int line) = mem_leak_free;
static void*(*realloc_fptr)(void* memory, size_t size, const char* file, int line) = mem_leak_realloc;
#else
static void *(*malloc_fptr)(size_t size, const char* file, int line) = normal_malloc;
static void (*free_fptr)(void* mem, const char* file, int line) = normal_free;
static void*(*realloc_fptr)(void* memory, size_t size, const char* file, int line) = normal_realloc;
#endif

void* cpputest_malloc_location_with_leak_detection(size_t size, const char* file, int line)
{
    return malloc_fptr(size, file, line);
}

void* cpputest_realloc_location_with_leak_detection(void* memory, size_t size, const char* file, int line)
{
    return realloc_fptr(memory, size, file, line);
}

void cpputest_free_location_with_leak_detection(void* buffer, const char* file, int line)
{
    free_fptr(buffer, file, line);
}

/********** C++ *************/

#if CPPUTEST_USE_MEM_LEAK_DETECTION
#undef new

#if CPPUTEST_USE_STD_CPP_LIB
#define UT_THROW_BAD_ALLOC_WHEN_NULL(memory) if (memory == NULL) throw std::bad_alloc();
#else
#define UT_THROW_BAD_ALLOC_WHEN_NULL(memory)
#endif

static void* threadsafe_mem_leak_operator_new (size_t size) UT_THROW(std::bad_alloc)
{
    MemLeakScopedMutex lock;
    void* memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewAllocator(), size);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void* threadsafe_mem_leak_operator_new_nothrow (size_t size) UT_NOTHROW
{
    MemLeakScopedMutex lock;
    return MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewAllocator(), size);
}

static void* threadsafe_mem_leak_operator_new_debug (size_t size, const char* file, int line) UT_THROW(std::bad_alloc)
{
    MemLeakScopedMutex lock;
    void *memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewAllocator(), size, (char*) file, line);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void* threadsafe_mem_leak_operator_new_array (size_t size) UT_THROW(std::bad_alloc)
{
    MemLeakScopedMutex lock;
    void* memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewArrayAllocator(), size);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void* threadsafe_mem_leak_operator_new_array_nothrow (size_t size) UT_NOTHROW
{
    MemLeakScopedMutex lock;
    return MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewArrayAllocator(), size);
}

static void* threadsafe_mem_leak_operator_new_array_debug (size_t size, const char* file, int line) UT_THROW(std::bad_alloc)
{
    MemLeakScopedMutex lock;
    void* memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewArrayAllocator(), size, (char*) file, line);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void threadsafe_mem_leak_operator_delete (void* mem) UT_NOTHROW
{
    MemLeakScopedMutex lock;
    MemoryLeakWarningPlugin::getGlobalDetector()->invalidateMemory((char*) mem);
    MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(getCurrentNewAllocator(), (char*) mem);
}

static void threadsafe_mem_leak_operator_delete_array (void* mem) UT_NOTHROW
{
    MemLeakScopedMutex lock;
    MemoryLeakWarningPlugin::getGlobalDetector()->invalidateMemory((char*) mem);
    MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(getCurrentNewArrayAllocator(), (char*) mem);
}


static void* mem_leak_operator_new (size_t size) UT_THROW(std::bad_alloc)
{
    void* memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewAllocator(), size);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void* mem_leak_operator_new_nothrow (size_t size) UT_NOTHROW
{
    return MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewAllocator(), size);
}

static void* mem_leak_operator_new_debug (size_t size, const char* file, int line) UT_THROW(std::bad_alloc)
{
    void *memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewAllocator(), size, (char*) file, line);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void* mem_leak_operator_new_array (size_t size) UT_THROW(std::bad_alloc)
{
    void* memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewArrayAllocator(), size);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void* mem_leak_operator_new_array_nothrow (size_t size) UT_NOTHROW
{
    return MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewArrayAllocator(), size);
}

static void* mem_leak_operator_new_array_debug (size_t size, const char* file, int line) UT_THROW(std::bad_alloc)
{
    void* memory = MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(getCurrentNewArrayAllocator(), size, (char*) file, line);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void mem_leak_operator_delete (void* mem) UT_NOTHROW
{
    MemoryLeakWarningPlugin::getGlobalDetector()->invalidateMemory((char*) mem);
    MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(getCurrentNewAllocator(), (char*) mem);
}

static void mem_leak_operator_delete_array (void* mem) UT_NOTHROW
{
    MemoryLeakWarningPlugin::getGlobalDetector()->invalidateMemory((char*) mem);
    MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(getCurrentNewArrayAllocator(), (char*) mem);
}

static void* normal_operator_new (size_t size) UT_THROW(std::bad_alloc)
{
    void* memory = PlatformSpecificMalloc(size);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void* normal_operator_new_nothrow (size_t size) UT_NOTHROW
{
    return PlatformSpecificMalloc(size);
}

static void* normal_operator_new_debug (size_t size, const char* /*file*/, int /*line*/) UT_THROW(std::bad_alloc)
{
    void* memory = PlatformSpecificMalloc(size);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void* normal_operator_new_array (size_t size) UT_THROW(std::bad_alloc)
{
    void* memory = PlatformSpecificMalloc(size);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void* normal_operator_new_array_nothrow (size_t size) UT_NOTHROW
{
    return PlatformSpecificMalloc(size);
}

static void* normal_operator_new_array_debug (size_t size, const char* /*file*/, int /*line*/) UT_THROW(std::bad_alloc)
{
    void* memory = PlatformSpecificMalloc(size);
    UT_THROW_BAD_ALLOC_WHEN_NULL(memory);
    return memory;
}

static void normal_operator_delete (void* mem) UT_NOTHROW
{
    PlatformSpecificFree(mem);
}

static void normal_operator_delete_array (void* mem) UT_NOTHROW
{
    PlatformSpecificFree(mem);
}

static void *(*operator_new_fptr)(size_t size) UT_THROW(std::bad_alloc) = mem_leak_operator_new;
static void *(*operator_new_nothrow_fptr)(size_t size) UT_NOTHROW = mem_leak_operator_new_nothrow;
static void *(*operator_new_debug_fptr)(size_t size, const char* file, int line) UT_THROW(std::bad_alloc) = mem_leak_operator_new_debug;
static void *(*operator_new_array_fptr)(size_t size) UT_THROW(std::bad_alloc) = mem_leak_operator_new_array;
static void *(*operator_new_array_nothrow_fptr)(size_t size) UT_NOTHROW = mem_leak_operator_new_array_nothrow;
static void *(*operator_new_array_debug_fptr)(size_t size, const char* file, int line) UT_THROW(std::bad_alloc) = mem_leak_operator_new_array_debug;
static void (*operator_delete_fptr)(void* mem) UT_NOTHROW = mem_leak_operator_delete;
static void (*operator_delete_array_fptr)(void* mem) UT_NOTHROW = mem_leak_operator_delete_array;

void* operator new(size_t size) UT_THROW(std::bad_alloc)
{
    return operator_new_fptr(size);
}

void* operator new(size_t size, const char* file, int line) UT_THROW(std::bad_alloc)
{
    return operator_new_debug_fptr(size, file, line);
}

void operator delete(void* mem) UT_NOTHROW
{
    operator_delete_fptr(mem);
}

void operator delete(void* mem, const char*, int) UT_NOTHROW
{
    operator_delete_fptr(mem);
}

void* operator new[](size_t size) UT_THROW(std::bad_alloc)
{
    return operator_new_array_fptr(size);
}

void* operator new [](size_t size, const char* file, int line) UT_THROW(std::bad_alloc)
{
    return operator_new_array_debug_fptr(size, file, line);
}

void operator delete[](void* mem) UT_NOTHROW
{
     operator_delete_array_fptr(mem);
}

void operator delete[](void* mem, const char*, int) UT_NOTHROW
{
     operator_delete_array_fptr(mem);
}


#if CPPUTEST_USE_STD_CPP_LIB

void* operator new(size_t size, const std::nothrow_t&) UT_NOTHROW
{
    return operator_new_nothrow_fptr(size);
}

void* operator new[](size_t size, const std::nothrow_t&) UT_NOTHROW
{
    return operator_new_array_nothrow_fptr(size);
}

#else

/* Have a similar method. This avoid unused operator_new_nothrow_fptr warning */

extern void* operator_new_nothrow(size_t size) UT_NOTHROW;
extern void* operator_new_array_nothrow(size_t size) UT_NOTHROW;

void* operator_new_nothrow(size_t size) UT_NOTHROW
{
    return operator_new_nothrow_fptr(size);
}

void* operator_new_array_nothrow(size_t size) UT_NOTHROW
{
    return operator_new_array_nothrow_fptr(size);
}

#endif
#endif

void MemoryLeakWarningPlugin::turnOffNewDeleteOverloads()
{
#if CPPUTEST_USE_MEM_LEAK_DETECTION
    operator_new_fptr = normal_operator_new;
    operator_new_nothrow_fptr = normal_operator_new_nothrow;
    operator_new_debug_fptr = normal_operator_new_debug;
    operator_new_array_fptr = normal_operator_new_array;
    operator_new_array_nothrow_fptr = normal_operator_new_array_nothrow;
    operator_new_array_debug_fptr = normal_operator_new_array_debug;
    operator_delete_fptr = normal_operator_delete;
    operator_delete_array_fptr = normal_operator_delete_array;
    malloc_fptr = normal_malloc;
    realloc_fptr = normal_realloc;
    free_fptr = normal_free;

#endif
}

void MemoryLeakWarningPlugin::turnOnNewDeleteOverloads()
{
#if CPPUTEST_USE_MEM_LEAK_DETECTION
    operator_new_fptr = mem_leak_operator_new;
    operator_new_nothrow_fptr = mem_leak_operator_new_nothrow;
    operator_new_debug_fptr = mem_leak_operator_new_debug;
    operator_new_array_fptr = mem_leak_operator_new_array;
    operator_new_array_nothrow_fptr = mem_leak_operator_new_array_nothrow;
    operator_new_array_debug_fptr = mem_leak_operator_new_array_debug;
    operator_delete_fptr = mem_leak_operator_delete;
    operator_delete_array_fptr = mem_leak_operator_delete_array;
    malloc_fptr = mem_leak_malloc;
    realloc_fptr = mem_leak_realloc;
    free_fptr = mem_leak_free;
#endif
}

bool MemoryLeakWarningPlugin::areNewDeleteOverloaded()
{
#if CPPUTEST_USE_MEM_LEAK_DETECTION
    return operator_new_fptr == mem_leak_operator_new;
#else
    return false;
#endif
}

void MemoryLeakWarningPlugin::turnOnThreadSafeNewDeleteOverloads()
{
#if CPPUTEST_USE_MEM_LEAK_DETECTION
    operator_new_fptr = threadsafe_mem_leak_operator_new;
    operator_new_nothrow_fptr = threadsafe_mem_leak_operator_new_nothrow;
    operator_new_debug_fptr = threadsafe_mem_leak_operator_new_debug;
    operator_new_array_fptr = threadsafe_mem_leak_operator_new_array;
    operator_new_array_nothrow_fptr = threadsafe_mem_leak_operator_new_array_nothrow;
    operator_new_array_debug_fptr = threadsafe_mem_leak_operator_new_array_debug;
    operator_delete_fptr = threadsafe_mem_leak_operator_delete;
    operator_delete_array_fptr = threadsafe_mem_leak_operator_delete_array;
    malloc_fptr = threadsafe_mem_leak_malloc;
    realloc_fptr = threadsafe_mem_leak_realloc;
    free_fptr = threadsafe_mem_leak_free;
#endif
}

void crash_on_allocation_number(unsigned alloc_number)
{
    static CrashOnAllocationAllocator crashAllocator;
    crashAllocator.setNumberToCrashOn(alloc_number);
    setCurrentMallocAllocator(&crashAllocator);
    setCurrentNewAllocator(&crashAllocator);
    setCurrentNewArrayAllocator(&crashAllocator);
}

class MemoryLeakWarningReporter: public MemoryLeakFailure
{
public:
    virtual ~MemoryLeakWarningReporter()
    {
    }

    virtual void fail(char* fail_string)
    {
        UtestShell* currentTest = UtestShell::getCurrent();
        currentTest->failWith(FailFailure(currentTest, currentTest->getName().asCharString(), currentTest->getLineNumber(), fail_string), TestTerminatorWithoutExceptions());
    } // LCOV_EXCL_LINE
};

static MemoryLeakFailure* globalReporter = 0;
static MemoryLeakDetector* globalDetector = 0;

MemoryLeakDetector* MemoryLeakWarningPlugin::getGlobalDetector()
{
    if (globalDetector == 0) {
        bool newDeleteOverloaded = areNewDeleteOverloaded();
        turnOffNewDeleteOverloads();

        globalReporter = new MemoryLeakWarningReporter;
        globalDetector = new MemoryLeakDetector(globalReporter);

        if (newDeleteOverloaded) turnOnNewDeleteOverloads();
    }
    return globalDetector;
}

MemoryLeakFailure* MemoryLeakWarningPlugin::getGlobalFailureReporter()
{
    return globalReporter;
}

void MemoryLeakWarningPlugin::destroyGlobalDetectorAndTurnOffMemoryLeakDetectionInDestructor(bool des)
{
    destroyGlobalDetectorAndTurnOfMemoryLeakDetectionInDestructor_ = des;
}

void MemoryLeakWarningPlugin::setGlobalDetector(MemoryLeakDetector* detector, MemoryLeakFailure* reporter)
{
    globalDetector = detector;
    globalReporter = reporter;
}

void MemoryLeakWarningPlugin::destroyGlobalDetector()
{
    turnOffNewDeleteOverloads();
    delete globalDetector;
    delete globalReporter;
    globalDetector = NULL;
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
    TestPlugin(name), ignoreAllWarnings_(false), destroyGlobalDetectorAndTurnOfMemoryLeakDetectionInDestructor_(false), expectedLeaks_(0)
{
    if (firstPlugin_ == 0) firstPlugin_ = this;

    if (localDetector) memLeakDetector_ = localDetector;
    else memLeakDetector_ = getGlobalDetector();

    memLeakDetector_->enable();
}

MemoryLeakWarningPlugin::~MemoryLeakWarningPlugin()
{
    if (destroyGlobalDetectorAndTurnOfMemoryLeakDetectionInDestructor_) {
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
        MemoryLeakWarningPlugin::destroyGlobalDetector();
    }
}

void MemoryLeakWarningPlugin::preTestAction(UtestShell& /*test*/, TestResult& result)
{
    memLeakDetector_->startChecking();
    failureCount_ = result.getFailureCount();
}

void MemoryLeakWarningPlugin::postTestAction(UtestShell& test, TestResult& result)
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


