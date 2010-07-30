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

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestOutput.h"

/* Sometimes stubs use the CppUTest assertions.
 * Its not correct to do so, but this small helper class will prevent a segmentation fault and instead
 * will give an error message and also the file/line of the check that was executed outside the tests.
 */
class OutsideTestRunnerUTest: public Utest
{
public:
	static OutsideTestRunnerUTest& instance()
	{
		static OutsideTestRunnerUTest instance_;
		return instance_;
	}
	virtual TestResult& getTestResult()
	{
		return defaultTestResult;
	}
	virtual void exitCurrentTest()
	{
	}
	;
	virtual ~OutsideTestRunnerUTest()
	{
	}
	;
private:
	OutsideTestRunnerUTest() :
		Utest("\n\t NOTE: Assertion happened without being in a test run (perhaps in main?)", "\n\t       Something is very wrong. Check this assertion and fix", "unknown file", 0),
				defaultTestResult(defaultOutput)
	{
	}
	;
	ConsoleTestOutput defaultOutput;
	TestResult defaultTestResult;
};




Utest::Utest() :
	group_("UndefinedTestGroup"), name_("UndefinedTest"), file_("UndefinedFile"), lineNumber_(0), next_(&NullTest::instance())
{
}

Utest::Utest(const char* groupName, const char* testName, const char* fileName, int lineNumber) :
	group_(groupName), name_(testName), file_(fileName), lineNumber_(lineNumber), next_(&NullTest::instance())
{
}

Utest::Utest(const char* groupName, const char* testName, const char* fileName, int lineNumber, Utest* nextTest) :
	group_(groupName), name_(testName), file_(fileName), lineNumber_(lineNumber), next_(nextTest)
{
}

Utest::~Utest()
{
}

void Utest::runOneTestWithPlugins(TestPlugin* plugin, TestResult& result)
{
	executePlatformSpecificRunOneTest(plugin, result);
}

void Utest::runOneTest(TestPlugin* plugin, TestResult& result)
{
	plugin->runAllPreTestAction(*this, result);
	run(result);
	plugin->runAllPostTestAction(*this, result);
}

void Utest::run(TestResult& result)
{
	//save test context, so that test class can be tested
	Utest* savedTest = getCurrent();
	TestResult* savedResult = getTestResult();

	result.countRun();
	setTestResult(&result);
	setCurrentTest(this);

	if (executePlatformSpecificSetup()) {
		executePlatformSpecificTestBody();
	}
	executePlatformSpecificTeardown();

	setCurrentTest(savedTest);
	setTestResult(savedResult);
}

void Utest::exitCurrentTest()
{
	executePlatformSpecificExitCurrentTest();
}

Utest *Utest::getNext() const
{
	return next_;
}

Utest* Utest::addTest(Utest *test)
{
	next_ = test;
	return this;
}

int Utest::countTests()
{
	return next_->countTests() + 1;
}

bool Utest::isNull() const
{
	return false;
}

SimpleString Utest::getMacroName() const
{
	return "TEST";
}

const SimpleString Utest::getName() const
{
	return SimpleString(name_);
}

const SimpleString Utest::getGroup() const
{
	return SimpleString(group_);
}

SimpleString Utest::getFormattedName() const
{
	SimpleString formattedName(getMacroName());
	formattedName += "(";
	formattedName += group_;
	formattedName += ", ";
	formattedName += name_;
	formattedName += ")";

	return formattedName;
}

const char* Utest::getProgressIndicator() const
{
	return ".";
}

void Utest::setFileName(const char* fileName)
{
	file_ = fileName;
}

void Utest::setLineNumber(int lineNumber)
{
	lineNumber_ = lineNumber;
}

void Utest::setGroupName(const char* groupName)
{
	group_ = groupName;
}

void Utest::setTestName(const char* testName)
{
	name_ = testName;
}

const SimpleString Utest::getFile() const
{
	return SimpleString(file_);
}

int Utest::getLineNumber() const
{
	return lineNumber_;
}

void Utest::setup()
{
}

void Utest::teardown()
{
}

bool Utest::shouldRun(const SimpleString& groupFilter, const SimpleString& nameFilter) const
{
	SimpleString group(group_);
	SimpleString name(name_);
	if (group.contains(groupFilter) && name.contains(nameFilter)) return true;

	return false;
}

