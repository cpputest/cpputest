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

///////////////////////////////////////////////////////////////////////////////
//
// TESTREGISTRY.H
//
// TestRegistry is a collection of tests that can be run
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_TestRegistry_h
#define D_TestRegistry_h

#include "SimpleString.h"

class Utest;
class TestResult;
class TestOutput;
class TestPlugin;

class TestRegistry
{
	public:
		TestRegistry();
		virtual ~TestRegistry();

		virtual void addTest(Utest *test);
		virtual void unDoLastAddTest();
		virtual int  countTests();
		virtual void runAllTests(TestResult& result, TestOutput*);
		virtual void verbose();
		virtual void nameFilter(const char*);
		virtual void groupFilter(const char*);

		virtual void installPlugin(TestPlugin* plugin);
		virtual void resetPlugins();
		virtual TestPlugin* getFirstPlugin();
		
		const char* getGroupFilter();
		const char* getNameFilter();
		
		static TestRegistry*  getCurrentRegistry();
		virtual void setCurrentRegistry(TestRegistry* registry);
  private:

    bool testShouldRun(Utest* test, TestResult& result);
    void print(Utest* test);

    TestOutput* 	output;
    Utest *				tests;
    bool 					verbose_;
    const char* 	nameFilter_;
    const char* 	groupFilter_;
    int 					dotCount;
    TestPlugin* 	firstPlugin_;
    static TestRegistry* currentRegistry_;

  };

#endif
