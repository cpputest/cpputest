
#ifndef D_MemoryLeakDetector_h
#define D_MemoryLeakDetector_h

#define MEM_LEAK_NONE "No memory leaks were detected."
#define MEM_LEAK_HEADER "Memory leak(s) found.\n"
#define MEM_LEAK_LEAK "Leak size: %d Allocated at: %s and line: %d. Type: \"%s\" Content: \"%.15s\"\n"
#define MEM_LEAK_FOOTER "Total number of leaks: "

#define MEM_LEAK_ALLOC_DEALLOC_MISMATCH "Allocation/deallocation type mismatch\n"
#define MEM_LEAK_MEMORY_CORRUPTION "Memory corruption (written out of bounds?)\n"
#define MEM_LEAK_ALLOC_LOCATION "   allocated at file: %s line: %d size: %d type: %s\n"
#define MEM_LEAK_DEALLOC_LOCATION "   dealllocated at file: %s line: %d type: %s\n"
#define MEM_LEAK_DEALLOC_NON_ALLOCATED "Deallocating non-allocated memory\n"

enum MemLeakPeriod { mem_leak_period_all, mem_leak_period_disabled, mem_leak_period_enabled, mem_leak_period_checking };
enum MemLeakAllocType {mem_leak_alloc_new, mem_leak_alloc_new_array, mem_leak_alloc_malloc, mem_leak_alloc_unknown};

#include <stdlib.h>
class MemoryLeakFailure
{
public:
   virtual ~MemoryLeakFailure() {};
   virtual void fail(char* fail_string)=0;
};

struct SimpleBuffer
{
   enum { SIMPLE_BUFFER_LEN = 4096};

   SimpleBuffer();
   void clear();
   void add(const char* format, ...);
   char* toString();

private:
   char buffer[SIMPLE_BUFFER_LEN];
   int positions_filled;
};

struct MemoryLeakDetectorNode
{
   MemoryLeakDetectorNode() : size(0), next(0) {};
   size_t size;
   char* memory;
   char* file;
   int line;
   MemLeakAllocType type;
   MemLeakPeriod period;

private:
   friend struct MemoryLeakDetectorList;
   MemoryLeakDetectorNode* next;
};

struct MemoryLeakDetectorList
{
   MemoryLeakDetectorList() : head(0) {};

   static void initNode(MemoryLeakDetectorNode* node, size_t size, char* memory, MemLeakPeriod period, char* file, int line, MemLeakAllocType type);

   void addNewNode(MemoryLeakDetectorNode* node);
   MemoryLeakDetectorNode* removeNode(char* memory);

   MemoryLeakDetectorNode* getFirstLeak(MemLeakPeriod period);
   MemoryLeakDetectorNode* getNextLeak(MemoryLeakDetectorNode* node, MemLeakPeriod period);
   MemoryLeakDetectorNode* getLeakFrom(MemoryLeakDetectorNode* node, MemLeakPeriod period);

   int getTotalLeaks(MemLeakPeriod period);
   bool hasLeaks(MemLeakPeriod period);
   void clearAllAccounting(MemLeakPeriod period);

   bool isInPeriod(MemoryLeakDetectorNode* node, MemLeakPeriod period);

private:
   MemoryLeakDetectorNode* head;
};

struct MemoryLeakDetectorTable
{
   void clearAllAccounting(MemLeakPeriod period);

   void addNewNode(MemoryLeakDetectorNode* node);
   MemoryLeakDetectorNode* removeNode(char* memory);

   bool hasLeaks(MemLeakPeriod period);
   int getTotalLeaks(MemLeakPeriod period);

   MemoryLeakDetectorNode* getFirstLeak(MemLeakPeriod period);
   MemoryLeakDetectorNode* getNextLeak(MemoryLeakDetectorNode* leak, MemLeakPeriod period);

private:
   int hash(char* memory);

   enum {hash_prime = MEMORY_LEAK_HASH_TABLE_SIZE};
   MemoryLeakDetectorList table[hash_prime];
};

class MemoryLeakDetector
{
public:
   MemoryLeakDetector();
	~MemoryLeakDetector() {}

	void init(MemoryLeakFailure* reporter);

   void enable();
   void disable();

   void disableAllocationTypeChecking();
   void enableAllocationTypeChecking();

   void startChecking();
   void stopChecking();

   const char* report(MemLeakPeriod period);
   void markCheckingPeriodLeaksAsNonCheckingPeriod();
   int totalMemoryLeaks(MemLeakPeriod period);
   void clearAllAccounting(MemLeakPeriod period);

   char* allocOperatorNew(size_t size);
   char* allocOperatorNew(size_t size, const char* file, int line);
   char* allocOperatorNewArray(size_t size);
   char* allocOperatorNewArray(size_t size, const char* file, int line);
   char* allocMalloc(size_t size);
   char* allocMalloc(size_t size, const char* file, int line);
   char* allocRealloc(char* memory, size_t size, const char* file, int line);

   void freeOperatorDelete(char* memory);
   void freeOperatorDeleteArray(char* memory);
   void freeFree(char* memory);
   void freeFree(char* memory, const char* file, int line);

   enum { memory_corruption_buffer_size = 3};
private:
   MemoryLeakFailure* reporter;
   MemLeakPeriod current_period;
   SimpleBuffer output_buffer;
   MemoryLeakDetectorTable memoryTable;
   bool doAllocationTypeChecking;


   void ConstructMemoryLeakReport(MemLeakPeriod period);
   void reportFailure(const char* message, const char* allocFile, int allocLine, size_t allocSize, MemLeakAllocType allocType, const char* freeFile, int freeLine, MemLeakAllocType freeType);
   char* alloc(size_t size, char* file, int line, MemLeakAllocType type);
   char* reallocate(char* memory, size_t size, char* file, int line, MemLeakAllocType type);
   const char* getTypeString(MemLeakAllocType type);
   void dealloc(char* memory, const char* file, int line, MemLeakAllocType type);

   char* reallocateMemoryAndExtraInfo(char* memory, size_t size);
   char* allocateMemoryAndExtraInfo(size_t size);
   void addMemoryCorruptionInformation(char* memory, size_t size);
   void checkForAllocMismatchOrCorruption(MemoryLeakDetectorNode* node, const char* file, int line, MemLeakAllocType type);

   void addMemoryLeakInfoAndCorruptionInfo(char* memory, size_t size, char* file, int line, MemLeakAllocType type);
   bool removeMemoryLeakInfoAndCheckCorruption(char* memory, const char* file, int line, MemLeakAllocType type);

   MemoryLeakDetectorNode* getNodeFromMemoryPointer(char* memory, size_t size);
};

#endif
