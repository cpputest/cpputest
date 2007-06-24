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

#include "Failure.h"
#include "Utest.h"
#include "TestOutput.h"


Failure::Failure(Utest* test, long lineNumber, const SimpleString& theMessage)
    : testName (test->getFormattedName())
    , fileName (test->getFile())
    , lineNumber (lineNumber)
    , message (theMessage)
{}


Failure::Failure(Utest* test, const SimpleString& theMessage)
    : testName (test->getFormattedName())
    , fileName (test->getFile())
    , lineNumber (test->getLineNumber())
    , message (theMessage)
{}

Failure::Failure(Utest* test, long lineNum)
    : testName (test->getFormattedName())
    , fileName (test->getFile())
    , lineNumber (lineNum)
    , message("no message")
{}

Failure::~Failure()
{}


void Failure::PrintLeader(TestOutput& p)const
  {
    p 
    << "\n"  << fileName.asCharString()
    << ":" <<lineNumber <<":"
    << "Failure in " << testName.asCharString() << "\n";
  }

void Failure::Print(TestOutput& p)const
  {
    PrintLeader(p);
    PrintSpecifics(p);
    PrintTrailer(p);
  }

void Failure::PrintSpecifics(TestOutput& p)const
  {
    p << "    " << message.asCharString();
  }

void Failure::PrintTrailer(TestOutput& p)const
  {
    p << "\n\n";
  }


