
#ifndef D_AllocLetTestFree_H
#define D_AllocLetTestFree_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct AllocLetTestFreeStruct * AllocLetTestFree;

AllocLetTestFree AllocLetTestFree_Create(void);
void AllocLetTestFree_Destroy(AllocLetTestFree);

#ifdef __cplusplus
}
#endif

#endif  /* D_FakeAllocLetTestFree_H */
