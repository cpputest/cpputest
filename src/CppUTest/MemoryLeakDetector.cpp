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
#include "CppUTest/MemoryLeakAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"

#define UNKNOWN ((char*)("<unknown>"))

SimpleStringBuffer::SimpleStringBuffer() :
	positions_filled_(0)
{
}


void SimpleStringBuffer::clear()
{
	positions_filled_ = 0;
	buffer_[0] = '\0';
}

void SimpleStringBuffer::add(const char* format, ...)
{
	int count = 0;
	va_list arguments;
	va_start(arguments, format);
	count = PlatformSpecificVSNprintf(buffer_ + positions_filled_,
			SIMPLE_STRING_BUFFER_LEN - positions_filled_, format, arguments);
	if (count > 0) positions_filled_ += count;
	va_end(arguments);
}

char* SimpleStringBuffer::toString()
{
	return buffer_;
}

///////////////////////

void MemoryLeakDetectorList::initNode(MemoryLeakDetectorNode* node,
		MemoryLeakAllocator* allocator, size_t size, char* memory,
		MemLeakPeriod period, const char* file, int line)
{
	if (node) {
		node->size_ = size;
		node->memory_ = memory;
		node->period_ = period;
		node->file_ = file;
		node->line_ = line;
		node->allocator_ = allocator;
	}
}

bool MemoryLeakDetectorList::isInPeriod(MemoryLeakDetectorNode* node,
		MemLeakPeriod period)
{
	return period == mem_leak_period_all || node->period_ == period
			|| (node->period_ != mem_leak_period_disabled && period
					== mem_leak_period_enabled);
}

