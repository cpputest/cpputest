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
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
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
#include "CppUTest/MemoryLeakDetector.h"
#include "CppUTest/TestMemoryAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/SimpleMutex.h"

static const char* UNKNOWN = "<unknown>";

static const char GuardBytes[] = {'B','A','S'};

SimpleStringBuffer::SimpleStringBuffer() :
    positions_filled_(0), write_limit_(SIMPLE_STRING_BUFFER_LEN-1)
{
    buffer_[0] = '\0';
}

void SimpleStringBuffer::clear()
{
    positions_filled_ = 0;
    buffer_[0] = '\0';
}

void SimpleStringBuffer::add(const char* format, ...)
{
    const size_t positions_left = write_limit_ - positions_filled_;
    if (positions_left == 0) return;

    va_list arguments;
    va_start(arguments, format);
    const int count = PlatformSpecificVSNprintf(buffer_ + positions_filled_, positions_left+1, format, arguments);
    if (count > 0) positions_filled_ += (size_t) count;
    if (positions_filled_ > write_limit_) positions_filled_ = write_limit_;
    va_end(arguments);
}

void SimpleStringBuffer::addMemoryDump(const void* memory, size_t memorySize)
{
    const unsigned char* byteMemory = (const unsigned char*)memory;
    const size_t maxLineBytes = 16;
    size_t currentPos = 0;
    size_t p;

    while (currentPos < memorySize) {
        add("    %04lx: ", (unsigned long) currentPos);
        size_t bytesInLine = memorySize - currentPos;
        if (bytesInLine > maxLineBytes) {
            bytesInLine = maxLineBytes;
        }
        const size_t leftoverBytes = maxLineBytes - bytesInLine;

        for (p = 0; p < bytesInLine; p++) {
            add("%02hx ", (unsigned short) byteMemory[currentPos + p]);
            if (p == ((maxLineBytes / 2) - 1)) {
                add(" ");
            }
        }
        for (p = 0; p < leftoverBytes; p++) {
            add("   ");
        }
        if (leftoverBytes > (maxLineBytes/2)) {
            add(" ");
        }

        add("|");
        for (p = 0; p < bytesInLine; p++) {
            char toAdd = (char)byteMemory[currentPos + p];
            if (toAdd < ' ' || toAdd > '~') {
                toAdd = '.';
            }
            add("%c", (int)toAdd);
        }
        add("|\n");
        currentPos += bytesInLine;
    }
}

char* SimpleStringBuffer::toString()
{
    return buffer_;
}

void SimpleStringBuffer::setWriteLimit(size_t write_limit)
{
    write_limit_ = write_limit;
    if (write_limit_ > SIMPLE_STRING_BUFFER_LEN-1)
        write_limit_ = SIMPLE_STRING_BUFFER_LEN-1;
}
void SimpleStringBuffer::resetWriteLimit()
{
    write_limit_ = SIMPLE_STRING_BUFFER_LEN-1;
}

bool SimpleStringBuffer::reachedItsCapacity()
{
    return positions_filled_ >= write_limit_;
}

////////////////////////

#define MEM_LEAK_TOO_MUCH "\netc etc etc etc. !!!! Too many memory leaks to report. Bailing out\n"
#define MEM_LEAK_FOOTER "Total number of leaks: "
#define MEM_LEAK_ADDITION_MALLOC_WARNING "NOTE:\n" \
                                         "\tMemory leak reports about malloc and free can be caused by allocating using the cpputest version of malloc,\n" \
                                         "\tbut deallocate using the standard free.\n" \
                                         "\tIf this is the case, check whether your malloc/free replacements are working (#define malloc cpputest_malloc etc).\n"

MemoryLeakOutputStringBuffer::MemoryLeakOutputStringBuffer()
    : total_leaks_(0), giveWarningOnUsingMalloc_(false)
{
}

