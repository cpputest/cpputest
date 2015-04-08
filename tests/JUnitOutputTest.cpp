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
    ~FileSystemForJUnitTestOutputTests() { clear(); }

    void clear(void)
    {
        while (firstFile_) {
            FileForJUnitOutputTests* fileToBeDeleted = firstFile_;
            firstFile_ = firstFile_->nextFile();
            delete fileToBeDeleted;
        }
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

extern "C" {
    static long millisTime = 0;
    static const char* theTime = "";

    static long MockGetPlatformSpecificTimeInMillis()
    {
        return millisTime;
    }

    static const char* MockGetPlatformSpecificTimeString()
    {
        return theTime;
    }
}

class JUnitTestOutputTestRunner
{
    TestResult result_;

    const char* currentGroupName_;
    UtestShell* currentTest_;
    bool firstTestInGroup_;
    int timeTheTestTakes_;
    TestFailure* testFailure_;

public:

    JUnitTestOutputTestRunner(TestResult result) :
        result_(result), currentGroupName_(0), currentTest_(0), firstTestInGroup_(true), timeTheTestTakes_(0), testFailure_(0)
    {
        millisTime = 0;
        theTime =  "1978-10-03T00:00:00";

        UT_PTR_SET(GetPlatformSpecificTimeInMillis, MockGetPlatformSpecificTimeInMillis);
        UT_PTR_SET(GetPlatformSpecificTimeString, MockGetPlatformSpecificTimeString);
    }

    JUnitTestOutputTestRunner& start()
    {
        result_.testsStarted();
        return *this;
    }

    JUnitTestOutputTestRunner& end()
    {
        endOfPreviousTestGroup();
        result_.testsEnded();
        return *this;
    }

    void endOfPreviousTestGroup()
    {
        runPreviousTest();
        if (currentTest_) {
            result_.currentGroupEnded(currentTest_);
            firstTestInGroup_ = true;
            delete currentTest_;
            currentTest_ = 0;
        }

        currentGroupName_ = 0;
    }

    JUnitTestOutputTestRunner& withGroup(const char* groupName)
    {
        endOfPreviousTestGroup();

        currentGroupName_ = groupName;
        return *this;
    }

    JUnitTestOutputTestRunner& withTest(const char* testName)
    {
        runPreviousTest();

        delete currentTest_;

        currentTest_ = new UtestShell(currentGroupName_, testName, "file", 1);

        return *this;
    }

    JUnitTestOutputTestRunner& withIgnoredTest(const char* testName)
    {
        runPreviousTest();
        delete currentTest_;

        currentTest_ = new IgnoredUtestShell(currentGroupName_, testName, "file", 1);
        return *this;
    }

    void runPreviousTest()
    {
        if (currentTest_ == 0) return;

        if (firstTestInGroup_) {
            result_.currentGroupStarted(currentTest_);
            firstTestInGroup_ = false;
        }
        result_.currentTestStarted(currentTest_);

        millisTime += timeTheTestTakes_;

        if (testFailure_) {
            result_.addFailure(*testFailure_);
            delete testFailure_;
            testFailure_ = 0;
        }

        result_.currentTestEnded(currentTest_);
    }

    JUnitTestOutputTestRunner& thatTakes(int timeElapsed)
    {
        timeTheTestTakes_ = timeElapsed;
        return *this;
    }

    JUnitTestOutputTestRunner& seconds()
    {
        return *this;
    }

    JUnitTestOutputTestRunner& thatFails(const char* message, const char* file, int line)
    {
        testFailure_ = new TestFailure(	currentTest_, file, line, message);
        return *this;
    }

    JUnitTestOutputTestRunner& atTime(const char* newTime)
    {
        theTime = newTime;
        return *this;
    }
};

extern "C" {
    static FileSystemForJUnitTestOutputTests fileSystem;

    static PlatformSpecificFile mockFOpen(const char* filename, const char*)
    {
        return fileSystem.openFile(filename);
    }

    static void mockFPuts(const char* str, PlatformSpecificFile file)
    {
        ((FileForJUnitOutputTests*)file)->write(str);
    }

    static void mockFClose(PlatformSpecificFile file)
    {
        ((FileForJUnitOutputTests*)file)->close();
    }
}

TEST_GROUP(JUnitOutputTest)
{
    StringBufferJUnitTestOutput *junitOutput;
    TestResult *result;
    JUnitTestOutputTestRunner *testCaseRunner;
    FileForJUnitOutputTests* outputFile;

    void setup()
    {
        UT_PTR_SET(PlatformSpecificFOpen, (PlatformSpecificFile(*)(const char*, const char*))mockFOpen);
        UT_PTR_SET(PlatformSpecificFPuts, mockFPuts);
        UT_PTR_SET(PlatformSpecificFClose, mockFClose);
        junitOutput = new StringBufferJUnitTestOutput();
        result = new TestResult(*junitOutput);
        testCaseRunner = new JUnitTestOutputTestRunner(*result);
        TestOutput::setWorkingEnvironment( TestOutput::vistualStudio );
    }

    void teardown()
    {
        delete testCaseRunner;
        delete result;
        delete junitOutput;
        fileSystem.clear();
        TestOutput::setWorkingEnvironment( TestOutput::detectEnvironment );
    }
};

TEST(JUnitOutputTest, withOneTestGroupAndOneTestOnlyWriteToOneFile)
{
    testCaseRunner->start()
            .withGroup("groupname").withTest("testname")
            .end();

    LONGS_EQUAL(1, fileSystem.amountOfFiles());
    CHECK(fileSystem.fileExists("cpputest_groupname.xml"));
}

TEST( JUnitOutputTest, withOneTestGroupAndOneTestOnlyWriteToOneFileWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "groupname" ).withTest( "testname" )
        .end();

    LONGS_EQUAL( 1, fileSystem.amountOfFiles() );
    CHECK( fileSystem.fileExists( "cpputest_groupname.xml" ) );

    STRCMP_EQUAL( "TEST(groupname, testname) - 0 ms\n"
        "\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST( JUnitOutputTest, withOneTestGroupAndOneTestOutputsValidXMLFiles ) {
    testCaseRunner->start()
        .withGroup( "groupname" ).withTest( "testname" )
        .end();

    outputFile = fileSystem.file( "cpputest_groupname.xml" );
    STRCMP_EQUAL( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n", outputFile->line( 1 ) );
}

TEST(JUnitOutputTest, withOneTestGroupAndOneTestOutputsValidXMLFilesWithVerbose){
    junitOutput->verbose();
    testCaseRunner->start()
            .withGroup("groupname").withTest("testname")
            .end();

    outputFile = fileSystem.file("cpputest_groupname.xml");
    STRCMP_EQUAL("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n", outputFile->line(1));

    STRCMP_EQUAL( "TEST(groupname, testname) - 0 ms\n"
        "\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, withOneTestGroupAndOneTestoutputsTestSuiteStartAndEndBlocks)
{
    testCaseRunner->start()
            .withGroup("groupname").withTest("testname")
            .end();

    outputFile = fileSystem.file("cpputest_groupname.xml");
    STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"groupname\" tests=\"1\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line(2));
    STRCMP_EQUAL("</testsuite>", outputFile->lineFromTheBack(1));
}

TEST( JUnitOutputTest, withOneTestGroupAndOneTestoutputsTestSuiteStartAndEndBlocksWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "groupname" ).withTest( "testname" )
        .end();

    outputFile = fileSystem.file( "cpputest_groupname.xml" );
    STRCMP_EQUAL( "<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"groupname\" tests=\"1\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line( 2 ) );
    STRCMP_EQUAL( "</testsuite>", outputFile->lineFromTheBack( 1 ) );

    STRCMP_EQUAL( "TEST(groupname, testname) - 0 ms\n"
        "\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, withOneTestGroupAndOneTestFileShouldContainAnEmptyPropertiesBlock)
{
    testCaseRunner->start()
            .withGroup("groupname").withTest("testname")
            .end();

    outputFile = fileSystem.file("cpputest_groupname.xml");
    STRCMP_EQUAL("<properties>\n", outputFile->line(3));
    STRCMP_EQUAL("</properties>\n", outputFile->line(4));
}

TEST( JUnitOutputTest, withOneTestGroupAndOneTestFileShouldContainAnEmptyPropertiesBlockWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "groupname" ).withTest( "testname" )
        .end();

    outputFile = fileSystem.file( "cpputest_groupname.xml" );
    STRCMP_EQUAL( "<properties>\n", outputFile->line( 3 ) );
    STRCMP_EQUAL( "</properties>\n", outputFile->line( 4 ) );

    STRCMP_EQUAL( "TEST(groupname, testname) - 0 ms\n"
        "\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, withOneTestGroupAndOneTestFileShouldContainAnEmptyStdoutBlock)
{
    testCaseRunner->start()
            .withGroup("groupname").withTest("testname")
            .end();

    outputFile = fileSystem.file("cpputest_groupname.xml");
    STRCMP_EQUAL("<system-out></system-out>\n", outputFile->lineFromTheBack(3));
}

TEST( JUnitOutputTest, withOneTestGroupAndOneTestFileShouldContainAnEmptyStdoutBlockWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "groupname" ).withTest( "testname" )
        .end();

    outputFile = fileSystem.file( "cpputest_groupname.xml" );
    STRCMP_EQUAL( "<system-out></system-out>\n", outputFile->lineFromTheBack( 3 ) );

    STRCMP_EQUAL( "TEST(groupname, testname) - 0 ms\n"
        "\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, withOneTestGroupAndOneTestFileShouldContainAnEmptyStderrBlock)
{
    testCaseRunner->start()
            .withGroup("groupname").withTest("testname")
            .end();

    outputFile = fileSystem.file("cpputest_groupname.xml");
    STRCMP_EQUAL("<system-err></system-err>\n", outputFile->lineFromTheBack(2));
}

TEST( JUnitOutputTest, withOneTestGroupAndOneTestFileShouldContainAnEmptyStderrBlockWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "groupname" ).withTest( "testname" )
        .end();

    outputFile = fileSystem.file( "cpputest_groupname.xml" );
    STRCMP_EQUAL( "<system-err></system-err>\n", outputFile->lineFromTheBack( 2 ) );

    STRCMP_EQUAL( "TEST(groupname, testname) - 0 ms\n"
        "\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, withOneTestGroupAndOneTestFileShouldContainsATestCaseBlock)
{
    testCaseRunner->start()
            .withGroup("groupname").withTest("testname")
            .end();

    outputFile = fileSystem.file("cpputest_groupname.xml");

    STRCMP_EQUAL("<testcase classname=\"groupname\" name=\"testname\" time=\"0.000\">\n", outputFile->line(5));
    STRCMP_EQUAL("</testcase>\n", outputFile->line(6));
}

TEST( JUnitOutputTest, withOneTestGroupAndOneTestFileShouldContainsATestCaseBlockWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "groupname" ).withTest( "testname" )
        .end();

    outputFile = fileSystem.file( "cpputest_groupname.xml" );

    STRCMP_EQUAL( "<testcase classname=\"groupname\" name=\"testname\" time=\"0.000\">\n", outputFile->line( 5 ) );
    STRCMP_EQUAL( "</testcase>\n", outputFile->line( 6 ) );

    STRCMP_EQUAL( "TEST(groupname, testname) - 0 ms\n"
        "\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST( JUnitOutputTest, withOneTestGroupAndTwoTestCasesCreateCorrectTestgroupBlockAndCorrectTestCaseBlock ) {
    testCaseRunner->start()
        .withGroup( "twoTestsGroup" ).withTest( "firstTestName" ).withTest( "secondTestName" )
        .end();

    outputFile = fileSystem.file( "cpputest_twoTestsGroup.xml" );

    STRCMP_EQUAL( "<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"twoTestsGroup\" tests=\"2\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line( 2 ) );
    STRCMP_EQUAL( "<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" time=\"0.000\">\n", outputFile->line( 5 ) );
    STRCMP_EQUAL( "</testcase>\n", outputFile->line( 6 ) );
    STRCMP_EQUAL( "<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" time=\"0.000\">\n", outputFile->line( 7 ) );
    STRCMP_EQUAL( "</testcase>\n", outputFile->line( 8 ) );
}

TEST(JUnitOutputTest, withOneTestGroupAndTwoTestCasesCreateCorrectTestgroupBlockAndCorrectTestCaseBlockWithVerbose)
{
    junitOutput->verbose();
    testCaseRunner->start()
            .withGroup("twoTestsGroup").withTest("firstTestName").withTest("secondTestName")
            .end();

    outputFile = fileSystem.file("cpputest_twoTestsGroup.xml");

    STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"twoTestsGroup\" tests=\"2\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line(2));
    STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" time=\"0.000\">\n", outputFile->line(5));
    STRCMP_EQUAL("</testcase>\n", outputFile->line(6));
    STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" time=\"0.000\">\n", outputFile->line(7));
    STRCMP_EQUAL("</testcase>\n", outputFile->line(8));

    STRCMP_EQUAL( "TEST(twoTestsGroup, firstTestName) - 0 ms\nTEST(twoTestsGroup, secondTestName) - 0 ms\n"
        "\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, withOneTestGroupAndTimeHasElapsedAndTimestampChanged)
{
    testCaseRunner->start().atTime("2013-07-04T22:28:00")
            .withGroup("timeGroup").withTest("Dummy").thatTakes(10).seconds()
            .end();

    outputFile = fileSystem.file("cpputest_timeGroup.xml");

    STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"timeGroup\" tests=\"1\" time=\"0.010\" timestamp=\"2013-07-04T22:28:00\">\n", outputFile->line(2));
}

TEST( JUnitOutputTest, withOneTestGroupAndTimeHasElapsedAndTimestampChangedWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start().atTime( "2013-07-04T22:28:00" )
        .withGroup( "timeGroup" ).withTest( "Dummy" ).thatTakes( 10 ).seconds()
        .end();

    outputFile = fileSystem.file( "cpputest_timeGroup.xml" );

    STRCMP_EQUAL( "<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"timeGroup\" tests=\"1\" time=\"0.010\" timestamp=\"2013-07-04T22:28:00\">\n", outputFile->line( 2 ) );

    STRCMP_EQUAL( "TEST(timeGroup, Dummy) - 10 ms"
        "\n\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 10 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, withOneTestGroupAndMultipleTestCasesWithElapsedTime)
{
    testCaseRunner->start()
            .withGroup("twoTestsGroup")
                .withTest("firstTestName").thatTakes(10).seconds()
                .withTest("secondTestName").thatTakes(50).seconds()
            .end();

    outputFile = fileSystem.file("cpputest_twoTestsGroup.xml");
    STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"twoTestsGroup\" tests=\"2\" time=\"0.060\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line(2));
    STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" time=\"0.010\">\n", outputFile->line(5));
    STRCMP_EQUAL("</testcase>\n", outputFile->line(6));
    STRCMP_EQUAL("<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" time=\"0.050\">\n", outputFile->line(7));
    STRCMP_EQUAL("</testcase>\n", outputFile->line(8));
}

TEST( JUnitOutputTest, withOneTestGroupAndMultipleTestCasesWithElapsedTimeWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "twoTestsGroup" )
        .withTest( "firstTestName" ).thatTakes( 10 ).seconds()
        .withTest( "secondTestName" ).thatTakes( 50 ).seconds()
        .end();

    outputFile = fileSystem.file( "cpputest_twoTestsGroup.xml" );
    STRCMP_EQUAL( "<testsuite errors=\"0\" failures=\"0\" hostname=\"localhost\" name=\"twoTestsGroup\" tests=\"2\" time=\"0.060\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line( 2 ) );
    STRCMP_EQUAL( "<testcase classname=\"twoTestsGroup\" name=\"firstTestName\" time=\"0.010\">\n", outputFile->line( 5 ) );
    STRCMP_EQUAL( "</testcase>\n", outputFile->line( 6 ) );
    STRCMP_EQUAL( "<testcase classname=\"twoTestsGroup\" name=\"secondTestName\" time=\"0.050\">\n", outputFile->line( 7 ) );
    STRCMP_EQUAL( "</testcase>\n", outputFile->line( 8 ) );

    STRCMP_EQUAL( "TEST(twoTestsGroup, firstTestName) - 10 ms\n"
        "TEST(twoTestsGroup, secondTestName) - 50 ms"
        "\n\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 60 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, withOneTestGroupAndOneFailingTest)
{
    testCaseRunner->start()
            .withGroup("testGroupWithFailingTest")
                .withTest("FailingTestName").thatFails("Test failed", "thisfile", 10)
            .end();

    outputFile = fileSystem.file("cpputest_testGroupWithFailingTest.xml");
    STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"1\" hostname=\"localhost\" name=\"testGroupWithFailingTest\" tests=\"1\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line(2));
    STRCMP_EQUAL("<testcase classname=\"testGroupWithFailingTest\" name=\"FailingTestName\" time=\"0.000\">\n", outputFile->line(5));
    STRCMP_EQUAL("<failure message=\"thisfile:10: Test failed\" type=\"AssertionFailedError\">\n", outputFile->line(6));
    STRCMP_EQUAL("</failure>\n", outputFile->line(7));
    STRCMP_EQUAL("</testcase>\n", outputFile->line(8));
}

TEST( JUnitOutputTest, withOneTestGroupAndOneFailingTestWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "testGroupWithFailingTest" )
        .withTest( "FailingTestName" ).thatFails( "Test failed", "thisfile", 10 )
        .end();

    outputFile = fileSystem.file( "cpputest_testGroupWithFailingTest.xml" );
    STRCMP_EQUAL( "<testsuite errors=\"0\" failures=\"1\" hostname=\"localhost\" name=\"testGroupWithFailingTest\" tests=\"1\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line( 2 ) );
    STRCMP_EQUAL( "<testcase classname=\"testGroupWithFailingTest\" name=\"FailingTestName\" time=\"0.000\">\n", outputFile->line( 5 ) );
    STRCMP_EQUAL( "<failure message=\"thisfile:10: Test failed\" type=\"AssertionFailedError\">\n", outputFile->line( 6 ) );
    STRCMP_EQUAL( "</failure>\n", outputFile->line( 7 ) );
    STRCMP_EQUAL( "</testcase>\n", outputFile->line( 8 ) );

    STRCMP_EQUAL( "TEST(testGroupWithFailingTest, FailingTestName)\nfile(1): error: Failure in TEST(testGroupWithFailingTest, FailingTestName)\nthisfile(10): error:\n\tTest failed\n\n - 0 ms"
        "\n\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST( JUnitOutputTest, withTwoTestGroupAndOneFailingTest ) {
    testCaseRunner->start()
        .withGroup( "testGroupWithFailingTest" )
        .withTest( "FirstTest" )
        .withTest( "FailingTestName" ).thatFails( "Test failed", "thisfile", 10 )
        .end();

    outputFile = fileSystem.file( "cpputest_testGroupWithFailingTest.xml" );

    STRCMP_EQUAL( "<testsuite errors=\"0\" failures=\"1\" hostname=\"localhost\" name=\"testGroupWithFailingTest\" tests=\"2\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line( 2 ) );
    STRCMP_EQUAL( "<testcase classname=\"testGroupWithFailingTest\" name=\"FailingTestName\" time=\"0.000\">\n", outputFile->line( 7 ) );
    STRCMP_EQUAL( "<failure message=\"thisfile:10: Test failed\" type=\"AssertionFailedError\">\n", outputFile->line( 8 ) );
}

TEST( JUnitOutputTest, withTwoTestGroupAndOneFailingTestWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
            .withGroup("testGroupWithFailingTest")
                .withTest("FirstTest")
                .withTest("FailingTestName").thatFails("Test failed", "thisfile", 10)
            .end();

    outputFile = fileSystem.file("cpputest_testGroupWithFailingTest.xml");

    STRCMP_EQUAL("<testsuite errors=\"0\" failures=\"1\" hostname=\"localhost\" name=\"testGroupWithFailingTest\" tests=\"2\" time=\"0.000\" timestamp=\"1978-10-03T00:00:00\">\n", outputFile->line(2));
    STRCMP_EQUAL("<testcase classname=\"testGroupWithFailingTest\" name=\"FailingTestName\" time=\"0.000\">\n", outputFile->line(7));
    STRCMP_EQUAL( "<failure message=\"thisfile:10: Test failed\" type=\"AssertionFailedError\">\n", outputFile->line( 8 ) );

    STRCMP_EQUAL( "TEST(testGroupWithFailingTest, FirstTest) - 0 ms\nTEST(testGroupWithFailingTest, FailingTestName)\nfile(1): error: Failure in TEST(testGroupWithFailingTest, FailingTestName)\n"
        "thisfile(10): error:\n\tTest failed\n\n - 0 ms"
        "\n\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, testFailureWithLessThanAndGreaterThanInsideIt)
{
    testCaseRunner->start()
            .withGroup("testGroupWithFailingTest")
                .withTest("FailingTestName").thatFails("Test <failed>", "thisfile", 10)
            .end();

    outputFile = fileSystem.file("cpputest_testGroupWithFailingTest.xml");

    STRCMP_EQUAL("<failure message=\"thisfile:10: Test [failed]\" type=\"AssertionFailedError\">\n", outputFile->line(6));
}

TEST( JUnitOutputTest, testFailureWithLessThanAndGreaterThanInsideItWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "testGroupWithFailingTest" )
        .withTest( "FailingTestName" ).thatFails( "Test <failed>", "thisfile", 10 )
        .end();

    outputFile = fileSystem.file( "cpputest_testGroupWithFailingTest.xml" );

    STRCMP_EQUAL( "<failure message=\"thisfile:10: Test [failed]\" type=\"AssertionFailedError\">\n", outputFile->line( 6 ) );

    STRCMP_EQUAL( "TEST(testGroupWithFailingTest, FailingTestName)\nfile(1): error: Failure in TEST(testGroupWithFailingTest, FailingTestName)\nthisfile(10): error:\n\tTest <failed>\n\n - 0 ms"
        "\n\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST( JUnitOutputTest, testFailureWithQuotesInIt ) {
    testCaseRunner->start()
        .withGroup( "testGroupWithFailingTest" )
        .withTest( "FailingTestName" ).thatFails( "Test \"failed\"", "thisfile", 10 )
        .end();

    outputFile = fileSystem.file( "cpputest_testGroupWithFailingTest.xml" );

    STRCMP_EQUAL( "<failure message=\"thisfile:10: Test 'failed'\" type=\"AssertionFailedError\">\n", outputFile->line( 6 ) );
}

TEST(JUnitOutputTest, testFailureWithQuotesInItWithVerbose) {
    junitOutput->verbose();
    testCaseRunner->start()
            .withGroup("testGroupWithFailingTest")
                .withTest("FailingTestName").thatFails("Test \"failed\"", "thisfile", 10)
            .end();

    outputFile = fileSystem.file("cpputest_testGroupWithFailingTest.xml");

    STRCMP_EQUAL( "<failure message=\"thisfile:10: Test 'failed'\" type=\"AssertionFailedError\">\n", outputFile->line( 6 ) );

    STRCMP_EQUAL( "TEST(testGroupWithFailingTest, FailingTestName)\nfile(1): error: Failure in TEST(testGroupWithFailingTest, FailingTestName)\nthisfile(10): error:\n\tTest \"failed\"\n\n - 0 ms"
        "\n\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, testFailureWithNewlineInIt)
{
    testCaseRunner->start()
            .withGroup("testGroupWithFailingTest")
                .withTest("FailingTestName").thatFails("Test \nfailed", "thisfile", 10)
            .end();

    outputFile = fileSystem.file("cpputest_testGroupWithFailingTest.xml");

    STRCMP_EQUAL("<failure message=\"thisfile:10: Test {newline}failed\" type=\"AssertionFailedError\">\n", outputFile->line(6));
}


TEST( JUnitOutputTest, testFailureWithNewlineInItWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "testGroupWithFailingTest" )
        .withTest( "FailingTestName" ).thatFails( "Test \nfailed", "thisfile", 10 )
        .end();

    outputFile = fileSystem.file( "cpputest_testGroupWithFailingTest.xml" );

    STRCMP_EQUAL( "<failure message=\"thisfile:10: Test {newline}failed\" type=\"AssertionFailedError\">\n", outputFile->line( 6 ) );

    STRCMP_EQUAL( "TEST(testGroupWithFailingTest, FailingTestName)\nfile(1): error: Failure in TEST(testGroupWithFailingTest, FailingTestName)\nthisfile(10): error:\n\tTest \nfailed\n\n - 0 ms"
        "\n\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, testFailureWithDifferentFileAndLine)
{
    testCaseRunner->start()
            .withGroup("testGroupWithFailingTest")
                .withTest("FailingTestName").thatFails("Test failed", "importantFile", 999)
            .end();

    outputFile = fileSystem.file("cpputest_testGroupWithFailingTest.xml");

    STRCMP_EQUAL("<failure message=\"importantFile:999: Test failed\" type=\"AssertionFailedError\">\n", outputFile->line(6));
}

TEST( JUnitOutputTest, testFailureWithDifferentFileAndLineWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "testGroupWithFailingTest" )
        .withTest( "FailingTestName" ).thatFails( "Test failed", "importantFile", 999 )
        .end();

    outputFile = fileSystem.file( "cpputest_testGroupWithFailingTest.xml" );

    STRCMP_EQUAL( "<failure message=\"importantFile:999: Test failed\" type=\"AssertionFailedError\">\n", outputFile->line( 6 ) );

    STRCMP_EQUAL( "TEST(testGroupWithFailingTest, FailingTestName)\nfile(1): error: Failure in TEST(testGroupWithFailingTest, FailingTestName)\nimportantFile(999): error:\n\tTest failed\n\n - 0 ms"
        "\n\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, testFailureWithAmpersands)
{
    testCaseRunner->start()
            .withGroup("testGroupWithFailingTest")
                .withTest("FailingTestName").thatFails("&object1 != &object2", "importantFile", 999)
            .end();

    outputFile = fileSystem.file("cpputest_testGroupWithFailingTest.xml");

    STRCMP_EQUAL("<failure message=\"importantFile:999: &amp;object1 != &amp;object2\" type=\"AssertionFailedError\">\n", outputFile->line(6));
}

TEST( JUnitOutputTest, testFailureWithAmpersandsWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "testGroupWithFailingTest" )
        .withTest( "FailingTestName" ).thatFails( "&object1 != &object2", "importantFile", 999 )
        .end();

    outputFile = fileSystem.file( "cpputest_testGroupWithFailingTest.xml" );

    STRCMP_EQUAL( "<failure message=\"importantFile:999: &amp;object1 != &amp;object2\" type=\"AssertionFailedError\">\n", outputFile->line( 6 ) );
    STRCMP_EQUAL( "TEST(testGroupWithFailingTest, FailingTestName)\nfile(1): error: Failure in TEST(testGroupWithFailingTest, FailingTestName)\nimportantFile(999): error:\n\t&object1 != &object2\n\n - 0 ms"
        "\n\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, aCoupleOfTestFailures)
{
    testCaseRunner->start()
            .withGroup("testGroup")
                .withTest("passingOne")
                .withTest("FailingTest").thatFails("Failure", "file", 99)
                .withTest("passingTwo")
                .withTest("passingThree")
                .withTest("AnotherFailingTest").thatFails("otherFailure", "anotherFile", 10)
            .end();

    outputFile = fileSystem.file("cpputest_testGroup.xml");

    STRCMP_EQUAL("<failure message=\"file:99: Failure\" type=\"AssertionFailedError\">\n", outputFile->line(8));
    STRCMP_EQUAL("<failure message=\"anotherFile:10: otherFailure\" type=\"AssertionFailedError\">\n", outputFile->line(16));
}

TEST( JUnitOutputTest, aCoupleOfTestFailuresWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "testGroup" )
        .withTest( "passingOne" )
        .withTest( "FailingTest" ).thatFails( "Failure", "file", 99 )
        .withTest( "passingTwo" )
        .withTest( "passingThree" )
        .withTest( "AnotherFailingTest" ).thatFails( "otherFailure", "anotherFile", 10 )
        .end();

    outputFile = fileSystem.file( "cpputest_testGroup.xml" );

    STRCMP_EQUAL( "<failure message=\"file:99: Failure\" type=\"AssertionFailedError\">\n", outputFile->line( 8 ) );
    STRCMP_EQUAL( "<failure message=\"anotherFile:10: otherFailure\" type=\"AssertionFailedError\">\n", outputFile->line( 16 ) );
    STRCMP_EQUAL( "TEST(testGroup, passingOne) - 0 ms\nTEST(testGroup, FailingTest)\nfile(99): error: Failure in TEST(testGroup, FailingTest)\n\tFailure\n\n - 0 ms\nTEST(testGroup, passingTwo)"
        " - 0 ms\nTEST(testGroup, passingThree) - 0 ms\nTEST(testGroup, AnotherFailingTest)\nfile(1): error: Failure in TEST(testGroup, AnotherFailingTest)\nanotherFile(10): error:\n"
        "\totherFailure\n\n - 0 ms\n\nErrors (2 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, testFailuresInSeparateGroups)
{
    testCaseRunner->start()
            .withGroup("testGroup")
                .withTest("passingOne")
                .withTest("FailingTest").thatFails("Failure", "file", 99)
            .withGroup("AnotherGroup")
                .withTest("AnotherFailingTest").thatFails("otherFailure", "anotherFile", 10)
            .end();

    outputFile = fileSystem.file("cpputest_testGroup.xml");

    STRCMP_EQUAL("<failure message=\"file:99: Failure\" type=\"AssertionFailedError\">\n", outputFile->line(8));

    outputFile = fileSystem.file("cpputest_AnotherGroup.xml");
    STRCMP_EQUAL("<failure message=\"anotherFile:10: otherFailure\" type=\"AssertionFailedError\">\n", outputFile->line(6));
}

TEST( JUnitOutputTest, testFailuresInSeparateGroupsWithVerbose) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "testGroup" )
        .withTest( "passingOne" )
        .withTest( "FailingTest" ).thatFails( "Failure", "file", 99 )
        .withGroup( "AnotherGroup" )
        .withTest( "AnotherFailingTest" ).thatFails( "otherFailure", "anotherFile", 10 )
        .end();

    outputFile = fileSystem.file( "cpputest_testGroup.xml" );

    STRCMP_EQUAL( "<failure message=\"file:99: Failure\" type=\"AssertionFailedError\">\n", outputFile->line( 8 ) );

    outputFile = fileSystem.file( "cpputest_AnotherGroup.xml" );
    STRCMP_EQUAL( "<failure message=\"anotherFile:10: otherFailure\" type=\"AssertionFailedError\">\n", outputFile->line( 6 ) );
    STRCMP_EQUAL( "TEST(testGroup, passingOne) - 0 ms\nTEST(testGroup, FailingTest)\nfile(99): error: Failure in TEST(testGroup, FailingTest)\n\tFailure\n\n - 0 ms\n"
        "TEST(AnotherGroup, AnotherFailingTest)\nfile(1): error: Failure in TEST(AnotherGroup, AnotherFailingTest)\nanotherFile(10): error:\n"
        "\totherFailure\n\n - 0 ms\n\nErrors (2 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, twoTestGroupsWriteToTwoDifferentFiles)
{
    testCaseRunner->start()
            .withGroup("firstTestGroup")
                .withTest("testName")
            .withGroup("secondTestGroup")
                .withTest("testName")
            .end();

    CHECK(fileSystem.file("cpputest_firstTestGroup.xml"));
    CHECK(fileSystem.file("cpputest_secondTestGroup.xml"));

}

TEST( JUnitOutputTest, twoTestGroupsWriteToTwoDifferentFilesWithVerbose ) {
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "firstTestGroup" )
        .withTest( "testName" )
        .withGroup( "secondTestGroup" )
        .withTest( "testName" )
        .end();

    CHECK( fileSystem.file( "cpputest_firstTestGroup.xml" ) );
    CHECK( fileSystem.file( "cpputest_secondTestGroup.xml" ) );
    STRCMP_EQUAL( "TEST(firstTestGroup, testName) - 0 ms\n"
        "TEST(secondTestGroup, testName) - 0 ms\n\n"
        "OK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, testGroupWithWeirdName)
{
    STRCMP_EQUAL("cpputest_group_weird_name.xml", junitOutput->createFileName("group/weird/name").asCharString());
}

TEST(JUnitOutputTest, TestCaseBlockWithAPackageName)
{
    junitOutput->setPackageName("packagename");
    testCaseRunner->start()
            .withGroup("groupname").withTest("testname")
            .end();

    outputFile = fileSystem.file("cpputest_groupname.xml");

    STRCMP_EQUAL("<testcase classname=\"packagename.groupname\" name=\"testname\" time=\"0.000\">\n", outputFile->line(5));
    STRCMP_EQUAL("</testcase>\n", outputFile->line(6));
}

TEST( JUnitOutputTest, TestCaseBlockWithAPackageNameWithVerbose) {
    junitOutput->setPackageName( "packagename" );
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "groupname" ).withTest( "testname" )
        .end();

    outputFile = fileSystem.file( "cpputest_groupname.xml" );

    STRCMP_EQUAL( "<testcase classname=\"packagename.groupname\" name=\"testname\" time=\"0.000\">\n", outputFile->line( 5 ) );
    STRCMP_EQUAL( "</testcase>\n", outputFile->line( 6 ) );
    STRCMP_EQUAL( "TEST(groupname, testname) - 0 ms\n\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}

TEST(JUnitOutputTest, TestCaseBlockForIgnoredTest)
{
   junitOutput->setPackageName("packagename");
   testCaseRunner->start()
      .withGroup("groupname").withIgnoredTest("testname")
      .end();

   outputFile = fileSystem.file("cpputest_groupname.xml");

   STRCMP_EQUAL("<testcase classname=\"packagename.groupname\" name=\"testname\" time=\"0.000\">\n", outputFile->line(5));
   STRCMP_EQUAL("<skipped />\n", outputFile->line(6));
   STRCMP_EQUAL("</testcase>\n", outputFile->line(7));
}

TEST( JUnitOutputTest, TestCaseBlockForIgnoredTestWithVerbose )
{
    junitOutput->setPackageName( "packagename" );
    junitOutput->verbose();
    testCaseRunner->start()
        .withGroup( "groupname" ).withIgnoredTest( "testname" )
        .end();

    outputFile = fileSystem.file( "cpputest_groupname.xml" );

    STRCMP_EQUAL( "<testcase classname=\"packagename.groupname\" name=\"testname\" time=\"0.000\">\n", outputFile->line( 5 ) );
    STRCMP_EQUAL( "<skipped />\n", outputFile->line( 6 ) );
    STRCMP_EQUAL( "</testcase>\n", outputFile->line( 7 ) );
    STRCMP_EQUAL( "IGNORE_TEST(groupname, testname) - 0 ms\n\nOK (0 tests, 0 ran, 0 checks, 0 ignored, 0 filtered out, 0 ms)\n\n", junitOutput->getOutput().asCharString() );
}
