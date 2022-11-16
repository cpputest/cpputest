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
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
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
#include "CppUTest/PlatformSpecificFunctions.h"

CommandLineArguments::CommandLineArguments(int ac, const char *const *av) :
    ac_(ac), av_(av), needHelp_(false), verbose_(false), veryVerbose_(false), color_(false), runTestsAsSeperateProcess_(false),
    listTestGroupNames_(false), listTestGroupAndCaseNames_(false), listTestLocations_(false), runIgnored_(false), reversing_(false),
    crashOnFail_(false), rethrowExceptions_(true), shuffling_(false), shufflingPreSeeded_(false), repeat_(1), shuffleSeed_(0),
    groupFilters_(NULLPTR), nameFilters_(NULLPTR), outputType_(OUTPUT_ECLIPSE)
{
}

CommandLineArguments::~CommandLineArguments()
{
    while(groupFilters_) {
        TestFilter* current = groupFilters_;
        groupFilters_ = groupFilters_->getNext();
        delete current;
    }
    while(nameFilters_) {
        TestFilter* current = nameFilters_;
        nameFilters_ = nameFilters_->getNext();
        delete current;
    }
}

bool CommandLineArguments::parse(TestPlugin* plugin)
{
    bool correctParameters = true;
    for (int i = 1; i < ac_; i++) {
        SimpleString argument = av_[i];

        if (argument == "-h") {
            needHelp_ = true;
            correctParameters = false;
        }
        else if (argument == "-v") verbose_ = true;
        else if (argument == "-vv") veryVerbose_ = true;
        else if (argument == "-c") color_ = true;
        else if (argument == "-p") runTestsAsSeperateProcess_ = true;
        else if (argument == "-b") reversing_ = true;
        else if (argument == "-lg") listTestGroupNames_ = true;
        else if (argument == "-ln") listTestGroupAndCaseNames_ = true;
        else if (argument == "-ll") listTestLocations_ = true;
        else if (argument == "-ri") runIgnored_ = true;
        else if (argument == "-f") crashOnFail_ = true;
        else if ((argument == "-e") || (argument == "-ci")) rethrowExceptions_ = false;
        else if (argument.startsWith("-r")) setRepeatCount(ac_, av_, i);
        else if (argument.startsWith("-g")) addGroupFilter(ac_, av_, i);
        else if (argument.startsWith("-t")) correctParameters = addGroupDotNameFilter(ac_, av_, i, "-t", false, false);
        else if (argument.startsWith("-st")) correctParameters = addGroupDotNameFilter(ac_, av_, i, "-st", true, false);
        else if (argument.startsWith("-xt")) correctParameters = addGroupDotNameFilter(ac_, av_, i, "-xt", false, true);
        else if (argument.startsWith("-xst")) correctParameters = addGroupDotNameFilter(ac_, av_, i, "-xst", true, true);
        else if (argument.startsWith("-sg")) addStrictGroupFilter(ac_, av_, i);
        else if (argument.startsWith("-xg")) addExcludeGroupFilter(ac_, av_, i);
        else if (argument.startsWith("-xsg")) addExcludeStrictGroupFilter(ac_, av_, i);
        else if (argument.startsWith("-n")) addNameFilter(ac_, av_, i);
        else if (argument.startsWith("-sn")) addStrictNameFilter(ac_, av_, i);
        else if (argument.startsWith("-xn")) addExcludeNameFilter(ac_, av_, i);
        else if (argument.startsWith("-xsn")) addExcludeStrictNameFilter(ac_, av_, i);
        else if (argument.startsWith("-s")) correctParameters = setShuffle(ac_, av_, i);
        else if (argument.startsWith("TEST(")) addTestToRunBasedOnVerboseOutput(ac_, av_, i, "TEST(");
        else if (argument.startsWith("IGNORE_TEST(")) addTestToRunBasedOnVerboseOutput(ac_, av_, i, "IGNORE_TEST(");
        else if (argument.startsWith("-o")) correctParameters = setOutputType(ac_, av_, i);
        else if (argument.startsWith("-p")) correctParameters = plugin->parseAllArguments(ac_, av_, i);
        else if (argument.startsWith("-k")) setPackageName(ac_, av_, i);
        else correctParameters = false;

        if (correctParameters == false) {
            return false;
        }
    }
    return true;
}