void MemoryLeakOutputStringBuffer::addAllocationLocation(const char* allocationFile, size_t allocationLineNumber, size_t allocationSize, TestMemoryAllocator* allocator)
{
    outputBuffer_.add("   allocated at file: %s line: %d size: %lu type: %s\n", allocationFile, (int) allocationLineNumber, (unsigned long) allocationSize, allocator->alloc_name());
}

void MemoryLeakOutputStringBuffer::addDeallocationLocation(const char* freeFile, size_t freeLineNumber, TestMemoryAllocator* allocator)
{
    outputBuffer_.add("   deallocated at file: %s line: %d type: %s\n", freeFile, (int) freeLineNumber, allocator->free_name());
}

void MemoryLeakOutputStringBuffer::addNoMemoryLeaksMessage()
{
    outputBuffer_.add("No memory leaks were detected.");
}

void MemoryLeakOutputStringBuffer::startMemoryLeakReporting()
{
    giveWarningOnUsingMalloc_ = false;
    total_leaks_ = 0;

    size_t memory_leak_normal_footer_size = sizeof(MEM_LEAK_FOOTER) + 10 + sizeof(MEM_LEAK_TOO_MUCH); /* the number of leaks */
    size_t memory_leak_foot_size_with_malloc_warning = memory_leak_normal_footer_size + sizeof(MEM_LEAK_ADDITION_MALLOC_WARNING);

    outputBuffer_.setWriteLimit(SimpleStringBuffer::SIMPLE_STRING_BUFFER_LEN - memory_leak_foot_size_with_malloc_warning);
}

void MemoryLeakOutputStringBuffer::reportMemoryLeak(MemoryLeakDetectorNode* leak)
{
    if (total_leaks_ == 0) {
        addMemoryLeakHeader();
    }

    total_leaks_++;
    outputBuffer_.add("Alloc num (%u) Leak size: %lu Allocated at: %s and line: %d. Type: \"%s\"\n\tMemory: <%p> Content:\n",
            leak->number_, (unsigned long) leak->size_, leak->file_, (int) leak->line_, leak->allocator_->alloc_name(), (void*) leak->memory_);
    outputBuffer_.addMemoryDump(leak->memory_, leak->size_);

    if (SimpleString::StrCmp(leak->allocator_->alloc_name(), (const char*) "malloc") == 0)
        giveWarningOnUsingMalloc_ = true;
}

void MemoryLeakOutputStringBuffer::stopMemoryLeakReporting()
{
    if (total_leaks_ == 0) {
        addNoMemoryLeaksMessage();
        return;
    }

    bool buffer_reached_its_capacity = outputBuffer_.reachedItsCapacity();
    outputBuffer_.resetWriteLimit();

    if (buffer_reached_its_capacity)
        addErrorMessageForTooMuchLeaks();

    addMemoryLeakFooter(total_leaks_);

    if (giveWarningOnUsingMalloc_)
        addWarningForUsingMalloc();

}

void MemoryLeakOutputStringBuffer::addMemoryLeakHeader()
{
    outputBuffer_.add("Memory leak(s) found.\n");
}

void MemoryLeakOutputStringBuffer::addErrorMessageForTooMuchLeaks()
{
    outputBuffer_.add(MEM_LEAK_TOO_MUCH);
}

void MemoryLeakOutputStringBuffer::addMemoryLeakFooter(size_t amountOfLeaks)
{
    outputBuffer_.add("%s %d\n", MEM_LEAK_FOOTER, (int) amountOfLeaks);
}

void MemoryLeakOutputStringBuffer::addWarningForUsingMalloc()
{
    outputBuffer_.add(MEM_LEAK_ADDITION_MALLOC_WARNING);
}

void MemoryLeakOutputStringBuffer::reportDeallocateNonAllocatedMemoryFailure(const char* freeFile, size_t freeLine, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter)
{
    reportFailure("Deallocating non-allocated memory\n", "<unknown>", 0, 0, NullUnknownAllocator::defaultAllocator(), freeFile, freeLine, freeAllocator, reporter);
}

