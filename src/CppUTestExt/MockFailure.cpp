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
#include "CppUTestExt/MockFailure.h"
#include "CppUTestExt/MockExpectedCall.h"
#include "CppUTestExt/MockExpectedCallsList.h"
#include "CppUTestExt/MockNamedValue.h"

class MockFailureReporterTestTerminator : public TestTerminator
{
public:
    MockFailureReporterTestTerminator(bool crashOnFailure) : crashOnFailure_(crashOnFailure)
    {
    }

    virtual void exitCurrentTest() const _override
    {
        if (crashOnFailure_)
            UT_CRASH();

        UtestShell::getCurrentTestTerminator().exitCurrentTest();
    } // LCOV_EXCL_LINE

    virtual ~MockFailureReporterTestTerminator() _destructor_override
    {
    }
private:
    bool crashOnFailure_;

};

void MockFailureReporter::failTest(const MockFailure& failure)
{
    if (!getTestToFail()->hasFailed())
        getTestToFail()->failWith(failure, MockFailureReporterTestTerminator(crashOnFailure_));
} // LCOV_EXCL_LINE

UtestShell* MockFailureReporter::getTestToFail()
{
    return UtestShell::getCurrent();
}

MockFailure::MockFailure(UtestShell* test) : TestFailure(test, "Test failed with MockFailure without an error! Something went seriously wrong.")
{
}

void MockFailure::addExpectationsAndCallHistory(const MockExpectedCallsList& expectations)
{
    message_ += "\tEXPECTED calls that WERE NOT fulfilled:\n";
    message_ += expectations.unfulfilledCallsToString("\t\t");
    message_ += "\n\tEXPECTED calls that WERE fulfilled:\n";
    message_ += expectations.fulfilledCallsToString("\t\t");
}

void MockFailure::addExpectationsAndCallHistoryRelatedTo(const SimpleString& name, const MockExpectedCallsList& expectations)
{
    MockExpectedCallsList expectationsForFunction;
    expectationsForFunction.addExpectationsRelatedTo(name, expectations);

    message_ += "\tEXPECTED calls that WERE NOT fulfilled related to function: ";
    message_ += name;
    message_ += "\n";

    message_ += expectationsForFunction.unfulfilledCallsToString("\t\t");

    message_ += "\n\tEXPECTED calls that WERE fulfilled related to function: ";
    message_ += name;
    message_ += "\n";

    message_ += expectationsForFunction.fulfilledCallsToString("\t\t");
}

MockExpectedCallsDidntHappenFailure::MockExpectedCallsDidntHappenFailure(UtestShell* test, const MockExpectedCallsList& expectations) : MockFailure(test)
{
    message_ = "Mock Failure: Expected call WAS NOT fulfilled.\n";
    addExpectationsAndCallHistory(expectations);
}

MockUnexpectedCallHappenedFailure::MockUnexpectedCallHappenedFailure(UtestShell* test, const SimpleString& name, const MockExpectedCallsList& expectations) : MockFailure(test)
{
    unsigned int amountOfActualCalls = expectations.amountOfActualCallsFulfilledFor(name);
    if (amountOfActualCalls > 0) {
        SimpleString ordinalNumber = StringFromOrdinalNumber(amountOfActualCalls + 1);
        message_ = StringFromFormat("Mock Failure: Unexpected additional (%s) call to function: ", ordinalNumber.asCharString());
    } else {
        message_ = "Mock Failure: Unexpected call to function: ";
    }
    message_ += name;
    message_ += "\n";
    addExpectationsAndCallHistory(expectations);
}

MockCallOrderFailure::MockCallOrderFailure(UtestShell* test, const MockExpectedCallsList& expectations) : MockFailure(test)
{
    MockExpectedCallsList expectationsForOutOfOrder;
    expectationsForOutOfOrder.addExpectations(expectations);
    expectationsForOutOfOrder.onlyKeepOutOfOrderExpectations();

    message_ = "Mock Failure: Out of order calls";
    message_ += "\n";
    addExpectationsAndCallHistory(expectationsForOutOfOrder);
}

