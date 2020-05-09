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
    free_memory(memory, 0, "MemoryLeakNode", 1);
}

char* TestMemoryAllocator::alloc_memory(size_t size, const char*, size_t)
{
    return checkedMalloc(size);
}

void TestMemoryAllocator::free_memory(char* memory, size_t, const char*, size_t)
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

MemoryLeakAllocator::MemoryLeakAllocator(TestMemoryAllocator* originalAllocator)
    : originalAllocator_(originalAllocator)
{
}

MemoryLeakAllocator::~MemoryLeakAllocator()
{
}

char* MemoryLeakAllocator::alloc_memory(size_t size, const char* file, size_t line)
{
    return MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(originalAllocator_, size, file, line);
}

void MemoryLeakAllocator::free_memory(char* memory, size_t, const char* file, size_t line)
{
    MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(originalAllocator_, memory, file, line);
}

const char* MemoryLeakAllocator::name() const
{
    return "MemoryLeakAllocator";
}

const char* MemoryLeakAllocator::alloc_name() const
{
    return originalAllocator_->alloc_name();
}

const char* MemoryLeakAllocator::free_name() const
{
    return originalAllocator_->free_name();
}

TestMemoryAllocator* MemoryLeakAllocator::actualAllocator()
{
    return originalAllocator_->actualAllocator();
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

char* CrashOnAllocationAllocator::alloc_memory(size_t size, const char* file, size_t line)
{
    if (MemoryLeakWarningPlugin::getGlobalDetector()->getCurrentAllocationNumber() == allocationToCrashOn_)
        UT_CRASH();

    return TestMemoryAllocator::alloc_memory(size, file, line);
}


NullUnknownAllocator::~NullUnknownAllocator()
{
}

char* NullUnknownAllocator::alloc_memory(size_t /*size*/, const char*, size_t)
{
    return NULLPTR;
}

void NullUnknownAllocator::free_memory(char* /*memory*/, size_t, const char*, size_t)
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
    size_t line_;
    LocationToFailAllocNode* next_;

    void failAtAllocNumber(int number, LocationToFailAllocNode* next)
    {
      init(next);
      allocNumberToFail_ = number;
    }

    void failNthAllocAt(int allocationNumber, const char* file, size_t line, LocationToFailAllocNode* next)
    {
      init(next);
      allocNumberToFail_ = allocationNumber;
      file_ = file;
      line_ = line;
    }

    bool shouldFail(int allocationNumber, const char* file, size_t line)
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

void FailableMemoryAllocator::failNthAllocAt(int allocationNumber, const char* file, size_t line)
{
    LocationToFailAllocNode* newNode = (LocationToFailAllocNode*) (void*) allocMemoryLeakNode(sizeof(LocationToFailAllocNode));
    newNode->failNthAllocAt(allocationNumber, file, line, head_);
    head_ = newNode;
}

char* FailableMemoryAllocator::alloc_memory(size_t size, const char* file, size_t line)
{
    currentAllocNumber_++;
    LocationToFailAllocNode* current = head_;
    LocationToFailAllocNode* previous = NULLPTR;

    while (current) {
      if (current->shouldFail(currentAllocNumber_, file, line)) {
        if (previous) previous->next_ = current->next_;
        else head_ = current->next_;

        free_memory((char*) current, size, __FILE__, __LINE__);
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
            failText = StringFromFormat("Expected failing alloc at %s:%d was never done", head_->file_, (int) head_->line_);
        else
            failText = StringFromFormat("Expected allocation number %d was never done", (int) head_->allocNumberToFail_);

        currentTest->failWith(FailFailure(currentTest, currentTest->getName().asCharString(), currentTest->getLineNumber(), failText));
    }
}

void FailableMemoryAllocator::clearFailedAllocs()
{
  LocationToFailAllocNode* current = head_;
  while (current) {
    head_ = current->next_;
    free_memory((char*) current, 0, __FILE__, __LINE__);
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

MemoryAccountantAllocationNode* MemoryAccountant::createNewAccountantAllocationNode(size_t size, MemoryAccountantAllocationNode* next) const
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

void MemoryAccountant::destroyAccountantAllocationNode(MemoryAccountantAllocationNode* node) const
{
    allocator_->free_memory((char*) node, sizeof(node), __FILE__, __LINE__);
}

MemoryAccountant::MemoryAccountant()
    : head_(NULLPTR), allocator_(defaultMallocAllocator()), useCacheSizes_(false)
{
}

MemoryAccountant::~MemoryAccountant()
{
    clear();
}

void MemoryAccountant::createCacheSizeNodes(size_t sizes[], size_t length)
{
    for (size_t i = 0; i < length; i++)
        findOrCreateNodeOfSize(sizes[i]);

    if (head_ == NULLPTR)
        head_ = createNewAccountantAllocationNode(0, NULLPTR);
    else {
        for (MemoryAccountantAllocationNode* lastNode = head_; lastNode; lastNode = lastNode->next_) {
            if (lastNode->next_ == NULLPTR) {
                lastNode->next_ = createNewAccountantAllocationNode(0, NULLPTR);
                break;
            }
        }
    }
}


void MemoryAccountant::useCacheSizes(size_t sizes[], size_t length)
{
    if (head_)
      FAIL("MemoryAccountant: Cannot set cache sizes as allocations already occured!");

    createCacheSizeNodes(sizes, length);
    useCacheSizes_ = true;
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
    head_ = NULLPTR;
}

MemoryAccountantAllocationNode* MemoryAccountant::findNodeOfSize(size_t size) const
{
    if (useCacheSizes_) {
        for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_) {
            if (size > node->size_ && node->next_ == NULLPTR)
                return node;
            else if (size <= node->size_ && !(node->next_->size_ != 0 && node->next_->size_ <= size))
                return node;
        }
    }
    else
        for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_)
            if (node->size_ == size)
                return node;
    return NULLPTR;
}

MemoryAccountantAllocationNode* MemoryAccountant::findOrCreateNodeOfSize(size_t size)
{
    if (useCacheSizes_)
      return findNodeOfSize(size);

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
    if (node->currentAllocations_)
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
    size_t theTotalAllocations = 0;

    for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_)
        theTotalAllocations += node->allocations_;

    return theTotalAllocations;
}

size_t MemoryAccountant::totalDeallocations() const
{
    size_t theTotalDeallocations = 0;

    for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_)
        theTotalDeallocations += node->deallocations_;

    return theTotalDeallocations;
}

