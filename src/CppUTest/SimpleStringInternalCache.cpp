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
#include "CppUTest/SimpleStringInternalCache.h"

struct SimpleStringMemoryBlock
{
    SimpleStringMemoryBlock* next_;
    char* memory_;
};

struct SimpleStringInternalCacheNode
{
    size_t size_;
    SimpleStringMemoryBlock* freeMemoryHead_;
    SimpleStringMemoryBlock* usedMemoryHead_;
};

SimpleStringInternalCache::SimpleStringInternalCache()
    : allocator_(defaultMallocAllocator()), cache_(NULLPTR), nonCachedAllocations_(NULLPTR), hasWarnedAboutDeallocations(false)
{
    cache_ = createInternalCacheNodes();
}

SimpleStringInternalCache::~SimpleStringInternalCache()
{
    allocator_ = defaultMallocAllocator();
    destroyInternalCacheNode(cache_);
}

void SimpleStringInternalCache::setAllocator(TestMemoryAllocator* allocator)
{
    allocator_ = allocator;
}

SimpleStringInternalCacheNode* SimpleStringInternalCache::createInternalCacheNodes()
{
    SimpleStringInternalCacheNode* node = (SimpleStringInternalCacheNode*) (void*) allocator_->alloc_memory(sizeof(SimpleStringInternalCacheNode) * amountOfInternalCacheNodes, __FILE__, __LINE__);

    for (int i = 0; i < amountOfInternalCacheNodes; i++) {
        node[i].freeMemoryHead_ = NULLPTR;
        node[i].usedMemoryHead_ = NULLPTR;
    }
    node[0].size_ = 32;
    node[1].size_ = 64;
    node[2].size_ = 96;
    node[3].size_ = 128;
    node[4].size_ = 256;
    return node;
}

bool SimpleStringInternalCache::isCached(size_t size)
{
    return size <= 256;
}

size_t SimpleStringInternalCache::getIndexForCache(size_t size)
{
    for (size_t i = 0; i < amountOfInternalCacheNodes; i++)
      if (size <= cache_[i].size_)
        return i;
    return 0; // LCOV_EXCL_LINE
}

SimpleStringInternalCacheNode* SimpleStringInternalCache::getCacheNodeFromSize(size_t size)
{
    size_t index = getIndexForCache(size);
    return &cache_[index];
}

void SimpleStringInternalCache::destroyInternalCacheNode(SimpleStringInternalCacheNode * node)
{
    allocator_->free_memory((char*) node, sizeof(SimpleStringInternalCacheNode) * amountOfInternalCacheNodes, __FILE__, __LINE__);
}

SimpleStringMemoryBlock* SimpleStringInternalCache::createSimpleStringMemoryBlock(size_t size, SimpleStringMemoryBlock* next)
{
    SimpleStringMemoryBlock* block = (SimpleStringMemoryBlock*) (void*) allocator_->alloc_memory(sizeof(SimpleStringMemoryBlock) , __FILE__, __LINE__);
    block->memory_ = allocator_->alloc_memory(size , __FILE__, __LINE__);
    block->next_ = next;
    return block;
}

void SimpleStringInternalCache::destroySimpleStringMemoryBlock(SimpleStringMemoryBlock * block, size_t size)
{
    allocator_->free_memory(block->memory_, size, __FILE__, __LINE__);
    allocator_->free_memory((char*) block, sizeof(SimpleStringMemoryBlock), __FILE__, __LINE__);
}

void SimpleStringInternalCache::destroySimpleStringMemoryBlockList(SimpleStringMemoryBlock * block, size_t size)
{
    SimpleStringMemoryBlock* current = block;
    while (current) {
        SimpleStringMemoryBlock* next = current->next_;
        destroySimpleStringMemoryBlock(current, size);
        current = next;
    }
}

SimpleStringMemoryBlock* SimpleStringInternalCache::addToSimpleStringMemoryBlockList(SimpleStringMemoryBlock* newBlock, SimpleStringMemoryBlock* previousHead)
{
    newBlock->next_ = previousHead;
    return newBlock;
}

bool SimpleStringInternalCache::hasFreeBlocksOfSize(size_t size)
{
    return getCacheNodeFromSize(size)->freeMemoryHead_ != NULLPTR;
}

SimpleStringMemoryBlock* SimpleStringInternalCache::reserveCachedBlockFrom(SimpleStringInternalCacheNode* node)
{
    SimpleStringMemoryBlock* block = node->freeMemoryHead_;
    node->freeMemoryHead_ = block->next_;
    node->usedMemoryHead_ = addToSimpleStringMemoryBlockList(block, node->usedMemoryHead_);
    return block;
}

SimpleStringMemoryBlock* SimpleStringInternalCache::allocateNewCacheBlockFrom(SimpleStringInternalCacheNode* node)
{
    SimpleStringMemoryBlock* block = createSimpleStringMemoryBlock(node->size_, node->usedMemoryHead_);
    node->usedMemoryHead_ = addToSimpleStringMemoryBlockList(block, node->usedMemoryHead_);
    return block;
}

void SimpleStringInternalCache::printDeallocatingUnknownMemory(char* memory)
{
    if (!hasWarnedAboutDeallocations) {
        hasWarnedAboutDeallocations = true;
        UtestShell::getCurrent()->print(StringFromFormat("\nWARNING: Attempting to deallocate a String buffer that was allocated while not caching. Ignoring it!\n"
                                                         "This is likely due statics and will cause problems.\n"
                                                          "Only warning once to avoid recursive warnings.\n"
                                                          "String we are deallocating: \"%s\"\n", memory).asCharString(), __FILE__, __LINE__);
    }
}

