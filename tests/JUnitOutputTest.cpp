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
#include "CppUTest/JUnitTestOutput.h"
#include "CppUTest/TestResult.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/SimpleString.h"

static long millisTime;

static const char* theTime = "1978-10-03T00:00:00";

extern "C" {

    typedef long (*LongFP_C)();
    typedef const char* (*StringFP_C)();

    static long MockGetPlatformSpecificTimeInMillis()
    {
	    return millisTime;
    }

    static const char* MockGetPlatformSpecificTimeString()
    {
	    return theTime;
    }

} // extern "C"

class FileForJUnitOutputTests
{
	SimpleString name_;
	bool isOpen_;
	SimpleString buffer_;
	FileForJUnitOutputTests* next_;

	SimpleStringCollection linesOfFile_;

public:

	FileForJUnitOutputTests(SimpleString filename, FileForJUnitOutputTests* next) :
		name_(filename), isOpen_(true), next_(next) {}

	FileForJUnitOutputTests* nextFile()
	{
		return next_;
	}

	SimpleString name()
	{
		return name_;
	}

	void write(const SimpleString& buffer)
	{
		buffer_ += buffer;
	}

	void close()
	{
		isOpen_ = false;
	}

	const char* line(size_t lineNumber)
	{
		buffer_.split("\n", linesOfFile_);
		return linesOfFile_[lineNumber-1].asCharString();

	}
	const char* lineFromTheBack(size_t lineNumberFromTheBack)
	{
		return line(amountOfLines() - (lineNumberFromTheBack - 1));
	}

	size_t amountOfLines()
	{
		buffer_.split("\n", linesOfFile_);
		return linesOfFile_.size();
	}

	SimpleString content()
	{
		return buffer_;
	}
};

class FileSystemForJUnitTestOutputTests
{
	FileForJUnitOutputTests* firstFile_;

public:
	FileSystemForJUnitTestOutputTests() : firstFile_(0) {}
	~FileSystemForJUnitTestOutputTests()
	{
		delete firstFile_;
	}

	FileForJUnitOutputTests* openFile(const SimpleString& filename)
	{
		firstFile_ = new FileForJUnitOutputTests(filename, firstFile_);
		return firstFile_;
	}

	int amountOfFiles() {
		int totalAmountOfFiles = 0;
		for (FileForJUnitOutputTests* current = firstFile_; current != NULL; current = current->nextFile())
			totalAmountOfFiles++;
		return totalAmountOfFiles;
	}

	bool fileExists(const char* filename)
	{
		FileForJUnitOutputTests *searchedFile = file(filename);
		return (searchedFile != NULL);
	}

	FileForJUnitOutputTests* file(const char* filename)
	{
		for (FileForJUnitOutputTests* current = firstFile_; current != NULL; current = current->nextFile())
			if (current->name() == filename)
				return current;
		return NULL;
	}
};

class JUnitTestOutputToBuffer: public JUnitTestOutput
{
public:

	FileSystemForJUnitTestOutputTests& fileSystem_;
	FileForJUnitOutputTests* currentOpenFile_;

	JUnitTestOutputToBuffer(FileSystemForJUnitTestOutputTests& fileSystem)
		: fileSystem_(fileSystem), currentOpenFile_(0) {}

	void writeToFile(const SimpleString& buffer)
	{
		currentOpenFile_->write(buffer);
	}

	void openFileForWrite(const SimpleString& filename)
	{
		currentOpenFile_ = fileSystem_.openFile(filename);
	}

	void closeFile()
	{
		currentOpenFile_->close();
		currentOpenFile_ = 0;
	}
};

		extern "C" long MockGetPlatformSpecificTimeInMillis();

class JUnitTestOutputTestRunner
{
	TestResult result_;
	const char* currentGroupName_;
	const char* currentTestName_;

	static long millisTime;
	static const char* theTime;

	static long MockGetPlatformSpecificTimeInMillis()
	{
		return millisTime;
	}

	static const char* MockGetPlatformSpecificTimeString()
	{
		return theTime;
	}

public:


