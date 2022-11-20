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
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/MemoryLeakWarningPlugin.h"
#include "CppUTest/MemoryLeakDetector.h"
#include "CppUTest/TestMemoryAllocator.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTest/SimpleMutex.h"
#include "DummyMemoryLeakDetector.h"

TEST_GROUP(MemoryLeakWarningLocalDetectorTest)
{
    DummyMemoryLeakFailure dummy;
};

TEST(MemoryLeakWarningLocalDetectorTest, localDetectorReturnsNewGlobalWhenNoneWasSet)
{
    MemoryLeakWarningPlugin memoryLeakWarningPlugin("TestMemoryLeakWarningPlugin", NULLPTR);
    CHECK(NULLPTR != memoryLeakWarningPlugin.getMemoryLeakDetector());
}

TEST(MemoryLeakWarningLocalDetectorTest, localDetectorIsTheOneSpecifiedInConstructor)
{
    MemoryLeakDetector localDetector(&dummy);
    MemoryLeakWarningPlugin memoryLeakWarningPlugin("TestMemoryLeakWarningPlugin", &localDetector);
    POINTERS_EQUAL(&localDetector, memoryLeakWarningPlugin.getMemoryLeakDetector());
}

TEST(MemoryLeakWarningLocalDetectorTest, localDetectorIsGlobalDetector)
{
    MemoryLeakDetector* globalDetector = MemoryLeakWarningPlugin::getGlobalDetector();
    MemoryLeakWarningPlugin memoryLeakWarningPlugin("TestMemoryLeakWarningPlugin", NULLPTR);
    MemoryLeakDetector* localDetector =  memoryLeakWarningPlugin.getMemoryLeakDetector();
    POINTERS_EQUAL(globalDetector, localDetector);
}

static char* leak1;
static long* leak2;

static MemoryLeakDetector* detector;
static MemoryLeakWarningPlugin* memPlugin;
static TestMemoryAllocator* allocator;

TEST_GROUP(MemoryLeakWarningTest)
{
    DummyMemoryLeakFailure dummy;
    TestTestingFixture* fixture;

    void setup() _override
    {
        fixture = new TestTestingFixture();
        detector = new MemoryLeakDetector(&dummy);
        allocator = new TestMemoryAllocator;
        memPlugin = new MemoryLeakWarningPlugin("TestMemoryLeakWarningPlugin", detector);
        fixture->installPlugin(memPlugin);
        memPlugin->enable();

        leak1 = NULLPTR;
        leak2 = NULLPTR;
    }

    void teardown() _override
    {
        detector->deallocMemory(allocator, leak1);
        detector->deallocMemory(allocator, leak2);

        delete fixture;
        delete memPlugin;
        delete detector;
        delete allocator;
    }
};

static void testTwoLeaks_()
{
    leak1 = detector->allocMemory(allocator, 10);
    leak2 = (long*) (void*) detector->allocMemory(allocator, 4);
}

#if CPPUTEST_USE_MEM_LEAK_DETECTION

TEST(MemoryLeakWarningTest, TwoLeaks)
{
    fixture->setTestFunction(testTwoLeaks_);
    fixture->runAllTests();

    LONGS_EQUAL(1, fixture->getFailureCount());
}

#else

TEST(MemoryLeakWarningTest, TwoLeaks)
{
    fixture->setTestFunction(testTwoLeaks_);
    fixture->runAllTests();

    LONGS_EQUAL(0, fixture->getFailureCount());
}

#endif


static void testLeakWarningWithPluginDisabled_()
{
    memPlugin->expectLeaksInTest(1);
    leak1 = (char*) cpputest_malloc_location_with_leak_detection(10, __FILE__, __LINE__);
}

TEST(MemoryLeakWarningTest, LeakWarningWithPluginDisabled)
{
    fixture->setTestFunction(testLeakWarningWithPluginDisabled_);

    MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();

    fixture->runAllTests();

    LONGS_EQUAL(0, fixture->getFailureCount());
    fixture->assertPrintContains("Warning: Expected 1 leak(s), but leak detection was disabled");

    cpputest_free_location_with_leak_detection(leak1, __FILE__, __LINE__);
    leak1 = NULLPTR;

    MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
}

static void testIgnore2_()
{
    memPlugin->expectLeaksInTest(2);
    leak1 = detector->allocMemory(allocator, 10);
    leak2 = (long*) (void*) detector->allocMemory(allocator, 4);
}

