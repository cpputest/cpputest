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
#include "CppUTest/PlatformSpecificFunctions.h"

CommandLineArguments::CommandLineArguments(int ac, const char *const *av) :
    ac_(ac), av_(av), verbose_(false), color_(false), runTestsAsSeperateProcess_(false), listTestGroupNames_(false), listTestGroupAndCaseNames_(false), runIgnored_(false), repeat_(1), groupFilters_(NULLPTR), nameFilters_(NULLPTR), outputType_(OUTPUT_ECLIPSE)
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

        if      (argument == "-v") verbose_ = true;
        else if (argument == "-c") color_ = true;
        else if (argument == "-p") runTestsAsSeperateProcess_ = true;
        else if (argument == "-lg") listTestGroupNames_ = true;
        else if (argument == "-ln") listTestGroupAndCaseNames_ = true;
        else if (argument == "-ri") runIgnored_ = true;
        else if (argument.startsWith("-r")) SetRepeatCount(ac_, av_, i);
        else if (argument.startsWith("-g")) AddGroupFilter(ac_, av_, i);
        else if (argument.startsWith("-sg")) AddStrictGroupFilter(ac_, av_, i);
        else if (argument.startsWith("-xg")) AddExcludeGroupFilter(ac_, av_, i);
        else if (argument.startsWith("-xsg")) AddExcludeStrictGroupFilter(ac_, av_, i);
        else if (argument.startsWith("-n")) AddNameFilter(ac_, av_, i);
        else if (argument.startsWith("-sn")) AddStrictNameFilter(ac_, av_, i);
        else if (argument.startsWith("-xn")) AddExcludeNameFilter(ac_, av_, i);
        else if (argument.startsWith("-xsn")) AddExcludeStrictNameFilter(ac_, av_, i);
        else if (argument.startsWith("TEST(")) AddTestToRunBasedOnVerboseOutput(ac_, av_, i, "TEST(");
        else if (argument.startsWith("IGNORE_TEST(")) AddTestToRunBasedOnVerboseOutput(ac_, av_, i, "IGNORE_TEST(");
        else if (argument.startsWith("-o")) correctParameters = SetOutputType(ac_, av_, i);
        else if (argument.startsWith("-p")) correctParameters = plugin->parseAllArguments(ac_, av_, i);
        else if (argument.startsWith("-k")) SetPackageName(ac_, av_, i);
        else correctParameters = false;

        if (correctParameters == false) {
            return false;
        }
    }
    return true;
}

const char* CommandLineArguments::usage() const
{
    return "usage [-v] [-c] [-p] [-lg] [-ln] [-ri] [-r#] [-g|sg|xg|xsg groupName]... [-n|sn|xn|xsn testName]... [\"TEST(groupName, testName)\"]... [-o{normal, junit, teamcity}] [-k packageName]\n";
}

bool CommandLineArguments::isVerbose() const
{
    return verbose_;
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

bool CommandLineArguments::isRunIgnored() const
{
    return runIgnored_;
}

bool CommandLineArguments::runTestsInSeperateProcess() const
{
    return runTestsAsSeperateProcess_;
}


int CommandLineArguments::getRepeatCount() const
{
    return repeat_;
}

const TestFilter* CommandLineArguments::getGroupFilters() const
{
    return groupFilters_;
}

const TestFilter* CommandLineArguments::getNameFilters() const
{
    return nameFilters_;
}

void CommandLineArguments::SetRepeatCount(int ac, const char *const *av, int& i)
{
    repeat_ = 0;

    SimpleString repeatParameter(av[i]);
    if (repeatParameter.size() > 2) repeat_ = SimpleString::AtoI(av[i] + 2);
    else if (i + 1 < ac) {
        repeat_ = SimpleString::AtoI(av[i + 1]);
        if (repeat_ != 0) i++;
    }

    if (0 == repeat_) repeat_ = 2;

}

SimpleString CommandLineArguments::getParameterField(int ac, const char * const *av, int& i, const SimpleString& parameterName)
{
    size_t parameterLength = parameterName.size();
    SimpleString parameter(av[i]);
    if (parameter.size() >  parameterLength) return av[i] + parameterLength;
    else if (i + 1 < ac) return av[++i];
    return "";
}

void CommandLineArguments::AddGroupFilter(int ac, const char *const *av, int& i)
{
    TestFilter* groupFilter = new TestFilter(getParameterField(ac, av, i, "-g"));
    groupFilters_ = groupFilter->add(groupFilters_);
}

void CommandLineArguments::AddStrictGroupFilter(int ac, const char *const *av, int& i)
{
    TestFilter* groupFilter = new TestFilter(getParameterField(ac, av, i, "-sg"));
    groupFilter->strictMatching();
    groupFilters_ = groupFilter->add(groupFilters_);
}

void CommandLineArguments::AddExcludeGroupFilter(int ac, const char *const *av, int& i)
{
    TestFilter* groupFilter = new TestFilter(getParameterField(ac, av, i, "-xg"));
    groupFilter->invertMatching();
    groupFilters_ = groupFilter->add(groupFilters_);
}

void CommandLineArguments::AddExcludeStrictGroupFilter(int ac, const char *const *av, int& i)
{
    TestFilter* groupFilter = new TestFilter(getParameterField(ac, av, i, "-xsg"));
    groupFilter->strictMatching();
    groupFilter->invertMatching();
    groupFilters_ = groupFilter->add(groupFilters_);
}

void CommandLineArguments::AddNameFilter(int ac, const char *const *av, int& i)
{
    TestFilter* nameFilter = new TestFilter(getParameterField(ac, av, i, "-n"));
    nameFilters_ = nameFilter->add(nameFilters_);
}

void CommandLineArguments::AddStrictNameFilter(int ac, const char *const *av, int& index)
{
    TestFilter* nameFilter = new TestFilter(getParameterField(ac, av, index, "-sn"));
    nameFilter->strictMatching();
    nameFilters_= nameFilter->add(nameFilters_);
}

void CommandLineArguments::AddExcludeNameFilter(int ac, const char *const *av, int& index)
{
    TestFilter* nameFilter = new TestFilter(getParameterField(ac, av, index, "-xn"));
    nameFilter->invertMatching();
    nameFilters_= nameFilter->add(nameFilters_);
}

void CommandLineArguments::AddExcludeStrictNameFilter(int ac, const char *const *av, int& index)
{
    TestFilter* nameFilter = new TestFilter(getParameterField(ac, av, index, "-xsn"));
    nameFilter->invertMatching();
    nameFilter->strictMatching();
    nameFilters_= nameFilter->add(nameFilters_);
}

void CommandLineArguments::AddTestToRunBasedOnVerboseOutput(int ac, const char *const *av, int& index, const char* parameterName)
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

void CommandLineArguments::SetPackageName(int ac, const char *const *av, int& i)
{
    SimpleString packageName = getParameterField(ac, av, i, "-k");
    if (packageName.size() == 0) return;

    packageName_ = packageName;
}

bool CommandLineArguments::SetOutputType(int ac, const char *const *av, int& i)
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

