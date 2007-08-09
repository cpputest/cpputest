#ifndef D_ClassName_H
#define D_ClassName_H

#include "CppUTest/VirtualCall.h"

///////////////////////////////////////////////////////////////////////////////
//
//  ClassName is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

struct ClassNamePrivateData
  {
    int exampleMember;
  };

typedef struct ClassNameTag
  {
    struct ClassNameTag* (*Destroy)(struct ClassNameTag* self);

    //Public override able methods
    int (*SomeFunction)(struct ClassNameTag* self, int);

    //void (*SomeOtherFunction)(struct ClassNameTag* self, int, char*);

    struct ClassNamePrivateData data;

  }
ClassName;


ClassName* ClassName_Create(int exampleInitializer);

#endif  // D_ClassName_H
