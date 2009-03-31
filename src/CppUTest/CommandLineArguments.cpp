#include "CppUTest/CommandLineArguments.h"
#include "CppUTest/SimpleString.h"
#include "CppUTest/TestPlugin.h"
#include <stdlib.h>


CommandLineArguments::CommandLineArguments(int ac, const char** av, TestPlugin* plugin)
: ac(ac)
, av(av)
, plugin_(plugin)
, verbose_(false)
, repeat_(1)
, groupFilter_("")
, nameFilter_("")
, outputType_(OUTPUT_ECLIPSE)
{
}

CommandLineArguments::~CommandLineArguments()
{
}

bool CommandLineArguments::parse()
{
	bool correctParameters = true;
	for (int i = 1; i < ac; i++) {
		SimpleString argument = av[i];
		if (argument == "-v")
			verbose_ = true;
		else if (argument.startsWith("-r"))
			SetRepeatCount(ac, av, i);
      	else if (argument.startsWith("-g"))
        	SetGroupFilter(ac, av, i);
      	else if (argument.startsWith("-n"))
        	SetNameFilter(ac, av, i);
      	else if (argument.startsWith("-o"))
        	correctParameters = SetOutputType(ac, av, i);
      	else if (argument.startsWith("-p"))
      		correctParameters = plugin_->parseArguments(ac, av, i);
		else
			correctParameters = false;

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

  SimpleString repeatParameter (av[i]);
  if (repeatParameter.size() > 2)
    repeat_ = atoi(av[i] + 2);
  else if (i + 1 < ac)
    {
      repeat_ = atoi(av[i+1]);
      if (repeat_ != 0)
        i++;
    }

  if (0 == repeat_)
    repeat_ = 2;

}

SimpleString CommandLineArguments::getParameterField(int ac, const char** av, int& i)
{
	SimpleString parameter(av[i]);
	if (parameter.size() > 2)
		return av[i] + 2;
	else if (i + 1 < ac)
		return av[++i];
	return 0;
}


void CommandLineArguments::SetGroupFilter(int ac, const char** av, int& i)
{
  groupFilter_ = getParameterField(ac, av, i);
}

void CommandLineArguments::SetNameFilter(int ac, const char** av, int& i)
{
   nameFilter_ = getParameterField(ac, av, i);
}

bool CommandLineArguments::SetOutputType(int ac, const char** av, int& i)
{
	SimpleString outputType = getParameterField(ac, av, i);
	if (outputType.size () == 0) return false;

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