TEST(MemoryLeakWarningTest, Ignore2)
{
    fixture->setTestFunction(testIgnore2_);
    fixture->runAllTests();
    LONGS_EQUAL(0, fixture->getFailureCount());
}

static void failAndLeakMemory_()
{
    leak1 = detector->allocMemory(allocator, 10);
    FAIL("");
}

TEST(MemoryLeakWarningTest, FailingTestDoesNotReportMemoryLeaks)
{
    fixture->setTestFunction(failAndLeakMemory_);
    fixture->runAllTests();
    LONGS_EQUAL(1, fixture->getFailureCount());
}

static bool cpputestHasCrashed;

TEST_GROUP(MemoryLeakWarningGlobalDetectorTest)
{
    MemoryLeakDetector* detector;
    MemoryLeakFailure* failureReporter;

    DummyMemoryLeakDetector * dummyDetector;
    MemoryLeakFailure* dummyReporter;

    GlobalMemoryAllocatorStash memoryAllocatorStash;

    static void crashMethod()
    {
        cpputestHasCrashed = true;
    }

    void setup() _override
    {
        memoryAllocatorStash.save();
        detector = MemoryLeakWarningPlugin::getGlobalDetector();
        failureReporter = MemoryLeakWarningPlugin::getGlobalFailureReporter();

        MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();

        dummyReporter = new DummyMemoryLeakFailure;
        dummyDetector = new DummyMemoryLeakDetector(dummyReporter);

        UtestShell::setCrashMethod(crashMethod);
        cpputestHasCrashed = false;
}

    void teardown() _override
    {
        MemoryLeakWarningPlugin::restoreNewDeleteOverloads();

        MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();

        if (!DummyMemoryLeakDetector::wasDeleted()) delete dummyDetector;
        if (!DummyMemoryLeakFailure::wasDeleted()) delete dummyReporter;
        MemoryLeakWarningPlugin::setGlobalDetector(detector, failureReporter);

        MemoryLeakWarningPlugin::restoreNewDeleteOverloads();

        UtestShell::resetCrashMethod();

        memoryAllocatorStash.restore();
    }
};

TEST(MemoryLeakWarningGlobalDetectorTest, turnOffNewOverloadsCausesNoAdditionalLeaks)
{
    size_t storedAmountOfLeaks = detector->totalMemoryLeaks(mem_leak_period_all);

    char* arrayMemory = new char[100];
    char* nonArrayMemory = new char;
    char* mallocMemory = (char*) cpputest_malloc_location_with_leak_detection(10, "file", 10);
    char* reallocMemory = (char*) cpputest_realloc_location_with_leak_detection(NULLPTR, 10, "file", 10);

    LONGS_EQUAL(storedAmountOfLeaks, detector->totalMemoryLeaks(mem_leak_period_all));

    cpputest_free_location_with_leak_detection(mallocMemory, "file", 10);
    cpputest_free_location_with_leak_detection(reallocMemory, "file", 10);
    delete [] arrayMemory;
    delete nonArrayMemory;
}

TEST(MemoryLeakWarningGlobalDetectorTest, destroyGlobalDetector)
{
    MemoryLeakWarningPlugin::setGlobalDetector(dummyDetector, dummyReporter);

    MemoryLeakWarningPlugin::destroyGlobalDetector();

    CHECK(DummyMemoryLeakDetector::wasDeleted());
    CHECK(DummyMemoryLeakFailure::wasDeleted());
}

TEST(MemoryLeakWarningGlobalDetectorTest, MemoryWarningPluginCanBeSetToDestroyTheGlobalDetector)
{
    MemoryLeakWarningPlugin* plugin = new MemoryLeakWarningPlugin("dummy");
    plugin->destroyGlobalDetectorAndTurnOffMemoryLeakDetectionInDestructor(true);
    MemoryLeakWarningPlugin::setGlobalDetector(dummyDetector, dummyReporter);

    delete plugin;

    CHECK(DummyMemoryLeakDetector::wasDeleted());
}

#if CPPUTEST_USE_MEM_LEAK_DETECTION

TEST(MemoryLeakWarningGlobalDetectorTest, crashOnLeakWithOperatorNew)
{
    MemoryLeakWarningPlugin::setGlobalDetector(dummyDetector, dummyReporter);

    MemoryLeakWarningPlugin::restoreNewDeleteOverloads();

    crash_on_allocation_number(1);
    char* memory = new char[100];
    CHECK(cpputestHasCrashed);
    delete [] memory;
    MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
}

