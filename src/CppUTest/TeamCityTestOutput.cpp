#include "CppUTest/TestHarness.h"
#include "CppUTest/TeamCityTestOutput.h"

TeamCityTestOutput::TeamCityTestOutput() : currtest_(NULLPTR), currGroup_()
{
}

TeamCityTestOutput::~TeamCityTestOutput()
{
}

void TeamCityTestOutput::printCurrentTestStarted(const UtestShell& test)
{
    print("##teamcity[testStarted name='");
    printEscaped(test.getName().asCharString());
    print("']\n");
    if (!test.willRun()) {
        print("##teamcity[testIgnored name='");
        printEscaped(test.getName().asCharString());
        print("']\n");
    }
    currtest_ = &test;
}

void TeamCityTestOutput::printCurrentTestEnded(const TestResult& res)
{
    if (!currtest_)
        return;

    print("##teamcity[testFinished name='");
    printEscaped(currtest_->getName().asCharString());
    print("' duration='");
    print(res.getCurrentTestTotalExecutionTime());
    print("']\n");
}

void TeamCityTestOutput::printCurrentGroupStarted(const UtestShell& test)
{
    currGroup_ = test.getGroup();
    print("##teamcity[testSuiteStarted name='");
    printEscaped(currGroup_.asCharString());
    print("']\n");
}

void TeamCityTestOutput::printCurrentGroupEnded(const TestResult& /*res*/)
{
    if (currGroup_ == "")
        return;

    print("##teamcity[testSuiteFinished name='");
    printEscaped(currGroup_.asCharString());
    print("']\n");
}

void TeamCityTestOutput::printEscaped(const char* s)
{
    while (*s) {
        char str[3];
        if ((*s == '\'') || (*s == '|') || (*s == '[') || (*s == ']')) {
            str[0] = '|';
            str[1] = *s;
            str[2] = 0;
        } else if (*s == '\r') {
            str[0] = '|';
            str[1] = 'r';
            str[2] = 0;
        } else if (*s == '\n') {
            str[0] = '|';
            str[1] = 'n';
            str[2] = 0;
        } else {
            str[0] = *s;
            str[1] = 0;
        }
        printBuffer(str);
        s++;
    }
}

void TeamCityTestOutput::printFailure(const TestFailure& failure)
{
    print("##teamcity[testFailed name='");
    printEscaped(failure.getTestNameOnly().asCharString());
    print("' message='");
    if (failure.isOutsideTestFile() || failure.isInHelperFunction()) {
        print("TEST failed (");
        print(failure.getTestFileName().asCharString());
        print(":");
        print(failure.getTestLineNumber());
        print("): ");
    }

    printEscaped(failure.getFileName().asCharString());
    print(":");
    print(failure.getFailureLineNumber());

    print("' details='");
    printEscaped(failure.getMessage().asCharString());
    print("']\n");
}
