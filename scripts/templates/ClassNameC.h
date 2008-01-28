#ifndef D_ClassName_H
#define D_ClassName_H

///////////////////////////////////////////////////////////////////////////////
//
//  ClassName is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    int placeHolder;
} ClassName;

ClassName* ClassName_create(void);
void ClassName_destroy(ClassName*);

#endif  // D_ClassName_H