TEST(MemoryLeakWarningGlobalDetectorTest, crashOnLeakWithOperatorNewArray)
{
    MemoryLeakWarningPlugin::setGlobalDetector(dummyDetector, dummyReporter);

    MemoryLeakWarningPlugin::restoreNewDeleteOverloads();

    crash_on_allocation_number(1);
    char* memory = new char;
    CHECK(cpputestHasCrashed);
    delete memory;

    MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
}

TEST(MemoryLeakWarningGlobalDetectorTest, crashOnLeakWithOperatorMalloc)
{
    MemoryLeakWarningPlugin::setGlobalDetector(dummyDetector, dummyReporter);

    MemoryLeakWarningPlugin::restoreNewDeleteOverloads();

    crash_on_allocation_number(1);
    char* memory = (char*) cpputest_malloc(10);
    CHECK(cpputestHasCrashed);
    cpputest_free(memory);

    MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
}

TEST(MemoryLeakWarningGlobalDetectorTest, gettingTheGlobalDetectorDoesNotRestoreTheMemoryLeakOverloadsWhenTheyWereAlreadyOff)
{
    MemoryLeakWarningPlugin::setGlobalDetector(NULLPTR, NULLPTR);
    MemoryLeakDetector* temporaryDetector = MemoryLeakWarningPlugin::getGlobalDetector();
    MemoryLeakFailure*  temporaryReporter = MemoryLeakWarningPlugin::getGlobalFailureReporter();

    MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();

    bool areNewDeleteOverloaded = MemoryLeakWarningPlugin::areNewDeleteOverloaded();
    MemoryLeakWarningPlugin::restoreNewDeleteOverloads();

    CHECK(!areNewDeleteOverloaded);

    delete temporaryReporter;
    delete temporaryDetector;
    MemoryLeakWarningPlugin::setGlobalDetector(NULLPTR, NULLPTR);
}

TEST(MemoryLeakWarningGlobalDetectorTest, checkIfTheMemoryLeakOverloadsAreOn)
{
    MemoryLeakWarningPlugin::turnOnDefaultNotThreadSafeNewDeleteOverloads();
    CHECK(MemoryLeakWarningPlugin::areNewDeleteOverloaded());

    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
}

TEST(MemoryLeakWarningGlobalDetectorTest, checkIfTheMemoryLeakOverloadsAreOff)
{
    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
    bool areNewDeleteOverloaded = MemoryLeakWarningPlugin::areNewDeleteOverloaded();

    MemoryLeakWarningPlugin::turnOnDefaultNotThreadSafeNewDeleteOverloads();
    CHECK(!areNewDeleteOverloaded);
}

TEST(MemoryLeakWarningGlobalDetectorTest, checkIfTheMemoryLeakOverloadsAreOnWithRestore)
{
    MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
    CHECK(MemoryLeakWarningPlugin::areNewDeleteOverloaded());
    MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
}

TEST(MemoryLeakWarningGlobalDetectorTest, checkIfTheMemoryLeakOverloadsAreOffWithSaveDisable)
{
    MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
    CHECK(!MemoryLeakWarningPlugin::areNewDeleteOverloaded());
    MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
}

TEST(MemoryLeakWarningGlobalDetectorTest, threadSafeMemoryLeakDetectorOverloadsAreAlsoOverloaded)
{
    MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
    MemoryLeakWarningPlugin::turnOnThreadSafeNewDeleteOverloads();
    CHECK(MemoryLeakWarningPlugin::areNewDeleteOverloaded());
    MemoryLeakWarningPlugin::turnOnDefaultNotThreadSafeNewDeleteOverloads();
    MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
}

#endif

#if CPPUTEST_USE_STD_CPP_LIB

TEST(MemoryLeakWarningGlobalDetectorTest, turnOffNewOverloadsNoThrowCausesNoAdditionalLeaks)
{
#undef new
    size_t storedAmountOfLeaks = detector->totalMemoryLeaks(mem_leak_period_all);

    char* nonMemoryNoThrow = new (std::nothrow) char;
    char* nonArrayMemoryNoThrow = new (std::nothrow) char[10];
    char* nonArrayMemoryThrow = new char[10];

    LONGS_EQUAL(storedAmountOfLeaks, detector->totalMemoryLeaks(mem_leak_period_all));

    ::operator delete(nonMemoryNoThrow, std::nothrow);
    ::operator delete[](nonArrayMemoryNoThrow, std::nothrow);
    ::operator delete[](nonArrayMemoryThrow, std::nothrow);
#ifdef CPPUTEST_USE_NEW_MACROS
    #include "CppUTest/MemoryLeakDetectorNewMacros.h"
#endif
}

