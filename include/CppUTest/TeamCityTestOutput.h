#ifndef D_TeamCityTestOutput_h
#define D_TeamCityTestOutput_h

#include "TestOutput.h"
#include "SimpleString.h"

class TeamCityTestOutput: public ConsoleTestOutput
{
public:
    TeamCityTestOutput(void);
    virtual ~TeamCityTestOutput(void) CPPUTEST_DESTRUCTOR_OVERRIDE;

    virtual void printCurrentTestStarted(const UtestShell& test) CPPUTEST_OVERRIDE;
    virtual void printCurrentTestEnded(const TestResult& res) CPPUTEST_OVERRIDE;
    virtual void printCurrentGroupStarted(const UtestShell& test) CPPUTEST_OVERRIDE;
    virtual void printCurrentGroupEnded(const TestResult& res) CPPUTEST_OVERRIDE;

    virtual void printFailure(const TestFailure& failure) CPPUTEST_OVERRIDE;

protected:

private:
    void printEscaped(const char* s);
    const UtestShell *currtest_;
    SimpleString currGroup_;
};

#endif
