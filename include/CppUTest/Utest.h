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

class TestResult;
class TestPlugin;
class TestFailure;
class TestFilter;
class TestTerminator;

extern bool doubles_equal(double d1, double d2, double threshold);

//////////////////// Utest

class UtestShell;

class Utest
{
public:
    Utest();
    virtual ~Utest();
    virtual void run();

    virtual void setup();
    virtual void teardown();
    virtual void testBody();
};

//////////////////// TestTerminator

class TestTerminator
{
public:
    virtual void exitCurrentTest() const=0;
    virtual ~TestTerminator();
};

class NormalTestTerminator : public TestTerminator
{
public:
    virtual void exitCurrentTest() const _override;
    virtual ~NormalTestTerminator() _destructor_override;
};

class TestTerminatorWithoutExceptions  : public TestTerminator
{
public:
    virtual void exitCurrentTest() const _override;
    virtual ~TestTerminatorWithoutExceptions() _destructor_override;
};

class CrashingTestTerminator : public NormalTestTerminator
{
public:
    virtual void exitCurrentTest() const _override;
    virtual ~CrashingTestTerminator() _destructor_override;
};

//////////////////// UtestShell

class UtestShell
{
public:
    static UtestShell *getCurrent();

    static const TestTerminator &getCurrentTestTerminator();

    static void setCrashOnFail();
    static void restoreDefaultTestTerminator();

public:
    UtestShell(const char* groupName, const char* testName, const char* fileName, size_t lineNumber);
    virtual ~UtestShell();

    virtual UtestShell* addTest(UtestShell* test);
    virtual UtestShell *getNext() const;
    virtual size_t countTests();

    bool shouldRun(const TestFilter* groupFilters, const TestFilter* nameFilters) const;
    const SimpleString getName() const;
    const SimpleString getGroup() const;
    virtual SimpleString getFormattedName() const;
    const SimpleString getFile() const;
    size_t getLineNumber() const;
    virtual bool willRun() const;
    virtual bool hasFailed() const;
    void countCheck();

