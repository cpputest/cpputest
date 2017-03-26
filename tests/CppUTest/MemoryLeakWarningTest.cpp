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

static char* leak1;
static long* leak2;

class DummyReporter: public MemoryLeakFailure
{
public:
    virtual ~DummyReporter()
    {
    }
    virtual void fail(char* /*fail_string*/)
    {
    }
};

static MemoryLeakDetector* detector;
static MemoryLeakWarningPlugin* memPlugin;
static DummyReporter dummy;
static TestMemoryAllocator* allocator;

TEST_GROUP(MemoryLeakWarningLocalDetectorTest)
{
};

TEST(MemoryLeakWarningLocalDetectorTest, localDetectorReturnsNewGlobalWhenNoneWasSet)
{
    MemoryLeakWarningPlugin memoryLeakWarningPlugin("TestMemoryLeakWarningPlugin", NULL);
    CHECK(0 != memoryLeakWarningPlugin.getMemoryLeakDetector());
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
    MemoryLeakWarningPlugin memoryLeakWarningPlugin("TestMemoryLeakWarningPlugin", NULL);
    MemoryLeakDetector* localDetector =  memoryLeakWarningPlugin.getMemoryLeakDetector();
    POINTERS_EQUAL(globalDetector, localDetector);
}


TEST_GROUP(MemoryLeakWarningTest)
{
    TestTestingFixture* fixture;

    void setup()
    {
        fixture = new TestTestingFixture();
        detector = new MemoryLeakDetector(&dummy);
        allocator = new TestMemoryAllocator;
        memPlugin = new MemoryLeakWarningPlugin("TestMemoryLeakWarningPlugin", detector);
        fixture->registry_->installPlugin(memPlugin);
        memPlugin->enable();

        leak1 = 0;
        leak2 = 0;
    }
    void teardown()
    {
        detector->deallocMemory(allocator, leak1);
        detector->deallocMemory(allocator, leak2);

        delete fixture;
        delete memPlugin;
        delete detector;
        delete allocator;
    }
};

static void _testTwoLeaks()
{
    leak1 = detector->allocMemory(allocator, 10);
    leak2 = (long*) (void*) detector->allocMemory(allocator, 4);
}

TEST(MemoryLeakWarningTest, TwoLeaks)
{
    fixture->setTestFunction(_testTwoLeaks);
    fixture->runAllTests();
    LONGS_EQUAL(1, fixture->getFailureCount());
    fixture->assertPrintContains("Total number of leaks:  2");
}

static void _testIgnore2()
{
    memPlugin->expectLeaksInTest(2);
    leak1 = detector->allocMemory(allocator, 10);
    leak2 = (long*) (void*) detector->allocMemory(allocator, 4);
}

TEST(MemoryLeakWarningTest, Ignore2)
{
    fixture->setTestFunction(_testIgnore2);
    fixture->runAllTests();
    LONGS_EQUAL(0, fixture->getFailureCount());
}

static void _failAndLeakMemory()
{
    leak1 = detector->allocMemory(allocator, 10);
    FAIL("");
}

TEST(MemoryLeakWarningTest, FailingTestDoesNotReportMemoryLeaks)
{
    fixture->setTestFunction(_failAndLeakMemory);
    fixture->runAllTests();
    LONGS_EQUAL(1, fixture->getFailureCount());
}

static bool memoryLeakDetectorWasDeleted = false;
static bool memoryLeakFailureWasDelete = false;

class DummyMemoryLeakDetector : public MemoryLeakDetector
{
public:
    DummyMemoryLeakDetector(MemoryLeakFailure* reporter) : MemoryLeakDetector(reporter) {}
    virtual ~DummyMemoryLeakDetector()
    {
        memoryLeakDetectorWasDeleted = true;
    }
};

class DummyMemoryLeakFailure : public MemoryLeakFailure
{
    virtual ~DummyMemoryLeakFailure()
    {
        memoryLeakFailureWasDelete = true;
    }
    virtual void fail(char*)
    {
    }
};


static bool cpputestHasCrashed;

TEST_GROUP(MemoryLeakWarningGlobalDetectorTest)
{
    MemoryLeakDetector* detector;
    MemoryLeakFailure* failureReporter;

    DummyMemoryLeakDetector * dummyDetector;
    MemoryLeakFailure* dummyReporter;

    static void crashMethod()
    {
        cpputestHasCrashed = true;
    }

    void setup()
    {
        detector = MemoryLeakWarningPlugin::getGlobalDetector();
        failureReporter = MemoryLeakWarningPlugin::getGlobalFailureReporter();

        memoryLeakDetectorWasDeleted = false;
        memoryLeakFailureWasDelete = false;

        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();

        dummyReporter = new DummyMemoryLeakFailure;
        dummyDetector = new DummyMemoryLeakDetector(dummyReporter);

        UtestShell::setCrashMethod(crashMethod);
        cpputestHasCrashed = false;
}

    void teardown()
    {
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
        if (!memoryLeakDetectorWasDeleted) delete dummyDetector;
        if (!memoryLeakFailureWasDelete) delete dummyReporter;

        MemoryLeakWarningPlugin::setGlobalDetector(detector, failureReporter);
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();

        UtestShell::resetCrashMethod();

        setCurrentMallocAllocatorToDefault();
        setCurrentNewAllocatorToDefault();
        setCurrentNewArrayAllocatorToDefault();
    }
};

