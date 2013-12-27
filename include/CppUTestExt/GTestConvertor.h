/*
 * Copyright (c) 2011, Michael Feathers, James Grenning and Bas Vodde
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

#ifndef GTESTCONVERTOR_H_
#define GTESTCONVERTOR_H_

#include "CppUTest/Utest.h"

#ifdef CPPUTEST_USE_REAL_GTEST

/*
 * Usage:
 *
 * This file must only be included in the main. The whole implementation is inline so that this can
 * be compiled on usage and not on CppUTest compile-time. This avoids a hard dependency with CppUTest
 * and with GTest
 *
 * Add the following lines to your main:
 *
 *      GTestConvertor convertor;
 *      convertor.addAllGTestToTestRegistry();
 *
 *
 */

class GTestResultReporter;

namespace testing {
	class TestInfo;
	class TestCase;
	class Test;
}

class GTestShell : public UtestShell
{
	::testing::TestInfo* testinfo_;
	GTestShell* next_;
public:
	GTestShell(::testing::TestInfo* testinfo, GTestShell* next);

	virtual Utest* createTest();

	GTestShell* nextGTest();
};

/* Enormous hack!
 *
 * This sucks enormously. We need to do two things in GTest that seem to not be possible without
 * this hack. Hopefully there is *another way*.
 *
 * We need to access the factory in the TestInfo in order to be able to create tests. The factory
 * is private and there seems to be no way to access it...
 *
 * We need to be able to call the Test SetUp and TearDown methods, but they are protected for
 * some reason. We can't subclass either as the tests are created with the TEST macro.
 *
 * If anyone knows how to get the above things done *without* these ugly #defines, let me know!
 *
 */

#define private public
#define protected public

#include "CppUTestExt/GTest.h"
#include "CppUTestExt/GMock.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest-death-test.h"
/*
 * We really need some of its internals as they don't have a public interface.
 *
 */
#define GTEST_IMPLEMENTATION_ 1
#include "src/gtest-internal-inl.h"

#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestFailure.h"
#include "CppUTest/TestResult.h"

class GTestConvertor
{
public:
	GTestConvertor(bool shouldSimulateFailureAtCreationToAllocateThreadLocalData = true);
	virtual ~GTestConvertor();

	virtual void addAllGTestToTestRegistry();
protected:
	virtual void simulateGTestFailureToPreAllocateAllTheThreadLocalData();

	virtual void addNewTestCaseForTestInfo(::testing::TestInfo* testinfo);
	virtual void addAllTestsFromTestCaseToTestRegistry(::testing::TestCase* testcase);

	virtual void createDummyInSequenceToAndFailureReporterAvoidMemoryLeakInGMock();
private:
	GTestResultReporter* reporter_;
	GTestShell* first_;
};



#endif

#endif
