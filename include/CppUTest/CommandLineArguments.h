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

#ifndef D_CommandLineArguments_H
#define D_CommandLineArguments_H

#include "SimpleString.h"
#include "TestOutput.h"
#include "TestFilter.h"

class TestPlugin;

class CommandLineArguments
{
public:
    explicit CommandLineArguments(int ac, const char *const *av);
    virtual ~CommandLineArguments();

    bool parse(TestPlugin* plugin);
    bool needHelp() const;
    bool isVerbose() const;
    bool isVeryVerbose() const;
    bool isColor() const;
    bool isListingTestGroupNames() const;
    bool isListingTestGroupAndCaseNames() const;
    bool isListingTestLocations() const;
    bool isRunIgnored() const;
    size_t getRepeatCount() const;
    bool isShuffling() const;
    bool isReversing() const;
    bool isCrashingOnFail() const;
    bool isRethrowingExceptions() const;
    size_t getShuffleSeed() const;
    const TestFilter* getGroupFilters() const;
    const TestFilter* getNameFilters() const;
    bool isJUnitOutput() const;
    bool isEclipseOutput() const;
    bool isTeamCityOutput() const;
    bool runTestsInSeperateProcess() const;
    const SimpleString& getPackageName() const;
    const char* usage() const;
    const char* help() const;

private:

    enum OutputType
    {
        OUTPUT_ECLIPSE, OUTPUT_JUNIT, OUTPUT_TEAMCITY
    };

    int ac_;
    const char *const *av_;

    bool needHelp_;
    bool verbose_;
    bool veryVerbose_;
    bool color_;
    bool runTestsAsSeperateProcess_;
    bool listTestGroupNames_;
    bool listTestGroupAndCaseNames_;
    bool listTestLocations_;
    bool runIgnored_;
    bool reversing_;
    bool crashOnFail_;
    bool rethrowExceptions_;
    bool shuffling_;
    bool shufflingPreSeeded_;
    size_t repeat_;
    size_t shuffleSeed_;
    TestFilter* groupFilters_;
    TestFilter* nameFilters_;
    OutputType outputType_;
    SimpleString packageName_;

    SimpleString getParameterField(int ac, const char *const *av, int& i, const SimpleString& parameterName);
    void setRepeatCount(int ac, const char *const *av, int& index);
    bool setShuffle(int ac, const char *const *av, int& index);
    void addGroupFilter(int ac, const char *const *av, int& index);
    bool addGroupDotNameFilter(int ac, const char *const *av, int& index, const SimpleString& parameterName, bool strict, bool exclude);
    void addStrictGroupFilter(int ac, const char *const *av, int& index);
    void addExcludeGroupFilter(int ac, const char *const *av, int& index);
    void addExcludeStrictGroupFilter(int ac, const char *const *av, int& index);
    void addNameFilter(int ac, const char *const *av, int& index);
    void addStrictNameFilter(int ac, const char *const *av, int& index);
    void addExcludeNameFilter(int ac, const char *const *av, int& index);
    void addExcludeStrictNameFilter(int ac, const char *const *av, int& index);
    void addTestToRunBasedOnVerboseOutput(int ac, const char *const *av, int& index, const char* parameterName);
    bool setOutputType(int ac, const char *const *av, int& index);
    void setPackageName(int ac, const char *const *av, int& index);

    CommandLineArguments(const CommandLineArguments&);
    CommandLineArguments& operator=(const CommandLineArguments&);

};

#endif
