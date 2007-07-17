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

#include "TestHarness.h"
#include "MockTestOutput.h"
#include "MemoryLeakWarningPlugin.h"
#include "GenericTest.h"

EXPORT_TEST_GROUP(MemoryLeakWarningTest);

namespace
{
	char* arrayToLeak1;
	char* arrayToLeak2;
	long* nonArrayToLeak;
  
	MemoryLeakWarningPlugin* memPlugin;
	MemoryLeakWarning* prevMemWarning;

	GenericTestFixture* fixture;
	void SetUp()
	{
		fixture = new GenericTestFixture();
  		prevMemWarning = MemoryLeakWarning::_latest;
  		memPlugin = new MemoryLeakWarningPlugin;
  		fixture->registry->installPlugin(memPlugin);
		
		arrayToLeak1 = 0;
		arrayToLeak2 = 0;
		nonArrayToLeak = 0;
	}
	void TearDown()
	{
		delete fixture;
  		delete memPlugin;
  		MemoryLeakWarning::_latest = prevMemWarning;
  		
  		if (arrayToLeak1) delete [] arrayToLeak1;
  		if (arrayToLeak2) delete [] arrayToLeak2;
  		if (nonArrayToLeak) delete nonArrayToLeak;
	}
}

void _testExpectOneLeak()
{
    EXPECT_N_LEAKS(1);
    arrayToLeak1 = new char[100];
}

TEST(MemoryLeakWarningTest, Ignore1)
{
	fixture->setTestFunction(_testExpectOneLeak);
	fixture->runAllTests(); 
	LONGS_EQUAL(0, fixture->getFailureCount());
}

void _testTwoLeaks()
{
    arrayToLeak2 = new char[10];
    nonArrayToLeak = new long;
}	

TEST(MemoryLeakWarningTest, TwoLeaks)
{
	fixture->setTestFunction(_testTwoLeaks);
	fixture->runAllTests(); 
	LONGS_EQUAL(1, fixture->getFailureCount());
}

void _testIgnore2()
{
    EXPECT_N_LEAKS(2);
    arrayToLeak2 = new char[10];
    nonArrayToLeak = new long;
}

TEST(MemoryLeakWarningTest, Ignore2)
{
	fixture->setTestFunction(_testIgnore2);
	fixture->runAllTests(); 
	LONGS_EQUAL(0, fixture->getFailureCount());
}
