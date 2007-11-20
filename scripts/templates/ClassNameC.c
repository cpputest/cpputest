#include "ClassName.h"
#include <stdlib.h>
#include <memory.h>

struct ClassName
{
  int define_implementation_data_here;
};

struct ClassName* ClassName_create(void)
{
    struct ClassName* aClassName = malloc(sizeof(struct ClassName));
    memset(aClassName, 0, sizeof(struct ClassName));
    //do member initialization here as needed
    return aClassName;
}

void ClassName_destroy(struct ClassName* aClassName)
{
    free(aClassName);
}