TEST(MemoryLeakWarningGlobalDetectorTest, turnOffNewOverloadsCausesNoAdditionalLeaks)
{
    int storedAmountOfLeaks = detector->totalMemoryLeaks(mem_leak_period_all);

    char* arrayMemory = new char[100];
    char* nonArrayMemory = new char;
    char* mallocMemory = (char*) cpputest_malloc_location_with_leak_detection(10, "file", 10);
    char* reallocMemory = (char*) cpputest_realloc_location_with_leak_detection(NULL, 10, "file", 10);

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

    CHECK(memoryLeakDetectorWasDeleted);
    CHECK(memoryLeakFailureWasDelete);
}

TEST(MemoryLeakWarningGlobalDetectorTest, MemoryWarningPluginCanBeSetToDestroyTheGlobalDetector)
{
    MemoryLeakWarningPlugin* plugin = new MemoryLeakWarningPlugin("dummy");
    plugin->destroyGlobalDetectorAndTurnOffMemoryLeakDetectionInDestructor(true);
    MemoryLeakWarningPlugin::setGlobalDetector(dummyDetector, dummyReporter);

    delete plugin;

    CHECK(memoryLeakDetectorWasDeleted);
}

#ifndef CPPUTEST_MEM_LEAK_DETECTION_DISABLED

TEST(MemoryLeakWarningGlobalDetectorTest, crashOnLeakWithOperatorNew)
{
    MemoryLeakWarningPlugin::setGlobalDetector(dummyDetector, dummyReporter);

    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();

    crash_on_allocation_number(1);
    char* memory = new char[100];
    CHECK(cpputestHasCrashed);
    delete [] memory;
}

TEST(MemoryLeakWarningGlobalDetectorTest, crashOnLeakWithOperatorNewArray)
{
    MemoryLeakWarningPlugin::setGlobalDetector(dummyDetector, dummyReporter);

    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();

    crash_on_allocation_number(1);
    char* memory = new char;
    CHECK(cpputestHasCrashed);
    delete memory;
}

TEST(MemoryLeakWarningGlobalDetectorTest, crashOnLeakWithOperatorMalloc)
{
    MemoryLeakWarningPlugin::setGlobalDetector(dummyDetector, dummyReporter);

    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();

    crash_on_allocation_number(1);
    char* memory = (char*) cpputest_malloc(10);
    CHECK(cpputestHasCrashed);
    cpputest_free(memory);
}

TEST(MemoryLeakWarningGlobalDetectorTest, gettingTheGlobalDetectorDoesNotRestoreTheMemoryLeakOverloadsWhenTheyWereAlreadyOff)
{
    MemoryLeakWarningPlugin::setGlobalDetector(NULL, NULL);
    MemoryLeakDetector* temporaryDetector = MemoryLeakWarningPlugin::getGlobalDetector();
    MemoryLeakFailure*  temporaryReporter = MemoryLeakWarningPlugin::getGlobalFailureReporter();

    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
    bool areNewDeleteOverloaded = MemoryLeakWarningPlugin::areNewDeleteOverloaded();
    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();

    CHECK(!areNewDeleteOverloaded);

    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
    delete temporaryReporter;
    delete temporaryDetector;
}

TEST(MemoryLeakWarningGlobalDetectorTest, checkIfTheMemoryLeakOverloadsAreOn)
{
    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    CHECK(MemoryLeakWarningPlugin::areNewDeleteOverloaded());

    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
}

TEST(MemoryLeakWarningGlobalDetectorTest, checkIfTheMemoryLeakOverloadsAreOff)
{
    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
    bool areNewDeleteOverloaded = MemoryLeakWarningPlugin::areNewDeleteOverloaded();

    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    CHECK(!areNewDeleteOverloaded);
}


#endif

#if CPPUTEST_USE_STD_CPP_LIB

TEST(MemoryLeakWarningGlobalDetectorTest, turnOffNewOverloadsNoThrowCausesNoAdditionalLeaks)
{
#undef new
    int storedAmountOfLeaks = detector->totalMemoryLeaks(mem_leak_period_all);

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

#ifndef CPPUTEST_MEM_LEAK_DETECTION_DISABLED

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
    void setup()
    {
        UT_PTR_SET(PlatformSpecificMutexLock, StubMutexLock);
        UT_PTR_SET(PlatformSpecificMutexUnlock, StubMutexUnlock);

        mutexLockCount = 0;
        mutexUnlockCount = 0;
    }

    void teardown()
    {
    }
};

TEST(MemoryLeakWarningThreadSafe, turnOnThreadSafeMallocFreeReallocOverloadsDebug)
{
    int storedAmountOfLeaks = MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all);

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

    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
}

TEST(MemoryLeakWarningThreadSafe, turnOnThreadSafeNewDeleteOverloadsDebug)
{
    int storedAmountOfLeaks = MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all);

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

    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
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

    int storedAmountOfLeaks = MemoryLeakWarningPlugin::getGlobalDetector()->totalMemoryLeaks(mem_leak_period_all);
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

    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
#ifdef CPPUTEST_USE_NEW_MACROS
    #include "CppUTest/MemoryLeakDetectorNewMacros.h"
#endif
}

#endif

#endif

#endif
