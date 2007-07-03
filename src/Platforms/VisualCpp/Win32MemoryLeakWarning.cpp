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

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <crtdbg.h>
#include <windows.h>
#include "UnitTestHarness/MemoryLeakWarning.h"

#ifdef   _DEBUG
#define  SET_CRT_DEBUG_FIELD(a) \
            _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#endif

static int expectCount = 0;


void MemoryLeakWarning::Enable()
{
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG|_CRTDBG_MODE_FILE );
  _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
  _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG|_CRTDBG_MODE_FILE );
  _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR );
  _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG|_CRTDBG_MODE_FILE );
  _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
  SET_CRT_DEBUG_FIELD( _CRTDBG_LEAK_CHECK_DF );
}

const char* MemoryLeakWarning::FinalReport()
{
  //windows reports leaks automatically when set up as above
  return "";
}

static _CrtMemState s1, s2, s3;


void MemoryLeakWarning::CheckPointUsage()
{
  expectCount = 0;
  _CrtMemCheckpoint( &s1 );
}

static char message[50] = "";
bool MemoryLeakWarning::UsageIsNotBalanced()
{
  _CrtMemCheckpoint( &s2 );
  if (_CrtMemDifference( &s3, &s1, &s2) )
  {
      if (s3.lCounts[1] == expectCount)
        return false;
      else
      {
        sprintf(message, "this test leaks %d blocks", s3.lCounts[1]);
        return true;
      }
  }
  else
  {
    return false;
  }
}

const char* MemoryLeakWarning::Message()
{
  return message;
}

void MemoryLeakWarning::IgnoreLeaks(int n)
{
    expectCount = n;
}