	JUnitTestOutputTestRunner(TestResult result) :
		result_(result), currentGroupName_(0), currentTestName_(0)
	{
		millisTime = 0;
		theTime =  "1978-10-03T00:00:00";

		SetPlatformSpecificTimeInMillisMethod((LongFP_C)MockGetPlatformSpecificTimeInMillis);
		SetPlatformSpecificTimeStringMethod((StringFP_C)MockGetPlatformSpecificTimeString);
	}

	~JUnitTestOutputTestRunner()
	{
		SetPlatformSpecificTimeInMillisMethod(0);
		SetPlatformSpecificTimeStringMethod(0);
	}

	JUnitTestOutputTestRunner& start()
	{
		result_.testsStarted();
		return *this;
	}

	JUnitTestOutputTestRunner& end()
	{
		endOfTestGroup();
		result_.testsEnded();
		return *this;
	}

	void endOfTestGroup()
	{
		if (currentGroupName_) {
			UtestShell currentTest(currentGroupName_, currentTestName_, "file", 1);
			result_.currentGroupEnded(&currentTest);
		}

		currentGroupName_ = 0;
	}

	JUnitTestOutputTestRunner& withGroupWithTest(const char* groupName, const char* testName, int timeElapsed = 0)
	{
		endOfTestGroup();
		currentGroupName_ = groupName;

		UtestShell currentTest(currentGroupName_, testName, "file", 1);

		result_.currentGroupStarted(&currentTest);

		withTest(testName, timeElapsed);

		return *this;
	}

	JUnitTestOutputTestRunner& withTest(const char* testName, int timeElapsed = 0)
	{
		currentTestName_ = testName;

		UtestShell currentTest(currentGroupName_, testName, "file", 1);

		result_.currentTestStarted(&currentTest);
		elapsedTime(timeElapsed);
		result_.currentTestEnded(&currentTest);

		return *this;
	}

	JUnitTestOutputTestRunner& elapsedTime(int timeElapsed)
	{
		millisTime += timeElapsed;
		return *this;
	}

	JUnitTestOutputTestRunner& atTime(const char* newTime)
	{
		theTime = newTime;
		return *this;
	}
};

long JUnitTestOutputTestRunner::millisTime = 0;
const char* JUnitTestOutputTestRunner::theTime = "";


TEST_GROUP(JUnitOutputTestNew)
{
	FileSystemForJUnitTestOutputTests fileSystem;

	JUnitTestOutputToBuffer *junitOutput;
	TestResult *result;
	JUnitTestOutputTestRunner *testCaseRunner;
	FileForJUnitOutputTests* outputFile;

	void setup()
	{
		junitOutput = new JUnitTestOutputToBuffer (fileSystem);
		result = new TestResult(*junitOutput);
		testCaseRunner = new JUnitTestOutputTestRunner(*result);
	}

	void teardown()
	{
		delete testCaseRunner;
		delete result;
		delete junitOutput;
	}
};

TEST(JUnitOutputTestNew, withOneTestGroupAndOneTestOnlyWriteToOneFile)
{
	testCaseRunner->start()
			.withGroupWithTest("groupname", "testname")
			.end();

	LONGS_EQUAL(1, fileSystem.amountOfFiles());
	CHECK(fileSystem.fileExists("cpputest_groupname.xml"));
}

TEST(JUnitOutputTestNew, withOneTestGroupAndOneTestOutputsValidXMLFiles)
{
	testCaseRunner->start()
			.withGroupWithTest("groupname", "testname")
			.end();

	outputFile = fileSystem.file("cpputest_groupname.xml");
	STRCMP_EQUAL("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n", outputFile->line(1));
}

TEST(JUnitOutputTestNew, withOneTestGroupAndOneTestoutputsTestSuiteStartAndEndBlocks)
{
	testCaseRunner->start()
			.withGroupWithTest("groupname", "testname")
			.end();

	outputFile = fileSystem.file("cpputest_groupname.xml");
	STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"groupname\" tests=\"1\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line(2));
	STRCMP_EQUAL("</testsuite>", outputFile->lineFromTheBack(1));
}

TEST(JUnitOutputTestNew, withOneTestGroupAndOneTestFileShouldContainAnEmptyPropertiesBlock)
{
	testCaseRunner->start()
			.withGroupWithTest("groupname", "testname")
			.end();

	outputFile = fileSystem.file("cpputest_groupname.xml");
	STRCMP_EQUAL("<properties>\n", outputFile->line(3));
	STRCMP_EQUAL("</properties>\n", outputFile->line(4));
}

