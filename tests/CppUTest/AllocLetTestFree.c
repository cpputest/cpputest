
#include "CppUTest/StandardCLibrary.h"

#if CPPUTEST_USE_STD_C_LIB

#include "AllocLetTestFree.h"

typedef struct AllocLetTestFreeStruct
{
    int placeHolderForHiddenStructElements;
} AllocLetTestFreeStruct;

AllocLetTestFree AllocLetTestFree_Create(void)
{
    size_t count = 1;
    AllocLetTestFree self = (AllocLetTestFree)calloc(count, sizeof(AllocLetTestFreeStruct));
    return self;
}

void AllocLetTestFree_Destroy(AllocLetTestFree self)
{
    AllocLetTestFree no_use = self;
    self = NULL;
    self = no_use;
    (void)self;

}
#endif
