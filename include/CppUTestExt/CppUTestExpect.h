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


/** @returns expectations of the namespace */
class Expectations;
Expectations expect(
    const SimpleString& mockNamespace = ""
    // MockFailureReporter* failureReporterForThisCall = nullptr
);


/**
 * @brief Registry of expectations
 */
class Expectation;
class Expectations
{
public:
    Expectations( const SimpleString& mockNamespace );

    /// whether to FAIL when a mocked call has no matching expectation
    void strict( const bool = false );

    /// adds an expectation for a call based on the name used in the actual()
    Expectation call( const SimpleString& functionName );

private:
    const SimpleString _mockNamespace;
};


class IModel;
class Expectation
{
public:

    /// @post registers the expectation
    ~Expectation();

    // /// expectation of position in sequence
    // Expectation inSequence( Sequence& );


    /// expectation for an input parameter with value
    Expectation with( const SimpleString& parameterName, const bool value );
    // Expectation with( const SimpleString& parameterName, const char value );
    // Expectation with( const SimpleString& parameterName, const unsigned char value );
    // Expectation with( const SimpleString& parameterName, const int value );
    // Expectation with( const SimpleString& parameterName, const unsigned int value );
    // Expectation with( const SimpleString& parameterName, const long value );
    // Expectation with( const SimpleString& parameterName, const unsigned long value );
    // Expectation with( const SimpleString& parameterName, const long long value );
    // Expectation with( const SimpleString& parameterName, const unsigned long long value );
    // Expectation with( const SimpleString& parameterName, const double value );
    // Expectation with( const SimpleString& parameterName, const void* value );
    // Expectation with( const SimpleString& parameterName, const void* const value );
    // Expectation with( const SimpleString& parameterName, const void* value, const std::size_t );
    // Expectation with( const SimpleString& parameterName, const void* const value, const std::size_t );
    // Expectation with( const SimpleString& parameterName, void (value*)() );
    // /// expectation for an input parameter matched by comparator with value
    // Expectation with( const ParameterComparator&, const bool value );
    // Expectation with( const ParameterComparator&, const char value );
    // Expectation with( const ParameterComparator&, const unsigned char value );
    // Expectation with( const ParameterComparator&, const int value );
    // Expectation with( const ParameterComparator&, const unsigned int value );
    // Expectation with( const ParameterComparator&, const long value );
    // Expectation with( const ParameterComparator&, const unsigned long value );
    // Expectation with( const ParameterComparator&, const long long value );
    // Expectation with( const ParameterComparator&, const unsigned long long value );
    // Expectation with( const ParameterComparator&, const double value );
    // Expectation with( const ParameterComparator&, const void* value ) ;
    // Expectation with( const ParameterComparator&, const void* const value ) ;
    // Expectation with( const ParameterComparator&, const void* value, const std::size_t ) ;
    // Expectation with( const ParameterComparator&, const void* const value, const std::size_t ) ;
    // Expectation with( const ParameterComparator&, void (value*)() );


    // /// expectation for the number of calls (cardinality)
    // Expectation times( const std::size_t count );


    // /// use model to perform behavior (i.e. use this to extend a mock into a model)
    // Expectation use( IModel& );


    // /// handle output parameter
    // Expectation output( const SimpleString& parameterName, const bool value );
    // Expectation output( const SimpleString& parameterName, const char value );
    // Expectation output( const SimpleString& parameterName, const unsigned char value );
    // Expectation output( const SimpleString& parameterName, const int value );
    // Expectation output( const SimpleString& parameterName, const unsigned int value );
    // Expectation output( const SimpleString& parameterName, const long value );
    // Expectation output( const SimpleString& parameterName, const unsigned long value );
    // Expectation output( const SimpleString& parameterName, const long long value );
    // Expectation output( const SimpleString& parameterName, const long long value );
    // Expectation output( const SimpleString& parameterName, const double value );
    // Expectation output( const SimpleString& parameterName, const void* value );
    // Expectation output( const SimpleString& parameterName, const void* const value );
    // Expectation output( const SimpleString& parameterName, const void* value, const std::size_t );
    // Expectation output( const SimpleString& parameterName, const void* const value, const std::size_t );
    // Expectation output( const SimpleString& parameterName, void (value*)() );
    // /// handle output parameter matching comparator
    // Expectation output( const ParameterComparator&, const bool value );
    // Expectation output( const ParameterComparator&, const char value );
    // Expectation output( const ParameterComparator&, const unsigned char value );
    // Expectation output( const ParameterComparator&, const int value );
    // Expectation output( const ParameterComparator&, const unsigned int value );
    // Expectation output( const ParameterComparator&, const long value );
    // Expectation output( const ParameterComparator&, const unsigned long value );
    // Expectation output( const ParameterComparator&, const long long value );
    // Expectation output( const ParameterComparator&, const unsigned long long value );
    // Expectation output( const ParameterComparator&, const double value );
    // Expectation output( const ParameterComparator&, const void* value ) ;
    // Expectation output( const ParameterComparator&, const void* const value ) ;
    // Expectation output( const ParameterComparator&, const void* value, const std::size_t ) ;
    // Expectation output( const ParameterComparator&, const void* const value, const std::size_t ) ;
    // Expectation output( const ParameterComparator&, void (value*)() );


    // /// return value (must come last)
    // void andReturn( const bool value );
    // void andReturn( const char value );
    // void andReturn( const unsigned char value );
    // void andReturn( const int value );
    // void andReturn( const unsigned int value );
    // void andReturn( const long value );
    // void andReturn( const unsigned long value );
    // void andReturn( const long long value );
    // void andReturn( const long long value );
    // void andReturn( const double value );
    // void andReturn( const void* value );
    // void andReturn( const void* const value );
    // void andReturn( const void* value, const std::size_t );
    // void andReturn( const void* const value, const std::size_t );
    // void andReturn( void (value*)() );
};


// class Actual;
// /// Behavior handler for an expectation (e.g. Aspect Oriented Programming AoP)
// class IModel
// {
//     /// modifies the actual call instance based on expectation and model state
//     virtual void behave( const Expectation&, Actual& ) = 0;
// };