void SimpleStringInternalCache::releaseCachedBlockFrom(char* memory, SimpleStringInternalCacheNode* node)
{
    if (node->usedMemoryHead_ && node->usedMemoryHead_->memory_ == memory) {
        SimpleStringMemoryBlock* block = node->usedMemoryHead_;
        node->usedMemoryHead_ = node->usedMemoryHead_->next_;
        node->freeMemoryHead_ = addToSimpleStringMemoryBlockList(block, node->freeMemoryHead_);
        return;
    }

    for (SimpleStringMemoryBlock* block = node->usedMemoryHead_; block; block = block->next_) {
        if (block->next_ && block->next_->memory_ == memory) {
            SimpleStringMemoryBlock* blockToFree = block->next_;
            block->next_ = block->next_->next_;
            node->freeMemoryHead_ = addToSimpleStringMemoryBlockList(blockToFree, node->freeMemoryHead_);
            return;
        }
    }
    printDeallocatingUnknownMemory(memory);

}

void SimpleStringInternalCache::releaseNonCachedMemory(char* memory, size_t size)
{
    if (nonCachedAllocations_ && nonCachedAllocations_->memory_ == memory) {
        SimpleStringMemoryBlock* block = nonCachedAllocations_;
        nonCachedAllocations_ = block->next_;
        destroySimpleStringMemoryBlock(block, size);
        return;
    }

    for (SimpleStringMemoryBlock* block = nonCachedAllocations_; block; block = block->next_) {
        if (block->next_ && block->next_->memory_ == memory) {
            SimpleStringMemoryBlock* blockToFree = block->next_;
            block->next_ = block->next_->next_;
            destroySimpleStringMemoryBlock(blockToFree, size);
            return;
        }
    }

    printDeallocatingUnknownMemory(memory);
}

char* SimpleStringInternalCache::alloc(size_t size)
{
    if (isCached(size)) {
        if (hasFreeBlocksOfSize(size))
            return reserveCachedBlockFrom(getCacheNodeFromSize(size))->memory_;
        else
            return allocateNewCacheBlockFrom(getCacheNodeFromSize(size))->memory_;
    }

    nonCachedAllocations_ = createSimpleStringMemoryBlock(size, nonCachedAllocations_);
    return nonCachedAllocations_->memory_;
}

void SimpleStringInternalCache::dealloc(char* memory, size_t size)
{
    if (isCached(size)) {
        size_t index = getIndexForCache(size);
        SimpleStringInternalCacheNode* cacheNode = &cache_[index];
        releaseCachedBlockFrom(memory, cacheNode);
        return;
    }
    releaseNonCachedMemory(memory, size);
}

void SimpleStringInternalCache::clearCache()
{
    for (size_t i = 0; i < amountOfInternalCacheNodes; i++) {
        destroySimpleStringMemoryBlockList(cache_[i].freeMemoryHead_, cache_[i].size_);
        cache_[i].freeMemoryHead_ = NULLPTR;
    }
}

void SimpleStringInternalCache::clearAllIncludingCurrentlyUsedMemory()
{
    for (size_t i = 0; i < amountOfInternalCacheNodes; i++) {
        destroySimpleStringMemoryBlockList(cache_[i].freeMemoryHead_, cache_[i].size_);
        destroySimpleStringMemoryBlockList(cache_[i].usedMemoryHead_, cache_[i].size_);
        cache_[i].freeMemoryHead_ = NULLPTR;
        cache_[i].usedMemoryHead_ = NULLPTR;
    }

    destroySimpleStringMemoryBlockList(nonCachedAllocations_, 0);
    nonCachedAllocations_ = NULLPTR;
}

GlobalSimpleStringCache::GlobalSimpleStringCache()
{
    allocator_ = new SimpleStringCacheAllocator(cache_, SimpleString::getStringAllocator());
    SimpleString::setStringAllocator(allocator_);
}

GlobalSimpleStringCache::~GlobalSimpleStringCache()
{
    SimpleString::setStringAllocator(allocator_->originalAllocator());
    cache_.clearAllIncludingCurrentlyUsedMemory();
    delete allocator_;
}

TestMemoryAllocator* GlobalSimpleStringCache::getAllocator()
{
    return allocator_;
}

SimpleStringCacheAllocator::SimpleStringCacheAllocator(SimpleStringInternalCache& cache, TestMemoryAllocator* origAllocator)
    : cache_(cache), originalAllocator_(origAllocator)
{
    cache_.setAllocator(origAllocator);
}

SimpleStringCacheAllocator::~SimpleStringCacheAllocator()
{
    cache_.setAllocator(NULLPTR);
}

char* SimpleStringCacheAllocator::alloc_memory(size_t size, const char*, size_t)
{
    return cache_.alloc(size);
}

void SimpleStringCacheAllocator::free_memory(char* memory, size_t size, const char*, size_t)
{
    cache_.dealloc(memory, size);
}

const char* SimpleStringCacheAllocator::name() const
{
    return "SimpleStringCacheAllocator";
}

const char* SimpleStringCacheAllocator::alloc_name() const
{
    return originalAllocator_->alloc_name();
}

const char* SimpleStringCacheAllocator::free_name() const
{
    return originalAllocator_->free_name();
}

TestMemoryAllocator* SimpleStringCacheAllocator::actualAllocator()
{
    return originalAllocator_->actualAllocator();
}

TestMemoryAllocator* SimpleStringCacheAllocator::originalAllocator()
{
    return originalAllocator_;
}

