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

GlobalMemoryAllocatorStash::GlobalMemoryAllocatorStash()
    : originalMallocAllocator(NULLPTR), originalNewAllocator(NULLPTR), originalNewArrayAllocator(NULLPTR)
{
}

void GlobalMemoryAllocatorStash::save()
{
    originalMallocAllocator = getCurrentMallocAllocator();
    originalNewAllocator = getCurrentNewAllocator();
    originalNewArrayAllocator = getCurrentNewArrayAllocator();
}

void GlobalMemoryAllocatorStash::restore()
{
    if (originalMallocAllocator) setCurrentMallocAllocator(originalMallocAllocator);
    if (originalNewAllocator) setCurrentNewAllocator(originalNewAllocator);
    if (originalNewArrayAllocator) setCurrentNewArrayAllocator(originalNewArrayAllocator);
}

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
    return alloc_memory(size, "MemoryLeakNode", 1);
}

void TestMemoryAllocator::freeMemoryLeakNode(char* memory)
{
    free_memory(memory, "MemoryLeakNode", 1);
}

char* TestMemoryAllocator::alloc_memory(size_t size, const char*, int)
{
    return checkedMalloc(size);
}

void TestMemoryAllocator::free_memory(char* memory, const char*, int)
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

TestMemoryAllocator* TestMemoryAllocator::actualAllocator()
{
    return this;
}

CrashOnAllocationAllocator::CrashOnAllocationAllocator() : allocationToCrashOn_(0)
{
}

CrashOnAllocationAllocator::~CrashOnAllocationAllocator()
{
}

void CrashOnAllocationAllocator::setNumberToCrashOn(unsigned allocationToCrashOn)
{
    allocationToCrashOn_ = allocationToCrashOn;
}

char* CrashOnAllocationAllocator::alloc_memory(size_t size, const char* file, int line)
{
    if (MemoryLeakWarningPlugin::getGlobalDetector()->getCurrentAllocationNumber() == allocationToCrashOn_)
        UT_CRASH();

    return TestMemoryAllocator::alloc_memory(size, file, line);
}


NullUnknownAllocator::~NullUnknownAllocator()
{
}

char* NullUnknownAllocator::alloc_memory(size_t /*size*/, const char*, int)
{
    return NULLPTR;
}

void NullUnknownAllocator::free_memory(char* /*memory*/, const char*, int)
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

FailableMemoryAllocator::~FailableMemoryAllocator()
{
}

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

char* FailableMemoryAllocator::alloc_memory(size_t size, const char* file, int line)
{
    currentAllocNumber_++;
    LocationToFailAllocNode* current = head_;
    LocationToFailAllocNode* previous = NULLPTR;

    while (current) {
      if (current->shouldFail(currentAllocNumber_, file, line)) {
        if (previous) previous->next_ = current->next_;
        else head_ = current->next_;

        free_memory((char*) current, __FILE__, __LINE__);
        return NULLPTR;
      }
      previous = current;
      current = current->next_;
    }
    return TestMemoryAllocator::alloc_memory(size, file, line);
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

        currentTest->failWith(FailFailure(currentTest, currentTest->getName().asCharString(), currentTest->getLineNumber(), failText));
    }
}

void FailableMemoryAllocator::clearFailedAllocs()
{
  LocationToFailAllocNode* current = head_;
  while (current) {
    head_ = current->next_;
    free_memory((char*) current, __FILE__, __LINE__);
    current = head_;
  }
  currentAllocNumber_ = 0;
}

struct MemoryAccountantAllocationNode
{
    size_t size_;
    size_t allocations_;
    size_t deallocations_;
    size_t maxAllocations_;
    size_t currentAllocations_;
    MemoryAccountantAllocationNode* next_;
};

MemoryAccountantAllocationNode* MemoryAccountant::createNewAccountantAllocationNode(size_t size, MemoryAccountantAllocationNode* next)
{
    MemoryAccountantAllocationNode* node = (MemoryAccountantAllocationNode*) (void*) allocator_->alloc_memory(sizeof(MemoryAccountantAllocationNode), __FILE__, __LINE__);
    node->size_ = size;
    node->allocations_ = 0;
    node->deallocations_ = 0;
    node->maxAllocations_ = 0;
    node->currentAllocations_ = 0;
    node->next_ = next;
    return node;
}

void MemoryAccountant::destroyAccountantAllocationNode(MemoryAccountantAllocationNode* node)
{
    allocator_->free_memory((char*) node, __FILE__, __LINE__);
}

MemoryAccountant::MemoryAccountant()
    : head_(NULLPTR), allocator_(defaultMallocAllocator())
{
}

void MemoryAccountant::setAllocator(TestMemoryAllocator* allocator)
{
    allocator_ = allocator;
}