void MemoryLeakOutputStringBuffer::reportAllocationDeallocationMismatchFailure(MemoryLeakDetectorNode* node, const char* freeFile, size_t freeLineNumber, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter)
{
    reportFailure("Allocation/deallocation type mismatch\n", node->file_, node->line_, node->size_, node->allocator_, freeFile, freeLineNumber, freeAllocator, reporter);
}

void MemoryLeakOutputStringBuffer::reportMemoryCorruptionFailure(MemoryLeakDetectorNode* node, const char* freeFile, size_t freeLineNumber, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter)
{
        reportFailure("Memory corruption (written out of bounds?)\n", node->file_, node->line_, node->size_, node->allocator_, freeFile, freeLineNumber, freeAllocator, reporter);
}

void MemoryLeakOutputStringBuffer::reportFailure(const char* message, const char* allocFile, size_t allocLine, size_t allocSize, TestMemoryAllocator* allocAllocator, const char* freeFile, size_t freeLine,
        TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter)
{
    outputBuffer_.add("%s", message);
    addAllocationLocation(allocFile, allocLine, allocSize, allocAllocator);
    addDeallocationLocation(freeFile, freeLine, freeAllocator);
    reporter->fail(toString());
}


char* MemoryLeakOutputStringBuffer::toString()
{
    return outputBuffer_.toString();
}

void MemoryLeakOutputStringBuffer::clear()
{
    outputBuffer_.clear();
}

////////////////////////

void MemoryLeakDetectorNode::init(char* memory, unsigned number, size_t size, TestMemoryAllocator* allocator, MemLeakPeriod period, unsigned char allocation_stage, const char* file, size_t line)
{
    number_ = number;
    memory_ = memory;
    size_ = size;
    allocator_ = allocator;
    period_ = period;
    allocation_stage_ = allocation_stage;
    file_ = file;
    line_ = line;
}

///////////////////////

bool MemoryLeakDetectorList::isInPeriod(MemoryLeakDetectorNode* node, MemLeakPeriod period)
{
    return period == mem_leak_period_all || node->period_ == period || (node->period_ != mem_leak_period_disabled && period == mem_leak_period_enabled);
}

bool MemoryLeakDetectorList::isInAllocationStage(MemoryLeakDetectorNode* node, unsigned char allocation_stage)
{
    return node->allocation_stage_ == allocation_stage;
}

void MemoryLeakDetectorList::clearAllAccounting(MemLeakPeriod period)
{
    MemoryLeakDetectorNode* cur = head_;
    MemoryLeakDetectorNode* prev = NULLPTR;

    while (cur) {
        if (isInPeriod(cur, period)) {
            if (prev) {
                prev->next_ = cur->next_;
                cur = prev;
            }
            else {
                head_ = cur->next_;
                cur = head_;
                continue;
            }
        }
        prev = cur;
        cur = cur->next_;
    }
}

