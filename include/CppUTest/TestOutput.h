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

#ifndef D_TestOutput_h
#define D_TestOutput_h

///////////////////////////////////////////////////////////////////////////////
//
//  This is a minimal printer interface.
//  We kept streams out too keep footprint small, and so the test
//  harness could be used with less capable compilers so more
//  platforms could use this test harness
//
///////////////////////////////////////////////////////////////////////////////

#include "SimpleString.h"

class UtestShell;
class TestFailure;
class TestResult;

class TestOutput
{
public:
    explicit TestOutput() {}
    virtual ~TestOutput() {}

    virtual void printTestsStarted() = 0;
    virtual void printTestsEnded( const TestResult& result ) = 0;
    virtual void printCurrentTestStarted( const UtestShell& test ) = 0;
    virtual void printCurrentTestEnded( const TestResult& res ) = 0;
    virtual void printCurrentGroupStarted( const UtestShell& test ) = 0;
    virtual void printCurrentGroupEnded( const TestResult& res ) = 0;

    virtual void printBuffer(const char*)=0;
    virtual void print( const char* ) = 0;
    virtual void print( long ) = 0;
    virtual void printDouble( double ) = 0;
    virtual void print( const TestFailure& failure ) = 0;
    virtual void printTestRun( int number, int total ) = 0;

    virtual void flush()=0;

    enum WorkingEnvironment {vistualStudio, eclipse, detectEnvironment};

    static void setWorkingEnvironment(WorkingEnvironment workEnvironment);
    static WorkingEnvironment getWorkingEnvironment();

protected:
    TestOutput(const TestOutput&);
    TestOutput& operator=(const TestOutput&);

    static WorkingEnvironment workingEnvironment_;
};

TestOutput& operator<<(TestOutput&, const char*);
TestOutput& operator<<(TestOutput&, long);

#endif
