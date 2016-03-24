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

#ifndef D_CommandLineArguments_H
#define D_CommandLineArguments_H

#include "SimpleString.h"
#include "TestOutput.h"
#include "TestFilter.h"

class TestPlugin;

class CommandLineArguments
{
public:
    explicit CommandLineArguments(int ac, const char** av);
    virtual ~CommandLineArguments();

    bool parse(TestPlugin* plugin);
    bool isVerbose() const;
    bool isColor() const;
    bool isListingTestGroupNames() const;
    bool isListingTestGroupAndCaseNames() const;
    bool isRunIgnored() const;
    int getRepeatCount() const;
    const TestFilter* getGroupFilters() const;
    const TestFilter* getNameFilters() const;
    bool isJUnitOutput() const;
    bool isEclipseOutput() const;
    bool isTeamCityOutput() const;
    bool runTestsInSeperateProcess() const;
    const SimpleString& getPackageName() const;
    const char* usage() const;

private:

    enum OutputType
    {
        OUTPUT_ECLIPSE, OUTPUT_JUNIT, OUTPUT_TEAMCITY
    };
    int ac_;
    const char** av_;

    bool verbose_;
    bool color_;
    bool runTestsAsSeperateProcess_;
    bool listTestGroupNames_;
    bool listTestGroupAndCaseNames_;
    bool runIgnored_;
    int repeat_;
    TestFilter* groupFilters_;
    TestFilter* nameFilters_;
    OutputType outputType_;
    SimpleString packageName_;

    SimpleString getParameterField(int ac, const char** av, int& i, const SimpleString& parameterName);
    void SetRepeatCount(int ac, const char** av, int& index);
    void AddGroupFilter(int ac, const char** av, int& index);
    void AddStrictGroupFilter(int ac, const char** av, int& index);
    void AddExcludeGroupFilter(int ac, const char** av, int& index);
    void AddExcludeStrictGroupFilter(int ac, const char** av, int& index);
    void AddNameFilter(int ac, const char** av, int& index);
    void AddStrictNameFilter(int ac, const char** av, int& index);
    void AddExcludeNameFilter(int ac, const char** av, int& index);
    void AddExcludeStrictNameFilter(int ac, const char** av, int& index);
    void AddTestToRunBasedOnVerboseOutput(int ac, const char** av, int& index, const char* parameterName);
    bool SetOutputType(int ac, const char** av, int& index);
    void SetPackageName(int ac, const char** av, int& index);

    CommandLineArguments(const CommandLineArguments&);
    CommandLineArguments& operator=(const CommandLineArguments&);

};

#endif