#if CPPUTEST_USE_MEM_LEAK_DETECTION

static int mutexLockCount = 0;
static int mutexUnlockCount = 0;

static void StubMutexLock(PlatformSpecificMutex)
{
    mutexLockCount++;
}

static void StubMutexUnlock(PlatformSpecificMutex)
{
    mutexUnlockCount++;
}

TEST_GROUP(MemoryLeakWarningThreadSafe)
{
    void setup() _override
    {
        UT_PTR_SET(PlatformSpecificMutexLock, StubMutexLock);
        UT_PTR_SET(PlatformSpecificMutexUnlock, StubMutexUnlock);

        mutexLockCount = 0;
        mutexUnlockCount = 0;
    }

    void teardown() _override
    {
    }
};

TEST(MemoryLeakWarningThreadSafe, turnOnThreadSafeMallocFreeReallocOverloadsDebug)
{
    size_t storedAmountOfLeaks = MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all);

    MemoryLeakWarningPlugin::turnOnThreadSafeNewDeleteOverloads();

    int *n = (int*) cpputest_malloc(sizeof(int));

    LONGS_EQUAL(storedAmountOfLeaks + 1, MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all));
    CHECK_EQUAL(1, mutexLockCount);
    CHECK_EQUAL(1, mutexUnlockCount);

    n = (int*) cpputest_realloc(n, sizeof(int)*3);

    LONGS_EQUAL(storedAmountOfLeaks + 1, MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all));
    CHECK_EQUAL(2, mutexLockCount);
    CHECK_EQUAL(2, mutexUnlockCount);

    cpputest_free(n);

    LONGS_EQUAL(storedAmountOfLeaks, MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all));
    CHECK_EQUAL(3, mutexLockCount);
    CHECK_EQUAL(3, mutexUnlockCount);

    MemoryLeakWarningPlugin::turnOnDefaultNotThreadSafeNewDeleteOverloads();
}

TEST(MemoryLeakWarningThreadSafe, turnOnThreadSafeNewDeleteOverloadsDebug)
{
    size_t storedAmountOfLeaks = MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all);

    MemoryLeakWarningPlugin::turnOnThreadSafeNewDeleteOverloads();

    int *n = new int;
    char *str = new char[20];

    LONGS_EQUAL(storedAmountOfLeaks + 2, MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all));
    CHECK_EQUAL(2, mutexLockCount);
    CHECK_EQUAL(2, mutexUnlockCount);

    delete [] str;
    delete n;

    LONGS_EQUAL(storedAmountOfLeaks, MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all));
    CHECK_EQUAL(4, mutexLockCount);
    CHECK_EQUAL(4, mutexUnlockCount);

    MemoryLeakWarningPlugin::turnOnDefaultNotThreadSafeNewDeleteOverloads();
}

#ifdef __clang__

IGNORE_TEST(MemoryLeakWarningThreadSafe, turnOnThreadSafeNewDeleteOverloads)
{
    /*  Clang misbehaves with -O2 - it will not overload operator new or
     *  operator new[] no matter what. Therefore, this test is must be ignored.
     */
}

#else

TEST(MemoryLeakWarningThreadSafe, turnOnThreadSafeNewDeleteOverloads)
{
#undef new

    size_t storedAmountOfLeaks = MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all);
    MemoryLeakWarningPlugin::turnOnThreadSafeNewDeleteOverloads();

    int *n = new int;
    int *n_nothrow = new (std::nothrow) int;
    char *str = new char[20];
    char *str_nothrow = new (std::nothrow) char[20];

    LONGS_EQUAL(storedAmountOfLeaks + 4, MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all));
    CHECK_EQUAL(4, mutexLockCount);
    CHECK_EQUAL(4, mutexUnlockCount);

    delete [] str_nothrow;
    delete [] str;
    delete n;
    delete n_nothrow;

    LONGS_EQUAL(storedAmountOfLeaks, MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all));
    CHECK_EQUAL(8, mutexLockCount);
    CHECK_EQUAL(8, mutexUnlockCount);

    MemoryLeakWarningPlugin::turnOnDefaultNotThreadSafeNewDeleteOverloads();
#ifdef CPPUTEST_USE_NEW_MACROS
    #include "CppUTest/MemoryLeakDetectorNewMacros.h"
#endif
}

#endif

#endif

#endif
