#include "ClassName.h"
#include <stdlib.h>
#include <memory.h>

//static local variables
typedef struct _ClassName
{
    int placeHolderForHiddenStructElements;
} _ClassName;

ClassName ClassName_Create(void)
{
     ClassName self = calloc(1, sizeof(_ClassName));
     return self;
}

void ClassName_Destroy(ClassName self)
{
    free(self);
}