TEST(JUnitOutputTestNew, withOneTestGroupAndOneTestFileShouldContainAnEmptyStdoutBlock)
{
	testCaseRunner->start()
			.withGroupWithTest("groupname", "testname")
			.end();

	outputFile = fileSystem.file("cpputest_groupname.xml");
	STRCMP_EQUAL("<system-out></system-out>\n", outputFile->lineFromTheBack(3));
}

TEST(JUnitOutputTestNew, withOneTestGroupAndOneTestFileShouldContainAnEmptyStderrBlock)
{
	testCaseRunner->start()
			.withGroupWithTest("groupname", "testname")
			.end();

	outputFile = fileSystem.file("cpputest_groupname.xml");
	STRCMP_EQUAL("<system-err></system-err>\n", outputFile->lineFromTheBack(2));
}

TEST(JUnitOutputTestNew, withOneTestGroupAndOneTestFileShouldContainsATestCaseBlock)
{
	testCaseRunner->start()
			.withGroupWithTest("groupname", "testname")
			.end();

	outputFile = fileSystem.file("cpputest_groupname.xml");

	STRCMP_EQUAL("<testcase classname=\"groupname\" name=\"testname\" time=\"0.000\">\n", outputFile->line(5));
	STRCMP_EQUAL("</testcase>\n", outputFile->line(6));
}

TEST(JUnitOutputTestNew, withOneTestGroupAndTwoTestCasesCreateCorrectTestgroupBlockAndCorrectTestCaseBlock)
{
	testCaseRunner->start()
			.withGroupWithTest("twoTestsGroup", "firstTestName").withTest("secondTestName")
			.end();

	outputFile = fileSystem.file("cpputest_twoTestsGroup.xml");

	STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"twoTestsGroup\" tests=\"2\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line(2));
	STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" time=\"0.000\">\n", outputFile->line(5));
	STRCMP_EQUAL("</testcase>\n", outputFile->line(6));
	STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" time=\"0.000\">\n", outputFile->line(7));
	STRCMP_EQUAL("</testcase>\n", outputFile->line(8));
}

TEST(JUnitOutputTestNew, withOneTestGroupAndTimeHasElapsedAndTimestampChanged)
{
	testCaseRunner->start().atTime("2013-07-04T22:28:00")
			.withGroupWithTest("timeGroup", "Dummy", 10)
			.end();

	outputFile = fileSystem.file("cpputest_timeGroup.xml");

	STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"timeGroup\" tests=\"1\" time=\"0.010\" timestamp=\"2013-07-04T22:28:00\">\n", outputFile->line(2));
}

TEST(JUnitOutputTestNew, withOneTestGroupAndMultipleTestCasesWithElapsedTime)
{
	testCaseRunner->start()
			.withGroupWithTest("twoTestsGroup", "firstTestName", 10)
				.withTest("secondTestName", 50)
			.end();

	outputFile = fileSystem.file("cpputest_twoTestsGroup.xml");
	STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"twoTestsGroup\" tests=\"2\" time=\"0.060\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line(2));
	STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" time=\"0.010\">\n", outputFile->line(5));
	STRCMP_EQUAL("</testcase>\n", outputFile->line(6));
	STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" time=\"0.050\">\n", outputFile->line(7));
	STRCMP_EQUAL("</testcase>\n", outputFile->line(8));
}

///////////////////// OLD CODE SHOULD GRADUALLY BE REMOVED //////////////////////////


class MockJUnitTestOutput: public JUnitTestOutput
{
public:
	enum
	{
		testGroupSize = 10
	};
	enum
	{
		defaultSize = 7
	};

	int filesOpened;
	int fileBalance;

	SimpleString fileName_;
	SimpleString buffer_;

	TestResult* res_;
	struct TestData
	{
		TestData() :
			tst_(0), testName_(0), failure_(0)
		{
		}

		UtestShell* tst_;
		SimpleString* testName_;
		TestFailure* failure_;
	};

