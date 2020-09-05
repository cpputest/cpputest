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

#ifndef D_MemoryLeakDetector_h
#define D_MemoryLeakDetector_h

enum MemLeakPeriod
{
    mem_leak_period_all,
    mem_leak_period_disabled,
    mem_leak_period_enabled,
    mem_leak_period_checking
};

class TestMemoryAllocator;
class SimpleMutex;

class MemoryLeakFailure
{
public:
    virtual ~MemoryLeakFailure()
    {
    }

    virtual void fail(char* fail_string)=0;
};

struct SimpleStringBuffer
{
    enum
    {
        SIMPLE_STRING_BUFFER_LEN = 4096
    };

    SimpleStringBuffer();
    void clear();
    void add(const char* format, ...) _check_format_(printf, 2, 3);
    void addMemoryDump(const void* memory, size_t memorySize);

    char* toString();

    void setWriteLimit(size_t write_limit);
    void resetWriteLimit();
    bool reachedItsCapacity();
private:
    char buffer_[SIMPLE_STRING_BUFFER_LEN];
    size_t positions_filled_;
    size_t write_limit_;
};

struct MemoryLeakDetectorNode;

class MemoryLeakOutputStringBuffer
{
public:
    MemoryLeakOutputStringBuffer();

    void clear();

    void startMemoryLeakReporting();
    void stopMemoryLeakReporting();

    void reportMemoryLeak(MemoryLeakDetectorNode* leak);

    void reportDeallocateNonAllocatedMemoryFailure(const char* freeFile, size_t freeLine, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter);
    void reportMemoryCorruptionFailure(MemoryLeakDetectorNode* node, const char* freeFile, size_t freeLineNumber, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter);
    void reportAllocationDeallocationMismatchFailure(MemoryLeakDetectorNode* node, const char* freeFile, size_t freeLineNumber, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter);
    char* toString();

private:
    void addAllocationLocation(const char* allocationFile, size_t allocationLineNumber, size_t allocationSize, TestMemoryAllocator* allocator);
    void addDeallocationLocation(const char* freeFile, size_t freeLineNumber, TestMemoryAllocator* allocator);

    void addMemoryLeakHeader();
    void addMemoryLeakFooter(size_t totalAmountOfLeaks);
    void addWarningForUsingMalloc();
    void addNoMemoryLeaksMessage();
    void addErrorMessageForTooMuchLeaks();

private:

    size_t total_leaks_;
    bool giveWarningOnUsingMalloc_;

    void reportFailure(const char* message, const char* allocFile,
            size_t allocLine, size_t allocSize,
            TestMemoryAllocator* allocAllocator, const char* freeFile,
            size_t freeLine, TestMemoryAllocator* freeAllocator, MemoryLeakFailure* reporter);

    SimpleStringBuffer outputBuffer_;
};

struct MemoryLeakDetectorNode
{
    MemoryLeakDetectorNode() :
        size_(0), number_(0), memory_(NULLPTR), file_(NULLPTR), line_(0), allocator_(NULLPTR), period_(mem_leak_period_enabled), allocation_stage_(0), next_(NULLPTR)
    {
    }

    void init(char* memory, unsigned number, size_t size, TestMemoryAllocator* allocator, MemLeakPeriod period, unsigned char allocation_stage, const char* file, size_t line);

    size_t size_;
    unsigned number_;
    char* memory_;
    const char* file_;
    size_t line_;
    TestMemoryAllocator* allocator_;
    MemLeakPeriod period_;
    unsigned char allocation_stage_;

private:
    friend struct MemoryLeakDetectorList;
    MemoryLeakDetectorNode* next_;
};

struct MemoryLeakDetectorList
{
    MemoryLeakDetectorList() :
        head_(NULLPTR)
    {}

    void addNewNode(MemoryLeakDetectorNode* node);
    MemoryLeakDetectorNode* retrieveNode(char* memory);
    MemoryLeakDetectorNode* removeNode(char* memory);

    MemoryLeakDetectorNode* getFirstLeak(MemLeakPeriod period);
    MemoryLeakDetectorNode* getFirstLeakForAllocationStage(unsigned char allocation_stage);

    MemoryLeakDetectorNode* getNextLeak(MemoryLeakDetectorNode* node, MemLeakPeriod period);
    MemoryLeakDetectorNode* getNextLeakForAllocationStage(MemoryLeakDetectorNode* node, unsigned char allocation_stage);

    MemoryLeakDetectorNode* getLeakFrom(MemoryLeakDetectorNode* node, MemLeakPeriod period);
    MemoryLeakDetectorNode* getLeakForAllocationStageFrom(MemoryLeakDetectorNode* node, unsigned char allocation_stage);

    size_t getTotalLeaks(MemLeakPeriod period);
    void clearAllAccounting(MemLeakPeriod period);

    bool isInPeriod(MemoryLeakDetectorNode* node, MemLeakPeriod period);
    bool isInAllocationStage(MemoryLeakDetectorNode* node, unsigned char allocation_stage);

private:
    MemoryLeakDetectorNode* head_;
};

