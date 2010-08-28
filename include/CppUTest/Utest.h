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

// This file contains the Test class along with the macros which make effective
// in the harness.

#ifndef D_UTest_h
#define D_UTest_h

#include "SimpleString.h"

/*! \brief UTest.h
 * \ingroup someGroup
 *
 * Something about UTest.h
 *
 * \see TEST_GROUP
 * \see TEST
 */

class TestResult;
class TestPlugin;
class TestFailure;

extern bool doubles_equal(double d1, double d2, double threshold);

class Utest
{
public:
	Utest(const char* groupName, const char* testName, const char* fileName,
			int lineNumber);
	virtual ~Utest();

	virtual void run(TestResult& result);
	virtual void runOneTestWithPlugins(TestPlugin* plugin, TestResult& result);
	virtual SimpleString getFormattedName() const;

	virtual Utest* addTest(Utest* test);
	virtual Utest *getNext() const;
	virtual bool isNull() const;
	virtual int countTests();

	bool shouldRun(const SimpleString& groupFilter, const SimpleString& nameFilter) const;
	const SimpleString getName() const;
	const SimpleString getGroup() const;
	const SimpleString getFile() const;
	int getLineNumber() const;
    const virtual char *getProgressIndicator() const;

    virtual void setup();
    virtual void teardown();
	virtual void testBody();

	static TestResult *getTestResult();
    static Utest *getCurrent();

    virtual void assertTrue(bool condition, const char *conditionString, const char *fileName, int lineNumber);
    virtual void assertCstrEqual(const char *expected, const char *actual, const char *fileName, int lineNumber);
    virtual void assertCstrNoCaseEqual(const char *expected, const char *actual, const char *fileName, int lineNumber);
    virtual void assertCstrContains(const char *expected, const char *actual, const char *fileName, int lineNumber);
    virtual void assertCstrNoCaseContains(const char *expected, const char *actual, const char *fileName, int lineNumber);
    virtual void assertLongsEqual(long  expected, long  actual, const char *fileName, int lineNumber);
    virtual void assertPointersEqual(const void *expected, const void *actual, const char *fileName, int lineNumber);
    virtual void assertDoublesEqual(double expected, double actual, double threshold, const char *fileName, int lineNumber);
    virtual void fail(const char *text, const char *fileName, int lineNumber);

    virtual void print(const char *text, const char *fileName, int lineNumber);
    virtual void print(const SimpleString & text, const char *fileName, int lineNumber);

    void setFileName(const char *fileName);
    void setLineNumber(int lineNumber);
    void setGroupName(const char *groupName);
    void setTestName(const char *testName);

    virtual void exitCurrentTest();
protected:
    virtual void runOneTest(TestPlugin *plugin, TestResult & result);
    virtual void executePlatformSpecificRunOneTest(TestPlugin *plugin, TestResult & result);
    virtual bool executePlatformSpecificSetup();
    virtual void executePlatformSpecificTestBody();
    virtual void executePlatformSpecificTeardown();
    virtual void executePlatformSpecificExitCurrentTest();

    Utest();
    Utest(const char *groupName, const char *testName, const char *fileName, int lineNumber, Utest *nextTest);

    virtual SimpleString getMacroName() const;
private:
    const char *group_;
    const char *name_;
    const char *file_;
    int lineNumber_;
    Utest *next_;

	void setTestResult(TestResult* result);
	void setCurrentTest(Utest* test);

	static Utest* currentTest_;
	static TestResult* testResult_;

    void failWith(const TestFailure& failure);
};

//////////////////// NulLTest

class NullTest: public Utest
{
public:
	explicit NullTest();
	explicit NullTest(const char* fileName, int lineNumber);
	virtual ~NullTest();

	void testBody()
	{
	}

	static NullTest& instance();

	virtual int countTests();
	virtual Utest*getNext() const;
	virtual bool isNull() const;
private:

	NullTest(const NullTest&);
	NullTest& operator=(const NullTest&);

};

//////////////////// ExecFunctionTest

class ExecFunctionTest: public Utest
{
public:
	void (*setup_)();
	void (*teardown_)();
	void (*testFunction_)();
	ExecFunctionTest(void(*set)() = 0, void(*tear)() = 0) :
		Utest("Generic", "Generic", "Generic", 1), setup_(set), teardown_(
				tear), testFunction_(0)
	{
	}
	void testBody()
	{
		if (testFunction_) testFunction_();
	}
	virtual void setup()
	{
		if (setup_) setup_();
	}
	virtual void teardown()
	{
		if (teardown_) teardown_();
	}
};

//////////////////// TestInstaller

class TestInstaller
{
public:
	explicit TestInstaller(Utest*, const char* groupName, const char* testName,
			const char* fileName, int lineNumber);
	virtual ~TestInstaller();

	void unDo();

private:

	TestInstaller(const TestInstaller&);
	TestInstaller& operator=(const TestInstaller&);

};

#endif
