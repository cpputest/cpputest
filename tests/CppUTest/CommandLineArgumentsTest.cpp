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
#include "CppUTest/CommandLineArguments.h"
#include "CppUTest/TestRegistry.h"

class OptionsPlugin: public TestPlugin
{
public:
    OptionsPlugin(const SimpleString& name) :
        TestPlugin(name)
    {
    }
    ~OptionsPlugin() _destructor_override
    {
    }
    bool parseArguments(int /*ac*/, const char *const * /*av*/, int /*index*/) _override
    {
        return true;
    }
};

TEST_GROUP(CommandLineArguments)
{
    CommandLineArguments* args;
    OptionsPlugin* plugin;

    void setup() _override
    {
        plugin = new OptionsPlugin("options");
        args = NULLPTR;
    }
    void teardown() _override
    {
        delete args;
        delete plugin;
    }

    bool newArgumentParser(int argc, const char *const *argv)
    {
        args = new CommandLineArguments(argc, argv);
        return args->parse(plugin);
    }
};

TEST(CommandLineArguments, Create)
{
}

TEST(CommandLineArguments, verboseSetMultipleParameters)
{
    const char* argv[] = { "tests.exe", "-v" };
    CHECK(newArgumentParser(2, argv));
    CHECK(args->isVerbose());
}

TEST(CommandLineArguments, veryVerbose)
{
    const char* argv[] = { "tests.exe", "-vv" };
    CHECK(newArgumentParser(2, argv));
    CHECK(args->isVeryVerbose());
}

TEST(CommandLineArguments, setColor)
{
    const char* argv[] = { "tests.exe", "-c" };
    CHECK(newArgumentParser(2, argv));
    CHECK(args->isColor());
}

TEST(CommandLineArguments, repeatSet)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-r3" };
    CHECK(newArgumentParser(argc, argv));
    LONGS_EQUAL(3, args->getRepeatCount());
}

TEST(CommandLineArguments, repeatSetDifferentParameter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-r", "4" };
    CHECK(newArgumentParser(argc, argv));
    LONGS_EQUAL(4, args->getRepeatCount());
}

TEST(CommandLineArguments, repeatSetDefaultsToTwoAndShuffleDisabled)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-r" };
    CHECK(newArgumentParser(argc, argv));
    LONGS_EQUAL(2, args->getRepeatCount());
}

TEST(CommandLineArguments, reverseEnabled)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-b" };
    CHECK(newArgumentParser(argc, argv));
    CHECK_TRUE(args->isReversing());
}

TEST(CommandLineArguments, shuffleDisabledByDefault)
{
    int argc = 1;
    const char* argv[] = { "tests.exe" };
    CHECK(newArgumentParser(argc, argv));
    CHECK_FALSE(args->isShuffling());
}

TEST(CommandLineArguments, shuffleEnabled)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-s" };
    CHECK(newArgumentParser(argc, argv));
    CHECK_TRUE(args->isShuffling());
}

TEST(CommandLineArguments, shuffleWithSeedZeroIsOk)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-s0" };
    CHECK_FALSE(newArgumentParser(argc, argv));
    CHECK_EQUAL(0, args->getShuffleSeed());
}

TEST(CommandLineArguments, shuffleEnabledSpecificSeedCase1)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-s999"};
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(999, args->getShuffleSeed());
}

TEST(CommandLineArguments, shuffleEnabledSpecificSeedCase2)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-s 888"};
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(888, args->getShuffleSeed());
}

TEST(CommandLineArguments, shuffleEnabledSpecificSeedCase3)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-s", "777"};
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(777, args->getShuffleSeed());
}

TEST(CommandLineArguments, shuffleBeforeDoesNotDisturbOtherSwitch)
{
    int argc = 4;
    const char* argv[] = { "tests.exe", "-s", "-sg", "group" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter groupFilter("group");
    groupFilter.strictMatching();
    CHECK_EQUAL(groupFilter, *args->getGroupFilters());
    CHECK_TRUE(args->isShuffling());
}

TEST(CommandLineArguments, runningTestsInSeperateProcesses)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-p" };
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->runTestsInSeperateProcess());
}

TEST(CommandLineArguments, setGroupFilter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-g", "group" };
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(TestFilter("group"), *args->getGroupFilters());
}

