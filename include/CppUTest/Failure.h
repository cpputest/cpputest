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


///////////////////////////////////////////////////////////////////////////////
//
// FAILURE.H
//
// Failure is a class which holds information for a specific
// test failure. It can be overriden for more complex failure messages
//
///////////////////////////////////////////////////////////////////////////////


#ifndef D_Failure_H
#define D_Failure_H

#include "SimpleString.h"

class Utest;
class TestOutput;

class Failure
  {

  public:
    Failure(Utest*, const char* fileName, long lineNumber, const SimpleString& theMessage);
    Failure(Utest*, const SimpleString& theMessage);
    Failure(Utest*, const char* fileName, long lineNumber);
    Failure(const Failure&);
    virtual ~Failure();

	virtual SimpleString getFileName() const;
	virtual SimpleString getTestName() const;
	virtual int getLineNumber() const;
	virtual SimpleString getMessage() const;

  protected:
    SimpleString testName;
    SimpleString fileName;
    long lineNumber;
    SimpleString message;

    Failure& operator=(const Failure&);

  };

class EqualsFailure : public Failure
  {
  public:

     EqualsFailure(Utest*, const char* fileName, long lineNumber,
                   const SimpleString& expected,
                   const SimpleString& actual);

  private:
    EqualsFailure(const EqualsFailure&);
    EqualsFailure& operator=(const EqualsFailure&);
  };

class ContainsFailure : public Failure
  {
  public:

     ContainsFailure(Utest*, const char* fileName, long lineNumber,
                   const SimpleString& expected,
                   const SimpleString& actual);

  private:
    ContainsFailure(const ContainsFailure&);
    ContainsFailure& operator=(const ContainsFailure&);
  };


#endif
