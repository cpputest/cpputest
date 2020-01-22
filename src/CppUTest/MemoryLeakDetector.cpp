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
#include "CppUTest/MemoryLeakDetector.h"
#include "CppUTest/TestMemoryAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/SimpleMutex.h"

#if CPPUTEST_GNU_CALLSTACK_SUPPORTED

#include <execinfo.h>
#include <cxxabi.h>
#include <stdio.h>

SimpleString getCallerInfo(const char* file, int line, void *caller);

#endif

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

void MemoryLeakOutputStringBuffer::addAllocationLocation(const char* allocationFile, int allocationLineNumber, size_t allocationSize, TestMemoryAllocator* allocator)
{
    outputBuffer_.add("   allocated at file: %s line: %d size: %lu type: %s\n", allocationFile, allocationLineNumber, (unsigned long) allocationSize, allocator->alloc_name());
}

void MemoryLeakOutputStringBuffer::addDeallocationLocation(const char* freeFile, int freeLineNumber, TestMemoryAllocator* allocator)
{
    outputBuffer_.add("   deallocated at file: %s line: %d type: %s\n", freeFile, freeLineNumber, allocator->free_name());
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

#if CPPUTEST_GNU_CALLSTACK_SUPPORTED

// TODO: Move this function to a new source file
static SimpleString demangle_symbol(const SimpleString &mangled_symbol)
{
    SimpleString result;

    int status;
    char* demangled_symbol = abi::__cxa_demangle( mangled_symbol.asCharString(), NULLPTR, NULLPTR, &status );
    if (demangled_symbol != NULLPTR) {
        if (status == 0) {
            result = demangled_symbol;
        }
        PlatformSpecificFree(demangled_symbol);
    }

    if (result.isEmpty()) {
        result = mangled_symbol;
    }

    return result;
}

// TODO: Move this function to a new source file
static SimpleString getAddress2Line(const SimpleString &filename, void *address)
{
    SimpleString cmd_result;

    SimpleString cmd_line = StringFromFormat("addr2line -e %s %p", filename.asCharString(), address);

    FILE* cmd_pipe = popen(cmd_line.asCharString(), "r");
    if (cmd_pipe) {
        char buffer[256];

        while (!feof(cmd_pipe)) {
            if (fgets(buffer, sizeof(buffer), cmd_pipe) != NULLPTR) {
                cmd_result += buffer;
            }
        }

        pclose(cmd_pipe);
    }

    return cmd_result;
}

// TODO: Move this function to a new source file
static SimpleString getAddressInfo(const SimpleString &filename, void *address)
{
    SimpleString result;

    SimpleString line_info = getAddress2Line(filename, address);

    size_t colon_pos = line_info.find(':');
    if (colon_pos != SimpleString::npos) {
        SimpleString source_file = line_info.subString(0, colon_pos);
        SimpleString line_num_str = line_info.subString(colon_pos + 1);
        int line_num = SimpleString::AtoI(line_num_str.asCharString());

        static const SimpleString UNKNOWN_SOURCE_FILE = "??";
        if (source_file != UNKNOWN_SOURCE_FILE) {
            result = StringFromFormat("Source '%s'<Line:%d>, ", source_file.asCharString(), line_num);
        }
    }

    return result;
}

// TODO: Move this function to a new source file
static bool parseSymbolInfo(const char* symbol_info, SimpleString &caller_info, SimpleString &binary_module)
{
    SimpleString buffer(symbol_info);

    size_t first_parenthesis_pos = buffer.find('(');
    size_t last_parenthesis_pos = buffer.rfind(')');
    if ((first_parenthesis_pos == SimpleString::npos) || (last_parenthesis_pos == SimpleString::npos)) {
        return false;
    }

    size_t plus_pos = buffer.rfindFrom(last_parenthesis_pos-1, '+');
    if ((plus_pos == SimpleString::npos) || (first_parenthesis_pos >= plus_pos)) {
        return false;
    }

    binary_module = buffer.subString(0, first_parenthesis_pos);

    SimpleString mangled_function_name = buffer.subString((first_parenthesis_pos + 1), (plus_pos - first_parenthesis_pos - 1));
    SimpleString demangled_function_name = demangle_symbol(mangled_function_name);

    caller_info = demangled_function_name + buffer.subString(plus_pos, (last_parenthesis_pos - plus_pos));

    return true;
}

// TODO: Move this function to a new source file
SimpleString getCallerInfo(const char* file, int line, void *caller)
{
    SimpleString result;

    if (caller != NULLPTR) {
        char** symbol_info = backtrace_symbols(&caller, 1);

        if (symbol_info != NULLPTR) {
            SimpleString caller_info, binary_module;
            if (parseSymbolInfo(symbol_info[0], caller_info, binary_module)) {
                result = StringFromFormat("Function %s, ", caller_info.asCharString());
                result += getAddressInfo(binary_module, caller);
                result += StringFromFormat("Binary '%s'", binary_module.asCharString());
            } else {
                result = StringFromFormat("Function %s", symbol_info[0]);
            }
            PlatformSpecificFree(symbol_info);
        } else {
            result = StringFromFormat("Unknown function (Address:%p), ", caller);
        }
    } else {
        result = StringFromFormat("Source '%s'<Line:%d>", file, line);
    }

    return result;
}

static SimpleString getCallerInfo(MemoryLeakDetectorNode *leak)
{
    return getCallerInfo(leak->file_, leak->line_, leak->caller_);
}

#endif

void MemoryLeakOutputStringBuffer::reportMemoryLeak(MemoryLeakDetectorNode* leak)
{
    if (total_leaks_ == 0) {
        addMemoryLeakHeader();
    }

    total_leaks_++;
#if CPPUTEST_GNU_CALLSTACK_SUPPORTED
    outputBuffer_.add("Alloc num (%u) Leak size: %lu Allocated from: %s. Type: \"%s\"\n\tMemory: <%p> Content:\n",
            leak->number_, (unsigned long) leak->size_, getCallerInfo(leak).asCharString(), leak->allocator_->alloc_name(), (void*) leak->memory_);
#else
    outputBuffer_.add("Alloc num (%u) Leak size: %lu Allocated from: Source '%s'<Line:%d>. Type: \"%s\"\n\tMemory: <%p> Content:\n",
            leak->number_, (unsigned long) leak->size_, leak->file_, leak->line_, leak->allocator_->alloc_name(), (void*) leak->memory_);
#endif
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

void MemoryLeakOutputStringBuffer::addMemoryLeakFooter(int amountOfLeaks)
{
    outputBuffer_.add("%s %d\n", MEM_LEAK_FOOTER, amountOfLeaks);
}

void MemoryLeakOutputStringBuffer::addWarningForUsingMalloc()
{
    outputBuffer_.add(MEM_LEAK_ADDITION_MALLOC_WARNING);
}

void MemoryLeakOutputStringBuffer::reportDeallocateNonAllocatedMemoryFailure(const char* freeFile, int freeLine, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter)
{
    reportFailure("Deallocating non-allocated memory\n", "<unknown>", 0, 0, NullUnknownAllocator::defaultAllocator(), freeFile, freeLine, freeAllocator, reporter);
}

void MemoryLeakOutputStringBuffer::reportAllocationDeallocationMismatchFailure(MemoryLeakDetectorNode* node, const char* freeFile, int freeLineNumber, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter)
{
    reportFailure("Allocation/deallocation type mismatch\n", node->file_, node->line_, node->size_, node->allocator_, freeFile, freeLineNumber, freeAllocator, reporter);
}

void MemoryLeakOutputStringBuffer::reportMemoryCorruptionFailure(MemoryLeakDetectorNode* node, const char* freeFile, int freeLineNumber, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter)
{
        reportFailure("Memory corruption (written out of bounds?)\n", node->file_, node->line_, node->size_, node->allocator_, freeFile, freeLineNumber, freeAllocator, reporter);
}

void MemoryLeakOutputStringBuffer::reportFailure(const char* message, const char* allocFile, int allocLine, size_t allocSize, TestMemoryAllocator* allocAllocator, const char* freeFile, int freeLine,
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

void MemoryLeakDetectorNode::init(char* memory, unsigned number, size_t size, TestMemoryAllocator* allocator, MemLeakPeriod period, const char* file, int line, void *caller)
{
    number_ = number;
    memory_ = memory;
    size_ = size;
    allocator_ = allocator;
    period_ = period;
    file_ = file;
    line_ = line;
    caller_ = caller;
}

///////////////////////

bool MemoryLeakDetectorList::isInPeriod(MemoryLeakDetectorNode* node, MemLeakPeriod period)
{
    return period == mem_leak_period_all || node->period_ == period || (node->period_ != mem_leak_period_disabled && period == mem_leak_period_enabled);
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

MemoryLeakDetectorNode* MemoryLeakDetectorList::getFirstLeak(MemLeakPeriod period)
{
    return getLeakFrom(head_, period);
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getNextLeak(MemoryLeakDetectorNode* node, MemLeakPeriod period)
{
    return getLeakFrom(node->next_, period);
}

int MemoryLeakDetectorList::getTotalLeaks(MemLeakPeriod period)
{
    int total_leaks = 0;
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

int MemoryLeakDetectorTable::getTotalLeaks(MemLeakPeriod period)
{
    int total_leaks = 0;
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

/////////////////////////////////////////////////////////////

MemoryLeakDetector::MemoryLeakDetector(MemoryLeakFailure* reporter)
{
    doAllocationTypeChecking_ = true;
    allocationSequenceNumber_ = 1;
    current_period_ = mem_leak_period_disabled;
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

void MemoryLeakDetector::storeLeakInformation(MemoryLeakDetectorNode * node, char *new_memory, size_t size, TestMemoryAllocator *allocator, const char *file, int line, void *caller)
{
    node->init(new_memory, allocationSequenceNumber_++, size, allocator, current_period_, file, line, caller);
    addMemoryCorruptionInformation(node->memory_ + node->size_);
    memoryTable_.addNewNode(node);
}

char* MemoryLeakDetector::reallocateMemoryAndLeakInformation(TestMemoryAllocator* allocator, char* memory, size_t size, const char* file, int line, void *caller, bool allocatNodesSeperately)
{
    char* new_memory = reallocateMemoryWithAccountingInformation(allocator, memory, size, file, line, caller, allocatNodesSeperately);
    if (new_memory == NULLPTR) return NULLPTR;

    MemoryLeakDetectorNode *node = createMemoryLeakAccountingInformation(allocator, size, new_memory, allocatNodesSeperately);
    storeLeakInformation(node, new_memory, size, allocator, file, line, caller);
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

void MemoryLeakDetector::checkForCorruption(MemoryLeakDetectorNode* node, const char* file, int line, TestMemoryAllocator* allocator, bool allocateNodesSeperately)
{
    if (!matchingAllocation(node->allocator_, allocator))
        outputBuffer_.reportAllocationDeallocationMismatchFailure(node, file, line, allocator, reporter_);
    else if (!validMemoryCorruptionInformation(node->memory_ + node->size_))
        outputBuffer_.reportMemoryCorruptionFailure(node, file, line, allocator, reporter_);
    else if (allocateNodesSeperately)
        allocator->freeMemoryLeakNode((char*) node);
}

char* MemoryLeakDetector::allocMemory(TestMemoryAllocator* allocator, size_t size, bool allocatNodesSeperately)
{
    return allocMemory(allocator, size, UNKNOWN, 0, NULLPTR, allocatNodesSeperately);
}

char* MemoryLeakDetector::allocateMemoryWithAccountingInformation(TestMemoryAllocator* allocator, size_t size, const char* file, int line, void *caller, bool allocatNodesSeperately)
{
    if (allocatNodesSeperately) return allocator->alloc_memory(sizeOfMemoryWithCorruptionInfo(size), file, line, caller);
    else return allocator->alloc_memory(sizeOfMemoryWithCorruptionInfo(size) + sizeof(MemoryLeakDetectorNode), file, line, caller);
}

char* MemoryLeakDetector::reallocateMemoryWithAccountingInformation(TestMemoryAllocator* /*allocator*/, char* memory, size_t size, const char* /*file*/, int /*line*/, void * /*caller*/, bool allocatNodesSeperately)
{
    if (allocatNodesSeperately) return (char*) PlatformSpecificRealloc(memory, sizeOfMemoryWithCorruptionInfo(size));
    else return (char*) PlatformSpecificRealloc(memory, sizeOfMemoryWithCorruptionInfo(size) + sizeof(MemoryLeakDetectorNode));
}

MemoryLeakDetectorNode* MemoryLeakDetector::createMemoryLeakAccountingInformation(TestMemoryAllocator* allocator, size_t size, char* memory, bool allocatNodesSeperately)
{
    if (allocatNodesSeperately) return (MemoryLeakDetectorNode*) (void*) allocator->allocMemoryLeakNode(sizeof(MemoryLeakDetectorNode));
    else return getNodeFromMemoryPointer(memory, size);
}

char* MemoryLeakDetector::allocMemory(TestMemoryAllocator* allocator, size_t size, const char* file, int line, void *caller, bool allocatNodesSeperately)
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

    char* memory = allocateMemoryWithAccountingInformation(allocator, size, file, line, caller, allocatNodesSeperately);
    if (memory == NULLPTR) return NULLPTR;
    MemoryLeakDetectorNode* node = createMemoryLeakAccountingInformation(allocator, size, memory, allocatNodesSeperately);

    storeLeakInformation(node, memory, size, allocator, file, line, caller);
    return node->memory_;
}

void MemoryLeakDetector::removeMemoryLeakInformationWithoutCheckingOrDeallocatingTheMemoryButDeallocatingTheAccountInformation(TestMemoryAllocator* allocator, void* memory, bool allocatNodesSeperately)
{
    MemoryLeakDetectorNode* node = memoryTable_.removeNode((char*) memory);
    if (allocatNodesSeperately) allocator->freeMemoryLeakNode( (char*) node);
}

void MemoryLeakDetector::deallocMemory(TestMemoryAllocator* allocator, void* memory, const char* file, int line, void *caller, bool allocatNodesSeperately)
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
        checkForCorruption(node, file, line, allocator, allocatNodesSeperately);
        allocator->free_memory((char*) memory, file, line, caller);
    }
}

void MemoryLeakDetector::deallocMemory(TestMemoryAllocator* allocator, void* memory, bool allocatNodesSeperately)
{
    deallocMemory(allocator, (char*) memory, UNKNOWN, 0, NULLPTR, allocatNodesSeperately);
}

char* MemoryLeakDetector::reallocMemory(TestMemoryAllocator* allocator, char* memory, size_t size, const char* file, int line, void *caller, bool allocatNodesSeperately)
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
    return reallocateMemoryAndLeakInformation(allocator, memory, size, file, line, caller, allocatNodesSeperately);
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

int MemoryLeakDetector::totalMemoryLeaks(MemLeakPeriod period)
{
    return memoryTable_.getTotalLeaks(period);
}
