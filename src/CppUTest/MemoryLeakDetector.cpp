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
#include "CppUTest/PlatformSpecificFunctions.h"

#define UNKNOWN ((char*)("<unknown>"))

SimpleBuffer::SimpleBuffer() : positions_filled(0)
{
};

void SimpleBuffer::clear()
{
   positions_filled = 0;
   buffer[0] = '\0';
}

void SimpleBuffer::add(const char* format, ...)
{
   int count = 0;
   va_list arguments;
   va_start(arguments, format);
   count = PlatformSpecificVSNprintf(buffer + positions_filled, SIMPLE_BUFFER_LEN - positions_filled, format, arguments);
   if (count > 0)
      positions_filled += count;
   va_end(arguments);
}

char* SimpleBuffer::toString()
{
   return buffer;
}

///////////////////////

void MemoryLeakDetectorList::initNode(MemoryLeakDetectorNode* node, size_t size, char* memory, MemLeakPeriod period, char* file, int line, MemLeakAllocType type)
{
   if (node) {
      node->size = size;
      node->memory = memory;
      node->period = period;
      node->file = file;
      node->line = line;
      node->type = type;
   }
}

bool MemoryLeakDetectorList::isInPeriod(MemoryLeakDetectorNode* node, MemLeakPeriod period)
{

	return period == mem_leak_period_all || node->period == period || (node->period != mem_leak_period_disabled && period == mem_leak_period_enabled);
}

void MemoryLeakDetectorList::clearAllAccounting(MemLeakPeriod period)
{
   MemoryLeakDetectorNode* cur = head;
   MemoryLeakDetectorNode* prev = 0;

   while (cur) {
      if (isInPeriod(cur, period)) {
         if (prev) {
            prev->next = cur->next;
            cur = prev;
         }
         else {
            head = cur->next;
            cur = head;
            continue;
         }
      }
      prev = cur;
      cur = cur->next;
   }
}

