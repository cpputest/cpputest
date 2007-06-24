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

#include "MemoryLeakWarning.h"

#include <stdlib.h>
#include <stdio.h>

static int allocatedBlocks = 0;
static int initialBlocksUsed = 0;
static int allocatedArrays = 0;
static int initialArraysUsed = 0;

static char message[100] = "";


void reportMemoryBallance()
{
  int blockBalance = allocatedBlocks - initialBlocksUsed;
  int arrayBalance = allocatedArrays - initialArraysUsed;
  if (blockBalance == 0 && arrayBalance == 0)
    ;
  else if (blockBalance + arrayBalance == 0)
    printf("No leaks but some arrays were deleted without []\n");
  else
    {
      if (blockBalance > 0)
        printf("Memory leak! %d blocks not deleted\n", blockBalance);
      if (arrayBalance > 0)
        printf("Memory leak! %d arrays not deleted\n", arrayBalance);
      if (blockBalance < 0)
        printf("More blocks deleted than newed! %d extra deletes\n", blockBalance);
      if (arrayBalance < 0)
        printf("More arrays deleted than newed! %d extra deletes\n", arrayBalance);

      printf("NOTE - some memory leaks appear to be allocated statics that are not released\n"
             "     - by the standard library\n"
             "     - Use the -r switch on your unit tests to repeat the test sequence\n"
             "     - If no leaks are reported on the second pass, it is likely a static\n"
             "     - that is not released\n");
    }
}


void MemoryLeakWarning::Enable()
{
  initialBlocksUsed = allocatedBlocks;
  initialArraysUsed = allocatedArrays;
  atexit(reportMemoryBallance);
}

const char*  MemoryLeakWarning::FinalReport()
{

  if (initialBlocksUsed != allocatedBlocks || initialArraysUsed != allocatedArrays )
    {
      printf("initial blocks=%d, allocated blocks=%d\ninitial arrays=%d, allocated arrays=%d\n",
             initialBlocksUsed, allocatedBlocks, initialArraysUsed, allocatedArrays);

      return "Memory new/delete imbalance after running tests\n";
    }
  else
    return "";
}

static int blockUsageCheckPoint = 0;
static int arrayUsageCheckPoint = 0;
static int ignoreCount = 0;

void MemoryLeakWarning::CheckPointUsage()
{
  blockUsageCheckPoint = allocatedBlocks;
  arrayUsageCheckPoint = allocatedArrays;
}

bool MemoryLeakWarning::UsageIsNotBalanced()
{
  int arrayBalance = allocatedArrays - arrayUsageCheckPoint;
  int blockBalance = allocatedBlocks - blockUsageCheckPoint;

  if (ignoreCount != 0 && blockBalance + arrayBalance == ignoreCount)
    return false;
  if (blockBalance == 0 && arrayBalance == 0)
    return false;
  else if (blockBalance + arrayBalance == 0)
    sprintf(message, "No leaks but some arrays were deleted without []\n");
  else
    {
      int nchars = 0;
      if (blockUsageCheckPoint != allocatedBlocks)
        nchars = sprintf(message, "this test leaks %d blocks",
                         allocatedBlocks - blockUsageCheckPoint);

      if (arrayUsageCheckPoint != allocatedArrays)
        sprintf(message + nchars, "this test leaks %d arrays",
                allocatedArrays - arrayUsageCheckPoint);
    }

  return true;
}

const char* MemoryLeakWarning::Message()
{
  return message;
}

void* operator new(size_t size)
{
  allocatedBlocks++;
  return malloc(size);
}

void operator delete(void* mem)
{
  allocatedBlocks--;
  free(mem);
}

void* operator new[](size_t size)
{
  allocatedArrays++;
  return malloc(size);
}

void operator delete[](void* mem)
{
  allocatedArrays--;
  free(mem);
}

void MemoryLeakWarning::IgnoreLeaks(int n)
{
    ignoreCount = n;
}
