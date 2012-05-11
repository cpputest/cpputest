#ifndef ALLOCATIONINCFILE_H
#define ALLOCATIONINCFILE_H

extern char* mallocAllocation(void);
extern void freeAllocation(void* memory);
extern void freeAllocationWithoutMacro(void* memory);

#endif