void MemoryLeakDetectorList::addNewNode(MemoryLeakDetectorNode* node)
{
   node->next = head;
   head = node;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::removeNode(char* memory)
{
   MemoryLeakDetectorNode* cur = head;
   MemoryLeakDetectorNode* prev = 0;
   while (cur) {
      if (cur->memory == memory) {
         if (prev) {
            prev->next = cur->next;
            return cur;
         }
         else {
            head = cur->next;
            return cur;
         }
      }
      prev = cur;
      cur = cur->next;
   }
   return 0;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getLeakFrom(MemoryLeakDetectorNode* node, MemLeakPeriod period)
{
   for (MemoryLeakDetectorNode* cur = node; cur; cur = cur->next)
      if (isInPeriod(cur, period)) return cur;
   return 0;
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getFirstLeak(MemLeakPeriod period)
{
   return getLeakFrom(head, period);
}

MemoryLeakDetectorNode* MemoryLeakDetectorList::getNextLeak(MemoryLeakDetectorNode* node, MemLeakPeriod period)
{
   return getLeakFrom(node->next, period);
}

int MemoryLeakDetectorList::getTotalLeaks(MemLeakPeriod period)
{
   int total_leaks = 0;
   for (MemoryLeakDetectorNode* node = head; node; node = node->next) {
      if (isInPeriod(node, period)) total_leaks++;
   }
   return total_leaks;
}

bool MemoryLeakDetectorList::hasLeaks(MemLeakPeriod period)
{
   for (MemoryLeakDetectorNode* node = head; node; node = node->next)
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
      table[i].clearAllAccounting(period);
}

void MemoryLeakDetectorTable::addNewNode(MemoryLeakDetectorNode* node)
{
   table[hash(node->memory)].addNewNode(node);
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::removeNode(char* memory)
{
   return table[hash(memory)].removeNode(memory);
}

bool MemoryLeakDetectorTable::hasLeaks(MemLeakPeriod period)
{
   for (int i = 0; i < hash_prime; i++)
      if (table[i].hasLeaks(period)) return true;
   return false;
}

int MemoryLeakDetectorTable::getTotalLeaks(MemLeakPeriod period)
{
   int total_leaks = 0;
   for (int i = 0; i < hash_prime; i++)
      total_leaks += table[i].getTotalLeaks(period);
   return total_leaks;
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::getFirstLeak(MemLeakPeriod period)
{
   for (int i = 0; i < hash_prime; i++) {
      MemoryLeakDetectorNode* node = table[i].getFirstLeak(period);
      if (node) return node;
   }
   return 0;
}

MemoryLeakDetectorNode* MemoryLeakDetectorTable::getNextLeak(MemoryLeakDetectorNode* leak, MemLeakPeriod period)
{
   int i = hash(leak->memory);
   MemoryLeakDetectorNode* node = table[i].getNextLeak(leak, period);
   if (node) return node;

   for (++i; i < hash_prime; i++) {
      node = table[i].getFirstLeak(period);
      if (node) return node;
   }
   return 0;
}

/////////////////////////////////////////////////////////////

MemoryLeakDetector::MemoryLeakDetector()
{
}

void MemoryLeakDetector::init(MemoryLeakFailure* report)
{
   doAllocationTypeChecking = true;
   current_period = mem_leak_period_disabled;
   reporter = report;
   output_buffer = SimpleBuffer();
   memoryTable = MemoryLeakDetectorTable();
}

void MemoryLeakDetector::clearAllAccounting(MemLeakPeriod period)
{
   memoryTable.clearAllAccounting(period);
}

void MemoryLeakDetector::startChecking()
{
   output_buffer.clear();
   current_period = mem_leak_period_checking;
}

void MemoryLeakDetector::stopChecking()
{
   current_period = mem_leak_period_enabled;
}

void MemoryLeakDetector::enable()
{
   current_period = mem_leak_period_enabled;
}

void MemoryLeakDetector::disable()
{
   current_period = mem_leak_period_disabled;
}

void MemoryLeakDetector::disableAllocationTypeChecking()
{
   doAllocationTypeChecking = false;
}

void MemoryLeakDetector::enableAllocationTypeChecking()
{
   doAllocationTypeChecking = true;
}

void MemoryLeakDetector::reportFailure(const char* message, const char* allocFile, int allocLine, size_t allocSize, MemLeakAllocType allocType, const char* freeFile, int freeLine, MemLeakAllocType freeType)
{
   output_buffer.add(message);
   output_buffer.add(MEM_LEAK_ALLOC_LOCATION, allocFile, allocLine, allocSize, getTypeString(allocType));
   output_buffer.add(MEM_LEAK_DEALLOC_LOCATION, freeFile, freeLine, getTypeString(freeType));
   reporter->fail(output_buffer.toString());
}

void* checkedMalloc(size_t size)
{
  void* mem = PlatformSpecificMalloc(size);
  if (mem == 0)
    FAIL("malloc returned nul pointer");
   return mem;
}

int calculateIntAlignedSize(size_t size)
{
	return (sizeof(int) - (size % sizeof(int))) + size;
}

MemoryLeakDetectorNode* MemoryLeakDetector::getNodeFromMemoryPointer(char* memory, size_t memory_size)
{
	return (MemoryLeakDetectorNode*) (memory + calculateIntAlignedSize(memory_size + memory_corruption_buffer_size));
}

char* MemoryLeakDetector::allocateMemoryAndExtraInfo(size_t size)
{
   return (char*) checkedMalloc(calculateIntAlignedSize(size + memory_corruption_buffer_size) + memory_corruption_buffer_size + sizeof(MemoryLeakDetectorNode));
}

char* MemoryLeakDetector::reallocateMemoryAndExtraInfo (char* memory, size_t size)
{
   return (char*) PlatformSpecificRealloc(memory, calculateIntAlignedSize(size + memory_corruption_buffer_size) + sizeof(MemoryLeakDetectorNode));
}


void MemoryLeakDetector::addMemoryCorruptionInformation(char* memory, size_t size)
{
   memory[size]   = 'B';
   memory[size+1] = 'A';
   memory[size+2] = 'S';
}

void MemoryLeakDetector::checkForAllocMismatchOrCorruption(MemoryLeakDetectorNode* node, const char* file, int line, MemLeakAllocType type)
{
   if (node->type != type && doAllocationTypeChecking)
      reportFailure(MEM_LEAK_ALLOC_DEALLOC_MISMATCH, node->file, node->line, node->size, node->type, file, line, type);
   else if (node->memory[node->size] != 'B' || node->memory[node->size+1] != 'A' || node->memory[node->size+2] != 'S' )
      reportFailure(MEM_LEAK_MEMORY_CORRUPTION, node->file, node->line, node->size, node->type, file, line, type);
}

void MemoryLeakDetector::addMemoryLeakInfoAndCorruptionInfo(char* memory, size_t size, char* file, int line, MemLeakAllocType type)
{
   addMemoryCorruptionInformation(memory, size);
   if (memory) {
      MemoryLeakDetectorNode* node = getNodeFromMemoryPointer(memory, size);
      MemoryLeakDetectorList::initNode(node, size, memory, current_period, file, line, type);
      memoryTable.addNewNode(node);
   }
}

bool MemoryLeakDetector::removeMemoryLeakInfoAndCheckCorruption(char* memory, const char* file, int line, MemLeakAllocType type)
{
   MemoryLeakDetectorNode* node = memoryTable.removeNode(memory);
   if (node) {
      checkForAllocMismatchOrCorruption(node, file, line, type);
      return true;
   }
   reportFailure(MEM_LEAK_DEALLOC_NON_ALLOCATED, "<unknown>", 0, 0, mem_leak_alloc_unknown, file, line, type);
   return false;
}

char* MemoryLeakDetector::alloc(size_t size, char* file, int line, MemLeakAllocType type)
{
   char* mem = allocateMemoryAndExtraInfo(size);
   addMemoryLeakInfoAndCorruptionInfo(mem, size, file, line, type);
   return mem;
}

void MemoryLeakDetector::dealloc(char* memory, const char* file, int line, MemLeakAllocType type)
{
   if (memory == 0) return;

   if (removeMemoryLeakInfoAndCheckCorruption(memory, file, line, type))
      PlatformSpecificFree(memory);
}

char* MemoryLeakDetector::reallocate(char* memory, size_t size, char* file, int line, MemLeakAllocType type)
{
   if (memory)
      removeMemoryLeakInfoAndCheckCorruption(memory, file, line, type);

   char* mem = reallocateMemoryAndExtraInfo (memory, size);
   addMemoryCorruptionInformation(mem, size);
   addMemoryLeakInfoAndCorruptionInfo(mem, size, file, line, type);
   return mem;
}

void MemoryLeakDetector::ConstructMemoryLeakReport(MemLeakPeriod period)
{
   MemoryLeakDetectorNode* leak = memoryTable.getFirstLeak(period);
   int total_leaks = 0;
   output_buffer.add(MEM_LEAK_HEADER);

   while (leak)
   {
      output_buffer.add(MEM_LEAK_LEAK, leak->size, leak->file, leak->line, getTypeString(leak->type), leak->memory);
      total_leaks++;
      leak = memoryTable.getNextLeak(leak, period);
   }
   output_buffer.add("%s %d", MEM_LEAK_FOOTER, total_leaks);
}

const char* MemoryLeakDetector::report(MemLeakPeriod period)
{
   if (!memoryTable.hasLeaks(period)) return MEM_LEAK_NONE;

   output_buffer.clear();
   ConstructMemoryLeakReport(period);

   return output_buffer.toString();
}

void MemoryLeakDetector::markCheckingPeriodLeaksAsNonCheckingPeriod()
{
   MemoryLeakDetectorNode* leak = memoryTable.getFirstLeak(mem_leak_period_checking);
   while (leak) {
      if (leak->period == mem_leak_period_checking) leak->period = mem_leak_period_enabled;
      leak = memoryTable.getNextLeak(leak, mem_leak_period_checking);
   }
  }

int MemoryLeakDetector::totalMemoryLeaks(MemLeakPeriod period)
{
   return memoryTable.getTotalLeaks(period);
}

char* MemoryLeakDetector::allocOperatorNew(size_t size)
{
   return alloc(size, UNKNOWN, 0, mem_leak_alloc_new);
}

char* MemoryLeakDetector::allocOperatorNew(size_t size, const char* file, int line)
{
   return alloc(size, const_cast<char*>(file), line, mem_leak_alloc_new);
}

char* MemoryLeakDetector::allocOperatorNewArray(size_t size)
{
   return alloc(size, UNKNOWN, 0, mem_leak_alloc_new_array);
}

char* MemoryLeakDetector::allocOperatorNewArray(size_t size, const char* file, int line)
{
   return alloc(size, const_cast<char*>(file), line, mem_leak_alloc_new_array);
}

char* MemoryLeakDetector::allocMalloc(size_t size)
{
   return alloc(size, UNKNOWN, 0, mem_leak_alloc_malloc);
}

char* MemoryLeakDetector::allocMalloc(size_t size, const char* file, int line)
{
   return alloc(size, const_cast<char*>(file), line, mem_leak_alloc_malloc);
}

char* MemoryLeakDetector::allocRealloc(char* memory, size_t size, const char* file, int line)
{
   return reallocate(memory, size, const_cast<char*>(file), line, mem_leak_alloc_malloc);
}

void MemoryLeakDetector::freeOperatorDeleteArray(char* memory)
{
   dealloc(memory, UNKNOWN, 0, mem_leak_alloc_new_array);
}

void MemoryLeakDetector::freeOperatorDelete(char* memory)
{
   dealloc(memory, UNKNOWN, 0, mem_leak_alloc_new);
}

void MemoryLeakDetector::freeFree(char* memory)
{
   dealloc(memory, UNKNOWN, 0, mem_leak_alloc_malloc);
}

void MemoryLeakDetector::freeFree(char* memory, const char* file, int line)
{
   dealloc(memory, file, line, mem_leak_alloc_malloc);
}

const char* MemoryLeakDetector::getTypeString(MemLeakAllocType type)
{
   switch (type) {
      case mem_leak_alloc_new: return "new";
      case mem_leak_alloc_new_array: return "new []";
      case mem_leak_alloc_malloc: return "malloc";
      case mem_leak_alloc_unknown: return "unknown";
   }
   return 0;
}
