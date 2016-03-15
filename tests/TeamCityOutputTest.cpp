#include "CppUTest/TestHarness.h"
#include "CppUTest/TeamCityTestOutput.h"
#include "CppUTest/PlatformSpecificFunctions.h"

class TeamCityOutputToBuffer : public TeamCityTestOutput
{
public:
    explicit TeamCityOutputToBuffer()
    {
    }

    virtual ~TeamCityOutputToBuffer()
    {
    }

    void printBuffer(const char* s) _override
    {
        output += s;
    }

    void flush() _override
    {
        output = "";
    }

    const SimpleString& getOutput()
    {
        return output;
    }

private:
    SimpleString output;
};

static long millisTime;

extern "C" {

    static long MockGetPlatformSpecificTimeInMillis()
    {
        return millisTime;
    }

}

TEST_GROUP(TeamCityOutputTest)
{
    TeamCityTestOutput* tcout;
    TeamCityOutputToBuffer* mock;
    UtestShell* tst;
    TestFailure *f, *f2, *f3;
    TestResult* result;

    void setup()
    {
        mock = new TeamCityOutputToBuffer();
        tcout = mock;
        tst = new UtestShell("group", "test", "file", 10);
        f = new TestFailure(tst, "failfile", 20, "failure message");
        f2 = new TestFailure(tst, "file", 20, "message");
        f3 = new TestFailure(tst, "file", 30, "apos' pipe| [brackets]\r\nCRLF");
        result = new TestResult(*mock);
        result->setTotalExecutionTime(10);
        millisTime = 0;
        UT_PTR_SET(GetPlatformSpecificTimeInMillis, MockGetPlatformSpecificTimeInMillis);
    }
    void teardown()
    {
        delete tcout;
        delete tst;
        delete f;
        delete f2;
        delete f3;
        delete result;
    }
};

TEST(TeamCityOutputTest, PrintGroupStarted)
{
    result->currentGroupStarted(tst);
    STRCMP_EQUAL("##teamcity[testSuiteStarted name='group']\n", mock->getOutput().asCharString());
}

TEST(TeamCityOutputTest, PrintGroupStartedAndEnded)
{
    const char* expected = "##teamcity[testSuiteStarted name='group']\n"
        "##teamcity[testSuiteFinished name='group']\n";
    result->currentGroupStarted(tst);
    result->currentGroupEnded(tst);
    STRCMP_EQUAL(expected, mock->getOutput().asCharString());
}

TEST(TeamCityOutputTest, PrintGroupEndedButNotStarted)
{
    result->currentGroupEnded(tst);
    STRCMP_EQUAL("", mock->getOutput().asCharString());
}

TEST(TeamCityOutputTest, PrintTestStarted)
{
    tcout->printCurrentTestStarted(*tst);
    STRCMP_EQUAL("##teamcity[testStarted name='test']\n", mock->getOutput().asCharString());
}

TEST(TeamCityOutputTest, PrintTestStartedAndEnded)
{
    result->currentTestStarted(tst);
    millisTime = 42;
    result->currentTestEnded(tst);
    STRCMP_EQUAL("##teamcity[testStarted name='test']\n##teamcity[testFinished name='test' duration='42']\n",
       mock->getOutput().asCharString());
}

TEST(TeamCityOutputTest, PrintTestEndedButNotStarted)
{
    result->currentTestEnded(tst);
    STRCMP_EQUAL("", mock->getOutput().asCharString());
}

TEST(TeamCityOutputTest, PrintTestIgnored)
{
    const char* expected =
        "##teamcity[testStarted name='test']\n"
        "##teamcity[testIgnored name='test']\n"
        "##teamcity[testFinished name='test' duration='41']\n";

    IgnoredUtestShell* itst = new IgnoredUtestShell("group", "test", "file", 10);
    result->currentTestStarted(itst);
    millisTime = 41;
    result->currentTestEnded(itst);
    STRCMP_EQUAL(expected, mock->getOutput().asCharString());
    delete itst;
}

TEST(TeamCityOutputTest, PrintWithFailureInSameFile)
{
    tcout->printFailure(*f2);
    const char* expected =
            "##teamcity[testFailed name='test' message='file:20' "
            "details='message']\n";
    STRCMP_EQUAL(expected, mock->getOutput().asCharString());
}

TEST(TeamCityOutputTest, PrintWithEscapedCharacters)
{
    tcout->printFailure(*f3);
    const char* expected =
            "##teamcity[testFailed name='test' message='file:30' "
            "details='apos|' pipe|| |[brackets|]"
            "|r|nCRLF']\n";
    STRCMP_EQUAL(expected, mock->getOutput().asCharString());
}

TEST(TeamCityOutputTest, PrintFailureWithFailInDifferentFile)
{
    tcout->printFailure(*f);
    const char* expected =
            "##teamcity[testFailed name='test' message='TEST failed (file:10): failfile:20' "
            "details='failure message']\n";
    STRCMP_EQUAL(expected, mock->getOutput().asCharString());
}

/* Todo:
 * -Detect when running in TeamCity and switch output to -o teamcity automatically
 */
