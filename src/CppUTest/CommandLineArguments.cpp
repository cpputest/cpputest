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

CommandLineArguments::CommandLineArguments(int ac, const char** av) :
	ac_(ac), av_(av), verbose_(false), repeat_(1), groupFilter_(""), nameFilter_(""), outputType_(OUTPUT_ECLIPSE)
{
}

CommandLineArguments::~CommandLineArguments()
{
}

bool CommandLineArguments::parse(TestPlugin* plugin)
{
	bool correctParameters = true;
	for (int i = 1; i < ac_; i++) {
		SimpleString argument = av_[i];
		if (argument == "-v") verbose_ = true;
		else if (argument.startsWith("-r")) SetRepeatCount(ac_, av_, i);
		else if (argument.startsWith("-g")) SetGroupFilter(ac_, av_, i);
		else if (argument.startsWith("-n")) SetNameFilter(ac_, av_, i);
		else if (argument.startsWith("-o")) correctParameters = SetOutputType(ac_, av_, i);
		else if (argument.startsWith("-p")) correctParameters = plugin->parseAllArguments(ac_, av_, i);
		else correctParameters = false;

		if (correctParameters == false) {
			return false;
		}
	}
	return true;
}

const char* CommandLineArguments::usage() const
{
	return "usage [-v] [-r#] [-g groupName] [-n testName] [-o{normal, junit}]\n";
}

bool CommandLineArguments::isVerbose() const
{
	return verbose_;
}

int CommandLineArguments::getRepeatCount() const
{
	return repeat_;
}

SimpleString CommandLineArguments::getGroupFilter() const
{
	return groupFilter_;
}

SimpleString CommandLineArguments::getNameFilter() const
{
	return nameFilter_;
}

void CommandLineArguments::SetRepeatCount(int ac, const char** av, int& i)
{
	repeat_ = 0;

	SimpleString repeatParameter(av[i]);
	if (repeatParameter.size() > 2) repeat_ = PlatformSpecificAtoI(av[i] + 2);
	else if (i + 1 < ac) {
		repeat_ = PlatformSpecificAtoI(av[i + 1]);
		if (repeat_ != 0) i++;
	}

	if (0 == repeat_) repeat_ = 2;

}

SimpleString CommandLineArguments::getParameterField(int ac, const char** av, int& i)
{
	SimpleString parameter(av[i]);
	if (parameter.size() > 2) return av[i] + 2;
	else if (i + 1 < ac) return av[++i];
	return "";
}

void CommandLineArguments::SetGroupFilter(int ac, const char** av, int& i)
{
	SimpleString gf = getParameterField(ac, av, i);
	groupFilter_ = gf;
}

void CommandLineArguments::SetNameFilter(int ac, const char** av, int& i)
{
	nameFilter_ = getParameterField(ac, av, i);
}

bool CommandLineArguments::SetOutputType(int ac, const char** av, int& i)
{
	SimpleString outputType = getParameterField(ac, av, i);
	if (outputType.size() == 0) return false;

	if (outputType == "normal" || outputType == "eclipse") {
		outputType_ = OUTPUT_ECLIPSE;
		return true;
	}
	if (outputType == "junit") {
		outputType_ = OUTPUT_JUNIT;
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