const char* CommandLineArguments::usage() const
{
    return "use -h for more extensive help\n"
           "usage [-h] [-v] [-vv] [-c] [-p] [-lg] [-ln] [-ll] [-ri] [-r[<#>]] [-f] [-e] [-ci]\n"
           "      [-g|sg|xg|xsg <groupName>]... [-n|sn|xn|xsn <testName>]... [-t|st|xt|xst <groupName>.<testName>]...\n"
           "      [-b] [-s [<seed>]] [\"[IGNORE_]TEST(<groupName>, <testName>)\"]...\n"
           "      [-o{normal|eclipse|junit|teamcity}] [-k <packageName>]\n";
}

const char* CommandLineArguments::help() const
{
    return
      "Thanks for using CppUTest.\n"
      "\n"
      "Options that do not run tests but query:\n"
      "  -h                - this wonderful help screen. Joy!\n"
      "  -lg               - print a list of group names, separated by spaces\n"
      "  -ln               - print a list of test names in the form of group.name, separated by spaces\n"
      "  -ll               - print a list of test names in the form of group.name.test_file_path.line\n"
      "\n"
      "Options that change the output format:\n"
      "  -c                - colorize output, print green if OK, or red if failed\n"
      "  -v                - verbose, print each test name as it runs\n"
      "  -vv               - very verbose, print internal information during test run\n"
      "\n"
      "Options that change the output location:\n"
      "  -onormal          - no output to files\n"
      "  -oeclipse         - equivalent to -onormal\n"
      "  -oteamcity        - output to xml files (as the name suggests, for TeamCity)\n"
      "  -ojunit           - output to JUnit ant plugin style xml files (for CI systems)\n"
      "  -k <packageName>  - add a package name in JUnit output (for classification in CI systems)\n"
      "\n"
      "\n"
      "Options that control which tests are run:\n"
      "  -g <group>        - only run tests whose group contains <group>\n"
      "  -n <name>         - only run tests whose name contains <name>\n"
      "  -t <group>.<name> - only run tests whose group and name contain <group> and <name>\n"
      "  -sg <group>       - only run tests whose group exactly matches <group>\n"
      "  -sn <name>        - only run tests whose name exactly matches <name>\n"
      "  -st <grp>.<name>  - only run tests whose group and name exactly match <grp> and <name>\n"
      "  -xg <group>       - exclude tests whose group contains <group>\n"
      "  -xn <name>        - exclude tests whose name contains <name>\n"
      "  -xt <grp>.<name>  - exclude tests whose group and name contain <grp> and <name>\n"
      "  -xsg <group>      - exclude tests whose group exactly matches <group>\n"
      "  -xsn <name>       - exclude tests whose name exactly matches <name>\n"
      "  -xst <grp>.<name> - exclude tests whose group and name exactly match <grp> and <name>\n"
      "  \"[IGNORE_]TEST(<group>, <name>)\"\n"
      "                    - only run tests whose group and name exactly match <group> and <name>\n"
      "                      (this can be used to copy-paste output from the -v option on the command line)\n"
      "\n"
      "Options that control how the tests are run:\n"
      "  -p                - run tests in a separate process\n"
      "  -b                - run the tests backwards, reversing the normal way\n"
      "  -s [<seed>]       - shuffle tests randomly (randomization seed is optional, must be greater than 0)\n"
      "  -r[<#>]           - repeat the tests <#> times (or twice if <#> is not specified)\n"
      "  -f                - Cause the tests to crash on failure (to allow the test to be debugged if necessary)\n"
      "  -e                - do not rethrow unexpected exceptions on failure\n"
      "  -ci               - continuous integration mode (equivalent to -e)\n";
}

bool CommandLineArguments::needHelp() const
{
    return needHelp_;
}

bool CommandLineArguments::isVerbose() const
{
    return verbose_;
}

bool CommandLineArguments::isVeryVerbose() const
{
    return veryVerbose_;
}

