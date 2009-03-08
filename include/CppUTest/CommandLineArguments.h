#ifndef D_CommandLineArguments_H
#define D_CommandLineArguments_H

///////////////////////////////////////////////////////////////////////////////
//
//  CommandLineArguments is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////
#include "SimpleString.h"
#include "TestOutput.h"

class TestPlugin;

class CommandLineArguments
  {

  public:
	enum OutputType {OUTPUT_NORMAL, OUTPUT_JUNIT};
    explicit CommandLineArguments(int ac, const char** av, TestPlugin*);
    virtual ~CommandLineArguments();

    bool parse();
	bool isVerbose() const;
	int getRepeatCount() const ;
	SimpleString getGroupFilter() const;
	SimpleString getNameFilter() const;
	OutputType getOutputType() const;
	const char* usage() const;

  private:

	int ac;
	const char** av;
	TestPlugin* plugin_;

    bool verbose_;
    int repeat_;
    SimpleString groupFilter_;
    SimpleString nameFilter_;
    OutputType outputType_;

    SimpleString getParameterField(int ac, const char** av, int& i);
    void SetRepeatCount(int ac, const char** av, int& index);
    void SetGroupFilter(int ac, const char** av, int& index);
    void SetNameFilter(int ac, const char** av, int& index);
    bool SetOutputType(int ac, const char** av, int& index);

    CommandLineArguments(const CommandLineArguments&);
    CommandLineArguments& operator=(const CommandLineArguments&);

  };

#endif  // D_CommandLineArguments_H
