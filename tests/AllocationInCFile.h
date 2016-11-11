#ifndef ALLOCATIONINCFILE_H
#define ALLOCATIONINCFILE_H

#ifdef __cplusplus
extern "C" {
#endif

extern char* mallocAllocation(void);
extern char* strdupAllocation(void);
extern char* strndupAllocation(void);
extern void freeAllocation(void* memory);
extern void freeAllocationWithoutMacro(void* memory);

#ifdef __cplusplus
}
#endif

#endif