TEST(CommandLineArguments, setCompleteGroupDotNameFilterInvalidArgument)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-t", "groupname" };
    CHECK_FALSE(newArgumentParser(argc, argv));
}
TEST(CommandLineArguments, setCompleteGroupDotNameFilter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-t", "group.name" };
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(TestFilter("group"), *args->getGroupFilters());
    CHECK_EQUAL(TestFilter("name"), *args->getNameFilters());
}


TEST(CommandLineArguments, setGroupFilterSameParameter)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-ggroup" };
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(TestFilter("group"), *args->getGroupFilters());
}

TEST(CommandLineArguments, setStrictGroupFilter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-sg", "group" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter groupFilter("group");
    groupFilter.strictMatching();
    CHECK_EQUAL(groupFilter, *args->getGroupFilters());
}

TEST(CommandLineArguments, setStrictGroupFilterSameParameter)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-sggroup" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter groupFilter("group");
    groupFilter.strictMatching();
    CHECK_EQUAL(groupFilter, *args->getGroupFilters());
}

TEST(CommandLineArguments, setExcludeGroupFilter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-xg", "group" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter groupFilter("group");
    groupFilter.invertMatching();
    CHECK_EQUAL(groupFilter, *args->getGroupFilters());
}

TEST(CommandLineArguments, setExcludeGroupFilterSameParameter)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-xggroup" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter groupFilter("group");
    groupFilter.invertMatching();
    CHECK_EQUAL(groupFilter, *args->getGroupFilters());
}

TEST(CommandLineArguments, setExcludeStrictGroupFilter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-xsg", "group" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter groupFilter("group");
    groupFilter.invertMatching();
    groupFilter.strictMatching();
    CHECK_EQUAL(groupFilter, *args->getGroupFilters());
}

TEST(CommandLineArguments, setExcludeStrictGroupFilterSameParameter)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-xsggroup" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter groupFilter("group");
    groupFilter.invertMatching();
    groupFilter.strictMatching();
    CHECK_EQUAL(groupFilter, *args->getGroupFilters());
}

TEST(CommandLineArguments, setNameFilter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-n", "name" };
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(TestFilter("name"), *args->getNameFilters());
}

TEST(CommandLineArguments, setNameFilterSameParameter)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-nname" };
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(TestFilter("name"), *args->getNameFilters());
}

TEST(CommandLineArguments, setStrictNameFilter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-sn", "name" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter nameFilter("name");
    nameFilter.strictMatching();
    CHECK_EQUAL(nameFilter, *args->getNameFilters());
}

TEST(CommandLineArguments, setStrictNameFilterSameParameter)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-snname" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter nameFilter("name");
    nameFilter.strictMatching();
    CHECK_EQUAL(nameFilter, *args->getNameFilters());
}

TEST(CommandLineArguments, setExcludeNameFilter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-xn", "name" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter nameFilter("name");
    nameFilter.invertMatching();
    CHECK_EQUAL(nameFilter, *args->getNameFilters());
}

TEST(CommandLineArguments, setExcludeNameFilterSameParameter)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-xnname" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter nameFilter("name");
    nameFilter.invertMatching();
    CHECK_EQUAL(nameFilter, *args->getNameFilters());
}

TEST(CommandLineArguments, setExcludeStrictNameFilter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-xsn", "name" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter nameFilter("name");
    nameFilter.invertMatching();
    nameFilter.strictMatching();
    CHECK_EQUAL(nameFilter, *args->getNameFilters());
}

TEST(CommandLineArguments, setExcludeStrictNameFilterSameParameter)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-xsnname" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter nameFilter("name");
    nameFilter.invertMatching();
    nameFilter.strictMatching();
    CHECK_EQUAL(nameFilter, *args->getNameFilters());
}

TEST(CommandLineArguments, setTestToRunUsingVerboseOutput)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "TEST(testgroup, testname) - stuff" };
    CHECK(newArgumentParser(argc, argv));

    TestFilter nameFilter("testname");
    TestFilter groupFilter("testgroup");
    nameFilter.strictMatching();
    groupFilter.strictMatching();
    CHECK_EQUAL(nameFilter, *args->getNameFilters());
    CHECK_EQUAL(groupFilter, *args->getGroupFilters());
}

TEST(CommandLineArguments, setTestToRunUsingVerboseOutputOfIgnoreTest)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "IGNORE_TEST(testgroup, testname) - stuff" };
    CHECK(newArgumentParser(argc, argv));

    TestFilter nameFilter("testname");
    TestFilter groupFilter("testgroup");
    nameFilter.strictMatching();
    groupFilter.strictMatching();
    CHECK_EQUAL(nameFilter, *args->getNameFilters());
    CHECK_EQUAL(groupFilter, *args->getGroupFilters());
}

