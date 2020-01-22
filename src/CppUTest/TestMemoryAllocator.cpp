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
#include "CppUTest/TestMemoryAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/MemoryLeakDetector.h"

static char* checkedMalloc(size_t size)
{
    char* mem = (char*) PlatformSpecificMalloc(size);
    if (mem == NULLPTR)
    FAIL("malloc returned null pointer");
    return mem;
}

static TestMemoryAllocator* currentNewAllocator = NULLPTR;
static TestMemoryAllocator* currentNewArrayAllocator = NULLPTR;
static TestMemoryAllocator* currentMallocAllocator = NULLPTR;

void setCurrentNewAllocator(TestMemoryAllocator* allocator)
{
    currentNewAllocator = allocator;
}

TestMemoryAllocator* getCurrentNewAllocator()
{
    if (currentNewAllocator == NULLPTR) setCurrentNewAllocatorToDefault();
    return currentNewAllocator;
}

void setCurrentNewAllocatorToDefault()
{
    currentNewAllocator = defaultNewAllocator();
}

TestMemoryAllocator* defaultNewAllocator()
{
    static TestMemoryAllocator allocator("Standard New Allocator", "new", "delete");
    return &allocator;
}

void setCurrentNewArrayAllocator(TestMemoryAllocator* allocator)
{
    currentNewArrayAllocator = allocator;
}

TestMemoryAllocator* getCurrentNewArrayAllocator()
{
    if (currentNewArrayAllocator == NULLPTR) setCurrentNewArrayAllocatorToDefault();
    return currentNewArrayAllocator;
}

void setCurrentNewArrayAllocatorToDefault()
{
    currentNewArrayAllocator = defaultNewArrayAllocator();
}

TestMemoryAllocator* defaultNewArrayAllocator()
{
    static TestMemoryAllocator allocator("Standard New [] Allocator", "new []", "delete []");
    return &allocator;
}

void setCurrentMallocAllocator(TestMemoryAllocator* allocator)
{
    currentMallocAllocator = allocator;
}

TestMemoryAllocator* getCurrentMallocAllocator()
{
    if (currentMallocAllocator == NULLPTR) setCurrentMallocAllocatorToDefault();
    return currentMallocAllocator;
}

void setCurrentMallocAllocatorToDefault()
{
    currentMallocAllocator = defaultMallocAllocator();
}

TestMemoryAllocator* defaultMallocAllocator()
{
    static TestMemoryAllocator allocator("Standard Malloc Allocator", "malloc", "free");
    return &allocator;
}

/////////////////////////////////////////////

TestMemoryAllocator::TestMemoryAllocator(const char* name_str, const char* alloc_name_str, const char* free_name_str)
    : name_(name_str), alloc_name_(alloc_name_str), free_name_(free_name_str), hasBeenDestroyed_(false)
{
}

TestMemoryAllocator::~TestMemoryAllocator()
{
    hasBeenDestroyed_ = true;
}

bool TestMemoryAllocator::hasBeenDestroyed()
{
    return hasBeenDestroyed_;
}

bool TestMemoryAllocator::isOfEqualType(TestMemoryAllocator* allocator)
{
    return SimpleString::StrCmp(this->name(), allocator->name()) == 0;
}

char* TestMemoryAllocator::allocMemoryLeakNode(size_t size)
{
    return alloc_memory(size, "MemoryLeakNode", 1, NULLPTR);
}

void TestMemoryAllocator::freeMemoryLeakNode(char* memory)
{
    free_memory(memory, "MemoryLeakNode", 1, NULLPTR);
}

char* TestMemoryAllocator::alloc_memory(size_t size, const char*, int, void*)
{
    return checkedMalloc(size);
}

void TestMemoryAllocator::free_memory(char* memory, const char*, int, void*)
{
    PlatformSpecificFree(memory);
}
const char* TestMemoryAllocator::name() const
{
    return name_;
}

const char* TestMemoryAllocator::alloc_name() const
{
    return alloc_name_;
}

const char* TestMemoryAllocator::free_name() const
{
    return free_name_;
}

CrashOnAllocationAllocator::CrashOnAllocationAllocator() : allocationToCrashOn_(0)
{
}

void CrashOnAllocationAllocator::setNumberToCrashOn(unsigned allocationToCrashOn)
{
    allocationToCrashOn_ = allocationToCrashOn;
}

char* CrashOnAllocationAllocator::alloc_memory(size_t size, const char* file, int line, void *caller)
{
    if (MemoryLeakWarningPlugin::getGlobalDetector()->getCurrentAllocationNumber() == allocationToCrashOn_)
        UT_CRASH();

    return TestMemoryAllocator::alloc_memory(size, file, line, caller);
}


