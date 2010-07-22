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

TEST_GROUP(MemoryLeakAllocatorTest)
{
	MemoryLeakAllocator* allocator;

	void teardown()
	{
		if (allocator) delete allocator;
	}
};

TEST(MemoryLeakAllocatorTest, SetCurrentNewAllocator)
{
	allocator = new StandardNewAllocator;
	MemoryLeakAllocator::setCurrentNewAllocator(allocator);
	POINTERS_EQUAL(allocator, MemoryLeakAllocator::getCurrentNewAllocator());
	MemoryLeakAllocator::setCurrentNewAllocatorToDefault();
	POINTERS_EQUAL(StandardNewAllocator::defaultAllocator(), MemoryLeakAllocator::getCurrentNewAllocator());
}

TEST(MemoryLeakAllocatorTest, SetCurrentNewArrayAllocator)
{
	allocator = new StandardNewArrayAllocator;
	MemoryLeakAllocator::setCurrentNewArrayAllocator(allocator);
	POINTERS_EQUAL(allocator, MemoryLeakAllocator::getCurrentNewArrayAllocator());
	MemoryLeakAllocator::setCurrentNewArrayAllocatorToDefault();
	POINTERS_EQUAL(StandardNewArrayAllocator::defaultAllocator(), MemoryLeakAllocator::getCurrentNewArrayAllocator());
}

TEST(MemoryLeakAllocatorTest, SetCurrentMallocAllocator)
{
	allocator = new StandardMallocAllocator;
	MemoryLeakAllocator::setCurrentMallocAllocator(allocator);
	POINTERS_EQUAL(allocator, MemoryLeakAllocator::getCurrentMallocAllocator());
	MemoryLeakAllocator::setCurrentMallocAllocatorToDefault();
	POINTERS_EQUAL(StandardMallocAllocator::defaultAllocator(), MemoryLeakAllocator::getCurrentMallocAllocator());
}

TEST(MemoryLeakAllocatorTest, MallocAllocation)
{
	allocator = new StandardMallocAllocator;
	allocator->free_memory(allocator->alloc_memory(100, "file", 1), "file", 1);
}

TEST(MemoryLeakAllocatorTest, MallocNames)
{
	allocator = new StandardMallocAllocator;
	STRCMP_EQUAL("Standard Malloc Allocator", allocator->name());
	STRCMP_EQUAL("malloc", allocator->alloc_name());
	STRCMP_EQUAL("free", allocator->free_name());
}

TEST(MemoryLeakAllocatorTest, NewAllocation)
{
	allocator = new StandardNewAllocator;
	allocator->free_memory(allocator->alloc_memory(100, "file", 1), "file", 1);
}

TEST(MemoryLeakAllocatorTest, NewNames)
{
	allocator = new StandardNewAllocator;
	STRCMP_EQUAL("Standard New Allocator", allocator->name());
	STRCMP_EQUAL("new", allocator->alloc_name());
	STRCMP_EQUAL("delete", allocator->free_name());
}

TEST(MemoryLeakAllocatorTest, NewArrayAllocation)
{
	allocator = new StandardNewArrayAllocator;
	allocator->free_memory(allocator->alloc_memory(100, "file", 1), "file", 1);
}

TEST(MemoryLeakAllocatorTest, NewArrayNames)
{
	allocator = new StandardNewArrayAllocator;
	STRCMP_EQUAL("Standard New [] Allocator", allocator->name());
	STRCMP_EQUAL("new []", allocator->alloc_name());
	STRCMP_EQUAL("delete []", allocator->free_name());
}

TEST(MemoryLeakAllocatorTest, NullUnknownAllocation)
{
	allocator = new NullUnknownAllocator;
	allocator->free_memory(allocator->alloc_memory(100, "file", 1), "file", 1);
}

TEST(MemoryLeakAllocatorTest, NullUnknownNames)
{
	allocator = new NullUnknownAllocator;
	STRCMP_EQUAL("Null Allocator", allocator->name());
	STRCMP_EQUAL("unknown", allocator->alloc_name());
	STRCMP_EQUAL("unknown", allocator->free_name());
}
