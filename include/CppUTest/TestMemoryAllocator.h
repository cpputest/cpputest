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

#ifndef D_TestMemoryAllocator_h
#define D_TestMemoryAllocator_h

struct MemoryLeakNode;
class TestMemoryAllocator;

extern void setCurrentNewAllocator(TestMemoryAllocator* allocator);
extern TestMemoryAllocator* getCurrentNewAllocator();
extern void setCurrentNewAllocatorToDefault();
extern TestMemoryAllocator* defaultNewAllocator();

extern void setCurrentNewArrayAllocator(TestMemoryAllocator* allocator);
extern TestMemoryAllocator* getCurrentNewArrayAllocator();
extern void setCurrentNewArrayAllocatorToDefault();
extern TestMemoryAllocator* defaultNewArrayAllocator();

extern void setCurrentMallocAllocator(TestMemoryAllocator* allocator);
extern TestMemoryAllocator* getCurrentMallocAllocator();
extern void setCurrentMallocAllocatorToDefault();
extern TestMemoryAllocator* defaultMallocAllocator();

class GlobalMemoryAllocatorStash
{
public:
    GlobalMemoryAllocatorStash();
    void save();
    void restore();

private:
    TestMemoryAllocator* originalMallocAllocator;
    TestMemoryAllocator* originalNewAllocator;
    TestMemoryAllocator* originalNewArrayAllocator;
};

class TestMemoryAllocator
{
public:
    TestMemoryAllocator(const char* name_str = "generic", const char* alloc_name_str = "alloc", const char* free_name_str = "free");
    virtual ~TestMemoryAllocator();
    bool hasBeenDestroyed();

    virtual char* alloc_memory(size_t size, const char* file, int line);
    virtual void free_memory(char* memory, const char* file, int line);

    virtual const char* name() const;
    virtual const char* alloc_name() const;
    virtual const char* free_name() const;

    virtual bool isOfEqualType(TestMemoryAllocator* allocator);

    virtual char* allocMemoryLeakNode(size_t size);
    virtual void freeMemoryLeakNode(char* memory);

    virtual TestMemoryAllocator* actualAllocator();

protected:

    const char* name_;
    const char* alloc_name_;
    const char* free_name_;

    bool hasBeenDestroyed_;
};

class CrashOnAllocationAllocator : public TestMemoryAllocator
{
    unsigned allocationToCrashOn_;
public:
    CrashOnAllocationAllocator();
    virtual ~CrashOnAllocationAllocator() _destructor_override;

    virtual void setNumberToCrashOn(unsigned allocationToCrashOn);

    virtual char* alloc_memory(size_t size, const char* file, int line) _override;
};


class NullUnknownAllocator: public TestMemoryAllocator
{
public:
    NullUnknownAllocator();
    virtual ~NullUnknownAllocator() _destructor_override;

    virtual char* alloc_memory(size_t size, const char* file, int line) _override;
    virtual void free_memory(char* memory, const char* file, int line) _override;

    static TestMemoryAllocator* defaultAllocator();
};

class LocationToFailAllocNode;

class FailableMemoryAllocator: public TestMemoryAllocator
{
public:
    FailableMemoryAllocator(const char* name_str = "failable alloc", const char* alloc_name_str = "alloc", const char* free_name_str = "free");
    virtual ~FailableMemoryAllocator() _destructor_override;

    virtual char* alloc_memory(size_t size, const char* file, int line) _override;
    virtual char* allocMemoryLeakNode(size_t size) _override;

    virtual void failAllocNumber(int number);
    virtual void failNthAllocAt(int allocationNumber, const char* file, int line);

    virtual void checkAllFailedAllocsWereDone();
    virtual void clearFailedAllocs();

protected:

    LocationToFailAllocNode* head_;
    int currentAllocNumber_;
};

struct MemoryAccountantAllocationNode;

class MemoryAccountant
{
public:
    MemoryAccountant();

    void clear();

    void alloc(size_t size);
    void dealloc(size_t size);

    size_t totalAllocationsOfSize(size_t size) const;
    size_t totalDeallocationsOfSize(size_t size) const;
    size_t maximumAllocationAtATimeOfSize(size_t size) const;

    size_t totalAllocations() const;
    size_t totalDeallocations() const;

    SimpleString report() const;

    void setAllocator(TestMemoryAllocator* allocator);
private:
    MemoryAccountantAllocationNode* findOrCreateNodeOfSize(size_t size);
    MemoryAccountantAllocationNode* findNodeOfSize(size_t size) const;

    MemoryAccountantAllocationNode* createNewAccountantAllocationNode(size_t size, MemoryAccountantAllocationNode* next);
    void destroyAccountantAllocationNode(MemoryAccountantAllocationNode* node);

    MemoryAccountantAllocationNode* head_;
    TestMemoryAllocator* allocator_;
};

struct AccountingTestMemoryAllocatorMemoryNode;

class AccountingTestMemoryAllocator : public TestMemoryAllocator
{
public:
    AccountingTestMemoryAllocator(MemoryAccountant& accountant, TestMemoryAllocator* originalAllocator);
    virtual ~AccountingTestMemoryAllocator() _destructor_override;

    virtual char* alloc_memory(size_t size, const char* file, int line) _override;
    virtual void free_memory(char* memory, const char* file, int line) _override;

    virtual TestMemoryAllocator* actualAllocator() _override;
    TestMemoryAllocator* originalAllocator();
private:

    void addMemoryToMemoryTrackingToKeepTrackOfSize(char* memory, size_t size);
    size_t removeMemoryFromTrackingAndReturnAllocatedSize(char* memory);

    size_t removeNextNodeAndReturnSize(AccountingTestMemoryAllocatorMemoryNode* node);
    size_t removeHeadAndReturnSize();

    MemoryAccountant& accountant_;
    TestMemoryAllocator* originalAllocator_;
    AccountingTestMemoryAllocatorMemoryNode* head_;
};

class GlobalMemoryAccountant
{
public:
    GlobalMemoryAccountant();
    ~GlobalMemoryAccountant();

    void start();
    void stop();
    SimpleString report();

    TestMemoryAllocator* getMallocAllocator();
    TestMemoryAllocator* getNewAllocator();
    TestMemoryAllocator* getNewArrayAllocator();

private:

    void restoreMemoryAllocators();

    MemoryAccountant accountant_;
    AccountingTestMemoryAllocator* mallocAllocator_;
    AccountingTestMemoryAllocator* newAllocator_;
    AccountingTestMemoryAllocator* newArrayAllocator_;
};

#endif