void Utest::assertTrue(bool condition, const char* conditionString, const char* fileName, int lineNumber)
{
	getTestResult()->countCheck();
	if (!(condition)) {
		SimpleString message("CHECK(");
		message += conditionString;
		message += ") failed";
		TestFailure _f(this, fileName, lineNumber, message);
		getTestResult()->addFailure(_f);
		Utest::getCurrent()->exitCurrentTest();
	}
}

SimpleString Utest::stringFromOrNull(const char * expected)
{
    return (expected) ? StringFrom(expected) : "(null)";
}

void Utest::fail(const char *text, const char* fileName, int lineNumber)
{
	TestFailure _f(this, fileName, lineNumber, text);
	getTestResult()->addFailure(_f);
    Utest::getCurrent()->exitCurrentTest();
}

int Utest::findStartOfCstrEqualsFailNoCase(const char * expected, const char* actual)
{
	int i;
    for (i = 0; tolower(actual[i]) == tolower(expected[i]); i++)
        ;
    return i;
}

int Utest::findStartOfCstrEqualsFail(const char * expected, const char* actual)
{
	int i;
    for (i = 0; actual[i] == expected[i]; i++)
        ;
    return i;
}

const char* Utest::formCStrEqualsFailMessage(const char* actual, int failStart)
{
	    const char * error = "<!>";
	    char * message;

	    message = new char[PlatformSpecificStrLen(actual) + PlatformSpecificStrLen(error) + 10];

	    int j;
	    for (j = 0; j < failStart; j++)
	        message[j] = actual[j];

	    for (int k = 0; k < (int)PlatformSpecificStrLen(error); j++, k++)
	        message[j] = error[k];

	    for (int i = failStart; actual[i]; i++, j++)
	        message[j] = actual[i];

	    message[j] = 0;

	    return message;
}


void Utest::failCstrEqualsTest(const char * expected, const char* actual, const char * fileName, int lineNumber)
{
	int failStart = findStartOfCstrEqualsFail(expected, actual);
	const char * markedActual = formCStrEqualsFailMessage(actual, failStart);
	EqualsFailure _f(this, fileName, lineNumber, stringFromOrNull(expected), stringFromOrNull(markedActual));
    testResult_->addFailure(_f);
    delete [] markedActual;
    Utest::getCurrent()->exitCurrentTest();
}

void Utest::failCstrEqualsTestNoCase(const char * expected, const char* actual, const char * fileName, int lineNumber)
{
	int failStart = findStartOfCstrEqualsFailNoCase(expected, actual);
	const char * markedActual = formCStrEqualsFailMessage(actual, failStart);
	EqualsFailure _f(this, fileName, lineNumber, stringFromOrNull(expected), stringFromOrNull(markedActual));
    testResult_->addFailure(_f);
    delete [] markedActual;
    Utest::getCurrent()->exitCurrentTest();
}

void Utest::failEqualsTest(const char * expected, const char* actual, const char * fileName, int lineNumber)
{
	EqualsFailure _f(this, fileName, lineNumber, stringFromOrNull(expected), stringFromOrNull(actual));
	getTestResult()->addFailure(_f);
    Utest::getCurrent()->exitCurrentTest();
}

void Utest::failContainsTest(const char * expected, const char* actual, const char * fileName, int lineNumber)
{
	ContainsFailure _f(this, fileName, lineNumber, StringFrom(expected), StringFrom(actual));
	getTestResult()->addFailure(_f);
    Utest::getCurrent()->exitCurrentTest();
}

void Utest::assertCstrEqual(const char* expected, const char* actual, const char* fileName, int lineNumber)
{
	getTestResult()->countCheck();
	if (actual == 0 && expected == 0) return;
	if (actual == 0 || expected == 0)
		failEqualsTest (expected, actual, fileName, lineNumber);
	if (PlatformSpecificStrCmp(expected, actual) != 0)
		failCstrEqualsTest (expected, actual, fileName, lineNumber);
}

void Utest::assertCstrNoCaseEqual(const char* expected, const char* actual, const char* fileName, int lineNumber)
{
	getTestResult()->countCheck();
	if (actual == 0 && expected == 0) return;
	if (actual == 0 || expected == 0)
		failEqualsTest (expected, actual, fileName, lineNumber);
	if (!SimpleString(expected).equalsNoCase(actual))
		failCstrEqualsTestNoCase(expected, actual, fileName, lineNumber);
}

