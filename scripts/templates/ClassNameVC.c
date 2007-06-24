#include "ClassName.h"
#include <stdlib.h>
#include <memory.h>

static ClassName* ClassName_destroy(ClassName* self)
{
    //Add clean up code here
    free(self);
    return 0;
}

static int ClassName_returnIncrement(ClassName* self, int i)
{
    return i+1;
}

ClassName* ClassName_Create(int exampleInitializer)
{
    ClassName* self = malloc(sizeof(ClassName));
    memset(self, 0, sizeof(ClassName));

    //Public function initializers here 
    vBind(self, Destroy, ClassName_destroy);
    vBind(self, SomeFunction, ClassName_returnIncrement);

    //Member initializers
    self->data.exampleMember = exampleInitializer;
    
    return self;
}