	struct TestGroupData
	{
		TestGroupData() :
			numberTests_(0), totalFailures_(0), name_(""), testData_(0)
		{
		}

		size_t numberTests_;
		size_t totalFailures_;
		SimpleString name_;

		TestData* testData_;
	};

	TestGroupData testGroupData_[testGroupSize];

	TestGroupData& currentGroup()
	{
		return testGroupData_[filesOpened - 1];
	}

	void resetXmlFile()
	{
		buffer_ = "";
	}

	MockJUnitTestOutput() :
		filesOpened(0), fileBalance(0), res_(0)
	{
		for (int i = 0; i < testGroupSize; i++) {
			testGroupData_[i].numberTests_ = 0;
			testGroupData_[i].totalFailures_ = 0;
		}
	}

	void setResult(TestResult* testRes)
	{
		res_ = testRes;
	}

	virtual ~MockJUnitTestOutput()
	{
		for (size_t i = 0; i < testGroupSize; i++) {
			for (size_t j = 0; j < testGroupData_[i].numberTests_; j++) {
				delete testGroupData_[i].testData_[j].tst_;
				delete testGroupData_[i].testData_[j].testName_;
				if (testGroupData_[i].testData_[j].failure_) delete testGroupData_[i].testData_[j].failure_;
			}
			if (testGroupData_[i].testData_) delete[] testGroupData_[i].testData_;
		}

		LONGS_EQUAL(0, fileBalance);
	}

	void writeToFile(const SimpleString& buf)
	{
		buffer_ += buf;
	}

	void openFileForWrite(const SimpleString& in_FileName)
	{
		filesOpened++;
		fileBalance++;
		fileName_ = in_FileName;
	}

	void closeFile()
	{
		CHECK_XML_FILE();
		resetXmlFile();
		fileBalance--;
	}

	void createTestsInGroup(int index, size_t amount, const char* group, const char* basename)
	{
		testGroupData_[index].name_ = group;
		testGroupData_[index].numberTests_ = amount;

		testGroupData_[index].testData_ = new TestData[amount];
		for (size_t i = 0; i < amount; i++) {
			TestData& testData = testGroupData_[index].testData_[i];
			testData.testName_ = new SimpleString(basename);
			*testData.testName_ += StringFrom((long) i);
			testData.tst_ = new UtestShell(group, testData.testName_->asCharString(), "file", 1);
		}
	}
	void runTests()
	{
		res_->testsStarted();
		for (int i = 0; i < testGroupSize; i++) {
			TestGroupData& data = testGroupData_[i];
			if (data.numberTests_ == 0) continue;

			millisTime = 0;
			res_->currentGroupStarted(data.testData_[0].tst_);
			for (size_t j = 0; j < data.numberTests_; j++) {
				TestData& testData = data.testData_[j];

				millisTime = 0;
				res_->currentTestStarted(testData.tst_);
				if (testData.failure_) print(*testData.failure_);
				millisTime = 10;
				res_->currentTestEnded(testData.tst_);
			}
			millisTime = 50;
			res_->currentGroupEnded(data.testData_[0].tst_);
		}
		res_->testsEnded();
	}

	void setFailure(int groupIndex, int testIndex, const char* fileName, int lineNumber, const char* message)
	{
		TestData& data = testGroupData_[groupIndex].testData_[testIndex];
		data.failure_ = new TestFailure(data.tst_, fileName, lineNumber, message);
		testGroupData_[groupIndex].totalFailures_++;
	}

	void CHECK_TEST_SUITE_START(SimpleString out)
	{
		TestGroupData& group = currentGroup();
		SimpleString buf = StringFromFormat("<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" tests=\"%d\" time=\"0.050\" timestamp=\"%s\">\n", (int) group.totalFailures_,
				group.name_.asCharString(), (int) group.numberTests_, theTime);
		CHECK_EQUAL(buf, out);
	}

	void CHECK_XML_FILE()
	{
		SimpleStringCollection col;
		buffer_.split("\n", col);
		CHECK_TEST_SUITE_START(col[1]);
		CHECK_TESTS(&col[4]);
	}

