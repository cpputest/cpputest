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

///////////////////////////////////////////////////////////////////////////////
//
//  MemoryLeakWarning.h
//
//  MemoryLeakWarning defines the inteface to a platform specific
//  memory leak detection class.  See Platforms directory for examples
//
///////////////////////////////////////////////////////////////////////////////

#define IGNORE_ALL_LEAKS_IN_TEST() MemoryLeakWarningPlugin::getFirstPlugin()->ignoreAllLeaksInTest();
#define EXPECT_N_LEAKS(n)          MemoryLeakWarningPlugin::getFirstPlugin()->expectLeaksInTest(n);

#if UT_NEW_OVERRIDES_ENABLED

extern "C" { /* include for size_t definition */
   #include "TestHarness_c.h"
}

   #ifdef new
      #undef new
   #endif

   void* operator new(size_t size);
   void* operator new[](size_t size);
   void* operator new(size_t size, const char* file, int line);
   void* operator new[](size_t size, const char* file, int line);
   void operator delete(void* mem, const char* file, int line);
   void operator delete[](void* mem, const char* file, int line);
   void operator delete(void* mem);
   void operator delete[](void* mem);

   #if UT_NEW_MACROS_ENABLED
      #define new new(__FILE__, __LINE__)
   #endif

#endif

class MemoryLeakDetector;

class MemoryLeakWarningPlugin : public TestPlugin
  {
  public:
  		MemoryLeakWarningPlugin(const SimpleString& name, MemoryLeakDetector* localDetector = 0);
  		virtual ~MemoryLeakWarningPlugin();

		virtual void preTestAction(Utest& test, TestResult& result);
		virtual void postTestAction(Utest& test, TestResult& result);

		virtual const char* FinalReport(int toBeDeletedLeaks = 0);

		void ignoreAllLeaksInTest();
		void expectLeaksInTest(int n);

		MemoryLeakDetector* getMemoryLeakDetector();
		static MemoryLeakWarningPlugin* getFirstPlugin();

		static MemoryLeakDetector* getGlobalDetector();
	private:
		MemoryLeakDetector* memLeakDetector;
		bool ignoreAllWarnings;
		int expectedLeaks;
		int failureCount;

		static MemoryLeakWarningPlugin* firstPlugin;
  };

#endif
