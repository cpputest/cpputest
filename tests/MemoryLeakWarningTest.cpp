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

#include "TestHarness.h"

EXPORT_TEST_GROUP(MemoryLeakWarningTest);

namespace
{
  char* arrayToLeak1;
  char* arrayToLeak2;
  long* nonArrayToLeak;
  void SetUp()
  {
  }
  void TearDown()
  {
  }
}

//tests run backwards and this one cares because it has to cleanup after the 
//leaky tests
TEST(MemoryLeakWarningTest, CleanUpLeaks)
{
    IGNORE_N_LEAKS(-3);
    delete [] arrayToLeak1;
    delete [] arrayToLeak2;
    delete nonArrayToLeak;
}

TEST(MemoryLeakWarningTest, Ignore1)
{
    IGNORE_N_LEAKS(1);
    arrayToLeak1 = new char[100];
}

TEST(MemoryLeakWarningTest, Ignore2)
{
    IGNORE_N_LEAKS(2);
    arrayToLeak2 = new char[10];
    nonArrayToLeak = new long;
}

