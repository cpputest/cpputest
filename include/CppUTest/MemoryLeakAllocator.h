#ifndef D_MemoryLeakAllocator_h
#define D_MemoryLeakAllocator_h

class MemoryLeakAllocator
{
public:
	virtual char* alloc_memory(size_t size)=0;
	virtual void free_memory(char* memory)=0;

	virtual const char* name()=0;
	virtual const char* alloc_name()=0;
	virtual const char* free_name()=0;

	virtual int isOfEqualType(MemoryLeakAllocator* allocator);
	virtual ~MemoryLeakAllocator()
	{
	}
	;

	static void setCurrentNewAllocator(MemoryLeakAllocator* allocator);
	static MemoryLeakAllocator* getCurrentNewAllocator();
	static void setCurrentNewAllocatorToDefault();

	static void setCurrentNewArrayAllocator(MemoryLeakAllocator* allocator);
	static MemoryLeakAllocator* getCurrentNewArrayAllocator();
	static void setCurrentNewArrayAllocatorToDefault();

private:
	static MemoryLeakAllocator* currentNewAllocator;
	static MemoryLeakAllocator* currentNewArrayAllocator;
};

class StandardMallocAllocator: public MemoryLeakAllocator
{
public:
	char* alloc_memory(size_t size);
	void free_memory(char* memory);

	const char* name();
	const char* alloc_name();
	const char* free_name();

	static MemoryLeakAllocator* defaultAllocator();
};

class StandardNewAllocator: public MemoryLeakAllocator
{
public:
	char* alloc_memory(size_t size);
	void free_memory(char* memory);

	const char* name();
	const char* alloc_name();
	const char* free_name();

	static MemoryLeakAllocator* defaultAllocator();
};

class StandardNewArrayAllocator: public MemoryLeakAllocator
{
public:
	char* alloc_memory(size_t size);
	void free_memory(char* memory);

	const char* name();
	const char* alloc_name();
	const char* free_name();

	static MemoryLeakAllocator* defaultAllocator();
};

class NullUnknownAllocator: public MemoryLeakAllocator
{
public:
	char* alloc_memory(size_t size);
	void free_memory(char* memory);

	const char* name();
	const char* alloc_name();
	const char* free_name();

	static MemoryLeakAllocator* defaultAllocator();
};

#endif
