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
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/TestPlugin.h"
#include "CppUTest/JUnitTestOutput.h"
#include "CppUTest/PlatformSpecificFunctions.h"

class DummyPluginWhichCountsThePlugins : public TestPlugin
{
public:

    bool returnValue;
    int amountOfPlugins;

    DummyPluginWhichCountsThePlugins(const SimpleString& name, TestRegistry* registry) :
        TestPlugin(name), returnValue(true), amountOfPlugins(0), registry_(registry)
    {
    }

    virtual bool parseArguments(int, const char**, int)
    {
        /* Remove ourselves from the count */
        amountOfPlugins = registry_->countPlugins() - 1;
        return returnValue;
    }
private:
    TestRegistry* registry_;
};

class CommandLineTestRunnerWithStringBufferOutput : public CommandLineTestRunner
{
public:
  StringBufferTestOutput* fakeJUnitOuputWhichIsReallyABuffer_;
  StringBufferTestOutput* fakeConsoleOutputWhichIsReallyABuffer;

  CommandLineTestRunnerWithStringBufferOutput(int argc, const char** argv, TestRegistry* registry)
    : CommandLineTestRunner(argc, argv, registry), fakeJUnitOuputWhichIsReallyABuffer_(NULL), fakeConsoleOutputWhichIsReallyABuffer(NULL)
  {}

  TestOutput* createConsoleOutput()
  {
    fakeConsoleOutputWhichIsReallyABuffer = new StringBufferTestOutput;
    return fakeConsoleOutputWhichIsReallyABuffer;
  }

  TestOutput* createJUnitOutput(const SimpleString&)
  {
    fakeJUnitOuputWhichIsReallyABuffer_ = new StringBufferTestOutput;
    return fakeJUnitOuputWhichIsReallyABuffer_;
  }
};

TEST_GROUP(CommandLineTestRunner)
{
    TestRegistry registry;
    UtestShell *oneTest_;
    DummyPluginWhichCountsThePlugins* pluginCountingPlugin;

    void setup()
    {
      oneTest_ = new UtestShell("group", "test", "file", 1);
      registry.addTest(oneTest_);
      pluginCountingPlugin = new DummyPluginWhichCountsThePlugins("PluginCountingPlugin", &registry);
    }
    void teardown()
    {
      delete pluginCountingPlugin;
      delete oneTest_;
    }
};

TEST(CommandLineTestRunner, OnePluginGetsInstalledDuringTheRunningTheTests)
{
    const char* argv[] = { "tests.exe", "-psomething"};

    registry.installPlugin(pluginCountingPlugin);

    CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(2, argv, &registry);
    commandLineTestRunner.runAllTestsMain();
    registry.removePluginByName("PluginCountingPlugin");

    LONGS_EQUAL(0, registry.countPlugins());
    LONGS_EQUAL(1, pluginCountingPlugin->amountOfPlugins);
}

TEST(CommandLineTestRunner, NoPluginsAreInstalledAtTheEndOfARunWhenTheArgumentsAreInvalid)
{
    const char* argv[] = { "tests.exe", "-fdskjnfkds"};

    CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(2, argv, &registry);
    commandLineTestRunner.runAllTestsMain();

    LONGS_EQUAL(0, registry.countPlugins());

}

TEST(CommandLineTestRunner, JunitOutputEnabled)
{
    const char* argv[] = { "tests.exe", "-ojunit"};

    CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(2, argv, &registry);
    commandLineTestRunner.runAllTestsMain();
    CHECK(commandLineTestRunner.fakeJUnitOuputWhichIsReallyABuffer_);
}

TEST(CommandLineTestRunner, JunitOutputAndVerboseEnabled)
{
    const char* argv[] = { "tests.exe", "-ojunit", "-v"};

    CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(3, argv, &registry);
    commandLineTestRunner.runAllTestsMain();
    STRCMP_CONTAINS("TEST(group, test)", commandLineTestRunner.fakeJUnitOuputWhichIsReallyABuffer_->getOutput().asCharString());
    STRCMP_CONTAINS("TEST(group, test)", commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput().asCharString());
}

TEST(CommandLineTestRunner, listTestGroupNamesShouldWorkProperly)
{
    const char* argv[] = { "tests.exe", "-lg" };

    CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(2, argv, &registry);
    commandLineTestRunner.runAllTestsMain();

    STRCMP_CONTAINS("group", commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput().asCharString());
}

TEST(CommandLineTestRunner, listTestGroupAndCaseNamesShouldWorkProperly)
{
    const char* argv[] = { "tests.exe", "-ln" };

    CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(2, argv, &registry);
    commandLineTestRunner.runAllTestsMain();

    STRCMP_CONTAINS("group.test", commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput().asCharString());
}

struct FakeOutput
{
    FakeOutput() : SaveFOpen(PlatformSpecificFOpen), SaveFPuts(PlatformSpecificFPuts),
        SaveFClose(PlatformSpecificFClose), SavePutchar(PlatformSpecificPutchar)
    {
        PlatformSpecificFOpen = fopen_fake;
        PlatformSpecificFPuts = fputs_fake;
        PlatformSpecificFClose = fclose_fake;
        PlatformSpecificPutchar = putchar_fake;
    }
    ~FakeOutput()
    {
        PlatformSpecificPutchar = SavePutchar;
        PlatformSpecificFOpen = SaveFOpen;
        PlatformSpecificFPuts = SaveFPuts;
        PlatformSpecificFClose = SaveFClose;
    }
    static PlatformSpecificFile fopen_fake(const char*, const char*)
    {
        return (PlatformSpecificFile)0;
    }
    static void fputs_fake(const char* str, PlatformSpecificFile)
    {
        file += str;
    }
    static void fclose_fake(PlatformSpecificFile)
    {
    }
    static int putchar_fake(int c)
    {
        console += StringFrom((char)c);
        return c;
    }
    static SimpleString file;
    static SimpleString console;
private:
    PlatformSpecificFile (*SaveFOpen)(const char*, const char*);
    void (*SaveFPuts)(const char*, PlatformSpecificFile);
    void (*SaveFClose)(PlatformSpecificFile);
    int (*SavePutchar)(int);
};

SimpleString FakeOutput::console = "";
SimpleString FakeOutput::file = "";

TEST(CommandLineTestRunner, realJunitOutputShouldBeCreatedAndWorkProperly)
{
    const char* argv[] = { "tests.exe", "-ojunit", "-v", "-kpackage", };

    FakeOutput* fakeOutput = new FakeOutput; /* UT_PTR_SET() is not reentrant */

    CommandLineTestRunner commandLineTestRunner(4, argv, &registry);
    commandLineTestRunner.runAllTestsMain();

    delete fakeOutput; /* Original output must be restored before further output occurs */

    STRCMP_CONTAINS("<testcase classname=\"package.group\" name=\"test\"", FakeOutput::file.asCharString());
    STRCMP_CONTAINS("TEST(group, test)", FakeOutput::console.asCharString());
}
