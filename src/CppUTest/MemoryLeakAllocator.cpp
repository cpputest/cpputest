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
#include "CppUTest/MemoryLeakAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"

static char* checkedMalloc(size_t size)
{
	char* mem = (char*) PlatformSpecificMalloc(size);
	if (mem == 0)
	FAIL("malloc returned nul pointer");
	return mem;
}

MemoryLeakAllocator* MemoryLeakAllocator::currentNewAllocator = 0;
MemoryLeakAllocator* MemoryLeakAllocator::currentNewArrayAllocator = 0;
MemoryLeakAllocator* MemoryLeakAllocator::currentMallocAllocator = 0;

bool MemoryLeakAllocator::isOfEqualType(MemoryLeakAllocator* allocator)
{
	return PlatformSpecificStrCmp(this->name(), allocator->name()) == 0;
}

void MemoryLeakAllocator::setCurrentNewAllocator(MemoryLeakAllocator* allocator)
{
	currentNewAllocator = allocator;
}

MemoryLeakAllocator* MemoryLeakAllocator::getCurrentNewAllocator()
{
	if (currentNewAllocator == 0) setCurrentNewAllocatorToDefault();
	return currentNewAllocator;
}

void MemoryLeakAllocator::setCurrentNewAllocatorToDefault()
{
	currentNewAllocator = StandardNewAllocator::defaultAllocator();
}

void MemoryLeakAllocator::setCurrentNewArrayAllocator(MemoryLeakAllocator* allocator)
{
	currentNewArrayAllocator = allocator;
}

MemoryLeakAllocator* MemoryLeakAllocator::getCurrentNewArrayAllocator()
{
	if (currentNewArrayAllocator == 0) setCurrentNewArrayAllocatorToDefault();
	return currentNewArrayAllocator;
}

void MemoryLeakAllocator::setCurrentNewArrayAllocatorToDefault()
{
	currentNewArrayAllocator = StandardNewArrayAllocator::defaultAllocator();
}

void MemoryLeakAllocator::setCurrentMallocAllocator(MemoryLeakAllocator* allocator)
{
	currentMallocAllocator = allocator;
}

MemoryLeakAllocator* MemoryLeakAllocator::getCurrentMallocAllocator()
{
	if (currentMallocAllocator == 0) setCurrentMallocAllocatorToDefault();
	return currentMallocAllocator;
}

void MemoryLeakAllocator::setCurrentMallocAllocatorToDefault()
{
	currentMallocAllocator = StandardMallocAllocator::defaultAllocator();
}

bool MemoryLeakAllocator::allocateMemoryLeakNodeSeparately()
{
	return false;
}

char* MemoryLeakAllocator::allocMemoryLeakNode(size_t size)
{
	return alloc_memory(size, "MemoryLeakNode", 1);
}

void MemoryLeakAllocator::freeMemoryLeakNode(char* memory)
{
	free_memory(memory, "MemoryLeakNode", 1);
}

char* StandardMallocAllocator::alloc_memory(size_t size, const char*, int)
{
	return checkedMalloc(size);
}

void StandardMallocAllocator::free_memory(char* memory, const char*, int)
{
	PlatformSpecificFree(memory);
}

const char* StandardMallocAllocator::name()
{
	return "Standard Malloc Allocator";
}

const char* StandardMallocAllocator::alloc_name()
{
	return "malloc";
}

const char* StandardMallocAllocator::free_name()
{
	return "free";
}

bool StandardMallocAllocator::allocateMemoryLeakNodeSeparately()
{
	return true;
}

MemoryLeakAllocator* StandardMallocAllocator::defaultAllocator()
{
	static StandardMallocAllocator allocator;
	return &allocator;
}

char* StandardNewAllocator::alloc_memory(size_t size, const char*, int)
{
	return checkedMalloc(size);
}

void StandardNewAllocator::free_memory(char* memory, const char*, int)
{
	PlatformSpecificFree(memory);
}

const char* StandardNewAllocator::name()
{
	return "Standard New Allocator";
}

const char* StandardNewAllocator::alloc_name()
{
	return "new";
}

const char* StandardNewAllocator::free_name()
{
	return "delete";
}

MemoryLeakAllocator* StandardNewAllocator::defaultAllocator()
{
	static StandardNewAllocator allocator;
	return &allocator;
}

char* StandardNewArrayAllocator::alloc_memory(size_t size, const char*, int)
{
	return checkedMalloc(size);
}

void StandardNewArrayAllocator::free_memory(char* memory, const char*, int)
{
	PlatformSpecificFree(memory);
}

const char* StandardNewArrayAllocator::name()
{
	return "Standard New [] Allocator";
}

const char* StandardNewArrayAllocator::alloc_name()
{
	return "new []";
}

const char* StandardNewArrayAllocator::free_name()
{
	return "delete []";
}

MemoryLeakAllocator* StandardNewArrayAllocator::defaultAllocator()
{
	static StandardNewArrayAllocator allocator;
	return &allocator;
}
char* NullUnknownAllocator::alloc_memory(size_t /*size*/, const char*, int)
{
	return 0;
}

void NullUnknownAllocator::free_memory(char* /*memory*/, const char*, int)
{
}

const char* NullUnknownAllocator::name()
{
	return "Null Allocator";
}

const char* NullUnknownAllocator::alloc_name()
{
	return "unknown";
}

const char* NullUnknownAllocator::free_name()
{
	return "unknown";
}

MemoryLeakAllocator* NullUnknownAllocator::defaultAllocator()
{
	static NullUnknownAllocator allocator;
	return &allocator;
}