SimpleString MemoryAccountant::reportNoAllocations() const
{
      return SimpleString("CppUTest Memory Accountant has not noticed any allocations or deallocations. Sorry\n");
}

SimpleString MemoryAccountant::reportTitle() const
{
      if (useCacheSizes_)
        return "CppUTest Memory Accountant report (with cache sizes):\n";
      return "CppUTest Memory Accountant report:\n";
}

SimpleString MemoryAccountant::reportHeader() const
{
    if (useCacheSizes_)
        return "Cache size          # allocations    # deallocations   max # allocations at one time\n";
    return "Allocation size     # allocations    # deallocations   max # allocations at one time\n";
}

#define MEMORY_ACCOUNTANT_ROW_FORMAT "%s               %5d            %5d             %5d\n"

SimpleString MemoryAccountant::reportFooter() const
{
    return SimpleString("   Thank you for your business\n");
}

SimpleString MemoryAccountant::stringSize(size_t size) const
{
    return (size == 0) ? "other" : StringFromFormat("%5d", (int) size);
}

SimpleString MemoryAccountant::report() const
{
    if (head_ == NULLPTR)
      return reportNoAllocations();

    SimpleString accountantReport = reportTitle() + reportHeader();

    for (MemoryAccountantAllocationNode* node = head_; node; node = node->next_)
        accountantReport += StringFromFormat(MEMORY_ACCOUNTANT_ROW_FORMAT, stringSize(node->size_).asCharString(), (int) node->allocations_, (int) node->deallocations_, (int) node->maxAllocations_);

    return accountantReport + reportFooter();
}

AccountingTestMemoryAllocator::AccountingTestMemoryAllocator(MemoryAccountant& accountant, TestMemoryAllocator* origAllocator)
    : accountant_(accountant), originalAllocator_(origAllocator), head_(NULLPTR)
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
    originalAllocator_->free_memory((char*) foundNode, size, __FILE__, __LINE__);
    return size;
}

size_t AccountingTestMemoryAllocator::removeHeadAndReturnSize()
{
    AccountingTestMemoryAllocatorMemoryNode* foundNode = head_;
    head_ = head_->next_;

    size_t size = foundNode->size_;
    originalAllocator_->free_memory((char*) foundNode, size, __FILE__, __LINE__);
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

char* AccountingTestMemoryAllocator::alloc_memory(size_t size, const char* file, size_t line)
{
    accountant_.alloc(size);
    char* memory = originalAllocator_->alloc_memory(size, file, line);
    addMemoryToMemoryTrackingToKeepTrackOfSize(memory, size);
    return memory;
}

void AccountingTestMemoryAllocator::free_memory(char* memory, size_t, const char* file, size_t line)
{
    size_t size = removeMemoryFromTrackingAndReturnAllocatedSize(memory);
    accountant_.dealloc(size);
    originalAllocator_->free_memory(memory, size, file, line);
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

void GlobalMemoryAccountant::useCacheSizes(size_t sizes[], size_t length)
{
    accountant_.useCacheSizes(sizes, length);
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

