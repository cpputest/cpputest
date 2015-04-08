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

#ifndef D_ConsoleTestOutput_h
#define D_ConsoleTestOutput_h

#include "CppUTest/TestOutput.h"


///////////////////////////////////////////////////////////////////////////////
//
//  ConsoleTestOutput.h
//
//  Printf Based Solution
//
///////////////////////////////////////////////////////////////////////////////

class ConsoleTestOutput: public TestOutput
{
public:
    explicit ConsoleTestOutput( bool verbose = false, bool color = false );
    virtual ~ConsoleTestOutput()
    {
    }

    virtual void printTestsStarted() _override;
    virtual void printTestsEnded( const TestResult& result ) _override;
    virtual void printCurrentTestStarted( const UtestShell& test ) _override;
    virtual void printCurrentTestEnded( const TestResult& res ) _override;
    virtual void printCurrentGroupStarted( const UtestShell& test ) _override;
    virtual void printCurrentGroupEnded( const TestResult& res ) _override;

    virtual void printBuffer( const char* ) _override;
    virtual void print( const char* ) _override;
    virtual void print( long ) _override;
    virtual void printDouble( double ) _override;
    virtual void print( const TestFailure& failure ) _override;
    virtual void printTestRun( int number, int total ) _override;
    virtual void setProgressIndicator( const char* );

    virtual void flush() _override;

protected:
  virtual void printEclipseErrorInFileOnLine( SimpleString file, int lineNumber );
  virtual void printVistualStudioErrorInFileOnLine( SimpleString file, int lineNumber );

  virtual void printProgressIndicator();
  void printFileAndLineForTestAndFailure( const TestFailure& failure );
  void printFileAndLineForFailure( const TestFailure& failure );
  void printFailureInTest( SimpleString testName );
  void printFailureMessage( SimpleString reason );
  void printErrorInFileOnLineFormattedForWorkingEnvironment( SimpleString testFile, int lineNumber );

  int dotCount_;
  bool verbose_;
  bool color_;
  const char* progressIndication_;

private:
    ConsoleTestOutput(const ConsoleTestOutput&);
    ConsoleTestOutput& operator=(const ConsoleTestOutput&);
};

#endif
