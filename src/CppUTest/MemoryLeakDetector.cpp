
#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define UNKNOWN const_cast<char*>("<unknown>")

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
   count = PlatformSpecificVSNprintf2(buffer + positions_filled, SIMPLE_BUFFER_LEN - positions_filled, format, arguments);
   if (count > 0)
      positions_filled += count;
   va_end(arguments);
}

char* SimpleBuffer::toString()
{
   return buffer;
}

///////////////////////

MemoryLeakDetectorNode* MemoryLeakDetectorList::allocNode(unsigned int size, char* memory, MemLeakPeriod period, char* file, int line, MemLeakAllocType type)
{
   MemoryLeakDetectorNode* node = (MemoryLeakDetectorNode*) malloc(sizeof(MemoryLeakDetectorNode));
   if (node) {
      node->size = size;
      node->memory = memory;
      node->period = period;
      node->file = file;
      node->line = line;
      node->type = type;
   }
   return node;
}

void MemoryLeakDetectorList::freeNode(MemoryLeakDetectorNode* node)
{
   free(node);
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
            freeNode(cur);
            cur = prev;
         }
         else {
            head = cur->next;
            freeNode (cur);
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
   return ((int) memory) % hash_prime;
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

MemoryLeakDetector::MemoryLeakDetector(MemoryLeakFailure* reporter)
: reporter(reporter), current_period(mem_leak_period_disabled), gone(false)
{
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

void MemoryLeakDetector::reportFailure(const char* message, const char* allocFile, int allocLine, int allocSize, MemLeakAllocType allocType, const char* freeFile, int freeLine, MemLeakAllocType freeType)
{
   output_buffer.add(message);
   output_buffer.add(MEM_LEAK_ALLOC_LOCATION, allocFile, allocLine, allocSize, getTypeString(allocType));
   output_buffer.add(MEM_LEAK_DEALLOC_LOCATION, freeFile, freeLine, getTypeString(freeType));
   reporter->fail(output_buffer.toString());
}

void MemoryLeakDetector::addMemoryCorruptionInformation(char* memory, int size)
{
   memory[size]   = 'B';
   memory[size+1] = 'A';
   memory[size+2] = 'S';
}

void MemoryLeakDetector::checkForAllocMismatchOrCorruption(MemoryLeakDetectorNode* node, const char* file, int line, MemLeakAllocType type)
{
   if (node->type != type)
      reportFailure(MEM_LEAK_ALLOC_DEALLOC_MISMATCH, node->file, node->line, node->size, node->type, file, line, type);
   else if (node->memory[node->size] != 'B' || node->memory[node->size+1] != 'A' || node->memory[node->size+2] != 'S' )
      reportFailure(MEM_LEAK_MEMORY_CORRUPTION, node->file, node->line, node->size, node->type, file, line, type);
}

void MemoryLeakDetector::addMemoryLeakInfoAndCorruptionInfo(char* memory, int size, char* file, int line, MemLeakAllocType type)
{
   addMemoryCorruptionInformation(memory, size);
   if (memory) {
      MemoryLeakDetectorNode* node = MemoryLeakDetectorList::allocNode(size, memory, current_period, file, line, type);
      memoryTable.addNewNode(node);
   }
}

bool MemoryLeakDetector::removeMemoryLeakInfoAndCheckCorruption(char* memory, const char* file, int line, MemLeakAllocType type)
{
   MemoryLeakDetectorNode* node = memoryTable.removeNode(memory);
   if (node) {
      checkForAllocMismatchOrCorruption(node, file, line, type);
      MemoryLeakDetectorList::freeNode(node);
      return true;
   }
   reportFailure(MEM_LEAK_DEALLOC_NON_ALLOCATED, "<unknown>", 0, 0, mem_leak_alloc_unknown, file, line, type);
   return false;
}

char* MemoryLeakDetector::alloc(unsigned int size, char* file, int line, MemLeakAllocType type)
{
   char* mem = (char*) malloc(size+3);
   addMemoryLeakInfoAndCorruptionInfo(mem, size, file, line, type);
   return mem;
}

void MemoryLeakDetector::dealloc(char* memory, const char* file, int line, MemLeakAllocType type)
{
   if (memory == 0) return;

   if (removeMemoryLeakInfoAndCheckCorruption(memory, file, line, type))
      free(memory);
}

char* MemoryLeakDetector::reallocate(char* memory, unsigned int size, char* file, int line, MemLeakAllocType type)
{
   if (memory)
      removeMemoryLeakInfoAndCheckCorruption(memory, file, line, type);

   char* mem = (char*) realloc(memory, size+3);
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

char* MemoryLeakDetector::allocOperatorNew(unsigned int size)
{
   return alloc(size, UNKNOWN, 0, mem_leak_alloc_new);
}

char* MemoryLeakDetector::allocOperatorNew(unsigned int size, const char* file, int line)
{
   return alloc(size, const_cast<char*>(file), line, mem_leak_alloc_new);
}

char* MemoryLeakDetector::allocOperatorNewArray(unsigned int size)
{
   return alloc(size, UNKNOWN, 0, mem_leak_alloc_new_array);
}

char* MemoryLeakDetector::allocOperatorNewArray(unsigned int size, const char* file, int line)
{
   return alloc(size, const_cast<char*>(file), line, mem_leak_alloc_new_array);
}

char* MemoryLeakDetector::allocMalloc(unsigned int size)
{
   return alloc(size, UNKNOWN, 0, mem_leak_alloc_malloc);
}

char* MemoryLeakDetector::allocMalloc(unsigned int size, const char* file, int line)
{
   return alloc(size, const_cast<char*>(file), line, mem_leak_alloc_malloc);
}

char* MemoryLeakDetector::allocRealloc(char* memory, unsigned int size, const char* file, int line)
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
