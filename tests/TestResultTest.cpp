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


static long timeInMillies;

class MockTestResult : public TestResult
{
public:

	MockTestResult(TestOutput& output) : TestResult(output){};
	
	virtual long GetPlatformSpecificTimeInMillis()
	{
		return timeInMillies;
	}
};

TEST_GROUP(TestResult)
{
	TestOutput* printer;
	MockTestOutput* mock;
	
	TestResult* res;
	MockTestResult * mockRes;

	TEST_SETUP()
	{
		mock = new MockTestOutput();
		printer = mock;
		mockRes = new MockTestResult(*printer);
		res = mockRes;
		timeInMillies = 0;
	}
	TEST_TEARDOWN()
	{
		delete printer;
		delete res;
	}	
};

TEST(TestResult, TestEndedWillPrintResultsAndExecutionTime)
{
	timeInMillies = 10;
	res->testsEnded();
	CHECK(mock->getOutput().contains("10.0"));
}