TEST(CommandLineArguments, setNormalOutput)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-onormal" };
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->isEclipseOutput());
}

TEST(CommandLineArguments, setEclipseOutput)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-oeclipse" };
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->isEclipseOutput());
}

TEST(CommandLineArguments, setNormalOutputDifferentParameter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-o", "normal" };
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->isEclipseOutput());
}

TEST(CommandLineArguments, setJUnitOutputDifferentParameter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-o", "junit" };
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->isJUnitOutput());
}

TEST(CommandLineArguments, setTeamCityOutputDifferentParameter)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-o", "teamcity" };
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->isTeamCityOutput());
}

TEST(CommandLineArguments, setOutputToGarbage)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-o", "garbage" };
    CHECK(!newArgumentParser(argc, argv));
}

TEST(CommandLineArguments, setPrintGroups)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-lg" };
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->isListingTestGroupNames());
}

TEST(CommandLineArguments, setPrintGroupsAndNames)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-ln" };
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->isListingTestGroupAndCaseNames());
}

TEST(CommandLineArguments, weirdParamatersReturnsFalse)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-SomethingWeird" };
    CHECK(!newArgumentParser(argc, argv));
}

TEST(CommandLineArguments, printUsage)
{
    STRCMP_EQUAL(
            "use -h for more extensive help\n"
            "usage [-h] [-v] [-vv] [-c] [-p] [-lg] [-ln] [-ri] [-r#] [-f]\n"
            "      [-g|sg|xg|xsg groupName]... [-n|sn|xn|xsn testName]... [-t groupName.testName]...\n"
            "      [-b] [-s [randomizerSeed>0]] [\"TEST(groupName, testName)\"]...\n"
            "      [-o{normal, junit, teamcity}] [-k packageName]\n",
            args->usage());
}

TEST(CommandLineArguments, helpPrintsTheHelp)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-h" };
    CHECK(!newArgumentParser(argc, argv));
    CHECK(args->needHelp());
}


TEST(CommandLineArguments, pluginKnowsOption)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-pPluginOption" };
    TestRegistry::getCurrentRegistry()->installPlugin(plugin);
    CHECK(newArgumentParser(argc, argv));
    TestRegistry::getCurrentRegistry()->removePluginByName("options");
}


TEST(CommandLineArguments, checkDefaultArguments)
{
    int argc = 1;
    const char* argv[] = { "tests.exe" };
    CHECK(newArgumentParser(argc, argv));
    CHECK(!args->isVerbose());
    LONGS_EQUAL(1, args->getRepeatCount());
    CHECK(NULLPTR == args->getGroupFilters());
    CHECK(NULLPTR == args->getNameFilters());
    CHECK(args->isEclipseOutput());
    CHECK(SimpleString("") == args->getPackageName());
    CHECK(!args->isCrashingOnFail());
}

TEST(CommandLineArguments, setPackageName)
{
    int argc = 3;
    const char* argv[] = { "tests.exe", "-k", "package" };
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(SimpleString("package"), args->getPackageName());
}

TEST(CommandLineArguments, lotsOfGroupsAndTests)
{
    int argc = 10;
    const char* argv[] = { "tests.exe", "-sggroup1", "-xntest1", "-sggroup2", "-sntest2", "-sntest3", "-sggroup3", "-sntest4", "-sggroup4", "-sntest5" };
    CHECK(newArgumentParser(argc, argv));
    TestFilter nameFilter("test1");
    nameFilter.invertMatching();
    TestFilter groupFilter("group1");
    groupFilter.strictMatching();
    CHECK_EQUAL(nameFilter, *args->getNameFilters()->getNext()->getNext()->getNext()->getNext());
    CHECK_EQUAL(groupFilter, *args->getGroupFilters()->getNext()->getNext()->getNext());
}

TEST(CommandLineArguments, lastParameterFieldMissing)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-k"};
    CHECK(newArgumentParser(argc, argv));
    CHECK_EQUAL(SimpleString(""), args->getPackageName());
}

TEST(CommandLineArguments, setOptRun)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-ri"};
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->isRunIgnored());
}

TEST(CommandLineArguments, setOptCrashOnFail)
{
    int argc = 2;
    const char* argv[] = { "tests.exe", "-f"};
    CHECK(newArgumentParser(argc, argv));
    CHECK(args->isCrashingOnFail());
}
