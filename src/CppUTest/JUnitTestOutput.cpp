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
#include "CppUTest/TestFailure.h"
#include "CppUTest/PlatformSpecificFunctions.h"

struct JUnitTestCaseResultNode
{
    JUnitTestCaseResultNode() :
        execTime_(0), failure_(0), ignored_(false), lineNumber_ (0), checkCount_ (0), next_(0)
    {
    }

    SimpleString name_;
    long execTime_;
    TestFailure* failure_;
    bool ignored_;
    SimpleString file_;
    int lineNumber_;
    int checkCount_;
    JUnitTestCaseResultNode* next_;
};

struct JUnitTestGroupResult
{
    JUnitTestGroupResult() :
        testCount_(0), failureCount_(0), totalCheckCount_(0), startTime_(0), groupExecTime_(0), head_(0), tail_(0)
    {
    }

    int testCount_;
    int failureCount_;
    int totalCheckCount_;
    long startTime_;
    long groupExecTime_;
    SimpleString group_;
    JUnitTestCaseResultNode* head_;
    JUnitTestCaseResultNode* tail_;
};

struct JUnitTestOutputImpl
{
    JUnitTestGroupResult results_;
    PlatformSpecificFile file_;
    SimpleString package_;
};

JUnitTestOutput::JUnitTestOutput() :
    impl_(new JUnitTestOutputImpl)
{
}

JUnitTestOutput::~JUnitTestOutput()
{
    resetTestGroupResult();
    delete impl_;
}

void JUnitTestOutput::resetTestGroupResult()
{
    impl_->results_.testCount_ = 0;
    impl_->results_.failureCount_ = 0;
    impl_->results_.group_ = "";
    JUnitTestCaseResultNode* cur = impl_->results_.head_;
    while (cur) {
        JUnitTestCaseResultNode* tmp = cur->next_;
        ;
        delete cur->failure_;
        delete cur;
        cur = tmp;
    }
    impl_->results_.head_ = 0;
    impl_->results_.tail_ = 0;
}

void JUnitTestOutput::printTestsStarted()
{
}

void JUnitTestOutput::printCurrentGroupStarted(const UtestShell& /*test*/)
{
}

void JUnitTestOutput::printCurrentTestEnded(const TestResult& result)
{
    impl_->results_.tail_->execTime_
            = result.getCurrentTestTotalExecutionTime();
    impl_->results_.tail_->checkCount_ = result.getCheckCount();
}

void JUnitTestOutput::printTestsEnded(const TestResult& /*result*/)
{
}

void JUnitTestOutput::printCurrentGroupEnded(const TestResult& result)
{
    impl_->results_.groupExecTime_ = result.getCurrentGroupTotalExecutionTime();
    writeTestGroupToFile();
    resetTestGroupResult();
}

void JUnitTestOutput::printCurrentTestStarted(const UtestShell& test)
{
    impl_->results_.testCount_++;
    impl_->results_.group_ = test.getGroup();
    impl_->results_.startTime_ = GetPlatformSpecificTimeInMillis();

    if (impl_->results_.tail_ == 0) {
        impl_->results_.head_ = impl_->results_.tail_
                = new JUnitTestCaseResultNode;
    }
    else {
        impl_->results_.tail_->next_ = new JUnitTestCaseResultNode;
        impl_->results_.tail_ = impl_->results_.tail_->next_;
    }
    impl_->results_.tail_->name_ = test.getName();
    impl_->results_.tail_->file_ = test.getFile();
    impl_->results_.tail_->lineNumber_ = test.getLineNumber();
    if (!test.willRun()) {
        impl_->results_.tail_->ignored_ = true;
    }
}

SimpleString JUnitTestOutput::createFileName(const SimpleString& group)
{
    SimpleString fileName = "cpputest_";
    fileName += group;
    fileName.replace('/', '_');
    fileName += ".xml";
    return fileName;
}

void JUnitTestOutput::setPackageName(const SimpleString& package)
{
    if (impl_ != NULL) {
        impl_->package_ = package;
    }
}

