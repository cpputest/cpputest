#include "ClassName.h"
#include <stdlib.h>
#include <memory.h>

//static local variables

void ClassName_VirtualFunction_impl(ClassName* self)
{
    //this is just an example
}


ClassName* ClassName_Create(void)
{
     ClassName* self = malloc(sizeof(ClassName));
     memset(self, 0, sizeof(ClassName));
     self->virtualFunction_renameThis = ClassName_VirtualFunction_impl;
     return self;
}

void ClassName_Destroy(ClassName* self)
{
    free(self);
}


