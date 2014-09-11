/*
 * Copyright (c) 2014, YewMing Chen
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
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/SimpleMutex.h"
#include "CppUTest/PlatformSpecificFunctions.h"

PlatformSpecificMutex PlatformSpecificMutexCreate(void)
{
    return mock().actualCall("PlatformSpecificMutexCreate").returnPointerValueOrDefault(0);
}

void PlatformSpecificMutexLock(PlatformSpecificMutex)
{
    mock().actualCall("PlatformSpecificMutexLock");
}

void PlatformSpecificMutexUnlock(PlatformSpecificMutex)
{
    mock().actualCall("PlatformSpecificMutexUnlock");
}

void PlatformSpecificMutexDestroy(PlatformSpecificMutex)
{
    mock().actualCall("PlatformSpecificMutexDestroy");
}



TEST_GROUP(SimpleMutexTest)
{
    void setup()
    {
    }
    
    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(SimpleMutexTest, CreateAndDestroy)
{
    mock().expectOneCall("PlatformSpecificMutexCreate");
    mock().expectOneCall("PlatformSpecificMutexDestroy");

    SimpleMutex mtx;
}

TEST(SimpleMutexTest, LockUnlockTest)
{
    mock().expectOneCall("PlatformSpecificMutexCreate");
    mock().expectOneCall("PlatformSpecificMutexLock");
    mock().expectOneCall("PlatformSpecificMutexUnlock");
    mock().expectOneCall("PlatformSpecificMutexDestroy");

    SimpleMutex mtx;
    mtx.Lock();
    mtx.Unlock();
}