bool CommandLineArguments::isColor() const
{
    return color_;
}

bool CommandLineArguments::isListingTestGroupNames() const
{
    return listTestGroupNames_;
}

bool CommandLineArguments::isListingTestGroupAndCaseNames() const
{
    return listTestGroupAndCaseNames_;
}

bool CommandLineArguments::isListingTestLocations() const
{
    return listTestLocations_;
}

bool CommandLineArguments::isRunIgnored() const
{
    return runIgnored_;
}

bool CommandLineArguments::runTestsInSeperateProcess() const
{
    return runTestsAsSeperateProcess_;
}


size_t CommandLineArguments::getRepeatCount() const
{
    return repeat_;
}

bool CommandLineArguments::isReversing() const
{
    return reversing_;
}

bool CommandLineArguments::isCrashingOnFail() const
{
    return crashOnFail_;
}

bool CommandLineArguments::isRethrowingExceptions() const
{
    return rethrowExceptions_;
}

bool CommandLineArguments::isShuffling() const
{
    return shuffling_;
}

size_t CommandLineArguments::getShuffleSeed() const
{
    return shuffleSeed_;
}

const TestFilter* CommandLineArguments::getGroupFilters() const
{
    return groupFilters_;
}

const TestFilter* CommandLineArguments::getNameFilters() const
{
    return nameFilters_;
}

void CommandLineArguments::setRepeatCount(int ac, const char *const *av, int& i)
{
    repeat_ = 0;

    SimpleString repeatParameter(av[i]);
    if (repeatParameter.size() > 2) repeat_ = (size_t) (SimpleString::AtoI(av[i] + 2));
    else if (i + 1 < ac) {
        repeat_ = (size_t) (SimpleString::AtoI(av[i + 1]));
        if (repeat_ != 0) i++;
    }

    if (0 == repeat_) repeat_ = 2;

}

bool CommandLineArguments::setShuffle(int ac, const char * const *av, int& i)
{
    shuffling_ = true;
    shuffleSeed_ = (unsigned int)GetPlatformSpecificTimeInMillis();
    if (shuffleSeed_ == 0) shuffleSeed_++;

    SimpleString shuffleParameter = av[i];
    if (shuffleParameter.size() > 2) {
        shufflingPreSeeded_ = true;
        shuffleSeed_ = SimpleString::AtoU(av[i] + 2);
    } else if (i + 1 < ac) {
        unsigned int parsedParameter = SimpleString::AtoU(av[i + 1]);
        if (parsedParameter != 0)
        {
            shufflingPreSeeded_ = true;
            shuffleSeed_ = parsedParameter;
            i++;
        }
    }
    return (shuffleSeed_ != 0);
}

SimpleString CommandLineArguments::getParameterField(int ac, const char * const *av, int& i, const SimpleString& parameterName)
{
    size_t parameterLength = parameterName.size();
    SimpleString parameter(av[i]);
    if (parameter.size() >  parameterLength) return av[i] + parameterLength;
    else if (i + 1 < ac) return av[++i];
    return "";
}

void CommandLineArguments::addGroupFilter(int ac, const char *const *av, int& i)
{
    TestFilter* groupFilter = new TestFilter(getParameterField(ac, av, i, "-g"));
    groupFilters_ = groupFilter->add(groupFilters_);
}

bool CommandLineArguments::addGroupDotNameFilter(int ac, const char *const *av, int& i, const SimpleString& parameterName, 
                                                 bool strict, bool exclude)
{
    SimpleString groupDotName = getParameterField(ac, av, i, parameterName);
    SimpleStringCollection collection;
    groupDotName.split(".", collection);

    if (collection.size() != 2) return false;

    TestFilter* groupFilter = new TestFilter(collection[0].subString(0, collection[0].size()-1));
    TestFilter* nameFilter = new TestFilter(collection[1]);
    if (strict)
    {
        groupFilter->strictMatching();
        nameFilter->strictMatching();
    }
    if (exclude)
    {
        groupFilter->invertMatching();
        nameFilter->invertMatching();
    }
    groupFilters_ = groupFilter->add(groupFilters_);
    nameFilters_ = nameFilter->add(nameFilters_);
    return true;
}

