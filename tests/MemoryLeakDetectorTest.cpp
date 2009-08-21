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
#include "CppUTest/MemoryLeakDetector.h"
#include "CppUTest/PlatformSpecificFunctions.h"

class MemoryLeakFailureForTest : public MemoryLeakFailure
{
public:
   virtual ~MemoryLeakFailureForTest(){};
   virtual void fail(char* fail_string) {*message = fail_string;}

   SimpleString *message;
};

TEST_GROUP(MemoryLeakDetectorTest)
{
   MemoryLeakDetector* detector;
   MemoryLeakFailureForTest *reporter;

   void setup()
   {
	  detector = new MemoryLeakDetector;
	  reporter = new MemoryLeakFailureForTest;
      detector->init(reporter);
      detector->enable();
      detector->startChecking();
      reporter->message = new SimpleString();
   }
   void teardown()
   {
	  delete reporter->message;
	  delete detector;
	  delete reporter;
   }
};


TEST(MemoryLeakDetectorTest, OneLeak)
{
   char* mem = detector->allocOperatorNew(3);
   detector->stopChecking();
   SimpleString output = detector->report(mem_leak_period_checking);
   CHECK(output.contains(MEM_LEAK_HEADER));
   CHECK(output.contains("size: 3"));
   CHECK(output.contains("new"));
   CHECK(output.contains(MEM_LEAK_FOOTER));
   PlatformSpecificFree(mem);
}

TEST(MemoryLeakDetectorTest, OneLeakOutsideCheckingPeriod)
{
   detector->stopChecking();
   char* mem = detector->allocOperatorNew(4);
   SimpleString output = detector->report(mem_leak_period_all);
   CHECK(output.contains(MEM_LEAK_HEADER));
   CHECK(output.contains("size: 4"));
   CHECK(output.contains("new"));
   CHECK(output.contains(MEM_LEAK_FOOTER));
   PlatformSpecificFree(mem);
}

TEST(MemoryLeakDetectorTest, NoLeaksWhatsoever)
{
   detector->stopChecking();
   STRCMP_EQUAL(MEM_LEAK_NONE, detector->report(mem_leak_period_checking));
   STRCMP_EQUAL(MEM_LEAK_NONE, detector->report(mem_leak_period_all));
}


TEST(MemoryLeakDetectorTest, TwoLeaksUsingOperatorNew)
{
   char* mem = detector->allocOperatorNew(4);
   char* mem2 = detector->allocOperatorNew(8);
   detector->stopChecking();
   SimpleString output = detector->report(mem_leak_period_checking);
   LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_checking));
   CHECK(output.contains("size: 8"));
   CHECK(output.contains("size: 4"));
   PlatformSpecificFree(mem);
   PlatformSpecificFree(mem2);
}

TEST(MemoryLeakDetectorTest, OneAllocButNoLeak)
{
   char* mem = detector->allocOperatorNew(4);
   detector->freeOperatorDelete(mem);
   detector->stopChecking();
   STRCMP_EQUAL(MEM_LEAK_NONE, detector->report(mem_leak_period_checking));
}

TEST(MemoryLeakDetectorTest, TwoAllocOneFreeOneLeak)
{
   char* mem = detector->allocOperatorNew(4);
   char* mem2 = detector->allocOperatorNew(12);
   detector->freeOperatorDelete(mem);
   detector->stopChecking();
   SimpleString output = detector->report(mem_leak_period_checking);
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
   CHECK(output.contains("size: 12"));
   CHECK(!output.contains("size: 4"));
   PlatformSpecificFree (mem2);
}

TEST(MemoryLeakDetectorTest, TwoAllocOneFreeOneLeakReverseOrder)
{
   char* mem = detector->allocOperatorNew(4);
   char* mem2 = detector->allocOperatorNew(12);
   detector->freeOperatorDelete(mem2);
   detector->stopChecking();
   SimpleString output = detector->report(mem_leak_period_checking);
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
   CHECK(!output.contains("size: 12"));
   CHECK(output.contains("size: 4"));
   PlatformSpecificFree (mem);
}

TEST(MemoryLeakDetectorTest, DeleteNonAlocatedMemory)
{
   char a;
   char* pa = &a;
   detector->freeFree(pa, "FREE.c", 100);
   detector->stopChecking();
   CHECK(reporter->message->contains(MEM_LEAK_DEALLOC_NON_ALLOCATED));
   CHECK(reporter->message->contains("   allocated at file: <unknown> line: 0 size: 0 type: unknown"));
   CHECK(reporter->message->contains("   dealllocated at file: FREE.c line: 100 type: malloc"));
}

TEST(MemoryLeakDetectorTest, IgnoreMemoryAllocatedOutsideCheckingPeriod)
{
   detector->stopChecking();
   char* mem = detector->allocOperatorNew(4);
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));
   detector->freeOperatorDelete(mem);
}

TEST(MemoryLeakDetectorTest, IgnoreMemoryAllocatedOutsideCheckingPeriodComplicatedCase)
{
   char* mem = detector->allocOperatorNew(4);
   detector->stopChecking();

   char* mem2 = detector->allocOperatorNew(8);
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
   detector->clearAllAccounting(mem_leak_period_checking);
   PlatformSpecificFree(mem);
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));

   detector->startChecking();
   char* mem3 = detector->allocOperatorNew(4);
   detector->stopChecking();
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
   LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_all));

   detector->clearAllAccounting(mem_leak_period_checking);
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));

   detector->clearAllAccounting(mem_leak_period_all);
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_all));
   PlatformSpecificFree(mem2);
   PlatformSpecificFree(mem3);
}

