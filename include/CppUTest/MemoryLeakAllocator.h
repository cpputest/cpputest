#ifndef D_MemoryLeakAllocator_h
#define D_MemoryLeakAllocator_h

struct MemoryLeakNode;

class MemoryLeakAllocator
{
public:
	virtual char* alloc_memory(size_t size, const char* file, int line)=0;
	virtual void free_memory(char* memory, const char* file, int line)=0;

	virtual const char* name()=0;
	virtual const char* alloc_name()=0;
	virtual const char* free_name()=0;

	virtual bool isOfEqualType(MemoryLeakAllocator* allocator);
	virtual ~MemoryLeakAllocator()
	{
	}

	virtual bool allocateMemoryLeakNodeSeparately();
	virtual char* allocMemoryLeakNode(size_t size);
	virtual void freeMemoryLeakNode(char* memory);

	static void setCurrentNewAllocator(MemoryLeakAllocator* allocator);
	static MemoryLeakAllocator* getCurrentNewAllocator();
	static void setCurrentNewAllocatorToDefault();

	static void setCurrentNewArrayAllocator(MemoryLeakAllocator* allocator);
	static MemoryLeakAllocator* getCurrentNewArrayAllocator();
	static void setCurrentNewArrayAllocatorToDefault();

	static void setCurrentMallocAllocator(MemoryLeakAllocator* allocator);
	static MemoryLeakAllocator* getCurrentMallocAllocator();
	static void setCurrentMallocAllocatorToDefault();

private:
	static MemoryLeakAllocator* currentNewAllocator;
	static MemoryLeakAllocator* currentNewArrayAllocator;
	static MemoryLeakAllocator* currentMallocAllocator;
};

class StandardMallocAllocator: public MemoryLeakAllocator
{
public:
	virtual char* alloc_memory(size_t size, const char* file, int line);
	virtual void free_memory(char* memory, const char* file, int line);

	const char* name();
	const char* alloc_name();
	const char* free_name();

	virtual bool allocateMemoryLeakNodeSeparately();

	static MemoryLeakAllocator* defaultAllocator();
};

class StandardNewAllocator: public MemoryLeakAllocator
{
public:
	virtual char* alloc_memory(size_t size, const char* file, int line);
	virtual void free_memory(char* memory, const char* file, int line);

	const char* name();
	const char* alloc_name();
	const char* free_name();

	static MemoryLeakAllocator* defaultAllocator();
};

class StandardNewArrayAllocator: public MemoryLeakAllocator
{
public:
	virtual char* alloc_memory(size_t size, const char* file, int line);
	virtual void free_memory(char* memory, const char* file, int line);

	const char* name();
	const char* alloc_name();
	const char* free_name();

	static MemoryLeakAllocator* defaultAllocator();
};

class NullUnknownAllocator: public MemoryLeakAllocator
{
public:
	virtual char* alloc_memory(size_t size, const char* file, int line);
	virtual void free_memory(char* memory, const char* file, int line);

	const char* name();
	const char* alloc_name();
	const char* free_name();

	static MemoryLeakAllocator* defaultAllocator();
};

#endif
