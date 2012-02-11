#include "AllocLetTestFree.h"
#include <stdlib.h>
#include <memory.h>

typedef struct AllocLetTestFreeStruct
{
    int placeHolderForHiddenStructElements;
} AllocLetTestFreeStruct;

AllocLetTestFree AllocLetTestFree_Create(void)
{
     AllocLetTestFree self = calloc(1, sizeof(AllocLetTestFreeStruct));
     return self;
}

void AllocLetTestFree_Destroy(AllocLetTestFree self)
{
}