struct MemoryLeakDetectorTable
{
    void clearAllAccounting(MemLeakPeriod period);

    void addNewNode(MemoryLeakDetectorNode* node);
    MemoryLeakDetectorNode* retrieveNode(char* memory);
    MemoryLeakDetectorNode* removeNode(char* memory);

    size_t getTotalLeaks(MemLeakPeriod period);

    MemoryLeakDetectorNode* getFirstLeak(MemLeakPeriod period);
    MemoryLeakDetectorNode* getFirstLeakForAllocationStage(unsigned char allocation_stage);
    MemoryLeakDetectorNode* getNextLeak(MemoryLeakDetectorNode* leak, MemLeakPeriod period);
    MemoryLeakDetectorNode* getNextLeakForAllocationStage(MemoryLeakDetectorNode* leak, unsigned char allocation_stage);

private:
    unsigned long hash(char* memory);

    enum
    {
        hash_prime = MEMORY_LEAK_HASH_TABLE_SIZE
    };
    MemoryLeakDetectorList table_[hash_prime];
};

class MemoryLeakDetector
{
public:
    MemoryLeakDetector(MemoryLeakFailure* reporter);
    virtual ~MemoryLeakDetector();

    void enable();
    void disable();

    void disableAllocationTypeChecking();
    void enableAllocationTypeChecking();

    void startChecking();
    void stopChecking();

    unsigned char getCurrentAllocationStage() const;
    void increaseAllocationStage();
    void decreaseAllocationStage();

    const char* report(MemLeakPeriod period);
    void markCheckingPeriodLeaksAsNonCheckingPeriod();
    size_t totalMemoryLeaks(MemLeakPeriod period);
    void clearAllAccounting(MemLeakPeriod period);

    char* allocMemory(TestMemoryAllocator* allocator, size_t size, bool allocatNodesSeperately = false);
    char* allocMemory(TestMemoryAllocator* allocator, size_t size,
            const char* file, size_t line, bool allocatNodesSeperately = false);
    void deallocMemory(TestMemoryAllocator* allocator, void* memory, bool allocatNodesSeperately = false);
    void deallocMemory(TestMemoryAllocator* allocator, void* memory, const char* file, size_t line, bool allocatNodesSeperately = false);
    void deallocAllMemoryInCurrentAllocationStage();
    char* reallocMemory(TestMemoryAllocator* allocator, char* memory, size_t size, const char* file, size_t line, bool allocatNodesSeperately = false);

    void invalidateMemory(char* memory);
    void removeMemoryLeakInformationWithoutCheckingOrDeallocatingTheMemoryButDeallocatingTheAccountInformation(TestMemoryAllocator* allocator, void* memory, bool allocatNodesSeperately);
    enum
    {
#ifdef CPPUTEST_DISABLE_MEM_CORRUPTION_CHECK
        memory_corruption_buffer_size = 0
#else
        memory_corruption_buffer_size = 3
#endif
    };

    unsigned getCurrentAllocationNumber();

    SimpleMutex* getMutex(void);
private:
    MemoryLeakFailure* reporter_;
    MemLeakPeriod current_period_;
    MemoryLeakOutputStringBuffer outputBuffer_;
    MemoryLeakDetectorTable memoryTable_;
    bool doAllocationTypeChecking_;
    unsigned allocationSequenceNumber_;
    unsigned char current_allocation_stage_;
    SimpleMutex* mutex_;

    char* allocateMemoryWithAccountingInformation(TestMemoryAllocator* allocator, size_t size, const char* file, size_t line, bool allocatNodesSeperately);
    char* reallocateMemoryWithAccountingInformation(TestMemoryAllocator* allocator, char* memory, size_t size, const char* file, size_t line, bool allocatNodesSeperately);
    MemoryLeakDetectorNode* createMemoryLeakAccountingInformation(TestMemoryAllocator* allocator, size_t size, char* memory, bool allocatNodesSeperately);


    bool validMemoryCorruptionInformation(char* memory);
    bool matchingAllocation(TestMemoryAllocator *alloc_allocator, TestMemoryAllocator *free_allocator);

    void storeLeakInformation(MemoryLeakDetectorNode * node, char *new_memory, size_t size, TestMemoryAllocator *allocator, const char *file, size_t line);
    void ConstructMemoryLeakReport(MemLeakPeriod period);

    size_t sizeOfMemoryWithCorruptionInfo(size_t size);
    MemoryLeakDetectorNode* getNodeFromMemoryPointer(char* memory, size_t size);

    char* reallocateMemoryAndLeakInformation(TestMemoryAllocator* allocator, char* memory, size_t size, const char* file, size_t line, bool allocatNodesSeperately);

    void addMemoryCorruptionInformation(char* memory);
    void checkForCorruption(MemoryLeakDetectorNode* node, const char* file, size_t line, TestMemoryAllocator* allocator, bool allocateNodesSeperately);
};

#endif
