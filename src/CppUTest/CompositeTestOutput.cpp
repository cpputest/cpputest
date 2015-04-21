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

#include "CppUTest/TestHarness.h"
#include "CppUTest/CompositeTestOutput.h"
#include "CppUTest/PlatformSpecificFunctions.h"

CompositeTestOutput::CompositeTestOutput()
  : outputOne_(NULL), outputTwo_(NULL)
{
}

CompositeTestOutput::~CompositeTestOutput()
{
}

void CompositeTestOutput::setOutputOne(TestOutput* output)
{
  outputOne_ = output;
}

void CompositeTestOutput::setOutputTwo(TestOutput* output)
{
  outputTwo_ = output;
}

void CompositeTestOutput::printTestsStarted()
{
  if (outputOne_) outputOne_->printTestsStarted();
  if (outputTwo_) outputTwo_->printTestsStarted();
}

void CompositeTestOutput::printTestsEnded(const TestResult& result)
{
  if (outputOne_) outputOne_->printTestsEnded(result);
  if (outputTwo_) outputTwo_->printTestsEnded(result);
}

void CompositeTestOutput::printCurrentTestStarted(const UtestShell& test)
{
  if (outputOne_) outputOne_->printCurrentTestStarted(test);
  if (outputTwo_) outputTwo_->printCurrentTestStarted(test);
}

void CompositeTestOutput::printCurrentTestEnded(const TestResult& res)
{
  if (outputOne_) outputOne_->printCurrentTestEnded(res);
  if (outputTwo_) outputTwo_->printCurrentTestEnded(res);
}

void CompositeTestOutput::printCurrentGroupStarted(const UtestShell& test)
{
  if (outputOne_) outputOne_->printCurrentGroupStarted(test);
  if (outputTwo_) outputTwo_->printCurrentGroupStarted(test);
}

void CompositeTestOutput::printCurrentGroupEnded(const TestResult& res)
{
  if (outputOne_) outputOne_->printCurrentGroupEnded(res);
  if (outputTwo_) outputTwo_->printCurrentGroupEnded(res);
}

void CompositeTestOutput::verbose()
{
  if (outputOne_) outputOne_->verbose();
  if (outputTwo_) outputTwo_->verbose();
}

void CompositeTestOutput::color()
{
  if (outputOne_) outputOne_->color();
  if (outputTwo_) outputTwo_->color();
}

void CompositeTestOutput::printBuffer(const char* buffer)
{
  if (outputOne_) outputOne_->printBuffer(buffer);
  if (outputTwo_) outputTwo_->printBuffer(buffer);
}

void CompositeTestOutput::print(const char* buffer)
{
  if (outputOne_) outputOne_->print(buffer);
  if (outputTwo_) outputTwo_->print(buffer);
}

void CompositeTestOutput::print(long number)
{
  if (outputOne_) outputOne_->print(number);
  if (outputTwo_) outputTwo_->print(number);
}

void CompositeTestOutput::printDouble(double number)
{
  if (outputOne_) outputOne_->printDouble(number);
  if (outputTwo_) outputTwo_->printDouble(number);
}

void CompositeTestOutput::print(const TestFailure& failure)
{
  if (outputOne_) outputOne_->print(failure);
  if (outputTwo_) outputTwo_->print(failure);
}

void CompositeTestOutput::setProgressIndicator(const char* indicator)
{
  if (outputOne_) outputOne_->setProgressIndicator(indicator);
  if (outputTwo_) outputTwo_->setProgressIndicator(indicator);
}

void CompositeTestOutput::flush()
{
  if (outputOne_) outputOne_->flush();
  if (outputTwo_) outputTwo_->flush();
}

