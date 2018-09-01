/*
 * Copyright (c) 2018, Stephen Holstein
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

#include "CppUTest/SimpleString.h"


class Actuals;

/** @returns expectations of the namespace */
Actuals& actual(
    const SimpleString& mockNamespace = ""
    // MockFailureReporter* failureReporterForThisCall = nullptr
);


class Actual;
/**
 * @brief Registry of expectations
 */
class Actuals
{
public:
    Actuals( const SimpleString& mockNamespace );

    /// adds an expectation for a call of the function name
    Actual call( const SimpleString& functionName );

private:
    const SimpleString _mockNamespace;
};


/// @post effects expectation
class Actual
{
public:
    /// @post effects expectation if return<type> has not been called
    ~Actual();

    /// expectation for an input parameter with value
    Actual with( const SimpleString& parameterName, const bool value );
    // Actual with( const SimpleString& parameterName, const char value );
    // Actual with( const SimpleString& parameterName, const unsigned char value );
    // Actual with( const SimpleString& parameterName, const int value );
    // Actual with( const SimpleString& parameterName, const unsigned int value );
    // Actual with( const SimpleString& parameterName, const long value );
    // Actual with( const SimpleString& parameterName, const unsigned long value );
    // Actual with( const SimpleString& parameterName, const long long value );
    // Actual with( const SimpleString& parameterName, const long long value );
    // Actual with( const SimpleString& parameterName, const double value );
    // Actual with( const SimpleString& parameterName, const void* value );
    // Actual with( const SimpleString& parameterName, const void* const value );
    // Actual with( const SimpleString& parameterName, const void* value, const std::size_t );
    // Actual with( const SimpleString& parameterName, const void* const value, const std::size_t );
    // Actual with( const SimpleString& parameterName, void (value*)() );

//     /// identify output parameter
//     Expectation output( const SimpleString& parameterName, bool& value );
//     Expectation output( const SimpleString& parameterName, char& value );
//     Expectation output( const SimpleString& parameterName, unsigned char& value );
//     Expectation output( const SimpleString& parameterName, int& value );
//     Expectation output( const SimpleString& parameterName, unsigned int& value );
//     Expectation output( const SimpleString& parameterName, long& value );
//     Expectation output( const SimpleString& parameterName, unsigned long& value );
//     Expectation output( const SimpleString& parameterName, long long& value );
//     Expectation output( const SimpleString& parameterName, unsigned long long& value );
//     Expectation output( const SimpleString& parameterName, double& value );
//     Expectation output( const SimpleString& parameterName, void*& value );
//     Expectation output( const SimpleString& parameterName, void* const& value );
//     Expectation output( const SimpleString& parameterName, void* value, const std::size_t );
//     Expectation output( const SimpleString& parameterName, void* const& value, const std::size_t );
//     Expectation output( const SimpleString& parameterName, void (value*&)() );

//  return based methods invoke matched expectation (or else do nothing and return 0)

//     bool returnBool();
//     char returnChar();
//     unsigned char returnUnsignedChar();
//     int returnInt();
//     unsigned int returnUnsignedInt();
//     long returnLong();
//     unsigned long returnLong();
//     long long returnLongLong();
//     unsigned long long returnUnsignedLongLong();
//     double returnDouble();
//     void* returnPointer();
//     const void* returnConstPointer();
//     void (*)() returnFunctionPointer();

// private:
//     const SimpleString& _mockName;  ///< namespace for this actual
};