void MemoryAccountant::clear()
{
    MemoryAccountantAllocationNode* node = head_;
    MemoryAccountantAllocationNode* to_be_deleted = NULLPTR;
    while (node) {
        to_be_deleted = node;
        node = node->next_;
        destroyAccountantAllocationNode(to_be_deleted);
    }
}

MemoryAccountantAllocationNode* MemoryAccountant::findNodeOfSize(size_t size) const
{
    for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_)
        if (node->size_ == size)
            return node;
    return NULLPTR;
}

MemoryAccountantAllocationNode* MemoryAccountant::findOrCreateNodeOfSize(size_t size)
{
    if (head_ && head_->size_ > size)
        head_ = createNewAccountantAllocationNode(size, head_);

    for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_) {
        if (node->size_ == size)
            return node;
        if (node->next_ == NULLPTR || node->next_->size_ > size)
            node->next_ = createNewAccountantAllocationNode(size, node->next_);
    }
    head_ = createNewAccountantAllocationNode(size, head_);
    return head_;
}

void MemoryAccountant::alloc(size_t size)
{
    MemoryAccountantAllocationNode* node = findOrCreateNodeOfSize(size);
    node->allocations_++;
    node->currentAllocations_++;
    node->maxAllocations_ = (node->currentAllocations_ > node->maxAllocations_) ? node->currentAllocations_ : node->maxAllocations_;
}

void MemoryAccountant::dealloc(size_t size)
{
    MemoryAccountantAllocationNode* node = findOrCreateNodeOfSize(size);
    node->deallocations_++;
    node->currentAllocations_--;
}

size_t MemoryAccountant::totalAllocationsOfSize(size_t size) const
{
    MemoryAccountantAllocationNode* node = findNodeOfSize(size);
    if (node)
      return node->allocations_;
    return 0;
}

size_t MemoryAccountant::totalDeallocationsOfSize(size_t size) const
{
    MemoryAccountantAllocationNode* node = findNodeOfSize(size);
    if (node)
      return node->deallocations_;
    return 0;
}

size_t MemoryAccountant::maximumAllocationAtATimeOfSize(size_t size) const
{
    MemoryAccountantAllocationNode* node = findNodeOfSize(size);
    if (node)
      return node->maxAllocations_;
    return 0;
}

size_t MemoryAccountant::totalAllocations() const
{
    size_t totalAllocations = 0;

    for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_)
        totalAllocations += node->allocations_;

    return totalAllocations;
}

size_t MemoryAccountant::totalDeallocations() const
{
    size_t totalDeallocations = 0;

    for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_)
        totalDeallocations += node->deallocations_;

    return totalDeallocations;
}

SimpleString MemoryAccountant::report() const
{
    if (head_ == NULLPTR)
      return SimpleString("CppUTest Memory Accountant has not noticed any allocations or deallocations. Sorry\n");

    SimpleString report("CppUTest Memory Accountant report:\n"
                        "Allocation size     # allocations    # deallocations   max # allocations at one time\n");
    for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_)
        report += StringFromFormat("%5d               %5d            %5d             %5d\n",
            (int) node->size_, (int) node->allocations_, (int) node->deallocations_, (int) node->maxAllocations_);
    report += SimpleString("   Thank you for your business\n");
    return report;
}

AccountingTestMemoryAllocator::AccountingTestMemoryAllocator(MemoryAccountant& accountant, TestMemoryAllocator* originalAllocator)
    : accountant_(accountant), originalAllocator_(originalAllocator), head_(NULLPTR)
{
}

AccountingTestMemoryAllocator::~AccountingTestMemoryAllocator()
{
}

struct AccountingTestMemoryAllocatorMemoryNode
{
    char* memory_;
    size_t size_;
    AccountingTestMemoryAllocatorMemoryNode* next_;
};

void AccountingTestMemoryAllocator::addMemoryToMemoryTrackingToKeepTrackOfSize(char* memory, size_t size)
{
    AccountingTestMemoryAllocatorMemoryNode* node = (AccountingTestMemoryAllocatorMemoryNode*) (void*) originalAllocator_->alloc_memory(sizeof(AccountingTestMemoryAllocatorMemoryNode), __FILE__, __LINE__);
    node->memory_ = memory;
    node->size_ = size;
    node->next_ = head_;
    head_ = node;
}

size_t AccountingTestMemoryAllocator::removeNextNodeAndReturnSize(AccountingTestMemoryAllocatorMemoryNode* node)
{
    AccountingTestMemoryAllocatorMemoryNode* foundNode = node->next_;
    node->next_ = node->next_->next_;

    size_t size = foundNode->size_;
    originalAllocator_->free_memory((char*) foundNode, __FILE__, __LINE__);
    return size;
}