char* NullUnknownAllocator::alloc_memory(size_t /*size*/, const char*, int, void *)
{
    return NULLPTR;
}

void NullUnknownAllocator::free_memory(char* /*memory*/, const char*, int, void *)
{
}

NullUnknownAllocator::NullUnknownAllocator()
    : TestMemoryAllocator("Null Allocator", "unknown", "unknown")
{
}


TestMemoryAllocator* NullUnknownAllocator::defaultAllocator()
{
    static NullUnknownAllocator allocator;
    return &allocator;
}

class LocationToFailAllocNode
{
  public:
    int allocNumberToFail_;
    int actualAllocNumber_;
    const char* file_;
    int line_;
    LocationToFailAllocNode* next_;

    void failAtAllocNumber(int number, LocationToFailAllocNode* next)
    {
      init(next);
      allocNumberToFail_ = number;
    }

    void failNthAllocAt(int allocationNumber, const char* file, int line, LocationToFailAllocNode* next)
    {
      init(next);
      allocNumberToFail_ = allocationNumber;
      file_ = file;
      line_ = line;
    }

    bool shouldFail(int allocationNumber, const char* file, int line)
    {
      if (file_ && SimpleString::StrCmp(file, file_) == 0 && line == line_) {
        actualAllocNumber_++;
        return actualAllocNumber_ == allocNumberToFail_;
      }
      if (allocationNumber == allocNumberToFail_)
        return true;
      return false;
    }

  private:
    void init(LocationToFailAllocNode* next = NULLPTR)
    {
      allocNumberToFail_ = 0;
      actualAllocNumber_ = 0;
      file_ = NULLPTR;
      line_ = 0;
      next_ = next;
    }

};

FailableMemoryAllocator::FailableMemoryAllocator(const char* name_str, const char* alloc_name_str, const char* free_name_str)
: TestMemoryAllocator(name_str, alloc_name_str, free_name_str), head_(NULLPTR), currentAllocNumber_(0)
{
}

void FailableMemoryAllocator::failAllocNumber(int number)
{
    LocationToFailAllocNode* newNode = (LocationToFailAllocNode*) (void*) allocMemoryLeakNode(sizeof(LocationToFailAllocNode));
    newNode->failAtAllocNumber(number, head_);
    head_ = newNode;
}

void FailableMemoryAllocator::failNthAllocAt(int allocationNumber, const char* file, int line)
{
    LocationToFailAllocNode* newNode = (LocationToFailAllocNode*) (void*) allocMemoryLeakNode(sizeof(LocationToFailAllocNode));
    newNode->failNthAllocAt(allocationNumber, file, line, head_);
    head_ = newNode;
}

char* FailableMemoryAllocator::alloc_memory(size_t size, const char* file, int line, void *caller)
{
    currentAllocNumber_++;
    LocationToFailAllocNode* current = head_;
    LocationToFailAllocNode* previous = NULLPTR;

    while (current) {
      if (current->shouldFail(currentAllocNumber_, file, line)) {
        if (previous) previous->next_ = current->next_;
        else head_ = current->next_;

        free_memory((char*) current, __FILE__, __LINE__, NULLPTR);
        return NULLPTR;
      }
      previous = current;
      current = current->next_;
    }
    return TestMemoryAllocator::alloc_memory(size, file, line, caller);
}

char* FailableMemoryAllocator::allocMemoryLeakNode(size_t size)
{
    return (char*)PlatformSpecificMalloc(size);
}

void FailableMemoryAllocator::checkAllFailedAllocsWereDone()
{
  if (head_) {
    UtestShell* currentTest = UtestShell::getCurrent();
    SimpleString failText;
    if (head_->file_)
      failText = StringFromFormat("Expected failing alloc at %s:%d was never done", head_->file_, head_->line_);
    else
      failText = StringFromFormat("Expected allocation number %d was never done", head_->allocNumberToFail_);

    currentTest->failWith(FailFailure(currentTest, currentTest->getName().asCharString(),
    currentTest->getLineNumber(), failText), TestTerminatorWithoutExceptions());
  }
}

void FailableMemoryAllocator::clearFailedAllocs()
{
  LocationToFailAllocNode* current = head_;
  while (current) {
    head_ = current->next_;
    free_memory((char*) current, __FILE__, __LINE__, NULLPTR);
    current = head_;
  }
  currentAllocNumber_ = 0;
}