	void CHECK_TESTS(SimpleString* arr)
	{
		for (size_t index = 0, curTest = 0; curTest < currentGroup().numberTests_; curTest++, index++) {
			SimpleString buf = StringFromFormat("<testcase classname=\"%s\" name=\"%s\" time=\"0.010\">\n", currentGroup().name_.asCharString(),
					currentGroup().testData_[curTest].tst_->getName().asCharString());
			CHECK_EQUAL(buf, arr[index]);
			if (currentGroup().testData_[curTest].failure_) {
				CHECK_FAILURE(arr, index, curTest);
			}
			buf = "</testcase>\n";
			CHECK_EQUAL(buf, arr[++index]);

		}
	}
	void CHECK_FAILURE(SimpleString* arr, size_t& i, size_t curTest)
	{
		TestFailure& f = *currentGroup().testData_[curTest].failure_;
		i++;
		SimpleString message = f.getMessage().asCharString();
		message.replace('"', '\'');
		message.replace('<', '[');
		message.replace('>', ']');
		message.replace("\n", "{newline}");
		SimpleString buf = StringFromFormat("<failure message=\"%s:%d: %s\" type=\"AssertionFailedError\">\n", f.getFileName().asCharString(), f.getFailureLineNumber(), message.asCharString());
		CHECK_EQUAL(buf, arr[i]);
		i++;
		STRCMP_EQUAL("</failure>\n", arr[i].asCharString());
	}
};


TEST_GROUP(JUnitOutputTest)
{

	MockJUnitTestOutput * output;
	TestResult *res;

	void setup()
	{
		output = new MockJUnitTestOutput();
		res = new TestResult(*output);
		output->setResult(res);
		SetPlatformSpecificTimeInMillisMethod(MockGetPlatformSpecificTimeInMillis);
		SetPlatformSpecificTimeStringMethod(MockGetPlatformSpecificTimeString);
	}
	void teardown()
	{
		delete output;
		delete res;
		SetPlatformSpecificTimeInMillisMethod(0);
		SetPlatformSpecificTimeStringMethod(0);
	}

	void runTests()
	{
		output->printTestsStarted();
		output->runTests();
		output->printTestsEnded(*res);
	}
};

TEST(JUnitOutputTest, fiveTestsInOneGroupAllPass)
{
	output->createTestsInGroup(0, 5, "group", "name");
	runTests();
}

TEST(JUnitOutputTest, multipleTestsInTwoGroupAllPass)
{
	output->createTestsInGroup(0, 3, "group", "name");
	output->createTestsInGroup(1, 8, "secondGroup", "secondName");
	runTests();
	LONGS_EQUAL(2, output->filesOpened);
}

TEST(JUnitOutputTest, oneTestInOneGroupFailed)
{
	output->createTestsInGroup(0, 1, "failedGroup", "failedName");
	output->setFailure(0, 0, "file", 1, "Test <\"just\"> failed");
	runTests();
}

TEST(JUnitOutputTest, fiveTestsInOneGroupAndThreeFail)
{
	output->printTestsStarted();
	output->createTestsInGroup(0, 5, "failedGroup", "failedName");
	output->setFailure(0, 0, "file", 1, "Test just failed");
	output->setFailure(0, 1, "file", 5, "Also failed");
	output->setFailure(0, 4, "file", 8, "And failed again");
	runTests();
}

TEST(JUnitOutputTest, fourGroupsAndSomePassAndSomeFail)
{
	output->printTestsStarted();
	output->createTestsInGroup(0, 5, "group1", "firstName");
	output->createTestsInGroup(1, 50, "group2", "secondName");
	output->createTestsInGroup(2, 3, "group3", "thirdName");
	output->createTestsInGroup(3, 5, "group4", "fourthName");

	output->setFailure(0, 0, "file", 1, "Test just failed");
	output->printTestsEnded(*res);
	runTests();
}

TEST(JUnitOutputTest, messageWithNewLine)
{
	output->createTestsInGroup(0, 1, "failedGroup", "failedName");
	output->setFailure(0, 0, "file", 1, "Test \n failed");
	runTests();
}

TEST(JUnitOutputTest, escapeSlashesInFilenames)
{
	STRCMP_EQUAL("cpputest_group_weird_name.xml", output->createFileName("group/weird/name").asCharString());
}


