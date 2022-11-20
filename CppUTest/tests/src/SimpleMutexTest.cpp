/*
 * Copyright (c) 2014,  Michael Feathers, James Grenning, Bas Vodde and Chen YewMing
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
#include "CppUTest/SimpleMutex.h"
#include "CppUTest/PlatformSpecificFunctions.h"

static int mutexCreateCount = 0;
static int mutexLockCount = 0;
static int mutexUnlockCount = 0;
static int mutexDestroyCount = 0;

static PlatformSpecificMutex StubMutexCreate(void)
{
    mutexCreateCount++;
    return NULLPTR;
}

static void StubMutexLock(PlatformSpecificMutex)
{
    mutexLockCount++;
}

static void StubMutexUnlock(PlatformSpecificMutex)
{
    mutexUnlockCount++;
}

static void StubMutexDestroy(PlatformSpecificMutex)
{
    mutexDestroyCount++;
}



TEST_GROUP(SimpleMutexTest)
{
    void setup() _override
    {
        UT_PTR_SET(PlatformSpecificMutexCreate, StubMutexCreate);
        UT_PTR_SET(PlatformSpecificMutexLock, StubMutexLock);
        UT_PTR_SET(PlatformSpecificMutexUnlock, StubMutexUnlock);
        UT_PTR_SET(PlatformSpecificMutexDestroy, StubMutexDestroy);

        mutexCreateCount = 0;
        mutexDestroyCount = 0;
        mutexLockCount = 0;
        mutexUnlockCount = 0;
    }

    void teardown() _override
    {
    }
};

TEST(SimpleMutexTest, CreateAndDestroy)
{
    {
        SimpleMutex mtx;
    }

    CHECK_EQUAL(1, mutexCreateCount);
    CHECK_EQUAL(1, mutexDestroyCount);
    CHECK_EQUAL(0, mutexLockCount);
    CHECK_EQUAL(0, mutexUnlockCount);
}

TEST(SimpleMutexTest, LockUnlockTest)
{
    {
        SimpleMutex mtx;
        mtx.Lock();
        mtx.Unlock();
    }

    CHECK_EQUAL(1, mutexCreateCount);
    CHECK_EQUAL(1, mutexLockCount);
    CHECK_EQUAL(1, mutexUnlockCount);
    CHECK_EQUAL(1, mutexDestroyCount);
}
