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

#ifndef D_SimpleStringInternalCache_h
#define D_SimpleStringInternalCache_h

#include "CppUTest/TestMemoryAllocator.h"

struct SimpleStringMemoryBlock;
struct SimpleStringInternalCacheNode;

class SimpleStringInternalCache
{
public:
    SimpleStringInternalCache();
    ~SimpleStringInternalCache();

    void setAllocator(TestMemoryAllocator* allocator);

    char* alloc(size_t size);
    void dealloc(char* memory, size_t size);

    bool hasFreeBlocksOfSize(size_t size);

    void clearCache();
    void clearAllIncludingCurrentlyUsedMemory();
private:
    void printDeallocatingUnknownMemory(char* memory);

    enum { amountOfInternalCacheNodes = 5};
    bool isCached(size_t size);
    size_t getIndexForCache(size_t size);
    SimpleStringInternalCacheNode* getCacheNodeFromSize(size_t size);

    SimpleStringInternalCacheNode* createInternalCacheNodes();
    void destroyInternalCacheNode(SimpleStringInternalCacheNode * node);
    SimpleStringMemoryBlock* createSimpleStringMemoryBlock(size_t sizeOfString, SimpleStringMemoryBlock* next);
    void destroySimpleStringMemoryBlock(SimpleStringMemoryBlock * block, size_t size);
    void destroySimpleStringMemoryBlockList(SimpleStringMemoryBlock * block, size_t size);

    SimpleStringMemoryBlock* reserveCachedBlockFrom(SimpleStringInternalCacheNode* node);
    void releaseCachedBlockFrom(char* memory, SimpleStringInternalCacheNode* node);
    void releaseNonCachedMemory(char* memory, size_t size);

    SimpleStringMemoryBlock* allocateNewCacheBlockFrom(SimpleStringInternalCacheNode* node);
    SimpleStringMemoryBlock* addToSimpleStringMemoryBlockList(SimpleStringMemoryBlock* newBlock, SimpleStringMemoryBlock* previousHead);

    TestMemoryAllocator* allocator_;
    SimpleStringInternalCacheNode* cache_;
    SimpleStringMemoryBlock* nonCachedAllocations_;
    bool hasWarnedAboutDeallocations;
};

class SimpleStringCacheAllocator : public TestMemoryAllocator
{
public:
    SimpleStringCacheAllocator(SimpleStringInternalCache& cache, TestMemoryAllocator* previousAllocator);
    virtual ~SimpleStringCacheAllocator() _destructor_override;

    virtual char* alloc_memory(size_t size, const char* file, size_t line) _override;
    virtual void free_memory(char* memory, size_t size, const char* file, size_t line) _override;

    virtual const char* name() const _override;
    virtual const char* alloc_name() const _override;
    virtual const char* free_name() const _override;

    virtual TestMemoryAllocator* actualAllocator() _override;
    TestMemoryAllocator* originalAllocator();
private:
    SimpleStringInternalCache& cache_;
    TestMemoryAllocator* originalAllocator_;
};

class GlobalSimpleStringCache
{
    SimpleStringCacheAllocator* allocator_;
    SimpleStringInternalCache cache_;

public:
    GlobalSimpleStringCache();
    ~GlobalSimpleStringCache();

    TestMemoryAllocator* getAllocator();
};

#endif