void JUnitTestOutput::writeXmlHeader()
{
    writeToFile("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
}

void JUnitTestOutput::writeTestSuiteSummary()
{
    SimpleString
            buf =
                    StringFromFormat(
                            "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" tests=\"%d\" time=\"%d.%03d\" timestamp=\"%s\">\n",
                            impl_->results_.failureCount_,
                            impl_->results_.group_.asCharString(),
                            impl_->results_.testCount_,
                            (int) (impl_->results_.groupExecTime_ / 1000), (int) (impl_->results_.groupExecTime_ % 1000),
                            GetPlatformSpecificTimeString());
    writeToFile(buf.asCharString());
}

void JUnitTestOutput::writeProperties()
{
    writeToFile("<properties>\n");
    writeToFile("</properties>\n");
}

void JUnitTestOutput::writeTestCases()
{
    JUnitTestCaseResultNode* cur = impl_->results_.head_;

    while (cur) {
        SimpleString buf = StringFromFormat(
                "<testcase classname=\"%s%s%s\" name=\"%s\" assertions=\"%d\" time=\"%d.%03d\" file=\"%s\" line=\"%d\">\n",
                impl_->package_.asCharString(),
                impl_->package_.isEmpty() == true ? "" : ".",
                impl_->results_.group_.asCharString(),
                cur->name_.asCharString(),
                cur->checkCount_ - impl_->results_.totalCheckCount_,
                (int) (cur->execTime_ / 1000), (int)(cur->execTime_ % 1000),
                cur->file_.asCharString(),
                cur->lineNumber_);
        writeToFile(buf.asCharString());

        impl_->results_.totalCheckCount_ = cur->checkCount_;

        if (cur->failure_) {
            writeFailure(cur);
        }
        else if (cur->ignored_) {
            writeToFile("<skipped />\n");
        }
        writeToFile("</testcase>\n");
        cur = cur->next_;
    }
}

void JUnitTestOutput::writeFailure(JUnitTestCaseResultNode* node)
{
    SimpleString message = node->failure_->getMessage().asCharString();
    message.replace('"', '\'');
    message.replace('<', '[');
    message.replace('>', ']');
    message.replace("&", "&amp;");
    message.replace("\n", "{newline}");
    SimpleString buf = StringFromFormat(
            "<failure message=\"%s:%d: %s\" type=\"AssertionFailedError\">\n",
            node->failure_->getFileName().asCharString(),
            node->failure_->getFailureLineNumber(), message.asCharString());
    writeToFile(buf.asCharString());
    writeToFile("</failure>\n");
}

void JUnitTestOutput::writeFileEnding()
{
    writeToFile("<system-out></system-out>\n");
    writeToFile("<system-err></system-err>\n");
    writeToFile("</testsuite>");
}

void JUnitTestOutput::writeTestGroupToFile()
{
    openFileForWrite(createFileName(impl_->results_.group_));
    writeXmlHeader();
    writeTestSuiteSummary();
    writeProperties();
    writeTestCases();
    writeFileEnding();
    closeFile();
}

// LCOV_EXCL_START

void JUnitTestOutput::printBuffer(const char*)
{
}

void JUnitTestOutput::print(const char*)
{
}

void JUnitTestOutput::print(long)
{
}

void JUnitTestOutput::flush()
{
}

// LCOV_EXCL_STOP

void JUnitTestOutput::printFailure(const TestFailure& failure)
{
    if (impl_->results_.tail_->failure_ == 0) {
        impl_->results_.failureCount_++;
        impl_->results_.tail_->failure_ = new TestFailure(failure);
    }
}

void JUnitTestOutput::openFileForWrite(const SimpleString& fileName)
{
    impl_->file_ = PlatformSpecificFOpen(fileName.asCharString(), "w");
}

void JUnitTestOutput::writeToFile(const SimpleString& buffer)
{
    PlatformSpecificFPuts(buffer.asCharString(), impl_->file_);
}

void JUnitTestOutput::closeFile()
{
    PlatformSpecificFClose(impl_->file_);
}