MockUnexpectedInputParameterFailure::MockUnexpectedInputParameterFailure(UtestShell* test, const SimpleString& functionName, const MockNamedValue& parameter, const MockExpectedCallsList& expectations)  : MockFailure(test)
{
    MockExpectedCallsList expectationsForFunctionWithParameterName;
    expectationsForFunctionWithParameterName.addExpectationsRelatedTo(functionName, expectations);
    expectationsForFunctionWithParameterName.onlyKeepExpectationsWithInputParameterName(parameter.getName());

    if (expectationsForFunctionWithParameterName.isEmpty()) {
        message_ = "Mock Failure: Unexpected parameter name to function \"";
        message_ += functionName;
        message_ += "\": ";
        message_ += parameter.getName();
    }
    else {
        message_ = "Mock Failure: Unexpected parameter value to parameter \"";
        message_ += parameter.getName();
        message_ += "\" to function \"";
        message_ += functionName;
        message_ += "\": <";
        message_ += StringFrom(parameter);
        message_ += ">";
    }

    message_ += "\n";
    addExpectationsAndCallHistoryRelatedTo(functionName, expectations);

    message_ += "\n\tACTUAL unexpected parameter passed to function: ";
    message_ += functionName;
    message_ += "\n";

    message_ += "\t\t";
    message_ += parameter.getType();
    message_ += " ";
    message_ += parameter.getName();
    message_ += ": <";
    message_ += StringFrom(parameter);
    message_ += ">";
}

MockUnexpectedOutputParameterFailure::MockUnexpectedOutputParameterFailure(UtestShell* test, const SimpleString& functionName, const MockNamedValue& parameter, const MockExpectedCallsList& expectations)  : MockFailure(test)
{
    MockExpectedCallsList expectationsForFunctionWithParameterName;
    expectationsForFunctionWithParameterName.addExpectationsRelatedTo(functionName, expectations);
    expectationsForFunctionWithParameterName.onlyKeepExpectationsWithOutputParameterName(parameter.getName());

    if (expectationsForFunctionWithParameterName.isEmpty()) {
        message_ = "Mock Failure: Unexpected output parameter name to function \"";
        message_ += functionName;
        message_ += "\": ";
        message_ += parameter.getName();
    }
    else {
        message_ = "Mock Failure: Unexpected parameter type \"";
        message_ += parameter.getType();
        message_ += "\" to output parameter \"";
        message_ += parameter.getName();
        message_ += "\" to function \"";
        message_ += functionName;
        message_ += "\"";
    }

    message_ += "\n";
    addExpectationsAndCallHistoryRelatedTo(functionName, expectations);

    message_ += "\n\tACTUAL unexpected output parameter passed to function: ";
    message_ += functionName;
    message_ += "\n";

    message_ += "\t\t";
    message_ += parameter.getType();
    message_ += " ";
    message_ += parameter.getName();
}

MockExpectedParameterDidntHappenFailure::MockExpectedParameterDidntHappenFailure(UtestShell* test, const SimpleString& functionName, 
                                                                                 const MockExpectedCallsList& allExpectations,
                                                                                 const MockExpectedCallsList& matchingExpectations) : MockFailure(test)
{
    message_ = "Mock Failure: Expected parameter for function \"";
    message_ += functionName;
    message_ += "\" did not happen.\n";

    message_ += "\tEXPECTED calls with MISSING parameters related to function: ";
    message_ += functionName;
    message_ += "\n";
    message_ += matchingExpectations.callsWithMissingParametersToString("\t\t", "\tMISSING parameters: ");
    message_ += "\n";

    addExpectationsAndCallHistoryRelatedTo(functionName, allExpectations);
}

MockNoWayToCompareCustomTypeFailure::MockNoWayToCompareCustomTypeFailure(UtestShell* test, const SimpleString& typeName) : MockFailure(test)
{
    message_ = StringFromFormat("MockFailure: No way to compare type <%s>. Please install a MockNamedValueComparator.", typeName.asCharString());
}

MockNoWayToCopyCustomTypeFailure::MockNoWayToCopyCustomTypeFailure(UtestShell* test, const SimpleString& typeName) : MockFailure(test)
{
    message_ = StringFromFormat("MockFailure: No way to copy type <%s>. Please install a MockNamedValueCopier.", typeName.asCharString());
}

MockUnexpectedObjectFailure::MockUnexpectedObjectFailure(UtestShell* test, const SimpleString& functionName, const void* actual, const MockExpectedCallsList& expectations) : MockFailure(test)
{
    message_ = StringFromFormat ("MockFailure: Function called on an unexpected object: %s\n"
                                 "\tActual object for call has address: <%p>\n", functionName.asCharString(),actual);
    addExpectationsAndCallHistoryRelatedTo(functionName, expectations);
}

MockExpectedObjectDidntHappenFailure::MockExpectedObjectDidntHappenFailure(UtestShell* test, const SimpleString& functionName, const MockExpectedCallsList& expectations) : MockFailure(test)
{
    message_ = StringFromFormat("Mock Failure: Expected call on object for function \"%s\" but it did not happen.\n", functionName.asCharString());
    addExpectationsAndCallHistoryRelatedTo(functionName, expectations);
}