void MemoryLeakDetectorList::addNewNode(MemoryLeakDetectorNode* node)
{
    node->next_ = head_;
    head_ = node;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::removeNode(char* memory)
{
    MemoryLeakDetectorNode* cur = head_;
    MemoryLeakDetectorNode* prev = NULLPTR;
    while (cur) {
        if (cur->memory_ == memory) {
            if (prev) {
                prev->next_ = cur->next_;
                return cur;
            }
            else {
                head_ = cur->next_;
                return cur;
            }
        }
        prev = cur;
        cur = cur->next_;
    }
    return NULLPTR;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::retrieveNode(char* memory)
{
  MemoryLeakDetectorNode* cur = head_;
  while (cur) {
    if (cur->memory_ == memory)
      return cur;
    cur = cur->next_;
  }
  return NULLPTR;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getLeakFrom(MemoryLeakDetectorNode* node, MemLeakPeriod period)
{
    for (MemoryLeakDetectorNode* cur = node; cur; cur = cur->next_)
        if (isInPeriod(cur, period)) return cur;
    return NULLPTR;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getLeakForAllocationStageFrom(MemoryLeakDetectorNode* node, unsigned char allocation_stage)
{
    for (MemoryLeakDetectorNode* cur = node; cur; cur = cur->next_)
        if (isInAllocationStage(cur, allocation_stage)) return cur;
    return NULLPTR;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getFirstLeak(MemLeakPeriod period)
{
    return getLeakFrom(head_, period);
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getFirstLeakForAllocationStage(unsigned char allocation_stage)
{
    return getLeakForAllocationStageFrom(head_, allocation_stage);
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getNextLeak(MemoryLeakDetectorNode* node, MemLeakPeriod period)
{
    return getLeakFrom(node->next_, period);
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getNextLeakForAllocationStage(MemoryLeakDetectorNode* node, unsigned char allocation_stage)
{
    return getLeakForAllocationStageFrom(node->next_, allocation_stage);
}



size_t MemoryLeakDetectorList::getTotalLeaks(MemLeakPeriod period)
{
    size_t total_leaks = 0;
    for (MemoryLeakDetectorNode* node = head_; node; node = node->next_) {
        if (isInPeriod(node, period)) total_leaks++;
    }
    return total_leaks;
}

/////////////////////////////////////////////////////////////

unsigned long MemoryLeakDetectorTable::hash(char* memory)
{
    return (unsigned long)((size_t)memory % hash_prime);
}

void MemoryLeakDetectorTable::clearAllAccounting(MemLeakPeriod period)
{
    for (int i = 0; i < hash_prime; i++)
        table_[i].clearAllAccounting(period);
}

void MemoryLeakDetectorTable::addNewNode(MemoryLeakDetectorNode* node)
{
    table_[hash(node->memory_)].addNewNode(node);
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::removeNode(char* memory)
{
    return table_[hash(memory)].removeNode(memory);
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::retrieveNode(char* memory)
{
  return table_[hash(memory)].retrieveNode(memory);
}

size_t MemoryLeakDetectorTable::getTotalLeaks(MemLeakPeriod period)
{
    size_t total_leaks = 0;
    for (int i = 0; i < hash_prime; i++)
        total_leaks += table_[i].getTotalLeaks(period);
    return total_leaks;
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::getFirstLeak(MemLeakPeriod period)
{
    for (int i = 0; i < hash_prime; i++) {
        MemoryLeakDetectorNode* node = table_[i].getFirstLeak(period);
        if (node) return node;
    }
    return NULLPTR;
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::getFirstLeakForAllocationStage(unsigned char allocation_stage)
{
    for (int i = 0; i < hash_prime; i++) {
        MemoryLeakDetectorNode* node = table_[i].getFirstLeakForAllocationStage(allocation_stage);
        if (node) return node;
    }
    return NULLPTR;
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::getNextLeak(MemoryLeakDetectorNode* leak, MemLeakPeriod period)
{
    unsigned long i = hash(leak->memory_);
    MemoryLeakDetectorNode* node = table_[i].getNextLeak(leak, period);
    if (node) return node;

    for (++i; i < hash_prime; i++) {
        node = table_[i].getFirstLeak(period);
        if (node) return node;
    }
    return NULLPTR;
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::getNextLeakForAllocationStage(MemoryLeakDetectorNode* leak, unsigned char allocation_stage)
{
    unsigned long i = hash(leak->memory_);
    MemoryLeakDetectorNode* node = table_[i].getNextLeakForAllocationStage(leak, allocation_stage);
    if (node) return node;

    for (++i; i < hash_prime; i++) {
        node = table_[i].getFirstLeakForAllocationStage(allocation_stage);
        if (node) return node;
    }
    return NULLPTR;
}

/////////////////////////////////////////////////////////////

MemoryLeakDetector::MemoryLeakDetector(MemoryLeakFailure* reporter)
{
    doAllocationTypeChecking_ = true;
    allocationSequenceNumber_ = 1;
    current_period_ = mem_leak_period_disabled;
    current_allocation_stage_ = 0;
    reporter_ = reporter;
    mutex_ = new SimpleMutex;
}

MemoryLeakDetector::~MemoryLeakDetector()
{
    if (mutex_)
    {
        delete mutex_;
    }
}

void MemoryLeakDetector::clearAllAccounting(MemLeakPeriod period)
{
    memoryTable_.clearAllAccounting(period);
}

void MemoryLeakDetector::startChecking()
{
    outputBuffer_.clear();
    current_period_ = mem_leak_period_checking;
}

void MemoryLeakDetector::stopChecking()
{
    current_period_ = mem_leak_period_enabled;
}

unsigned char MemoryLeakDetector::getCurrentAllocationStage() const
{
    return current_allocation_stage_;
}

void MemoryLeakDetector::enable()
{
    current_period_ = mem_leak_period_enabled;
}

void MemoryLeakDetector::disable()
{
    current_period_ = mem_leak_period_disabled;
}

void MemoryLeakDetector::disableAllocationTypeChecking()
{
    doAllocationTypeChecking_ = false;
}

void MemoryLeakDetector::enableAllocationTypeChecking()
{
    doAllocationTypeChecking_ = true;
}

unsigned MemoryLeakDetector::getCurrentAllocationNumber()
{
    return allocationSequenceNumber_;
}

void MemoryLeakDetector::increaseAllocationStage()
{
    current_allocation_stage_++;
}

void MemoryLeakDetector::decreaseAllocationStage()
{
    current_allocation_stage_--;
}

SimpleMutex *MemoryLeakDetector::getMutex()
{
    return mutex_;
}

static size_t calculateVoidPointerAlignedSize(size_t size)
{
#ifndef CPPUTEST_DISABLE_MEM_CORRUPTION_CHECK
    return (sizeof(void*) - (size % sizeof(void*))) + size;
#else
   return size;
#endif
}

size_t MemoryLeakDetector::sizeOfMemoryWithCorruptionInfo(size_t size)
{
    return calculateVoidPointerAlignedSize(size + memory_corruption_buffer_size);
}

MemoryLeakDetectorNode* MemoryLeakDetector::getNodeFromMemoryPointer(char* memory, size_t memory_size)
{
    return (MemoryLeakDetectorNode*) (void*) (memory + sizeOfMemoryWithCorruptionInfo(memory_size));
}

void MemoryLeakDetector::storeLeakInformation(MemoryLeakDetectorNode * node, char *new_memory, size_t size, TestMemoryAllocator *allocator, const char *file, size_t line)
{
    node->init(new_memory, allocationSequenceNumber_++, size, allocator, current_period_, current_allocation_stage_, file, line);
    addMemoryCorruptionInformation(node->memory_ + node->size_);
    memoryTable_.addNewNode(node);
}

char* MemoryLeakDetector::reallocateMemoryAndLeakInformation(TestMemoryAllocator* allocator, char* memory, size_t size, const char* file, size_t line, bool allocatNodesSeperately)
{
    char* new_memory = reallocateMemoryWithAccountingInformation(allocator, memory, size, file, line, allocatNodesSeperately);
    if (new_memory == NULLPTR) return NULLPTR;

    MemoryLeakDetectorNode *node = createMemoryLeakAccountingInformation(allocator, size, new_memory, allocatNodesSeperately);
    storeLeakInformation(node, new_memory, size, allocator, file, line);
    return node->memory_;
}

void MemoryLeakDetector::invalidateMemory(char* memory)
{
#ifndef CPPUTEST_DISABLE_HEAP_POISON
  MemoryLeakDetectorNode* node = memoryTable_.retrieveNode(memory);
  if (node)
    PlatformSpecificMemset(memory, 0xCD, node->size_);
#endif
}

void MemoryLeakDetector::addMemoryCorruptionInformation(char* memory)
{
   for (size_t i=0; i<memory_corruption_buffer_size; i++)
      memory[i] = GuardBytes[i % sizeof(GuardBytes)];
}

bool MemoryLeakDetector::validMemoryCorruptionInformation(char* memory)
{
   for (size_t i=0; i<memory_corruption_buffer_size; i++)
      if (memory[i] != GuardBytes[i % sizeof(GuardBytes)])
          return false;
   return true;
}

bool MemoryLeakDetector::matchingAllocation(TestMemoryAllocator *alloc_allocator, TestMemoryAllocator *free_allocator)
{
    if (alloc_allocator == free_allocator) return true;
    if (!doAllocationTypeChecking_) return true;
    return free_allocator->isOfEqualType(alloc_allocator);
}

void MemoryLeakDetector::checkForCorruption(MemoryLeakDetectorNode* node, const char* file, size_t line, TestMemoryAllocator* allocator, bool allocateNodesSeperately)
{
    if (!matchingAllocation(node->allocator_->actualAllocator(), allocator->actualAllocator()))
        outputBuffer_.reportAllocationDeallocationMismatchFailure(node, file, line, allocator->actualAllocator(), reporter_);
    else if (!validMemoryCorruptionInformation(node->memory_ + node->size_))
        outputBuffer_.reportMemoryCorruptionFailure(node, file, line, allocator->actualAllocator(), reporter_);
    else if (allocateNodesSeperately)
        allocator->freeMemoryLeakNode((char*) node);
}

char* MemoryLeakDetector::allocMemory(TestMemoryAllocator* allocator, size_t size, bool allocatNodesSeperately)
{
    return allocMemory(allocator, size, UNKNOWN, 0, allocatNodesSeperately);
}

char* MemoryLeakDetector::allocateMemoryWithAccountingInformation(TestMemoryAllocator* allocator, size_t size, const char* file, size_t line, bool allocatNodesSeperately)
{
    if (allocatNodesSeperately) return allocator->alloc_memory(sizeOfMemoryWithCorruptionInfo(size), file, line);
    else return allocator->alloc_memory(sizeOfMemoryWithCorruptionInfo(size) + sizeof(MemoryLeakDetectorNode), file, line);
}

char* MemoryLeakDetector::reallocateMemoryWithAccountingInformation(TestMemoryAllocator* /*allocator*/, char* memory, size_t size, const char* /*file*/, size_t /*line*/, bool allocatNodesSeperately)
{
    if (allocatNodesSeperately) return (char*) PlatformSpecificRealloc(memory, sizeOfMemoryWithCorruptionInfo(size));
    else return (char*) PlatformSpecificRealloc(memory, sizeOfMemoryWithCorruptionInfo(size) + sizeof(MemoryLeakDetectorNode));
}

MemoryLeakDetectorNode* MemoryLeakDetector::createMemoryLeakAccountingInformation(TestMemoryAllocator* allocator, size_t size, char* memory, bool allocatNodesSeperately)
{
    if (allocatNodesSeperately) return (MemoryLeakDetectorNode*) (void*) allocator->allocMemoryLeakNode(sizeof(MemoryLeakDetectorNode));
    else return getNodeFromMemoryPointer(memory, size);
}

char* MemoryLeakDetector::allocMemory(TestMemoryAllocator* allocator, size_t size, const char* file, size_t line, bool allocatNodesSeperately)
{
#ifdef CPPUTEST_DISABLE_MEM_CORRUPTION_CHECK
   allocatNodesSeperately = true;
#endif
    /* With malloc, it is harder to guarantee that the allocator free is called.
     * This is because operator new is overloaded via linker symbols, but malloc just via #defines.
     * If the same allocation is used and the wrong free is called, it will deallocate the memory leak information
     * without the memory leak detector ever noticing it!
     * So, for malloc, we'll allocate the memory separately so we can detect this and give a proper error.
     */

    char* memory = allocateMemoryWithAccountingInformation(allocator, size, file, line, allocatNodesSeperately);
    if (memory == NULLPTR) return NULLPTR;
    MemoryLeakDetectorNode* node = createMemoryLeakAccountingInformation(allocator, size, memory, allocatNodesSeperately);

    storeLeakInformation(node, memory, size, allocator, file, line);
    return node->memory_;
}

void MemoryLeakDetector::removeMemoryLeakInformationWithoutCheckingOrDeallocatingTheMemoryButDeallocatingTheAccountInformation(TestMemoryAllocator* allocator, void* memory, bool allocatNodesSeperately)
{
    MemoryLeakDetectorNode* node = memoryTable_.removeNode((char*) memory);
    if (allocatNodesSeperately) allocator->freeMemoryLeakNode( (char*) node);
}

void MemoryLeakDetector::deallocMemory(TestMemoryAllocator* allocator, void* memory, const char* file, size_t line, bool allocatNodesSeperately)
{
    if (memory == NULLPTR) return;

    MemoryLeakDetectorNode* node = memoryTable_.removeNode((char*) memory);
    if (node == NULLPTR) {
        outputBuffer_.reportDeallocateNonAllocatedMemoryFailure(file, line, allocator, reporter_);
        return;
    }
#ifdef CPPUTEST_DISABLE_MEM_CORRUPTION_CHECK
   allocatNodesSeperately = true;
#endif
    if (!allocator->hasBeenDestroyed()) {
        size_t size = node->size_;
        checkForCorruption(node, file, line, allocator, allocatNodesSeperately);
        allocator->free_memory((char*) memory, size, file, line);
    }
}

void MemoryLeakDetector::deallocMemory(TestMemoryAllocator* allocator, void* memory, bool allocatNodesSeperately)
{
    deallocMemory(allocator, (char*) memory, UNKNOWN, 0, allocatNodesSeperately);
}

void MemoryLeakDetector::deallocAllMemoryInCurrentAllocationStage()
{
    char* memory = NULLPTR;
    MemoryLeakDetectorNode* node = memoryTable_.getFirstLeakForAllocationStage(current_allocation_stage_);
    while (node) {
        memory = node->memory_;
        TestMemoryAllocator* allocator = node->allocator_;
        node = memoryTable_.getNextLeakForAllocationStage(node, current_allocation_stage_);
        deallocMemory(allocator, memory, __FILE__, __LINE__);
    }
}

char* MemoryLeakDetector::reallocMemory(TestMemoryAllocator* allocator, char* memory, size_t size, const char* file, size_t line, bool allocatNodesSeperately)
{
#ifdef CPPUTEST_DISABLE_MEM_CORRUPTION_CHECK
   allocatNodesSeperately = true;
#endif
    if (memory) {
        MemoryLeakDetectorNode* node = memoryTable_.removeNode(memory);
        if (node == NULLPTR) {
            outputBuffer_.reportDeallocateNonAllocatedMemoryFailure(file, line, allocator, reporter_);
            return NULLPTR;
        }
        checkForCorruption(node, file, line, allocator, allocatNodesSeperately);
    }
    return reallocateMemoryAndLeakInformation(allocator, memory, size, file, line, allocatNodesSeperately);
}

void MemoryLeakDetector::ConstructMemoryLeakReport(MemLeakPeriod period)
{
    MemoryLeakDetectorNode* leak = memoryTable_.getFirstLeak(period);

    outputBuffer_.startMemoryLeakReporting();

    while (leak) {
        outputBuffer_.reportMemoryLeak(leak);
        leak = memoryTable_.getNextLeak(leak, period);
    }

    outputBuffer_.stopMemoryLeakReporting();
}

const char* MemoryLeakDetector::report(MemLeakPeriod period)
{
    ConstructMemoryLeakReport(period);

    return outputBuffer_.toString();
}

void MemoryLeakDetector::markCheckingPeriodLeaksAsNonCheckingPeriod()
{
    MemoryLeakDetectorNode* leak = memoryTable_.getFirstLeak(mem_leak_period_checking);
    while (leak) {
        if (leak->period_ == mem_leak_period_checking) leak->period_ = mem_leak_period_enabled;
        leak = memoryTable_.getNextLeak(leak, mem_leak_period_checking);
    }
}

size_t MemoryLeakDetector::totalMemoryLeaks(MemLeakPeriod period)
{
    return memoryTable_.getTotalLeaks(period);
}