TEST(MemoryLeakDetectorTest, OneLeakUsingOperatorNewWithFileLine)
{
   char* mem = detector->allocOperatorNew(4, "file.cpp", 1234);
   detector->stopChecking();
   SimpleString output = detector->report(mem_leak_period_checking);
   CHECK(output.contains("file.cpp"));
   CHECK(output.contains("1234"));
   PlatformSpecificFree(mem);
}

TEST(MemoryLeakDetectorTest, OneAllocAndFreeUsingArrayNew)
{
   char* mem = detector->allocOperatorNewArray(10, "file.cpp", 1234);
   char* mem2 = detector->allocOperatorNewArray(12);
   LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_all));
   SimpleString output = detector->report(mem_leak_period_checking);
   CHECK(output.contains("new []"));
   detector->freeOperatorDeleteArray(mem);
   detector->freeOperatorDeleteArray(mem2);
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_all));
   detector->stopChecking();
}

TEST(MemoryLeakDetectorTest, OneAllocAndFreeUsingMalloc)
{
   char* mem = detector->allocMalloc(10, "file.cpp", 1234);
   char* mem2 = detector->allocMalloc(12);
   LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_checking));
   SimpleString output = detector->report(mem_leak_period_checking);
   CHECK(output.contains("malloc"));
   detector->freeFree(mem);
   detector->freeFree(mem2, "file.c", 5678);
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_all));
   detector->stopChecking();
}

TEST(MemoryLeakDetectorTest, OneRealloc)
{
   char* mem1 = detector->allocMalloc(10, "file.cpp", 1234);

   char* mem2 = detector->allocRealloc(mem1, 1000, "other.cpp", 5678);

   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
   SimpleString output = detector->report(mem_leak_period_checking);
   CHECK(output.contains("other.cpp"));

   detector->freeFree(mem2);
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_all));
   detector->stopChecking();
}

TEST(MemoryLeakDetectorTest, AllocOneTypeFreeAnotherType)
{
   char* mem = detector->allocOperatorNewArray(100, "ALLOC.c", 10);
   detector->freeFree(mem, "FREE.c", 100);
   detector->stopChecking();
   CHECK(reporter->message->contains(MEM_LEAK_ALLOC_DEALLOC_MISMATCH));
   CHECK(reporter->message->contains("   allocated at file: ALLOC.c line: 10 size: 100 type: new []"));
   CHECK(reporter->message->contains("   dealllocated at file: FREE.c line: 100 type: malloc"));
}

TEST(MemoryLeakDetectorTest, AllocOneTypeFreeAnotherTypeWithCheckingDisabled)
{
   detector->disableAllocationTypeChecking ();
   char* mem = detector->allocOperatorNewArray(100, "ALLOC.c", 10);
   detector->freeFree(mem, "FREE.c", 100);
   detector->stopChecking();
   STRCMP_EQUAL("", reporter->message->asCharString());
   detector->enableAllocationTypeChecking ();
}

TEST(MemoryLeakDetectorTest, MarkCheckingPeriodLeaksAsNonCheckingPeriod)
{
   char* mem = detector->allocOperatorNewArray(100);
   char* mem2 = detector->allocOperatorNewArray(100);
   detector->stopChecking();
   LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_checking));
   LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_all));
   detector->markCheckingPeriodLeaksAsNonCheckingPeriod();
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
   LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_all));
   PlatformSpecificFree(mem);
   PlatformSpecificFree(mem2);
}

TEST(MemoryLeakDetectorTest, memoryCorruption)
{
   char* mem = detector->allocMalloc(10, "ALLOC.c", 10);
   mem[10] = 'O';
   mem[11] = 'H';
   detector->freeFree(mem, "FREE.c", 100);
   detector->stopChecking();
   CHECK(reporter->message->contains(MEM_LEAK_MEMORY_CORRUPTION));
   CHECK(reporter->message->contains("   allocated at file: ALLOC.c line: 10 size: 10 type: malloc"));
   CHECK(reporter->message->contains("   dealllocated at file: FREE.c line: 100 type: malloc"));
}

TEST(MemoryLeakDetectorTest, safelyDeleteNULL)
{
   detector->freeOperatorDelete(0);
   STRCMP_EQUAL("", reporter->message->asCharString());
}

TEST(MemoryLeakDetectorTest, periodDisabled)
{
   detector->disable();
   char* mem = detector->allocMalloc(2);
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_disabled));
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_enabled));
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
   detector->freeFree(mem);
}

TEST(MemoryLeakDetectorTest, periodEnabled)
{
   detector->enable();
   char* mem = detector->allocMalloc(2);
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_disabled));
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_enabled));
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
   detector->freeFree(mem);
}

TEST(MemoryLeakDetectorTest, periodChecking)
{
   char* mem = detector->allocMalloc(2);
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));
   LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_disabled));
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_enabled));
   LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
   detector->freeFree(mem);
}

