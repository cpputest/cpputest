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
#ifndef TEST_DOUBLE_H
#define TEST_DOUBLE_H

#include <CppUTest/SimpleString.h>
#include <CppUTestExt/TestDoubleParameter.h>

/// fail the test if an actual call doesn't match an expectation
void failUnexpected();

/// fail the test if an actual call doesn't match the order of expectations
void strictOrder();

/// fail the test if there are unmet expectations
/// @post resets expectation framework state
void checkExpectations();


namespace TestDouble { class ExpectedCall; }
/// create an expectation using a [Builder pattern](https://en.wikipedia.org/wiki/Builder_pattern)
TestDouble::ExpectedCall& expectCall( const SimpleString &call );


// Expectation framework (i.e. Test Double)
//======================================================================================================================
namespace TestDouble {

class ActualCall;

bool shouldFailUnexpected();    ///< returns  state of expectation framework, set to true by failUnexpected()
bool shouldEnforceOrder();      ///< returns  state of expectation framework, set to true by strictOrder()

/// retval 0    no expectation found
const TestDouble::ExpectedCall* findExpectation( const ActualCall &call );

/// list of expectations
class ExpectationChain
{
public:
  const ExpectedCall* const pExpectedCall;
  int                       actualCount = 0;
  ExpectationChain*         pNext = 0;

  ExpectationChain( const ExpectedCall* const &_pExpectedCall, ExpectationChain* const &pLast );
  ~ExpectationChain();
};

class ExpectationQueue
{
public:
  void enqueue( const ExpectedCall* const &pCall );
  ExpectationChain* get() const { return _pExpectations; }

  /// detect un-actualized expectations and then clear expectations
  SimpleString check();
private:
  ExpectationChain*   _pExpectations = 0;
  ExpectationChain*   _pTail = 0;         ///< reference to last enqueued
};

} // namespace TestDouble

#endif /* TEST_DOUBLE_H */