void CommandLineArguments::addStrictGroupFilter(int ac, const char *const *av, int& i)
{
    TestFilter* groupFilter = new TestFilter(getParameterField(ac, av, i, "-sg"));
    groupFilter->strictMatching();
    groupFilters_ = groupFilter->add(groupFilters_);
}

void CommandLineArguments::addExcludeGroupFilter(int ac, const char *const *av, int& i)
{
    TestFilter* groupFilter = new TestFilter(getParameterField(ac, av, i, "-xg"));
    groupFilter->invertMatching();
    groupFilters_ = groupFilter->add(groupFilters_);
}

void CommandLineArguments::addExcludeStrictGroupFilter(int ac, const char *const *av, int& i)
{
    TestFilter* groupFilter = new TestFilter(getParameterField(ac, av, i, "-xsg"));
    groupFilter->strictMatching();
    groupFilter->invertMatching();
    groupFilters_ = groupFilter->add(groupFilters_);
}

void CommandLineArguments::addNameFilter(int ac, const char *const *av, int& i)
{
    TestFilter* nameFilter = new TestFilter(getParameterField(ac, av, i, "-n"));
    nameFilters_ = nameFilter->add(nameFilters_);
}

void CommandLineArguments::addStrictNameFilter(int ac, const char *const *av, int& index)
{
    TestFilter* nameFilter = new TestFilter(getParameterField(ac, av, index, "-sn"));
    nameFilter->strictMatching();
    nameFilters_= nameFilter->add(nameFilters_);
}

void CommandLineArguments::addExcludeNameFilter(int ac, const char *const *av, int& index)
{
    TestFilter* nameFilter = new TestFilter(getParameterField(ac, av, index, "-xn"));
    nameFilter->invertMatching();
    nameFilters_= nameFilter->add(nameFilters_);
}

void CommandLineArguments::addExcludeStrictNameFilter(int ac, const char *const *av, int& index)
{
    TestFilter* nameFilter = new TestFilter(getParameterField(ac, av, index, "-xsn"));
    nameFilter->invertMatching();
    nameFilter->strictMatching();
    nameFilters_= nameFilter->add(nameFilters_);
}

void CommandLineArguments::addTestToRunBasedOnVerboseOutput(int ac, const char *const *av, int& index, const char* parameterName)
{
    SimpleString wholename = getParameterField(ac, av, index, parameterName);
    SimpleString testname = wholename.subStringFromTill(',', ')');
    testname = testname.subString(2);
    TestFilter* namefilter = new TestFilter(testname);
    TestFilter* groupfilter = new TestFilter(wholename.subStringFromTill(wholename.at(0), ','));
    namefilter->strictMatching();
    groupfilter->strictMatching();
    groupFilters_ = groupfilter->add(groupFilters_);
    nameFilters_ = namefilter->add(nameFilters_);
}

void CommandLineArguments::setPackageName(int ac, const char *const *av, int& i)
{
    SimpleString packageName = getParameterField(ac, av, i, "-k");
    if (packageName.size() == 0) return;

    packageName_ = packageName;
}

bool CommandLineArguments::setOutputType(int ac, const char *const *av, int& i)
{
    SimpleString outputType = getParameterField(ac, av, i, "-o");
    if (outputType.size() == 0) return false;

    if (outputType == "normal" || outputType == "eclipse") {
        outputType_ = OUTPUT_ECLIPSE;
        return true;
    }
    if (outputType == "junit") {
        outputType_ = OUTPUT_JUNIT;
        return true;
    }
    if (outputType == "teamcity") {
        outputType_ = OUTPUT_TEAMCITY;
        return true;
    }

    return false;
}

bool CommandLineArguments::isEclipseOutput() const
{
    return outputType_ == OUTPUT_ECLIPSE;
}

bool CommandLineArguments::isJUnitOutput() const
{
    return outputType_ == OUTPUT_JUNIT;
}

bool CommandLineArguments::isTeamCityOutput() const
{
    return outputType_ == OUTPUT_TEAMCITY;
}

const SimpleString& CommandLineArguments::getPackageName() const
{
    return packageName_;
}

