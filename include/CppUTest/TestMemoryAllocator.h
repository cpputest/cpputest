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

class TestMemoryAllocator
{
public:
    TestMemoryAllocator(const char* name_str = "generic", const char* alloc_name_str = "alloc", const char* free_name_str = "free");
    virtual ~TestMemoryAllocator();
    bool hasBeenDestroyed();

    virtual char* alloc_memory(size_t size, const char* file, int line);
    virtual void free_memory(char* memory, const char* file, int line);

    virtual const char* name();
    virtual const char* alloc_name();
    virtual const char* free_name();

    virtual bool isOfEqualType(TestMemoryAllocator* allocator);

    virtual char* allocMemoryLeakNode(size_t size);
    virtual void freeMemoryLeakNode(char* memory);

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

    virtual void setNumberToCrashOn(unsigned allocationToCrashOn);

    virtual char* alloc_memory(size_t size, const char* file, int line) _override;
};


class NullUnknownAllocator: public TestMemoryAllocator
{
public:
    NullUnknownAllocator();
    virtual char* alloc_memory(size_t size, const char* file, int line) _override;
    virtual void free_memory(char* memory, const char* file, int line) _override;

    static TestMemoryAllocator* defaultAllocator();
};

class LocationToFailAllocNode;

class FailableMemoryAllocator: public TestMemoryAllocator
{
public:
    FailableMemoryAllocator(const char* name_str = "failable alloc", const char* alloc_name_str = "alloc", const char* free_name_str = "free");

    virtual char* alloc_memory(size_t size, const char* file, int line);
    virtual char* allocMemoryLeakNode(size_t size);

    virtual void failAllocNumber(int number);
    virtual void failNthAllocAt(int allocationNumber, const char* file, int line);

    virtual void checkAllFailedAllocsWereDone();
    virtual void clearFailedAllocs();

protected:

    LocationToFailAllocNode* head_;
    int currentAllocNumber_;
};

#endif