size_t AccountingTestMemoryAllocator::removeHeadAndReturnSize()
{
    AccountingTestMemoryAllocatorMemoryNode* foundNode = head_;
    head_ = head_->next_;

    size_t size = foundNode->size_;
    originalAllocator_->free_memory((char*) foundNode, __FILE__, __LINE__);
    return size;
}

size_t AccountingTestMemoryAllocator::removeMemoryFromTrackingAndReturnAllocatedSize(char* memory)
{
    if (head_ && head_->memory_ == memory)
        return removeHeadAndReturnSize();

    for (AccountingTestMemoryAllocatorMemoryNode* node = head_; node; node = node->next_) {
        if (node->next_ && node->next_->memory_ == memory)
            return removeNextNodeAndReturnSize(node);
    }

    return 0;
}

char* AccountingTestMemoryAllocator::alloc_memory(size_t size, const char* file, int line)
{
    accountant_.alloc(size);
    char* memory = originalAllocator_->alloc_memory(size, file, line);
    addMemoryToMemoryTrackingToKeepTrackOfSize(memory, size);
    return memory;
}

void AccountingTestMemoryAllocator::free_memory(char* memory, const char* file, int line)
{
    size_t size = removeMemoryFromTrackingAndReturnAllocatedSize(memory);
    accountant_.dealloc(size);
    originalAllocator_->free_memory(memory, file, line);
}

TestMemoryAllocator* AccountingTestMemoryAllocator::actualAllocator()
{
    return originalAllocator_->actualAllocator();
}

TestMemoryAllocator* AccountingTestMemoryAllocator::originalAllocator()
{
    return originalAllocator_;
}

const char* AccountingTestMemoryAllocator::alloc_name() const
{
    return originalAllocator_->alloc_name();
}

const char* AccountingTestMemoryAllocator::free_name() const
{
    return originalAllocator_->free_name();
}

GlobalMemoryAccountant::GlobalMemoryAccountant()
    : mallocAllocator_(NULLPTR), newAllocator_(NULLPTR), newArrayAllocator_(NULLPTR)
{
}

GlobalMemoryAccountant::~GlobalMemoryAccountant()
{
    restoreMemoryAllocators();
    delete mallocAllocator_;
    delete newAllocator_;
    delete newArrayAllocator_;
}

void GlobalMemoryAccountant::start()
{
    if (mallocAllocator_ != NULLPTR)
      FAIL("Global allocator start called twice!");

    mallocAllocator_ = new AccountingTestMemoryAllocator(accountant_, getCurrentMallocAllocator());
    newAllocator_ = new AccountingTestMemoryAllocator(accountant_, getCurrentNewAllocator());
    newArrayAllocator_ = new AccountingTestMemoryAllocator(accountant_, getCurrentNewArrayAllocator());

    accountant_.setAllocator(getCurrentMallocAllocator());

    setCurrentMallocAllocator(mallocAllocator_);
    setCurrentNewAllocator(newAllocator_);
    setCurrentNewArrayAllocator(newArrayAllocator_);
}

void GlobalMemoryAccountant::restoreMemoryAllocators()
{
    if (getCurrentMallocAllocator() == mallocAllocator_)
        setCurrentMallocAllocator(mallocAllocator_->originalAllocator());

    if (getCurrentNewAllocator() == newAllocator_)
        setCurrentNewAllocator(newAllocator_->originalAllocator());

    if (getCurrentNewArrayAllocator() == newArrayAllocator_)
        setCurrentNewArrayAllocator(newArrayAllocator_->originalAllocator());
}

void GlobalMemoryAccountant::stop()
{
    if (mallocAllocator_ == NULLPTR)
      FAIL("GlobalMemoryAccount: Stop called without starting");

    if (getCurrentMallocAllocator() != mallocAllocator_)
        FAIL("GlobalMemoryAccountant: Malloc memory allocator has been changed while accounting for memory");

    if (getCurrentNewAllocator() != newAllocator_)
        FAIL("GlobalMemoryAccountant: New memory allocator has been changed while accounting for memory");

    if (getCurrentNewArrayAllocator() != newArrayAllocator_)
        FAIL("GlobalMemoryAccountant: New Array memory allocator has been changed while accounting for memory");

    restoreMemoryAllocators();
}

SimpleString GlobalMemoryAccountant::report()
{
    return accountant_.report();
}

TestMemoryAllocator* GlobalMemoryAccountant::getMallocAllocator()
{
    return mallocAllocator_;
}

TestMemoryAllocator* GlobalMemoryAccountant::getNewAllocator()
{
    return newAllocator_;
}

TestMemoryAllocator* GlobalMemoryAccountant::getNewArrayAllocator()
{
    return newArrayAllocator_;
}


