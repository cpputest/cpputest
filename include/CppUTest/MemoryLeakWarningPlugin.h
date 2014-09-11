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

#ifndef D_MemoryLeakWarningPlugin_h
#define D_MemoryLeakWarningPlugin_h

#include "TestPlugin.h"
#include "MemoryLeakDetectorNewMacros.h"

#define IGNORE_ALL_LEAKS_IN_TEST() MemoryLeakWarningPlugin::getFirstPlugin()->ignoreAllLeaksInTest();
#define EXPECT_N_LEAKS(n)          MemoryLeakWarningPlugin::getFirstPlugin()->expectLeaksInTest(n);

extern void crash_on_allocation_number(unsigned alloc_number);

class MemoryLeakDetector;
class MemoryLeakFailure;

class MemoryLeakWarningPlugin: public TestPlugin
{
public:
    MemoryLeakWarningPlugin(const SimpleString& name, MemoryLeakDetector* localDetector = 0);
    virtual ~MemoryLeakWarningPlugin();

    virtual void preTestAction(UtestShell& test, TestResult& result) _override;
    virtual void postTestAction(UtestShell& test, TestResult& result) _override;

    virtual const char* FinalReport(int toBeDeletedLeaks = 0);

    void ignoreAllLeaksInTest();
    void expectLeaksInTest(int n);

    void destroyGlobalDetectorAndTurnOffMemoryLeakDetectionInDestructor(bool des);

    MemoryLeakDetector* getMemoryLeakDetector();

    static MemoryLeakWarningPlugin* getFirstPlugin();

    static MemoryLeakDetector* getGlobalDetector();
    static MemoryLeakFailure* getGlobalFailureReporter();
    static void setGlobalDetector(MemoryLeakDetector* detector, MemoryLeakFailure* reporter);
    static void destroyGlobalDetector();

    static void turnOffNewDeleteOverloads();
    static void turnOnNewDeleteOverloads();
    static void turnOnThreadSafeNewDeleteOverloads();
    static bool areNewDeleteOverloaded();
private:
    MemoryLeakDetector* memLeakDetector_;
    bool ignoreAllWarnings_;
    bool destroyGlobalDetectorAndTurnOfMemoryLeakDetectionInDestructor_;
    int expectedLeaks_;
    int failureCount_;

    static MemoryLeakWarningPlugin* firstPlugin_;
};

extern void* cpputest_malloc_location_with_leak_detection(size_t size, const char* file, int line);
extern void* cpputest_realloc_location_with_leak_detection(void* memory, size_t size, const char* file, int line);
extern void cpputest_free_location_with_leak_detection(void* buffer, const char* file, int line);

#endif