void Utest::assertCstrContains(const char* expected, const char* actual, const char* fileName, int lineNumber)
{
	getTestResult()->countCheck();
	if (actual == 0 && expected == 0) return;
    if(actual == 0 || expected == 0)
    	failContainsTest(expected, actual, fileName, lineNumber);
    if (!SimpleString(actual).contains(expected))
    	failContainsTest(expected, actual, fileName, lineNumber);
}

void Utest::assertCstrNoCaseContains(const char* expected, const char* actual, const char* fileName, int lineNumber)
{
	getTestResult()->countCheck();
	if (actual == 0 && expected == 0) return;
    if(actual == 0 || expected == 0)
    	failContainsTest(expected, actual, fileName, lineNumber);
    if (!SimpleString(actual).containsNoCase(expected))
    	failContainsTest(expected, actual, fileName, lineNumber);
}

void Utest::assertLongsEqual(long expected, long actual, const char* fileName, int lineNumber)
{
	getTestResult()->countCheck();
	if (expected != actual) {
		SimpleString aDecimal = StringFrom(actual);
		SimpleString aHex = HexStringFrom(actual);
		SimpleString eDecimal = StringFrom(expected);
		SimpleString eHex = HexStringFrom(expected);

		SimpleString::padStringsToSameLength(aDecimal, eDecimal, ' ');
		SimpleString::padStringsToSameLength(aHex, eHex, '0');

		SimpleString actualReported = aDecimal + " 0x" + aHex;
		SimpleString expectedReported = eDecimal + " 0x" + eHex;

		failEqualsTest(expectedReported.asCharString(), actualReported.asCharString(), fileName, lineNumber);
	}
}

void Utest::assertPointersEqual(void* expected, void* actual, const char* fileName, int lineNumber)
{
	getTestResult()->countCheck();
	if (expected != actual)
		failEqualsTest(StringFrom(expected).asCharString(), StringFrom(actual).asCharString(), fileName, lineNumber);
}

void Utest::assertDoublesEqual(double expected, double actual, double threshold, const char* fileName, int lineNumber)
{
	getTestResult()->countCheck();
	if (PlatformSpecificFabs(expected - actual) > threshold)
		failEqualsTest(StringFrom(expected).asCharString(), StringFrom(actual).asCharString(), fileName, lineNumber);
}

void Utest::print(const char *text, const char* fileName, int lineNumber)
{
	SimpleString stringToPrint = "\n";
	stringToPrint += fileName;
	stringToPrint += ":";
	stringToPrint += StringFrom(lineNumber);
	stringToPrint += " ";
	stringToPrint += text;
	getTestResult()->print(stringToPrint.asCharString());
}

void Utest::print(const SimpleString& text, const char* fileName, int lineNumber)
{
	print(text.asCharString(), fileName, lineNumber);
}

TestResult* Utest::testResult_ = NULL;
Utest* Utest::currentTest_ = NULL;

void Utest::setTestResult(TestResult* result)
{
	testResult_ = result;
}

void Utest::setCurrentTest(Utest* test)
{
	currentTest_ = test;
}

TestResult* Utest::getTestResult()
{
	if (testResult_ == NULL)
		return &OutsideTestRunnerUTest::instance().getTestResult();
	return testResult_;
}

Utest* Utest::getCurrent()
{
	if (currentTest_ == NULL)
		return &OutsideTestRunnerUTest::instance();
	return currentTest_;
}

////////////// NullTest ////////////


NullTest::NullTest() :
	Utest("NullGroup", "NullName", "NullFile", -1, 0)
{
}

NullTest::~NullTest()
{
}

NullTest& NullTest::instance()
{
	static NullTest _instance;
	return _instance;
}

int NullTest::countTests()
{
	return 0;
}

Utest* NullTest::getNext() const
{
	return &instance();
}

bool NullTest::isNull() const
{
	return true;
}

////////////// TestInstaller ////////////

TestInstaller::TestInstaller(Utest* t, const char* groupName, const char* testName, const char* fileName, int lineNumber)
{
	t->setGroupName(groupName);
	t->setTestName(testName);
	t->setFileName(fileName);
	t->setLineNumber(lineNumber);
	TestRegistry::getCurrentRegistry()->addTest(t);
}

TestInstaller::~TestInstaller()
{
}

void TestInstaller::unDo()
{
	TestRegistry::getCurrentRegistry()->unDoLastAddTest();
}
