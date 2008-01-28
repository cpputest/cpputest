#include "ClassName.h"
#include <stdlib.h>
#include <memory.h>

//static local variables


ClassName* ClassName_create(void)
{
     ClassName* self = malloc(sizeof(ClassName));
     memset(self, 0, sizeof(ClassName));
     //do member initialization here as needed
     return self;
}

void ClassName_destroy(ClassName* self)
{
    free(self);
}