    virtual void assertTrue(bool condition, const char *checkString, const char *conditionString, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertCstrEqual(const char *expected, const char *actual, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertCstrNEqual(const char *expected, const char *actual, size_t length, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertCstrNoCaseEqual(const char *expected, const char *actual, const char* text, const char *fileName, size_t lineNumber);
    virtual void assertCstrContains(const char *expected, const char *actual, const char* text, const char *fileName, size_t lineNumber);
    virtual void assertCstrNoCaseContains(const char *expected, const char *actual, const char* text, const char *fileName, size_t lineNumber);
    virtual void assertLongsEqual(long expected, long actual, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertUnsignedLongsEqual(unsigned long expected, unsigned long actual, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertLongLongsEqual(cpputest_longlong expected, cpputest_longlong actual, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertUnsignedLongLongsEqual(cpputest_ulonglong expected, cpputest_ulonglong actual, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertSignedBytesEqual(signed char expected, signed char actual, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertPointersEqual(const void *expected, const void *actual, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertFunctionPointersEqual(void (*expected)(), void (*actual)(), const char* text, const char* fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertDoublesEqual(double expected, double actual, double threshold, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertEquals(bool failed, const char* expected, const char* actual, const char* text, const char* file, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertBinaryEqual(const void *expected, const void *actual, size_t length, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertBitsEqual(unsigned long expected, unsigned long actual, unsigned long mask, size_t byteCount, const char* text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void assertCompare(bool comparison, const char *checkString, const char *comparisonString, const char *text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void fail(const char *text, const char *fileName, size_t lineNumber, const TestTerminator& testTerminator = getCurrentTestTerminator());
    virtual void exitTest(const TestTerminator& testTerminator = getCurrentTestTerminator());

    virtual void print(const char *text, const char *fileName, size_t lineNumber);
    virtual void print(const SimpleString & text, const char *fileName, size_t lineNumber);
    virtual void printVeryVerbose(const char* text);

    void setFileName(const char *fileName);
    void setLineNumber(size_t lineNumber);
    void setGroupName(const char *groupName);
    void setTestName(const char *testName);

    static void crash();
    static void setCrashMethod(void (*crashme)());
    static void resetCrashMethod();

    virtual bool isRunInSeperateProcess() const;
    virtual void setRunInSeperateProcess();

    virtual void setRunIgnored();

    virtual Utest* createTest();
    virtual void destroyTest(Utest* test);

    virtual void runOneTest(TestPlugin* plugin, TestResult& result);
    virtual void runOneTestInCurrentProcess(TestPlugin *plugin, TestResult & result);

    virtual void failWith(const TestFailure& failure);
    virtual void failWith(const TestFailure& failure, const TestTerminator& terminator);

protected:
    UtestShell();
    UtestShell(const char *groupName, const char *testName, const char *fileName, size_t lineNumber, UtestShell *nextTest);

    virtual SimpleString getMacroName() const;
    TestResult *getTestResult();
private:
    const char *group_;
    const char *name_;
    const char *file_;
    size_t lineNumber_;
    UtestShell *next_;
    bool isRunAsSeperateProcess_;
    bool hasFailed_;

    void setTestResult(TestResult* result);
    void setCurrentTest(UtestShell* test);
    bool match(const char* target, const TestFilter* filters) const;

    static UtestShell* currentTest_;
    static TestResult* testResult_;

    static const TestTerminator *currentTestTerminator_;
};



//////////////////// ExecFunctionTest

class ExecFunctionTestShell;

class ExecFunctionTest : public Utest
{
public:
    ExecFunctionTest(ExecFunctionTestShell* shell);
    void testBody() _override;
    virtual void setup() _override;
    virtual void teardown() _override;
private:
    ExecFunctionTestShell* shell_;
};

//////////////////// ExecFunction

class ExecFunction
{
public:
    ExecFunction();
    virtual ~ExecFunction();

    virtual void exec()=0;
};

class ExecFunctionWithoutParameters : public ExecFunction
{
public:
    void (*testFunction_)();

    ExecFunctionWithoutParameters(void(*testFunction)());
    virtual ~ExecFunctionWithoutParameters() _destructor_override;

    virtual void exec() _override;
};

//////////////////// ExecFunctionTestShell

class ExecFunctionTestShell : public UtestShell
{
public:
    void (*setup_)();
    void (*teardown_)();
    ExecFunction* testFunction_;

    ExecFunctionTestShell(void(*set)() = NULLPTR, void(*tear)() = NULLPTR) :
        UtestShell("ExecFunction", "ExecFunction", "ExecFunction", 1), setup_(set), teardown_(tear), testFunction_(NULLPTR)
    {
    }

    Utest* createTest() _override { return new ExecFunctionTest(this); }
    virtual ~ExecFunctionTestShell() _destructor_override;
};

//////////////////// CppUTestFailedException

class CppUTestFailedException
{
public:
    int dummy_;
};

//////////////////// IgnoredTest

class IgnoredUtestShell : public UtestShell
{
public:
    IgnoredUtestShell();
    virtual ~IgnoredUtestShell() _destructor_override;
    explicit IgnoredUtestShell(const char* groupName, const char* testName,
            const char* fileName, size_t lineNumber);
    virtual bool willRun() const _override;
    virtual void setRunIgnored() _override;
protected:
    virtual SimpleString getMacroName() const _override;
    virtual void runOneTest(TestPlugin* plugin, TestResult& result) _override;
private:

    IgnoredUtestShell(const IgnoredUtestShell&);
    IgnoredUtestShell& operator=(const IgnoredUtestShell&);

    bool runIgnored_;

};

//////////////////// UtestShellPointerArray

class UtestShellPointerArray
{
public:
    UtestShellPointerArray(UtestShell* firstTest);
    ~UtestShellPointerArray();

    void shuffle(size_t seed);
    void reverse();
    void relinkTestsInOrder();
    UtestShell* getFirstTest() const;
    UtestShell* get(size_t index) const;

private:

    void swap(size_t index1, size_t index2);

    UtestShell** arrayOfTests_;
    size_t count_;
};


//////////////////// TestInstaller

class TestInstaller
{
public:
    explicit TestInstaller(UtestShell& shell, const char* groupName, const char* testName,
            const char* fileName, size_t lineNumber);
    virtual ~TestInstaller();

    void unDo();

private:

    TestInstaller(const TestInstaller&);
    TestInstaller& operator=(const TestInstaller&);

};

#endif
