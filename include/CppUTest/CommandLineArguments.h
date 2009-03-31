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
    explicit CommandLineArguments(int ac, const char** av, TestPlugin*);
    virtual ~CommandLineArguments();

    bool parse();
	bool isVerbose() const;
	int getRepeatCount() const ;
	SimpleString getGroupFilter() const;
	SimpleString getNameFilter() const;
	bool isJUnitOutput() const;
	bool isEclipseOutput() const;
	const char* usage() const;

  private:

	enum OutputType {OUTPUT_ECLIPSE, OUTPUT_JUNIT};
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