void MemoryLeakDetectorList::clearAllAccounting(MemLeakPeriod period)
{
	MemoryLeakDetectorNode* cur = head_;
	MemoryLeakDetectorNode* prev = 0;

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
	MemoryLeakDetectorNode* prev = 0;
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
	return 0;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getLeakFrom(
		MemoryLeakDetectorNode* node, MemLeakPeriod period)
{
	for (MemoryLeakDetectorNode* cur = node; cur; cur = cur->next_)
		if (isInPeriod(cur, period)) return cur;
	return 0;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getFirstLeak(
		MemLeakPeriod period)
{
	return getLeakFrom(head_, period);
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getNextLeak(
		MemoryLeakDetectorNode* node, MemLeakPeriod period)
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

bool MemoryLeakDetectorList::hasLeaks(MemLeakPeriod period)
{
	for (MemoryLeakDetectorNode* node = head_; node; node = node->next_)
		if (isInPeriod(node, period)) return true;
	return false;
}

/////////////////////////////////////////////////////////////

int MemoryLeakDetectorTable::hash(char* memory)
{
	return ((size_t) memory) % hash_prime;
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

bool MemoryLeakDetectorTable::hasLeaks(MemLeakPeriod period)
{
	for (int i = 0; i < hash_prime; i++)
		if (table_[i].hasLeaks(period)) return true;
	return false;
}

int MemoryLeakDetectorTable::getTotalLeaks(MemLeakPeriod period)
{
	int total_leaks = 0;
	for (int i = 0; i < hash_prime; i++)
		total_leaks += table_[i].getTotalLeaks(period);
	return total_leaks;
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::getFirstLeak(
		MemLeakPeriod period)
{
	for (int i = 0; i < hash_prime; i++) {
		MemoryLeakDetectorNode* node = table_[i].getFirstLeak(period);
		if (node) return node;
	}
	return 0;
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::getNextLeak(
		MemoryLeakDetectorNode* leak, MemLeakPeriod period)
{
	int i = hash(leak->memory_);
	MemoryLeakDetectorNode* node = table_[i].getNextLeak(leak, period);
	if (node) return node;

	for (++i; i < hash_prime; i++) {
		node = table_[i].getFirstLeak(period);
		if (node) return node;
	}
	return 0;
}

/////////////////////////////////////////////////////////////

MemoryLeakDetector::MemoryLeakDetector()
{
}

void MemoryLeakDetector::init(MemoryLeakFailure* reporter)
{
	doAllocationTypeChecking_ = true;
	current_period_ = mem_leak_period_disabled;
	reporter_ = reporter;
	output_buffer_ = SimpleStringBuffer();
	memoryTable_ = MemoryLeakDetectorTable();
}

void MemoryLeakDetector::clearAllAccounting(MemLeakPeriod period)
{
	memoryTable_.clearAllAccounting(period);
}

void MemoryLeakDetector::startChecking()
{
	output_buffer_.clear();
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

void MemoryLeakDetector::reportFailure(const char* message,
		const char* allocFile, int allocLine, size_t allocSize,
		MemoryLeakAllocator* allocAllocator, const char* freeFile,
		int freeLine, MemoryLeakAllocator* freeAllocator)
{
	output_buffer_.add(message);
	output_buffer_.add(MEM_LEAK_ALLOC_LOCATION, allocFile, allocLine, allocSize,
			allocAllocator->alloc_name());
	output_buffer_.add(MEM_LEAK_DEALLOC_LOCATION, freeFile, freeLine,
			freeAllocator->free_name());
	reporter_->fail(output_buffer_.toString());
}

int calculateIntAlignedSize(size_t size)
{
	return (sizeof(int) - (size % sizeof(int))) + size;
}

MemoryLeakDetectorNode* MemoryLeakDetector::getNodeFromMemoryPointer(
		char* memory, size_t memory_size)
{
	return (MemoryLeakDetectorNode*) (memory + calculateIntAlignedSize(
			memory_size + memory_corruption_buffer_size));
}

char* MemoryLeakDetector::allocateMemoryAndExtraInfo(
		MemoryLeakAllocator* allocator, size_t size)
{
	return allocator->alloc_memory(calculateIntAlignedSize(size
			+ memory_corruption_buffer_size) + memory_corruption_buffer_size
			+ sizeof(MemoryLeakDetectorNode));
}

char* MemoryLeakDetector::reallocateMemoryAndExtraInfo(char* memory,
		size_t size)
{
	return (char*) PlatformSpecificRealloc(memory, calculateIntAlignedSize(size
			+ memory_corruption_buffer_size) + sizeof(MemoryLeakDetectorNode));
}

void MemoryLeakDetector::addMemoryCorruptionInformation(char* memory,
		size_t size)
{
	memory[size] = 'B';
	memory[size + 1] = 'A';
	memory[size + 2] = 'S';
}

void MemoryLeakDetector::checkForAllocMismatchOrCorruption(
		MemoryLeakDetectorNode* node, const char* file, int line,
		MemoryLeakAllocator* allocator)
{
	if (node->allocator_ != allocator && doAllocationTypeChecking_) {
		if (!allocator->isOfEqualType(node->allocator_)) reportFailure(
				MEM_LEAK_ALLOC_DEALLOC_MISMATCH, node->file_, node->line_,
				node->size_, node->allocator_, file, line, allocator);
	}
	else if (node->memory_[node->size_] != 'B' || node->memory_[node->size_ + 1]
			!= 'A' || node->memory_[node->size_ + 2] != 'S') reportFailure(
			MEM_LEAK_MEMORY_CORRUPTION, node->file_, node->line_, node->size_,
			node->allocator_, file, line, allocator);
}

void MemoryLeakDetector::addMemoryLeakInfoAndCorruptionInfo(char* memory,
		size_t size, const char* file, int line, MemoryLeakAllocator* allocator)
{
	addMemoryCorruptionInformation(memory, size);
	if (memory) {
		MemoryLeakDetectorNode* node = getNodeFromMemoryPointer(memory, size);
		MemoryLeakDetectorList::initNode(node, allocator, size, memory,
				current_period_, file, line);
		memoryTable_.addNewNode(node);
	}
}

bool MemoryLeakDetector::removeMemoryLeakInfoAndCheckCorruption(char* memory,
		const char* file, int line, MemoryLeakAllocator* allocator)
{
	MemoryLeakDetectorNode* node = memoryTable_.removeNode(memory);
	if (node) {
		checkForAllocMismatchOrCorruption(node, file, line, allocator);
		return true;
	}
	reportFailure(MEM_LEAK_DEALLOC_NON_ALLOCATED, "<unknown>", 0, 0,
			NullUnknownAllocator::defaultAllocator(), file, line, allocator);
	return false;
}

char* MemoryLeakDetector::allocMemory(MemoryLeakAllocator* allocator,
		size_t size)
{
	return allocMemory(allocator, size, UNKNOWN, 0);
}

char* MemoryLeakDetector::allocMemory(MemoryLeakAllocator* allocator,
		size_t size, const char* file, int line)
{
	char* mem = allocateMemoryAndExtraInfo(allocator, size);
	if (mem)
		addMemoryLeakInfoAndCorruptionInfo(mem, size, file, line, allocator);
	return mem;
}

void MemoryLeakDetector::deallocMemory(MemoryLeakAllocator* allocator,
		void* memory, const char* file, int line)
{
	if (memory == 0) return;

	if (removeMemoryLeakInfoAndCheckCorruption((char*) memory, file, line,
			allocator)) allocator->free_memory((char*) memory);
}

void MemoryLeakDetector::deallocMemory(MemoryLeakAllocator* allocator,
		void* memory)
{
	deallocMemory(allocator, (char*) memory, UNKNOWN, 0);
}

char* MemoryLeakDetector::reallocMemory(MemoryLeakAllocator* allocator,
		char* memory, size_t size, const char* file, int line)
{
	if (memory) removeMemoryLeakInfoAndCheckCorruption(memory, file, line,
			allocator);

	char* mem = reallocateMemoryAndExtraInfo(memory, size);
	addMemoryCorruptionInformation(mem, size);
	addMemoryLeakInfoAndCorruptionInfo(mem, size, file, line, allocator);
	return mem;
}

void MemoryLeakDetector::ConstructMemoryLeakReport(MemLeakPeriod period)
{
	MemoryLeakDetectorNode* leak = memoryTable_.getFirstLeak(period);
	int total_leaks = 0;
	output_buffer_.add(MEM_LEAK_HEADER);

	while (leak) {
		output_buffer_.add(MEM_LEAK_LEAK, leak->size_, leak->file_, leak->line_,
				leak->allocator_->alloc_name(), leak->memory_);
		total_leaks++;
		leak = memoryTable_.getNextLeak(leak, period);
	}
	output_buffer_.add("%s %d", MEM_LEAK_FOOTER, total_leaks);
}

const char* MemoryLeakDetector::report(MemLeakPeriod period)
{
	if (!memoryTable_.hasLeaks(period)) return MEM_LEAK_NONE;

	output_buffer_.clear();
	ConstructMemoryLeakReport(period);

	return output_buffer_.toString();
}

void MemoryLeakDetector::markCheckingPeriodLeaksAsNonCheckingPeriod()
{
	MemoryLeakDetectorNode* leak = memoryTable_.getFirstLeak(
			mem_leak_period_checking);
	while (leak) {
		if (leak->period_ == mem_leak_period_checking) leak->period_
				= mem_leak_period_enabled;
		leak = memoryTable_.getNextLeak(leak, mem_leak_period_checking);
	}
}

int MemoryLeakDetector::totalMemoryLeaks(MemLeakPeriod period)
{
	return memoryTable_.getTotalLeaks(period);
}
